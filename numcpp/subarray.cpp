#include <cmath>
#include <sstream>

namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Slice indexing                                                         //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by the
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
    void slice_subarray<T>::operator= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = val;
        }
    }

    // Compound assignment.
    template <class T>
    void slice_subarray<T>::operator+= (const array<T> &v) {
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
    void slice_subarray<T>::operator-= (const array<T> &v) {
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
    void slice_subarray<T>::operator*= (const array<T> &v) {
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
    void slice_subarray<T>::operator/= (const array<T> &v) {
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
    void slice_subarray<T>::operator%= (const array<T> &v) {
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
    void slice_subarray<T>::operator&= (const array<T> &v) {
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
    void slice_subarray<T>::operator|= (const array<T> &v) {
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
    void slice_subarray<T>::operator^= (const array<T> &v) {
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
    void slice_subarray<T>::operator<<= (const array<T> &v) {
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
    void slice_subarray<T>::operator>>= (const array<T> &v) {
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

    template <class T>
    void slice_subarray<T>::operator+= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] += val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator-= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] -= val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator*= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] *= val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator/= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] /= val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator%= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] %= val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator&= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] &= val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator|= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] |= val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator^= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] ^= val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator<<= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = val;
        }
    }

    template <class T>
    void slice_subarray<T>::operator>>= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = val;
        }
    }

    // Returns a reference to the element at position i in the sub-array.
    template <class T>
    T& slice_subarray<T>::operator[] (size_t i) {
        return this->values[this->slc.start + i*this->slc.step];
    }

    template <class T>
    const T& slice_subarray<T>::operator[] (size_t i) const {
        return this->values[this->slc.start + i*this->slc.step];
    }

    // Return a copy of the elements selected by the sub-array.
    template <class T>
    array<T> slice_subarray<T>::copy() const {
        size_t n = this->size();
        array<T> out(n);
        for (size_t i = 0; i < n; ++i) {
            out[i] = (*this)[i];
        }
        return out;
    }

    // Returns the number of elements selected by the sub-array.
    template <class T>
    size_t slice_subarray<T>::size() const {
        return ceil(
            ((double)this->slc.stop - (double)this->slc.start) / this->slc.step
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by the
    // integer array.
    template <class T>
    index_subarray<T> array<T>::operator[] (const array<size_t> &index) {
        index_subarray<T> view;
        view.values = this->values;
        view.index = index;
        return view;
    }

    template <class T>
    const index_subarray<T>
    array<T>::operator[] (const array<size_t> &index) const {
        index_subarray<T> view;
        view.values = this->values;
        view.index = index;
        return view;
    }

    // Assignment operators.
    template <class T>
    void index_subarray<T>::operator= (const array<T> &v) {
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
    void index_subarray<T>::operator= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] = val;
        }
    }

    // Compound assignment.
    template <class T>
    void index_subarray<T>::operator+= (const array<T> &v) {
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
    void index_subarray<T>::operator-= (const array<T> &v) {
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
    void index_subarray<T>::operator*= (const array<T> &v) {
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
    void index_subarray<T>::operator/= (const array<T> &v) {
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
    void index_subarray<T>::operator%= (const array<T> &v) {
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
    void index_subarray<T>::operator&= (const array<T> &v) {
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
    void index_subarray<T>::operator|= (const array<T> &v) {
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
    void index_subarray<T>::operator^= (const array<T> &v) {
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
    void index_subarray<T>::operator<<= (const array<T> &v) {
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
    void index_subarray<T>::operator>>= (const array<T> &v) {
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

    template <class T>
    void index_subarray<T>::operator+= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] += val;
        }
    }

    template <class T>
    void index_subarray<T>::operator-= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] -= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator*= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] *= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator/= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] /= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator%= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] %= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator&= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] &= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator|= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] |= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator^= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] ^= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator<<= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] <<= val;
        }
    }

    template <class T>
    void index_subarray<T>::operator>>= (const T &val) {
        size_t n = this->size();
        for (size_t i = 0; i < n; ++i) {
            (*this)[i] >>= val;
        }
    }

    // Returns a reference to the element at position i in the sub-array.
    template <class T>
    T& index_subarray<T>::operator[] (size_t i) {
        return this->values[this->index[i]];
    }

    template <class T>
    const T& index_subarray<T>::operator[] (size_t i) const {
        return this->values[this->index[i]];
    }

    // Return a copy of the elements selected by the sub-array.
    template <class T>
    array<T> index_subarray<T>::copy() const {
        size_t n = this->size();
        array<T> out(n);
        for (size_t i = 0; i < n; ++i) {
            out[i] = (*this)[i];
        }
        return out;
    }

    // Returns the number of elements selected by the sub-array.
    template <class T>
    size_t index_subarray<T>::size() const {
        return this->index.size();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Boolean array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by the
    // boolean array.
    template <class T>
    mask_subarray<T> array<T>::operator[] (const array<bool> &mask) {
        mask_subarray<T> view;
        view.values = this->values;
        view.mask = mask;
        return view;
    }

    template <class T>
    const mask_subarray<T>
    array<T>::operator[] (const array<bool> &mask) const {
        mask_subarray<T> view;
        view.values = this->values;
        view.mask = mask;
        return view;
    }

    // Assignment operators.
    template <class T>
    void mask_subarray<T>::operator= (const array<T> &v) {
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
    void mask_subarray<T>::operator= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] = val;
            }
        }
    }

    // Compound assignment.
    template <class T>
    void mask_subarray<T>::operator+= (const array<T> &v) {
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
    void mask_subarray<T>::operator-= (const array<T> &v) {
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
    void mask_subarray<T>::operator*= (const array<T> &v) {
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
    void mask_subarray<T>::operator/= (const array<T> &v) {
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
    void mask_subarray<T>::operator%= (const array<T> &v) {
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
    void mask_subarray<T>::operator&= (const array<T> &v) {
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
    void mask_subarray<T>::operator|= (const array<T> &v) {
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
    void mask_subarray<T>::operator^= (const array<T> &v) {
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
    void mask_subarray<T>::operator<<= (const array<T> &v) {
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
    void mask_subarray<T>::operator>>= (const array<T> &v) {
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
    void mask_subarray<T>::operator+= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] += val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator-= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] -= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator*= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] *= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator/= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] /= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator%= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] %= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator&= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] &= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator|= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] |= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator^= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] ^= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator<<= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] <<= val;
            }
        }
    }

    template <class T>
    void mask_subarray<T>::operator>>= (const T &val) {
        for (size_t i = 0; i < this->mask.size(); ++i) {
            if (this->mask[i]) {
                this->values[i] >>= val;
            }
        }
    }

    // Return a copy of the elements selected by the sub-array.
    template <class T>
    array<T> mask_subarray<T>::copy() const {
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

    // Returns the number of elements selected by the sub-array.
    template <class T>
    size_t mask_subarray<T>::size() const {
        size_t n = 0;
        for (size_t i = 0; i < this->mask.size(); ++i) {
            n += this->mask[i];
        }
        return n;
    }
}
