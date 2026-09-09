/* GENERATED at install time -- the identity of lib/libmc_core.a in THIS drop.
 *
 * Compile it into a global your bench harness can read, so that a stale core inside a fresh .bin
 * is caught rather than measured:
 *
 *     const char g_mc_core_id[] = MC_CORE_ID;
 *
 * It is delivered beside the archive rather than inside it because it is a hash OF the archive --
 * see cmake/mc_core_id.cmake for why that cannot be inverted. */
#ifndef MC_CORE_ID_H
#define MC_CORE_ID_H
#define MC_CORE_ID "167dbd41d71bb05b"
#endif
