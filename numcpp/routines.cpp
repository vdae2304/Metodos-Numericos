namespace numcpp {
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
        const T &start, const T &stop, size_t num, bool endpoint
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
        const T &start, const T &stop, size_t num, bool endpoint, const T &base
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
        const T &start, const T &stop, size_t num, bool endpoint
    ) {
        array<T> out(num);
        T base = pow(stop/start, 1.0/(num - endpoint));
        for (size_t i = 0; i < num; ++i) {
            out[i] = start * pow(base, i);
        }
        return out;
    }

    // Extract a diagonal from a matrix.
    template <class T>
    array<T> diagonal(const matrix<T> &A, int offset) {
        size_t start = (offset >= 0) ? 0 : -offset;
        size_t stop = min(A.rows(), A.columns() - offset);
        array<T> out(stop - start);
        for (size_t i = start; i < stop; ++i) {
            out[i - start] = A[i][i + offset];
        }
        return out;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Matrix creation routines                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Return a new uninitialized matrix.
    template <class T>
    matrix<T> empty(size_t m, size_t n) {
        return matrix<T>(m, n);
    }

    // Return a new matrix setting values to zero.
    template <class T>
    matrix<T> zeros(size_t m, size_t n) {
        return matrix<T>(m, n, T(0));
    }

    // Return a new array setting values to one.
    template <class T>
    matrix<T> ones(size_t m, size_t n) {
        return matrix<T>(m, n, T(1));
    }

    // Return a new array of given length filled with value.
    template <class T>
    matrix<T> full(size_t m, size_t n, const T &val) {
        return matrix<T>(m, n, val);
    }

    // Construct a diagonal matrix.
    template <class T>
    matrix<T> diagonal(const array<T> &v, int offset) {
        size_t n, start, stop;
        if (offset >= 0) {
            n = v.size() + offset;
            start = 0;
            stop = n - offset;
        }
        else {
            n = v.size() - offset;
            start = -offset;
            stop = n;
        }
        matrix<T> out(n, n, T(0));
        for (size_t i = start; i < stop; ++i) {
            out[i][i + offset] = v[i - start];
        }
        return out;
    }

    // Returns a matrix with ones on the diagonal and zeros elsewhere.
    template <class T>
    matrix<T> eye(size_t m, size_t n, int offset) {
        matrix<T> out(m, n, T(0));
        size_t i = (offset >= 0) ? 0 : -offset;
        for (; i < m && i + offset < n; ++i) {
            out[i][i + offset] = T(1);
        }
        return out;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Logic functions                                                        //
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

    // Returns true if two arrays are element-wise equal within a tolerance.
    template <class T>
    bool allclose(
        const array<T> &v, const array<T> &w, const T &atol, const T &rtol
    ) {
        if (v.size() != w.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << v.size() << ",) (" << w.size() << ",)";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < v.size(); ++i) {
            if (abs(v[i] - w[i]) > atol + rtol*abs(w[i])) {
                return false;
            }
        }
        return true;
    }

    // Returns true if two matrices are element-wise equal within a tolerance.
    template <class T>
    bool allclose(
        const matrix<T> &A, const matrix<T> &B, const T &atol, const T &rtol
    ) {
        if (A.rows() != B.rows() || A.columns() != B.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                if (abs(A[i][j] - B[i][j]) > atol + rtol*abs(B[i][j])) {
                    return false;
                }
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

    ////////////////////////////////////////////////////////////////////////////
    // Manipulation routines                                                  //
    ////////////////////////////////////////////////////////////////////////////

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding element in v.
    template <class T, class Function>
    array<T> apply(Function f, const array<T> &v) {
        array<T> out(v);
        out.apply(f);
        return out;
    }

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding elements in v and w.
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
        array<T> out(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            out[i] = f(v[i], val);
        }
        return out;
    }

    template <class T, class Function>
    array<T> apply(Function f, const T &val, const array<T> &v) {
        array<T> out(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            out[i] = f(val, v[i]);
        }
        return out;
    }

    // Returns a matrix with each of its elements initialized to the result of
    // applying f to the corresponding element in A.
    template <class T, class Function>
    matrix<T> apply(Function f, const matrix<T> &A) {
        matrix<T> out(A);
        out.apply(f);
        return out;
    }

    // Returns a matrix with each of its elements initialized to the result of
    // applying f to the corresponding elements in A and B.
    template <class T, class Function>
    matrix<T> apply(Function f, const matrix<T> &A, const matrix<T> &B) {
        if (A.rows() != B.rows() || A.columns() != B.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<T> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = f(A[i][j], B[i][j]);
            }
        }
        return out;
    }

    template <class T, class Function>
    matrix<T> apply(Function f, const matrix<T> &A, const T &val) {
        matrix<T> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = f(A[i][j], val);
            }
        }
        return out;
    }

    template <class T, class Function = T(T, T)>
    matrix<T> apply(Function f, const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = f(val, A[i][j]);
            }
        }
        return out;
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

    // Return a matrix whose values are limited to [a_min, a_max].
    // Given an interval, values outside the interval are clipped to the
    // interval edges.
    template <class T>
    matrix<T> clip(const matrix<T> &A, const T &a_min, const T &a_max) {
        matrix<T> out(A);
        out.clip(a_min, a_max);
        return out;
    }

    // Swap contents of two arrays.
    template <class T>
    void swap(array<T> &v, array<T> &w) {
        v.swap(w);
    }

    // Swap contents of two matrices.
    template <class T>
    void swap(matrix<T> &A, matrix<T> &B) {
        return A.swap(B);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Sorting and searching                                                  //
    ////////////////////////////////////////////////////////////////////////////

    // Return the index of the maximum value in the array.
    template <class T>
    size_t argmax(const array<T> &v) {
        return v.argmax();
    }

    // Return the index of the maximum value in the matrix.
    template <class T>
    std::pair<size_t, size_t> argmax(const matrix<T> &A) {
        return A.argmax();
    }

    template <class T>
    array<size_t> argmax(const matrix<T> &A, size_t axis) {
        return A.argmax(axis);
    }

    // Return the index of the minimum value in the array.
    template <class T>
    size_t argmin(const array<T> &v) {
        return v.argmin();
    }

    // Return the index of the minimum value in the matrix.
    template <class T>
    std::pair<size_t, size_t> argmin(const matrix<T> &A) {
        return A.argmin();
    }

    template <class T>
    array<size_t> argmin(const matrix<T> &A, size_t axis) {
        return A.argmin(axis);
    }

    // Returns the indices that would sort the array.
    template <class T>
    array<size_t> argsort(const array<T> &v) {
        return v.argsort();
    }

    // Return a sorted copy of an array.
    template <class T>
    array<T> sort(const array<T> &v) {
        array<T> sorted(v);
        sorted.sort();
        return sorted;
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

    // Return elements chosen from two arrays depending on condition.
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

    ////////////////////////////////////////////////////////////////////////////
    // Insertion-Deletion                                                     //
    ////////////////////////////////////////////////////////////////////////////

    // Concatenate arrays and matrices vertically.
    template <class T>
    matrix<T> column_stack(const array<T> &v, const array<T> &w) {
        matrix<T> A(v.size(), 1, v.data());
        matrix<T> B(w.size(), 1, w.data());
        return column_stack(A, B);
    }

    template <class T>
    matrix<T> column_stack(const matrix<T> &A, const array<T> &v) {
        matrix<T> B(v.size(), 1, v.data());
        return column_stack(A, B);
    }

    template <class T>
    matrix<T> column_stack(const array<T> &v, const matrix<T> &A) {
        matrix<T> B(v.size(), 1, v.data());
        return column_stack(B, A);
    }

    template <class T>
    matrix<T> column_stack(const matrix<T> &A, const matrix<T> &B) {
        if (A.rows() != B.rows()) {
            std::ostringstream error;
            error << "column_stack: Number of rows does not match: ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<T> out(A.rows(), A.columns() + B.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = A[i][j];
            }
        }
        for (size_t i = 0; i < B.rows(); ++i) {
            for (size_t j = 0; j < B.columns(); ++j) {
                out[i][j + A.columns()] = B[i][j];
            }
        }
        return out;
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

    // Concatenate arrays and matrices horizontally.
    template <class T>
    matrix<T> row_stack(const array<T> &v, const array<T> &w) {
        matrix<T> A(1, v.size(), v.data());
        matrix<T> B(1, w.size(), w.data());
        return row_stack(A, B);
    }

    template <class T>
    matrix<T> row_stack(const matrix<T> &A, const array<T> &v) {
        matrix<T> B(1, v.size(), v.data());
        return row_stack(A, B);
    }

    template <class T>
    matrix<T> row_stack(const array<T> &v, const matrix<T> &A) {
        matrix<T> B(1, v.size(), v.data());
        return row_stack(B, A);
    }

    template <class T>
    matrix<T> row_stack(const matrix<T> &A, const matrix<T> &B) {
        if (A.columns() != B.columns()) {
            std::ostringstream error;
            error << "row_stack: Number of columns does not match: ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<T> out(A.rows() + B.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = A[i][j];
            }
        }
        for (size_t i = 0; i < B.rows(); ++i) {
            for (size_t j = 0; j < B.columns(); ++j) {
                out[i + A.rows()][j] = B[i][j];
            }
        }
        return out;
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
            error << "insert: indices and values size does not match ("
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

    ////////////////////////////////////////////////////////////////////////////
    // Basic math functions                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Return the cumulative product of the array elements.
    template <class T>
    array<T> cumprod(const array<T> &v) {
        return v.cumprod();
    }

    // Return the cumulative sum of the array elements.
    template <class T>
    array<T> cumsum(const array<T> &v) {
        return v.cumsum();
    }

    // Return the product of the array elements.
    template <class T>
    T prod(const array<T> &v) {
        return v.prod();
    }

    // Return the product of the matrix elements.
    template <class T>
    T prod(const matrix<T> &A) {
        return A.prod();
    }

    template <class T>
    array<T> prod(const matrix<T> &A, size_t axis) {
        return A.prod(axis);
    }

    // Return the sum of the array elements.
    template <class T>
    T sum(const array<T> &v) {
        return v.sum();
    }

    // Return the sum of the matrix elements.
    template <class T>
    T sum(const matrix<T> &A) {
        return A.sum();
    }

    template <class T>
    array<T> sum(const matrix<T> &A, size_t axis) {
        return A.sum(axis);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Basic linear algebra                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Return the dot product of two arrays.
    template <class T>
    T dot(const array<T> &v, const array<T> &w) {
        return v.dot(w);
    }

    // Return the matrix multiplication of a row vector and a matrix.
    template <class T>
    array<T> dot(const array<T> &v, const matrix<T> &A) {
        return v.dot(A);
    }

    // Return the matrix multiplication of a matrix and a column vector.
    template <class T>
    array<T> dot(const matrix<T> &A, const array<T> &v) {
        return A.dot(v);
    }

    // Return the matrix multiplication of two matrices.
    template <class T>
    matrix<T> dot(const matrix<T> &A, const matrix<T> &B) {
        return A.dot(B);
    }

    // Returns the sum along the diagonal in the matrix.
    template <class T>
    T trace(const matrix<T> &A, int offset) {
        return A.trace(offset);
    }

    // Returns a copy of the matrix transposed.
    template <class T>
    matrix<T> transpose(const matrix<T> &A) {
        return A.transpose();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Basic statistics                                                       //
    ////////////////////////////////////////////////////////////////////////////

    // Returns the Pearson's correlation coefficient of x and y.
    template <class T>
    T corrcoef(const array<T> &x, const array<T> &y) {
        size_t ddof = x.size() - 1;
        return cov(x, y, ddof) / (x.stddev(ddof) * y.stddev(ddof));
    }

    // Returns the correlation matrix.
    template <class T>
    matrix<T> corrcoef(const matrix<T> &X, bool rowvar) {
        size_t ddof = rowvar ? X.columns() - 1 : X.rows() - 1;
        matrix<T> cov_matrix = cov(X, rowvar, ddof);
        for (size_t i = 0; i < cov_matrix.rows(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                cov_matrix[i][j] /= sqrt(cov_matrix[i][i] * cov_matrix[j][j]);
                cov_matrix[j][i] = cov_matrix[i][j];
            }
        }
        for (size_t i = 0; i < cov_matrix.rows(); ++i) {
            cov_matrix[i][i] = T(1);
        }
        return cov_matrix;
    }

    // Returns the covariance of x and y.
    template <class T>
    T cov(const array<T> &x, const array<T> &y, size_t ddof) {
        if (x.size() != y.size()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << x.size() << ",) (" << y.size() << ",)";
            throw std::runtime_error(error.str());
        }
        T x_mean = x.mean();
        T y_mean = y.mean();
        T out = T(0);
        for (size_t i = 0; i < x.size(); ++i) {
            out += (x[i] - x_mean) * (y[i] - y_mean);
        }
        out /= (x.size() - ddof);
        return out;
    }

    // Returns the covariance matrix.
    template <class T>
    matrix<T> cov(const matrix<T> &X, bool rowvar, size_t ddof) {
        if (rowvar) {
            matrix<T> cov_matrix(X.rows(), X.rows(), T(0));
            array<T> array_mean = X.mean(1);
            for (size_t i = 0; i < X.rows(); ++i) {
                for (size_t j = 0; j < X.rows(); ++j) {
                    for (size_t k = 0; k < X.columns(); ++k) {
                        cov_matrix[i][j] += (X[i][k] - array_mean[i]) *
                                            (X[j][k] - array_mean[j]);
                    }
                }
            }
            cov_matrix /= (X.columns() - ddof);
            return cov_matrix;
        }
        else {
            matrix<T> cov_matrix(X.columns(), X.columns(), T(0));
            array<T> array_mean = X.mean(0);
            for (size_t i = 0; i < X.columns(); ++i) {
                for (size_t j = 0; j < X.columns(); ++j) {
                    for (size_t k = 0; k < X.rows(); ++k) {
                        cov_matrix[i][j] += (X[k][i] - array_mean[i]) *
                                            (X[k][j] - array_mean[j]);
                    }
                }
            }
            cov_matrix /= (X.rows() - ddof);
            return cov_matrix;
        }
    }

    // Returns the maximum value contained in the array.
    template <class T>
    T max(const array<T> &v) {
        return v.max();
    }

    // Returns the maximum value contained in the matrix.
    template <class T>
    T max(const matrix<T> &A) {
        return A.max();
    }

    template <class T>
    array<T> max(const matrix<T> &A, size_t axis) {
        return A.max(axis);
    }

    // Returns the average of the array elements.
    template <class T>
    T mean(const array<T> &v) {
        return v.mean();
    }

    // Returns the average of the matrix elements.
    template <class T>
    T mean(const matrix<T> &A) {
        return A.mean();
    }

    template <class T>
    array<T> mean(const matrix<T> &A, size_t axis) {
        return A.mean(axis);
    }

    // Returns the minimum value contained in the array.
    template <class T>
    T min(const array<T> &v) {
        return v.min();
    }

    // Returns the minimum value contained in the matrix.
    template <class T>
    T min(const matrix<T> &A) {
        return A.min();
    }

    template <class T>
    array<T> min(const matrix<T> &A, size_t axis) {
        return A.min(axis);
    }

    // Returns the standard deviation of the array elements.
    template <class T>
    T stddev(const array<T> &v, size_t ddof) {
        return v.stddev(ddof);
    }

    // Returns the standard deviation of the matrix elements.
    template <class T>
    T stddev(const matrix<T> &A, size_t ddof) {
        return A.stddev(ddof);
    }

    template <class T>
    array<T> stddev(const matrix<T> &A, size_t ddof, size_t axis) {
        return A.stddev(ddof, axis);
    }

    // Returns the variance of the array elements.
    template <class T>
    T var(const array<T> &v, size_t ddof) {
        return v.var(ddof);
    }

    // Returns the variance of the matrix elements.
    template <class T>
    T var(const matrix<T> &A, size_t ddof) {
        return A.var(ddof);
    }

    template <class T>
    array<T> var(const matrix<T> &A, size_t ddof, size_t axis) {
        return A.var(ddof, axis);
    }
}
