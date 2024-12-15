extern "C" void AvxFloatingPointTest(double a, double b);

void FloatExtensionTest() {
    /* simple float test */
    float f1 = 0.1f;
    float f2 = 0.2f;

    f1 = f1 + f2;

    /* simple double test */
    double d1 = 0.1;
    double d2 = 0.2;

    d2 = d1 + d2;

    /* simple long double test */
    long double ld1 = 0.1;
    long double ld2 = 0.2;

    ld2 = ld1 + ld2;

    /* final sum */
    long double sum = f1 + d2 + ld2;

    /* testing avx */
    AvxFloatingPointTest(d2, d1);
}
