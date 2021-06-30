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
                if (std::abs(LU[pivot][k]) < std::abs(LU[i][k])) {
                    pivot = i;
                }
            }
            if (std::abs(LU[pivot][k]) <= tol) {
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
        P = numcpp::zeros(A.rows(), A.columns());
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
        L = numcpp::zeros(A.rows(), A.columns());
        D = numcpp::zeros(A.rows());

        for (size_t j = 0; j < A.columns(); ++j) {
            L[j][j] = T(1);
            for (size_t k = 0; k < j; ++k) {
                D[j] += L[j][k] * D[k] * L[j][k];
            }
            D[j] = A[j][j] - D[j];
            if (std::abs(D[j]) <= tol) {
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
        L = numcpp::zeros(A.rows(), A.columns());

        for (size_t j = 0; j < A.columns(); ++j) {
            for (size_t k = 0; k < j; ++k) {
                L[j][j] += L[j][k] * L[j][k];
            }
            L[j][j] = A[j][j] - L[j][j];

            if (L[j][j] <= tol) {
                throw LinAlgError("Non symmetric positive-definite matrix.");
            }
            L[j][j] = std::sqrt(L[j][j]);

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
        if (A.columns() != b.size()) {
            std::ostringstream error;
            error << "matmul: Number of columns in left operand does not match "
                  << "number of rows in right operand: (" << A.rows() << ","
                  << A.columns() << ") (" << b.size() << ",)";
            throw std::runtime_error(error.str());
        }

        T tol = std::numeric_limits<T>::epsilon();
        numcpp::array<T> x = numcpp::zeros(b.size());
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
                    if (std::abs(A[i][i]) <= tol) {
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
                    if (std::abs(A[i][i]) <= tol) {
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
        if (A.columns() != b.rows()) {
            std::ostringstream error;
            error << "matmul: Number of columns in left operand does not match "
                  << "number of rows in right operand: (" << A.rows() << ","
                  << A.columns() << ") (" << b.rows() << "," << b.columns()
                  << ")";
            throw std::runtime_error(error.str());
        }

        T tol = std::numeric_limits<T>::epsilon();
        numcpp::matrix<T> x = numcpp::zeros(b.rows(), b.columns());
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
                        if (std::abs(A[i][i]) <= tol) {
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
                        if (std::abs(A[i][i]) <= tol) {
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
}

#endif // LINALG_H_INCLUDED
