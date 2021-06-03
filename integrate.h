#ifndef INTEGRATE_H_INCLUDED
#define INTEGRATE_H_INCLUDED

#include <cmath>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include "matrix.h"
#include "linalg.h"

namespace num_met {

    // Calcula los coeficientes de diferencias finitas centrales.
    template <class OutputIterator>
    void central_diff_weights(
        unsigned int n,        // Numero de coeficientes a calcular.
        OutputIterator coeff,  // Iterador correspondiente a los coeficientes.
        unsigned int order = 1 // Orden de la derivada.
    ) {
        using T = typename std::iterator_traits<OutputIterator>::value_type;
        if (n <= order || n % 2 == 0) {
            throw std::invalid_argument(
                "Number of coefficients must be odd and larger than the order "
                "of the derivative."
            );
        }
        matrix<T> A(n, n), b(n, 1, T());
        for (unsigned int j = 0; j < n; ++j) {
            A[0][j] = 1.0;
            for (unsigned int i = 1; i < n; ++i) {
                A[i][j] = A[i - 1][j] * (j - (n - 1) / 2.0);
            }
        }
        b[order][0] = 1.0;
        for (unsigned int i = 2; i <= order; ++i) {
            b[order][0] *= T(i);
        }
        matrix<T> x = solve(A, b);
        for (unsigned int i = 0; i < n; ++i) {
            *coeff = x[i][0];
            ++coeff;
        }
    }


    /* Calcula una derivada en un punto. */
    template <class Function, class T>
    T derivative(
        Function f,            // La funcion a derivar.
        T x,                  // El punto sobre el cual derivar.
        T h = 1e-6,           // El tamaño del paso.
        unsigned int n = 1,   // Orden de la derivada.
        unsigned int npts = 3 // Numero de puntos para aproximar la derivada.
    ) {
        if (n == 0) {
            return f(x);
        }
        else if (n == 1 && npts == 3) {
            return (f(x + h) - f(x - h)) / (2.0*h);
        }
        else {
            // Calcula los coeficientes de diferencias finitas centrales.
            T *coeff = new T[npts];
            central_diff_weights(npts, coeff, n);

            T df = 0, dx = 1;
            int t = -((npts - 1) / 2);
            for (unsigned int i = 0; i < npts; ++i, ++t) {
                df += coeff[i] * f(x + t*h);
            }
            for (unsigned int i = 0; i < n; ++i) {
                dx *= h;
            }

            delete[] coeff;
            return df/dx;
        }
    }


    /* Evalua una integral definida. */
    template <class Function, class T>
    T integrate(
        Function f,           // La funcion a integrar.
        T a,                  // Limite inferior de integracion.
        T b,                  // Limite superior de integracion.
        unsigned int n = 1024 // Tamaño de la particion.
    ) {
        unsigned int m = 0;
        while ((1 << m) <= n) {
            m++;
        }

        // Regla del trapecio.
        matrix<T> R(m, m);
        R[0][0] = (b - a) * (f(a) + f(b)) / 2.0;
        for (unsigned int i = 1; i < m; ++i) {
            T r = 0;
            T h = (b - a) / (1 << i);
            for (unsigned int k = 0; k < (1 << (i - 1)); ++k) {
                r += f(a + (2*k + 1)*h);
            }
            R[i][0] = 0.5*R[i - 1][0] + h*r;
        }

        // Metodo de Romberg.
        for (unsigned int j = 1; j < m; ++j) {
            for (unsigned int i = j; i < m; ++i) {
                T h = (1 << (2*j)) - 1;
                R[i][j] = R[i][j - 1] + (R[i][j - 1] - R[i - 1][j - 1]) / h;
            }
        }
        return R[m - 1][m - 1];
    }


    /* Evalua una integral doble. */
    template <class Function, class T, class LowerLimit, class UpperLimit>
    T double_integrate(
        Function f,           // La funcion a integrar.
        T a,                  // Limite inferior de la primera variable.
        T b,                  // Limite superior de la primera variable.
        LowerLimit c,         // Limite inferior de la segunda variable.
        UpperLimit d,         // Limite superior de la segunda variable.
        unsigned int m = 256, // Tamaño de la particion sobre el primer eje.
        unsigned int n = 256  // Tamaño de la particion sobre el segundo eje.
    ) {
        // Fijamos la primera variable e integramos con respecto a la segunda.
        std::function<T(T)> inner_integral = [=](T x) {
            std::function<T(T)> g = [=](T y) {
                return f(x, y);
            };
            return integrate(g, c(x), d(x), n);
        };
        return integrate(inner_integral, a, b, m);
    }


    /* Evalua una integral triple. */
    template <
        class Function,
        class T,
        class LowerLimit1,
        class UpperLimit1,
        class LowerLimit2,
        class UpperLimit2
    >
    T triple_integrate(
        Function f,           // La funcion a integrar.
        T a,                  // Limite inferior de la primera variable.
        T b,                  // Limite superior de la primera variable.
        LowerLimit1 c,        // Limite inferior de la segunda variable.
        UpperLimit1 d,        // Limite superior de la segunda variable.
        LowerLimit2 g,        // Limite inferior de la tercera variable.
        UpperLimit2 h,        // Limite superior de la tercera variable.
        unsigned int l = 128, // Tamaño de la particion sobre el primer eje.
        unsigned int m = 128, // Tamaño de la particion sobre el segundo eje.
        unsigned int n = 128  // Tamaño de la particion sobre el tercer eje.
    ) {
        // Fijamos la primera y segunda variable e integramos con respecto a la
        // tercera.
        std::function<T(T, T)> inner_integral = [=](T x, T y) {
            std::function<T(T)> F = [=](T z) {
                return f(x, y, z);
            };
            return integrate(F, g(x, y), h(x, y), n);
        };
        return double_integrate(inner_integral, a, b, c, d, l, m);
    }


    /* Resuelve el problema de valor inicial:
       y'(x) = f(x, y(x)),      y(x0) = y0
    donde y puede ser una función escalar o una función vectorial. */
    template <
        class Function,
        class InputIterator,
        class InitialValue,
        class OutputIterator
    >
    void solve_ode(
        Function f,           // La funcion que define la ecuacion.
        InputIterator xfirst, // Dominio de la ecuacion.
        InputIterator xlast,
        InitialValue y0,      // Condicion inicial.
        OutputIterator yfirst // Solucion de la ecuacion.
    ) {
        // Valor inicial.
        using T = typename std::iterator_traits<InputIterator>::value_type;
        T x0 = *xfirst;
        ++xfirst;
        *yfirst = y0;
        ++yfirst;

        // Metodo de Runge-Kutta de orden 4.
        while (xfirst != xlast) {
            T h = *xfirst - x0;

            InitialValue K1 = h*f(x0,         y0         );
            InitialValue K2 = h*f(x0 + h/2.0, y0 + K1/2.0);
            InitialValue K3 = h*f(x0 + h/2.0, y0 + K2/2.0);
            InitialValue K4 = h*f(x0 + h,     y0 + K3    );
            *yfirst = y0 + (K1 + K4)/6.0 + (K2 + K3)/3.0;

            x0 = *xfirst;
            ++xfirst;
            y0 = *yfirst;
            ++yfirst;
        }
    }

}

#endif // INTEGRATE_H_INCLUDED
