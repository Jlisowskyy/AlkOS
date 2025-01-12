/* internal includes */
#include <time.h>

double difftime(time_t time_end, time_t time_beg) {
}

time_t time(time_t *arg) {
}

clock_t clock() {
}

int timespec_get(struct timespec *ts, int base) {
}

int timespec_getres(struct timespec *ts, int base) {
}

char *asctime(const tm *time_ptr) {
}

errno_t asctime_s(char *buf, rsize_t bufsz, const tm *time_ptr) {
}

char *ctime(const time_t *timer) {
}

errno_t ctime_s(char *buf, rsize_t bufsz, const time_t *timer) {
}

size_t strftime(char *s, size_t max_size, const char *format, const tm *time_ptr) {
}

time_t mktime(struct tm *time_ptr) {
}

struct tm *localtime_s(const time_t *timer, struct tm *result) {
}

struct tm *localtime_r(const time_t *timer, struct tm *result) {
}

struct tm *localtime(const time_t *timer) {
}

struct tm *gmtime_s(const time_t *timer, struct tm *result) {
}

struct tm *gmtime_r(const time_t *timer, struct tm *result) {
}

struct tm *gmtime(const time_t *timer) {
}
