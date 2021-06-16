#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

namespace numcpp {

    ////////////////////////////////////////////////////////////////////////////
    // Constructors and destructor                                            //
    ////////////////////////////////////////////////////////////////////////////

    // Default constructor. Constructs an empty matrix with no elements.
    template <class T>
    matrix<T>::matrix() {
        this->nrows = 0;
        this->ncols = 0;
        this->values = nullptr;
    }

    // Size constructor. Constructs a matrix with m rows and n columns.
    template <class T>
    matrix<T>::matrix(size_t m, size_t n) {
        this->nrows = m;
        this->ncols = n;
        this->values = new T[m * n];
    }

    // Fill constructor. Constructs a matrix with m rows and n columns, each
    // element initialized to val.
    template <class T>
    matrix<T>::matrix(size_t m, size_t n, const T &val) {
        this->nrows = m;
        this->ncols = n;
        this->values = new T[m * n];
        for (size_t i = 0; i < m * n; ++i) {
            this->values[i] = val;
        }
    }

    // Container constructor. Constructs an array with m rows and n columns,
    // each element initialized to its corresponding element in the container,
    // in the same order.
    template <class T>
    template <class InputIterator>
    matrix<T>::matrix(size_t m, size_t n, InputIterator first) {
        this->nrows = m;
        this->ncols = n;
        this->values = new T[m * n];
        for (size_t i = 0; i < m * n; ++i) {
            this->values[i] = *first;
            ++first;
        }
    }

    // Copy constructor. Constructs a matrix with a copy of each of the
    // elements in A, in the same order.
    template <class T>
    matrix<T>::matrix(const matrix &A) {
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        this->values = new T[this->nrows * this->ncols];
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] = A.values[i];
        }
    }

    // Move constructor. Constructs a matrix that acquires the elements of A.
    template <class T>
    matrix<T>::matrix(matrix &&A) {
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        this->values = A.values;
        A.nrows = 0;
        A.ncols = 0;
        A.values = nullptr;
    }

    // Initializer list of lists. Constructs a matrix with a copy of each of the
    // elements in il, in the same order.
    template <class T>
    matrix<T>::matrix(std::initializer_list< std::initializer_list<T> > ill) {
        this->nrows = ill.size();
        this->ncols = 0;
        for (const std::initializer_list<T> &il : ill) {
            if (this->ncols < il.size()) {
                this->ncols = il.size();
            }
        }
        this->values = new T[this->nrows * this->ncols];
        size_t n = 0;
        for (const std::initializer_list<T> &il : ill) {
            for (const T &val : il) {
                this->values[n++] = val;
            }
            n += this->ncols - il.size();
        }
    }

    // Destructor. Destroys the matrix.
    template <class T>
    matrix<T>::~matrix() {
        delete[] this->values;
        this->nrows = 0;
        this->ncols = 0;
        this->values = nullptr;
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

    // Return a new matrix from a text file.
    template <class T>
    matrix<T> load_txt(const char *file, char delimiter, bool header) {
        std::ifstream loader(file);

        if (!loader.is_open()) {
            std::ostringstream error;
            error << "No such file or directory: " << file;
            throw std::runtime_error(error.str());
        }

        std::vector< std::vector<T> > data;
        std::string line, token;
        size_t ncols = 0;
        T value;

        if (header) {
            std::getline(loader, line);
        }
        while (std::getline(loader, line)) {
            std::istringstream tokenizer(line);
            data.push_back(std::vector<T>());
            while (std::getline(tokenizer, token, delimiter)) {
                std::istringstream in(token);
                in >> value;
                data.back().push_back(value);
            }
            if (ncols < data.back().size()) {
                ncols = data.back().size();
            }
        }
        loader.close();

        matrix<T> out(data.size(), ncols);
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                out[i][j] = data[i][j];
            }
        }

        return out;
    }

    template <class T>
    matrix<T> load_txt(
        const char *file,
        char delimiter,
        bool header,
        array<std::string> &names
    ) {
        std::ifstream loader(file);

        if (!loader.is_open()) {
            std::ostringstream error;
            error << "No such file or directory: " << file;
            throw std::runtime_error(error.str());
        }

        std::vector< std::vector<T> > data;
        std::vector<std::string> colnames;
        std::string line, token;
        size_t ncols = 0;
        T value;

        if (header) {
            std::getline(loader, line);
            std::istringstream tokenizer(line);
            while (std::getline(tokenizer, token, delimiter)) {
                colnames.push_back(token);
            }
        }
        while (std::getline(loader, line)) {
            std::istringstream tokenizer(line);
            data.push_back(std::vector<T>());
            while (std::getline(tokenizer, token, delimiter)) {
                std::istringstream in(token);
                in >> value;
                data.back().push_back(value);
            }
            if (ncols < data.back().size()) {
                ncols = data.back().size();
            }
        }
        loader.close();

        names = array<std::string>(colnames.begin(), colnames.end());
        names.resize(ncols, "no name");

        matrix<T> out(data.size(), ncols);
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                out[i][j] = data[i][j];
            }
        }

        return out;
    }

    // Save a matrix to a text file.
    template <class T>
    void save_txt(const char *file, const matrix<T> &A, char delimiter) {
        std::ofstream out(file);

        if (!out.is_open()) {
            std::ostringstream error;
            error << "Can not modify: " << file;
            throw std::runtime_error(error.str());
        }

        out << std::setprecision(printoptions::precision);
        if (printoptions::sign) {
            out << std::showpos;
        }
        else {
            out << std::noshowpos;
        }
        if (printoptions::floatmode == "default") {
            out << std::defaultfloat;
        }
        else if (printoptions::floatmode == "fixed") {
            out << std::fixed;
        }
        else if (printoptions::floatmode == "scientific") {
            out << std::scientific;
        }
        else {
            throw std::invalid_argument(
                "printoptions::floatmode must be one of \"default\", "
                "\"fixed\" or \"scientific\""
            );
        }

        for (size_t i = 0; i < A.rows(); ++i) {
            std::string sep = "";
            for (size_t j = 0; j < A.columns(); ++j) {
                out << sep << A[i][j];
                sep = delimiter;
            }
            out << "\n";
        }
        out.close();
    }

    template <class T>
    void save_txt(
        const char *file,
        const matrix<T> &A,
        char delimiter,
        const array<std::string> &names
    ) {
        std::ofstream out(file);

        if (!out.is_open()) {
            std::ostringstream error;
            error << "Can not modify: " << file;
            throw std::runtime_error(error.str());
        }

        out << std::setprecision(printoptions::precision);
        if (printoptions::sign) {
            out << std::showpos;
        }
        else {
            out << std::noshowpos;
        }
        if (printoptions::floatmode == "default") {
            out << std::defaultfloat;
        }
        else if (printoptions::floatmode == "fixed") {
            out << std::fixed;
        }
        else if (printoptions::floatmode == "scientific") {
            out << std::scientific;
        }
        else {
            throw std::invalid_argument(
                "printoptions::floatmode must be one of \"default\", "
                "\"fixed\" or \"scientific\""
            );
        }

        std::string sep = "";
        for (size_t i = 0; i < names.size(); ++i) {
            out << sep << names[i];
            sep = delimiter;
        }
        out << "\n";
        for (size_t i = 0; i < A.rows(); ++i) {
            sep = "";
            for (size_t j = 0; j < A.columns(); ++j) {
                out << sep << A[i][j];
                sep = delimiter;
            }
            out << "\n";
        }
        out.close();
    }

    // Return a new matrix from a binary file.
    template <class T>
    matrix<T> load_matrix(const char *file) {
        FILE *pfile = fopen(file, "rb");
        if (pfile != NULL) {
            size_t m, n;
            fread(&m, sizeof(size_t), 1, pfile);
            fread(&n, sizeof(size_t), 1, pfile);
            matrix<T> out(m, n);
            fread(out.data(), sizeof(T), m * n, pfile);
            fclose(pfile);
            return out;
        }
        else {
            std::ostringstream error;
            error << "No such file or directory: " << file;
            throw std::runtime_error(error.str());
        }
    }

    // Save a matrix to a binary file.
    template <class T>
    void save_matrix(const char *file, const matrix<T> &A) {
        FILE *pfile = fopen(file, "wb");
        if (pfile != NULL) {
            size_t m = A.rows(), n = A.columns();
            fwrite(&m, sizeof(size_t), 1, pfile);
            fwrite(&n, sizeof(size_t), 1, pfile);
            fwrite(A.data(), sizeof(T), m * n, pfile);
            fclose(pfile);
        }
        else {
            std::ostringstream error;
            error << "Can not modify: " << file;
            throw std::runtime_error(error.str());
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Assignment operators                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Copy assignment. Assigns the contents of A to *this after resizing the
    // object (if necessary).
    template <class T>
    matrix<T>& matrix<T>::operator= (const matrix<T> &A) {
        if (this->nrows * this->ncols != A.nrows * A.ncols) {
            delete[] this->values;
            this->values = new T[A.nrows * A.ncols];
        }
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] = A.values[i];
        }
        return *this;
    }

    // Move assignment. Acquires the contents of A.
    template <class T>
    matrix<T>& matrix<T>::operator= (matrix<T> &&A) {
        if (this != &A) {
            delete[] this->values;
            this->nrows = A.nrows;
            this->ncols = A.ncols;
            this->values = A.values;
            A.nrows = 0;
            A.ncols = 0;
            A.values = nullptr;
        }
        return *this;
    }

    // Fill assignment. Assigns val to every element.
    template <class T>
    matrix<T>& matrix<T>::operator= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] = val;
        }
    }

    // Compound assignments.
    template <class T>
    matrix<T>& matrix<T>::operator+= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] += A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator-= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] -= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator*= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] *= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator/= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] /= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator%= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] %= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator&= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] &= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator|= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] |= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator^= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] ^= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator<<= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] <<= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator>>= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] >>= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator+= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] += val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator-= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] -= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator*= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] *= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator/= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] /= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator%= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] %= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator&= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] &= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator|= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] |= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator^= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] ^= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator<<= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] <<= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator>>= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] >>= val;
        }
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Operator overloading                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Unary operators.
    template <class T>
    matrix<T> operator+ (const matrix<T> &A) {
        return A;
    }

    template <class T>
    matrix<T> operator- (const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = -A[i][j];
            }
        }
        return out;
    }

    template <class T>
    matrix<T> operator~ (const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = ~A[i][j];
            }
        }
        return out;
    }

    matrix<bool> operator! (const matrix<bool> &A) {
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = !A[i][j];
            }
        }
        return out;
    }

    // Arithmetic operators.
    template<class T>
    matrix<T> operator+ (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out += B;
    }

    template<class T>
    matrix<T> operator+ (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out += val;
    }

    template<class T>
    matrix<T> operator+ (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out += A;
    }

    template<class T>
    matrix<T> operator- (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out -= B;
    }

    template<class T>
    matrix<T> operator- (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out -= val;
    }

    template<class T>
    matrix<T> operator- (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out -= A;
    }

    template<class T>
    matrix<T> operator* (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out *= B;
    }

    template<class T>
    matrix<T> operator* (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out *= val;
    }

    template<class T>
    matrix<T> operator* (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out *= A;
    }

    template<class T>
    matrix<T> operator/ (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out /= B;
    }

    template<class T>
    matrix<T> operator/ (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out /= val;
    }

    template<class T>
    matrix<T> operator/ (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out /= A;
    }

    template<class T>
    matrix<T> operator% (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out %= B;
    }

    template<class T>
    matrix<T> operator% (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out %= val;
    }

    template<class T>
    matrix<T> operator% (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out %= A;
    }

    // Bitwise operators.
    template<class T>
    matrix<T> operator& (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out &= B;
    }

    template<class T>
    matrix<T> operator& (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out &= val;
    }

    template<class T>
    matrix<T> operator& (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out &= A;
    }

    template<class T>
    matrix<T> operator| (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out &= B;
    }

    template<class T>
    matrix<T> operator| (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out |= val;
    }

    template<class T>
    matrix<T> operator| (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out |= A;
    }

    template<class T>
    matrix<T> operator^ (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out ^= B;
    }

    template<class T>
    matrix<T> operator^ (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out ^= val;
    }

    template<class T>
    matrix<T> operator^ (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out ^= A;
    }

    template<class T>
    matrix<T> operator<< (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out <<= B;
    }

    template<class T>
    matrix<T> operator<< (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out <<= val;
    }

    template<class T>
    matrix<T> operator<< (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out <<= A;
    }

    template<class T>
    matrix<T> operator>> (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out >>= B;
    }

    template<class T>
    matrix<T> operator>> (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out >>= val;
    }

    template<class T>
    matrix<T> operator>> (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out >>= A;
    }

    // Relational operators.
    template<class T>
    matrix<bool> operator== (const matrix<T> &A, const matrix<T> &B) {
        if (A.rows() != B.rows() || A.columns() != B.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (A[i][j] == B[i][j]);
            }
        }
        return out;
    }

    template<class T>
    matrix<bool> operator== (const matrix<T> &A, const T &val) {
        matrix<T> B(A.rows(), A.columns(), val);
        return (A == B);
    }

    template<class T>
    matrix<bool> operator== (const T &val, const matrix<T> &A) {
        matrix<T> B(A.rows(), A.columns(), val);
        return (B == A);
    }

    template<class T>
    matrix<bool> operator!= (const matrix<T> &A, const matrix<T> &B) {
        return !(A == B);
    }

    template<class T>
    matrix<bool> operator!= (const matrix<T> &A, const T &val) {
        return !(A == val);
    }

    template<class T>
    matrix<bool> operator!= (const T &val, const matrix<T> &A) {
        return !(val == A);
    }

    template<class T>
    matrix<bool> operator< (const matrix<T> &A, const matrix<T> &B) {
        if (A.rows() != B.rows() || A.columns() != B.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (A[i][j] < B[i][j]);
            }
        }
        return out;
    }

    template<class T>
    matrix<bool> operator< (const matrix<T> &A, const T &val) {
        matrix<T> B(A.rows(), A.columns(), val);
        return (A < B);
    }

    template<class T>
    matrix<bool> operator< (const T &val, const matrix<T> &A) {
        matrix<T> B(A.rows(), A.columns(), val);
        return (B < A);
    }

    template<class T>
    matrix<bool> operator> (const matrix<T> &A, const matrix<T> &B) {
        return (B < A);
    }

    template<class T>
    matrix<bool> operator> (const matrix<T> &A, const T &val) {
        return (val < A);
    }

    template<class T>
    matrix<bool> operator> (const T &val, const matrix<T> &A) {
        return (A < val);
    }

    template<class T>
    matrix<bool> operator<= (const matrix<T> &A, const matrix<T> &B) {
        return !(B < A);
    }

    template<class T>
    matrix<bool> operator<= (const matrix<T> &A, const T &val) {
        return !(val < A);
    }

    template<class T>
    matrix<bool> operator<= (const T &val, const matrix<T> &A) {
        return !(A < val);
    }

    template<class T>
    matrix<bool> operator>= (const matrix<T> &A, const matrix<T> &B) {
        return !(A < B);
    }

    template<class T>
    matrix<bool> operator>= (const matrix<T> &A, const T &val) {
        return !(A < val);
    }

    template<class T>
    matrix<bool> operator>= (const T &val, const matrix<T> &A) {
        return !(val < A);
    }

    // Standard output.
    template <class T>
    std::ostream& operator<< (std::ostream &ostr, const matrix<T> &A) {
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

        array<size_t> width(A.columns(), 0);
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                std::ostringstream buffer;
                buffer.setf(ostr.flags());
                buffer << A[i][j];
                if (width[j] < buffer.str().size()) {
                    width[j] = buffer.str().size();
                }
            }
        }

        ostr << "[";
        if (
            A.rows() < printoptions::threshold ||
            A.rows() <= 2*printoptions::edgeitems
        ) {
            std::string vsep = "";
            for (size_t i = 0; i < A.rows(); ++i) {
                ostr << vsep << "[";
                if (
                    A.columns() < printoptions::threshold ||
                    A.columns() <= 2*printoptions::edgeitems
                ) {
                    std::string hsep = "";
                    for (size_t j = 0; j < A.columns(); ++j) {
                        ostr << hsep << std::setw(width[j]) << A[i][j];
                        hsep = ", ";
                    }
                }
                else {
                    std::string hsep = "";
                    for (size_t j = 0; j < printoptions::edgeitems; ++j) {
                        ostr << hsep << std::setw(width[j]) << A[i][j];
                        hsep = ", ";
                    }
                    hsep = ", ..., ";
                    for (size_t j = 0; j < printoptions::edgeitems; ++j) {
                        ostr << hsep << std::setw(width[j])
                             << A[i][A.columns() - printoptions::edgeitems + j];
                        hsep = ", ";
                    }
                }
                ostr << "]";
                vsep = "\n ";
            }
        }
        else {
            std::string vsep = "";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << vsep << "[";
                if (
                    A.columns() < printoptions::threshold ||
                    A.columns() <= 2*printoptions::edgeitems
                ) {
                    std::string hsep = "";
                    for (size_t j = 0; j < A.columns(); ++j) {
                        ostr << hsep << std::setw(width[j]) << A[i][j];
                        hsep = ", ";
                    }
                }
                else {
                    std::string hsep = "";
                    for (size_t j = 0; j < printoptions::edgeitems; ++j) {
                        ostr << hsep << std::setw(width[j]) << A[i][j];
                        hsep = ", ";
                    }
                    hsep = ", ..., ";
                    for (size_t j = 0; j < printoptions::edgeitems; ++j) {
                        ostr << hsep << std::setw(width[j])
                             << A[i][A.columns() - printoptions::edgeitems + j];
                        hsep = ", ";
                    }
                }
                ostr << "]";
                vsep = "\n ";
            }

            vsep = "\n ...\n ";
            for (size_t i = 0; i < printoptions::edgeitems; ++i) {
                ostr << vsep << "[";
                if (
                    A.columns() < printoptions::threshold ||
                    A.columns() <= 2*printoptions::edgeitems
                ) {
                    std::string hsep = "";
                    for (size_t j = 0; j < A.columns(); ++j) {
                        ostr << hsep << std::setw(width[j])
                             << A[A.rows() - printoptions::edgeitems + i][j];
                        hsep = ", ";
                    }
                }
                else {
                    std::string hsep = "";
                    for (size_t j = 0; j < printoptions::edgeitems; ++j) {
                        ostr << hsep << std::setw(width[j])
                             << A[A.rows() - printoptions::edgeitems + i][j];
                        hsep = ", ";
                    }
                    hsep = ", ..., ";
                    for (size_t j = 0; j < printoptions::edgeitems; ++j) {
                        ostr << hsep << std::setw(width[j])
                             << A[A.rows() - printoptions::edgeitems + i]
                                 [A.columns() - printoptions::edgeitems + j];
                        hsep = ", ";
                    }
                }
                ostr << "]";
                vsep = "\n ";
            }
        }
        ostr << "]";

        ostr.setf(default_flags);
        return ostr;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Matrix indexing                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Array subscript. Returns a pointer to the beginning of the i-th row.
    template <class T>
    T* matrix<T>::operator[] (size_t i) {
        return this->values + (i * this->ncols);
    }

    template <class T>
    const T* matrix<T>::operator[] (size_t i) const {
        return this->values + (i * this->ncols);
    }

    // Returns a reference to the element at row index.first and column
    // index.second in the matrix.
    template <class T>
    T& matrix<T>::operator[] (std::pair<size_t, size_t> index) {
        size_t i = index.first, j = index.second;
        return this->values[i * this->ncols + j];
    }

    template <class T>
    const T& matrix<T>::operator[] (std::pair<size_t, size_t> index) const {
        size_t i = index.first, j = index.second;
        return this->values[i * this->ncols + j];
    }

    ////////////////////////////////////////////////////////////////////////////
    // Slice indexing                                                         //
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Integer array indexing                                                 //
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods                                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Returns the number of columns in the matrix.
    template <class T>
    size_t matrix<T>::columns() const {
        return this->ncols;
    }

    // Returns a pointer to the memory array used internally by the matrix.
    template <class T>
    T* matrix<T>::data() {
        return this->values;
    }

    template <class T>
    const T* matrix<T>::data() const {
        return this->values;
    }

    // Returns the number of rows in the matrix.
    template <class T>
    size_t matrix<T>::rows() const {
        return this->nrows;
    }

    // Returns the number of rows and columns.
    template <class T>
    std::pair<size_t, size_t> matrix<T>::shape() const {
        return std::make_pair(this->nrows, this->ncols);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Global functions                                                       //
    ////////////////////////////////////////////////////////////////////////////
}

#endif // MATRIX_H_INCLUDED
