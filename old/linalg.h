#ifndef LINALG_H_INCLUDED
#define LINALG_H_INCLUDED

#include <cmath>
#include <complex>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "matrix.h"

namespace num_met {    

    /* Calcula la factorizacion LU con pivoteo de una matriz.
    Argumentos:
        A    - La matriz a factorizar.
        LU   - Matriz correspondiente a la factorizacion. Los elementos de L estaran en la parte inferior y los elementos de 
               U en la parte superior. La diagonal de 1s de L es descartada.
        P    - Iterador correspondiente a la permutacion.
        sign - Si es true, guarda al final de P el signo de la permutacion. */
    template <class T, class RandomAccessIterator>
    void lu_decomposition(const matrix<T> &A, matrix<T> &LU, RandomAccessIterator P, bool sign = false) {
        if (A.rows() != A.columns())
            throw std::invalid_argument("Expected square matrix.");

        LU = A;
        for (unsigned int i = 0; i < LU.rows(); ++i)
            P[i] = i;
        if (sign)
            P[LU.rows()] = 1;
        
        for (unsigned int k = 0; k < LU.rows(); ++k) {
            // Pivoteo.
            unsigned int piv = k;
            for (unsigned int i = k + 1; i < LU.rows(); ++i) 
                if (std::abs(LU[piv][k]) < std::abs(LU[i][k]))
                    piv = i;
            if (std::abs(LU[piv][k]) <= std::numeric_limits<T>::epsilon())
                continue;
            if (piv != k) {
                if (sign)
                    P[LU.rows()] *= -1;
                for (unsigned int j = 0; j < LU.columns(); ++j)
                    std::swap(LU[piv][j], LU[k][j]);
                std::swap(P[piv], P[k]);
            }

            // Eliminacion gaussiana.
            for (unsigned int i = k + 1; i < LU.rows(); ++i) {
                LU[i][k] /= LU[k][k];
                for (unsigned int j = k + 1; j < LU.columns(); ++j)
                    LU[i][j] -= LU[i][k] * LU[k][j];
            }
        }
    }


    /* Calcula la factorizacion de Cholesky de una matriz.
    Argumentos:
        A - La matriz a factorizar.
        L - Matriz correspondiente a la factorizacion. Los elementos de la factorizacion estaran contenidos tanto en la parte 
            superior como en parte la inferior de L. */
    template <class T>
    void cholesky_decomposition(const matrix<T> &A, matrix<T> &L) {
        if (A.rows() != A.columns())
            throw std::invalid_argument("Expected square matrix."); 
        L = matrix<T>(A.rows(), A.columns());

        for (unsigned int j = 0; j < A.columns(); ++j) {
            L[j][j] = T();
            for (unsigned int k = 0; k < j; ++k)
                L[j][j] += L[j][k] * L[j][k];
            L[j][j] = A[j][j] - L[j][j];

            if (L[j][j] < 0 || std::sqrt(L[j][j]) <= std::numeric_limits<T>::epsilon())
                throw std::runtime_error("Matrix is not positive definite.");

            L[j][j] = std::sqrt(L[j][j]);
            for (unsigned int i = j + 1; i < A.rows(); ++i) {
                L[i][j] = T();
                for (unsigned int k = 0; k < j; ++k)
                    L[i][j] += L[i][k] * L[j][k];
                L[i][j] = (A[i][j] - L[i][j]) / L[j][j];
                L[j][i] = L[i][j];
            }
        }
    }


    /* Calcula la factorizacion QR de una matriz.
    Argumentos:
       A    - La matriz a factorizar.
       Q, R - Matrices donde se guardara la factorizacion. */
    template <class T>
    void qr_decomposition(const matrix<T> &A, matrix<T> &Q, matrix<T> &R) {
        if (A.rows() < A.columns()) 
            throw std::invalid_argument("Expected more rows than columns.");
        Q = identity<T>(A.rows());
        R = A;
        
        for (unsigned int k = 0; k < R.columns(); ++k) {
            // Reflexion de Householder.
            T v0 = R[k][k];
            T vnorm = T();
            for (unsigned int i = k + 1; i < R.rows(); ++i)
                vnorm += R[i][k] * R[i][k];
            R[k][k] += (v0 >= 0 ? 1.0 : -1.0) * std::sqrt(vnorm + v0*v0);
            
            vnorm += R[k][k]*R[k][k];
            if (std::sqrt(vnorm) <= std::numeric_limits<T>::epsilon())
                throw std::runtime_error("Linearly dependent columns.");

            matrix<T> H(R.rows() - k, R.rows() - k);
            for (unsigned int i = 0; i < H.rows(); ++i)
                for (unsigned int j = 0; j < H.columns(); ++j)
                    H[i][j] = (2*R[i + k][k]*R[j + k][k]) / vnorm;

            // Actualiza las matrices.
            R[k][k] = v0;
            R(k, R.rows(), k, R.columns()) -= matmul(H, R(k, R.rows(), k, R.columns()).to_matrix());
            Q(0, Q.rows(), k, Q.columns()) -= matmul(Q(0, Q.rows(), k, Q.columns()).to_matrix(), H);
        }

        // Fuerza a ser 0 la parte inferior de R.
        for (unsigned int j = 0; j < R.columns(); ++j)
            for (unsigned int i = j + 1; i < R.rows(); ++i)
                R[i][j] = T();
    }


    /* Calcula los eigenvalores de una matriz.
    Argumentos: 
       A        - Una matriz cuadrada.
       eigvals  - Un iterador correspondiente a los eigenvalores.
       max_iter - Maximo numero de iteraciones.
       tol      - Tolerancia. */
    template <class T, class OutputIterator>
    void eigenvalues(const matrix<T> &A, OutputIterator eigvals, unsigned int max_iter = 1000, const T &tol = 1e-6) {
        if (A.rows() != A.columns())
            throw std::invalid_argument("Expected square matrix.");
        matrix<T> B = A, Q, R;

        for (unsigned int iter = 0; iter < max_iter; ++iter) {
            // Verifica que los elementos debajo de la subdiagonal sean 0s y en la subdiagonal no haya no-ceros consecutivos.
            bool converge = true;
            for (unsigned int i = 0; converge && i < B.rows(); ++i) {
                for (unsigned int j = 0; converge && j + 1 < i; ++j)
                    if (std::abs(B[i][j]) > tol)
                        converge = false;
                if (i + 2 < B.rows() && std::abs(B[i + 1][i]) > tol && std::abs(B[i + 2][i + 1]) > tol)
                    converge = false;
            }   
            if (converge)
                break;
            // Metodo QR.
            qr_decomposition(B, Q, R);
            B = matmul(R, Q);
        }

        // Calcula los eigenvalores.
        for (unsigned int i = 0; i < B.rows(); ++i) {
            // Hay un bloque de 2 x 2 sobre la diagonal. Esto corresponde a un par de eigenvalores complejos.
            if (i + 1 < B.rows() && std::abs(B[i + 1][i]) > tol) {
                std::complex<T> b = B[i][i] + B[i + 1][i + 1];
                std::complex<T> c = B[i][i]*B[i + 1][i + 1] - B[i][i + 1]*B[i + 1][i];
                *eigvals = (b - std::sqrt(b*b - 4.0*c)) / 2.0;
                ++eigvals;
                *eigvals = (b + std::sqrt(b*b - 4.0*c)) / 2.0;
                ++i;
            }
            // Solo hay un valor en la diagonal.
            else 
                *eigvals = B[i][i];
            ++eigvals;
        }
    }


    /* Calcula los eigenvalores y eigenvecroes de una matriz simetrica. 
    Argumentos:
       A   - Una matriz simetrica.
       V   - La matriz con los eigenvectores en las columnas.
       D   - Un iterador correspondiente a los eigenvalores. 
       tol - Tolerancia. */
    template <class T, class OutputIterator>
    void eigen_symmetric(const matrix<T> &A, matrix<T> &V, OutputIterator D, const T &tol = 1e-6) {
        if (A.rows() != A.columns())
            throw std::invalid_argument("Expected square matrix.");
        // Metodo de Jacobi para eigenvectores y eigenvalores.
        V = identity<T>(A.rows());
        matrix<T> B(A);

        while (true) {
            // Mayor elemento fuera de la diagonal.
            unsigned int i = 1, j = 0;
            for (unsigned int r = 0; r < B.rows(); ++r)
                for (unsigned int c = 0; c < r; ++c)
                    if (std::abs(B[i][j]) < std::abs(B[r][c])) {
                        i = r;
                        j = c;
                    }

            // Convergencia del metodo.
            if (std::abs(B[i][j]) <= tol) {
                for (unsigned int k = 0; k < B.rows(); ++k) {
                    *D = B[k][k];
                    ++D;
                }
                break;
            }

            // Rotacion de Givens.
            T bii = B[i][i];
            T bij = B[i][j];
            T bjj = B[j][j];
            T delta = (bjj - bii) / (2.0*bij);
            T t = (delta >= 0 ? 1.0 : -1.0) / (std::abs(delta) + std::sqrt(1.0 + delta*delta));
            T c = 1.0 / std::sqrt(1 + t*t);
            T s = c*t;

            // Actualiza V.
            for (unsigned int k = 0; k < V.rows(); ++k) {
                T vki = V[k][i];
                T vkj = V[k][j];
                V[k][i] = c*vki - s*vkj;
                V[k][j] = s*vki + c*vkj;
            }

            // Actualiza B.
            B[i][i] = c*c*bii - 2*s*c*bij + s*s*bjj;
            B[i][j] = B[j][i] = 0.0;
            B[j][j] = s*s*bii + 2*s*c*bij + c*c*bjj;
            for (unsigned int k = 0; k < B.rows(); ++k) 
                if (k != i && k != j) {
                    T bki = B[k][i];
                    T bkj = B[k][j];
                    B[i][k] = B[k][i] = c*bki - s*bkj;
                    B[j][k] = B[k][j] = s*bki + c*bkj;
                }
        }
    }


    /* Construye la matriz de valores singulares. 
    Argumentos:
       m, n  - Las dimensiones de la matriz.
       svals - Un iterador correspondiente a los valores singulares. 
    Regresa:
       La matriz con los valores singulares en la diagonal. */
    template <class InputIterator, class T = typename std::iterator_traits<InputIterator>::value_type>
    matrix<T> diag_svd(unsigned int m, unsigned int n, InputIterator svals) {
        matrix<T> S(m, n, T());
        for (unsigned int i = 0; i < m && i < n; ++i) {
            S[i][i] = *svals;
            ++svals;
        }
        return S;
    }


    /* Calcula la factorizacion SVD de una matriz rectangular. 
    Argumentos:
       A             - La matriz a factorizar.
       U             - La matriz de vectores singulares izquierdos.
       S             - Un iterador correspondiente a los valores singulares. 
       V             - La matriz de vectores singulares derechos.
       full_matrices - Si es true, U, S y V seran matrices de mxm, mxn y nxn. Si es false, de mxk, kxk y kxn, donde k = min(m, n),
       tol           - Tolerancia. */
    template <class T, class OutputIterator>
    void svd_decomposition(const matrix<T> &A, matrix<T> &U, OutputIterator S, matrix<T> &V, bool full_matrices = true, const T &tol = 1e-6) {
        if ((full_matrices && A.rows() <= A.columns()) || (!full_matrices && A.rows() >= A.columns())) {
            // Genera la matriz V.
            T *D = new T[A.columns()];
            eigen_symmetric(matmul(transpose(A), A), V, D, tol);

            // Genera los valores singulares en orden no decreciente.
            for (unsigned int i = 0; i < A.rows() && i < A.columns(); ++i) {
                unsigned int piv = i;
                for (unsigned int j = i + 1; j < V.rows(); ++j)
                    if (D[piv] < D[j])
                        piv = j;
                for (unsigned int j = 0; j < V.rows(); ++j)
                    std::swap(V[j][piv], V[j][i]);
                std::swap(D[piv], D[i]);
                *S = D[i] = std::sqrt(D[i]);
                ++S;
            }

            // Genera la matriz U.
            U = matmul(A, V);
            if (full_matrices)
                U = U(0, A.rows(), 0, A.rows()).to_matrix();
            for (unsigned int j = 0; j < U.columns(); ++j)
                if (D[j] > std::numeric_limits<T>::epsilon()) {
                    for (unsigned int i = 0; i < U.rows(); ++i) 
                        U[i][j] /= D[j];
                }
            delete[] D;
        }
        else 
            svd_decomposition(transpose(A), V, S, U, full_matrices, tol);
    }


    /* Resuelve la ecuacion Ax = b suponiendo que A es una matrix triangular.
    Argumentos:
       A             - Una matriz triangular.
       b             - Lado derecho de la ecuacion.
       lower         - Escoger true si A es triangular inferior. Escoger false si A es triangular superior.
       unit_diagonal - Si es false, utiliza los valores en la diagonal de A. Si es true, supone que hay 1s en la diagonal.
    Regresa: 
       Una matriz con la solucion de la ecuacion. */
    template <class T>
    matrix<T> solve_triangular(const matrix<T> &A, const matrix<T> &b, bool lower, bool unit_diagonal = false) {
        if (A.rows() != A.columns() || A.rows() != b.rows())
            throw std::invalid_argument("No square matrix or matrix shapes don't match.");
        matrix<T> x(A.columns(), b.columns(), T());
        if (lower) {
            for (unsigned int j = 0; j < b.columns(); ++j) {
                for (unsigned int i = 0; i < b.rows(); ++i) {
                    for (unsigned int k = 0; k < i; ++k)
                        x[i][j] += A[i][k] * x[k][j];
                    if (unit_diagonal)
                        x[i][j] = b[i][j] - x[i][j];
                    else {
                        if (std::abs(A[i][i]) <= std::numeric_limits<T>::epsilon())
                            throw std::runtime_error("Singular matrix.");
                        x[i][j] = (b[i][j] - x[i][j]) / A[i][i];
                    }
                }
            }
        }
        else {
            for (unsigned int j = 0; j < b.columns(); ++j) {
                for (int i = b.rows() - 1; i >= 0; --i) {
                    for (unsigned int k = i + 1; k < A.columns(); ++k)
                        x[i][j] += A[i][k] * x[k][j];
                    if (unit_diagonal)
                        x[i][j] = b[i][j] - x[i][j];
                    else {
                        if (std::abs(A[i][i]) <= std::numeric_limits<T>::epsilon())
                            throw std::runtime_error("Singular matrix.");
                        x[i][j] = (b[i][j] - x[i][j]) / A[i][i];
                    }
                }
            }
        }
        return x;
    }


    /* Resuelve la ecuacion Ax = b dada la factorizacion LU de A.
    Argumentos:
       LU, P - Los elementos de la factorizacion.
       b     - Lado derecho de la ecuacion.
    Regresa: 
       Una matriz con la solucion de la ecuacion. */
    template <class T, class RandomAccessIterator>
    matrix<T> solve_lu(const matrix<T> &LU, RandomAccessIterator P, const matrix<T> &b) {
        matrix<T> x(b.rows(), b.columns());
        // Permuta b.
        for (unsigned int i = 0; i < b.rows(); ++i)
            for (unsigned int j = 0; j < b.columns(); ++j)
                x[i][j] = b[P[i]][j];
        // Resuelve por pedazos.
        x = solve_triangular(LU, x, true, true);
        x = solve_triangular(LU, x, false, false);
        return x;
    }


    /* Resuelve la ecuacion Ax = b dada la factorizacion de cholesky de A.
    Argumentos:
       L - Los elementos de la factorizacion.
       b - Lado derecho de la ecuacion.
    Regresa: 
       Una matriz con la solucion de la ecuacion. */
    template <class T>
    matrix<T> solve_cholesky(const matrix<T> &L, const matrix<T> &b) {
        matrix<T> x;
        x = solve_triangular(L, b, true, false);
        x = solve_triangular(L, x, false, false);
        return x;
    }


    /* Resuelve la ecuacion Ax = b.
    Argumentos:
       A - Lado izquierdo de la ecuacion.
       b - Lado derecho de la ecuacion.
    Regresa: 
       Una matriz con la solucion de la ecuacion. */
    template <class T>
    matrix<T> solve(const matrix<T> &A, const matrix<T> &b) {
        unsigned int *P = new unsigned int[A.rows()];
        matrix<T> LU;
        lu_decomposition(A, LU, P);
        matrix<T> x = solve_lu(LU, P, b);
        delete[] P;
        return x;
    }


    /* Resuelve la ecuacion Ax = b por minimos cuadrados.
    Argumentos:
       A - Lado izquierdo de la ecuacion.
       b - Lado derecho de la ecuacion.
    Regresa: 
       Una matriz con la solucion de la ecuacion. */
    template <class T>
    matrix<T> linear_lstsq(const matrix<T> &A, const matrix<T> &b) {
        // Prueba resolviendo el sistema At.A = At.b por Cholesky.
        try {
            if (A.rows() < A.columns()) 
                throw std::invalid_argument("Expected more rows than columns.");

            matrix<T> At = transpose(A), L, y;
            cholesky_decomposition(matmul(At, A), L);
            y = matmul(At, b);
            matrix<T> x = solve_cholesky(L, y);
            return x;
        }
        // Resuelve directamente usando la factorizacion SVD de A.
        catch (...) {
            matrix<T> U, V;
            T *S = new T[std::min(A.rows(), A.columns())];
            svd_decomposition(A, U, S, V, false);

            matrix<T> x = matmul(transpose(U), b);
            for (unsigned int i = 0; i < x.rows(); ++i) 
            	if (S[i] > std::numeric_limits<T>::epsilon()) {
                    for (unsigned int j = 0; j < x.columns(); ++j)
                        x[i][j] /= S[i];
                }
            x = matmul(V, x);

            delete[] S;
            return x;
        }
    }


    /* Invierte una matriz.
    Argumentos:
       A - La matriz a invertir.
    Regresa:
       La matriz inversa. */
    template <class T>
    matrix<T> inverse(const matrix<T> &A) {
        return solve(A, identity<T>(A.rows()));
    }


    /* Calcula el determinante de una matriz.
    Argumentos:
       A - Una matriz cuadrada.
    Regresa:
       El determinante de la matriz. */
    template <class T>
    T determinant(const matrix<T> &A) {
        int *P = new int[A.rows() + 1];
        matrix<T> LU;
        lu_decomposition(A, LU, P, true);

        T det = T(P[A.rows()]);
        for (unsigned int i = 0; i < A.rows(); ++i)
            det *= LU[i][i];

        delete[] P;
        return det;
    }


    /* Calcula la pseudo-inversa una matriz.
    Argumentos:
       A - La matriz a invertir.
    Regresa:
       La matriz pseudo-inversa. */
    template <class T>
    matrix<T> pseudoinverse(const matrix<T> &A) {
        return linear_lstsq(A, identity<T>(A.rows()));
    }

}

#endif // LINALG_H_INCLUDED