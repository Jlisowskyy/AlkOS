#include <time.h>
#include <cmos/rtc.hpp>
#include <timers.hpp>

time_t QuerySystemTime()
{
    tm rtcTime = ReadRtcTime();
    return mktime(&rtcTime);
}
