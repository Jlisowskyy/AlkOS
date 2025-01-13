#include <time.h>
#include <stdio.h>

[[deprecated]] char *asctime(const tm *time_ptr) {
    static char buf[26];



    return buf;
}

errno_t asctime_s(char *buf, rsize_t bufsz, const tm *time_ptr) {
}