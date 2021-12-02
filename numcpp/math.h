#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <cmath>

namespace numcpp {

    using std::cos;
    using std::sin;
    using std::tan;
    using std::hypot;
    using std::acos;
    using std::asin;
    using std::atan;
    using std::atan2;

    using std::cosh;
    using std::sinh;
    using std::tanh;
    using std::acosh;
    using std::asinh;
    using std::atanh;

    using std::exp;
    using std::exp2;
    using std::log;
    using std::log2;
    using std::log10;

    using std::pow;
    using std::sqrt;
    using std::cbrt;

    using std::ceil;
    using std::floor;
    using std::round;
    using std::trunc;

    using std::abs;
    using std::min;
    using std::max;

    using std::isfinite;
    using std::isinf;
    using std::isnan;
    using std::nan;

    // This namespace contains several constants.
    const double inf = HUGE_VAL;
    const double infty = inf;
    const double infinity = inf;

    const double e = exp(1.0);
    const double pi = acos(-1.0);
    const double golden = (1.0 + sqrt(5.0)) / 2.0;

    // Returns an array/matrix containing the absolute values of all the
    // elements of x, in the same order.
    template <class T>
    array<T> abs(const array<T> &x);

    template <class T>
    matrix<T> abs(const matrix<T> &x);

    // Returns an array/matrix containing the principal values of the arc cosine
    // of all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> acos(const array<T> &x);

    template <class T>
    matrix<T> acos(const matrix<T> &x);

    // Returns an array/matrix containing the area hyperbolic cosines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> acosh(const array<T> &x);

    template <class T>
    matrix<T> acosh(const matrix<T> &x);

    // Returns an array/matrix containing the principal values of the arc sine
    // of all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> asin(const array<T> &x);

    template <class T>
    matrix<T> asin(const matrix<T> &x);

    // Returns an array/matrix containing the area hyperbolic sines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> asinh(const array<T> &x);

    template <class T>
    matrix<T> asinh(const matrix<T> &x);

    // Returns an array/matrix containing the principal values of the arc
    // tangent of all the elements of x, expressed in radians, in the same
    // order.
    template <class T>
    array<T> atan(const array<T> &x);

    template <class T>
    matrix<T> atan(const matrix<T> &x);

    // Returns an array/matrix containing the principal value of the arc
    // tangent of all the elements, in the same order. The tangent for which it
    // is calculated is the quotient of coordinates y/x, using their sign to
    // determine the appropriate quadrant.
    template <class T>
    array<T> atan2(const array<T> &y, const array<T> &x);

    template <class T>
    array<T> atan2(const array<T> &y, const T &x);

    template <class T>
    array<T> atan2(const T &y, const array<T> &x);

    template <class T>
    matrix<T> atan2(const matrix<T> &y, const matrix<T> &x);

    template <class T>
    matrix<T> atan2(const matrix<T> &y, const T &x);

    template <class T>
    matrix<T> atan2(const T &y, const matrix<T> &x);

    // Returns an array/matrix containing the area hyperbolic tangents of all
    // the elements of x, in the same order.
    template <class T>
    array<T> atanh(const array<T> &x);

    template <class T>
    matrix<T> atanh(const matrix<T> &x);

    // Returns an array/matrix containing the cubic root of all the elements of
    // x, in the same order.
    template <class T>
    array<T> cbrt(const array<T> &x);

    template <class T>
    matrix<T> cbrt(const matrix<T> &x);

    // Returns an array/matrix containing the results of rounding upward all the
    // elements of x, in the same order.
    template <class T>
    array<T> ceil(const array<T> &x);

    template <class T>
    matrix<T> ceil(const matrix<T> &x);

    // Returns an array/matrix containing the cosines of all the elements of x,
    // in the same order.
    template <class T>
    array<T> cos(const array<T> &x);

    template <class T>
    matrix<T> cos(const matrix<T> &x);

    // Returns an array/matrix containing the hyperbolic cosines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> cosh(const array<T> &x);

    template <class T>
    matrix<T> cosh(const matrix<T> &x);

    // Convert angles from radians to degrees.
    double degrees(double x);

    float degrees(float x);

    long double degrees(long double x);

    template <class T>
    double degrees(T x);

    // Returns an array/matrix containing the angles on degrees.
    template <class T>
    array<T> degrees(const array<T> &x);

    template <class T>
    matrix<T> degrees(const matrix<T> &x);

    // Returns an array/matrix containing the exponential function values of all
    // the elements of x, in the same order.
    template <class T>
    array<T> exp(const array<T> &x);

    template <class T>
    matrix<T> exp(const matrix<T> &x);

    // Returns an array/matrix containing the base-2 exponential function values
    // of all the elements of x, in the same order.
    template <class T>
    array<T> exp2(const array<T> &x);

    template <class T>
    matrix<T> exp2(const matrix<T> &x);

    // Returns an array/matrix containing the results of rounding downward all
    // the elements of x, in the same order.
    template <class T>
    array<T> floor(const array<T> &x);

    template <class T>
    matrix<T> floor(const matrix<T> &x);

    // Returns the greatest common divisor of the integers m and n.
    int gcd(int m, int n);

    long gcd(long m, long n);

    long long gcd(long long m, long long n);

    unsigned gcd(unsigned m, unsigned n);

    unsigned long gcd(unsigned long m, unsigned long n);

    unsigned long long gcd(unsigned long long m, unsigned long long n);

    // Returns the element-wise greatest common divisor.
    template <class T>
    array<T> gcd(const array<T> &m, const array<T> &n);

    template <class T>
    array<T> gcd(const array<T> &m, const T &n);

    template <class T>
    array<T> gcd(const T &m, const array<T> &n);

    template <class T>
    matrix<T> gcd(const matrix<T> &m, const matrix<T> &n);

    template <class T>
    matrix<T> gcd(const matrix<T> &m, const T &n);

    template <class T>
    matrix<T> gcd(const T &m, const matrix<T> &n);

    // Returns the hypotenuse of a right-angled triangle whose legs are x and y.
    template <class T>
    array<T> hypot(const array<T> &x, const array<T> &y);

    template <class T>
    array<T> hypot(const array<T> &x, const T &y);

    template <class T>
    array<T> hypot(const T &x, const array<T> &y);

    template <class T>
    matrix<T> hypot(const matrix<T> &x, const matrix<T> &y);

    template <class T>
    matrix<T> hypot(const matrix<T> &x, const T &y);

    template <class T>
    matrix<T> hypot(const T &x, const matrix<T> &y);

    // Returns the lowest common multiple of the integers m and n.
    int lcm(int m, int n);

    long lcm(long m, long n);

    long long lcm(long long m, long long n);

    unsigned lcm(unsigned m, unsigned n);

    unsigned long lcm(unsigned long m, unsigned long n);

    unsigned long long lcm(unsigned long long m, unsigned long long n);

    // Returns the element-wise lowest common multiple.
    template <class T>
    array<T> lcm(const array<T> &m, const array<T> &n);

    template <class T>
    array<T> lcm(const array<T> &m, const T &n);

    template <class T>
    array<T> lcm(const T &m, const array<T> &n);

    template <class T>
    matrix<T> lcm(const matrix<T> &m, const matrix<T> &n);

    template <class T>
    matrix<T> lcm(const matrix<T> &m, const T &n);

    template <class T>
    matrix<T> lcm(const T &m, const matrix<T> &n);

    // Returns an array/matrix containing the natural logarithm (base-e
    // logarithm) of all the elements of x, in the same order.
    template <class T>
    array<T> log(const array<T> &x);

    template <class T>
    matrix<T> log(const matrix<T> &x);

    // Returns an array/matrix containing the binary logarithm (base-2
    // logarithm) of all the elements of x, in the same order.
    template <class T>
    array<T> log2(const array<T> &x);

    template <class T>
    matrix<T> log2(const matrix<T> &x);

    // Returns an array/matrix containing the common logarithm (base-10
    // logarithm) of all the elements of x, in the same order.
    template <class T>
    array<T> log10(const array<T> &x);

    template <class T>
    matrix<T> log10(const matrix<T> &x);

    // Return element-wise maximum.
    template <class T>
    array<T> maximum(const array<T> &x, const array<T> &y);

    template <class T>
    array<T> maximum(const array<T> &x, const T &y);

    template <class T>
    array<T> maximum(const T &x, const array<T> &y);

    template <class T>
    matrix<T> maximum(const matrix<T> &x, const matrix<T> &y);

    template <class T>
    matrix<T> maximum(const matrix<T> &x, const T &y);

    template <class T>
    matrix<T> maximum(const T &x, const matrix<T> &y);

    // Return element-wise minimum.
    template <class T>
    array<T> minimum(const array<T> &x, const array<T> &y);

    template <class T>
    array<T> minimum(const array<T> &x, const T &y);

    template <class T>
    array<T> minimum(const T &x, const array<T> &y);

    template <class T>
    matrix<T> minimum(const matrix<T> &x, const matrix<T> &y);

    template <class T>
    matrix<T> minimum(const matrix<T> &x, const T &y);

    template <class T>
    matrix<T> minimum(const T &x, const matrix<T> &y);

    // Returns an array/matrix containing the results of the power operation on
    // all the elements, in the same order. The results calculated are x raised
    // to the power y.
    template <class T>
    array<T> pow(const array<T> &x, const array<T> &y);

    template <class T>
    array<T> pow(const array<T> &x, const T &y);

    template <class T>
    array<T> pow(const T &x, const array<T> &y);

    template <class T>
    matrix<T> pow(const matrix<T> &x, const matrix<T> &y);

    template <class T>
    matrix<T> pow(const matrix<T> &x, const T &y);

    template <class T>
    matrix<T> pow(const T &x, const matrix<T> &y);

    // Convert angles from degrees to radians.
    double radians(double x);

    float radians(float x);

    long double radians(long double x);

    template <class T>
    double radians(T x);

    // Returns an array/matrix containing the angles on radians.
    template <class T>
    array<T> radians(const array<T> &x);

    template <class T>
    matrix<T> radians(const matrix<T> &x);

    // Returns an array/matrix containing the results of rounding all the
    // elements of x toward the nearest integer value, in the same order.
    template <class T>
    array<T> round(const array<T> &x);

    template <class T>
    matrix<T> round(const matrix<T> &x);

    // Rounds to the given number of decimal places.
    double round(double x, size_t decimals);

    float round(float x, size_t decimals);

    long double round(long double x, size_t decimals);

    // Returns an array/matrix containing the results of rounding all the
    // elements of x to the given number of decimal places.
    template <class T>
    array<T> round(const array<T> &x, size_t decimals);

    template <class T>
    matrix<T> round(const matrix<T> &x, size_t decimals);

    // The sign function returns -1 if x < 0, 0 if x == 0 and 1 if x > 0.
    template <class T>
    int sign(T x);

    // Return an element-wise indication of the sign of a number.
    template <class T>
    array<int> sign(const array<T> &x);

    template <class T>
    matrix<int> sign(const matrix<T> &x);

    // Returns an array/matrix containing the sines of all the elements of x,
    // in the same order.
    template <class T>
    array<T> sin(const array<T> &x);

    template <class T>
    matrix<T> sin(const matrix<T> &x);

    // Returns an array/matrix containing the hyperbolic sines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> sinh(const array<T> &x);

    template <class T>
    matrix<T> sinh(const matrix<T> &x);

    // Returns an array/matrix containing the square root of all the elements
    // of x, in the same order.
    template <class T>
    array<T> sqrt(const array<T> &x);

    template <class T>
    matrix<T> sqrt(const matrix<T> &x);

    // Returns the square of a number.
    template <class T>
    T square(T x);

    // Returns an array/matrix containing the squares of all the elements of x,
    // in the same order.
    template <class T>
    array<T> square(const array<T> &x);

    template <class T>
    matrix<T> square(const matrix<T> &x);

    // Returns an array/matrix containing the tangents of all the elements of x,
    // in the same order.
    template <class T>
    array<T> tan(const array<T> &x);

    template <class T>
    matrix<T> tan(const matrix<T> &x);

    // Returns an array/matrix containing the hyperbolic tangents of all the
    // elements of x, in the same order.
    template <class T>
    array<T> tanh(const array<T> &x);

    template <class T>
    matrix<T> tanh(const matrix<T> &x);

    // Returns an array/matrix containing the results of rounding all the
    // elements of x toward zero, in the same order.
    template <class T>
    array<T> trunc(const array<T> &x);

    template <class T>
    matrix<T> trunc(const matrix<T> &x);
}

#endif // MATH_H_INCLUDED
