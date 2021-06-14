#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

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

    ////////////////////////////////////////////////////////////////////////////
    // Trigonometric functions                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the cosines of all the elements of x, in the
    // same order.
    template <class T>
    array<T> cos(const array<T> &x) {
        return apply(cos, x);
    }

    // Returns an array containing the sines of all the elements of x, in the
    // same order.
    template <class T>
    array<T> sin(const array<T> &x) {
        return apply(sin, x);
    }

    // Returns an array containing the tangents of all the elements of x, in
    // the same order.
    template <class T>
    array<T> tan(const array<T> &x) {
        return apply(tan, x);
    }

    // Returns the hypotenuse of a right-angled triangle whose legs are x and y.
    template <class T>
    array<T> hypot(const array<T> &x, const array<T> &y) {
        return apply(hypot, x, y);
    }

    // Returns an array containing the principal values of the arc cosine of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> acos(const array<T> &x) {
        return apply(acos, x);
    }

    // Returns an array containing the principal values of the arc sine of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> asin(const array<T> &x) {
        return apply(asin, x);
    }

    // Returns an array containing the principal values of the arc tangent of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> atan(const array<T> &x) {
        return apply(atan, x);
    }

    // Returns an array containing the principal value of the arc tangent of
    // all the elements, in the same order. The tangent for which it is
    // calculated is the quotient of coordinates y/x, using their sign to
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

    // Convert angles from radians to degrees.
    double degrees(double x) {
        return 180*x / constants::pi;
    }

    float degrees(float x) {
        return 180*x / constants::pi;
    }

    long double degrees(long double x) {
        return 180*x / constants::pi;
    }

    template <class T>
    double degrees(T x) {
        return 180*x / constants::pi;
    }

    template <class T>
    array<T> degrees(const array<T> &x) {
        return apply(degrees, x);
    }

    // Convert angles from degrees to radians.
    double radians(double x) {
        return constants::pi*x / 180;
    }

    float radians(float x) {
        return constants::pi*x / 180;
    }

    long double radians(long double x) {
        return constants::pi*x / 180;
    }

    template <class T>
    double radians(T x) {
        return constants::pi*x / 180;
    }

    template <class T>
    array<T> radians(const array<T> &x) {
        return apply(radians, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Hyperbolic functions                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the hyperbolic cosines of all the elements
    // of x, in the same order.
    template <class T>
    array<T> cosh(const array<T> &x) {
        return apply(cosh, x);
    }

    // Returns an array containing the hyperbolic sines of all the elements
    // of x, in the same order.
    template <class T>
    array<T> sinh(const array<T> &x) {
        return apply(sinh, x);
    }

    // Returns an array containing the hyperbolic tangents of all the elements
    // of x, in the same order.
    template <class T>
    array<T> tanh(const array<T> &x) {
        return apply(tanh, x);
    }

    // Returns an array containing the area hyperbolic cosines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> acosh(const array<T> &x) {
        return apply(acosh, x);
    }

    // Returns an array containing the area hyperbolic sines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> asinh(const array<T> &x) {
        return apply(asinh, x);
    }

    // Returns an array containing the area hyperbolic tangents of all the
    // elements of x, in the same order.
    template <class T>
    array<T> atanh(const array<T> &x) {
        return apply(atanh, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Exponents and logarithms                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the exponential function values of all the
    // elements of x, in the same order.
    template <class T>
    array<T> exp(const array<T> &x) {
        return apply(exp, x);
    }

    // Returns an array containing the base-2 exponential function values of
    // all the elements of x, in the same order.
    template <class T>
    array<T> exp2(const array<T> &x) {
        return apply(exp2, x);
    }

    // Returns an array containing the natural logarithm (base-e logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log(const array<T> &x) {
        return apply(log, x);
    }

    // Returns an array containing the binary logarithm (base-2 logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log2(const array<T> &x) {
        return apply(log2, x);
    }

    // Returns an array containing the common logarithm (base-10 logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log10(const array<T> &x) {
        return apply(log10, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Rounding                                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the results of rounding upward all the
    // elements of x, in the same order.
    template <class T>
    array<T> ceil(const array<T> &x) {
        return apply(ceil, x);
    }

    // Returns an array containing the results of rounding downward all the
    // elements of x, in the same order.
    template <class T>
    array<T> floor(const array<T> &x) {
        return apply(floor, x);
    }

    // Returns an array containing the results of rounding all the elements of
    // x toward the nearest integer value, in the same order.
    template <class T>
    array<T> round(const array<T> &x) {
        return apply(round, x);
    }

    // Returns an array containing the results of rounding all the elements of
    // x toward zero, in the same order.
    template <class T>
    array<T> trunc(const array<T> &x) {
        return apply(trunc, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Power functions                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the results of the power operation on all
    // the elements, in the same order. The results calculated are x raised to
    // the power y.
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

    // Returns the square of a number.
    template <class T>
    T square(T x) {
        return x*x;
    }

    // Returns an array containing the squares of all the elements of x, in the
    // same order.
    template <class T>
    array<T> square(const array<T> &x) {
        return apply(square, x);
    }

    // Returns an array containing the square root of all the elements of x, in
    // the same order.
    template <class T>
    array<T> sqrt(const array<T> &x) {
        return apply(sqrt, x);
    }

    // Returns an array containing the cubic root of all the elements of x, in
    // the same order.
    template <class T>
    array<T> cbrt(const array<T> &x) {
        return apply(cbrt, x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Miscellaneous                                                          //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array containing the absolute values of all the elements of
    // x, in the same order.
    template <class T>
    array<T> abs(const array<T> &x) {
        return apply(abs, x);
    }

    // The sign function returns -1 if x < 0, 0 if x == 0 and 1 if x > 0.
    template <class T>
    T sign(T x) {
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
    array<T> sign(const array<T> &x) {
        return apply(sign, x);
    }

    // Return element-wise maximum.
    template <class T>
    array<T> maximum(const array<T> &x, const array<T> &y) {
        return apply([](T x, T y) { return max(x, y); }, x, y);
    }

    // Return element-wise minimum.
    template <class T>
    array<T> minimum(const array<T> &x, const array<T> &y) {
        return apply([](T x, T y) { return min(x, y); }, x, y);
    }
}

#endif // MATH_H_INCLUDED
