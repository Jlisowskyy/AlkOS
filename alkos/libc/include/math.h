#ifndef LIBC_INCLUDE_MATH_H_
#define LIBC_INCLUDE_MATH_H_

/**********************************************
 * Constants
 **********************************************/

static constexpr double M_E        = 2.7182818284590452354;   // e
static constexpr double M_LOG2E    = 1.4426950408889634074;   // log2(e)
static constexpr double M_LOG10E   = 0.43429448190325182765;  // log10(e)
static constexpr double M_LN2      = 0.69314718055994530942;  // ln(2)
static constexpr double M_LN10     = 2.30258509299404568402;  // ln(10)
static constexpr double M_PI       = 3.14159265358979323846;  // pi
static constexpr double M_PI_2     = 1.57079632679489661923;  // pi/2
static constexpr double M_PI_4     = 0.78539816339744830962;  // pi/4
static constexpr double M_1_PI     = 0.31830988618379067154;  // 1/pi
static constexpr double M_2_PI     = 0.63661977236758134308;  // 2/pi
static constexpr double M_2_SQRTPI = 1.12837916709551257390;  // 2/sqrt(pi)
static constexpr double M_SQRT2    = 1.41421356237309504880;  // sqrt(2)
static constexpr double M_SQRT1_2  = 0.70710678118654752440;  // 1/sqrt(2)

/**********************************************
 * Math functions
 **********************************************/

double modf(double num, double *integralPart);
bool isnan(double num);
bool isinf(double num);
double fabs(double num);

#endif  // LIBC_INCLUDE_MATH_H_
