#ifndef LINALG_H_INCLUDED
#define LINALG_H_INCLUDED

#include "numcpp.h"
#include <stdexcept>

namespace scicpp {

    ////////////////////////////////////////////////////////////////////////////
    // Exceptions                                                             //
    ////////////////////////////////////////////////////////////////////////////

    class LinAlgError : public std::exception {
    public:
        std::string what_arg;

        LinAlgError(const std::string &what_arg);

        const char* what() const throw();
    };

    ////////////////////////////////////////////////////////////////////////////
    // Basic functions                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Compute the norm of a vector.
    template <class T>
    T norm(const numcpp::array<T> &v, double p = 2);

    ////////////////////////////////////////////////////////////////////////////
    // LU decomposition                                                       //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the pivoted LU decomposition of a matrix.
    template <class T>
    void lu_factor(
        const numcpp::matrix<T> &A,
        numcpp::array<size_t> &piv,
        numcpp::matrix<T> &LU
    );

    template <class T>
    void lu(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &P,
        numcpp::matrix<T> &L,
        numcpp::matrix<T> &U
    );

    ////////////////////////////////////////////////////////////////////////////
    // LDL decomposition                                                      //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the LDL^T decomposition of a symmetric matrix.
    template <class T>
    void ldl(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &L,
        numcpp::array<T> &D
    );

    ////////////////////////////////////////////////////////////////////////////
    // Cholesky decomposition                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the Cholesky decomposition of a symmetric positive-definite
    // matrix.
    template <class T>
    void cholesky(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &L
    );

    ////////////////////////////////////////////////////////////////////////////
    // Linear equations solvers                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Solves the equation Ax = b for x assuming A is a triangular matrix.
    template <class T>
    numcpp::array<T> solve_triangular(
        const numcpp::matrix<T> &A,
        const numcpp::array<T> &b,
        bool lower,
        bool transpose = false,
        bool unit_diagonal = false
    );

    template <class T>
    numcpp::matrix<T> solve_triangular(
        const numcpp::matrix<T> &A,
        const numcpp::matrix<T> &b,
        bool lower,
        bool transpose = false,
        bool unit_diagonal = false
    );

    // Solves the equation Ax = b for x given the LU factorization of A.
    template <class T>
    numcpp::array<T> lu_solve(
        const numcpp::array<size_t> &piv,
        const numcpp::matrix<T> &LU,
        const numcpp::array<T> &b
    );

    template <class T>
    numcpp::matrix<T> lu_solve(
        const numcpp::array<size_t> &piv,
        const numcpp::matrix<T> &LU,
        const numcpp::matrix<T> &b
    );

    // Solves the equation Ax = b for x given the LDL factorization of A.
    template <class T>
    numcpp::array<T> ldl_solve(
        const numcpp::matrix<T> &L,
        const numcpp::array<T> &D,
        const numcpp::array<T> &b
    );

    template <class T>
    numcpp::matrix<T> ldl_solve(
        const numcpp::matrix<T> &L,
        const numcpp::array<T> &D,
        const numcpp::matrix<T> &b
    );

    // Solves the equation Ax = b for x given the Cholesky factorization of A.
    template <class T>
    numcpp::array<T> cholesky_solve(
        const numcpp::matrix<T> &L,
        const numcpp::array<T> &b
    );

    template <class T>
    numcpp::matrix<T> cholesky_solve(
        const numcpp::matrix<T> &L,
        const numcpp::matrix<T> &b
    );

    // Solves the equation Ax = b for x.
    template <class T>
    numcpp::array<T> solve(
        const numcpp::matrix<T> &A,
        const numcpp::array<T> &b,
        const std::string &assume_a = "gen"
    );

    template <class T>
    numcpp::matrix<T> solve(
        const numcpp::matrix<T> &A,
        const numcpp::matrix<T> &b,
        const std::string &assume_a = "gen"
    );

    // Compute the inverse of a matrix.
    template <class T>
    numcpp::matrix<T> inv(const numcpp::matrix<T> &A);

    // Compute the determinant of a matrix.
    template <class T>
    T det(const numcpp::matrix<T> &A);

    ////////////////////////////////////////////////////////////////////////////
    // QR decomposition                                                       //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the "raw" QR decomposition of a matrix.
    template <class T>
    void qr_raw(
        const numcpp::matrix<T> &A,
        numcpp::array<T> &tau,
        numcpp::matrix<T> &R
    );

    // Computes the QR decomposition of a matrix.
    template <class T>
    void qr(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &Q,
        numcpp::matrix<T> &R,
        bool full_matrices = true
    );

    ////////////////////////////////////////////////////////////////////////////
    // Hessenberg decomposition                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the Hessenberg form of a matrix.
    template <class T>
    void hessenberg_raw(
        const numcpp::matrix<T> &A,
        numcpp::array<T> &tau,
        numcpp::matrix<T> &H
    );

    template <class T>
    void hessenberg(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &Q,
        numcpp::matrix<T> &H
    );

    ////////////////////////////////////////////////////////////////////////////
    // Eigenvalues and eigenvectors                                           //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the eigenvalues and eigenvectors of a symmetric matrix.
    template <class T>
    void eigen_symm(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &V,
        numcpp::array<T> &D
    );

    // Computes the eigenvalues (only) of a symmetric matrix.
    template <class T>
    numcpp::array<T> eigenvals_symm(const numcpp::matrix<T> &A);

    ////////////////////////////////////////////////////////////////////////////
    // Singular value decomposition                                           //
    ////////////////////////////////////////////////////////////////////////////

    // Constructs the sigma matrix in SVD from singular values.
    template <class T>
    numcpp::matrix<T> diagsvd(const numcpp::array<T> &S, size_t m, size_t n);

    // Computes the singular value decomposition of a matrix.
    template <class T>
    void svd(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &U,
        numcpp::array<T> &S,
        numcpp::matrix<T> &V,
        bool full_matrices = true
    );

    // Compute the singular values of a matrix.
    template <class T>
    numcpp::array<T> svdvals(const numcpp::matrix<T> &A);

    ////////////////////////////////////////////////////////////////////////////
    // Linear least squares                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Compute the least squares solution to the equation Ax = b.
    template <class T>
    numcpp::array<T> lstsq(
        const numcpp::matrix<T> &A,
        const numcpp::array<T> &b,
        const std::string &method = "svd",
        const T cond = 1e-8
    );

    template <class T>
    numcpp::matrix<T> lstsq(
        const numcpp::matrix<T> &A,
        const numcpp::matrix<T> &b,
        const std::string &method = "svd",
        const T cond = 1e-8
    );

    // Compute the Moore-Penrose pseudo-inverse of a matrix.
    template <class T>
    numcpp::matrix<T> pinv(const numcpp::matrix<T> &A, const T &cond = 1e-8);

    // Constructs an orthonormal basis for the range of A using SVD.
    template <class T>
    numcpp::matrix<T> orth(
        const numcpp::matrix<T> &A,
        const T &cond = 1e-8
    );

    // Constructs an orthonormal basis for the null space of A using SVD.
    template <class T>
    numcpp::matrix<T> null_space(
        const numcpp::matrix<T> &A,
        const T &cond = 1e-8
    );
}

#include "linalg.cpp"

#endif // LINALG_H_INCLUDED
