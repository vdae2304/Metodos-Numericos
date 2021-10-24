#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <ios>
#include <string>

// This header implements basic routines for input/output of arrays and
// matrices.

namespace numcpp {
    // Context manager for setting print options.
    namespace printoptions {
        // Number of digits of precision for floating point output.
        size_t precision = 8;

        // Total number of array elements which trigger summarization rather
        // than full representation.
        size_t threshold = 100;

        // Number of array items in summary at beginning and end of each
        // dimension.
        size_t edgeitems = 3;

        // Controls printing of the sign of numeric types. If true, always
        // print the sign of positive values. If false, omit the sign character
        // of positive values.
        bool sign = false;

        // Controls the interpretation of the precision option for
        // floating-point types. Can take the following values:
        // - "defaultmode": write floating-point values in default notation.
        // - "fixed":       write floating-point values in fixed-point notation.
        // - "scientific":  write floating-point values in scientific notation.
        enum class floatmode_t {defaultmode, fixed, scientific};
        floatmode_t floatmode = floatmode_t::defaultmode;
    };

    // Return a new array from a binary file.
    template <class T>
    array<T> load(const char *file) ;

    // Save an array to a binary file.
    template <class T>
    void save(const char *file, const array<T> &v);

    // Return a new matrix from a binary file.
    template <class T>
    matrix<T> load_matrix(const char *file);

    // Save a matrix to a binary file.
    template <class T>
    void save_matrix(const char *file, const matrix<T> &A);

    // Return a new matrix from a text file.
    template <class T>
    matrix<T> load_txt(const char *file, char delimiter, bool header);

    template <class T>
    matrix<T> load_txt(
        const char *file, char delimiter, bool header, array<std::string> &names
    );

    // Save a matrix to a text file.
    template <class T>
    void save_txt(const char *file, const matrix<T> &A, char delimiter);

    template <class T>
    void save_txt(
        const char *file, const matrix<T> &A, char delimiter,
        const array<std::string> &names
    );

    // Outputs an array.
    template <class T>
    std::ostream& operator<< (std::ostream &ostr, const array<T> &v);

    // Outputs a matrix.
    template <class T>
    std::ostream& operator<< (std::ostream &ostr, const matrix<T> &A);
}

#endif // IO_H_INCLUDED
