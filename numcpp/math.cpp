namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Trigonometric functions                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array/matrix containing the cosines of all the elements of x,
    // in the same order.
    template <class T>
    array<T> cos(const array<T> &x) {
        return apply(cos, x);
    }

    template <class T>
    matrix<T> cos(const matrix<T> &x) {
        return apply(cos, x);
    }

    // Returns an array/matrix containing the sines of all the elements of x,
    // in the same order.
    template <class T>
    array<T> sin(const array<T> &x) {
        return apply(sin, x);
    }

    template <class T>
    matrix<T> sin(const matrix<T> &x) {
        return apply(sin, x);
    }

    // Returns an array/matrix containing the tangents of all the elements of x,
    // in the same order.
    template <class T>
    array<T> tan(const array<T> &x) {
        return apply(tan, x);
    }

    template <class T>
    matrix<T> tan(const matrix<T> &x) {
        return apply(tan, x);
    }

    // Returns the hypotenuse of a right-angled triangle whose legs are x and y.
    template <class T>
    array<T> hypot(const array<T> &x, const array<T> &y) {
        return apply(hypot, x, y);
    }

    template <class T>
    array<T> hypot(const array<T> &x, const T &y) {
        return apply(hypot, x, y);
    }

    template <class T>
    array<T> hypot(const T &x, const array<T> &y) {
        return apply(hypot, x, y);
    }

    template <class T>
    matrix<T> hypot(const matrix<T> &x, const matrix<T> &y) {
        return apply(hypot, x, y);
    }

    template <class T>
    matrix<T> hypot(const matrix<T> &x, const T &y) {
        return apply(hypot, x, y);
    }

    template <class T>
    matrix<T> hypot(const T &x, const matrix<T> &y) {
        return apply(hypot, x, y);
    }

    // Returns an array/matrix containing the principal values of the arc cosine
    // of all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> acos(const array<T> &x) {
        return apply(acos, x);
    }

    template <class T>
    matrix<T> acos(const matrix<T> &x) {
        return apply(acos, x);
    }

    // Returns an array/matrix containing the principal values of the arc sine
    // of all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> asin(const array<T> &x) {
        return apply(asin, x);
    }

    template <class T>
    matrix<T> asin(const matrix<T> &x) {
        return apply(asin, x);
    }

    // Returns an array/matrix containing the principal values of the arc
    // tangent of all the elements of x, expressed in radians, in the same
    // order.
    template <class T>
    array<T> atan(const array<T> &x) {
        return apply(atan, x);
    }

    template <class T>
    matrix<T> atan(const matrix<T> &x) {
        return apply(atan, x);
    }

    // Returns an array/matrix containing the principal value of the arc
    // tangent of all the elements, in the same order. The tangent for which it
    // is calculated is the quotient of coordinates y/x, using their sign to
    // determine the appropriate quadrant.
    template <class T>
    array<T> atan2(const array<T> &y, const array<T> &x) {
        return apply(atan2, y, x);
    }

    template <class T>
    array<T> atan2(const array<T> &y, const T &x) {
        return apply(atan2, y, x);
    }

    template <class T>
    array<T> atan2(const T &y, const array<T> &x) {
        return apply(atan2, y, x);
    }

    template <class T>
    matrix<T> atan2(const matrix<T> &y, const matrix<T> &x) {
        return apply(atan2, y, x);
    }

    template <class T>
    matrix<T> atan2(const matrix<T> &y, const T &x) {
        return apply(atan2, y, x);
    }

    template <class T>
    matrix<T> atan2(const T &y, const matrix<T> &x) {
        return apply(atan2, y, x);
    }

    // Convert angles from radians to degrees.
    double degrees(double x) {
        return 180.0*x / pi;
    }

    float degrees(float x) {
        return 180.0f*x / pi;
    }

    long double degrees(long double x) {
        return 180.0L*x / pi;
    }

    template <class T>
    double degrees(T x) {
        return 180.0*x / pi;
    }

    // Returns an array/matrix containing the angles on degrees.
    template <class T>
    array<T> degrees(const array<T> &x) {
        return apply(degrees, x);
    }

    template <class T>
    matrix<T> degrees(const matrix<T> &x) {
        return apply(degrees, x);
    }

    // Convert angles from degrees to radians.
    double radians(double x) {
        return pi*x / 180.0;
    }

    float radians(float x) {
        return pi*x / 180.0f;
    }

    long double radians(long double x) {
        return pi*x / 180.0L;
    }

    template <class T>
    double radians(T x) {
        return pi*x / 180.0;
    }

    // Returns an array/matrix containing the angles on radians.
    template <class T>
    array<T> radians(const array<T> &x) {
        return apply(radians, x);
    }

    template <class T>
    matrix<T> radians(const matrix<T> &x) {
        return apply(radians, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Hyperbolic functions                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array/matrix containing the hyperbolic cosines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> cosh(const array<T> &x) {
        return apply(cosh, x);
    }

    template <class T>
    matrix<T> cosh(const matrix<T> &x) {
        return apply(cosh, x);
    }

    // Returns an array/matrix containing the hyperbolic sines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> sinh(const array<T> &x) {
        return apply(sinh, x);
    }

    template <class T>
    matrix<T> sinh(const matrix<T> &x) {
        return apply(sinh, x);
    }

    // Returns an array/matrix containing the hyperbolic tangents of all the
    // elements of x, in the same order.
    template <class T>
    array<T> tanh(const array<T> &x) {
        return apply(tanh, x);
    }

    template <class T>
    matrix<T> tanh(const matrix<T> &x) {
        return apply(tanh, x);
    }

    // Returns an array/matrix containing the area hyperbolic cosines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> acosh(const array<T> &x) {
        return apply(acosh, x);
    }

    template <class T>
    matrix<T> acosh(const matrix<T> &x) {
        return apply(acosh, x);
    }

    // Returns an array/matrix containing the area hyperbolic sines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> asinh(const array<T> &x) {
        return apply(asinh, x);
    }

    template <class T>
    matrix<T> asinh(const matrix<T> &x) {
        return apply(asinh, x);
    }

    // Returns an array/matrix containing the area hyperbolic tangents of all
    // the elements of x, in the same order.
    template <class T>
    array<T> atanh(const array<T> &x) {
        return apply(atanh, x);
    }

    template <class T>
    matrix<T> atanh(const matrix<T> &x) {
        return apply(atanh, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Exponents and logarithms                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array/matrix containing the exponential function values of all
    // the elements of x, in the same order.
    template <class T>
    array<T> exp(const array<T> &x) {
        return apply(exp, x);
    }

    template <class T>
    matrix<T> exp(const matrix<T> &x) {
        return apply(exp, x);
    }

    // Returns an array/matrix containing the base-2 exponential function values
    // of all the elements of x, in the same order.
    template <class T>
    array<T> exp2(const array<T> &x) {
        return apply(exp2, x);
    }

    template <class T>
    matrix<T> exp2(const matrix<T> &x) {
        return apply(exp2, x);
    }

    // Returns an array/matrix containing the natural logarithm (base-e
    // logarithm) of all the elements of x, in the same order.
    template <class T>
    array<T> log(const array<T> &x) {
        return apply(log, x);
    }

    template <class T>
    matrix<T> log(const matrix<T> &x) {
        return apply(log, x);
    }

    // Returns an array/matrix containing the binary logarithm (base-2
    // logarithm) of all the elements of x, in the same order.
    template <class T>
    array<T> log2(const array<T> &x) {
        return apply(log2, x);
    }

    template <class T>
    matrix<T> log2(const matrix<T> &x) {
        return apply(log2, x);
    }

    // Returns an array/matrix containing the common logarithm (base-10
    // logarithm) of all the elements of x, in the same order.
    template <class T>
    array<T> log10(const array<T> &x) {
        return apply(log10, x);
    }

    template <class T>
    matrix<T> log10(const matrix<T> &x) {
        return apply(log10, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Rounding                                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array/matrix containing the results of rounding upward all the
    // elements of x, in the same order.
    template <class T>
    array<T> ceil(const array<T> &x) {
        return apply(ceil, x);
    }

    template <class T>
    matrix<T> ceil(const matrix<T> &x) {
        return apply(ceil, x);
    }

    // Returns an array/matrix containing the results of rounding downward all
    // the elements of x, in the same order.
    template <class T>
    array<T> floor(const array<T> &x) {
        return apply(floor, x);
    }

    template <class T>
    matrix<T> floor(const matrix<T> &x) {
        return apply(floor, x);
    }

    // Returns an array/matrix containing the results of rounding all the
    // elements of x toward the nearest integer value, in the same order.
    template <class T>
    array<T> round(const array<T> &x) {
        return apply(round, x);
    }

    template <class T>
    matrix<T> round(const matrix<T> &x) {
        return apply(round, x);
    }

    // Returns an array/matrix containing the results of rounding all the
    // elements of x toward zero, in the same order.
    template <class T>
    array<T> trunc(const array<T> &x) {
        return apply(trunc, x);
    }

    template <class T>
    matrix<T> trunc(const matrix<T> &x) {
        return apply(trunc, x);
    }

    // Rounds to the given number of decimal places.
    double round(double x, size_t decimals) {
        return round(x*pow(10.0, decimals)) / pow(10.0, decimals);
    }

    float round(float x, size_t decimals) {
        return round(x*pow(10.0f, decimals)) / pow(10.0f, decimals);
    }

    long double round(long double x, size_t decimals) {
        return round(x*pow(10.0L, decimals)) / pow(10.0L, decimals);
    }

    // Returns an array/matrix containing the results of rounding all the
    // elements of x to the given number of decimal places.
    template <class T>
    array<T> round(const array<T> &x, size_t decimals) {
        return apply<T, T(T, size_t)>(round, x, decimals);
    }

    template <class T>
    matrix<T> round(const matrix<T> &x, size_t decimals) {
        return apply<T, T(T, size_t)>(round, x, decimals);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Power functions                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array/matrix containing the results of the power operation on
    // all the elements, in the same order. The results calculated are x raised
    // to the power y.
    template <class T>
    array<T> pow(const array<T> &x, const array<T> &y) {
        return apply(pow, x, y);
    }

    template <class T>
    array<T> pow(const array<T> &x, const T &y) {
        return apply(pow, x, y);
    }

    template <class T>
    array<T> pow(const T &x, const array<T> &y) {
        return apply(pow, x, y);
    }

    template <class T>
    matrix<T> pow(const matrix<T> &x, const matrix<T> &y) {
        return apply(pow, x, y);
    }

    template <class T>
    matrix<T> pow(const matrix<T> &x, const T &y) {
        return apply(pow, x, y);
    }

    template <class T>
    matrix<T> pow(const T &x, const matrix<T> &y) {
        return apply(pow, x, y);
    }

    // Returns the square of a number.
    template <class T>
    T square(T x) {
        return x*x;
    }

    // Returns an array/matrix containing the squares of all the elements of x,
    // in the same order.
    template <class T>
    array<T> square(const array<T> &x) {
        return apply(square, x);
    }

    template <class T>
    matrix<T> square(const matrix<T> &x) {
        return apply(square, x);
    }

    // Returns an array/matrix containing the square root of all the elements
    // of x, in the same order.
    template <class T>
    array<T> sqrt(const array<T> &x) {
        return apply(sqrt, x);
    }

    template <class T>
    matrix<T> sqrt(const matrix<T> &x) {
        return apply(sqrt, x);
    }

    // Returns an array/matrix containing the cubic root of all the elements of
    // x, in the same order.
    template <class T>
    array<T> cbrt(const array<T> &x) {
        return apply(cbrt, x);
    }

    template <class T>
    matrix<T> cbrt(const matrix<T> &x) {
        return apply(cbrt, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Miscellaneous                                                          //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array/matrix containing the absolute values of all the
    // elements of x, in the same order.
    template <class T>
    array<T> abs(const array<T> &x) {
        return apply(abs, x);
    }

    template <class T>
    matrix<T> abs(const matrix<T> &x) {
        return apply(abs, x);
    }

    // The sign function returns -1 if x < 0, 0 if x == 0 and 1 if x > 0.
    template <class T>
    int sign(T x) {
        if (x > 0) {
            return 1;
        }
        else if (x < 0) {
            return -1;
        }
        else {
            return 0;
        }
    }

    // Return an element-wise indication of the sign of a number.
    template <class T>
    array<int> sign(const array<T> &x) {
        return apply<T, int(T)>(sign, x);
    }

    template <class T>
    matrix<int> sign(const matrix<T> &x) {
        return apply<T, int(T)>(sign, x);
    }

    // Returns the greatest common divisor of the integers m and n.
    int gcd(int m, int n) {
        while (n != 0) {
            int mod = m % n;
            m = n;
            n = mod;
        }
        return abs(m);
    }

    long gcd(long m, long n) {
        while (n != 0) {
            long mod = m % n;
            m = n;
            n = mod;
        }
        return abs(m);
    }

    long long gcd(long long m, long long n) {
        while (n != 0) {
            long long mod = m % n;
            m = n;
            n = mod;
        }
        return abs(m);
    }

    unsigned gcd(unsigned m, unsigned n) {
        while (n != 0) {
            unsigned mod = m % n;
            m = n;
            n = mod;
        }
        return m;
    }

    unsigned long gcd(unsigned long m, unsigned long n) {
        while (n != 0) {
            unsigned long mod = m % n;
            m = n;
            n = mod;
        }
        return m;
    }

    unsigned long long gcd(unsigned long long m, unsigned long long n) {
        while (n != 0) {
            unsigned long long mod = m % n;
            m = n;
            n = mod;
        }
        return m;
    }

    // Returns the element-wise greatest common divisor.
    template <class T>
    array<T> gcd(const array<T> &m, const array<T> &n) {
        return apply(gcd, m, n);
    }

    template <class T>
    array<T> gcd(const array<T> &m, const T &n) {
        return apply(gcd, m, n);
    }

    template <class T>
    array<T> gcd(const T &m, const array<T> &n) {
        return apply(gcd, m, n);
    }

    template <class T>
    matrix<T> gcd(const matrix<T> &m, const matrix<T> &n) {
        return apply(gcd, m, n);
    }

    template <class T>
    matrix<T> gcd(const matrix<T> &m, const T &n) {
        return apply(gcd, m, n);
    }

    template <class T>
    matrix<T> gcd(const T &m, const matrix<T> &n) {
        return apply(gcd, m, n);
    }

    // Returns the lowest common multiple of the integers m and n.
    int lcm(int m, int n) {
        return abs(m * n) / gcd(m, n);
    }

    long lcm(long m, long n) {
        return abs(m * n) / gcd(m, n);
    }

    long long lcm(long long m, long long n) {
        return abs(m * n) / gcd(m, n);
    }

    unsigned lcm(unsigned m, unsigned n) {
        return (m * n) / gcd(m, n);
    }

    unsigned long lcm(unsigned long m, unsigned long n) {
        return (m * n) / gcd(m, n);
    }

    unsigned long long lcm(unsigned long long m, unsigned long long n) {
        return (m * n) / gcd(m, n);
    }

    // Returns the element-wise lowest common multiple.
    template <class T>
    array<T> lcm(const array<T> &m, const array<T> &n) {
        return apply(lcm, m, n);
    }

    template <class T>
    array<T> lcm(const array<T> &m, const T &n) {
        return apply(lcm, m, n);
    }

    template <class T>
    array<T> lcm(const T &m, const array<T> &n) {
        return apply(lcm, m, n);
    }

    template <class T>
    matrix<T> lcm(const matrix<T> &m, const matrix<T> &n) {
        return apply(lcm, m, n);
    }

    template <class T>
    matrix<T> lcm(const matrix<T> &m, const T &n) {
        return apply(lcm, m, n);
    }

    template <class T>
    matrix<T> lcm(const T &m, const matrix<T> &n) {
        return apply(lcm, m, n);
    }

    // Return element-wise maximum.
    template <class T>
    array<T> maximum(const array<T> &x, const array<T> &y) {
        return apply<T, const T&(const T&, const T&)>(max, x, y);
    }

    template <class T>
    array<T> maximum(const array<T> &x, const T &y) {
        return apply<T, const T&(const T&, const T&)>(max, x, y);
    }

    template <class T>
    array<T> maximum(const T &x, const array<T> &y) {
        return apply<T, const T&(const T&, const T&)>(max, x, y);
    }

    template <class T>
    matrix<T> maximum(const matrix<T> &x, const matrix<T> &y) {
        return apply<T, const T&(const T&, const T&)>(max, x, y);
    }

    template <class T>
    matrix<T> maximum(const matrix<T> &x, const T &y) {
        return apply<T, const T&(const T&, const T&)>(max, x, y);
    }

    template <class T>
    matrix<T> maximum(const T &x, const matrix<T> &y) {
        return apply<T, const T&(const T&, const T&)>(max, x, y);
    }

    // Return element-wise minimum.
    template <class T>
    array<T> minimum(const array<T> &x, const array<T> &y) {
        return apply<T, const T&(const T&, const T&)>(min, x, y);
    }

    template <class T>
    array<T> minimum(const array<T> &x, const T &y) {
        return apply<T, const T&(const T&, const T&)>(min, x, y);
    }

    template <class T>
    array<T> minimum(const T &x, const array<T> &y) {
        return apply<T, const T&(const T&, const T&)>(min, x, y);
    }

    template <class T>
    matrix<T> minimum(const matrix<T> &x, const matrix<T> &y) {
        return apply<T, const T&(const T&, const T&)>(min, x, y);
    }

    template <class T>
    matrix<T> minimum(const matrix<T> &x, const T &y) {
        return apply<T, const T&(const T&, const T&)>(min, x, y);
    }

    template <class T>
    matrix<T> minimum(const T &x, const matrix<T> &y) {
        return apply<T, const T&(const T&, const T&)>(min, x, y);
    }
}
