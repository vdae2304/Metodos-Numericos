#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Constructors and destructor                                            //
    ////////////////////////////////////////////////////////////////////////////

    // Default constructor. Constructs an empty array with no elements.
    template <class T>
    array<T>::array() {
        this->length = 0;
        this->values = nullptr;
    }

    // Size constructor. Constructs an array with n elements.
    template <class T>
    array<T>::array(size_t n) {
        this->length = n;
        this->values = new T[n];
    }

    // Fill constructor. Constructs an array with n elements, each initialized
    // to val.
    template <class T>
    array<T>::array(size_t n, const T &val) {
        this->length = n;
        this->values = new T[n];
        for (size_t i = 0; i < n; ++i) {
            this->values[i] = val;
        }
    }

    // Range constructor. Constructs an array with as many elements as the
    // range [first, last), with each element constructed from its
    // corresponding element in that range, in the same order.
    template <class T>
    template <class InputIterator>
    array<T>::array(InputIterator first, InputIterator last) {
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

    // Copy constructor. Constructs an array with a copy of each of the
    // elements in v, in the same order.
    template <class T>
    array<T>::array(const array<T> &v) {
        this->length = v.length;
        this->values = new T[this->length];
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] = v.values[i];
        }
    }

    template <class T>
    template <class U>
    array<T>::array(const array<U> &v) {
        this->length = v.length;
        this->values = new T[this->length];
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] = U(v.values[i]);
        }
    }

    // Move constructor. Constructs an array that acquires the elements of v.
    template <class T>
    array<T>::array(array<T> &&v) {
        this->length = v.length;
        this->values = v.values;
        v.length = 0;
        v.values = nullptr;
    }

    // Subarray constructor. Constructs an array with a copy of each of the
    // elements in v, in the same order.
    template <class T>
    array<T>::array(const subarray<T> &v) {
        this->length = v.size();
        this->values = new T[this->length];
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] = v[i];
        }
    }

    // Initializer list. Constructs an array with a copy of each of the
    // elements in il, in the same order.
    template <class T>
    array<T>::array(std::initializer_list<T> il) {
        this->length = il.size();
        this->values = new T[this->length];
        size_t n = 0;
        for (const T &val : il) {
            this->values[n++] = val;
        }
    }

    // Destructor. Destroys the array.
    template <class T>
    array<T>::~array() {
        delete[] this->values;
        this->length = 0;
        this->values = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Assignment operators                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Copy assignment. Assigns the contents of v to *this after resizing the
    // object (if necessary).
    template <class T>
    array<T>& array<T>::operator= (const array<T> &v) {
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

    template <class T>
    template <class U>
    array<T>& array<T>::operator= (const array<U> &v) {
        if (this->length != v.length) {
            delete[] this->values;
            this->length = v.length;
            this->values = new T[this->length];
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] = U(v.values[i]);
        }
        return *this;
    }

    // Subarray assignment. Assigns the contents of v to *this after resizing 
    // the object (if necessary).
    template <class T>
    array<T>& array<T>::operator= (const subarray<T> &v) {
        if (this->length != v.size()) {
            delete[] this->values;
            this->length = v.size();
            this->values = new T[this->length];
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] = v[i];
        }
        return *this;
    }

    // Move assignment. Acquires the contents of v.
    template <class T>
    array<T>& array<T>::operator= (array<T> &&v) {
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
    template <class T>
    array<T>& array<T>::operator= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] = val;
        }
        return *this;
    }

    // Compound assignments.
    template <class T>
    array<T>& array<T>::operator+= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] += v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator-= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] -= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator*= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] *= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator/= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] /= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator%= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] %= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator&= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] &= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator|= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] |= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator^= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] ^= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator<<= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] <<= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator>>= (const array<T> &v) {
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] >>= v.values[i];
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator+= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] += val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator-= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] -= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator*= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] *= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator/= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] /= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator%= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] %= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator&= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] &= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator|= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] |= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator^= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] ^= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator<<= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] <<= val;
        }
        return *this;
    }

    template <class T>
    array<T>& array<T>::operator>>= (const T &val) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] >>= val;
        }
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Operator overloading                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Unary operators.
    template <class T>
    array<T> operator+ (const array<T> &v) {
        return v;
    }

    template <class T>
    array<T> operator- (const array<T> &v) {
        array<T> out(v);
        return v *= -1;
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

    // Arithmetic operators.
    template <class T>
    array<T> operator+ (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out += w;
    }

    template <class T>
    array<T> operator+ (const array<T> &v, const T &val) {
        array<T> out(v);
        return out += val;
    }

    template <class T>
    array<T> operator+ (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out += v;
    }

    template <class T>
    array<T> operator- (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out -= w;
    }

    template <class T>
    array<T> operator- (const array<T> &v, const T &val) {
        array<T> out(v);
        return out -= val;
    }

    template <class T>
    array<T> operator- (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out -= v;
    }

    template <class T>
    array<T> operator* (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out *= w;
    }

    template <class T>
    array<T> operator* (const array<T> &v, const T &val) {
        array<T> out(v);
        return out *= val;
    }

    template <class T>
    array<T> operator* (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out *= v;
    }

    template <class T>
    array<T> operator/ (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out /= w;
    }

    template <class T>
    array<T> operator/ (const array<T> &v, const T &val) {
        array<T> out(v);
        return out /= val;
    }

    template <class T>
    array<T> operator/ (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out /= v;
    }

    template <class T>
    array<T> operator% (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out %= w;
    }

    template <class T>
    array<T> operator% (const array<T> &v, const T &val) {
        array<T> out(v);
        return out %= val;
    }

    template <class T>
    array<T> operator% (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out %= v;
    }

    // Bitwise operators.
    template <class T>
    array<T> operator& (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out &= w;
    }

    template <class T>
    array<T> operator& (const array<T> &v, const T &val) {
        array<T> out(v);
        return out &= val;
    }

    template <class T>
    array<T> operator& (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out &= v;
    }

    template <class T>
    array<T> operator| (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out |= w;
    }

    template <class T>
    array<T> operator| (const array<T> &v, const T &val) {
        array<T> out(v);
        return out |= val;
    }

    template <class T>
    array<T> operator| (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out |= v;
    }

    template <class T>
    array<T> operator^ (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out ^= w;
    }

    template <class T>
    array<T> operator^ (const array<T> &v, const T &val) {
        array<T> out(v);
        return out ^= val;
    }

    template <class T>
    array<T> operator^ (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out ^= v;
    }

    template <class T>
    array<T> operator<< (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out <<= w;
    }

    template <class T>
    array<T> operator<< (const array<T> &v, const T &val) {
        array<T> out(v);
        return out <<= val;
    }

    template <class T>
    array<T> operator<< (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out <<= v;
    }

    template <class T>
    array<T> operator>> (const array<T> &v, const array<T> &w) {
        array<T> out(v);
        return out >>= w;
    }

    template <class T>
    array<T> operator>> (const array<T> &v, const T &val) {
        array<T> out(v);
        return out >>= val;
    }

    template <class T>
    array<T> operator>> (const T &val, const array<T> &v) {
        array<T> out(v.size(), val);
        return out >>= v;
    }

    // Logical operators.
    array<bool> operator&& (const array<bool> &v, const array<bool> &w) {
        return (v & w);
    }

    array<bool> operator&& (const array<bool> &v, bool val) {
        return (v & val);
    }

    array<bool> operator&& (bool val, const array<bool> &v) {
        return (val & v);
    }

    array<bool> operator|| (const array<bool> &v, const array<bool> &w) {
        return (v | w);
    }

    array<bool> operator|| (const array<bool> &v, bool val) {
        return (v | val);
    }

    array<bool> operator|| (bool val, const array<bool> &v) {
        return (val | v);
    }

    // Relational operators.
    template <class T>
    array<bool> operator== (const array<T> &v, const array<T> &w) {
        if (v.size() != w.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << v.size() << ",) (" << w.size() << ",)";
            throw std::runtime_error(error.str());
        }
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (v[i] == w[i]);
        }
        return out;
    }

    template <class T>
    array<bool> operator== (const array<T> &v, const T &val) {
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (v[i] == val);
        }
        return out;
    }

    template <class T>
    array<bool> operator== (const T &val, const array<T> &v) {
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (val == v[i]);
        }
        return out;
    }

    template <class T>
    array<bool> operator!= (const array<T> &v, const array<T> &w) {
        return !(v == w);
    }

    template <class T>
    array<bool> operator!= (const array<T> &v, const T &val) {
        return !(v == val);
    }

    template <class T>
    array<bool> operator!= (const T &val, const array<T> &v) {
        return !(val == v);
    }

    template <class T>
    array<bool> operator< (const array<T> &v, const array<T> &w) {
        if (v.size() != w.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << v.size() << ",) (" << w.size() << ",)";
            throw std::runtime_error(error.str());
        }
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (v[i] < w[i]);
        }
        return out;
    }

    template <class T>
    array<bool> operator< (const array<T> &v, const T &val) {
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (v[i] < val);
        }
        return out;
    }

    template <class T>
    array<bool> operator< (const T &val, const array<T> &v) {
        array<bool> out(v.size());
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = (val < v[i]);
        }
        return out;
    }

    template <class T>
    array<bool> operator> (const array<T> &v, const array<T> &w) {
        return (w < v);
    }

    template <class T>
    array<bool> operator> (const array<T> &v, const T &val) {
        return (val < v);
    }

    template <class T>
    array<bool> operator> (const T &val, const array<T> &v) {
        return (v < val);
    }

    template <class T>
    array<bool> operator<= (const array<T> &v, const array<T> &w) {
        return !(w < v);
    }

    template <class T>
    array<bool> operator<= (const array<T> &v, const T &val) {
        return !(val < v);
    }

    template <class T>
    array<bool> operator<= (const T &val, const array<T> &v) {
        return !(v < val);
    }

    template <class T>
    array<bool> operator>= (const array<T> &v, const array<T> &w) {
        return !(v < w);
    }

    template <class T>
    array<bool> operator>= (const array<T> &v, const T &val) {
        return !(v < val);
    }

    template <class T>
    array<bool> operator>= (const T &val, const array<T> &v) {
        return !(val < v);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Array indexing                                                         //
    ////////////////////////////////////////////////////////////////////////////

    // Returns a reference to the element at position i in the array.
    template <class T>
    T& array<T>::operator[] (size_t i) {
        return this->values[i];
    }

    template <class T>
    const T& array<T>::operator[] (size_t i) const {
        return this->values[i];
    }

    ////////////////////////////////////////////////////////////////////////////
    // Methods                                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Apply a function to each of the elements in *this.
    template <class T>
    template <class Function>
    void array<T>::apply(Function f) {
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] = f((*this)[i]);
        }
    }

    // Return the index of the maximum value.
    template <class T>
    size_t array<T>::argmax() const {
        size_t index = 0;
        for (size_t i = 1; i < this->size(); ++i) {
            if ((*this)[index] < (*this)[i]) {
                index = i;
            }
        }
        return index;
    }

    // Return the index of the minimum value.
    template <class T>
    size_t array<T>::argmin() const {
        size_t index = 0;
        for (size_t i = 1; i < this->size(); ++i) {
            if ((*this)[i] < (*this)[index]) {
                index = i;
            }
        }
        return index;
    }

    // Returns the indices that would sort this array.
    template <class T>
    array<size_t> array<T>::argsort() const {
        array<size_t> indices(this->size());
        for (size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }
        std::sort(
            indices.data(),
            indices.data() + indices.size(),
            [&](size_t i, size_t j) {
                  return ((*this)[i] == (*this)[j]) ? i < j
                                                    : (*this)[i] < (*this)[j];
            }
        );
        return indices;
    }

    // Clip (limit) the values in the array. Given an interval, values outside
    // the interval are clipped to the interval edges.
    template <class T>
    void array<T>::clip(const T &a_min, const T &a_max) {
        for (size_t i = 0; i < this->size(); ++i) {
            if ((*this)[i] < a_min) {
                (*this)[i] = a_min;
            }
            else if (a_max < (*this)[i]) {
                (*this)[i] = a_max;
            }
        }
    }

    // Return the cumulative product of the elements.
    template <class T>
    array<T> array<T>::cumprod() const {
        array<T> out(this->size());
        out[0] = (*this)[0];
        for (size_t i = 1; i < this->size(); ++i) {
            out[i] = out[i - 1] * (*this)[i];
        }
        return out;
    }

    // Return the cumulative sum of the elements.
    template <class T>
    array<T> array<T>::cumsum() const {
        array<T> out(this->size());
        out[0] = (*this)[0];
        for (size_t i = 1; i < this->size(); ++i) {
            out[i] = out[i - 1] + (*this)[i];
        }
        return out;
    }

    // Returns a pointer to the memory array used internally by the array.
    template <class T>
    T* array<T>::data() {
        return this->values;
    }

    template <class T>
    const T* array<T>::data() const {
        return this->values;
    }

    // Return the dot product of two arrays.
    template <class T>
    T array<T>::dot(const array<T> &v) const {
        if (this->size() != v.size()) {
            std::ostringstream error;
            error << "dot: Number of elements in left operand does not match "
                  << "number of elements in right operand: (" << this->size()
                  << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        T out = T(0);
        for (size_t i = 0; i < this->size(); ++i) {
            out += (*this)[i] * v[i];
        }
        return out;
    }

    // Return the matrix multiplication of a row vector and a matrix.
    template <class T>
    array<T> array<T>::dot(const matrix<T> &A) const {
        if (this->size() != A.rows()) {
            std::ostringstream error;
            error << "matmul: Number of columns in left operand does not match "
                  << "number of rows in right operand: (," << this->size()
                  << ") (" << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        array<T> out(A.columns(), T(0));
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[j] += (*this)[i] * A.at(i, j);
            }
        }
        return out;
    }

    // Returns the maximum value contained in the array.
    template <class T>
    T array<T>::max() const {
        return (*this)[this->argmax()];
    }

    // Returns the average of the array elements.
    template <class T>
    T array<T>::mean() const {
        return this->sum() / this->size();
    }

    // Returns the minimum value contained in the array.
    template <class T>
    T array<T>::min() const {
        return (*this)[this->argmin()];
    }

    // Return the product of the array elements.
    template <class T>
    T array<T>::prod() const {
        T out = T(1);
        for (size_t i = 0; i < this->size(); ++i) {
            out *= (*this)[i];
        }
        return out;
    }

    // Resizes the array, changing its size to n elements.
    // If n is smaller than the current size, the content is reduced to its
    // first n elements, removing those beyond.
    // If n is greater than the current size, the content is expanded by
    // inserting at the end as many elements as needed to reach a size of n.
    template <class T>
    void array<T>::resize(size_t n, const T &val) {
        if (this->length != n) {
            array<T> tmp_copy(*this);
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
    }

    // Returns the number of elements in the array.
    template <class T>
    size_t array<T>::size() const {
        return this->length;
    }

    // Sort an array in-place.
    template <class T>
    void array<T>::sort() {
        std::sort(this->data(), this->data() + this->size());
    }

    // Returns the standard deviation of the array elements.
    template <class T>
    T array<T>::stddev(size_t ddof) const {
        return std::sqrt(this->var(ddof));
    }

    // Return the sum of the array elements.
    template <class T>
    T array<T>::sum() const {
        T out = T(0);
        for (size_t i = 0; i < this->size(); ++i) {
            out += (*this)[i];
        }
        return out;
    }

    // Swap contents with v.
    template <class T>
    void array<T>::swap(array<T> &v) {
        size_t tmp_length = this->length;
        T *tmp_values = this->values;
        this->length = v.length;
        this->values = v.values;
        v.length = tmp_length;
        v.values = tmp_values;
    }

    // Returns the variance of the array elements.
    template <class T>
    T array<T>::var(size_t ddof) const {
        T array_mean = this->mean();
        T out = T(0);
        for (size_t i = 0; i < this->size(); ++i) {
            T deviation = (*this)[i] - array_mean;
            out += deviation*deviation;
        }
        out /= (this->size() - ddof);
        return out;
    }
}
