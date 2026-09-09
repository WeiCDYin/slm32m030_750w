#ifndef MC_REC_STORAGE_H
#define MC_REC_STORAGE_H
#include "rec.h"
#ifdef __cplusplus
extern "C" {
#endif

/* rec_t's DEFINITION, kept out of rec.h so that including the interface does not hand a caller
 * the fields. Include this only to ALLOCATE one -- the port owns the instance, the way it owns
 * the buffer -- or to test the implementation. Everyone else takes a rec_t * from rec.h, reads
 * r->pub, writes r->cmd, and calls the API.
 *
 * The point is not secrecy: a debugger still sees every field, and the host still resolves them
 * out of the ELF. It is that a rename inside this file cannot break a client, because a client
 * that never saw the field cannot have named it. */
struct rec_s {
    /* FIELD ORDER IS BY ACCESS SIZE, NOT BY ROLE. Thumb's compact load/store forms encode the
     * field offset in 5 bits scaled by the access width, so a member reachable at a small offset
     * costs one 16-bit instruction and one past the limit costs a 32-bit encoding on ARMv7-M --
     * or, on ARMv6-M where no wide encoding exists, an extra instruction to build the address.
     * The limits are 31 for a byte, 62 for a halfword, 124 for a word, so the first 32 bytes are
     * scarce and are spent on the ISR's BYTE fields. rec_step touches every scalar below on a
     * sampling tick, and it runs inside the carrier ISR's WCET bracket (main.c), so this is the
     * one struct in the tree where layout is worth arranging by hand.
     *
     * pub STAYS FIRST: its `state` is the idle check every tick opens with (rec.c), and at offset
     * 12 it is inside the byte limit. Everything the cold path owns -- cmd, and the ch table that
     * only rec_bind_ch writes -- goes after, since indexed access to an array computes its base
     * address anyway and gains nothing from sitting low. */

    /* --- the outside's read side; first, for pub.state (see above) ----------------------- */
    rec_pub_t pub;                         /* read by the outside, written only in here */

    /* --- ISR-hot scalars: bytes inside 31, halfwords inside 62 -------------------------- */
    volatile uint8_t  primed;              /* the edge detector has left the band (see rec.c) */
    volatile uint8_t  trig_ch;             /* WHICH CHANNEL, not an address: the level is compared
                                            * against the very value the buffer keeps, so the
                                            * threshold you set is the threshold you see. Record
                                            * it if you want to trigger on it. */
    volatile uint8_t  trig_edge;           /* rec_trig_edge_t; NO_EDGE fires at once */
    volatile uint8_t  pre_trig_pct;        /* pre-trigger share of the buffer, percent */
    volatile uint16_t wr;                  /* next record */
    /* ISR-private, but volatile all the same: rec_arm's commit works because a volatile store
     * cannot be reordered past another one, and that only covers these if they are volatile too. */
    volatile uint16_t n_records_left;      /* number of samples to take after the trigger; counts down to 0 */
    volatile uint16_t decim_cnt;           /* calls since the last recorded one */
    volatile int32_t  trig_lvl;            /* in the channel's stored units, at that channel's width --
                                            * 32 bits so a wide channel can be triggered anywhere in
                                            * its range */
    volatile int32_t  trig_hyst;           /* the signal must leave the band on the wrong side before a
                                            * crossing counts; 0 on a state/stage channel, a few hundred
                                            * LSB on a q15 current, which dithers across any bare level */
    volatile uint8_t  offset[REC_MAX_CH];  /* number of int16s into a record where a channel starts */

    /* --- cold path: written by the setters and by rec_run, never sized for the ISR ------- */
    rec_cmd_t cmd;                         /* written by the outside, consumed by rec_run */
    volatile rec_ch_t ch[REC_MAX_CH];
};

#ifdef __cplusplus
}
#endif
#endif
