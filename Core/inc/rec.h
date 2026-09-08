#ifndef MC_REC_H
#define MC_REC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* A wave recorder that runs at the end of ISR. Host can specify what variables to record without re-flashing firmware.
 *
 * THE WORDS THIS FILE USES. They are used in exactly these senses, here and in tools/wave.py:
 *
 *   channel   one bound variable: an address and how to read it. 0..num_ch_used-1.
 *   sample    ONE channel's value at ONE instant. What rec_get_sample returns.
 *   record    EVERY bound channel sampled at the same instant. One per recorded rec_step call,
 *             so the counters -- depth, n_records_taken, n_records_pre_trig, n_records_left --
 *             all count records, never samples.
 *   int16     the storage unit. A record is `stride` int16s; a channel spends one, or two if its
 *             source is 32 bits (low half first, then high). offset[ch] is how many int16s into
 *             a record that channel starts.
 *   depth     records the buffer holds at this binding: cap/stride.
 *   window    the records kept around the trigger, which is the whole buffer:
 *             n_records_pre_trig + the trigger record + n_records_left == depth.
 *
 * So the capture is a table: records are rows, channels are columns, and rec_get_sample(r, idx,
 * ch) reads one entry of it.
 *
 * HOW THE BUFFER IS LAID OUT. Records run end to end, and within a record the channels run in
 * BIND ORDER -- which is why channel order is the reader's column order. Bind ia (i16), stage
 * (u8) and a Q32 angle, and one record costs 4 int16s:
 *
 *      offset[]      0      1       2       3
 *                 +------+-------+-------+-------+------+-------+-------+-------+
 *      buf ->     |  ia  | stage | th_lo | th_hi |  ia  | stage | th_lo | th_hi | ...
 *                 +------+-------+-------+-------+------+-------+-------+-------+
 *                 \________ record k ___________/\_______ record k+1 __________/
 *                          stride = 4 int16s
 *
 *   buf[record * stride + offset[ch]]      one sample; the second int16 follows if 32-bit
 *   depth = cap / stride                   how many records fit -- 4096/4 = 1024 here
 *
 * A 32-bit channel is stored LOW HALF FIRST, so the pair reads back as a little-endian int32 and
 * no record ever has to be 4-byte aligned. That is also why width costs WINDOW rather than bits:
 * the byte budget is fixed, so a wider record simply means fewer of them.
 *
 * The buffer is a RING. Records are written in order and wrap at depth, so record 0 in memory is
 * not the oldest -- see READOUT below, and let rec_get_sample do that arithmetic.
 *
 * READOUT is a plain memory read of buf: at REC_DONE nothing is writing it, so there is no
 * tearing and no need for probe.h's absolute-index chase. The window is a ring, so it does not
 * start at record 0 -- it starts n_records_pre_trig records behind the trigger and wraps:
 *     first = (trig_idx + depth - n_records_pre_trig) % depth,   then n records forward
 *     t = (i - n_records_pre_trig) * decim / fs_hz
 * and t = 0 lands on the sample that fired. rec_get_sample() IS that arithmetic; a C reader should
 * call it rather than repeat it. The host cannot call in, so tools/wave.py mirrors it -- this
 * file is the definition the mirror answers to.
 *
 * USING IT from C. A caller that can execute code calls these; a caller that can only move
 * memory -- the host over SWD -- writes the command frame below and rec_run makes the same
 * calls on its behalf. Either way the sequence is this one:
 *
 *     rec_init(&g_rec, buf, N, PWM_FREQ_HZ);                // once, at bring-up: storage + call rate
 *     rec_bind_ch(&g_rec, 0, &g_in.iabc_meas.a, REC_I16);   // channel order IS the reader's
 *     rec_bind_ch(&g_rec, 1, &g_mc.if_foc_stage, REC_U8);   // column order
 *     rec_set_trig(&g_rec, 1, RISING_EDGE, 3, 0);           // fire when channel 1 crosses 3 upward
 *     rec_set_pre_trig(&g_rec, 25);                         // a quarter of the window before t = 0
 *     rec_set_decim(&g_rec, 1);                             // record on every step
 *     if(rec_arm(&g_rec) != REC_OK) { ... }                 // pub.err says WHICH check refused
 *     ...cause the event; rec_step() records as the port calls it...
 *     while(rec_get_pub(&g_rec)->state != REC_DONE) { }
 *     for(uint16_t i = 0; i < rec_get_pub(&g_rec)->n_records_taken; i++)   // oldest first, so
 *         use(rec_get_sample(&g_rec, i, 0));                  // t = 0 is at n_records_pre_trig
 *
 * rec_arm IS THE GATE. It re-validates and clamps whatever it finds rather than trusting the
 * setters, because a debugger can still write these fields behind the module's back -- so a bad
 * config is caught in one place, whoever wrote it. */

#define REC_MAX_CH 8    /* maximum number of channels */

/* How to read a bound address: the width the LOAD must use, and whether it sign-extends. What
 * reaches the buffer is a bit pattern the host reinterprets. The 8-bit widths are not for
 * completeness -- if_foc_stage and the supervisor's state are one byte each, and they are the
 * channels a bench capture most often triggers ON. */
typedef enum { REC_I8 = 0, REC_U8, REC_I16, REC_U16, REC_I32, REC_U32 } rec_type_t;

typedef enum { REC_IDLE = 0, REC_ARMED, REC_TRIGGERED, REC_DONE } rec_state_t;

/* Trigger edge, NO_EDGE means "start now" */
typedef enum { NO_EDGE = 0, RISING_EDGE, FALLING_EDGE, EITHER_EDGE } rec_trig_edge_t;

/* err info */
typedef enum {
    REC_OK = 0,
    REC_ERR_NO_BUF,      /* recorder has no buffer */
    REC_ERR_NO_CHAN,     /* num_ch_used == 0: nothing is bound */
    REC_ERR_TOO_MANY_CH, /* num_ch_used above REC_MAX_CH */
    REC_ERR_UNBOUND,     /* a channel below num_ch_used has no address */
    REC_ERR_TRIG_CH,     /* trig_ch is not one of the bound channels */
    REC_ERR_DEPTH,       /* cap/stride < 2: no room for a post-trigger window */
    REC_ERR_BAD_OP       /* rec_run was handed an opcode it does not know */
} rec_err_t;

/* WHY THE ENUM FIELDS ARE uint8_t, HERE AND BELOW. Every enum this file stores -- rec_type_t,
 * rec_trig_edge_t, rec_state_t, rec_err_t, rec_op_t -- is held in a uint8_t with the enum named in
 * the comment, and passed as the enum at the API. The enums themselves are the type-safe surface;
 * these are storage.
 *
 * The reason is that this LAYOUT is an interface. The host resolves members by offset and width
 * out of the DWARF, so the width is part of the contract -- and an enum's width is not fixed by C.
 * arm-none-eabi-gcc defaults to -fshort-enums and would make it one byte; the x86-64 host test
 * build makes it four. One declaration, two layouts, with the tests exercising one of them and the
 * tools talking to the other. A uint8_t is the same byte everywhere with no build flag to get
 * right, and it is what keeps the command frame six bytes of opcode and arguments rather than
 * twenty-four. */
typedef struct {
    const void *addr;   /* the address of a variable to sample in a channel */
    uint8_t     type;   /* rec_type_t: how to read that variable */
} rec_ch_t;

/* --- the command frame: the ONE thing outside this module may write ---------------------------
 *
 * Every op here IS an API call, marshalled. A caller that can execute code calls the function; a
 * caller that can only move memory -- the host over SWD -- writes this frame and rec_run makes the
 * call for it. So there is one implementation of every rule, and no client reimplements the
 * module's bookkeeping by poking at its fields.
 *
 * It is a MAILBOX, not a queue: one op at a time, and rec_run clears op to REC_OP_NONE when it has
 * consumed it. The sender waits for that zero before writing the next -- which is also the only
 * handshake a debug probe can drive, since it writes one word at a time with no atomicity. */
typedef enum {
    REC_OP_NONE = 0,     /* inbox empty */
    REC_OP_BIND_CH,      /* ch, type, addr  -> rec_bind_ch */
    REC_OP_UNBIND_ALL,   /*                 -> rec_unbind_all */
    REC_OP_SET_TRIG,     /* ch, edge, lvl, hyst -> rec_set_trig */
    REC_OP_SET_PRE_TRIG, /* pct             -> rec_set_pre_trig */
    REC_OP_SET_DECIM,    /* decim           -> rec_set_decim */
    REC_OP_ARM,          /*                 -> rec_arm */
    REC_OP_STOP          /*                 -> rec_stop */
} rec_op_t;

typedef struct {
    volatile uint8_t  op;      /* rec_op_t; back to REC_OP_NONE once rec_run has acted */
    volatile uint8_t  err;     /* rec_err_t from the op just consumed */
    volatile uint8_t  ch;      /* BIND_CH, SET_TRIG: which channel */
    volatile uint8_t  type;    /* BIND_CH: rec_type_t */
    volatile uint8_t  edge;    /* SET_TRIG: rec_trig_edge_t */
    volatile uint8_t  pct;     /* SET_PRE_TRIG */
    volatile uint16_t decim;   /* SET_DECIM */
    const void *volatile addr; /* BIND_CH: the address to watch. A POINTER, not an integer --
                                * it is pointer-sized on whatever this is built for, which is
                                * what lets a host test drive the frame at all, and the probe
                                * writes it the same way it reads buf. */
    volatile int32_t  lvl;     /* SET_TRIG */
    volatile int32_t  hyst;    /* SET_TRIG */
} rec_cmd_t;

/* Storage, time base, and where the capture has got to. A reader needs every one of these to make
 * sense of the buffer: the ring cannot be unwrapped without depth, stride and trig_idx, and the
 * records carry no timestamp, so a buffer with no rate is uninterpretable.
 *
 * Everything NOT in here is the module's own business and may be renamed without telling anyone,
 * which is the point of drawing the line -- the alternative is a client naming internals, where
 * every tidy-up is a breaking change. */
typedef struct {
    int16_t          *buf;
    uint16_t          cap;                 /* buf length in int16s */
    uint32_t          fs_hz;               /* the rate rec_step is CALLED */
    volatile uint8_t  state;               /* rec_state_t */
    volatile uint8_t  err;                 /* rec_err_t from the last arm; REC_OK once one took */
    volatile uint8_t  num_ch_used;         /* HOW MANY are bound, not which: 1..REC_MAX_CH, so
                                            * the bound channels are 0..num_ch_used-1
                                            * (0 is REC_ERR_NO_CHAN) */
    volatile uint8_t  stride;              /* int16s one record costs: the widths summed */
    volatile uint16_t depth;               /* records the buffer holds at this stride: cap/stride */
    volatile uint16_t decim;               /* record one rec_step call in decim (0/1 == all) */
    volatile uint16_t n_records_pre_trig;  /* pre_trig_pct resolved to records */
    volatile uint16_t n_records_taken;     /* records written since arm, saturating at depth */
    volatile uint16_t trig_idx;            /* record holding the sample that FIRED (t = 0) */
} rec_pub_t;

/* The handle. Its definition lives in rec_storage.h: include that only to ALLOCATE one.
 * Everything below takes a rec_t *, and everything a client needs to see is in the
 * rec_pub_t it reads and the rec_cmd_t it writes. */
typedef struct rec_s rec_t;

/* Bind the port's buffer and time base, and land in REC_IDLE. cap is in int16s. */
void rec_init(rec_t *r, int16_t *buf, uint16_t cap, uint32_t fs_hz);

/* --- configure (cold path; a no-op on an out-of-range channel) -------------------------- */

// bind a variable to a channel, recorder has max 8 channels. 
void rec_bind_ch(rec_t *r, uint8_t ch, const void *addr, rec_type_t type);

/* Drop every channel. The table, not the arm: rec_stop is what halts a capture. */
void rec_unbind_all(rec_t *r);

/* What fires the capture. ch is a bound channel, and lvl is in that channel's stored units.
 * NO_EDGE ignores ch, lvl and hyst. */
void rec_set_trig(rec_t *r, uint8_t ch, rec_trig_edge_t edge, int32_t lvl, int32_t hyst);

/* Where t = 0 sits: percent of the window recorded BEFORE the trigger. */
void rec_set_pre_trig(rec_t *r, uint8_t pct);

/* downsample, record a sample in n tick (0 and 1 both mean every tick).  */
void rec_set_decim(rec_t *r, uint16_t n);

/* --- run -------------------------------------------------------------------------------- */

/* Validate the config, derive the window, and arm. Cold path (it divides). REC_OK, or the check
 * that refused -- in which case it stays IDLE and the reason also lands in r->err. */
rec_err_t rec_arm(rec_t *r);

/* Abandon the capture and go back to IDLE. The bound channels stay bound. */
void rec_stop(rec_t *r);

/* One step: offer the bound channels once. THE CALLER'S RATE IS THE SAMPLE RATE -- this file
 * counts calls, not time, and fs_hz is only what the port said that rate was. Cheap enough to
 * call unconditionally: IDLE/DONE return immediately. */
void rec_step(rec_t *r);

/* Consume one command from the inbox, if there is one, and make the API call it names. Cold path:
 * the port calls this in its main loop, beside its other housekeeping. The result lands in
 * cmd.err, and cmd.op returns to REC_OP_NONE to say the frame has been read. */
void rec_run(rec_t *r);

/* The published view -- state, err, and everything a reader needs to unwrap the buffer. This is
 * how a client with only this header sees the capture: the fields are public, the struct holding
 * them is not. */
const rec_pub_t *rec_get_pub(const rec_t *r);

/* --- read back, once state is REC_DONE -------------------------------------------------- */

/* One recorded sample: record idx of the window (0..n_records_taken-1, oldest first), channel
 * The ring wrap and the record layout live in here. Out of range reads 0.
 *
 * The stored BIT PATTERN, not a value in the channel's type: a narrow channel sign-extends from
 * its int16, a wide one is the full 32 bits, and an unsigned channel is the caller's cast. */
int32_t rec_get_sample(const rec_t *r, uint16_t idx, uint8_t ch);
#ifdef __cplusplus
}
#endif
#endif
