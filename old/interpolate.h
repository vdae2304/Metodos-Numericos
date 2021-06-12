#ifndef INTERPOLATE_H_INCLUDED
#define INTERPOLATE_H_INCLUDED

#include <cmath>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include "matrix.h"
#include "linalg.h"

namespace num_met {

    /* Genera una muestra de numeros equi-espaciados sobre un intervalo. */
    template<class T, class OutputIterator>
    void linspace(
        T a,                  // Extremo izquierdo del intervalo.
        T b,                  // Extremo derecho del intervalo.
        OutputIterator first, // Iterador correspondiente a la muestra.
        OutputIterator last,
        bool endpoint = true  // Si es true, b sera el ultimo valor en la muestra.
    ) {
        unsigned int n = std::distance(first, last) - endpoint;
        unsigned int i = 0;
        while (first != last) {
            *first = a + i*(b - a) / n;
            ++i;
            ++first;
        }
    }


    /* Evalua un polinomio en un punto. */
    template <class T, class InputIterator>
    T polynomial(
        T x,                 // El punto a evaluar.
        InputIterator first, // Iterador correspondiente a los coeficientes del
        InputIterator last   // polinomio.
    ) {
        // Esquema de Horner.
        T val = T();
        while (first != last) {
            val = x * val + *first;
            ++first;
        }
        return val;
    }


    /* Calcula la matriz de Vandermonde. */
    template <
        class InputIterator,
        class T = typename std::iterator_traits<InputIterator>::value_type
    >
    matrix<T> vandermonde(
        unsigned int m,     // Numero de renglones.
        unsigned int n,     // Numero de columnas.
        InputIterator first // Iterador correspondiente a los puntos.
    ) {
        matrix<T> V(m, n);
        for (unsigned int i = 0; i < m; ++i) {
            V[i][0] = T(1);
            for (unsigned int j = 1; j < n; ++j) {
                V[i][j] = *first * V[i][j - 1];
            }
            ++first;
        }
        return V;
    }


    /* Interpola un polinomio sobre una lista de puntos. */
    template <class InputIterator1, class InputIterator2, class OutputIterator>
    void poly_interpolate(
        unsigned int npts,     // Numero de puntos a interpolar.
        InputIterator1 xfirst, // Iterador correspondiente a las coordenadas x.
        InputIterator2 yfirst, // Iterador correspondiente a las coordenadas y.
        OutputIterator coeff   // Iterador correspondiente a los coeficientes.
    ) {
        // Construye el sistema de ecuaciones.
        using T = typename std::iterator_traits<OutputIterator>::value_type;
        matrix<T> V = vandermonde(npts, npts, xfirst);
        matrix<T> b(npts, 1, yfirst);
        matrix<T> x = solve(V, b);
        // Guarda las coeficientes del polinomio.
        for (unsigned int i = x.rows(); i > 0; --i) {
            *coeff = x[i - 1][0];
            ++coeff;
        }
    }


    /* Ajusta un polinomio de determinado grado sobre una lista de puntos. */
    template <class InputIterator1, class InputIterator2, class OutputIterator>
    void poly_fit(
        unsigned int n,        // Grado del polinomio.
        unsigned int npts,     // Numero de puntos a interpolar.
        InputIterator1 xfirst, // Iterador correspondiente a las coordenadas x.
        InputIterator2 yfirst, // Iterador correspondiente a las coordenadas y.
        OutputIterator coeff   // Iterador correspondiente a los coeficientes.
    ) {
        // Construye el sistema de ecuaciones.
        using T = typename std::iterator_traits<OutputIterator>::value_type;
        matrix<T> V = vandermonde(npts, n + 1, xfirst);
        matrix<T> b(npts, 1, yfirst);
        matrix<T> x = linear_lstsq(V, b);
        // Guarda las coeficientes del polinomio.
        for (unsigned int i = x.rows(); i > 0; --i) {
            *coeff = x[i - 1][0];
            ++coeff;
        }
    }


    /* Evalua un polinomio a pedazos en un punto. */
    template <class T>
    T piecewise_polynomial(
        T x,                    // El punto a evaluar.
        const matrix<T> &coeff, // Los coeficientes del polinomio a pedazos.
        unsigned int hint = -1  // Indice del subintervalo donde esta el punto.
    ) {
        if (x < coeff[0][0] || x > coeff[coeff.rows() - 1][0]) {
            throw std::invalid_argument("Point outside the spline domain.");
        }
        // Busca el subintervalo del spline que lo contiene.
        unsigned int i = hint;
        if (i >= coeff.rows() - 1 || x < coeff[i][0] || x > coeff[i + 1][0]) {
            unsigned int lo = 0, hi = coeff.rows() - 1;
            while (lo < hi) {
                unsigned int mid = (lo + hi + 1) / 2;
                if (x > coeff[mid][0]) {
                    lo = mid;
                }
                else {
                    hi = mid - 1;
                }
            }
            i = lo;
        }
        // Evalua.
        T val = T();
        for (unsigned int j = coeff.columns() - 1; j > 0; --j) {
            val = val * (x - coeff[i][0]) + coeff[i][j];
        }
        return val;
    }


    /* Interpola un spline natural cubico sobre una lista de puntos. */
    template <
        class InputIterator1,
        class InputIterator2,
        class T = typename std::iterator_traits<InputIterator1>::value_type
    >
    matrix<T> spline_interpolate(
        unsigned int npts,     // Numero de puntos a interpolar.
        InputIterator1 xfirst, // Iterador correspondiente a las coordenadas x.
        InputIterator2 yfirst  // Iterador correspondiente a las coordenadas y.
    ) {
        matrix<T> coeff(npts, 5, T());

        // Guarda los puntos de interpolacion.
        for (unsigned int i = 0; i < npts; ++i) {
            coeff[i][0] = *xfirst;
            coeff[i][1] = *yfirst;
            ++xfirst;
            ++yfirst;
        }

        // Distancias en x y en y.
        T *hx = new T[npts - 1];
        T *hy = new T[npts - 1];
        for (unsigned int i = 0; i < npts - 1; ++i) {
            hx[i] = coeff[i + 1][0] - coeff[i][0];
            hy[i] = coeff[i + 1][1] - coeff[i][1];
        }

        // Define un sistema de ecuaciones tridiagonal.
        T *mu = new T[npts - 1];
        T *z = new T[npts - 1];
        mu[0] = z[0] = T();
        for (unsigned int i = 1; i < npts - 1; ++i) {
        	T l = 2.0*(hx[i] + hx[i - 1]) - hx[i - 1]*mu[i - 1];
        	T alpha = 3.0*hy[i] / hx[i] - 3.0*hy[i - 1] / hx[i - 1];
            mu[i] = hx[i] / l;
            z[i] = (alpha - hx[i - 1]*z[i - 1]) / l;
        }

        // Lo resuelve.
        for (int i = npts - 2; i >= 0; --i) {
            coeff[i][3] = z[i] - mu[i]*coeff[i + 1][3];
            coeff[i][2] = hy[i] / hx[i] -
                hx[i]*(coeff[i + 1][3] + 2.0*coeff[i][3]) / 3.0;
            coeff[i][4] = (coeff[i + 1][3] - coeff[i][3]) / (3.0*hx[i]);
        }

        delete[] hx;
        delete[] hy;
        delete[] mu;
        delete[] z;
        return coeff;
    }

}

#endif // INTERPOLATE_H_INCLUDED
