/* GENERATED at install time -- the identity of lib/libmc_core.a in THIS release.
 *
 * Compile it into a global your bench harness can read, so that a stale core inside a fresh .bin
 * is caught rather than measured:
 *
 *     const char g_mc_core_hash[] = MC_CORE_HASH;
 *
 * SHA-256 over the OBJECT FILES inside lib/libmc_core.a, truncated to the first 16 hex digits --
 * 64 bits of the 256. Not sha256sum of the .a: that is not reproducible, because ar stamps member
 * timestamps into the archive wrapper. Delivered beside the archive rather than inside it because
 * it is a hash of the code that would be doing the carrying; see cmake/mc_core_hash.cmake for why
 * that cannot be inverted. */
#ifndef MC_CORE_HASH_H
#define MC_CORE_HASH_H
#define MC_CORE_HASH "4671456610677d6d"
#endif
