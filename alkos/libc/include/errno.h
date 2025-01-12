#ifndef LIBC_INCLUDE_ERRNO_H_
#define LIBC_INCLUDE_ERRNO_H_

// ------------------------------
// Error type
// ------------------------------

typedef int error_t;

typedef error_t errno_t;

// ------------------------------
// Errno access
// ------------------------------

extern error_t __access_errno(void);
#define errno (*__access_errno())

// ------------------------------
// Error defines
// ------------------------------


#endif // LIBC_INCLUDE_ERRNO_H_
