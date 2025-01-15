#include <assert.h>
#include <time.h>

tm *localtime_r(const time_t *timer, tm *result)
{
    assert(false && "Not implemented!");
    return nullptr;
}

tm *localtime(const time_t *timer)
{
    static tm buffer;
    return localtime_r(timer, &buffer);
}
