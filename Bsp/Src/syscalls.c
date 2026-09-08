/*
 * Minimal newlib syscall stubs for the bare-metal target.
 *
 * libc_nano's default nosys stubs print "not implemented and will always
 * fail" warnings at link time (assert -> __assert_func -> fiprintf -> _write
 * pulls the whole chain in). These replacements give every syscall a quiet,
 * well-defined behaviour -- no console, no file system, no heap beyond the
 * linker-provided `end` symbol.
 */
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>

int _close(int fd)
{
    (void)fd;
    return -1;
}

int _fstat(int fd, struct stat *st)
{
    (void)fd;
    if (st)
        st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    (void)fd;
    return 1;
}

int _lseek(int fd, off_t off, int whence)
{
    (void)fd;
    (void)off;
    (void)whence;
    return -1;
}

int _read(int fd, char *buf, size_t cnt)
{
    (void)fd;
    (void)buf;
    (void)cnt;
    return -1;
}

int _write(int fd, const char *buf, size_t cnt)
{
    (void)fd;
    (void)buf;
    (void)cnt;
    return -1;
}

int _getpid(void)
{
    return 1;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    return -1;
}

/* Heap grows up from the linker script's `end` symbol (see slm32m030.ld). */
extern char end[];

static char *heap_end;

caddr_t _sbrk(size_t incr)
{
    char *prev;

    if (heap_end == 0)
        heap_end = end;
    prev = heap_end;
    heap_end += incr;
    return (caddr_t)prev;
}
