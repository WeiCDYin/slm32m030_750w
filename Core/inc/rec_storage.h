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
    /* --- the two sides of the boundary ------------------------------------------------- */
    rec_pub_t pub;                         /* read by the outside, written only in here */
    rec_cmd_t cmd;                         /* written by the outside, consumed by rec_run */

    /* --- config: written by the setters, read by the ISR -------------------------------- */
    volatile rec_ch_t ch[REC_MAX_CH];
    volatile uint8_t  trig_ch;             /* WHICH CHANNEL, not an address: the level is compared
                                            * against the very value the buffer keeps, so the
                                            * threshold you set is the threshold you see. Record
                                            * it if you want to trigger on it. */
    volatile uint8_t  trig_edge;           /* rec_trig_edge_t; NO_EDGE fires at once */
    volatile int32_t  trig_lvl;            /* in the channel's stored units, at that channel's width --
                                            * 32 bits so a wide channel can be triggered anywhere in
                                            * its range */
    volatile int32_t  trig_hyst;           /* the signal must leave the band on the wrong side before a
                                            * crossing counts; 0 on a state/stage channel, a few hundred
                                            * LSB on a q15 current, which dithers across any bare level */
    volatile uint8_t  pre_trig_pct;        /* pre-trigger share of the buffer, percent */

    /* --- position: written by the ISR --------------------------------------------------- */
    volatile uint8_t  offset[REC_MAX_CH];  /* number of int16s into a record where a channel starts */
    volatile uint16_t wr;                  /* next record */
    /* ISR-private, but volatile all the same: rec_arm's commit works because a volatile store
     * cannot be reordered past another one, and that only covers these if they are volatile too. */
    volatile uint16_t n_records_left;      /* number of samples to take after the trigger; counts down to 0 */
    volatile uint16_t decim_cnt;           /* calls since the last recorded one */
    volatile uint8_t  primed;              /* the edge detector has left the band (see rec.c) */
};

#ifdef __cplusplus
}
#endif
#endif
