#include <cmath>
#include <sstream>

namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Submatrix base class                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Assignment operators.
    template <class T>
    void submatrix<T>::operator= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) = A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) = val;
            }
        }
    }

    // Compound assignment (submatrix - matrix).
    template <class T>
    void submatrix<T>::operator+= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) += A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator-= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) -= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator*= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) *= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator/= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) /= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator%= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) %= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator&= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) &= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator|= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) |= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator^= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) ^= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator<<= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) <<= A[i][j];
            }
        }
    }

    template <class T>
    void submatrix<T>::operator>>= (const matrix<T> &A) {
        size_t m = this->rows(), n = this->columns();
        if (m != A.rows() || n != A.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << m << "," << n << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) >>= A[i][j];
            }
        }
    }

    // Compound assignment (submatrix - scalar).
    template <class T>
    void submatrix<T>::operator+= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) += val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator-= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) -= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator*= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) *= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator/= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) /= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator%= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) %= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator&= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) &= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator|= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) |= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator^= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) ^= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator<<= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) <<= val;
            }
        }
    }

    template <class T>
    void submatrix<T>::operator>>= (const T &val) {
        size_t m = this->rows(), n = this->columns();
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                this->at(i, j) >>= val;
            }
        }
    }

    template <class T>
    T& submatrix<T>::at(size_t i, size_t j) {
        return (*this->values)[i][j];
    }

    template <class T>
    const T& submatrix<T>::at(size_t i, size_t j) const {
        return (*this->values)[i][j];
    }

    template <class T>
    size_t submatrix<T>::rows() const {
        return 0;
    }

    template <class T>
    size_t submatrix<T>::columns() const {
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Slice-Slice indexing                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    slice_slice_submatrix<T> matrix<T>::at(slice i, slice j) {
        slice_slice_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    template <class T>
    const slice_slice_submatrix<T> matrix<T>::at(slice i, slice j) const {
        slice_slice_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    // Assignment operators.
    template <class T>
    void slice_slice_submatrix<T>::operator= (const matrix<T> &A) {
        submatrix<T>::operator= (A);
    }

    template <class T>
    void slice_slice_submatrix<T>::operator= (const T &val) {
        submatrix<T>::operator= (val);
    }

    // Returns a reference to the element at row i and column j in the the 
    // submatrix.
    template <class T>
    T& slice_slice_submatrix<T>::at(size_t i, size_t j) {
        return (*this->values)[this->i.start + i*this->i.step]
                              [this->j.start + j*this->j.step];
    }

    template <class T>
    const T& slice_slice_submatrix<T>::at(size_t i, size_t j) const {
        return (*this->values)[this->i.start + i*this->i.step]
                              [this->j.start + j*this->j.step];
    }

    // Returns the number of columns selected by the sub-matrix.
    template <class T>
    size_t slice_slice_submatrix<T>::columns() const {
        return ceil(
            ((double)this->j.stop - (double)this->j.start) / this->j.step
        );
    }

    // Returns the number of rows selected by the sub-matrix.
    template <class T>
    size_t slice_slice_submatrix<T>::rows() const {
        return ceil(
            ((double)this->i.stop - (double)this->i.start) / this->i.step
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Slice-Integer array indexing                                           //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    slice_index_submatrix<T> matrix<T>::at(slice i, const array<size_t> &j) {
        slice_index_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    template <class T>
    const slice_index_submatrix<T> 
    matrix<T>::at(slice i, const array<size_t> &j) const {
        slice_index_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    // Assignment operators.
    template <class T>
    void slice_index_submatrix<T>::operator= (const matrix<T> &A) {
        submatrix<T>::operator= (A);
    }

    template <class T>
    void slice_index_submatrix<T>::operator= (const T &val) {
        submatrix<T>::operator= (val);
    }

    // Returns a reference to the element at row i and column j in the the 
    // submatrix.
    template <class T>
    T& slice_index_submatrix<T>::at(size_t i, size_t j) {
        return (*this->values)[this->i.start + i*this->i.step][this->j[j]];
    }

    template <class T>
    const T& slice_index_submatrix<T>::at(size_t i, size_t j) const {
        return (*this->values)[this->i.start + i*this->i.step][this->j[j]];
    }

    // Returns the number of columns selected by the sub-matrix.
    template <class T>
    size_t slice_index_submatrix<T>::columns() const {
        return this->j.size();
    }

    // Returns the number of rows selected by the sub-matrix.
    template <class T>
    size_t slice_index_submatrix<T>::rows() const {
        return ceil(
            ((double)this->i.stop - (double)this->i.start) / this->i.step
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer array-Slice indexing                                           //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    index_slice_submatrix<T> matrix<T>::at(const array<size_t> &i, slice j) {
        index_slice_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    template <class T>
    const index_slice_submatrix<T> 
    matrix<T>::at(const array<size_t> &i, slice j) const {
        index_slice_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    // Assignment operators.
    template <class T>
    void index_slice_submatrix<T>::operator= (const matrix<T> &A) {
        submatrix<T>::operator= (A);
    }

    template <class T>
    void index_slice_submatrix<T>::operator= (const T &val) {
        submatrix<T>::operator= (val);
    }

    // Returns a reference to the element at row i and column j in the the 
    // submatrix.
    template <class T>
    T& index_slice_submatrix<T>::at(size_t i, size_t j) {
        return (*this->values)[this->i[i]][this->j.start + j*this->j.step];
    }

    template <class T>
    const T& index_slice_submatrix<T>::at(size_t i, size_t j) const {
        return (*this->values)[this->i[i]][this->j.start + j*this->j.step];
    }

    // Returns the number of columns selected by the sub-matrix.
    template <class T>
    size_t index_slice_submatrix<T>::columns() const {
        return ceil(
            ((double)this->j.stop - (double)this->j.start) / this->j.step
        );
    }

    // Returns the number of rows selected by the sub-matrix.
    template <class T>
    size_t index_slice_submatrix<T>::rows() const {
        return this->i.size();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer array-Integer array indexing                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    index_index_submatrix<T> 
    matrix<T>::at(const array<size_t> &i, const array<size_t> &j) {
        index_index_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    template <class T>
    const index_index_submatrix<T> 
    matrix<T>::at(const array<size_t> &i, const array<size_t> &j) const {
        index_slice_submatrix<T> view;
        view.values = this;
        view.i = i;
        view.j = j;
        return view;
    }

    // Assignment operators.
    template <class T>
    void index_index_submatrix<T>::operator= (const matrix<T> &A) {
        submatrix<T>::operator= (A);
    }

    template <class T>
    void index_index_submatrix<T>::operator= (const T &val) {
        submatrix<T>::operator= (val);
    }

    // Returns a reference to the element at row i and column j in the the 
    // submatrix.
    template <class T>
    T& index_index_submatrix<T>::at(size_t i, size_t j) {
        return (*this->values)[this->i[i]][this->j[j]];
    }

    template <class T>
    const T& index_index_submatrix<T>::at(size_t i, size_t j) const {
        return (*this->values)[this->i[i]][this->j[j]];
    }

    // Returns the number of columns selected by the sub-matrix.
    template <class T>
    size_t index_index_submatrix<T>::columns() const {
        return this->j.size();
    }

    // Returns the number of rows selected by the sub-matrix.
    template <class T>
    size_t index_index_submatrix<T>::rows() const {
        return this->i.size();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer-Slice indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    slice_subarray<T> matrix<T>::at(size_t i, slice j) {
        slice_subarray<T> view;
        view.values = this->values;
        view.slc = slice(i*this->ncols + j.start, 
                         i*this->ncols + j.stop, 
                         j.step);
        return view;
    }

    template <class T>
    const slice_subarray<T> matrix<T>::at(size_t i, slice j) const {
        slice_subarray<T> view;
        view.values = this->values;
        view.slc = slice(i*this->ncols + j.start, 
                         i*this->ncols + j.stop, 
                         j.step);
        return view;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Slice-Integer indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    slice_subarray<T> matrix<T>::at(slice i, size_t j) {
        slice_subarray<T> view;
        view.values = this->values;
        view.slc = slice(i.start*this->ncols + j, 
                         i.stop*this->ncols + j, 
                         i.step*this->ncols);
        return view;
    }

    template <class T>
    const slice_subarray<T> matrix<T>::at(slice i, size_t j) const {
        slice_subarray<T> view;
        view.values = this->values;
        view.slc = slice(i.start*this->ncols + j, 
                         i.stop*this->ncols + j, 
                         i.step*this->ncols);
        return view;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer-Integer array indexing                                         //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    index_subarray<T> matrix<T>::at(size_t i, const array<size_t> &j) {
        index_subarray<T> view;
        view.values = this->values;
        view.index = array<size_t>(j.size());
        for (size_t k = 0; k < j.size(); ++k) {
            view.index[k] = i * this->ncols + j[k];
        }
        return view;
    }

    template <class T>
    const index_subarray<T> 
    matrix<T>::at(size_t i, const array<size_t> &j) const {
        index_subarray<T> view;
        view.values = this->values;
        view.index = array<size_t>(j.size());
        for (size_t k = 0; k < j.size(); ++k) {
            view.index[k] = i * this->ncols + j[k];
        }
        return view;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer array-Integer indexing                                         //
    ////////////////////////////////////////////////////////////////////////////

    // Return a submatrix object that selects the elements specified by the
    // slices.
    template <class T>
    index_subarray<T> matrix<T>::at(const array<size_t> &i, size_t j) {
        index_subarray<T> view;
        view.values = this->values;
        view.index = array<size_t>(i.size());
        for (size_t k = 0; k < i.size(); ++k) {
            view.index[k] = i[k] * this->ncols + j;
        }
        return view;
    }

    template <class T>
    const index_subarray<T> 
    matrix<T>::at(const array<size_t> &i, size_t j) const {
        index_subarray<T> view;
        view.values = this->values;
        view.index = array<size_t>(i.size());
        for (size_t k = 0; k < i.size(); ++k) {
            view.index[k] = i[k] * this->ncols + j;
        }
        return view;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Coordinate array indexing                                              //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by the
    // array of pairs.
    template <class T>
    index_subarray<T>
    matrix<T>::at(const array< std::pair<size_t, size_t> > &index) {
        index_subarray<T> view;
        view.values = this->values;
        view.index = array<size_t>(index.size());
        for (size_t k = 0; k < index.size(); ++k) {
            view.index[k] = index[k].first * this->ncols + index[k].second;
        }
        return view;
    }

    template <class T>
    const index_subarray<T>
    matrix<T>::at(const array< std::pair<size_t, size_t> > &index) const {
        index_subarray<T> view;
        view.values = this->values;
        view.index = array<size_t>(index.size());
        for (size_t k = 0; k < index.size(); ++k) {
            view.index[k] = index[k].first * this->ncols + index[k].second;
        }
        return view;
    }
}
