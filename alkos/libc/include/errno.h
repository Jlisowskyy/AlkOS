#ifndef LIBC_INCLUDE_ERRNO_H_
#define LIBC_INCLUDE_ERRNO_H_

#include <todo.h>

// ------------------------------
// Error type
// ------------------------------

typedef int error_t;

typedef error_t errno_t;

// ------------------------------
// Errno access
// ------------------------------

/* Errno should be thread local */
TODO_THREADING

extern error_t *__access_errno(void);

#define errno (*__access_errno())

// ------------------------------
// Error defines
// ------------------------------

/* ISO C standard errors */

#define __e(x) ((error_t)(x))
#define EDOM   __e(1)            /* Math argument out of domain of func */
#define EILSEQ __e(2)            /* Illegal byte sequence */
#define ERANGE __e(3)            /* Result too large */

/* POSIX errors */
/* source: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html */
#define E2BIG           __e(4)    /* Argument list too long */
#define EACCES          __e(5)    /* Permission denied */
#define EADDRINUSE      __e(6)    /* Address in use */
#define EADDRNOTAVAIL   __e(7)    /* Address not available */
#define EAFNOSUPPORT    __e(8)    /* Address family not supported */
#define EAGAIN          __e(9)    /* Resource unavailable, try again */
#define EALREADY        __e(10)   /* Connection already in progress */
#define EBADF           __e(11)   /* Bad file descriptor */
#define EBADMSG         __e(12)   /* Bad message */
#define EBUSY           __e(13)   /* Device or resource busy */
#define ECANCELED       __e(14)   /* Operation canceled */
#define ECHILD          __e(15)   /* No child processes */
#define ECONNABORTED    __e(16)   /* Connection aborted */


#endif // LIBC_INCLUDE_ERRNO_H_
