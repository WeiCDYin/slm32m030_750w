#include "rec.h"
#include "rec_storage.h"
#include <stdbool.h>
/* Wave recorder. See rec.h for the contract and for the split with the host. Nothing below
 * knows what any channel MEANS -- it copies addresses and watches one number. */

/* One bound address, loaded at its own width. The cast is what the width and the signedness are
 * for: the target preserves BITS and the host reinterprets them. */
static int32_t rec_load_channel(const void *addr, uint8_t type) {
    switch(type) {
    case REC_I8:  return (int32_t)(*(const volatile int8_t   *)addr);
    case REC_U8:  return (int32_t)(*(const volatile uint8_t  *)addr);
    case REC_I16: return (int32_t)(*(const volatile int16_t  *)addr);
    case REC_U16: return (int32_t)(*(const volatile uint16_t *)addr);
    case REC_U32: return (int32_t)(*(const volatile uint32_t *)addr);
    default:      return (int32_t)(*(const volatile int32_t  *)addr);
    }
}

/* How many int16s a channel costs a record. Packing the 8-bit ones into half an int16 would buy
 * a quarter of a channel and cost the reader an alignment rule. */
static uint8_t rec_n_int16(uint8_t type) {
    return (type == REC_I32 || type == REC_U32) ? 2u : 1u;
}

/* One channel out of one record. Low half first, high second -- the order the host reads back. */
static int32_t row_get(const rec_t *r, const int16_t *row, uint8_t ch) {
    const int16_t *at = &row[r->offset[ch]];
    if(rec_n_int16(r->ch[ch].type) == 2u)
        return (int32_t)((uint32_t)(uint16_t)at[0] | ((uint32_t)(uint16_t)at[1] << 16));
    return at[0];
}

/* A stored value as a SORT KEY, so one set of comparisons serves both signednesses.
 *
 * The buffer holds bit patterns, and row_get hands them back sign-extended from their int16. Compare
 * those as int32 and an unsigned channel breaks at its top bit: a Q32 angle past 180 degrees reads
 * NEGATIVE, so a rising trigger on raw phase waits for a crossing that already happened. Comparing
 * as uint32 breaks the signed channels instead.
 *
 * Both orderings are the same ordering shifted, so map each into uint32 once and compare there:
 * unsigned keeps its value (masked to the width actually stored), signed flips its top bit,
 * which maps INT32_MIN..INT32_MAX onto 0..UINT32_MAX monotonically. */
static uint32_t trig_key(uint8_t type, int32_t v) {
    switch(type) {
    case REC_U8: case REC_U16: return (uint32_t)(uint16_t)v;   /* one int16, zero-extended */
    case REC_U32:              return (uint32_t)v;
    default:                   return (uint32_t)v ^ 0x80000000u;
    }
}

/* Edge detector, with the hysteresis a bare `prev < lvl && cur >= lvl` needs.
 *
 * A q15 current dithers by tens of LSB, so a bare crossing test fires on noise -- repeatedly, and
 * at a moment that has nothing to do with the event. The scope-standard fix is a PRIMED flag: the
 * signal must first leave the band on the WRONG side, and only then does a crossing count. primed
 * doubles as WHICH side it left on (1 = below, 2 = above), which is all EITHER_EDGE needs.
 *
 * hyst = 0 degenerates to a plain threshold, which is what a stage/state channel wants -- those
 * do not dither, and priming on `v < lvl` is exactly "it was not there yet".
 *
 * The band saturates rather than wrapping: at the ends of the range there is simply no room to
 * leave on that side, and a wrapped bound would prime on the wrong half of the range. */
static bool trig_fired(rec_t *r, int32_t v) {
    const uint8_t  type = r->ch[r->trig_ch].type;
    const uint32_t x    = trig_key(type, v);
    const uint32_t lvl  = trig_key(type, r->trig_lvl);
    const uint32_t band = (uint32_t)(r->trig_hyst < 0 ? -r->trig_hyst : r->trig_hyst);
    const uint32_t lo   = (lvl > band) ? lvl - band : 0u;
    const uint32_t hi   = (0xFFFFFFFFu - band > lvl) ? lvl + band : 0xFFFFFFFFu;

    switch(r->trig_edge) {
    case NO_EDGE:
        return true;                                  /* "start now": a plain post-trigger log */
    case RISING_EDGE:
        if(!r->primed) { r->primed = (uint8_t)(x < lo); return false; }
        if(x >= lvl)   { r->primed = 0; return true; }
        return false;
    case FALLING_EDGE:
        if(!r->primed) { r->primed = (uint8_t)(x > hi) << 1; return false; }
        if(x <= lvl)   { r->primed = 0; return true; }
        return false;
    default:                                          /* EITHER_EDGE */
        if(!r->primed) {
            if(x < lo)      r->primed = 1;
            else if(x > hi) r->primed = 2;
            return false;
        }
        if((r->primed == 1 && x >= lvl) || (r->primed == 2 && x <= lvl)) {
            r->primed = 0;
            return true;
        }
        return false;
    }
}

void rec_init(rec_t *r, int16_t *buf, uint16_t cap, uint32_t fs_hz) {
    if(!r) return;
    r->pub.buf   = buf;
    r->pub.cap   = buf ? cap : 0u;
    r->pub.fs_hz = fs_hz;
    for(uint8_t i = 0; i < REC_MAX_CH; i++) {
        r->ch[i].addr = 0;
        r->ch[i].type = REC_I16;
    }
    r->pub.num_ch_used = 0;  r->trig_ch = 0;  r->trig_edge = NO_EDGE;
    r->trig_lvl    = 0;  r->trig_hyst    = 0;  r->pub.decim     = 1;  r->pre_trig_pct = 0;
    r->pub.state = REC_IDLE;  r->pub.err = REC_OK;  r->pub.depth = 0;  r->pub.n_records_pre_trig = 0;
    r->pub.stride = 0;
    r->cmd.op = REC_OP_NONE;  r->cmd.err = REC_OK;
    for(uint8_t i = 0; i < REC_MAX_CH; i++) r->offset[i] = 0;
    r->wr    = 0;  r->pub.n_records_taken = 0;  r->pub.trig_idx = 0;
    r->n_records_left = 0;  r->decim_cnt = 0;  r->primed = 0;
}

/* The setters. Thin on purpose: each one writes the field the HOST would poke by name, so the two
 * configuration paths stay the same path and rec_arm remains the only validator. What they buy is
 * a legal ORDER a caller can read off the header, and num_ch_used that keeps itself. */
void rec_bind_ch(rec_t *r, uint8_t ch, const void *addr, rec_type_t type) {
    if(!r || ch >= REC_MAX_CH) return;
    r->ch[ch].addr = addr;
    r->ch[ch].type = (uint8_t)type;
    /* The table is dense: binding the top channel claims the ones under it, and a hole is then
     * rec_arm's REC_ERR_UNBOUND. */
    if(ch >= r->pub.num_ch_used) r->pub.num_ch_used = (uint8_t)(ch + 1u);
}

void rec_unbind_all(rec_t *r) {
    if(!r) return;
    for(uint8_t i = 0; i < REC_MAX_CH; i++) r->ch[i].addr = 0;
    r->pub.num_ch_used = 0;
}

void rec_set_trig(rec_t *r, uint8_t ch, rec_trig_edge_t edge, int32_t lvl, int32_t hyst) {
    if(!r) return;
    r->trig_ch   = ch;
    r->trig_edge = (uint8_t)edge;
    r->trig_lvl  = lvl;
    r->trig_hyst = hyst;
}

void rec_set_pre_trig(rec_t *r, uint8_t pct) {
    if(r) r->pre_trig_pct = pct;
}

void rec_set_decim(rec_t *r, uint16_t n) {
    if(r) r->pub.decim = n;
}

/* Arm. Cold path -- it divides twice, which is exactly why it is not in rec_step.
 *
 * DEPTH IS DERIVED, NOT CONFIGURED: the buffer is a fixed byte budget and the bound channel count
 * spends it, so binding two channels buys four times the window eight do, with no rebuild and
 * nothing for the operator to size.
 *
 * The write ORDER is the whole concurrency story, and it is the poke doorbell's: drop to IDLE
 * first so the ISR stops touching the state, rewrite the config, and let the store to `state` be
 * the commit. The ISR only ever advances ARMED -> TRIGGERED -> DONE and never writes config, so
 * the two sides own disjoint fields and no critical section is needed. */
static rec_err_t refuse(rec_t *r, rec_err_t why) {
    r->pub.err = (uint8_t)why;
    return why;
}

rec_err_t rec_arm(rec_t *r) {
    if(!r) return REC_ERR_NO_BUF;
    if(!r->pub.buf) return refuse(r, REC_ERR_NO_BUF);
    r->pub.state = REC_IDLE;

    const uint8_t num_ch_used = r->pub.num_ch_used;
    if(num_ch_used == 0u)         return refuse(r, REC_ERR_NO_CHAN);
    if(num_ch_used > REC_MAX_CH) return refuse(r, REC_ERR_TOO_MANY_CH);
    for(uint8_t i = 0; i < num_ch_used; i++) {
        if(!r->ch[i].addr) return refuse(r, REC_ERR_UNBOUND);
                                                     /* a hole in the table is a host bug, not a
                                                      * channel to quietly skip: the channel
                                                      * indices ARE the host's column order */
    }
    if(r->trig_ch >= num_ch_used) return refuse(r, REC_ERR_TRIG_CH);

    /* The record layout, derived once so the ISR only adds a precomputed offset. */
    uint8_t stride = 0;
    for(uint8_t i = 0; i < num_ch_used; i++) {
        r->offset[i] = stride;
        stride = (uint8_t)(stride + rec_n_int16(r->ch[i].type));
    }
    r->pub.stride = stride;

    const uint16_t depth = (uint16_t)(r->pub.cap / stride);
    if(depth < 2u) return refuse(r, REC_ERR_DEPTH);  /* nowhere to put a post-trigger window */

    uint8_t pct = r->pre_trig_pct;
    if(pct > 100u) pct = 100u;
    uint16_t pre = (uint16_t)(((uint32_t)depth * pct) / 100u);
    if(pre > (uint16_t)(depth - 1u)) pre = (uint16_t)(depth - 1u);   /* the trigger sample itself
                                                                      * needs a record */
    r->pub.depth = depth;
    r->pub.n_records_pre_trig   = pre;
    r->wr    = 0;  r->pub.n_records_taken = 0;  r->pub.trig_idx = 0;
    r->n_records_left = 0;  r->decim_cnt = 0;  r->primed = 0;
    r->pub.err   = REC_OK;
    r->pub.state = REC_ARMED;                            /* the commit, written LAST */
    return REC_OK;
}

/* Abandon the capture. IDLE is what stops the ISR touching anything, and it is the same field the
 * host drops to zero when it clears a session -- one verb for what both sides already do. */
void rec_stop(rec_t *r) {
    if(r) r->pub.state = REC_IDLE;
}

/* One step. The port calls this at the END of its ISR, so a record holds the currents that went
 * into that tick and the duties that came out of it -- but that is the port's arrangement, not
 * something this file knows or needs. */
void rec_step(rec_t *r) {
    const uint8_t st = r->pub.state;
    if(st != REC_ARMED && st != REC_TRIGGERED) 
        return;
    rec_pub_t *p = &r->pub;
    const uint16_t dec = r->decim_cnt + 1u;            /* decim 0 and 1 both mean every call */
    if(dec < p->decim) {
        r->decim_cnt = dec;
        return;
    }
    r->decim_cnt = 0;

    const uint8_t  num_ch_used = p->num_ch_used;
    const uint16_t cur = r->wr;
    int16_t *rec = &p->buf[(uint32_t)cur * p->stride];
    for(uint8_t i = 0; i < num_ch_used; i++) {
        const int32_t v = rec_load_channel(r->ch[i].addr, r->ch[i].type);
        int16_t *at = &rec[r->offset[i]];
        at[0] = (int16_t)v;                          /* low half, and all of a narrow channel */
        if(rec_n_int16(r->ch[i].type) == 2u) {
            at[1] = (int16_t)((uint32_t)v >> 16);
        }
    }

    const uint16_t nxt = (uint16_t)(cur + 1u);
    r->wr = (nxt >= p->depth) ? 0u : nxt;            /* compare, not a modulo: depth is derived
                                                  * from stride and is not a power of two */
    if(p->n_records_taken < p->depth)
        p->n_records_taken = (uint16_t)(p->n_records_taken + 1u);

    if(st == REC_ARMED) {
        /* The pre-trigger window must be REAL data. n_records_taken counts the sample just
         * written, so the comparison says that many records already sit behind it; without the
         * guard an early edge hands back a window whose front half is whatever the buffer held
         * before the arm -- and nothing about the returned trace would say so. */
        if(p->n_records_taken > p->n_records_pre_trig
           && trig_fired(r, row_get(r, rec, r->trig_ch))) {
            p->trig_idx = cur;                           /* t = 0 is the sample that fired */
            r->n_records_left = (uint16_t)(p->depth - p->n_records_pre_trig - 1u);
            p->state = r->n_records_left ? REC_TRIGGERED : REC_DONE;  /* pre 100% -> already full */
        }
    } else {
        r->n_records_left = (uint16_t)(r->n_records_left - 1u);
        if(r->n_records_left == 0u) p->state = REC_DONE;
    }
}

/* The window is a ring: it starts n_records_pre_trig BEHIND the trigger and wraps. That and the
 * record layout are here and nowhere else on this side, so a reader walks 0..n_records_taken-1
 * and gets time order, with t = 0 at i == n_records_pre_trig. */
int32_t rec_get_sample(const rec_t *r, uint16_t idx, uint8_t ch) {
    if(!r) return 0;
    const rec_pub_t *p = &r->pub;
    if(!p->buf || p->depth == 0u || idx >= p->n_records_taken || ch >= p->num_ch_used) return 0;
    const uint16_t first = (uint16_t)((p->trig_idx + p->depth - p->n_records_pre_trig) % p->depth);
    const uint16_t rec   = (uint16_t)((first + idx) % p->depth);
    return row_get(r, &p->buf[(uint32_t)rec * p->stride], ch);
}

/* The inbox, drained one op per call. Every case is a marshalled API call and nothing more: the
 * rules -- num_ch_used bookkeeping, the clamps, the arm ordering -- stay in the functions, so a
 * client that can only write memory gets the same behaviour as one that can call.
 *
 * op is cleared LAST, after err is published, because that zero is the handshake: a sender that
 * sees REC_OP_NONE must be able to trust the err beside it. */
void rec_run(rec_t *r) {
    if(!r) return;
    const uint8_t op = r->cmd.op;
    if(op == REC_OP_NONE) return;

    rec_err_t e = REC_OK;
    switch(op) {
    case REC_OP_BIND_CH:
        rec_bind_ch(r, r->cmd.ch, r->cmd.addr, (rec_type_t)r->cmd.type);
        break;
    case REC_OP_UNBIND_ALL:   rec_unbind_all(r);                        break;
    case REC_OP_SET_TRIG:
        rec_set_trig(r, r->cmd.ch, (rec_trig_edge_t)r->cmd.edge, r->cmd.lvl, r->cmd.hyst);
        break;
    case REC_OP_SET_PRE_TRIG: rec_set_pre_trig(r, r->cmd.pct);          break;
    case REC_OP_SET_DECIM:    rec_set_decim(r, r->cmd.decim);               break;
    case REC_OP_ARM:          e = rec_arm(r);                           break;
    case REC_OP_STOP:         rec_stop(r);                              break;
    default:                  e = REC_ERR_BAD_OP;                       break;
    }
    r->cmd.err = (uint8_t)e;
    r->cmd.op  = REC_OP_NONE;
}

/* The published view. A pointer, not a copy: it is volatile and the ISR moves it under the reader,
 * which is exactly what a reader polling for REC_DONE wants. */
const rec_pub_t *rec_get_pub(const rec_t *r) {
    return r ? &r->pub : 0;
}
