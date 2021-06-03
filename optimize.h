#ifndef OPTIMIZE_H_INCLUDED
#define OPTIMIZE_H_INCLUDED

#include <cmath>
#include <complex>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include "matrix.h"
#include "linalg.h"

namespace num_met {

    /* Calcula todas las raices de un polinomio con coeficientes reales. */
    template <
        class InputIterator,
        class OutputIterator,
        class T = typename std::iterator_traits<InputIterator>::value_type
    >
    void poly_root(
        unsigned int n,               // Grado del polinomio.
        InputIterator coef,           // Coeficientes del polinomio.
        OutputIterator roots,         // Raices del polinomio (Salida).
        unsigned int max_iter = 1000, // Maximo numero de iteraciones.
        T tol = 1e-6                  // Tolerancia.
    ) {
        // Polinomio lineal: y = mx + b.
        if (n == 1) {
            T m = *coef;
            ++coef;
            T b = *coef;
            *roots = -b / m;
        }
        // Polinomio cuadratico: y = ax^2 + bx + c.
        else if (n == 2) {
            std::complex<T> a = *coef;
            ++coef;
            std::complex<T> b = *coef;
            ++coef;
            std::complex<T> c = *coef;
            *roots = (-b - std::sqrt(b*b - 4.0*a*c)) / (2.0*a);
            ++roots;
            *roots = (-b + std::sqrt(b*b - 4.0*a*c)) / (2.0*a);
        }
        // Caso general. Calcular los eigenvalores de la matriz compañera.
        else {
            matrix<T> companion(n, n, T());
            T an = *coef;
            ++coef;
            for (unsigned int i = 1; i < n; ++i) {
                companion[i][i - 1] = 1.0;
            }
            for (unsigned int i = n; i > 0; --i) {
                companion[i - 1][n - 1] = -*coef / an;
                ++coef;
            }
            eigenvalues(companion, roots, max_iter, tol);
        }
    }


    /* Encuentra una raiz de una funcion usando el metodo de biseccion. */
    template <class Function, class T>
    std::pair<T, unsigned int> bisection(
        Function f,                   // Funcion cuya raiz queremos encontrar.
        T a,                          // Extremo izquierdo del intervalo.
        T b,                          // Extremo derecho del intervalo.
        unsigned int max_iter = 1000, // Maximo numero de iteraciones.
        T tol = 1e-6                  // Tolerancia.
    ) {
        for (unsigned int n = 0; n <= max_iter; ++n) {
            T m = (a + b) / 2;
            T fa = f(a), fb = f(b), fm = f(m);
            // Convergencia del metodo.
            if (std::fabs(fm) <= tol) {
                return std::make_pair(m, n);
            }
            // Metodo de biseccion.
            if ((fa < 0 && fm > 0) || (fa > 0 && fm < 0)) {
                b = m;
            }
            else if ((fm < 0 && fb > 0) || (fm > 0 && fb < 0)) {
                a = m;
            }
            else {
                throw std::invalid_argument(
                    "f(a) and f(b) must have different signs."
                );
            }
        }
        throw std::runtime_error(
            "Failed to converge after "+std::to_string(max_iter)+" iterations."
        );
    }


    /* Encuentra una raiz de una funcion usando el metodo de Newton o el metodo
       de la secante. */
    template <class Function1, class Function2, class T>
    std::pair<T, unsigned int> newton(
        Function1 f,                  // Funcion cuya raiz queremos calcular.
        Function2 df,                 // Derivada de la funcion.
        T x0,                         // Aproximacion inicial.
        T x1,                         // Segunda aproximacion a la raiz.
        unsigned int max_iter = 1000, // Maximo numero de iteraciones.
        T tol = 1e-6                  // Tolerancia.
    ) {
        for (unsigned int n = 0; n <= max_iter; ++n) {
            // Convergencia el metodo.
            T fx = f(x0);
            if (std::fabs(fx) <= tol) {
                return std::make_pair(x0, n);
            }
            // Metodo de Newton.
            T dfx = df(x0);
            if (std::fabs(dfx) <= tol) {
                throw std::runtime_error("Derivative was zero.");
            }
            x0 -= fx / dfx;
        }
        throw std::runtime_error(
            "Failed to converge after "+std::to_string(max_iter)+" iterations."
        );
    }

    template <class Function, class T>
    std::pair<T, unsigned int> newton(
        Function f,                   // Funcion cuya raiz queremos calcular.
        std::nullptr_t df,            // Derivada de la funcion.
        T x0,                         // Aproximacion inicial.
        T x1,                         // Segunda aproximacion a la raiz.
        unsigned int max_iter = 1000, // Maximo numero de iteraciones.
        T tol = 1e-6                  // Tolerancia.
    ) {
        for (int n = 0; n <= max_iter; ++n) {
            // Convergencia el metodo.
            T fx = f(x0);
            if (std::fabs(fx) <= tol) {
                return std::make_pair(x0, n);
            }
            // Metodo de la secante.
            T fx1 = f(x1);
            if (std::fabs(fx1 - fx) <= tol) {
                throw std::runtime_error("Derivative was zero.");
            }
            T x = x1 - (x1 - x0)/(fx1 - fx) * fx1;
            x0 = x1;
            x1 = x;
        }
        throw std::runtime_error(
            "Failed to converge after "+std::to_string(max_iter)+" iterations."
        );
    }

}

#endif // OPTIMIZE_H_INCLUDED
