#include "qmath.h"

/* Extracted from pll.c, where it lived as a static alongside a second copy inside
 * stsmo_sqrt_pu (stsmo.c). pll.h called for exactly this once a third caller appeared;
 * mc.c's duty-vector magnitude is that caller. Semantics are unchanged from pll.c's
 * original -- same loop, same Q15_MAX clamp -- so no observer behaviour moves with it. */
q15_t isqrt_q30(uint32_t v) {
    uint32_t rem = 0, root = 0;
    for(int i = 0; i < 16; i++) {
        root <<= 1;
        rem = (rem << 2) | (v >> 30);
        v <<= 2;
        if(root < rem) { rem -= root | 1u; root |= 2u; }
    }
    root >>= 1;
    return (q15_t)(root > (uint32_t)Q15_MAX ? Q15_MAX : root);
}
