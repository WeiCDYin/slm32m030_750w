#include "ntc.h"

/* Direct lookup table: index = 12-bit ADC code >> 4 (0..255), value = temperature
 * [deg C]. NTC (10k, B=3950) pulls the node up to 5 V, a 10k resistor pulls it
 * down to GND, so temperature rises -> NTC resistance falls -> code rises:
 *   R = R25*exp(B*(1/T - 1/T25)); code = 4096*10k/(R+10k)
 * Each entry stores the temperature at the middle of its 16-code bin, clamped
 * to the -40..120 deg C range. O(1) lookup -- no search, no FPU. */
#define NTC_CODE_SHIFT (4u)
#define NTC_CODE_OPEN  (50u)   /* <= this: divider near 0 V -> NTC open */
#define NTC_CODE_SHORT (4060u) /* >= this: divider near 5 V -> NTC shorted */

static const int8_t ntc_code_table[256] = {
    -40, -40, -40, -40, -40, -40, -39, -37, -35, -34, -32, -31, -30, -28, -27, -26, -25, -24, -23, -22, -21, -21, -20, -19, -18, -17, -17, -16, -15,
    -15, -14, -13, -13, -12, -12, -11, -11, -10, -9,  -9,  -8,  -8,  -7,  -7,  -6,  -6,  -5,  -5,  -4,  -4,  -4,  -3,  -3,  -2,  -2,  -1,  -1,  0,
    0,   0,   1,   1,   2,   2,   2,   3,   3,   4,   4,   4,   5,   5,   5,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   10,  10,  10,  11,
    11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,
    21,  21,  22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  26,  26,  26,  27,  27,  27,  28,  28,  28,  29,  29,  29,  30,  30,  31,  31,
    31,  32,  32,  32,  33,  33,  34,  34,  34,  35,  35,  36,  36,  36,  37,  37,  38,  38,  38,  39,  39,  40,  40,  41,  41,  41,  42,  42,  43,
    43,  44,  44,  45,  45,  46,  46,  46,  47,  47,  48,  48,  49,  50,  50,  51,  51,  52,  52,  53,  53,  54,  55,  55,  56,  56,  57,  58,  58,
    59,  60,  60,  61,  62,  62,  63,  64,  65,  66,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  82,  83,  84,  86,
    87,  89,  91,  92,  94,  96,  98,  100, 103, 105, 108, 111, 114, 118, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
};

int16_t ntc_temp_c(uint16_t code)
{
    if (code <= NTC_CODE_OPEN)
        return (int16_t)3000; /* NTC open: node at 0 V, flag over-temp */
    if (code >= NTC_CODE_SHORT)
        return (int16_t)-400; /* NTC shorted: node at 5 V */
    return (int16_t)ntc_code_table[code >> NTC_CODE_SHIFT];
}
