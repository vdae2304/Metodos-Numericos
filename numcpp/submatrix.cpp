#include <cmath>
#include <sstream>

namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Slice indexing                                                         //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-matrix object that selects the elements specified by the
    // slice.
    template <class T>
    submatrix<T> matrix<T>::at(slice i, slice j) {
        submatrix<T> view;
        view.parent = this;
        view.i = i;
        view.j = j;
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(slice i, slice j) const {
        submatrix<T> view;
        view.parent = this;
        view.i = i;
        view.j = j;
        return view;
    }

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

    // Compound assignment.
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

    // Matrix subscript. Returns a reference to the element at row i and column
    // j in the the sub-matrix.
    template <class T>
    T& submatrix<T>::at(size_t i, size_t j) {
        return (*this->parent)[this->i.start + i*this->i.step]
                              [this->j.start + j*this->j.step];
    }

    template <class T>
    const T& submatrix<T>::at(size_t i, size_t j) const {
        return (*this->parent)[this->i.start + i*this->i.step]
                              [this->j.start + j*this->j.step];
    }

    // Returns the number of columns selected by the sub-matrix.
    template <class T>
    size_t submatrix<T>::columns() const {
        return ceil(
            ((double)this->j.stop - (double)this->j.start) / this->j.step
        );
    }

    // Return a copy of the elements selected by the sub-matrix.
    template <class T>
    matrix<T> submatrix<T>::copy() const {
        size_t m = this->rows(), n = this->columns();
        matrix<T> out(m, n);
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                out[i][j] = this->at(i, j);
            }
        }
        return out;
    }

    // Returns the number of rows selected by the sub-matrix.
    template <class T>
    size_t submatrix<T>::rows() const {
        return ceil(
            ((double)this->i.stop - (double)this->i.start) / this->i.step
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integer array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by the
    // integer array.
    template <class T>
    index_subarray<T>
    matrix<T>::at(const array<size_t> &i, const array<size_t> &j) {
        index_subarray<T> view;
        view.values = this->values;
        view.index.resize(std::min(i.size(), j.size()));
        for (size_t k = 0; k < view.index.size(); ++k) {
            view.index[k] = i[k] * this->ncols + j[k];
        }
        return view;
    }

    template <class T>
    const index_subarray<T>
    matrix<T>::at(const array<size_t> &i, const array<size_t> &j) const {
        index_subarray<T> view;
        view.values = this->values;
        view.index.resize(std::min(i.size(), j.size()));
        for (size_t k = 0; k < view.index.size(); ++k) {
            view.index[k] = i[k] * this->ncols + j[k];
        }
        return view;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Boolean array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Return a sub-array object that selects the elements specified by the
    // boolean array.
    template <class T>
    mask_subarray<T> matrix<T>::at(const matrix<bool> &mask) {
        mask_subarray<T> view;
        view.values = this->values;
        view.mask = mask.flatten();
        return view;
    }

    template <class T>
    const mask_subarray<T> matrix<T>::at(const matrix<bool> &mask) const {
        mask_subarray<T> view;
        view.values = this->values;
        view.mask = mask.flatten();
        return view;
    }
}
