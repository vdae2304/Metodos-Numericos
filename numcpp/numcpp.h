#ifndef NUMCPP_H_INCLUDED
#define NUMCPP_H_INCLUDED

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
        private:
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

            // Destructor.
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

    // Matrices are two dimensional arrays.
    template <class T>
    class matrix {
        ;
    };

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
                ostr << v[i] << ' ';
            }
        }
        else {
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << v[i] << ' ';
            }
            ostr << "... ";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << v[v.size() - printoptions::edgeitems + i] << ' ';
            }
        }
        ostr << "]\n";
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
                std::cerr << "(" << largest << " " << smallest << ")\n";
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
                ostr << v[i] << ' ';
            }
        }
        else {
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                if (v[i] >= 0 && printoptions::sign != '-') {
                    ostr << printoptions::sign;
                }
                ostr << v[i] << ' ';
            }
            ostr << " ...";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                size_t j = v.size() - printoptions::edgeitems + i;
                if (v[j] >= 0 && printoptions::sign != '-') {
                    ostr << printoptions::sign;
                }
                ostr << v[j] << ' ';
            }
        }

        ostr << std::defaultfloat << "]\n";
        return ostr;
    }

}

#endif // NUMCPP_H_INCLUDED
