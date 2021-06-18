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
                buffer << std::setprecision(printoptions::precision) << A[i][j];
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

    // Returns a reference to the element at row i and column j in the
    // the matrix.
    template <class T>
    T& matrix<T>::at(size_t i, size_t j) {
        return this->values[i * this->ncols + j];
    }

    template <class T>
    const T& matrix<T>::at(size_t i, size_t j) const {
        return this->values[i * this->ncols + j];
    }

    ////////////////////////////////////////////////////////////////////////////
    // Sub-matrix indexing                                                    //
    ////////////////////////////////////////////////////////////////////////////

    // Index-slice indexing: Return a sub-matrix object that selects the
    // elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(size_t i, slice j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = full(1, i);
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(size_t i, slice j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = full(1, i);
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    // Index-integer array indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(size_t i, const array<size_t> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = full(1, i);
        view.col_indices = j;
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(size_t i, const array<size_t> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = full(1, i);
        view.col_indices = j;
        return view;
    }

    // Index-boolean array indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(size_t i, const array<bool> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = full(1, i);
        view.col_indices = where(j);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(size_t i, const array<bool> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = full(1, i);
        view.col_indices = where(j);
        return view;
    }

    // Slice-index indexing: Return a sub-matrix object that selects the
    // elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(slice i, size_t j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = full(1, j);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(slice i, size_t j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = full(1, j);
        return view;
    }

    // Integer array-index indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<size_t> &i, size_t j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = full(1, j);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(const array<size_t> &i, size_t j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = full(1, j);
        return view;
    }

    // Boolean array-index indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<bool> &i, size_t j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = full(1, j);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(const array<bool> &i, size_t j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = full(1, j);
        return view;
    }

    // Slice-slice indexing: Return a sub-matrix object that selects the
    // elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(slice i, slice j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(slice i, slice j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    // Slice-integer array indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(slice i, const array<size_t> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = j;
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(slice i, const array<size_t> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = j;
        return view;
    }

    // Slice-boolean array indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(slice i, const array<bool> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = where(j);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(slice i, const array<bool> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = arange(i.start, i.stop, i.step);
        view.col_indices = where(j);
        return view;
    }

    // Integer array-slice indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<size_t> &i, slice j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(const array<size_t> &i, slice j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    // Integer array-integer array indexing: Return a sub-matrix object that
    // selects the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<size_t> &i, const array<size_t> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = j;
        return view;
    }

    template <class T>
    const submatrix<T>
    matrix<T>::at(const array<size_t> &i, const array<size_t> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = j;
        return view;
    }

    // Integer array-boolean array indexing: Return a sub-matrix object that
    // selects the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<size_t> &i, const array<bool> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = where(j);
        return view;
    }

    template <class T>
    const submatrix<T>
    matrix<T>::at(const array<size_t> &i, const array<bool> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = i;
        view.col_indices = where(j);
        return view;
    }

    // Boolean array-slice indexing: Return a sub-matrix object that selects
    // the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<bool> &i, slice j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    template <class T>
    const submatrix<T> matrix<T>::at(const array<bool> &i, slice j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = arange(j.start, j.stop, j.step);
        return view;
    }

    // Boolean array-integer array indexing: Return a sub-matrix object that
    // selects the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<bool> &i, const array<size_t> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = j;
        return view;
    }

    template <class T>
    const submatrix<T>
    matrix<T>::at(const array<bool> &i, const array<size_t> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = j;
        return view;
    }

    // Boolean array-boolean array indexing: Return a sub-matrix object that
    // selects the elements specified by its arguments.
    template <class T>
    submatrix<T> matrix<T>::at(const array<bool> &i, const array<bool> &j) {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = where(j);
        return view;
    }

    template <class T>
    const submatrix<T>
    matrix<T>::at(const array<bool> &i, const array<bool> &j) const {
        submatrix<T> view;
        view.parent = this;
        view.row_indices = where(i);
        view.col_indices = where(j);
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
        return (*this->parent)[this->row_indices[i]][this->col_indices[j]];
    }

    template <class T>
    const T& submatrix<T>::at(size_t i, size_t j) const {
        return (*this->parent)[this->row_indices[i]][this->col_indices[j]];
    }

    // Returns the number of columns selected by the sub-matrix.
    template <class T>
    size_t submatrix<T>::columns() const {
        return this->col_indices.size();
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
        return this->row_indices.size();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Methods                                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Apply a function to each of the elements in *this.
    template <class T>
    template <class Function>
    void matrix<T>::apply(Function f) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] = f(this->values[i]);
        }
    }

    // Return the index of the maximum value.
    template <class T>
    std::pair<size_t, size_t> matrix<T>::argmax() const {
        size_t index = 0;
        for (size_t i = 1; i < this->nrows * this->ncols; ++i) {
            if (this->values[index] < this->values[i]) {
                index = i;
            }
        }
        return std::make_pair(index / this->ncols, index % this->ncols);
    }

    // Return the index of the maximum value along a given axis.
    template <class T>
    array<size_t> matrix<T>::argmax(size_t axis) const {
        if (axis == 0) {
            array<size_t> out(this->ncols, 0);
            for (size_t i = 1; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    if (this->at(out[j], j) < this->at(i, j)) {
                        out[j] = i;
                    }
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<size_t> out(this->nrows, 0);
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 1; j < this->ncols; ++j) {
                    if (this->at(i, out[i]) < this->at(i, j)) {
                        out[i] = j;
                    }
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Return the index of the minimum value.
    template <class T>
    std::pair<size_t, size_t> matrix<T>::argmin() const {
        size_t index = 0;
        for (size_t i = 1; i < this->nrows * this->ncols; ++i) {
            if (this->values[i] < this->values[index]) {
                index = i;
            }
        }
        return std::make_pair(index / this->ncols, index % this->ncols);
    }

    // Return the index of the minimum value along a given axis.
    template <class T>
    array<size_t> matrix<T>::argmin(size_t axis) const {
        if (axis == 0) {
            array<size_t> out(this->ncols, 0);
            for (size_t i = 1; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    if (this->at(i, j) < this->at(out[j], j)) {
                        out[j] = i;
                    }
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<size_t> out(this->nrows, 0);
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 1; j < this->ncols; ++j) {
                    if (this->at(i, j) < this->at(i, out[i])) {
                        out[i] = j;
                    }
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Copy of the matrix, cast to a specified type.
    template <class T>
    template <class U>
    matrix<U> matrix<T>::astype() const {
        matrix<U> out(this->nrows, this->ncols);
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < this->ncols; ++j) {
                out.at(i, j) = U(this->at(i, j));
            }
        }
        return out;
    }

    // Clip (limit) the values in the matrix. Given an interval, values outside
    // the interval are clipped to the interval edges.
    template <class T>
    void matrix<T>::clip(const T &a_min, const T &a_max) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            if (this->values[i] < a_min) {
                this->values[i] = a_min;
            }
            else if (a_max < this->values[i]) {
                this->values[i] = a_max;
            }
        }
    }

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

    // Returns the matrix-vector multiplication between *this and v.
    template <class T>
    array<T> matrix<T>::dot(const array<T> &v) const {
        if (this->ncols != v.size()) {
            std::ostringstream error;
            error << "matmul: Number of columns in left operand does not match "
                  << "number of rows in right operand: ("
                  << this->nrows << "," << this->ncols << ") ("
                  << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        array<T> out(this->nrows, T(0));
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < this->ncols; ++j) {
                out[i] += this->at(i, j) * v[j];
            }
        }
        return out;
    }

    // Returns the matrix multiplication between *this and A.
    template <class T>
    matrix<T> matrix<T>::dot(const matrix &A) const {
        if (this->ncols != A.rows()) {
            std::ostringstream error;
            error << "matmul: Number of columns in left operand does not match "
                  << "number of rows in right operand: ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.rows() << "," << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<T> out(this->nrows, A.columns(), T(0));
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                for (size_t k = 0; k < this->ncols; ++k) {
                    out.at(i, j) += this->at(i, k) * A.at(k, j);
                }
            }
        }
        return out;
    }

    // Returns a copy of the matrix collapsed into an array.
    template <class T>
    array<T> matrix<T>::flatten() const {
        size_t length = this->nrows * this->ncols;
        return array<T>(this->data(), this->data() + length);
    }

    // Returns the maximum value contained in the matrix.
    template <class T>
    T matrix<T>::max() const {
        std::pair<size_t, size_t> index = this->argmax();
        return this->at(index.first, index.second);
    }

    // Returns the maximum value contained in the matrix along a given axis.
    template <class T>
    array<T> matrix<T>::max(size_t axis) const {
        array<size_t> indices = this->argmax(axis);
        array<T> out(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            out[i] = (axis == 0) ? this->at(indices[i], i)
                                 : this->at(i, indices[i]);
        }
        return out;
    }

    // Returns the average of the matrix elements.
    template <class T>
    T matrix<T>::mean() const {
        return this->sum() / (this->nrows * this->ncols);
    }

    // Returns the average of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::mean(size_t axis) const {
        array<T> out = this->sum(axis);
        out /= (axis == 0) ? this->nrows : this->ncols;
        return out;
    }

    // Returns the minimum value contained in the matrix.
    template <class T>
    T matrix<T>::min() const {
        std::pair<size_t, size_t> index = this->argmin();
        return this->at(index.first, index.second);
    }

    // Returns the minimum value contained in the matrix along a given axis.
    template <class T>
    array<T> matrix<T>::min(size_t axis) const {
        array<size_t> indices = this->argmin(axis);
        array<T> out(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            out[i] = (axis == 0) ? this->at(indices[i], i)
                                 : this->at(i, indices[i]);
        }
        return out;
    }

    // Return the product of the matrix elements.
    template <class T>
    T matrix<T>::prod() const {
        T out = T(1);
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            out *= this->values[i];
        }
        return out;
    }

    // Return the product of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::prod(size_t axis) const {
        if (axis == 0) {
            array<T> out(this->ncols, T(1));
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    out[j] *= this->at(i, j);
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<T> out(this->nrows, T(1));
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    out[i] *= this->at(i, j);
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Resizes the matrix, changing its size to m rows and n columns.
    // If the new size is smaller than the current size, the content is
    // reduced, removing the last elements.
    // If the new size is greater than the current size, the content is
    // expanded by inserting at the end as many elements as needed.
    // In any case, the contents will be rearranged to fit the new size.
    template <class T>
    void matrix<T>::resize(size_t m, size_t n, const T &val) {
        if (this->nrows * this->ncols != m * n) {
            matrix<T> tmp_copy(*this);
            delete[] this->values;
            this->values = new T[m * n];
            for (size_t i = 0; i < this->nrows*this->ncols && i < m*n; ++i) {
                this->values[i] = tmp_copy.values[i];
            }
            for (size_t i = this->nrows*this->ncols; i < m*n; ++i) {
                this->values[i] = val;
            }
        }
        this->nrows = m;
        this->ncols = n;
    }

    // Returns the number of rows in the matrix.
    template <class T>
    size_t matrix<T>::rows() const {
        return this->nrows;
    }

    // Returns the standard deviation of the matrix elements.
    template <class T>
    T matrix<T>::stddev(size_t ddof) const {
        return std::sqrt(this->var(ddof));
    }

    // Returns the standard deviation of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::stddev(size_t ddof, size_t axis) const {
        array<T> out = this->var(ddof, axis);
        out.apply(std::sqrt);
        return out;
    }

    // Return the sum of the matrix elements.
    template <class T>
    T matrix<T>::sum() const {
        T out = T(0);
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            out += this->values[i];
        }
        return out;
    }

    // Return the sum of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::sum(size_t axis) const {
        if (axis == 0) {
            array<T> out(this->ncols, T(0));
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    out[j] += this->at(i, j);
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<T> out(this->nrows, T(0));
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    out[i] += this->at(i, j);
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Swap contents with A.
    template <class T>
    void matrix<T>::swap(matrix &A) {
        size_t tmp_nrows = this->nrows;
        size_t tmp_ncols = this->ncols;
        T *tmp_values = this->values;
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        this->values = A.values;
        A.nrows = tmp_nrows;
        A.ncols = tmp_ncols;
        A.values = tmp_values;
    }

    // Returns the sum along the diagonal in the matrix.
    template <class T>
    T matrix<T>::trace(int offset) const {
        T out = T(0);
        size_t i = (offset >= 0) ? 0 : -offset;
        for (; i < this->nrows && i + offset < this->ncols; ++i) {
            out += this->at(i, i + offset);
        }
        return out;
    }

    // Returns a copy of the matrix transposed.
    template <class T>
    matrix<T> matrix<T>::transpose() const {
        matrix<T> out(this->ncols, this->nrows);
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < this->ncols; ++j) {
                out.at(j, i) = this->at(i, j);
            }
        }
        return out;
    }

    // Returns the variance of the matrix elements.
    template <class T>
    T matrix<T>::var(size_t ddof) const {
        T matrix_mean = this->mean();
        T out = T(0);
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            T deviation = this->values[i] - matrix_mean;
            out += deviation*deviation;
        }
        out /= (this->nrows * this->ncols - ddof);
        return out;
    }

    template <class T>
    array<T> matrix<T>::var(size_t ddof, size_t axis) const {
        if (axis == 0) {
            array<T> matrix_mean = this->mean(axis);
            array<T> out(this->ncols, T(0));
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    T deviation = this->at(i, j) - matrix_mean[j];
                    out[j] += deviation * deviation;
                }
            }
            out /= (this->nrows - ddof);
            return out;
        }
        else if (axis == 1) {
            array<T> matrix_mean = this->mean(axis);
            array<T> out(this->nrows, T(0));
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    T deviation = this->at(i, j) - matrix_mean[i];
                    out[i] += deviation * deviation;
                }
            }
            out /= (this->ncols - ddof);
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Global functions                                                       //
    ////////////////////////////////////////////////////////////////////////////

    // Returns a matrix with each of its elements initialized to the result of
    // applying f to the corresponding element in A.
    template <class T, class Function = T(T)>
    matrix<T> apply(Function f, const matrix<T> &A) {
        matrix<T> out(A);
        out.apply(f);
        return out;
    }

    // Returns a matrix with each of its elements initialized to the result of
    // applying f to the corresponding element in A and B.
    template <class T, class Function = T(T, T)>
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

    template <class T, class Function = T(T, T)>
    matrix<T> apply(Function f, const matrix<T> &A, const T &val) {
        matrix<T> B(A.rows(), A.columns(), val);
        return apply(f, A, B);
    }

    template <class T, class Function = T(T, T)>
    matrix<T> apply(Function f, const T &val, const matrix<T> &A) {
        matrix<T> B(A.rows(), A.columns(), val);
        return apply(f, B, A);
    }

    // Return the index of the maximum value.
    template <class T>
    std::pair<size_t, size_t> argmax(const matrix<T> &A) {
        return A.argmax();
    }

    template <class T>
    array<size_t> argmax(const matrix<T> &A, size_t axis) {
        return A.argmax(axis);
    }

    // Return an matrix whose values are limited to [a_min, a_max].
    // Given an interval, values outside the interval are clipped to the
    // interval edges.
    template <class T>
    matrix<T> clip(const matrix<T> &A, const T &a_min, const T &a_max) {
        matrix<T> out(A);
        out.clip(a_min, a_max);
        return out;
    }

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

    // Returns the correlation matrix.
    template <class T>
    T corrcoef(const array<T> &x, const array<T> &y) {
        size_t ddof = x.size() - 1;
        return cov(x, y, ddof) / (x.stddev(ddof) * y.stddev(ddof));
    }

    template <class T>
    matrix<T> corrcoef(const matrix<T> &X, bool rowvar) {
        size_t ddof = rowvar ? X.columns() - 1 : X.rows() - 1;
        matrix<T> cov_matrix = cov(X, rowvar);
        for (size_t i = 0; i < cov_matrix.rows(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                cov_matrix[i][j] /=
                    std::sqrt(cov_matrix[i][i] * cov_matrix[j][j]);
                cov_matrix[j][i] = cov_matrix[i][j];
            }
        }
        for (size_t i = 0; i < cov_matrix.rows(); ++i) {
            cov_matrix[i][i] = T(1);
        }
        return cov_matrix;
    }

    // Returns the covariance between two arrays.
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

    // Extract a diagonal or construct a diagonal matrix.
    template <class T>
    array<T> diagonal(const matrix<T> &A, int offset) {
        size_t start = (offset >= 0) ? 0 : -offset;
        size_t stop = std::min(A.rows(), A.columns() - offset);
        array<T> out(stop - start);
        for (size_t i = start; i < stop; ++i) {
            out[i - start] = A[i][i + offset];
        }
        return out;
    }

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

    // Returns the dot product of a matrix and a column vector.
    template <class T>
    array<T> dot(const matrix<T> &A, const array<T> &v) {
        return A.dot(v);
    }

    // Returns the dot product of two matrices.
    template <class T>
    matrix<T> dot(const matrix<T> &A, const matrix<T> &B) {
        return A.dot(B);
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

    // Returns the maximum value contained in the matrix.
    template <class T>
    T max(const matrix<T> &A) {
        return A.max();
    }

    template <class T>
    array<T> max(const matrix<T> &A, size_t axis) {
        return A.max(axis);
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

    // Returns the minimum value contained in the matrix.
    template <class T>
    T min(const matrix<T> &A) {
        return A.min();
    }

    template <class T>
    array<T> min(const matrix<T> &A, size_t axis) {
        return A.min(axis);
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

    // Returns the standard deviation of the matrix elements.
    template <class T>
    T stddev(const matrix<T> &A, size_t ddof) {
        return A.stddev(ddof);
    }

    template <class T>
    array<T> stddev(const matrix<T> &A, size_t ddof, size_t axis) {
        return A.stddev(ddof, axis);
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

    // Swap contents between two arrays.
    template <class T>
    void swap(matrix<T> &A, matrix<T> &B) {
        return A.swap(B);
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

#endif // MATRIX_H_INCLUDED
