/* GENERATED per ABI row by CMakeLists.txt -- DO NOT EDIT.
 *
 * This release is 0.1.0, built for armv6m-armclang.
 *
 * IF YOU ARE READING THIS BECAUSE A BUILD FAILED, it is one of two checks, and each one is asking
 * a different question:
 *
 *   "undefined reference to mc_abi__armv6m_armclang__v0_1_0"
 *       DID THE .h FILES AND THE .a FILE COME FROM THE SAME RELEASE? No -- you have mixed two
 *       downloads. These headers are from 0.1.0; the library you linked is from some
 *       other release. Use the inc/ and lib/ from ONE drop.
 *       Easy to get into: copy a new inc/ over an old project and forget the lib/, or point the
 *       build at an install directory that was refreshed at some other time.
 *
 *   "size of array 'mc_t_is_not_the_size_this_release_was_built_for...' is negative"
 *   "static assertion failed: mc_t_is_not_the_size..."
 *       DOES YOUR COMPILER PUT THE FIELDS OF mc_t IN THE SAME PLACES OURS DID? No. Compare your
 *       settings against ABI.txt at the top of this drop -- enum size (-fshort-enums) and struct
 *       packing are what move them. The two compilers have to agree, because YOU declare mc_t
 *       (`static mc_t g_mc;`) while the library that reads those fields was compiled by us.
 *
 * WHY BOTH. Neither check can do the other's job. The size check compares your compiler against
 * THIS HEADER, and a header agrees with itself -- so old headers plus a new library would pass it
 * and still be wrong. The tag compares the header against THE LIBRARY, and cannot see a consumer
 * who compiled correct headers with the wrong flags. Together they close the chain:
 *     your compiler  ==  these headers  ==  this library
 *
 * Neither costs anything at run time, and neither has to be switched on: they arrive with a header
 * you already include, because a check you have to remember to run is absent on the day it
 * matters. What they are protecting against is the one class of C mistake that reports nothing --
 * it compiles, it links, it runs, and the drive reads its fields from the wrong offsets. */
#ifndef MC_VERSION_H
#define MC_VERSION_H

#define MC_VERSION_MAJOR  0
#define MC_VERSION_MINOR  1
#define MC_VERSION_PATCH  0
#define MC_VERSION_STR   "0.1.0"
#define MC_ABI           "armv6m-armclang"
#define MC_T_SIZE_EXPECTED 112

/* CHECK 1 -- DID THESE .h FILES AND THAT .a FILE COME FROM THE SAME RELEASE? Asked at LINK time.
 *
 * Only this release's library defines this symbol, and only this release's headers ask for it, so
 * files from two different downloads cannot resolve against each other. The NAME carries the row and the version so the linker's
 * message is a diagnosis rather than a puzzle. mc.h calls it from mc_abi_check(), because a
 * declaration nothing references links happily against anything. */
#define MC_ABI_TAG mc_abi__armv6m_armclang__v0_1_0
#ifdef __cplusplus
extern "C" {
#endif
void MC_ABI_TAG(void);
#ifdef __cplusplus
}
#endif

/* CHECK 2 -- DOES YOUR COMPILER PUT THE FIELDS OF mc_t WHERE OURS DID? Asked at COMPILE time.
 *
 * sizeof(mc_t) was MEASURED when this row was built (112 bytes), not assumed. The
 * customer writes `static mc_t g_mc;` themselves, so their compiler decides the layout -- and the
 * axes that move it fail SILENTLY otherwise. Enum size is the live one here: nothing in this tree
 * sets -fshort-enums, so every build takes its toolchain's default, and the same headers give
 * sizeof(mc_t) 144 with int-sized enums against 136 with short ones. That is not a link error and
 * not a warning; it is a drive writing spd_cmd where the core reads something else.
 *
 * THE BUILD IS C99 (CMakeLists.txt), and so is a customer's, so _Static_assert is out -- it is
 * C11. The same call types.h already makes about _Alignas. GCC accepts it in C99 mode as an
 * extension, which is exactly the trap: it would compile here and fail on a stricter toolchain
 * holding the same headers.
 *
 * So: the negative-array typedef, valid all the way back to C89. It carries no message, hence the
 * NAME argument -- the identifier IS the diagnosis, because that is what the compiler prints.
 * __LINE__ keeps two assertions in one translation unit from colliding, since C99 makes a repeated
 * typedef an error. C++ gets the real thing, stringified. */
#if defined(__cplusplus)
#  define MC_STATIC_ASSERT(cond, name) static_assert(cond, #name)
#else
#  define MC_CAT_(a, b) a##b
#  define MC_CAT(a, b)  MC_CAT_(a, b)
#  define MC_STATIC_ASSERT(cond, name)        typedef char MC_CAT(name##__line_, __LINE__)[(cond) ? 1 : -1]
#endif

#endif
