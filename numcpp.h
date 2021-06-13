#ifndef NUMCPP_H_INCLUDED
#define NUMCPP_H_INCLUDED

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace numcpp {

    using std::cos;
    using std::sin;
    using std::tan;
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
    using std::fmod;

    using std::abs;
    using std::min;
    using std::max;

    using std::isfinite;
    using std::isinf;
    using std::isnan;

    // This namespace contains several constants.
    namespace constants {
        const long double e = exp(1.0L);
        const long double pi = acos(-1.0L);
    };

    // This class represents an array slice selector. It does not contain nor
    // refers to any element - it only describes a selection of elements to be
    // used as an index in valarray::operator[].
    class slice {
        public:
            size_t start, stop, step;

            slice(size_t stop = 0) {
                this->start = 0;
                this->stop = stop;
                this->step = 1;
            }

            slice(size_t start, size_t stop, size_t step = 1) {
                this->start = start;
                this->stop = stop;
                this->step = step;
            }
    };

    // This class is used as an intermediate type returned by array's subscript
    // operator (operator[]). It references the elements in the array object
    // that are selected by the subscript, and overloads the assignment and
    // compound assignment operators, allowing direct access to the elements in
    // the selection. The type is convertible to an array, producing a new
    // object with copies of the referred elements.
    template <class T, class subscript> class subarray;

    // Arrays are fixed-size sequence containers: they hold a specific number
    // of elements ordered in a strict linear sequence.
    template <class T>
    class array {
        protected:
            size_t length;
            T *values;

        public:
            typedef size_t size_type;
            typedef T value_type;

            // Default constructor. Constructs an empty array with no elements.
            array();

            // Size constructor. Constructs an array with n elements.
            array(size_t n);

            // Fill constructor. Constructs an array with n elements, each
            // initialized to val.
            array(size_t n, const T &val);

            // Range constructor. Constructs an array with as many elements as
            // the range [first, last), with each element constructed from its
            // corresponding element in that range, in the same order.
            template <class InputIterator>
            array(InputIterator first, InputIterator last);

            // Copy constructor. Constructs an array with the contents of v.
            array(const array &v);

            // Move constructor. Constructs an array that acquires the elements
            // of v.
            array(array &&v);

            // Initializer list. Constructs an array with a copy of each of the
            // elements in il, in the same order.
            array(std::initializer_list<T> il);

            // Destructor. Destroys the array.
            ~array();

            // Copy assignment. Assigns the contents of v to *this after
            // resizing the object (if necessary).
            array& operator= (const array &v);

            // Move assignment. Acquires the contents of v.
            array& operator= (array &&v);

            // Fill assignment. Assigns val to every element.
            array& operator= (const T &val);

            // Compound assignments.
            array& operator+= (const array &v);

            array& operator-= (const array &v);

            array& operator*= (const array &v);

            array& operator/= (const array &v);

            array& operator%= (const array &v);

            array& operator&= (const array &v);

            array& operator|= (const array &v);

            array& operator^= (const array &v);

            array& operator<<= (const array &v);

            array& operator>>= (const array &v);

            array& operator+= (const T &val);

            array& operator-= (const T &val);

            array& operator*= (const T &val);

            array& operator/= (const T &val);

            array& operator%= (const T &val);

            array& operator&= (const T &val);

            array& operator|= (const T &val);

            array& operator^= (const T &val);

            array& operator<<= (const T &val);

            array& operator>>= (const T &val);

            // Returns the number of elements in the array.
            size_t size() const;

            // Returns a reference to the element at position i in the array.
            T& operator[] (size_t i);

            const T& operator[] (size_t i) const;

            // Return a sub-array object that selects the elements specified by
            // its argument.
            subarray<T, slice> operator[] (const slice &slc);

            const subarray<T, slice> operator[] (const slice &slc) const;

            subarray<T, size_t> operator[] (const array<size_t> &indices);

            const subarray<T, size_t>
            operator[] (const array<size_t> &indices) const;

            subarray<T, bool> operator[] (const array<bool> &mask);

            const subarray<T, bool> operator[] (const array<bool> &mask) const;

            // Returns a pointer to the memory array used internally by the
            // array.
            T* data();

            const T* data() const;

            // Resizes the array, changing its size to n elements.
            // If n is smaller than the current size, the content is reduced to
            // its first n elements, removing those beyond.
            // If n is greater than the current size, the content is expanded
            // by inserting at the end as many elements as needed to reach a
            // size of n.
            void resize(size_t n, const T &val = T());

            // Apply a function to each of the elements in *this.
            void apply(T func(const T&));

            void apply(T func(T));

            // Return the index of the maximum value.
            size_t argmax() const;

            // Return the index of the minimum value.
            size_t argmin() const;

            // Returns the indices that would sort this array.
            array<size_t> argsort() const;

            // Copy of the array, cast to a specified type.
            template <class U> array<U> astype() const;

            // Clip (limit) the values in the array. Given an interval, values
            // outside the interval are clipped to the interval edges.
            void clip(const T &a_min, const T &a_max);

            // Return the cumulative product of the elements.
            array cumprod() const;

            // Return the cumulative sum of the elements.
            array cumsum() const;

            // Return the dot product of two arrays.
            T dot(const array &v) const;

            // Returns the maximum value contained in the array.
            T max() const;

            // Returns the minimum value contained in the array.
            T min() const;

            // Returns the average of the array elements.
            T mean() const;

            // Return the product of the array elements.
            T prod() const;

            // Sort an array in-place.
            void sort();

            // Returns the standard deviation of the array elements.
            T stddev(size_t ddof = 0) const;

            // Return the sum of the array elements.
            T sum() const;

            // Swap contents with v.
            void swap(array &v);

            // Returns the variance of the array elements.
            T var(size_t ddof = 0) const;
    };

    // This class is used as an intermediate type returned by array's subscript
    // operator (operator[]) when used with slices.
    template <class T>
    class subarray<T, slice> {
        protected:
            T *values;
            slice slc;

            friend class array<T>;

        public:
            // Assignment operators.
            void operator= (const array<T> &v);

            void operator= (const T &val);

            // Compound assignment.
            void operator+= (const array<T> &v);

            void operator-= (const array<T> &v);

            void operator*= (const array<T> &v);

            void operator/= (const array<T> &v);

            void operator%= (const array<T> &v);

            void operator&= (const array<T> &v);

            void operator|= (const array<T> &v);

            void operator^= (const array<T> &v);

            void operator<<= (const array<T> &v);

            void operator>>= (const array<T> &v);

            void operator+= (const T &val);

            void operator-= (const T &val);

            void operator*= (const T &val);

            void operator/= (const T &val);

            void operator%= (const T &val);

            void operator&= (const T &val);

            void operator|= (const T &val);

            void operator^= (const T &val);

            void operator<<= (const T &val);

            void operator>>= (const T &val);

            array<T> copy() const;

            size_t size() const;
    };

    // This class is used as an intermediate type returned by array's subscript
    // operator (operator[]) when used with an array of indices.
    template <class T>
    class subarray<T, size_t> {
        protected:
            T *values;
            array<size_t> indices;

            friend class array<T>;

        public:
            // Assignment operators.
            void operator= (const array<T> &v);

            void operator= (const T &val);

            // Compound assignment.
            void operator+= (const array<T> &v);

            void operator-= (const array<T> &v);

            void operator*= (const array<T> &v);

            void operator/= (const array<T> &v);

            void operator%= (const array<T> &v);

            void operator&= (const array<T> &v);

            void operator|= (const array<T> &v);

            void operator^= (const array<T> &v);

            void operator<<= (const array<T> &v);

            void operator>>= (const array<T> &v);

            void operator+= (const T &val);

            void operator-= (const T &val);

            void operator*= (const T &val);

            void operator/= (const T &val);

            void operator%= (const T &val);

            void operator&= (const T &val);

            void operator|= (const T &val);

            void operator^= (const T &val);

            void operator<<= (const T &val);

            void operator>>= (const T &val);

            array<T> copy() const;

            size_t size() const;
    };

    // This class is used as an intermediate type returned by array's subscript
    // operator (operator[]) when used with masks.
    template <class T>
    class subarray<T, bool> {
        protected:
            T *values;
            array<bool> mask;

            friend class array<T>;

        public:
            // Assignment operators.
            void operator= (const array<T> &v);

            void operator= (const T &val);

            // Compound assignment.
            void operator+= (const array<T> &v);

            void operator-= (const array<T> &v);

            void operator*= (const array<T> &v);

            void operator/= (const array<T> &v);

            void operator%= (const array<T> &v);

            void operator&= (const array<T> &v);

            void operator|= (const array<T> &v);

            void operator^= (const array<T> &v);

            void operator<<= (const array<T> &v);

            void operator>>= (const array<T> &v);

            void operator+= (const T &val);

            void operator-= (const T &val);

            void operator*= (const T &val);

            void operator/= (const T &val);

            void operator%= (const T &val);

            void operator&= (const T &val);

            void operator|= (const T &val);

            void operator^= (const T &val);

            void operator<<= (const T &val);

            void operator>>= (const T &val);

            array<T> copy() const;

            size_t size() const;
    };

    // Context manager for setting print options.
    namespace printoptions {
        // Number of digits of precision for floating point output (default 8).
        size_t precision = 8;

        // Total number of array elements which trigger summarization rather
        // than full representation (default 100).
        size_t threshold = 100;

        // Number of array items in summary at beginning and end of each
        // dimension (default 3).
        size_t edgeitems = 3;

        // Controls printing of the sign of numeric types. If true, always
        // print the sign of positive values. If false, omit the sign character
        // of positive values. (default false)
        bool sign = false;

        // Controls the interpretation of the precision option for
        // floating-point types. Can take the following values:
        // - "default":    write floating-point values in default
        //                 floating-point notation.
        // - "fixed":      write floating-point values in fixed-point notation.
        // - "scientific": write floating-point values in scientific notation.
        std::string floatmode = "default";
    };

    template <class T>
    std::ostream& operator<< (std::ostream &ostr, const array<T> &v) {
        ostr << std::boolalpha << std::setprecision(printoptions::precision);

        if (printoptions::sign) {
            ostr << std::showpos;
        }
        else {
            ostr << std::noshowpos;
        }

        if (printoptions::floatmode == "default") {
            ostr << std::defaultfloat;
        }
        else if (printoptions::floatmode == "fixed") {
            ostr << std::fixed;
        }
        else if (printoptions::floatmode == "scientific") {
            ostr << std::scientific;
        }
        else {
            throw std::invalid_argument(
                "printoptions::floatmode must be one of \"default\", "
                "\"fixed\" or \"scientific\""
            );
        }

        ostr << "[";
        if (
            v.size() < printoptions::threshold ||
            v.size() <= 2*printoptions::edgeitems
        ) {
            std::string sep = "";
            for (size_t i = 0; i < v.size(); ++i) {
                ostr << sep << v[i];
                sep = ", ";
            }
        }
        else {
            std::string sep = "";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << sep << v[i];
                sep = ", ";
            }
            sep = ", ..., ";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << sep << v[v.size() - printoptions::edgeitems + i];
                sep = ", ";
            }
        }
        ostr << "]";

        return ostr;
    }

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
        if (y.size() != x.size()) {
            throw std::runtime_error(
                "operands could not be broadcast together with shapes (" +
                std::to_string(y.size()) + ",) (" + std::to_string(x.size()) +
                ",)"
            );
        }
        array<T> out(y.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = atan2(y[i], x[i]);
        }
        return out;
    }

    template <class T>
    array<T> atan2(const array<T> &y, const T &x) {
        array<T> x_as_array(y.size(), x);
        return atan2(y, x_as_array);
    }

    template <class T>
    array<T> atan2(const T &y, const array<T> &x) {
        array<T> y_as_array(x.size(), y);
        return atan2(y_as_array, x);
    }

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

    // Returns an array containing the results of the power operation on all
    // the elements, in the same order. The results calculated are x raised to
    // the power y.
    template <class T>
    array<T> pow(const array<T> &x, const array<T> &y) {
        if (x.size() != y.size()) {
            throw std::runtime_error(
                "operands could not be broadcast together with shapes (" +
                std::to_string(x.size()) + ",) (" + std::to_string(y.size()) +
                ",)"
            );
        }
        array<T> out(y.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = pow(x[i], y[i]);
        }
        return out;
    }

    template <class T>
    array<T> pow(const array<T> &x, const T &y) {
        array<T> y_as_array(x.size(), y);
        return pow(x, y_as_array);
    }

    template <class T>
    array<T> pow(const T &x, const array<T> &y) {
        array<T> x_as_array(y.size(), x);
        return pow(x_as_array, y);
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

    // Returns an array containing the results of the floating-point remainder
    // on all the elements, in the same order. The results calculated are:
    //     fmod = numer - tquot * denom
    // where tquot is the truncated (i.e., rounded towards zero) result of:
    // numer/denom.
    template <class T>
    array<T> fmod(const array<T> &numer, const array<T> &denom) {
        if (numer.size() != denom.size()) {
            throw std::runtime_error(
                "operands could not be broadcast together with shapes (" +
                std::to_string(numer.size()) + ",) (" +
                std::to_string(denom.size()) + ",)"
            );
        }
        array<T> out(numer.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = fmod(numer[i], denom[i]);
        }
        return out;
    }

    template <class T>
    array<T> fmod(const array<T> &numer, const T &denom) {
        array<T> denom_as_array(numer.size(), denom);
        return fmod(numer, denom_as_array);
    }

    template <class T>
    array<T> fmod(const T &numer, const array<T> &denom) {
        array<T> numer_as_array(denom.size(), numer);
        return fmod(numer_as_array, denom);
    }

    // Returns an array containing the absolute values of all the elements of
    // x, in the same order.
    template <class T>
    array<T> abs(const array<T> &x) {
        return apply(abs, x);
    }
}

#include "numcpp/array.h"

#endif // NUMCPP_H_INCLUDED
