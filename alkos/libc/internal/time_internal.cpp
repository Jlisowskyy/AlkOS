#include <todo.h>
#include <extensions/time.hpp>
#include <time_internal.hpp>

uint64_t __GetLocalTimezoneOffsetNs()
{
    TODO_TIMEZONES
    /* Hard coded UTC */
    static constexpr uint64_t kUctOffset = 1;

    return kNanosInSecond * kSecondsInHour * kUctOffset;
}

uint64_t __GetDstTimezoneOffsetNs()
{
    TODO_TIMEZONES
    /* Hard coded Poland */
    static constexpr uint64_t kPolandOffset = 1;

    return kNanosInSecond * kSecondsInHour * kPolandOffset;
}
