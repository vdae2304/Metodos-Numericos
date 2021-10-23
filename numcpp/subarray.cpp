#include <cmath>
#include <sstream>

namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Subarray base class                                                    //
    ////////////////////////////////////////////////////////////////////////////

    // Assignment operators.
    template <class T>
    void subarray<T>::operator= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = v[i];
        }
    }

    template <class T>
    void subarray<T>::operator= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = val;
        }
    }

    // Compound assignment (subarray - array).
    template <class T>
    void subarray<T>::operator+= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] += v[i];
        }
    }

    template <class T>
    void subarray<T>::operator-= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] -= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator*= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] *= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator/= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] /= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator%= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] %= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator&= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] &= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator|= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] |= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator^= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] ^= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator<<= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] <<= v[i];
        }
    }

    template <class T>
    void subarray<T>::operator>>= (const array<T> &v) {
        size_t n = this->size();
        if (n != v.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << n << ",) (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] >>= v[i];
        }
    }

    // Compound assignment (subarray - scalar).
    template <class T>
    void subarray<T>::operator+= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] += val;
        }
    }

    template <class T>
    void subarray<T>::operator-= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] -= val;
        }
    }

    template <class T>
    void subarray<T>::operator*= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] *= val;
        }
    }

    template <class T>
    void subarray<T>::operator/= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] /= val;
        }
    }

    template <class T>
    void subarray<T>::operator%= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] %= val;
        }
    }

    template <class T>
    void subarray<T>::operator&= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] &= val;
        }
    }

    template <class T>
    void subarray<T>::operator|= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] |= val;
        }
    }

    template <class T>
    void subarray<T>::operator^= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] ^= val;
        }
    }

    template <class T>
    void subarray<T>::operator<<= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = val;
        }
    }

    template <class T>
    void subarray<T>::operator>>= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = val;
        }
    }

    // Returns a reference to the element at position i in the subarray.
    template <class T>
    T& subarray<T>::operator[] (size_t i) {
        return this->values[i];
    }

    template <class T>
    const T& subarray<T>::operator[] (size_t i) const {
        return this->values[i];
    }

    // Returns the number of elements selected by the subarray.
    template <class T>
    size_t subarray<T>::size() const {
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Slice indexing                                                         //
    ////////////////////////////////////////////////////////////////////////////

    // Return a slice_subarray object that selects the elements specified by the
    // slice.
    template <class T>
    slice_subarray<T> array<T>::operator[] (slice slc) {
        slice_subarray<T> view;
        view.values = this->values;
        view.slc = slc;
        return view;
    }

    template <class T>
    const slice_subarray<T> array<T>::operator[] (slice slc) const {
        slice_subarray<T> view;
        view.values = this->values;
        view.slc = slc;
        return view;
    }

    // Assignment operators.
    template <class T>
    void slice_subarray<T>::operator= (const array<T> &v) {
        subarray<T>::operator= (v);
    }

    template <class T>
    void slice_subarray<T>::operator= (const T &val) {
        subarray<T>::operator= (val);
    }

    // Returns a reference to the element at position i in the subarray.
    template <class T>
    T& slice_subarray<T>::operator[] (size_t i) {
        return this->values[this->slc.start + i*this->slc.step];
    }

    template <class T>
    const T& slice_subarray<T>::operator[] (size_t i) const {
        return this->values[this->slc.start + i*this->slc.step];
    }

    // Returns the number of elements selected by the subarray.
    template <class T>
    size_t slice_subarray<T>::size() const {
        return ceil(
            ((double)this->slc.stop - (double)this->slc.start) / this->slc.step
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a index_subarray object that selects the elements specified by the
    // integer array.
    template <class T>
    index_subarray<T> array<T>::operator[] (const array<size_t> &index) {
        index_subarray<T> view;
        view.values = this->values;
        view.index = index;
        return view;
    }

    template <class T>
    const index_subarray<T> array<T>::operator[] (const array<size_t> &index) 
    const {
        index_subarray<T> view;
        view.values = this->values;
        view.index = index;
        return view;
    }

    // Assignment operators.
    template <class T>
    void index_subarray<T>::operator= (const array<T> &v) {
        subarray<T>::operator= (v);
    }

    template <class T>
    void index_subarray<T>::operator= (const T &val) {
        subarray<T>::operator= (val);
    }

    // Returns a reference to the element at position i in the subarray.
    template <class T>
    T& index_subarray<T>::operator[] (size_t i) {
        return this->values[this->index[i]];
    }

    template <class T>
    const T& index_subarray<T>::operator[] (size_t i) const {
        return this->values[this->index[i]];
    }

    // Returns the number of elements selected by the subarray.
    template <class T>
    size_t index_subarray<T>::size() const {
        return this->index.size();
    }
}
