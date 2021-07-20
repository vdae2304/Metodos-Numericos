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

    // This namespace contains several constants.
    namespace constants {
        const long double inf = HUGE_VALL;
        const long double infty = inf;
        const long double infinity = inf;

        const long double e = exp(1.0L);
        const long double pi = acos(-1.0L);
    };

    // Returns an array containing the cosines of all the elements of x, in the
    // same order.
    template <class T>
    array<T> cos(const array<T> &x);

    // Returns an array containing the sines of all the elements of x, in the
    // same order.
    template <class T>
    array<T> sin(const array<T> &x);

    // Returns an array containing the tangents of all the elements of x, in
    // the same order.
    template <class T>
    array<T> tan(const array<T> &x);

    // Returns the hypotenuse of a right-angled triangle whose legs are x and y.
    template <class T>
    array<T> hypot(const array<T> &x, const array<T> &y);

    // Returns an array containing the principal values of the arc cosine of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> acos(const array<T> &x);

    // Returns an array containing the principal values of the arc sine of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> asin(const array<T> &x);

    // Returns an array containing the principal values of the arc tangent of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> atan(const array<T> &x);

    // Returns an array containing the principal value of the arc tangent of
    // all the elements, in the same order. The tangent for which it is
    // calculated is the quotient of coordinates y/x, using their sign to
    // determine the appropriate quadrant.
    template <class T>
    array<T> atan2(const array<T> &y, const array<T> &x);

    template <class T>
    array<T> atan2(const array<T> &y, const T &x);

    template <class T>
    array<T> atan2(const T &y, const array<T> &x);

    // Convert angles from radians to degrees.
    double degrees(double x);

    float degrees(float x);

    long double degrees(long double x);

    template <class T>
    double degrees(T x);

    // Returns an array containing the angles on degrees.
    template <class T>
    array<T> degrees(const array<T> &x);

    // Convert angles from degrees to radians.
    double radians(double x);

    float radians(float x);

    long double radians(long double x);

    template <class T>
    double radians(T x);

    // Returns an array containing the angles on radians.
    template <class T>
    array<T> radians(const array<T> &x);

    ////////////////////////////////////////////////////////////////////////////
    // Hyperbolic functions                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the hyperbolic cosines of all the elements
    // of x, in the same order.
    template <class T>
    array<T> cosh(const array<T> &x);

    // Returns an array containing the hyperbolic sines of all the elements
    // of x, in the same order.
    template <class T>
    array<T> sinh(const array<T> &x);

    // Returns an array containing the hyperbolic tangents of all the elements
    // of x, in the same order.
    template <class T>
    array<T> tanh(const array<T> &x);

    // Returns an array containing the area hyperbolic cosines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> acosh(const array<T> &x);

    // Returns an array containing the area hyperbolic sines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> asinh(const array<T> &x);

    // Returns an array containing the area hyperbolic tangents of all the
    // elements of x, in the same order.
    template <class T>
    array<T> atanh(const array<T> &x);

    ////////////////////////////////////////////////////////////////////////////
    // Exponents and logarithms                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the exponential function values of all the
    // elements of x, in the same order.
    template <class T>
    array<T> exp(const array<T> &x);

    // Returns an array containing the base-2 exponential function values of
    // all the elements of x, in the same order.
    template <class T>
    array<T> exp2(const array<T> &x);

    // Returns an array containing the natural logarithm (base-e logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log(const array<T> &x);

    // Returns an array containing the binary logarithm (base-2 logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log2(const array<T> &x);

    // Returns an array containing the common logarithm (base-10 logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log10(const array<T> &x);

    ////////////////////////////////////////////////////////////////////////////
    // Rounding                                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the results of rounding upward all the
    // elements of x, in the same order.
    template <class T>
    array<T> ceil(const array<T> &x);

    // Returns an array containing the results of rounding downward all the
    // elements of x, in the same order.
    template <class T>
    array<T> floor(const array<T> &x);

    // Returns an array containing the results of rounding all the elements of
    // x toward the nearest integer value, in the same order.
    template <class T>
    array<T> round(const array<T> &x);

    // Returns an array containing the results of rounding all the elements of
    // x toward zero, in the same order.
    template <class T>
    array<T> trunc(const array<T> &x);

    ////////////////////////////////////////////////////////////////////////////
    // Power functions                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the results of the power operation on all
    // the elements, in the same order. The results calculated are x raised to
    // the power y.
    template <class T>
    array<T> pow(const array<T> &x, const array<T> &y);

    template <class T>
    array<T> pow(const array<T> &x, const T &y);

    template <class T>
    array<T> pow(const T &x, const array<T> &y);

    // Returns the square of a number.
    template <class T>
    T square(T x);

    // Returns an array containing the squares of all the elements of x, in the
    // same order.
    template <class T>
    array<T> square(const array<T> &x);

    // Returns an array containing the square root of all the elements of x, in
    // the same order.
    template <class T>
    array<T> sqrt(const array<T> &x);

    // Returns an array containing the cubic root of all the elements of x, in
    // the same order.
    template <class T>
    array<T> cbrt(const array<T> &x);

    ////////////////////////////////////////////////////////////////////////////
    // Miscellaneous                                                          //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the absolute values of all the elements of
    // x, in the same order.
    template <class T>
    array<T> abs(const array<T> &x);

    // The sign function returns -1 if x < 0, 0 if x == 0 and 1 if x > 0.
    template <class T>
    T sign(T x);

    // Return an element-wise indication of the sign of a number.
    template <class T>
    array<T> sign(const array<T> &x);

    // Returns the greatest common divisor of the integers m and n.
    template <class T>
    T gcd(T m, T n);

    template <class T>
    array<T> gcd(const array<T> &m, const array<T> &n);

    // Returns the lowest common multiple of the integers m and n.
    template <class T>
    T lcm(T m, T n);

    template <class T>
    array<T> lcm(const array<T> &m, const array<T> &n);

    // Return element-wise maximum.
    template <class T>
    array<T> maximum(const array<T> &x, const array<T> &y);

    // Return element-wise minimum.
    template <class T>
    array<T> minimum(const array<T> &x, const array<T> &y);
}

#endif // MATH_H_INCLUDED
