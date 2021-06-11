#ifndef NUMCPP_H_INCLUDED
#define NUMCPP_H_INCLUDED

#include <algorithm>
#include <cmath>
#include <cstdio>
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
            array() {
                this->length = 0;
                this->values = nullptr;
            }

            // Size constructor. Constructs an array with n elements.
            array(size_t n) {
                this->length = n;
                this->values = new T[n];
            }

            // Fill constructor. Constructs an array with n elements, each
            // initialized to val.
            array(size_t n, const T &val) {
                this->length = n;
                this->values = new T[n];
                for (size_t i = 0; i < n; ++i) {
                    this->values[i] = val;
                }
            }

            // Range constructor. Constructs an array with as many elements as
            // the range [first, last), with each element constructed from its
            // corresponding element in that range, in the same order.
            template <class InputIterator>
            array(InputIterator first, InputIterator last) {
                size_t n = 0;
                for (InputIterator it = first; it != last; ++it) {
                    ++n;
                }
                this->length = n;
                this->values = new T[n];
                for (size_t i = 0; i < n; ++i) {
                    this->values[i] = *first;
                    ++first;
                }
            }

            // Copy constructor. Constructs an array with the contents of v.
            array(const array &v) {
                this->length = v.length;
                this->values = new T[this->length];
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] = v.values[i];
                }
            }

            // Move constructor. Constructs an array that acquires the elements
            // of v.
            array(array &&v) {
                this->length = v.length;
                this->values = v.values;
                v.length = 0;
                v.values = nullptr;
            }

            // Initializer list. Constructs an array with a copy of each of the
            // elements in il, in the same order.
            array(std::initializer_list<T> il) {
                this->length = il.size();
                this->values = new T[this->length];
                const T* it = il.begin();
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] = *it;
                    ++it;
                }
            }

            // Destructor. Destroys the array.
            ~array() {
                delete[] this->values;
                this->length = 0;
                this->values = nullptr;
            }

            // Return a new uninitialized array.
            static array empty(size_t n) {
                return array(n);
            }

            // Return a new array setting values to zero.
            static array zeros(size_t n) {
                return array(n, T(0));
            }

            // Return a new array setting values to one.
            static array ones(size_t n) {
                return array(n, T(1));
            }

            // Return a new array of given length filled with value.
            static array full(size_t n, const T &val) {
                return array(n, val);
            }

            // Return a new array from a binary file.
            static array load(const char *file) {
                FILE *pfile = fopen(file, "rb");
                array out;
                if (pfile != NULL) {
                    fread(&out.length, sizeof(size_t), 1, pfile);
                    out.values = new T[out.length];
                    fread(out.values, sizeof(T), out.length, pfile);
                    fclose(pfile);
                }
                else {
                    throw std::runtime_error(
                        std::string("No such file or directory: ") + file
                    );
                }
                return out;
            }

            // Save an array to a binary file.
            void save(const char *file) const {
                FILE *pfile = fopen(file, "wb");
                fwrite(&this->length, sizeof(size_t), 1, pfile);
                fwrite(this->values, sizeof(T), this->length, pfile);
                fclose(pfile);
            }

            // Copy assignment. Assigns the contents of v to *this after
            // resizing the object (if necessary).
            array& operator= (const array &v) {
                if (this->length != v.length) {
                    delete[] this->values;
                    this->length = v.length;
                    this->values = new T[this->length];
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] = v.values[i];
                }
                return *this;
            }

            // Move assignment. Acquires the contents of v.
            array& operator= (array &&v) {
                if (this != &v) {
                    delete[] this->values;
                    this->length = v.length;
                    this->values = v.values;
                    v.length = 0;
                    v.values = nullptr;
                }
                return *this;
            }

            // Fill assignment. Assigns val to every element.
            array& operator= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] = val;
                }
                return *this;
            }

            // Compound assignments.
            array& operator+= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] += v.values[i];
                }
                return *this;
            }

            array& operator-= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] -= v.values[i];
                }
                return *this;
            }

            array& operator*= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] *= v.values[i];
                }
                return *this;
            }

            array& operator/= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] /= v.values[i];
                }
                return *this;
            }

            array& operator%= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] %= v.values[i];
                }
                return *this;
            }

            array& operator&= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] &= v.values[i];
                }
                return *this;
            }

            array& operator|= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] |= v.values[i];
                }
                return *this;
            }

            array& operator^= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] ^= v.values[i];
                }
                return *this;
            }

            array& operator<<= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] <<= v.values[i];
                }
                return *this;
            }

            array& operator>>= (const array &v) {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] >>= v.values[i];
                }
                return *this;
            }

            array& operator+= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] += val;
                }
                return *this;
            }

            array& operator-= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] -= val;
                }
                return *this;
            }

            array& operator*= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] *= val;
                }
                return *this;
            }

            array& operator/= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] /= val;
                }
                return *this;
            }

            array& operator%= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] %= val;
                }
                return *this;
            }

            array& operator&= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] &= val;
                }
                return *this;
            }

            array& operator|= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] |= val;
                }
                return *this;
            }

            array& operator^= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] ^= val;
                }
                return *this;
            }

            array& operator<<= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] <<= val;
                }
                return *this;
            }

            array& operator>>= (const T &val) {
                for (size_t i = 0; i < this->length; ++i) {
                    this->values[i] >>= val;
                }
                return *this;
            }

            // Returns the number of elements in the array.
            size_t size() const {
                return this->length;
            }

            // Returns a reference to the element at position i in the array.
            T& operator[] (size_t i) {
                return this->values[i];
            }

            const T& operator[] (size_t i) const {
                return this->values[i];
            }

            // Returns a pointer to the memory array used internally by the
            // array.
            T* data() {
                return this->values;
            }

            const T* data() const {
                return this->values;
            }

            // Returns an array with each of its elements initialized to the
            // result of applying func to its corresponding element in *this.
            array apply(T func(const T&)) const {
                array out(this->length);
                for (size_t i = 0; i < out.length; ++i) {
                    out.values[i] = funct(this->values[i]);
                }
                return out;
            }

            array apply(T func(T)) const {
                array out(this->length);
                for (size_t i = 0; i < out.length; ++i) {
                    out.values[i] = func(this->values[i]);
                }
                return out;
            }

            // Return the index of the maximum value.
            size_t argmax() const {
                size_t index = 0;
                for (size_t i = 1; i < this->length; ++i) {
                    if (this->values[index] < this->values[i]) {
                        index = i;
                    }
                }
                return index;
            }

            // Return the index of the minimum value.
            size_t argmin() const {
                size_t index = 0;
                for (size_t i = 1; i < this->length; ++i) {
                    if (this->values[i] < this->values[index]) {
                        index = i;
                    }
                }
                return index;
            }

            // Returns the indices that would sort this array.
            array<size_t> argsort() const {
                array<size_t> indices(this->length);
                for (size_t i = 0; i < this->length; ++i) {
                    indices[i] = i;
                }
                std::sort(
                    indices.data(),
                    indices.data() + this->length,
                    [=](size_t i, size_t j) {
                          return this->values[i] < this->values[j];
                    }
                );
                return indices;
            }

            // Copy of the array, cast to a specified type.
            template <class U>
            array<U> astype() const {
                array<U> out(this->length);
                for (size_t i = 0; i < this->length; ++i) {
                    out[i] = U(this->values[i]);
                }
                return out;
            }

            // Return an array whose values are limited to [a_min, a_max].
            // Given an interval, values outside the interval are clipped to
            // the interval edges.
            array clip(const T &a_min, const T &a_max) const {
                array out(this->length);
                for (size_t i = 0; i < this->length; ++i) {
                    if (this->values[i] < a_min) {
                        out[i] = a_min;
                    }
                    else if (a_max < this->values[i]) {
                        out[i] = a_max;
                    }
                    else {
                        out[i] = this->values[i];
                    }
                }
                return out;
            }

            // Return the cumulative product of the elements.
            array cumprod() const {
                array out(this->length);
                out[0] = this->values[0];
                for (size_t i = 1; i < this->length; ++i) {
                    out[i] = out[i - 1] * this->values[i];
                }
                return out;
            }

            // Return the cumulative sum of the elements.
            array cumsum() const {
                array out(this->length);
                out[0] = this->values[0];
                for (size_t i = 1; i < this->length; ++i) {
                    out[i] = out[i - 1] + this->values[i];
                }
                return out;
            }

            // Return the dot product of two arrays.
            T dot(const array &v) const {
                if (this->length != v.length) {
                    throw std::runtime_error(
                        "operands could not be broadcast together with shapes"
                        " (" + std::to_string(this->length) + ",) (" +
                        std::to_string(v.length) + ",)"
                    );
                }
                T out = T(0);
                for (size_t i = 0; i < this->length; ++i) {
                    out += this->values[i] * v.values[i];
                }
                return out;
            }

            // Returns the maximum value contained in the array.
            T max() const {
                return this->values[this->argmax()];
            }

            // Returns the minimum value contained in the array.
            T min() const {
                return this->values[this->argmin()];
            }

            // Returns the average of the array elements.
            T mean() const {
                return this->sum() / this->length;
            }

            // Return the product of the array elements.
            T prod() const {
                T out = T(1);
                for (size_t i = 0; i < this->length; ++i) {
                    out *= this->values[i];
                }
                return out;
            }

            // Resizes the array, changing its size to n elements.
            // If n is smaller than the current size, the content is reduced to
            // its first n elements, removing those beyond.
            // If n is greater than the current size, the content is expanded
            // by inserting at the end as many elements as needed to reach a
            // size of n.
            void resize(size_t n, const T &val = T()) {
                array tmp_copy(*this);
                delete[] this->values;
                this->length = n;
                this->values = new T[n];
                for (size_t i = 0; i < n && i < tmp_copy.length; ++i) {
                    this->values[i] = tmp_copy.values[i];
                }
                for (size_t i = tmp_copy.length; i < n; ++i) {
                    this->values[i] = val;
                }
            }

            // Sort an array in-place.
            void sort() {
                std::sort(this->values, this->values + this->length);
            }

            // Returns the standard deviation of the array elements.
            T stddev(size_t ddof = 0) const {
                return sqrt(this->var(ddof));
            }

            // Return the sum of the array elements.
            T sum() const {
                T out = T(0);
                for (size_t i = 0; i < this->length; ++i) {
                    out += this->values[i];
                }
                return out;
            }

            // Returns the variance of the array elements.
            T var(size_t ddof = 0) const {
                T array_mean = this->mean();
                T out = T(0);
                for (size_t i = 0; i < this->length; ++i) {
                    T deviation = this->values[i] - array_mean;
                    out += deviation*deviation;
                }
                out /= (this->length - ddof);
                return out;
            }
    };

    // unary array operators.
    template <class T>
    array<T> operator+ (const array<T> &v) {
        return v;
    }

    template <class T>
    array<T> operator- (const array<T> &v) {
        array<T> out(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            out[i] = -v[i];
        }
        return out;
    }

    template <class T>
    array<T> operator~ (const array<T> &v) {
        array<T> out(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            out[i] = ~v[i];
        }
        return out;
    }

    array<bool> operator! (const array<bool> &v) {
        array<bool> out(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            out[i] = !v[i];
        }
        return out;
    }

    // binary array operators
    template<class T>
    array<T> operator+ (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out += w;
    }

    template<class T>
    array<T> operator+ (const array<T> &v, const T &val) {
        array<T> out(v);
        return out += val;
    }

    template<class T>
    array<T> operator+ (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out += v;
    }

    template<class T>
    array<T> operator- (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out -= w;
    }

    template<class T>
    array<T> operator- (const array<T> &v, const T &val) {
        array<T> out(v);
        return out -= val;
    }

    template<class T>
    array<T> operator- (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out -= v;
    }

    template<class T>
    array<T> operator* (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out *= w;
    }

    template<class T>
    array<T> operator* (const array<T> &v, const T &val) {
        array<T> out(v);
        return out *= val;
    }

    template<class T>
    array<T> operator* (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out *= v;
    }

    template<class T>
    array<T> operator/ (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out /= w;
    }

    template<class T>
    array<T> operator/ (const array<T> &v, const T &val) {
        array<T> out(v);
        return out /= val;
    }

    template<class T>
    array<T> operator/ (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out /= v;
    }

    template<class T>
    array<T> operator% (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out %= w;
    }

    template<class T>
    array<T> operator% (const array<T> &v, const T &val) {
        array<T> out(v);
        return out %= val;
    }

    template<class T>
    array<T> operator% (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out %= v;
    }

    template<class T>
    array<T> operator& (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out &= w;
    }

    template<class T>
    array<T> operator& (const array<T> &v, const T &val) {
        array<T> out(v);
        return out &= val;
    }

    template<class T>
    array<T> operator& (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out &= v;
    }

    template<class T>
    array<T> operator| (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out |= w;
    }

    template<class T>
    array<T> operator| (const array<T> &v, const T &val) {
        array<T> out(v);
        return out |= val;
    }

    template<class T>
    array<T> operator| (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out |= v;
    }

    template<class T>
    array<T> operator^ (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out ^= w;
    }

    template<class T>
    array<T> operator^ (const array<T> &v, const T &val) {
        array<T> out(v);
        return out ^= val;
    }

    template<class T>
    array<T> operator^ (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out ^= v;
    }

    template<class T>
    array<T> operator<< (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out <<= w;
    }

    template<class T>
    array<T> operator<< (const array<T> &v, const T &val) {
        array<T> out(v);
        return out <<= val;
    }

    template<class T>
    array<T> operator<< (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out <<= v;
    }

    template<class T>
    array<T> operator>> (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out >>= w;
    }

    template<class T>
    array<T> operator>> (const array<T> &v, const T &val) {
        array<T> out(v);
        return out >>= val;
    }

    template<class T>
    array<T> operator>> (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out >>= v;
    }

    template<class T>
    array<bool> operator== (const array<T> &v, const array<T> &w) {
        if (v.size() != w.size()) {
            throw std::runtime_error(
                "operands could not be broadcast together with shapes (" +
                std::to_string(v.size()) + ",) (" + std::to_string(w.size()) +
                 ",)"
            );
        }
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (v[i] == w[i]);
        }
        return out;
    }

    template<class T>
    array<bool> operator== (const array<T> &v, const T &val) {
        array<T> w(v.size(), val);
        return (v == w);
    }

    template<class T>
    array<bool> operator== (const T &val, const array<T> &v) {
        array<T> w(v.size(), val);
        return (w == v);
    }

    template<class T>
    array<bool> operator!= (const array<T> &v, const array<T> &w) {
        return !(v == w);
    }

    template<class T>
    array<bool> operator!= (const array<T> &v, const T &val) {
        return !(v == val);
    }

    template<class T>
    array<bool> operator!= (const T &val, const array<T> &v) {
        return !(val == v);
    }

    template<class T>
    array<bool> operator< (const array<T> &v, const array<T> &w) {
        if (v.size() != w.size()) {
            throw std::runtime_error(
                "operands could not be broadcast together with shapes (" +
                std::to_string(v.size()) + ",) (" + std::to_string(w.size()) +
                 ",)"
            );
        }
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (v[i] < w[i]);
        }
        return out;
    }

    template<class T>
    array<bool> operator< (const array<T> &v, const T &val) {
        array<T> w(v.size(), val);
        return (v < w);
    }

    template<class T>
    array<bool> operator< (const T &val, const array<T> &v) {
        array<T> w(v.size(), val);
        return (w < v);
    }

    template<class T>
    array<bool> operator> (const array<T> &v, const array<T> &w) {
        return (w < v);
    }

    template<class T>
    array<bool> operator> (const array<T> &v, const T &val) {
        return (val < v);
    }

    template<class T>
    array<bool> operator> (const T &val, const array<T> &v) {
        return (v < val);
    }

    template<class T>
    array<bool> operator<= (const array<T> &v, const array<T> &w) {
        return !(w < v);
    }

    template<class T>
    array<bool> operator<= (const array<T> &v, const T &val) {
        return !(val < v);
    }

    template<class T>
    array<bool> operator<= (const T &val, const array<T> &v) {
        return !(v < val);
    }

    template<class T>
    array<bool> operator>= (const array<T> &v, const array<T> &w) {
        return !(w < v);
    }

    template<class T>
    array<bool> operator>= (const array<T> &v, const T &val) {
        return !(v < val);
    }

    template<class T>
    array<bool> operator>= (const T &val, const array<T> &v) {
        return !(val < v);
    }

    // Context manager for setting print options.
    class printoptions {
        public:
            // Number of digits of precision for floating point output
            // (default 8).
            static size_t precision;

            // Total number of array elements which trigger summarization
            // rather than full representation (default 1000).
            static size_t threshold;

            // Number of array items in summary at beginning and end of each
            // dimension (default 3).
            static size_t edgeitems;

            // If true, always print floating point numbers using fixed point
            // notation, in which case numbers equal to zero in the current
            // precision will print as zero. If false, then scientific notation
            // is used when absolute value of the smallest number is < 1e-4 or
            // the ratio of the maximum absolute value to the minimum is > 1e3.
            // The default is False.
            static bool suppress;

            // Controls printing of the sign of floating-point types. If '+',
            // always print the sign of positive values. If ' ', always prints
            // a space (whitespace character) in the sign position of positive
            // values. If '-', omit the sign character of positive values.
            // (default '-')
            static char sign;
    };

    size_t printoptions::precision = 8;
    size_t printoptions::threshold = 1000;
    size_t printoptions::edgeitems = 3;
    bool printoptions::suppress = false;
    char printoptions::sign = '-';

    template <
        class T,
        std::enable_if_t<!std::is_floating_point<T>::value, bool> = true
    >
    std::ostream& operator<< (std::ostream &ostr, const array<T> &v) {
        ostr << std::boolalpha << "[ ";
        if (
            v.size() < printoptions::threshold ||
            v.size() <= 2*printoptions::edgeitems
        ) {
            for (size_t i = 0; i < v.size(); ++i) {
                ostr << v[i] << " ";
            }
        }
        else {
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << v[i] << " ";
            }
            ostr << "... ";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << v[v.size() - printoptions::edgeitems + i] << " ";
            }
        }
        ostr << "]";
        return ostr;
    }

    template <
        class T,
        std::enable_if_t<std::is_floating_point<T>::value, bool> = true
    >
    std::ostream& operator<< (std::ostream &ostr, const array<T> &v) {
        ostr << std::setprecision(printoptions::precision) << "[ ";

        if (printoptions::suppress) {
            ostr << std::fixed;
        }
        else {
            T largest = abs(v[0]);
            T smallest = largest;
            for (size_t i = 1; i < v.size(); ++i) {
                T current = abs(v[i]);
                if (current > largest) {
                    largest = current;
                }
                else if (current != 0 && current < smallest) {
                    smallest = current;
                }
            }
            if (smallest < 1e-4 || largest/smallest > 1e3) {
                ostr << std::scientific;
            }
        }

        if (
            v.size() < printoptions::threshold ||
            v.size() <= 2*printoptions::edgeitems
        ) {
            for (size_t i = 0; i < v.size(); ++i) {
                if (v[i] >= 0 && printoptions::sign != '-') {
                    ostr << printoptions::sign;
                }
                ostr << v[i] << " ";
            }
        }
        else {
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                if (v[i] >= 0 && printoptions::sign != '-') {
                    ostr << printoptions::sign;
                }
                ostr << v[i] << " ";
            }
            ostr << " ...";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                size_t j = v.size() - printoptions::edgeitems + i;
                if (v[j] >= 0 && printoptions::sign != '-') {
                    ostr << printoptions::sign;
                }
                ostr << v[j] << " ";
            }
        }

        ostr << std::defaultfloat << "]";
        return ostr;
    }

    // Returns an array containing the cosines of all the elements of x, in the
    // same order.
    template <class T>
    array<T> cos(const array<T> &x) {
        return x.apply(cos);
    }

    // Returns an array containing the sines of all the elements of x, in the
    // same order.
    template <class T>
    array<T> sin(const array<T> &x) {
        return x.apply(sin);
    }

    // Returns an array containing the tangents of all the elements of x, in
    // the same order.
    template <class T>
    array<T> tan(const array<T> &x) {
        return x.apply(tan);
    }

    // Returns an array containing the principal values of the arc cosine of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> acos(const array<T> &x) {
        return x.apply(acos);
    }

    // Returns an array containing the principal values of the arc sine of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> asin(const array<T> &x) {
        return x.apply(asin);
    }

    // Returns an array containing the principal values of the arc tangent of
    // all the elements of x, expressed in radians, in the same order.
    template <class T>
    array<T> atan(const array<T> &x) {
        return x.apply(atan);
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
        return x.apply(cosh);
    }

    // Returns an array containing the hyperbolic sines of all the elements
    // of x, in the same order.
    template <class T>
    array<T> sinh(const array<T> &x) {
        return x.apply(sinh);
    }

    // Returns an array containing the hyperbolic tangents of all the elements
    // of x, in the same order.
    template <class T>
    array<T> tanh(const array<T> &x) {
        return x.apply(tanh);
    }

    // Returns an array containing the area hyperbolic cosines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> acosh(const array<T> &x) {
        return x.apply(acosh);
    }

    // Returns an array containing the area hyperbolic sines of all the
    // elements of x, in the same order.
    template <class T>
    array<T> asinh(const array<T> &x) {
        return x.apply(asinh);
    }

    // Returns an array containing the area hyperbolic tangents of all the
    // elements of x, in the same order.
    template <class T>
    array<T> atanh(const array<T> &x) {
        return x.apply(atanh);
    }

    // Returns an array containing the exponential function values of all the
    // elements of x, in the same order.
    template <class T>
    array<T> exp(const array<T> &x) {
        return x.apply(exp);
    }

    // Returns an array containing the base-2 exponential function values of
    // all the elements of x, in the same order.
    template <class T>
    array<T> exp2(const array<T> &x) {
        return x.apply(exp2);
    }

    // Returns an array containing the natural logarithm (base-e logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log(const array<T> &x) {
        return x.apply(log);
    }

    // Returns an array containing the binary logarithm (base-2 logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log2(const array<T> &x) {
        return x.apply(log2);
    }

    // Returns an array containing the common logarithm (base-10 logarithm) of
    // all the elements of x, in the same order.
    template <class T>
    array<T> log10(const array<T> &x) {
        return x.apply(log10);
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
        return x.apply(sqrt);
    }

    // Returns an array containing the cubic root of all the elements of x, in
    // the same order.
    template <class T>
    array<T> cbrt(const array<T> &x) {
        return x.apply(cbrt);
    }

    // Returns an array containing the results of rounding upward all the
    // elements of x, in the same order.
    template <class T>
    array<T> ceil(const array<T> &x) {
        return x.apply(ceil);
    }

    // Returns an array containing the results of rounding downward all the
    // elements of x, in the same order.
    template <class T>
    array<T> floor(const array<T> &x) {
        return x.apply(floor);
    }

    // Returns an array containing the results of rounding all the elements of
    // x toward the nearest integer value, in the same order.
    template <class T>
    array<T> round(const array<T> &x) {
        return x.apply(round);
    }

    // Returns an array containing the results of rounding all the elements of
    // x toward zero, in the same order.
    template <class T>
    array<T> trunc(const array<T> &x) {
        return x.apply(trunc);
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
        return pow(numer, denom_as_array);
    }

    template <class T>
    array<T> fmod(const T &numer, const array<T> &denom) {
        array<T> numer_as_array(denom.size(), numer);
        return pow(numer_as_array, denom);
    }

    // Returns an array containing the absolute values of all the elements of
    // x, in the same order.
    template <class T>
    array<T> abs(const array<T> &x) {
        return x.apply(abs);
    }

    // Returns true if all elements evaluate to true.
    bool all(const array<bool> &v) {
        for (size_t i = 0; i < v.size(); ++i) {
            if (!v[i]) {
                return false;
            }
        }
        return true;
    }

    // Returns true if any of the elements evaluate to true.
    bool any(const array<bool> &v) {
        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i]) {
                return true;
            }
        }
        return false;
    }

    // Return the index of the maximum value.
    template <class T>
    size_t argmax(const array<T> &v) {
        return v.argmax();
    }

    // Return the index of the minimum value.
    template <class T>
    size_t argmin(const array<T> &v) {
        return v.argmin();
    }

    // Returns the indices that would sort this array.
    template <class T>
    size_t argsort(const array<T> &v) {
        return v.argsort();
    }

    // Return an array whose values are limited to [a_min, a_max].
    // Given an interval, values outside the interval are clipped to the
    // interval edges.
    template <class T>
    array<T> clip(const array<T> &v, const T &a_min, const T &a_max) {
        return v.clip(a_min, a_max);
    }

    // Return the cumulative product of the elements.
    template <class T>
    array<T> cumprod(const array<T> &v) {
        return v.cumprod();
    }

    // Return the cumulative sum of the elements.
    template <class T>
    array<T> cumsum(const array<T> &v) {
        return v.cumsum();
    }

    // Return the dot product of two arrays.
    template <class T>
    T dot(const array<T> &v, const array<T> &w) {
        return v.dot(w);
    }

    // Returns the maximum value contained in the array.
    template <class T>
    T max(const array<T> &v) {
        return v.max();
    }

    // Returns the average of the array elements.
    template <class T>
    T mean(const array<T> &v) {
        return v.mean();
    }

    // Returns the minimum value contained in the array.
    template <class T>
    T min(const array<T> &v) {
        return v.min();
    }

    // Return the product of the array elements.
    template <class T>
    T prod(const array<T> &v) {
        return v.prod();
    }

    // Return a sorted copy of an array.
    template <class T>
    array<T> sort(const array<T> &v) {
        array<T> sorted(v);
        sorted.sort();
        return sorted;
    }

    // Returns the standard deviation of the array elements.
    template <class T>
    T stddev(const array<T> &v) {
        return v.stddev();
    }

    // Return the sum of the array elements.
    template <class T>
    T sum(const array<T> &v) {
        return v.sum();
    }

    // Returns the variance of the array elements.
    template <class T>
    T var(const array<T> &v) {
        return v.var();
    }
}

#endif // NUMCPP_H_INCLUDED
