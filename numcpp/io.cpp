#include <cstdio>
#include <iomanip>
#include <ios>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Binary files                                                           //
    ////////////////////////////////////////////////////////////////////////////

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
    // Text files                                                             //
    ////////////////////////////////////////////////////////////////////////////

    // Return a new matrix from a text file.
    template <class T>
    matrix<T> load_txt(const char *file, char delimiter, bool header) {
        std::ifstream loader(file);
        if (!loader.is_open()) {
            std::ostringstream error;
            error << "No such file or directory: " << file;
            throw std::runtime_error(error.str());
        }

        size_t nrows = 0, ncols = 0;
        std::string line, token;
        if (header) {
            std::getline(loader, line);
        }
        while (std::getline(loader, line)) {
            std::istringstream tokenizer(line);
            size_t line_width = 0;
            while (std::getline(tokenizer, token, delimiter)) {
                ++line_width;
            }
            ++nrows;
            if (ncols < line_width) {
                ncols = line_width;
            }
        }
        loader.clear();
        loader.seekg(0, std::ios::beg);

        matrix<T> out(nrows, ncols);
        if (header) {
            std::getline(loader, line);
        }
        for (size_t i = 0; std::getline(loader, line); ++i) {
            std::istringstream tokenizer(line);
            for (size_t j = 0; std::getline(tokenizer, token, delimiter); ++j) {
                std::istringstream in(token);
                in >> out[i][j];
            }
        }

        loader.close();
        return out;
    }

    template <class T>
    matrix<T> load_txt(
        const char *file, char delimiter, bool header, array<std::string> &names
    ) {
        std::ifstream loader(file);
        if (!loader.is_open()) {
            std::ostringstream error;
            error << "No such file or directory: " << file;
            throw std::runtime_error(error.str());
        }

        size_t nrows = 0, ncols = 0;
        std::string line, token;
        if (header) {
            std::getline(loader, line);
        }
        while (std::getline(loader, line)) {
            std::istringstream tokenizer(line);
            size_t line_width = 0;
            while (std::getline(tokenizer, token, delimiter)) {
                ++line_width;
            }
            ++nrows;
            if (ncols < line_width) {
                ncols = line_width;
            }
        }
        loader.clear();
        loader.seekg(0, std::ios::beg);

        matrix<T> out(nrows, ncols);
        names.resize(ncols, "");
        if (header) {
            std::getline(loader, line);
            std::istringstream tokenizer(line);
            for (size_t j = 0; std::getline(tokenizer, token, delimiter); ++j) {
                names[j] = token;
            }
        }
        for (size_t i = 0; std::getline(loader, line); ++i) {
            std::istringstream tokenizer(line);
            for (size_t j = 0; std::getline(tokenizer, token, delimiter); ++j) {
                std::istringstream in(token);
                in >> out[i][j];
            }
        }

        loader.close();
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
        const char *file, const matrix<T> &A, char delimiter,
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

    ////////////////////////////////////////////////////////////////////////////
    // Standard output                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Outputs an array.
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

    // Outputs a matrix.
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
}
