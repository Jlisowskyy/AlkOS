/* internal includes */
#include <time.h>
#include <assert.h>

double difftime(const time_t time_end, const time_t time_beg) {
    return static_cast<double>(time_end - time_beg);
}

time_t time(time_t *arg) {
    assert(false && "Not implemented!");
    TODO_CLOCKS
    return time_t{};
}

clock_t clock() {
    assert(false && "Not implemented!");
    TODO_CLOCKS
    return clock_t{};
}

int timespec_get(struct timespec *ts, int base) {
    assert(false && "Not implemented!");
    TODO_CLOCKS
    return int{};
}

int timespec_getres(struct timespec *ts, int base) {
    assert(false && "Not implemented!");
    TODO_CLOCKS
    return int{};
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
