#include <time.h>
#include <stdio.h>
#include <assert.h>

[[deprecated]] char *asctime(const tm *time_ptr) {
}

errno_t asctime_s(char *buf, rsize_t bufsz, const tm *time_ptr) {
}
