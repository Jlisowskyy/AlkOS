#include "defines.hpp"
#include "math.h"
#include "stdio.h"
#include "string.h"

#include <stdarg.h>
#include <stdint.h>

static size_t GetDecimal(char **str);
static void ReverseString(char *str, size_t len);
static size_t RemoveTrailingZeros(char *str);
static void ShiftStringRight(char *str);
static int RoundFormattedDoubleDecimal(char *str, bool scientific);
static bool RoundFormattedDoubleHex(char *str, char lower);

static double ipow(unsigned int base, int n);
static int ilog(double num, unsigned int base);
static bool IsNegative(double num);
static bool IsSubnormal(double num);

static char *FormatHex(uintmax_t num, char *str, int lower, bool prefix, int precision, bool zeroPadding, int width);
static char *FormatOct(uintmax_t num, char *str, bool prefix, int precision, bool zeroPadding, int width);
static char *FormatUInt(uintmax_t num, char *str);

template <bool scientific>
static char *FormatDouble(
    double num, char *str, unsigned int precision, char lower, bool trailZeros, bool hashForm,
    bool significantDigits = false
);
static char *FormatDoubleHex(
    double num, char *str, int precision, char lower, bool trailZeros, bool hashForm, bool zeroPadding, int width
);

// ------------------------------------------------------------------------------------

static constexpr int IS_LOWER                 = 0x20; // If char is lowercase the 0x20 bit is set
static constexpr int BUFFER_SIZE              = 1024;
static constexpr int DOUBLE_PRECISION_DIG     = 6;
static constexpr int DOUBLE_HEX_PRECISION_DIG = 13;
static constexpr double INTEGRAL_PRECISION    = 1e-4;
static constexpr double FACTORIAL_PRECISION   = 1e-16;

enum class LengthModifier
{
    None = '\0',
    H    = 'H', // hh
    h    = 'h', // h
    L    = 'L', // ll
    l    = 'l', // l
    z    = 'z'  // z
};

FAST_CALL static void PutChar(char *str, int &i, size_t size, char c)
{
    if (size > 0)
    {
        str[i < static_cast<int>(size) ? i : size - 1] = c;
        ++i;
    }
}

int snprintf(char *str, size_t size, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int ret = vsnprintf(str, size, format, args);
    va_end(args);
    return ret;
}

int vsnprintf(char *str, size_t size, const char *format, va_list va)
{
    int i = 0;
    char buf[BUFFER_SIZE]; // TODO: Calculate buffer size
    const char *prefix;
    char *data;
    int width, precision;
    size_t conversionLength = 0;
    bool reversePadding = false, zeroPadding = false, isHash = false, isSpace = false, isPlus = false;
    auto lengthModifier = LengthModifier::None;

    // Iterate over the format string
    for (auto iter = const_cast<char *>(format);; ++iter)
    {
        if (*iter == '\0')
        {
            if (size > 0)
                str[i < static_cast<int>(size) ? i : size - 1] = '\0';
            return i;
        }

        if (*iter != '%')
        {
            PutChar(str, i, size, *iter);
            continue;
        }

        width          = -1;
        precision      = -1;
        prefix         = "";
        reversePadding = false;
        zeroPadding    = false;
        isHash         = false;
        isSpace        = false;
        isPlus         = false;
        lengthModifier = LengthModifier::None;
        data           = buf;

        ++iter;

        // Parse format specifiers
        while (true)
        {
            switch (*iter)
            {
            case '\0':
                return -1;

            case '%':
                str[i++] = '%';
                break;

            case '-':
                reversePadding = true;
                ++iter;
                continue;

            case '.':
                precision = 0;
                ++iter;
                continue;

            case '0':
                if (width < 0 && precision < 0 && !zeroPadding && !reversePadding)
                {
                    zeroPadding = true;
                    ++iter;
                    continue;
                }
                [[fallthrough]];
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (precision >= 0)
                    precision = static_cast<int>(GetDecimal(&iter));
                else
                    width = static_cast<int>(GetDecimal(&iter));
                continue;

            case '*':
            {
                if (precision >= 0)
                {
                    precision = va_arg(va, int);
                }
                else
                {
                    width = va_arg(va, int);
                    if (width < 0)
                    {
                        width *= -1;
                        reversePadding = true;
                    }
                }
                ++iter;
                continue;
            }

            case '+':
                isPlus = true;
                ++iter;
                continue;

            case ' ':
                isSpace = true;
                ++iter;
                continue;

            case '#':
                isHash = true;
                ++iter;
                continue;

            case 'h':
            case 'l':
            case 'z':
                if (static_cast<LengthModifier>(*iter) == LengthModifier::h && lengthModifier == LengthModifier::h)
                {
                    lengthModifier = LengthModifier::H;
                }
                else if (static_cast<LengthModifier>(*iter) == LengthModifier::l && lengthModifier == LengthModifier::l)
                {
                    lengthModifier = LengthModifier::L;
                }
                else if (lengthModifier == LengthModifier::None)
                {
                    lengthModifier = static_cast<LengthModifier>(*iter);
                }
                else
                {
                    PutChar(str, i, size, '%');
                    PutChar(str, i, size, *iter);
                    break;
                }
                ++iter;
                continue;

            case 'd':
            case 'i':
            case 'u':
            {
                uintmax_t d;

                if (lengthModifier == LengthModifier::z)
                {
                    if (*iter == 'u')
                        d = va_arg(va, uintmax_t);
                    else
                        d = va_arg(va, intmax_t);
                }
                else if (lengthModifier == LengthModifier::L)
                {
                    if (*iter == 'u')
                        d = va_arg(va, unsigned long long);
                    else
                        d = va_arg(va, long long);
                }
                else if (lengthModifier == LengthModifier::l)
                {
                    if (*iter == 'u')
                        d = va_arg(va, unsigned long);
                    else
                        d = va_arg(va, long);
                }
                else if (lengthModifier == LengthModifier::h)
                {
                    if (*iter == 'u')
                        d = static_cast<unsigned short>(va_arg(va, unsigned int));
                    else
                        d = static_cast<short>(va_arg(va, int));
                }
                else if (lengthModifier == LengthModifier::H)
                {
                    if (*iter == 'u')
                        d = static_cast<unsigned char>(va_arg(va, unsigned int));
                    else
                        d = static_cast<char>(va_arg(va, int));
                }
                else if (*iter == 'u')
                {
                    d = va_arg(va, unsigned int);
                }
                else
                {
                    d = va_arg(va, int);
                }

                if (*iter != 'u' && static_cast<intmax_t>(d) < 0)
                {
                    d *= -1;
                    prefix = "-";
                    width--;
                }
                else if (isPlus)
                {
                    prefix = "+";
                    width--;
                }
                else if (isSpace)
                {
                    prefix = " ";
                    width--;
                }

                FormatUInt(d, data);
                conversionLength = strlen(data);

                break;
            }

            case 'p':
            case 'x':
            case 'X':
            {
                uintmax_t x;

                if (*iter == 'p')
                {
                    x = reinterpret_cast<uintptr_t>(va_arg(va, void *));
                    if (x == 0)
                    {
                        strcpy(data, "(nil)");
                        conversionLength = 5;
                        zeroPadding      = false;
                        precision        = 0;
                        break;
                    }
                    isHash = true;
                }
                else if (lengthModifier == LengthModifier::L)
                {
                    x = va_arg(va, unsigned long long);
                }
                else if (lengthModifier == LengthModifier::l)
                {
                    x = va_arg(va, unsigned long);
                }
                else if (lengthModifier == LengthModifier::h)
                {
                    x = static_cast<unsigned short>(va_arg(va, unsigned int));
                }
                else if (lengthModifier == LengthModifier::H)
                {
                    x = static_cast<unsigned char>(va_arg(va, unsigned int));
                }
                else
                {
                    x = va_arg(va, unsigned int);
                }

                if (precision >= 0)
                {
                    FormatHex(x, data, *iter & IS_LOWER, isHash, precision, true, -1);
                    zeroPadding = false;
                }
                else if (zeroPadding)
                {
                    FormatHex(x, data, *iter & IS_LOWER, isHash, -1, zeroPadding, width);
                    if (x != 0)
                        zeroPadding = false;
                }
                else
                    FormatHex(x, data, *iter & IS_LOWER, isHash, -1, false, -1);

                conversionLength = strlen(data);

                break;
            }

            case 'o':
            {
                uintmax_t x;

                if (lengthModifier == LengthModifier::L)
                {
                    x = va_arg(va, unsigned long long);
                }
                else if (lengthModifier == LengthModifier::l)
                {
                    x = va_arg(va, unsigned long);
                }
                else if (lengthModifier == LengthModifier::h)
                {
                    x = static_cast<unsigned short>(va_arg(va, unsigned int));
                }
                else if (lengthModifier == LengthModifier::H)
                {
                    x = static_cast<unsigned char>(va_arg(va, unsigned int));
                }
                else
                {
                    x = va_arg(va, unsigned int);
                }

                if (precision >= 0)
                {
                    FormatOct(x, data, isHash, precision, true, -1);
                    zeroPadding = false;
                }
                else if (zeroPadding)
                {
                    FormatOct(x, data, isHash, -1, zeroPadding, width);
                    if (x != 0)
                        zeroPadding = false;
                }
                else
                    FormatOct(x, data, isHash, -1, false, -1);

                conversionLength = strlen(data);

                break;
            }

            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
            case 'a':
            case 'A':
            {
                double f = va_arg(va, double);

                // f < 0 does not work for nan, checking sign bit instead
                if (IsNegative(f))
                {
                    f *= -1;
                    prefix = "-";
                    width--;
                }
                else if (isPlus)
                {
                    prefix = "+";
                    width--;
                }
                else if (isSpace)
                {
                    prefix = " ";
                    width--;
                }

                if (isnan(f))
                {
                    for (size_t j = 0; j < 3; ++j) data[j] = "NAN"[j] | (*iter & IS_LOWER);
                    conversionLength = 3;
                    zeroPadding      = false;
                    precision        = 0;
                    break;
                }

                if (isinf(f))
                {
                    for (size_t j = 0; j < 3; ++j) data[j] = "INF"[j] | (*iter & IS_LOWER);
                    conversionLength = 3;
                    zeroPadding      = false;
                    precision        = 0;
                    break;
                }

                switch (*iter | IS_LOWER)
                {
                case 'f':
                    FormatDouble<false>(
                        f, data, precision >= 0 ? precision : DOUBLE_PRECISION_DIG, *iter & IS_LOWER, false, isHash
                    );
                    break;

                case 'e':
                    FormatDouble<true>(
                        f, data, precision >= 0 ? precision : DOUBLE_PRECISION_DIG, *iter & IS_LOWER, false, isHash
                    );
                    break;

                case 'g':
                {
                    int e             = ilog(f, 10);
                    int num_precision = precision >= 0 ? precision : DOUBLE_PRECISION_DIG;
                    if ((e >= num_precision || e < -4) && (e != num_precision || e != 0))
                        FormatDouble<true>(f, data, num_precision, *iter & IS_LOWER, true, isHash, true);
                    else
                    {
                        FormatDouble<false>(f, data, num_precision, *iter & IS_LOWER, true, isHash, true);
                        if (zeroPadding)
                            precision = width;
                        else
                            precision = 0;
                    }

                    break;
                }

                case 'a':
                    FormatDoubleHex(f, data, precision, *iter & IS_LOWER, precision == -1, isHash, zeroPadding, width);
                    break;
                }

                conversionLength = strlen(data);

                break;
            }

            case 's':
            {
                char *s = va_arg(va, char *);

                if (s == nullptr)
                {
                    strcpy(data, "(null)");
                    conversionLength = 6;
                    zeroPadding      = false;
                    precision        = 0;
                    break;
                }

                conversionLength = strlen(data);
                if (precision >= 0 && conversionLength > static_cast<size_t>(precision))
                {
                    conversionLength = precision;
                }

                break;
            }

            case 'c':
            {
                char c = va_arg(va, int);

                buf[0]           = c;
                conversionLength = 1;
                break;
            }

            default:
                PutChar(str, i, size, '%');
                PutChar(str, i, size, *iter);
                break;

            } // switch

            break;
        }

        // Add padding and width with formatted data to the output buffer

        if (precision < 0 && zeroPadding)
            precision = width;

        width -= conversionLength;
        precision -= conversionLength;

        if (precision > 0)
            width -= precision;

        if (!reversePadding && !zeroPadding)
        {
            while (width-- > 0)
            {
                PutChar(str, i, size, ' ');
            }
        }

        while (*prefix)
        {
            PutChar(str, i, size, *prefix++);
        }

        while (precision-- > 0)
        {
            PutChar(str, i, size, '0');
        }

        while (conversionLength-- > 0)
        {
            PutChar(str, i, size, *data++);
        }

        while (width-- > 0)
        {
            PutChar(str, i, size, ' ');
        }
    }
}

/**
 *  String utilities
 */

static size_t GetDecimal(char **str)
{
    int i = 0;
    while (**str >= '0' && **str <= '9') i = i * 10 + *(*str)++ - '0';
    return i;
}

static void ReverseString(char *str, size_t len)
{
    if (!len)
        return;

    for (size_t i = 0, j = len - 1; i < j; i++, j--)
    {
        char tmp = str[i];
        str[i]   = str[j];
        str[j]   = tmp;
    }
}

static void ShiftStringRight(char *str)
{
    size_t len = strlen(str);
    for (size_t i = len + 1; i > 0; --i)
    {
        str[i] = str[i - 1];
    }
}

static int RoundFormattedDoubleDecimal(char *str, bool scientific)
{
    char *end = str + strlen(str) - 1;
    int carry = 0;

    // Check if the last character needs to round up
    if (*end >= '5')
    {
        carry = 1;
        for (char *ptr = end - 1; ptr >= str; --ptr)
        {
            if (*ptr == '.')
                continue;
            int digit = (*ptr - '0') + carry;
            *ptr      = '0' + (digit % 10);
            carry     = digit / 10;
            if (!carry)
                break;
        }
        if (carry)
        {
            if (!scientific)
                ShiftStringRight(str);
            str[0] = '1';
            end++;
        }
    }

    *end = '\0';

    // Return if carry propagated to exponent
    return carry;
}

static bool RoundFormattedDoubleHex(char *str, char lower)
{
    char *end = str + strlen(str) - 1;
    int carry = 0;

    // Check if the last character needs to round up
    if (*end == '8' || *end == '9' || *end >= ('A' | lower))
    {
        carry = 1;
        for (char *ptr = end - 1; ptr >= str; --ptr)
        {
            if (*ptr == '.')
                break;

            int digit = (*ptr >= '0' && *ptr <= '9') ? (*ptr - '0') : ((*ptr & ~32) - 'A' + 10); // Convert to 0-15
            digit += carry;

            if (digit < 16)
            {
                *ptr  = (digit < 10) ? ('0' + digit) : (('A' | lower) + digit - 10); // Convert back to char
                carry = 0;
                break;
            }
            else
            {
                *ptr  = '0'; // Rollover
                carry = 1;
            }
        }
    }

    *end = '\0';

    // Return if carry propagated to exponent
    return carry;
}

static size_t RemoveTrailingZeros(char *str)
{
    size_t i  = 0;
    char *end = str + strlen(str) - 1;
    while (end > str && *end == '0')
    {
        end--;
        i++;
    }
    *(end + 1) = '\0';
    return i;
}

/**
 *  Double formatting utilities
 */

static double ipow(unsigned int base, int n)
{
    int i    = 1;
    double p = 1.0, m;

    if (n < 0)
    {
        n = -n;
        m = 1.0 / base;
    }
    else
    {
        m = base;
    }

    for (; i <= n; i++)
    {
        p *= m;
    }

    return p;
}

static int ilog(double num, unsigned int base)
{
    int res = 0;

    if (num == 0.0)
    {
        return 0;
    }

    if (num < 0)
    {
        num *= -1;
    }

    if (num < 1)
    {
        while (num < 1)
        {
            num *= base;
            res--;
        }
    }
    else
    {
        while (num >= base)
        {
            num /= base;
            res++;
        }
    }

    return res;
}

FAST_CALL static bool IsNegative(double num)
{
    auto intdbl = reinterpret_cast<uint64_t *>(&num);
    return (*intdbl & (1ULL << 63)) != 0;
}

FAST_CALL static bool IsSubnormal(double num)
{
    auto intdbl         = reinterpret_cast<uint64_t *>(&num);
    const auto exponent = static_cast<short>(*intdbl >> 52 & 0x7FF);
    const auto mantisa  = *intdbl & (-1ULL >> 12);
    return exponent == 0 && mantisa != 0;
}

/**
 *  Integer formatting utilities
 */

static char *FormatHex(uintmax_t num, char *str, int lower, bool prefix, int precision, bool zeroPadding, int width)
{
    int i = 0;

    if (num == 0)
    {
        if (precision == -1)
            str[i++] = '0';
        else
        {
            for (int j = 0; j < precision; ++j) str[i++] = '0';
        }
        str[i] = '\0';

        return str;
    }

    for (; num; num >>= 4) str[i++] = "0123456789ABCDEF"[num & 15] | lower;

    if (zeroPadding)
    {
        int zeros = 0;
        if (precision != -1)
            zeros = precision - i;
        else
            zeros = width - i - ((prefix) ? 2 : 0);

        for (int j = 0; j < zeros; ++j) str[i++] = '0';
    }

    if (prefix)
    {
        str[i++] = 'X' | lower;
        str[i++] = '0';
    }

    str[i] = '\0';

    ReverseString(str, i);

    return str;
}

static char *FormatOct(uintmax_t num, char *str, bool prefix, int precision, bool zeroPadding, int width)
{
    int i = 0;

    if (num == 0)
    {
        if (precision == -1)
            str[i++] = '0';
        else
        {
            for (int j = 0; j < precision; ++j) str[i++] = '0';
        }
        str[i] = '\0';

        return str;
    }

    for (; num; num >>= 3) str[i++] = '0' + (num & 7);

    if (zeroPadding)
    {
        int zeros = 0;
        if (precision != -1)
            zeros = precision - i - ((prefix) ? 1 : 0);
        else
            zeros = width - i - ((prefix) ? 1 : 0);

        for (int j = 0; j < zeros; ++j) str[i++] = '0';
    }

    if (prefix)
    {
        str[i++] = '0';
    }

    str[i] = '\0';

    ReverseString(str, i);

    return str;
}

static char *FormatUInt(uintmax_t num, char *str)
{
    int i = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i]   = '\0';

        return str;
    }

    for (; num; num /= 10) str[i++] = '0' + num % 10;

    str[i] = '\0';

    ReverseString(str, i);

    return str;
}

/**
 *  Double formatting functions
 */

template <bool scientific>
static char *FormatDouble(
    double num, char *str, unsigned int precision, char lower, bool trailZeros, bool hashForm, bool significantDigits
)
{
    size_t i                = 0;
    int exp                 = 0; // Exponent
    int subnormalCorrection = 0;
    bool foundNonZero       = false;
    size_t significantCount = 0;

    if (significantDigits && precision == 0)
        precision = 1;

    // Normalize
    if (scientific)
    {
        if (IsSubnormal(num))
        {
            subnormalCorrection = -(ilog(num, 10) + 308);
            num *= ipow(10, subnormalCorrection);
            exp = -308;
        }
        else
            exp = ilog(num, 10);

        if (exp < 0)
            num *= ipow(10, -exp);
        else
            num /= ipow(10, exp);
    }

    double integralPart;
    double fractionalPart = modf(num, &integralPart);

    if (scientific)
    {
        int digit = static_cast<char>((modf(num / 10, &integralPart) + INTEGRAL_PRECISION) * 10);
        str[i++]  = '0' + digit;
    }
    else
    {
        if (integralPart == 0.0)
        {
            str[i++] = '0';
        }
        else
        {
            for (num = integralPart; num != 0.0;)
            {
                num /= 10;
                int digit = static_cast<char>((modf(num, &integralPart) + INTEGRAL_PRECISION) * 10);
                str[i++]  = '0' + digit;
                num       = integralPart;
            }
        }
    }

    // Reverse the integral part
    if (!scientific)
        ReverseString(str, i);

    if (significantDigits)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if (str[j] != '0' || foundNonZero)
            {
                foundNonZero = true;
                if (++significantCount == precision)
                    break;
            }
        }
    }

    // Add decimal point
    if (precision > 0 || hashForm)
    {
        str[i++] = '.';
    }

    // Fractional part
    for (size_t j = 0;; ++j)
    {
        char digit = static_cast<char>((fractionalPart + FACTORIAL_PRECISION) * 10);
        if (digit < 0 || digit > 9)
            break; // Underflow
        str[i++]       = '0' + digit;
        fractionalPart = (fractionalPart * 10) - static_cast<double>(digit);

        if (significantDigits)
        {
            if (digit != 0 || foundNonZero)
            {
                foundNonZero = true;
                if (++significantCount == precision + 1)
                    break;
            }
            else if (num == 0.0 && j >= precision - 1)
                break;
        }
        else if (j >= precision)
            break;
    }
    str[i] = '\0';

    int carry = RoundFormattedDoubleDecimal(str, scientific);
    i--;

    // Remove trailing zeros
    if (trailZeros)
    {
        size_t count = 0;
        if (!hashForm || str[i - 2] == '.')
            count = RemoveTrailingZeros(str);
        i -= count;

        if ((count == precision && !hashForm) || (!hashForm && str[i - 1] == '.'))
            i--;
    }

    if (carry)
        exp++;

    if (scientific)
    {
        str[i++] = 'E' | lower;

        if (exp < 0)
        {
            exp *= -1;
            str[i++] = '-';
        }
        else
        {
            str[i++] = '+';
        }

        if (exp < 10)
            str[i++] = '0';

        FormatUInt(exp + subnormalCorrection, str + i);

        return str;
    }
    else
    {
        str[i] = '\0';

        return str;
    }
}

static char *FormatDoubleHex(
    double num, char *str, int precision, char lower, bool trailZeros, bool hashForm, bool zeroPadding, int width
)
{
    size_t i = 0;
    int exp  = ilog(num, 2); // Exponent

    // TODO: Is it possible to remove this buffer?
    char temp[BUFFER_SIZE];

    // Add prefix
    str[i++] = '0';
    str[i++] = 'X' | lower;

    if (num == 0.0)
    {
        str[i++] = '0';
        if (hashForm || precision != -1)
            str[i++] = '.';
        while (precision-- > 0) str[i++] = '0';
        str[i++] = 'P' | lower;
        str[i++] = '+';
        str[i++] = '0';
        str[i]   = '\0';

        return str;
    }

    precision = precision != -1 ? precision : DOUBLE_HEX_PRECISION_DIG;

    i         = 0;
    temp[i++] = '1';

    // Add decimal point
    if (precision > 0 || hashForm)
        temp[i++] = '.';

    const size_t frac = i;

    // Normalize
    if (exp < 0.0)
        num *= ipow(2, -exp);
    else
        num /= ipow(2, exp);

    // Fractional part
    double integralPart;
    double fractionalPart = modf(num, &integralPart);
    for (int j = 0; j < precision + 1; ++j)
    {
        fractionalPart = modf(fractionalPart * 16, &integralPart);
        temp[i++]      = "0123456789ABCDEF"[static_cast<char>(integralPart) & 15] | lower;
    }
    temp[i] = '\0';

    bool carry = RoundFormattedDoubleHex(temp + frac, lower);
    --i;

    if (trailZeros)
    {
        size_t count = RemoveTrailingZeros(temp);
        i -= count;

        if (static_cast<int>(count) == precision && !hashForm)
            i--;
    }

    if (carry)
        exp++;

    temp[i++] = 'P' | lower;

    if (exp >= 0)
    {
        temp[i++] = '+';
    }
    else
    {
        exp *= -1;
        temp[i++] = '-';
    }

    int len = strlen(FormatUInt(exp, temp + i));

    // Add zero padding
    {
        size_t j;
        for (j = 2; zeroPadding && j < width - i - len; ++j) str[j] = '0';

        strcpy(str + j, temp);
    }

    return str;
}