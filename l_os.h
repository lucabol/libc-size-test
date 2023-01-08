// STANDARD C INCLUDE FOR FREESTANDING {{{1

#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// CONFIG {{{1
// Override this to change declaration of functions
#ifndef LASTS_FDEF
#define LASTS_FDEF static inline
#endif

// DECLARATIONS {{{1

#ifndef _WIN32 // This gets defined by mingw
typedef   signed long       ssize_t;
#endif

typedef   unsigned int        mode_t;
typedef   signed int          pid_t;
typedef   signed long         off_t;
typedef   ptrdiff_t           LASTS_FD;
#define   LASTS_STDIN         0
#define   LASTS_STDOUT        1
#define   LASTS_STDERR        2

// Standard C library
inline size_t lasts_wcslen(const wchar_t *s); // Has to be inline for MINGW to compile w/out warns
LASTS_FDEF size_t lasts_strlen(const char *str);
LASTS_FDEF char *lasts_strcpy(char *dst, const char *src);
LASTS_FDEF char *lasts_strchr(const char *s, int c);
LASTS_FDEF char *lasts_strrchr(const char *s, int c);
LASTS_FDEF char *lasts_strstr(const char *s1, const char *s2);
LASTS_FDEF int lasts_strncmp(const char *s1, const char *s2, size_t n);

LASTS_FDEF int lasts_isdigit(int c);
LASTS_FDEF long lasts_atol(const char *s);
LASTS_FDEF int lasts_atoi(const char *s);
LASTS_FDEF char *lasts_ltoa(long in, char* buffer, int radix);

LASTS_FDEF void *lasts_memmove(void *dst, const void *src, size_t len);
LASTS_FDEF void *lasts_memset(void *dst, int b, size_t len);
LASTS_FDEF int lasts_memcmp(const void *s1, const void *s2, size_t n);
__attribute__((weak,unused))
void *lasts_memcpy(void *dst, const void *src, size_t len);

// System interaction
LASTS_FDEF void lasts_exit(int status);

LASTS_FDEF LASTS_FD lasts_open_read(const char* file);
LASTS_FDEF LASTS_FD lasts_open_write(const char* file);
LASTS_FDEF LASTS_FD lasts_open_readwrite(const char* file);
LASTS_FDEF LASTS_FD lasts_open_append(const char* file);
LASTS_FDEF LASTS_FD lasts_open_trunc(const char* file);
LASTS_FDEF int lasts_close(LASTS_FD fd);
LASTS_FDEF ssize_t lasts_read(LASTS_FD fd, void *buf, size_t count);
LASTS_FDEF ssize_t lasts_write(LASTS_FD fd, const void *buf, size_t count);

// My functions
LASTS_FDEF void lasts_exitif(bool condition, int code, char *message);

inline void putchar_(char ch) {
  lasts_write(LASTS_STDOUT, &ch, 1);
} 

#ifndef LASTS_DONT_OVERRIDE
#  define wcslen lasts_wcslen
#  define strlen lasts_strlen
#  define strcpy lasts_strcpy
#  define strchr lasts_strchr
#  define strrchr lasts_strrchr
#  define strstr lasts_strstr
#  define isdigit lasts_isdigit
#  define atol lasts_atol
#  define atoi lasts_atoi
#  define ltoa lasts_ltoa
#  define memmove lasts_memmove
#  define memset lasts_memset
#  define memcmp lasts_memcmp
#  define memcpy lasts_memcpy

#  define exit lasts_exit
#  define close lasts_close
#  define read lasts_read
#  define write lasts_write
#  define lseek lasts_lseek
#  define dup lasts_dup
#  define execve lasts_execve
#  define fork lasts_fork
#  define mkdir lasts_mkdir
#  define chdir lasts_chdir
#  define sched_yield lasts_sched_yield
#  define exitif lasts_exitif
#  define open_read lasts_open_read
#  define open_write lasts_open_write
#  define open_readwrite lasts_open_readwrite
#  define open_append lasts_open_append
#  define open_trunc lasts_open_trunc

#define STDIN    LASTS_STDIN
#define STDOUT   LASTS_STDOUT
#define STDERR   LASTS_STDERR
#endif // LASTS_DONT_OVERRIDE

// SYSTEM AGNOSTIC FUNCTIONS {{{1

/* some size-optimized reimplementations of a few common str* and mem*
 * functions. They're marked LASTS_FDEF, except memcpy() which is used
 * by libgcc on ARM, so they are marked weak instead in order not to cause an
 * error when building a program made of multiple files (not recommended).
 */

LASTS_FDEF __attribute__((unused))
void lasts_exitif(bool condition, int code, char *message) {
    if(condition) {
        if(message) lasts_write(LASTS_STDERR, message, lasts_strlen(message));
        lasts_exit(code);
    }
}

inline __attribute__((unused))
size_t lasts_wcslen(const wchar_t *s) {
    size_t len = 0;
    while(s[len] != L'\0') ++len;
    return len;
}

LASTS_FDEF __attribute__((unused))
size_t lasts_strlen(const char *str)
{
    size_t len;

    for (len = 0; str[len]; len++);
    return len;
}

#if defined(__has_builtin)
# if __has_builtin (__builtin_strlen)
#       define lasts_strelen __builtin_strlen
# endif
#endif

LASTS_FDEF __attribute__((unused))
void *lasts_memmove(void *dst, const void *src, size_t len)
{
    size_t pos = (dst <= src) ? -1 : (long)len;
    void *ret = dst;

    while (len--) {
        pos += (dst <= src) ? 1 : -1;
        ((char *)dst)[pos] = ((char *)src)[pos];
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
void *lasts_memset(void *dst, int b, size_t len)
{
    char *p = dst;

    while (len--)
        *(p++) = b;
    return dst;
}

#if defined(__has_builtin)
# if __has_builtin (__builtin_memset)
#       define lasts_memset __builtin_memset
# endif
#endif

LASTS_FDEF __attribute__((unused))
int lasts_memcmp(const void *s1, const void *s2, size_t n)
{
    size_t ofs = 0;
    char c1 = 0;

    while (ofs < n && !(c1 = ((char *)s1)[ofs] - ((char *)s2)[ofs])) {
        ofs++;
    }
    return c1;
}

LASTS_FDEF __attribute__((unused))
char *lasts_strcpy(char *dst, const char *src)
{
    char *ret = dst;

    while ((*dst++ = *src++));
    return ret;
}

LASTS_FDEF __attribute__((unused))
char *lasts_strchr(const char *s, int c)
{
    while (*s) {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    return NULL;
}

LASTS_FDEF __attribute__((unused))
char *lasts_strrchr(const char *s, int c)
{
    const char *ret = NULL;

    while (*s) {
        if (*s == (char)c)
            ret = s;
        s++;
    }
    return (char *)ret;
}


LASTS_FDEF  __attribute__((unused))
int lasts_strncmp(const char *s1, const char *s2, size_t n) {
    unsigned char u1, u2;

    while (n-- > 0)
    {
        u1 = (unsigned char) *s1++;
        u2 = (unsigned char) *s2++;
        if (u1 != u2)
            return u1 - u2;
        if (u1 == '\0')
            return 0;
    }
    return 0;
}

LASTS_FDEF  __attribute__((unused))
char *lasts_strstr(const char *s1, const char *s2) {
    const size_t len = strlen (s2);
    while (*s1)
    {
        if (!lasts_memcmp (s1, s2, len))
            return (char *)s1;
        ++s1;
    }
    return (0);
}

LASTS_FDEF __attribute__((unused))
int lasts_isdigit(int c)
{
    return (unsigned int)(c - '0') <= 9;
}

LASTS_FDEF __attribute__((unused))
long lasts_atol(const char *s)
{
    unsigned long ret = 0;
    unsigned long d;
    int neg = 0;

    if (*s == '-') {
        neg = 1;
        s++;
    }

    while (1) {
        d = (*s++) - '0';
        if (d > 9)
            break;
        ret *= 10;
        ret += d;
    }

    return neg ? -ret : ret;
}

LASTS_FDEF __attribute__((unused))
int lasts_atoi(const char *s)
{
    return lasts_atol(s);
}

LASTS_FDEF __attribute__((unused))
char *lasts_ltoa(long in, char* buffer, int radix)
{
    char       *pos = buffer + sizeof(buffer) - 1;
    int         neg = in < 0;
    unsigned long n = neg ? -in : in;

    *pos-- = '\0';
    do {
        *pos-- = '0' + n % radix;
        n /= radix;
        if (pos < buffer)
            return pos + 1;
    } while (n);

    if (neg)
        *pos-- = '-';
    return pos + 1;
}

__attribute__((weak,unused))
void *lasts_memcpy(void *dst, const void *src, size_t len)
{
    return lasts_memmove(dst, src, len);
}
// UNIX PREAMBLE {{{1
#if !defined(_WIN32)

#include <asm/unistd.h>
#include <asm/ioctls.h>
#include <asm/errno.h>
#include <linux/fs.h>
#include <linux/loop.h>
#include <linux/time.h>


// TODO: don't think I need errno. Leaving there for now disabled.
#ifndef NOLIBC_IGNORE_ERRNO
static int errno;
#define SET_ERRNO(v) do { errno = (v); } while (0)
#else
#define SET_ERRNO(v) do { } while (0)
#endif

/* errno codes all ensure that they will not conflict with a valid pointer
 * because they all correspond to the highest addressable memory page.
 */
#define MAX_ERRNO 4095

/* for poll() */
struct pollfd {
    int fd;
    short int events;
    short int revents;
};

/* all the *at functions */
#ifndef AT_FDCWD
#define AT_FDCWD             -100
#endif

/* lseek */
#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2

/* reboot */
#define LINUX_REBOOT_MAGIC1         0xfee1dead
#define LINUX_REBOOT_MAGIC2         0x28121969
#define LINUX_REBOOT_CMD_HALT       0xcdef0123
#define LINUX_REBOOT_CMD_POWER_OFF  0x4321fedc
#define LINUX_REBOOT_CMD_RESTART    0x01234567
#define LINUX_REBOOT_CMD_SW_SUSPEND 0xd000fce2

#define WEXITSTATUS(status)   (((status) & 0xff00) >> 8)
#define WIFEXITED(status)     (((status) & 0x7f) == 0)

/* for SIGCHLD */
#include <asm/signal.h>

#if defined(__x86_64__)
/* Syscalls for x86_64 :
 *   - registers are 64-bit
 *   - syscall number is passed in rax
 *   - arguments are in rdi, rsi, rdx, r10, r8, r9 respectively
 *   - the system call is performed by calling the syscall instruction
 *   - syscall return comes in rax
 *   - rcx and r8..r11 may be clobbered, others are preserved.
 *   - the arguments are cast to long and assigned into the target registers
 *     which are then simply passed as registers to the asm code, so that we
 *     don't have to experience issues with register constraints.
 *   - the syscall number is always specified last in order to allow to force
 *     some registers before (gcc refuses a %-register at the last position).
 */

#define my_syscall0(num)                                                      \
         long _ret;                                                            \
         register long _num  asm("rax") = (num);                               \
         \
         asm volatile (                                                        \
                         "syscall\n"                                                   \
                         : "=a" (_ret)                                                 \
                         : "0"(_num)                                                   \
                         : "rcx", "r8", "r9", "r10", "r11", "memory", "cc"             \
                         )

#define my_syscall1(num, arg1)                                                \
         long _ret;                                                            \
         register long _num  asm("rax") = (num);                               \
         register long _arg1 asm("rdi") = (long)(arg1);                        \
         \
         asm volatile (                                                        \
                         "syscall\n"                                                   \
                         : "=a" (_ret)                                                 \
                         : "r"(_arg1),                                                 \
                         "0"(_num)                                                   \
                         : "rcx", "r8", "r9", "r10", "r11", "memory", "cc"             \
                         )

#define my_syscall2(num, arg1, arg2)                                          \
         long _ret;                                                            \
         register long _num  asm("rax") = (num);                               \
         register long _arg1 asm("rdi") = (long)(arg1);                        \
         register long _arg2 asm("rsi") = (long)(arg2);                        \
         \
         asm volatile (                                                        \
                         "syscall\n"                                                   \
                         : "=a" (_ret)                                                 \
                         : "r"(_arg1), "r"(_arg2),                                     \
                         "0"(_num)                                                   \
                         : "rcx", "r8", "r9", "r10", "r11", "memory", "cc"             \
                         )

#define my_syscall3(num, arg1, arg2, arg3)                                    \
         long _ret;                                                            \
         register long _num  asm("rax") = (num);                               \
         register long _arg1 asm("rdi") = (long)(arg1);                        \
         register long _arg2 asm("rsi") = (long)(arg2);                        \
         register long _arg3 asm("rdx") = (long)(arg3);                        \
         \
         asm volatile (                                                        \
                         "syscall\n"                                                   \
                         : "=a" (_ret)                                                 \
                         : "r"(_arg1), "r"(_arg2), "r"(_arg3),                         \
                         "0"(_num)                                                   \
                         : "rcx", "r8", "r9", "r10", "r11", "memory", "cc"             \
                         )

#define my_syscall4(num, arg1, arg2, arg3, arg4)                              \
         long _ret;                                                            \
         register long _num  asm("rax") = (num);                               \
         register long _arg1 asm("rdi") = (long)(arg1);                        \
         register long _arg2 asm("rsi") = (long)(arg2);                        \
         register long _arg3 asm("rdx") = (long)(arg3);                        \
         register long _arg4 asm("r10") = (long)(arg4);                        \
         \
         asm volatile (                                                        \
                         "syscall\n"                                                   \
                         : "=a" (_ret), "=r"(_arg4)                                    \
                         : "r"(_arg1), "r"(_arg2), "r"(_arg3), "r"(_arg4),             \
                         "0"(_num)                                                   \
                         : "rcx", "r8", "r9", "r11", "memory", "cc"                    \
                         )

#define my_syscall5(num, arg1, arg2, arg3, arg4, arg5)                        \
         long _ret;                                                            \
         register long _num  asm("rax") = (num);                               \
         register long _arg1 asm("rdi") = (long)(arg1);                        \
         register long _arg2 asm("rsi") = (long)(arg2);                        \
         register long _arg3 asm("rdx") = (long)(arg3);                        \
         register long _arg4 asm("r10") = (long)(arg4);                        \
         register long _arg5 asm("r8")  = (long)(arg5);                        \
         \
         asm volatile (                                                        \
                         "syscall\n"                                                   \
                         : "=a" (_ret), "=r"(_arg4), "=r"(_arg5)                       \
                         : "r"(_arg1), "r"(_arg2), "r"(_arg3), "r"(_arg4), "r"(_arg5), \
                         "0"(_num)                                                   \
                         : "rcx", "r9", "r11", "memory", "cc"                          \
                         )

#define my_syscall6(num, arg1, arg2, arg3, arg4, arg5, arg6)                  \
        ({                                                                            \
         long _ret;                                                            \
         register long _num  asm("rax") = (num);                               \
         register long _arg1 asm("rdi") = (long)(arg1);                        \
         register long _arg2 asm("rsi") = (long)(arg2);                        \
         register long _arg3 asm("rdx") = (long)(arg3);                        \
         register long _arg4 asm("r10") = (long)(arg4);                        \
         register long _arg5 asm("r8")  = (long)(arg5);                        \
         register long _arg6 asm("r9")  = (long)(arg6);                        \
         \
         asm volatile (                                                        \
                         "syscall\n"                                                   \
                         : "=a" (_ret), "=r"(_arg4), "=r"(_arg5)                       \
                         : "r"(_arg1), "r"(_arg2), "r"(_arg3), "r"(_arg4), "r"(_arg5), \
                         "r"(_arg6), "0"(_num)                                       \
                         : "rcx", "r11", "memory", "cc"                                \
                         );                                                                    \
                         _ret;                                                                 \
                         })

/* startup code */
asm(".section .text\n"
    ".global _start\n"
    "_start:\n"
    "pop %rdi\n"                // argc   (first arg, %rdi)
    "mov %rsp, %rsi\n"          // argv[] (second arg, %rsi)
    "lea 8(%rsi,%rdi,8),%rdx\n" // then a NULL then envp (third arg, %rdx)
    "and $-16, %rsp\n"          // x86 ABI : esp must be 16-byte aligned when
    "sub $8, %rsp\n"            // entering the callee
    "call main\n"               // main() returns the status code, we'll exit with it.
    "movzb %al, %rdi\n"         // retrieve exit code from 8 lower bits
    "mov $60, %rax\n"           // NR_exit == 60
    "syscall\n"                 // really exit
    "hlt\n"                     // ensure it does not return
    "");

/* fcntl / open */
#define O_RDONLY            0
#define O_WRONLY            1
#define O_RDWR              2
#define O_CREAT          0x40
#define O_EXCL           0x80
#define O_NOCTTY        0x100
#define O_TRUNC         0x200
#define O_APPEND        0x400
#define O_NONBLOCK      0x800
#define O_DIRECTORY   0x10000

/* The struct returned by the stat() syscall, equivalent to stat64(). The
 * syscall returns 116 bytes and stops in the middle of __unused.
 */
struct sys_stat_struct {
    unsigned long st_dev;
    unsigned long st_ino;
    unsigned long st_nlink;
    unsigned int  st_mode;
    unsigned int  st_uid;

    unsigned int  st_gid;
    unsigned int  __pad0;
    unsigned long st_rdev;
    long          st_size;
    long          st_blksize;

    long          st_blocks;
    unsigned long st_atime;
    unsigned long st_atime_nsec;
    unsigned long st_mtime;

    unsigned long st_mtime_nsec;
    unsigned long st_ctime;
    unsigned long st_ctime_nsec;
    long          __unused[3];
};

#else
#error "Just linux x86_64 and windows are supported. Paste relevant nolibc.h sections for more archs"
#endif

/* Below are the C functions used to declare the raw syscalls. They try to be
 * architecture-agnostic, and return either a success or -errno. Declaring them
 * LASTS_FDEF will lead to them being inlined in most cases, but it's still possible
 * to reference them by a pointer if needed.
 */

LASTS_FDEF __attribute__((noreturn,unused))
void sys_exit(int status)
{
    my_syscall1(__NR_exit, status & 255);
    while(1); // shut the "noreturn" warnings.
}

LASTS_FDEF __attribute__((unused))
int sys_close(int fd)
{
    my_syscall1(__NR_close, fd);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
int sys_dup(int fd)
{
    my_syscall1(__NR_dup, fd);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
int sys_execve(const char *filename, char *const argv[], char *const envp[])
{
    my_syscall3(__NR_execve, filename, argv, envp);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
pid_t sys_fork(void)
{
#ifdef __NR_clone
    /* note: some archs only have clone() and not fork(). Different archs
     * have a different API, but most archs have the flags on first arg and
     * will not use the rest with no other flag.
     */
    my_syscall5(__NR_clone, SIGCHLD, 0, 0, 0, 0);
    return _ret;
#elif defined(__NR_fork)
    my_syscall0(__NR_fork);
    return _ret;
#else
#error Neither __NR_clone nor __NR_fork defined, cannot implement sys_fork()
#endif
}

LASTS_FDEF __attribute__((unused))
int sys_fsync(int fd)
{
    my_syscall1(__NR_fsync, fd);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
pid_t sys_getpgid(pid_t pid)
{
    my_syscall1(__NR_getpgid, pid);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
pid_t sys_getpgrp(void)
{
    return sys_getpgid(0);
}

LASTS_FDEF __attribute__((unused))
pid_t sys_getpid(void)
{
    my_syscall0(__NR_getpid);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
off_t sys_lseek(int fd, off_t offset, int whence)
{
    my_syscall3(__NR_lseek, fd, offset, whence);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
int sys_mkdir(const char *path, mode_t mode)
{
#ifdef __NR_mkdirat
    my_syscall3(__NR_mkdirat, AT_FDCWD, path, mode);
    return _ret;
#elif defined(__NR_mkdir)
    my_syscall2(__NR_mkdir, path, mode);
    return _ret;
#else
#error Neither __NR_mkdirat nor __NR_mkdir defined, cannot implement sys_mkdir()
#endif
}
LASTS_FDEF __attribute__((unused))
int sys_chdir(const char *path)
{
    my_syscall1(__NR_chdir, path);
    return _ret;
}
LASTS_FDEF __attribute__((unused))
int sys_open(const char *path, int flags, mode_t mode)
{
#ifdef __NR_openat
    my_syscall4(__NR_openat, AT_FDCWD, path, flags, mode);
    return _ret;
#elif defined(__NR_open)
    my_syscall3(__NR_open, path, flags, mode);
    return _ret;
#else
#error Neither __NR_openat nor __NR_open defined, cannot implement sys_open()
#endif
}

LASTS_FDEF __attribute__((unused))
ssize_t sys_read(int fd, void *buf, size_t count)
{
    my_syscall3(__NR_read, fd, buf, count);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
ssize_t sys_reboot(int magic1, int magic2, int cmd, void *arg)
{
    my_syscall4(__NR_reboot, magic1, magic2, cmd, arg);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
int sys_sched_yield(void)
{
    my_syscall0(__NR_sched_yield);
    return _ret;
}

LASTS_FDEF __attribute__((unused))
ssize_t sys_write(int fd, const void *buf, size_t count)
{
    my_syscall3(__NR_write, fd, buf, count);
    return _ret;
}

// SYSTEM UNIX FUNCTIONS {{{1
/* Below are the libc-compatible syscalls which return x or -1 and set errno.
 * They rely on the functions above. Similarly they're marked LASTS_FDEF so that it
 * is possible to assign pointers to them if needed.
 */

LASTS_FDEF __attribute__((noreturn,unused))
void lasts_exit(int status)
{
    sys_exit(status);
}

LASTS_FDEF __attribute__((unused))
int lasts_chdir(const char *path)
{
    int ret = sys_chdir(path);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
int lasts_close(LASTS_FD fd)
{
    int ret = sys_close(fd);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
int lasts_dup(LASTS_FD fd)
{
    int ret = sys_dup(fd);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}


LASTS_FDEF __attribute__((unused))
int lasts_execve(const char *filename, char *const argv[], char *const envp[])
{
    int ret = sys_execve(filename, argv, envp);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
pid_t lasts_fork(void)
{
    pid_t ret = sys_fork();

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
off_t lasts_lseek(LASTS_FD fd, off_t offset, int whence)
{
    off_t ret = sys_lseek(fd, offset, whence);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
int lasts_mkdir(const char *path, mode_t mode)
{
    int ret = sys_mkdir(path, mode);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open(const char *path, int flags, mode_t mode)
{
    int ret = sys_open(path, flags, mode);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
ssize_t lasts_read(LASTS_FD fd, void *buf, size_t count)
{
    ssize_t ret = sys_read(fd, buf, count);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
int lasts_sched_yield(void)
{
    int ret = sys_sched_yield();

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
ssize_t lasts_write(LASTS_FD fd, const void *buf, size_t count)
{
    ssize_t ret = sys_write(fd, buf, count);

    if (ret < 0) {
        SET_ERRNO(-ret);
        ret = -1;
    }
    return ret;
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_read(const char* file) {
    return lasts_open(file, O_RDONLY, 0);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_write(const char* file) {
    return lasts_open(file, O_WRONLY | O_CREAT, 0644);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_readwrite(const char* file) {
    return lasts_open(file, O_RDWR | O_CREAT, 0644);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_append(const char* file) {
    return lasts_open(file, O_WRONLY | O_APPEND | O_CREAT, 0600);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_trunc(const char* file) {
    return lasts_open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
}
#else
// WINDOWS PREAMBLE {{{1

#define WIN32_LEAN_AND_MEAN 1
// See http://utf8everywhere.org/ for the general idea of managing text as utf-8 on windows
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <shellapi.h>

// MINGW complains I need to define this. It gets called inside main apparently
void __main() {}

int main(int argc, char* argv[]);

// Some routines liberally taken from various versions of minicrt, mincrt and tinylib. Look them up.

// CommandLineToArgvW is not in kernel32.dll
// Under Windows we take utf16 at the boundaries, but represents string internally as utf-8 (aka char).
static char** parseCommandLine(char* szCmdLine, int * argc)
{
    int arg_count = 0;
    int char_count = 0;
    char break_char = ' ';
    char *c;
    char **ret;
    char *ret_str;

    //
    //  Consume all spaces.  After this, we're either at
    //  the end of string, or we have an arg, and it
    //  might start with a quote
    //

    c = szCmdLine;
    while (*c == ' ') c++;
    if (*c == '"') {
        break_char = '"';
        c++;
    }

    while (*c != '\0') {
        if (*c == break_char) {
            break_char = ' ';
            c++;
            while (*c == break_char) c++;
            if (*c == '"') {
                break_char = '"';
                c++;
            }
            arg_count++;
        } else {
            char_count++;

            //
            //  If we hit a break char, we count the argument then.
            //  If we hit end of string, count it here; note we're
            //  only counting it if we counted a character before it
            //  (ie., trailing whitespace is not an arg.)
            //

            c++;

            if (*c == '\0') {
                arg_count++;
            }
        }
    }

    *argc = arg_count;

    ret = LocalAlloc( LMEM_FIXED, (arg_count * sizeof(char*)) + (char_count + arg_count) * sizeof(char));

    ret_str = (char*)(ret + arg_count);

    arg_count = 0;
    ret[arg_count] = ret_str;

    //
    //  Consume all spaces.  After this, we're either at
    //  the end of string, or we have an arg, and it
    //  might start with a quote
    //

    c = szCmdLine;
    while (*c == ' ') c++;
    if (*c == '"') {
        break_char = '"';
        c++;
    }

    while (*c != '\0') {
        if (*c == break_char) {
            *ret_str = '\0';
            ret_str++;

            break_char = ' ';
            c++;
            while (*c == break_char) c++;
            if (*c == '"') {
                break_char = '"';
                c++;
            }
            if (*c != '\0') {
                arg_count++;
                ret[arg_count] = ret_str;
            }
        } else {
            *ret_str = *c;
            ret_str++;

            //
            //  If we hit a break char, we count the argument then.
            //  If we hit end of string, count it here; note we're
            //  only counting it if we counted a character before it
            //  (ie., trailing whitespace is not an arg.)
            //

            c++;

            if (*c == '\0') {
                *ret_str = '\0';
            }
        }
    }


    return ret;
}

int WINAPI mainCRTStartup(void)
{
    char **szArglist;
    int nArgs;
    int i;

    wchar_t *u16cmdline = GetCommandLineW();

    // Max bytes needed when converting from utf-16 to utf-8.
    // https://stackoverflow.com/questions/55056322/maximum-utf-8-string-size-given-utf-16-size
    size_t required_size = wcslen(u16cmdline) * 4;

    // Cannot blow the stack because of limited lenght of windows cmd line :
    // https://stackoverflow.com/questions/3205027/maximum-length-of-command-line-string
    char buffer[required_size];

    int size = WideCharToMultiByte(CP_UTF8, 0, u16cmdline, -1, buffer,required_size, NULL, NULL);
    exitif(!size, GetLastError(), "Error converting command line to utf-8. Are you using some ungodly character?");

    szArglist = parseCommandLine(buffer, &nArgs);

    if( NULL == szArglist )
    {
        return 0;
    }
    else {
        i = main(nArgs, szArglist);
    }

    LocalFree(szArglist);

    return(i);
}

// SYSTEM WINDOWS FUNCTIONS {{{1

LASTS_FDEF __attribute__((noreturn,unused))
void lasts_exit(int status)
{
    ExitProcess(status);
}

#define O_RDONLY            GENERIC_READ
#define O_WRONLY            GENERIC_WRITE
#define O_RDWR              GENERIC_READ | GENERIC_WRITE
#define O_CREAT          0x40
#define O_EXCL           0x80
#define O_NOCTTY        0x100
#define O_TRUNC         0x200
#define O_APPEND        0x400
#define O_NONBLOCK      0x800
#define O_DIRECTORY   0x10000

#ifdef NONE
LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open(const char *path, int flags, mode_t mode) {
    size_t utf8Len = strlen(path) + 1; // +1 for terminating null
    // The worst case scenario is for each byte to be a separate codepoint, which takes 2 bytes in u16.
    size_t utf16Len = utf8Len * 2;
    wchar_t buffer[utf16Len];

    int result = MultiByteToWideChar(
                     CP_UTF8,                // convert from UTF-8
                     MB_ERR_INVALID_CHARS,   // error on invalid chars
                     path,                   // source UTF-8 string
                     utf8Len,                // total length of source UTF-8 string,
                     // in CHAR's (= bytes), including end-of-string \0
                     buffer,                 // destination buffer
                     utf16Len                // size of destination buffer, in WCHAR's
                 );

    exitif(!result, GetLastError(), "Error converting file name from utf-8 to utf-16.");

    HANDLE fd = CreateFileW(
                    buffer,
                    flags,

}

#endif

#define RET_BOOL_TO_INT(result) if(result) return 0; else return -1

LASTS_FDEF __attribute__((unused))
ssize_t lasts_write(LASTS_FD fd, const void *buf, size_t count)
{
    DWORD written;
    HANDLE out;
    if(fd == LASTS_STDOUT) {
        SetConsoleOutputCP(CP_UTF8);
        out = GetStdHandle(STD_OUTPUT_HANDLE);
    } else {
        out = (HANDLE)fd;
    }
    BOOL result = WriteFile(out, buf, count, &written, NULL);
    RET_BOOL_TO_INT(result);
}

LASTS_FDEF __attribute__((unused))
ssize_t lasts_read(LASTS_FD fd, void *buf, size_t count)
{
    DWORD read;
    HANDLE in;
    if(fd == LASTS_STDIN) {
        SetConsoleCP(CP_UTF8);
        in = GetStdHandle(STD_INPUT_HANDLE);
    } else {
        in = (HANDLE)fd;
    }
    BOOL result = ReadFile(in, buf, count, &read, NULL);
    RET_BOOL_TO_INT(result);
}


LASTS_FDEF __attribute__((unused))
int lasts_close(LASTS_FD fd) {
    BOOL result = CloseHandle((HANDLE)fd);
    RET_BOOL_TO_INT(result);
}


LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_win_open_gen(const char* file, DWORD desired, DWORD shared, DWORD dispo) {

    size_t utf8Len = strlen(file) + 1; // +1 for terminating null
    // The worst case scenario is for each byte to be a separate codepoint, which takes 2 bytes in u16.
    size_t utf16Len = utf8Len * 2;
    wchar_t buffer[utf16Len];

    int result = MultiByteToWideChar(
                     CP_UTF8,                // convert from UTF-8
                     MB_ERR_INVALID_CHARS,   // error on invalid chars
                     file,                   // source UTF-8 string
                     utf8Len,                // total length of source UTF-8 string,
                     // in CHAR's (= bytes), including end-of-string \0
                     buffer,                 // destination buffer
                     utf16Len                // size of destination buffer, in WCHAR's
                 );

    if(!result) return -1;

    HANDLE hFile = CreateFileW(buffer,        // file to open
                               desired,          // open for reading
                               shared,       // share for reading
                               NULL,                  // default security
                               dispo,         // existing file only
                               FILE_ATTRIBUTE_NORMAL, // normal file
                               NULL);                 // no attr. template

    if(hFile == INVALID_HANDLE_VALUE) return -1;
    else return (LASTS_FD)hFile;
}
LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_read(const char* file) {
    return lasts_win_open_gen(file, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_write(const char* file) {
    return lasts_win_open_gen(file, GENERIC_WRITE, 0, CREATE_ALWAYS);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_readwrite(const char* file) {
    return lasts_win_open_gen(file, GENERIC_READ | GENERIC_WRITE, 0, CREATE_ALWAYS);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_append(const char* file) {
    return lasts_win_open_gen(file, FILE_APPEND_DATA, FILE_SHARE_READ, OPEN_ALWAYS);
}

LASTS_FDEF __attribute__((unused))
LASTS_FD lasts_open_trunc(const char* file) {
    return lasts_win_open_gen(file, GENERIC_WRITE, 0, TRUNCATE_EXISTING | OPEN_ALWAYS);
}
#endif

#ifdef __cplusplus
}
#endif
