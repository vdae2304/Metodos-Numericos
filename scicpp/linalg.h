#ifndef LINALG_H_INCLUDED
#define LINALG_H_INCLUDED

#include <cmath>
#include <limits>
#include <stdexcept>

namespace scicpp {

    ////////////////////////////////////////////////////////////////////////////
    // Exceptions                                                             //
    ////////////////////////////////////////////////////////////////////////////

    class LinAlgError : public std::exception {
    public:
        std::string what_arg;

        LinAlgError(const std::string &what_arg) {
            this->what_arg = what_arg;
        }

        const char* what() const throw() {
            return this->what_arg.c_str();
        }
    };

    ////////////////////////////////////////////////////////////////////////////
    // Basic functions                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Compute the norm of a vector.
    template <class T>
    T norm(const numcpp::array<T> &v, double p = 2) {
        T out = T(0);
        if (p < 1) {
            throw std::invalid_argument("norm: p must be at least 1.");
        }
        else if (p == 1) {
            for (size_t i = 0; i < v.size(); ++i) {
                out += numcpp::abs(v[i]);
            }
        }
        else if (p == 2) {
            out = numcpp::sqrt(v.dot(v));
        }
        else if (p == numcpp::constants::inf) {
            for (size_t i = 0; i < v.size(); ++i) {
                out = numcpp::max(out, numcpp::abs(v[i]));
            }
        }
        else {
            for (size_t i = 0; i < v.size(); ++i) {
                out += numcpp::pow(numcpp::abs(v[i]), p);
            }
            out = numcpp::pow(out, 1.0/p);
        }
        return out;
    }

    ////////////////////////////////////////////////////////////////////////////
    // LU decomposition                                                       //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the pivoted LU decomposition of a matrix.
    template <class T>
    void lu_factor(
        const numcpp::matrix<T> &A,
        numcpp::array<size_t> &piv,
        numcpp::matrix<T> &LU
    ) {
        if (A.rows() != A.columns()) {
            throw std::runtime_error("lu_factor: Expected square matrix.");
        }

        T tol = std::numeric_limits<T>::epsilon();
        LU = A;
        piv = numcpp::arange(LU.rows());

        for (size_t k = 0; k < LU.rows(); ++k) {
            size_t pivot = k;
            for (size_t i = k + 1; i < LU.rows(); ++i) {
                if (numcpp::abs(LU[pivot][k]) < numcpp::abs(LU[i][k])) {
                    pivot = i;
                }
            }
            if (numcpp::abs(LU[pivot][k]) <= tol) {
                continue;
            }

            if (pivot != k) {
                for (size_t j = 0; j < LU.columns(); ++j) {
                    std::swap(LU[pivot][j], LU[k][j]);
                }
                std::swap(piv[pivot], piv[k]);
            }

            for (size_t i = k + 1; i < LU.rows(); ++i) {
                LU[i][k] /= LU[k][k];
                for (size_t j = k + 1; j < LU.columns(); ++j) {
                    LU[i][j] -= LU[i][k] * LU[k][j];
                }
            }
        }
    }

    template <class T>
    void lu(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &P,
        numcpp::matrix<T> &L,
        numcpp::matrix<T> &U
    ) {
        numcpp::array<size_t> piv;
        lu_factor(A, piv, L);
        P = numcpp::zeros<T>(A.rows(), A.columns());
        for (size_t i = 0; i < P.rows(); ++i) {
            P[i][piv[i]] = T(1);
        }
        U = L;
        for (size_t i = 0; i < L.rows(); ++i) {
            L[i][i] = T(1);
            for (size_t j = i + 1; j < L.columns(); ++j) {
                L[i][j] = T(0);
            }
        }
        for (size_t i = 0; i < U.rows(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                U[i][j] = T(0);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // LDL decomposition                                                      //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the LDL^T decomposition of a symmetric matrix.
    template <class T>
    void ldl(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &L,
        numcpp::array<T> &D
    ) {
        if (A.rows() != A.columns()) {
            throw std::invalid_argument("ldl: Expected square matrix.");
        }

        T tol = std::numeric_limits<T>::epsilon();
        L = numcpp::zeros<T>(A.rows(), A.columns());
        D = numcpp::zeros<T>(A.rows());

        for (size_t j = 0; j < A.columns(); ++j) {
            L[j][j] = T(1);
            for (size_t k = 0; k < j; ++k) {
                D[j] += L[j][k] * D[k] * L[j][k];
            }
            D[j] = A[j][j] - D[j];
            if (numcpp::abs(D[j]) <= tol) {
                continue;
            }

            for (size_t i = j + 1; i < A.rows(); ++i) {
                for (size_t k = 0; k < j; ++k) {
                    L[i][j] += L[i][k] * D[k] * L[j][k];
                }
                L[i][j] = (A[i][j] - L[i][j]) / D[j];
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Cholesky decomposition                                                 //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the Cholesky decomposition of a symmetric positive-definite
    // matrix.
    template <class T>
    void cholesky(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &L
    ) {
        if (A.rows() != A.columns()) {
            throw std::invalid_argument("cholesky: Expected square matrix.");
        }

        T tol = std::numeric_limits<T>::epsilon();
        L = numcpp::zeros<T>(A.rows(), A.columns());

        for (size_t j = 0; j < A.columns(); ++j) {
            for (size_t k = 0; k < j; ++k) {
                L[j][j] += L[j][k] * L[j][k];
            }
            L[j][j] = A[j][j] - L[j][j];

            if (L[j][j] <= tol) {
                throw LinAlgError("Non symmetric positive-definite matrix.");
            }
            L[j][j] = numcpp::sqrt(L[j][j]);

            for (size_t i = j + 1; i < A.rows(); ++i) {
                for (size_t k = 0; k < j; ++k) {
                    L[i][j] += L[i][k] * L[j][k];
                }
                L[i][j] = (A[i][j] - L[i][j]) / L[j][j];
            }
        }
    }

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
    ) {
        if (A.rows() != A.columns()) {
            throw std::runtime_error("solve: Expected square matrix.");
        }
        if (A.rows() != b.size()) {
            std::ostringstream error;
            error << "solve: Number of rows in left operand does not match "
                  << "number of rows in right operand: (" << A.rows() << ","
                  << A.columns() << ") (" << b.size() << ",)";
            throw std::runtime_error(error.str());
        }

        T tol = std::numeric_limits<T>::epsilon();
        numcpp::array<T> x = numcpp::zeros<T>(b.size());
        if (transpose) {
            lower = !lower;
        }

        if (lower) {
            for (size_t i = 0; i < b.size(); ++i) {
                for (size_t k = 0; k < i; ++k) {
                    x[i] += (transpose ? A[k][i] : A[i][k]) * x[k];
                }
                x[i] = b[i] - x[i];
                if (!unit_diagonal) {
                    if (numcpp::abs(A[i][i]) <= tol) {
                        throw LinAlgError("Singular matrix.");
                    }
                    x[i] /= A[i][i];
                }
            }
        }
        else  {
            for (size_t i = b.size() - 1; i != -1; --i) {
                for (size_t k = i + 1; k < A.columns(); ++k) {
                    x[i] += (transpose ? A[k][i] : A[i][k]) * x[k];
                }
                x[i] = b[i] - x[i];
                if (!unit_diagonal) {
                    if (numcpp::abs(A[i][i]) <= tol) {
                        throw LinAlgError("Singular matrix.");
                    }
                    x[i] /= A[i][i];
                }
            }
        }

        return x;
    }

    template <class T>
    numcpp::matrix<T> solve_triangular(
        const numcpp::matrix<T> &A,
        const numcpp::matrix<T> &b,
        bool lower,
        bool transpose = false,
        bool unit_diagonal = false
    ) {
        if (A.rows() != A.columns()) {
            throw std::runtime_error("solve: Expected square matrix.");
        }
        if (A.rows() != b.rows()) {
            std::ostringstream error;
            error << "solve: Number of rows in left operand does not match "
                  << "number of rows in right operand: (" << A.rows() << ","
                  << A.columns() << ") (" << b.rows() << "," << b.columns()
                  << ")";
            throw std::runtime_error(error.str());
        }

        T tol = std::numeric_limits<T>::epsilon();
        numcpp::matrix<T> x = numcpp::zeros<T>(b.rows(), b.columns());
        if (transpose) {
            lower = !lower;
        }

        if (lower) {
            for (size_t j = 0; j < b.columns(); ++j) {
                for (size_t i = 0; i < b.rows(); ++i) {
                    for (size_t k = 0; k < i; ++k) {
                        x[i][j] += (transpose ? A[k][i] : A[i][k]) * x[k][j];
                    }
                    x[i][j] = b[i][j] - x[i][j];
                    if (!unit_diagonal) {
                        if (numcpp::abs(A[i][i]) <= tol) {
                            throw LinAlgError("Singular matrix.");
                        }
                        x[i][j] /= A[i][i];
                    }
                }
            }
        }
        else  {
            for (size_t j = 0; j < b.columns(); ++j) {
                for (size_t i = b.rows() - 1; i != -1; --i) {
                    for (size_t k = i + 1; k < A.columns(); ++k) {
                        x[i][j] += (transpose ? A[k][i] : A[i][k]) * x[k][j];
                    }
                    x[i][j] = b[i][j] - x[i][j];
                    if (!unit_diagonal) {
                        if (numcpp::abs(A[i][i]) <= tol) {
                            throw LinAlgError("Singular matrix.");
                        }
                        x[i][j] /= A[i][i];
                    }
                }
            }
        }

        return x;
    }

    // Solves the equation Ax = b for x given the LU factorization of A.
    template <class T>
    numcpp::array<T> lu_solve(
        const numcpp::array<size_t> &piv,
        const numcpp::matrix<T> &LU,
        const numcpp::array<T> &b
    ) {
        numcpp::array<T> x(b.size());
        for (size_t i = 0; i < b.size(); ++i) {
            x[i] = b[piv[i]];
        }
        x = solve_triangular(LU, x, true, false, true);
        x = solve_triangular(LU, x, false, false, false);
        return x;
    }

    template <class T>
    numcpp::matrix<T> lu_solve(
        const numcpp::array<size_t> &piv,
        const numcpp::matrix<T> &LU,
        const numcpp::matrix<T> &b
    ) {
        numcpp::matrix<T> x(b.rows(), b.columns());
        for (size_t i = 0; i < b.rows(); ++i) {
            for (size_t j = 0; j < b.columns(); ++j) {
                x[i][j] = b[piv[i]][j];
            }
        }
        x = solve_triangular(LU, x, true, false, true);
        x = solve_triangular(LU, x, false, false, false);
        return x;
    }

    // Solves the equation Ax = b for x given the LDL factorization of A.
    template <class T>
    numcpp::array<T> ldl_solve(
        const numcpp::matrix<T> &L,
        const numcpp::array<T> &D,
        const numcpp::array<T> &b
    ) {
        numcpp::array<T> x = b;
        x = solve_triangular(L, x, true, false, true);
        x /= D;
        x = solve_triangular(L, x, true, true, true);
        return x;
    }

    template <class T>
    numcpp::matrix<T> ldl_solve(
        const numcpp::matrix<T> &L,
        const numcpp::array<T> &D,
        const numcpp::matrix<T> &b
    ) {
        numcpp::matrix<T> x = b;
        x = solve_triangular(L, x, true, false, true);
        for (size_t i = 0; i < x.rows(); ++i) {
            for (size_t j = 0; j < x.columns(); ++j) {
                x[i][j] /= D[i];
            }
        }
        x = solve_triangular(L, x, true, true, true);
        return x;
    }

    // Solves the equation Ax = b for x given the Cholesky factorization of A.
    template <class T>
    numcpp::array<T> cholesky_solve(
        const numcpp::matrix<T> &L,
        const numcpp::array<T> &b
    ) {
        numcpp::array<T> x = b;
        x = solve_triangular(L, x, true, false, false);
        x = solve_triangular(L, x, true, true, false);
        return x;
    }

    template <class T>
    numcpp::matrix<T> cholesky_solve(
        const numcpp::matrix<T> &L,
        const numcpp::matrix<T> &b
    ) {
        numcpp::matrix<T> x = b;
        x = solve_triangular(L, x, true, false, false);
        x = solve_triangular(L, x, true, true, false);
        return x;
    }

    // Solves the equation Ax = b for x.
    template <class T>
    numcpp::array<T> solve(
        const numcpp::matrix<T> &A,
        const numcpp::array<T> &b,
        const std::string &assume_a = "gen"
    ) {
        if (assume_a == "gen") {
            numcpp::array<size_t> piv;
            numcpp::matrix<T> LU;
            lu_factor(A, piv, LU);
            return lu_solve(piv, LU, b);
        }
        else if (assume_a == "sym") {
            numcpp::matrix<T> L;
            numcpp::array<T> D;
            ldl(A, L, D);
            return ldl_solve(L, D, b);
        }
        else if (assume_a == "pos") {
            numcpp::matrix<T> L;
            cholesky(A, L);
            return cholesky_solve(L, b);
        }
        else {
            throw std::invalid_argument(
                "assume_a must be one of \"gen\", \"sym\" or \"pos\"."
            );
        }
    }

    template <class T>
    numcpp::matrix<T> solve(
        const numcpp::matrix<T> &A,
        const numcpp::matrix<T> &b,
        const std::string &assume_a = "gen"
    ) {
        if (assume_a == "gen") {
            numcpp::array<size_t> piv;
            numcpp::matrix<T> LU;
            lu_factor(A, piv, LU);
            return lu_solve(piv, LU, b);
        }
        else if (assume_a == "sym") {
            numcpp::matrix<T> L;
            numcpp::array<T> D;
            ldl(A, L, D);
            return ldl_solve(L, D, b);
        }
        else if (assume_a == "pos") {
            numcpp::matrix<T> L;
            cholesky(A, L);
            return cholesky_solve(L, b);
        }
        else {
            throw std::invalid_argument(
                "assume_a must be one of \"gen\", \"sym\" or \"pos\"."
            );
        }
    }

    // Compute the inverse of a matrix.
    template <class T>
    numcpp::matrix<T> inv(const numcpp::matrix<T> &A) {
        return solve(A, numcpp::eye<T>(A.rows(), A.columns()));
    }

    // Compute the determinant of a matrix.
    template <class T>
    T det(const numcpp::matrix<T> &A) {
        numcpp::array<size_t> piv;
        numcpp::matrix<T> LU;
        lu_factor(A, piv, LU);

        T out = T(1);
        for (size_t i = 0; i < LU.rows(); ++i) {
            out *= LU[i][i];
        }
        for (size_t i = 0; i < piv.size(); ++i) {
            size_t j = i;
            while (piv[j] != j) {
                j = piv[j];
                piv[j] = j;
                out *= -1;
            }
        }

        return out;
    }

    ////////////////////////////////////////////////////////////////////////////
    // QR decomposition                                                       //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the "raw" QR decomposition of a matrix.
    template <class T>
    void qr_raw(
        const numcpp::matrix<T> &A,
        numcpp::array<T> &tau,
        numcpp::matrix<T> &R
    ) {
        if (A.rows() < A.columns()) {
            throw std::runtime_error(
                "qr: Number of columns must be less than number of rows."
            );
        }

        T tol = std::numeric_limits<T>::epsilon();
        tau.resize(A.columns());
        R = A;

        for (size_t k = 0; k < R.columns(); ++k) {
            numcpp::array<T> v(R.rows() - k);
            numcpp::matrix<T> R_copy(R.rows() - k, R.columns() - k);
            for (size_t i = k; i < R.rows(); ++i) {
                v[i - k] = R[i][k];
                for (size_t j = k; j < R.columns(); ++j) {
                    R_copy[i - k][j - k] = R[i][j];
                }
            }
            v[0] += (v[0] >= T(0)) ? numcpp::sqrt(v.dot(v))
                                   : -numcpp::sqrt(v.dot(v));

            T squared_norm = v.dot(v);
            if (squared_norm <= tol) {
                throw LinAlgError("Linearly dependent columns.");
            }

            for (size_t i = k; i < R.rows(); ++i) {
                for (size_t j = k; j < R.columns(); ++j) {
                    for (size_t l = k; l < R.rows(); ++l) {
                        R[i][j] -= (2.0*v[i - k]*v[l - k] / squared_norm) *
                                   R_copy[l - k][j - k];
                    }
                }
            }

            tau[k] = v[0];
            for (size_t i = k + 1; i < R.rows(); ++i) {
                R[i][k] = v[i - k];
            }
        }
    }

    // Computes the QR decomposition of a matrix.
    template <class T>
    void qr(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &Q,
        numcpp::matrix<T> &R,
        bool full_matrices = true
    ) {
        numcpp::array<T> tau;
        Q = numcpp::eye<T>(A.rows(), full_matrices ? A.rows() : A.columns());
        qr_raw(A, tau, R);

        for (size_t k = tau.size() - 1; k != -1; --k) {
            numcpp::array<T> v(R.rows() - k);
            v[0] = tau[k];
            for (size_t i = k + 1; i < R.rows(); ++i) {
                v[i - k] = R[i][k];
                R[i][k] = T(0);
            }

            numcpp::matrix<T> Q_copy(Q.rows() - k, Q.columns() - k);
            for (size_t i = k; i < Q.rows(); ++i) {
                for (size_t j = k; j < Q.columns(); ++j) {
                    Q_copy[i - k][j - k] = Q[i][j];
                }
            }

            T squared_norm = v.dot(v);
            for (size_t i = k; i < Q.rows(); ++i) {
                for (size_t j = k; j < Q.columns(); ++j) {
                    for (size_t l = k; l < Q.rows(); ++l) {
                        Q[i][j] -= (2.0*v[i - k]*v[l - k] / squared_norm) *
                                   Q_copy[l - k][j - k];
                    }
                }
            }
        }

        if (!full_matrices) {
            R.resize(R.columns(), R.columns());
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Hessenberg decomposition                                               //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the Hessenberg form of a matrix.
    template <class T>
    void hessenberg_raw(
        const numcpp::matrix<T> &A,
        numcpp::array<T> &tau,
        numcpp::matrix<T> &H
    ) {
        if (A.rows() != A.columns()) {
            throw std::runtime_error("hessenberg: Expected square matrix.");
        }

        T tol = std::numeric_limits<T>::epsilon();
        tau.resize(A.rows() - 1);
        H = A;

        for (size_t k = 0; k < H.columns() - 1; ++k) {
            numcpp::array<T> v(H.rows() - k);
            v[0] = T(0);
            for (size_t i = k + 1; i < H.rows(); ++i) {
                v[i - k] = H[i][k];
            }
            v[1] += (v[1] >= T(0)) ? numcpp::sqrt(v.dot(v))
                                   : -numcpp::sqrt(v.dot(v));

            T squared_norm = v.dot(v);
            if (squared_norm <= tol) {
                continue;
            }

            numcpp::matrix<T> H_copy(H.rows(), H.columns());
            for (size_t i = k + 1; i < H.rows(); ++i) {
                for (size_t j = k; j < H.columns(); ++j) {
                    H_copy[i][j] = H[i][j];
                }
            }
            for (size_t i = k + 1; i < H.rows(); ++i) {
                for (size_t j = k; j < H.columns(); ++j) {
                    for (size_t l = k + 1; l < H.rows(); ++l) {
                        H[i][j] -= (2.0*v[i - k]*v[l - k] / squared_norm) *
                                   H_copy[l][j];
                    }
                }
            }
            for (size_t i = 0; i < H.rows(); ++i) {
                for (size_t j = k + 1; j < H.columns(); ++j) {
                    H_copy[i][j] = H[i][j];
                }
            }
            for (size_t i = 0; i < H.rows(); ++i) {
                for (size_t j = k + 1; j < H.columns(); ++j) {
                    for (size_t l = k + 1; l < H.rows(); ++l) {
                        H[i][j] -= H_copy[i][l] *
                                   (2.0*v[l - k]*v[j - k] / squared_norm);
                    }
                }
            }

            tau[k] = v[1];
            for (size_t i = k + 2; i < H.rows(); ++i) {
                H[i][k] = v[i - k];
            }
        }
    }

    template <class T>
    void hessenberg(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &Q,
        numcpp::matrix<T> &H
    ) {
        T tol = std::numeric_limits<T>::epsilon();
        numcpp::array<T> tau;
        Q = numcpp::eye<T>(A.rows(), A.columns());
        hessenberg_raw(A, tau, H);

        for (size_t k = H.columns() - 2; k != -1; --k) {
            numcpp::array<T> v(H.rows() - k);
            v[0] = T(0);
            v[1] = tau[k];
            for (size_t i = k + 2; i < H.rows(); ++i) {
                v[i - k] = H[i][k];
                H[i][k] = T(0);
            }

            T squared_norm = v.dot(v);
            if (squared_norm <= tol) {
                continue;
            }

            numcpp::matrix<T> Q_copy(Q.rows() - k, Q.columns() - k);
            for (size_t i = k; i < Q.rows(); ++i) {
                for (size_t j = k; j < Q.columns(); ++j) {
                    Q_copy[i - k][j - k] = Q[i][j];
                }
            }
            for (size_t i = k; i < Q.rows(); ++i) {
                for (size_t j = k; j < Q.columns(); ++j) {
                    for (size_t l = k; l < Q.rows(); ++l) {
                        Q[i][j] -= (2.0*v[i - k]*v[l - k] / squared_norm) *
                                   Q_copy[l - k][j - k];
                    }
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Eigenvalues and eigenvectors                                           //
    ////////////////////////////////////////////////////////////////////////////

    // Computes the eigenvalues and eigenvectors of a symmetric matrix.
    template <class T>
    void eigen_symm(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &V,
        numcpp::array<T> &D
    ) {
        if (A.rows() != A.columns()) {
            throw std::runtime_error("eigen_symm: Expected square matrix.");
        }

        size_t max_iter = 1000000;
        T tol = std::numeric_limits<T>::epsilon();
        V = numcpp::eye<T>(A.rows(), A.columns());
        numcpp::matrix<T> S = A;

        numcpp::array<size_t> argmax(A.rows(), 0);
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (numcpp::abs(A[i][argmax[i]]) < numcpp::abs(A[i][j])) {
                    argmax[i] = j;
                }
            }
        }

        while (max_iter--) {
            size_t i = 1, j = argmax[1];
            for (size_t k = 1; k < S.rows(); ++k) {
                if (numcpp::abs(S[i][j]) < numcpp::abs(S[k][argmax[k]])) {
                    i = k;
                    j = argmax[k];
                }
            }

            if (numcpp::abs(S[i][j]) <= tol) {
                D = numcpp::diagonal(S);
                return;
            }

            T sii = S[i][i], sij = S[i][j], sjj = S[j][j];
            T theta = (sjj - sii) / (2.0*sij);
            T t = (theta >= T(0)) ? 1.0/(theta + numcpp::sqrt(1 + theta*theta))
                                  : 1.0/(theta - numcpp::sqrt(1 + theta*theta));
            T c = 1.0/numcpp::sqrt(1 + t*t);
            T s = c*t;

            for (size_t k = 0; k < V.rows(); ++k) {
                T vki = V[k][i];
                T vkj = V[k][j];
                V[k][i] = c*vki - s*vkj;
                V[k][j] = s*vki + c*vkj;
            }

            S[i][i] = c*c*sii - 2.0*s*c*sij + s*s*sjj;
            S[i][j] = S[j][i] = T(0);
            S[j][j] = s*s*sii + 2.0*s*c*sij + c*c*sjj;
            for (size_t k = 0; k < S.rows(); ++k) {
                if (k != i && k != j) {
                    T ski = S[k][i];
                    T skj = S[k][j];
                    S[i][k] = S[k][i] = c*ski - s*skj;
                    S[j][k] = S[k][j] = s*ski + c*skj;
                }
            }

            for (size_t k = 1; k < S.rows(); ++k) {
                if (k == i || k == j || argmax[k] == i || argmax[k] == j) {
                    for (size_t l = 0; l < k; ++l) {
                        if (numcpp::abs(S[k][argmax[k]]) < numcpp::abs(S[k][l]))
                        {
                            argmax[k] = l;
                        }
                    }
                }
                else {
                    if (numcpp::abs(S[k][argmax[k]]) < numcpp::abs(S[k][i])) {
                        argmax[k] = i;
                    }
                    if (numcpp::abs(S[k][argmax[k]] < numcpp::abs(S[k][j]))) {
                        argmax[k] = j;
                    }
                }
            }
        }

        throw LinAlgError("eigen_symm: Algorithm failed to converge.");
    }

    // Computes the eigenvalues (only) of a symmetric matrix.
    template <class T>
    numcpp::array<T> eigenvals_symm(const numcpp::matrix<T> &A) {
        if (A.rows() != A.columns()) {
            throw std::runtime_error("eigenvals_symm: Expected square matrix.");
        }

        size_t max_iter = 1000000;
        T tol = std::numeric_limits<T>::epsilon();
        numcpp::matrix<T> S = A;

        numcpp::array<size_t> argmax(A.rows(), 0);
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (numcpp::abs(A[i][argmax[i]]) < numcpp::abs(A[i][j])) {
                    argmax[i] = j;
                }
            }
        }

        while (max_iter--) {
            size_t i = 1, j = argmax[1];
            for (size_t k = 1; k < S.rows(); ++k) {
                if (numcpp::abs(S[i][j]) < numcpp::abs(S[k][argmax[k]])) {
                    i = k;
                    j = argmax[k];
                }
            }

            if (numcpp::abs(S[i][j]) <= tol) {
                return numcpp::diagonal(S);
            }

            T sii = S[i][i], sij = S[i][j], sjj = S[j][j];
            T theta = (sjj - sii) / (2.0*sij);
            T t = (theta >= T(0)) ? 1.0/(theta + numcpp::sqrt(1 + theta*theta))
                                  : 1.0/(theta - numcpp::sqrt(1 + theta*theta));
            T c = 1.0/numcpp::sqrt(1 + t*t);
            T s = c*t;

            S[i][i] = c*c*sii - 2.0*s*c*sij + s*s*sjj;
            S[i][j] = S[j][i] = T(0);
            S[j][j] = s*s*sii + 2.0*s*c*sij + c*c*sjj;
            for (size_t k = 0; k < S.rows(); ++k) {
                if (k != i && k != j) {
                    T ski = S[k][i];
                    T skj = S[k][j];
                    S[i][k] = S[k][i] = c*ski - s*skj;
                    S[j][k] = S[k][j] = s*ski + c*skj;
                }
            }

            for (size_t k = 1; k < S.rows(); ++k) {
                if (k == i || k == j || argmax[k] == i || argmax[k] == j) {
                    for (size_t l = 0; l < k; ++l) {
                        if (numcpp::abs(S[k][argmax[k]]) < numcpp::abs(S[k][l]))
                        {
                            argmax[k] = l;
                        }
                    }
                }
                else {
                    if (numcpp::abs(S[k][argmax[k]]) < numcpp::abs(S[k][i])) {
                        argmax[k] = i;
                    }
                    if (numcpp::abs(S[k][argmax[k]] < numcpp::abs(S[k][j]))) {
                        argmax[k] = j;
                    }
                }
            }
        }

        throw LinAlgError("eigenvals_symm: Algorithm failed to converge.");
    }

    ////////////////////////////////////////////////////////////////////////////
    // Singular value decomposition                                           //
    ////////////////////////////////////////////////////////////////////////////

    // Constructs the sigma matrix in SVD from singular values.
    template <class T>
    numcpp::matrix<T> diagsvd(const numcpp::array<T> &S, size_t m, size_t n) {
        numcpp::matrix<T> out = numcpp::zeros<T>(m, n);
        for (size_t i = 0; i < S.size(); ++i) {
            out[i][i] = S[i];
        }
        return out;
    }

    // Computes the singular value decomposition of a matrix.
    template <class T>
    void svd(
        const numcpp::matrix<T> &A,
        numcpp::matrix<T> &U,
        numcpp::array<T> &S,
        numcpp::matrix<T> &V,
        bool full_matrices = true
    ) {
        if (
            (full_matrices && A.rows() > A.columns()) ||
            (!full_matrices && A.rows() < A.columns())
        ) {
            T tol = std::numeric_limits<T>::epsilon();
            eigen_symm(numcpp::dot(A, A.transpose()), U, S);

            for (size_t k = 0; k < S.size(); ++k) {
                size_t piv = k;
                for (size_t i = k + 1; i < S.size(); ++i) {
                    if (S[piv] < S[i]) {
                        piv = i;
                    }
                }
                std::swap(S[k], S[piv]);
                S[k] = (S[k] >= 0) ? numcpp::sqrt(S[k]) : T(0);
                for (size_t i = 0; i < U.rows(); ++i) {
                    std::swap(U[i][k], U[i][piv]);
                }
            }

            if (full_matrices) {
                S.resize(A.columns());
                V = numcpp::zeros<T>(A.columns(), A.columns());
            }
            else {
                V = numcpp::zeros<T>(A.columns(), A.rows());
            }

            for (size_t j = 0; j < V.columns(); ++j) {
                if (S[j] <= tol) {
                    continue;
                }
                for (size_t i = 0; i < V.rows(); ++i) {
                    for (size_t k = 0; k < U.rows(); ++k) {
                        V[i][j] += A[k][i] * U[k][j];
                    }
                    V[i][j] /= S[j];
                }
            }
        }
        else {
            T tol = std::numeric_limits<T>::epsilon();
            eigen_symm(numcpp::dot(A.transpose(), A), V, S);

            for (size_t k = 0; k < S.size(); ++k) {
                size_t piv = k;
                for (size_t i = k + 1; i < S.size(); ++i) {
                    if (S[piv] < S[i]) {
                        piv = i;
                    }
                }
                std::swap(S[k], S[piv]);
                S[k] = (S[k] >= 0) ? numcpp::sqrt(S[k]) : T(0);
                for (size_t i = 0; i < V.rows(); ++i) {
                    std::swap(V[i][k], V[i][piv]);
                }
            }

            if (full_matrices) {
                S.resize(A.rows());
                U = numcpp::zeros<T>(A.rows(), A.rows());
            }
            else {
                U = numcpp::zeros<T>(A.rows(), A.columns());
            }

            for (size_t j = 0; j < U.columns(); ++j) {
                if (S[j] <= tol) {
                    continue;
                }
                for (size_t i = 0; i < U.rows(); ++i) {
                    for (size_t k = 0; k < V.rows(); ++k) {
                        U[i][j] += A[i][k] * V[k][j];
                    }
                    U[i][j] /= S[j];
                }
            }
        }
    }

    // Compute the singular values of a matrix.
    template <class T>
    numcpp::array<T> svdvals(const numcpp::matrix<T> &A) {
        numcpp::array<T> S;
        if (A.rows() < A.columns()) {
            S = eigenvals_symm(numcpp::dot(A, A.transpose()));
        }
        else {
            S = eigenvals_symm(numcpp::dot(A.transpose(), A));
        }
        for (size_t i = 0; i < S.size(); ++i) {
            size_t piv = i;
            for (size_t j = i + 1; j < S.size(); ++j) {
                if (S[piv] < S[j]) {
                    piv = j;
                }
            }
            std::swap(S[i], S[piv]);
            S[i] = (S[i] >= T(0)) ? numcpp::sqrt(S[i]) : T(0);
        }
        return S;
    }

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
    ) {
        if (A.rows() != b.size()) {
            std::ostringstream error;
            error << "lstsq: Number of rows in left operand does not match "
                  << "number of rows in right operand: (" << A.rows() << ","
                  << A.columns() << ") (" << b.size() << ",)";
            throw std::runtime_error(error.str());
        }

        if (method == "svd") {
            numcpp::matrix<T> U, V;
            numcpp::array<T> S;
            svd(A, U, S, V, false);

            numcpp::array<T> x = numcpp::zeros<T>(S.size());
            for (size_t i = 0; i < S.size(); ++i) {
                if (S[i] <= cond*S[0]) {
                    continue;
                }
                for (size_t j = 0; j < U.rows(); ++j) {
                    x[i] += U[j][i] * b[j];
                }
                x[i] /= S[i];
            }
            x = numcpp::dot(V, x);

            return x;
        }
        else if (method == "qr") {
            numcpp::matrix<T> Q, R;
            qr(A, Q, R, false);
            numcpp::array<T> x = numcpp::dot(Q.transpose(), b);
            x = solve_triangular(R, x, false, false, false);
            return x;
        }
        else {
            throw std::invalid_argument(
                "method must be one of \"qr\" or \"svd\"."
            );
        }
    }

    template <class T>
    numcpp::matrix<T> lstsq(
        const numcpp::matrix<T> &A,
        const numcpp::matrix<T> &b,
        const std::string &method = "svd",
        const T cond = 1e-8
    ) {
        if (A.rows() != b.rows()) {
            std::ostringstream error;
            error << "lstsq: Number of rows in left operand does not match "
                  << "number of rows in right operand: (" << A.rows() << ","
                  << A.columns() << ") (" << b.rows() << "," << b.columns()
                  << ")";
            throw std::runtime_error(error.str());
        }

        if (method == "svd") {
            numcpp::matrix<T> U, V;
            numcpp::array<T> S;
            svd(A, U, S, V, false);

            numcpp::matrix<T> x = numcpp::zeros<T>(S.size(), b.columns());
            for (size_t i = 0; i < S.size(); ++i) {
                if (S[i] <= cond*S[0]) {
                    continue;
                }
                for (size_t j = 0; j < b.columns(); ++j) {
                    for (size_t k = 0; k < U.rows(); ++k) {
                        x[i][j] += U[k][i] * b[k][j];
                    }
                    x[i][j] /= S[i];
                }
            }
            x = numcpp::dot(V, x);

            return x;
        }
        else if (method == "qr") {
            numcpp::matrix<T> Q, R;
            qr(A, Q, R, false);
            numcpp::matrix<T> x = numcpp::dot(Q.transpose(), b);
            x = solve_triangular(R, x, false, false, false);
            return x;
        }
        else {
            throw std::invalid_argument(
                "method must be one of \"qr\" or \"svd\"."
            );
        }
    }

    // Compute the Moore-Penrose pseudo-inverse of a matrix.
    template <class T>
    numcpp::matrix<T> pinv(const numcpp::matrix<T> &A, const T &cond = 1e-8) {
        return lstsq(A, numcpp::eye<T>(A.rows(), A.rows()), "svd", cond);
    }

    // Constructs an orthonormal basis for the range of A using SVD.
    template <class T>
    numcpp::matrix<T> orth(
        const numcpp::matrix<T> &A,
        const T &cond = 1e-8
    ) {
        numcpp::matrix<T> U, V;
        numcpp::array<T> S;
        svd(A, U, S, V, false);

        size_t k;
        for (k = 0; k < S.size(); ++k) {
            if (S[k] <= cond*S[0]) {
                break;
            }
        }

        numcpp::matrix<T> out(U.rows(), k);
        for (size_t i = 0; i < U.rows(); ++i) {
            for (size_t j = 0; j < k; ++j) {
                out[i][j] = U[i][j];
            }
        }
        return out;
    }

    // Constructs an orthonormal basis for the null space of A using SVD.
    template <class T>
    numcpp::matrix<T> null_space(
        const numcpp::matrix<T> &A,
        const T &cond = 1e-8
    ) {
        numcpp::matrix<T> U, V;
        numcpp::array<T> S;
        svd(A, U, S, V, true);

        size_t k;
        for (k = 0; k < S.size(); ++k) {
            if (S[k] <= cond*S[0]) {
                break;
            }
        }

        numcpp::matrix<T> out(V.rows(), V.columns() - k);
        for (size_t i = 0; i < V.rows(); ++i) {
            for (size_t j = k; j < V.columns(); ++j) {
                out[i][j - k] = V[i][j];
            }
        }
        return out;
    }
}

#endif // LINALG_H_INCLUDED
