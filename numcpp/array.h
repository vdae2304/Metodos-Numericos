#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

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

    // Move constructor. Constructs an array that acquires the elements of v.
    template <class T>
    array<T>::array(array<T> &&v) {
        this->length = v.length;
        this->values = v.values;
        v.length = 0;
        v.values = nullptr;
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
    // Array creation routines                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Return a new uninitialized array.
    template <class T>
    array<T> empty(size_t n) {
        return array<T>(n);
    }

    // Return a new array setting values to zero.
    template <class T>
    array<T> zeros(size_t n) {
        return array<T>(n, T(0));
    }

    // Return a new array setting values to one.
    template <class T>
    array<T> ones(size_t n) {
        return array<T>(n, T(1));
    }

    // Return a new array of given length filled with value.
    template <class T>
    array<T> full(size_t n, const T &val) {
        return array<T>(n, val);
    }

    // Return a new array from a binary file.
    template <class T>
    array<T> load(const char *file) {
        FILE *pfile = fopen(file, "rb");
        if (pfile != NULL) {
            size_t n;
            fread(&n, sizeof(size_t), 1, pfile);
            array<T> out(n);
            fread(out.data(), sizeof(T), n, pfile);
            fclose(pfile);
            return out;
        }
        else {
            std::ostringstream error;
            error << "No such file or directory: " << file;
            throw std::runtime_error(error.str());
        }
    }

    // Save an array to a binary file.
    template <class T>
    void save(const char *file, const array<T> &v) {
        FILE *pfile = fopen(file, "wb");
        if (pfile != NULL) {
            size_t n = v.size();
            fwrite(&n, sizeof(size_t), 1, pfile);
            fwrite(v.data(), sizeof(T), n, pfile);
            fclose(pfile);
        }
        else {
            std::ostringstream error;
            error << "Can not modify: " << file;
            throw std::runtime_error(error.str());
        }
    }

    // Return evenly spaced values within a given interval. Values are
    // generated within the half-open interval [start, stop) (in other words,
    // the interval including start but excluding stop).
    template <class T>
    array<T> arange(const T &stop) {
        size_t n = ceil(stop);
        array<T> out(n);
        for (size_t i = 0; i < n; ++i) {
            out[i] = T(i);
        }
        return out;
    }

    template <class T>
    array<T> arange(const T &start, const T &stop, const T &step) {
        size_t n = ceil((stop - start) / (double)step);
        array<T> out(n);
        for (size_t i = 0; i < n; ++i) {
            out[i] = start + i*step;
        }
        return out;
    }

    // Return evenly spaced numbers over a specified interval. Returns num
    // evenly spaced samples, calculated over the interval [start, stop].
    // The endpoint of the interval can optionally be excluded.
    template <class T>
    array<T> linspace(
        const T &start, const T &stop,
        size_t num,
        bool endpoint
    ) {
        array<T> out(num);
        T step = (stop - start) / (num - endpoint);
        for (size_t i = 0; i < num; ++i) {
            out[i] = start + i*step;
        }
        return out;
    }

    // Return numbers spaced evenly on a log scale. In linear space, the
    // sequence starts at pow(base, start) (base to the power of start) and
    // ends with pow(base, stop).
    template <class T>
    array<T> logspace(
        const T &start, const T &stop,
        size_t num, bool endpoint,
        const T &base
    ) {
        array<T> out(num);
        T step = (stop - start) / (num - endpoint);
        for (size_t i = 0; i < num; ++i) {
            out[i] = pow(base, start + i*step);
        }
        return out;
    }

    // Return numbers spaced evenly on a log scale (a geometric progression).
    // This is similar to logspace, but with endpoints specified directly. Each
    // output sample is a constant multiple of the previous.
    template <class T>
    array<T> geomspace(
        const T &start, const T &stop,
        size_t num,
        bool endpoint
    ) {
        array<T> out(num);
        T base = pow(stop/start, 1.0/(num - endpoint));
        for (size_t i = 0; i < num; ++i) {
            out[i] = start * pow(base, i);
        }
        return out;
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

    // Arithmetic operators.
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

    // Bitwise operators.
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

    // Relational operators.
    template<class T>
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
        return !(v < w);
    }

    template<class T>
    array<bool> operator>= (const array<T> &v, const T &val) {
        return !(v < val);
    }

    template<class T>
    array<bool> operator>= (const T &val, const array<T> &v) {
        return !(val < v);
    }

    // Standard output.
    template <class T>
    std::ostream& operator<< (std::ostream &ostr, const array<T> &v) {
        std::ios_base::fmtflags default_flags(ostr.flags());
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

        ostr.setf(default_flags);
        return ostr;
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
    // Slice indexing                                                         //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by its
    // argument.
    template <class T>
    subarray<T, slice> array<T>::operator[] (const slice &slc) {
        subarray<T, slice> view;
        view.values = this->values;
        view.slc = slc;
        return view;
    }

    template <class T>
    const subarray<T, slice> array<T>::operator[] (const slice &slc) const {
        subarray<T, slice> view;
        view.values = this->values;
        view.slc = slc;
        return view;
    }

    // Assignment operators.
    template <class T>
    void subarray<T, slice>::operator= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] = v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] = val;
        }
    }

    // Compound assignment operators.
    template <class T>
    void subarray<T, slice>::operator+= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] += v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator-= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] -= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator*= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] *= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator/= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] /= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator%= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] %= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator&= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] &= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator|= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] |= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator^= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] ^= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator<<= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] <<= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator>>= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] >>= v[i];
        }
    }

    template <class T>
    void subarray<T, slice>::operator+= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] += val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator-= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] -= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator*= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] *= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator/= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] /= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator%= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] %= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator&= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] &= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator|= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] |= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator^= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] ^= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator<<= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] <<= val;
        }
    }

    template <class T>
    void subarray<T, slice>::operator>>= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->slc.start + i*this->slc.step] >>= val;
        }
    }

    // Return a copy of the elements selected by the slice.
    template <class T>
    array<T> subarray<T, slice>::copy() const {
        size_t n = this->size();
        array<T> out(n);
        for (size_t i = 0; i < n; ++i) {
            out[i] = this->values[this->slc.start + i*this->slc.step];
        }
        return out;
    }

    // Returns the number of elements selected by the slice.
    template <class T>
    size_t subarray<T, slice>::size() const {
        return ceil((double)(this->slc.stop - this->slc.start)/this->slc.step);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by its
    // argument.
    template <class T>
    subarray<T, size_t> array<T>::operator[] (const array<size_t> &indices) {
        subarray<T, size_t> view;
        view.values = this->values;
        view.indices = indices;
        return view;
    }

    template <class T>
    const subarray<T, size_t>
    array<T>::operator[] (const array<size_t> &indices) const {
        subarray<T, size_t> view;
        view.values = this->values;
        view.indices = indices;
        return view;
    }

    // Assignment operators.
    template <class T>
    void subarray<T, size_t>::operator= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] = v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] = val;
        }
    }

    // Compound assignment.
    template <class T>
    void subarray<T, size_t>::operator+= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] += v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator-= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] -= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator*= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] *= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator/= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] /= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator%= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] %= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator&= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] &= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator|= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] |= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator^= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] ^= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator<<= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] <<= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator>>= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] >>= v[i];
        }
    }

    template <class T>
    void subarray<T, size_t>::operator+= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] += val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator-= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] -= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator*= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] *= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator/= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] /= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator%= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] %= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator&= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] &= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator|= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] |= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator^= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] ^= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator<<= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] <<= val;
        }
    }

    template <class T>
    void subarray<T, size_t>::operator>>= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            this->values[this->indices[i]] >>= val;
        }
    }

    // Return a copy of the elements selected by the indices.
    template <class T>
    array<T> subarray<T, size_t>::copy() const {
        size_t n = this->size();
        array<T> out(n);
        for (size_t i = 0; i < n; ++i) {
            out[i] = this->values[this->indices[i]];
        }
        return out;
    }

    // Returns the number of elements selected by the indices.
    template <class T>
    size_t subarray<T, size_t>::size() const {
        return this->indices.size();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Boolean array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by its
    // argument.
    template <class T>
    subarray<T, bool> array<T>::operator[] (const array<bool> &mask) {
        subarray<T, bool> view;
        view.values = this->values;
        view.mask = mask;
        return view;
    }

    template <class T>
    const subarray<T, bool>
    array<T>::operator[] (const array<bool> &mask) const {
        subarray<T, bool> view;
        view.values = this->values;
        view.mask = mask;
        return view;
    }

    // Assignment operators.
    template <class T>
    void subarray<T, bool>::operator= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] = v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] = val;
            }
        }
    }

    // Compound assignment.
    template <class T>
    void subarray<T, bool>::operator+= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] += v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator-= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] -= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator*= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] *= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator/= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] /= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator%= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] %= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator&= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] &= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator|= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] |= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator^= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] ^= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator<<= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] <<= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator>>= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] >>= v[n++];
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator+= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] += val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator-= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] -= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator*= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] *= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator/= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] /= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator%= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] %= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator&= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] &= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator|= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] |= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator^= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] ^= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator<<= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] <<= val;
            }
        }
    }

    template <class T>
    void subarray<T, bool>::operator>>= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] >>= val;
            }
        }
    }

    // Return a copy of the elements selected by the mask.
    template <class T>
    array<T> subarray<T, bool>::copy() const {
        size_t n = this->size();
        array<T> out(n);
        n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                out[n++] = this->values[i];
            }
        }
        return out;
    }

    // Returns the number of elements selected by the mask.
    template <class T>
    size_t subarray<T, bool>::size() const {
        size_t n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            n += this->mask[i];
        }
        return n;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Methods                                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Apply a function to each of the elements in *this.
    template <class T>
    template <class Function>
    void array<T>::apply(Function f) {
        for (size_t i = 0; i < this->length; ++i) {
            this->values[i] = f(this->values[i]);
        }
    }

    // Return the index of the maximum value.
    template <class T>
    size_t array<T>::argmax() const {
        size_t index = 0;
        for (size_t i = 1; i < this->length; ++i) {
            if (this->values[index] < this->values[i]) {
                index = i;
            }
        }
        return index;
    }

    // Return the index of the minimum value.
    template <class T>
    size_t array<T>::argmin() const {
        size_t index = 0;
        for (size_t i = 1; i < this->length; ++i) {
            if (this->values[i] < this->values[index]) {
                index = i;
            }
        }
        return index;
    }

    // Returns the indices that would sort this array.
    template <class T>
    array<size_t> array<T>::argsort() const {
        array<size_t> indices(this->length);
        for (size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }
        std::sort(
            indices.data(),
            indices.data() + indices.size(),
            [=](size_t i, size_t j) {
                  return this->values[i] < this->values[j];
            }
        );
        return indices;
    }

    // Copy of the array, cast to a specified type.
    template <class T>
    template <class U>
    array<U> array<T>::astype() const {
        array<U> out(this->length);
        for (size_t i = 0; i < this->length; ++i) {
            out[i] = U(this->values[i]);
        }
        return out;
    }

    // Clip (limit) the values in the array. Given an interval, values outside
    // the interval are clipped to the interval edges.
    template <class T>
    void array<T>::clip(const T &a_min, const T &a_max) {
        for (size_t i = 0; i < this->length; ++i) {
            if (this->values[i] < a_min) {
                this->values[i] = a_min;
            }
            else if (a_max < this->values[i]) {
                this->values[i] = a_max;
            }
        }
    }

    // Return the cumulative product of the elements.
    template <class T>
    array<T> array<T>::cumprod() const {
        array<T> out(this->length);
        out[0] = this->values[0];
        for (size_t i = 1; i < this->length; ++i) {
            out[i] = out[i - 1] * this->values[i];
        }
        return out;
    }

    // Return the cumulative sum of the elements.
    template <class T>
    array<T> array<T>::cumsum() const {
        array<T> out(this->length);
        out[0] = this->values[0];
        for (size_t i = 1; i < this->length; ++i) {
            out[i] = out[i - 1] + this->values[i];
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
        if (this->length != v.length) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->length << ",) (" << v.length << ",)";
            throw std::runtime_error(error.str());
        }
        T out = T(0);
        for (size_t i = 0; i < this->length; ++i) {
            out += this->values[i] * v.values[i];
        }
        return out;
    }

    // Returns the maximum value contained in the array.
    template <class T>
    T array<T>::max() const {
        return this->values[this->argmax()];
    }

    // Returns the average of the array elements.
    template <class T>
    T array<T>::mean() const {
        return this->sum() / this->length;
    }

    // Returns the minimum value contained in the array.
    template <class T>
    T array<T>::min() const {
        return this->values[this->argmin()];
    }

    // Return the product of the array elements.
    template <class T>
    T array<T>::prod() const {
        T out = T(1);
        for (size_t i = 0; i < this->length; ++i) {
            out *= this->values[i];
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

    // Returns the number of elements in the array.
    template <class T>
    size_t array<T>::size() const {
        return this->length;
    }

    // Sort an array in-place.
    template <class T>
    void array<T>::sort() {
        std::sort(this->values, this->values + this->length);
    }

    // Returns the standard deviation of the array elements.
    template <class T>
    T array<T>::stddev(size_t ddof) const {
        return sqrt(this->var(ddof));
    }

    // Return the sum of the array elements.
    template <class T>
    T array<T>::sum() const {
        T out = T(0);
        for (size_t i = 0; i < this->length; ++i) {
            out += this->values[i];
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
        for (size_t i = 0; i < this->length; ++i) {
            T deviation = this->values[i] - array_mean;
            out += deviation*deviation;
        }
        out /= (this->length - ddof);
        return out;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Global functions                                                       //
    ////////////////////////////////////////////////////////////////////////////

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

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding element in v.
    template <class T, class Function>
    array<T> apply(Function f, const array<T> &v) {
        array<T> out(v);
        out.apply(f);
        return out;
    }

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding element in v and w.
    template <class T, class Function>
    array<T> apply(Function f, const array<T> &v, const array<T> &w) {
        if (v.size() != w.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << v.size() << ",) (" << w.size() << ",)";
            throw std::runtime_error(error.str());
        }
        array<T> out(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            out[i] = f(v[i], w[i]);
        }
        return out;
    }

    template <class T, class Function>
    array<T> apply(Function f, const array<T> &v, const T &val) {
        array<T> w(v.size(), val);
        return apply(f, v, w);
    }

    template <class T, class Function>
    array<T> apply(Function f, const T &val, const array<T> &v) {
        array<T> w(v.size(), val);
        return apply(f, w, v);
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
        array<T> clipped(v);
        clipped.clip(a_min, a_max);
        return clipped;
    }

    // Concatenate (join) two arrays.
    template <class T>
    array<T> concatenate(const array<T> &v, const array<T> &w) {
        array<T> out(v.size() + w.size());
        for (size_t i = 0; i < v.size(); ++i) {
            out[i] = v[i];
        }
        for (size_t i = 0; i < w.size(); ++i) {
            out[v.size() + i] = w[i];
        }
        return out;
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

    // Delete values from an array.
    template <class T>
    array<T> erase(const array<T> &v, size_t index) {
        array<T> out(v.size() - 1);
        for (size_t i = 0; i < index; ++i) {
            out[i] = v[i];
        }
        for (size_t i = index + 1; i < v.size(); ++i) {
            out[i - 1] = v[i];
        }
        return out;
    }

    template <class T>
    array<T> erase(const array<T> &v, const array<size_t> indices) {
        array<bool> keep(v.size(), true);
        for (size_t i = 0; i < indices.size(); ++i) {
            keep[indices[i]] = false;
        }
        array<T> out(v.size() - indices.size());
        size_t n = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            if (keep[i]) {
                out[n++] = v[i];
            }
        }
        return out;
    }

    // Insert values before the given indices.
    template <class T>
    array<T> insert(const array<T> &v, size_t index, const T &value) {
        array<T> out(v.size() + 1);
        for (size_t i = 0; i < index; ++i) {
            out[i] = v[i];
        }
        out[index] = value;
        for (size_t i = index + 1; i < out.size(); ++i) {
            out[i] = v[i - 1];
        }
        return out;
    }

    template <class T>
    array<T> insert(
        const array<T> &v, const array<size_t> &indices, const array<T> &values
    ) {
        if (indices.size() != values.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << indices.size() << ",) (" << values.size() << ",)";
            throw std::runtime_error(error.str());
        }
        array<T> out(v.size() + indices.size());
        array<size_t> sorted = indices.argsort();
        size_t n = 0, j = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            while (j < indices.size() && indices[sorted[j]] == i) {
                out[n++] = values[sorted[j++]];
            }
            out[n++] = v[i];
        }
        while (j < indices.size()) {
            out[n++] = values[sorted[j++]];
        }
        return out;
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
    T stddev(const array<T> &v, size_t ddof) {
        return v.stddev(ddof);
    }

    // Return the sum of the array elements.
    template <class T>
    T sum(const array<T> &v) {
        return v.sum();
    }

    // Swap contents between two arrays.
    template <class T>
    void swap(array<T> &v, array<T> &w) {
        v.swap(w);
    }

    // Returns the variance of the array elements.
    template <class T>
    T var(const array<T> &v, size_t ddof) {
        return v.var(ddof);
    }

    // Return the indices of the elements that evaluate to true.
    array<size_t> where(const array<bool> &condition) {
        size_t n = 0;
        for (size_t i = 0; i < condition.size(); ++i) {
            n += condition[i];
        }
        array<size_t> out(n);
        n = 0;
        for (size_t i = 0; i < condition.size(); ++i) {
            if (condition[i]) {
                out[n++] = i;
            }
        }
        return out;
    }

    // Return elements chosen from v or w depending on condition.
    template <class T>
    array<T> where(const array<bool> &condition, const array<T> &expr_true) {
        size_t n = 0;
        for (size_t i = 0; i < condition.size(); ++i) {
            n += condition[i];
        }
        array<T> out(n);
        n = 0;
        for (size_t i = 0; i < condition.size(); ++i) {
            if (condition[i]) {
                out[n++] = expr_true[i];
            }
        }
        return out;
    }

    template <class T>
    array<T> where(
       const array<bool> &condition,
       const array<T> &expr_true, const array<T> &expr_false
    ) {
        array<T> out(condition.size());
        for (size_t i = 0; i < condition.size(); ++i) {
            out[i] = condition[i] ? expr_true[i] : expr_false[i];
        }
        return out;
    }
}

#endif // ARRAY_H_INCLUDED
