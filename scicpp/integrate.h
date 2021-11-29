#ifndef INTEGRATE_H_INCLUDED
#define INTEGRATE_H_INCLUDED

#include <iostream>
#include <iomanip>
#include "linalg.h"
#include "numcpp.h"
#include <sstream>
#include <stdexcept>

namespace scicpp {

    ////////////////////////////////////////////////////////////////////////////
    // Integration                                                            //
    ////////////////////////////////////////////////////////////////////////////

    // Compute the sample points and weights for Gauss-Legendre quadrature.
    template <class T>
    void leggauss(
        size_t n, numcpp::array<T> &points, numcpp::array<T> &weights
    ) {
        numcpp::matrix<T> A(n, n, T(0));
        numcpp::matrix<T> V;
        numcpp::array<T> D;
        for (size_t i = 1; i < n; ++i) {
            A[i - 1][i] = i / numcpp::sqrt(T(4.0*i*i - 1.0));
            A[i][i - 1] = A[i - 1][i];
        }
        eigen_symm(A, V, D);

        numcpp::array<size_t> indices = numcpp::argsort(D);
        points.resize(n);
        weights.resize(n);
        for (size_t i = 0; i < n; ++i) {
            points[i] = D[indices[i]];
            weights[i] = 2.0*V[0][indices[i]]*V[0][indices[i]];
        }
    }

    // Compute a definite integral using fixed-order Gaussian quadrature.
    template <class T, class Function = T(T)>
    T fixed_quad(Function f, T a, T b, size_t n) {
        T integral = 0.0;
        numcpp::array<T> points, weights;
        leggauss(n, points, weights);
        for (size_t i = 0; i < n; ++i) {
            integral += weights[i] * f((b - a)/2 * points[i] + (a + b)/2);
        }
        integral = (b - a)/2 * integral;
        return integral;
    }

    // Compute a definite integral.
    template <class T, class Function = T(T)>
    T quad(
        Function f, T a, T b,
        bool show = false,
        T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
    ) {
        T value = (b - a) * f((a + b) / 2);
        for (size_t iter = 2; iter <= maxiter; ++iter) {
            T new_value = fixed_quad(f, a, b, iter);
            if (numcpp::isclose(new_value, value, tol, rtol)) {
                if (show) {
                    std::cout << "Converged after " << iter
                              << " iterations, value is " << new_value << "\n";
                }
                return new_value;
            }
            value = new_value;
        }
        if (show) {
            std::cout << "Failed to converge after " << maxiter
                      << " iterations, value is " << value << "\n";
        }
        return value;
    }

    // Compute a definite integral using Romberg method.
    template <class T, class Function = T(T)>
    T romberg(
        Function f, T a, T b,
        bool show = false,
        T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 10
    ) {
        numcpp::matrix<T> R(maxiter, maxiter);
        size_t iter = 0;
        bool converged = false;

        R[0][0] = (b - a) * (f(a) + f(b)) / 2;
        for (iter = 1; iter < maxiter; ++iter) {
            size_t n = 1 << iter;
            T h = (b - a) / n;
            R[iter][0] = 0.0;
            for (size_t k = 1; k < n; k += 2) {
                R[iter][0] += f(a + k*h);
            }
            R[iter][0] = 0.5*R[iter - 1][0] + h*R[iter][0];

            for (size_t i = 1; i <= iter; ++i) {
                T q = 1 << (2*i);
                R[iter][i] = (q*R[iter][i - 1] - R[iter - 1][i - 1]) / (q - 1);
            }
            if (
                numcpp::isclose(R[iter - 1][iter - 1], R[iter][iter], tol, rtol)
            ) {
                ++iter;
                converged = true;
                break;
            }
        }

        if (show) {
            numcpp::matrix<T> steps(maxiter, 2);
            for (size_t i = 0; i < iter; ++i) {
                steps[i][0] = 1 << i;
                steps[i][1] = 1.0 / steps[i][0];
            }
            numcpp::matrix<T> info = numcpp::column_stack(steps, R);

            numcpp::array<size_t> width(info.columns(), 0);
            width[0] = 5;
            width[1] = 8;
            for (size_t i = 0; i < iter; ++i) {
                for (size_t j = 0; j <= i + 2; ++j) {
                    std::ostringstream buffer;
                    buffer.setf(std::cout.flags());
                    buffer << std::setprecision(std::cout.precision())
                           << info[i][j];
                    if (width[j] < buffer.str().size()) {
                        width[j] = buffer.str().size();
                    }
                }
            }

            std::cout << std::setw(width[0]) << "Steps" << " "
                      << std::setw(width[1]) << "StepSize" << " Results\n";
            for (size_t i = 0; i < iter; ++i) {
                for (size_t j = 0; j <= i + 2; ++j) {
                    std::cout << std::setw(width[j]) << info[i][j] << " ";
                }
                std::cout << "\n";
            }
            if (converged) {
                std::cout << "Converged after " << iter << " iterations, ";
            }
            else {
                std::cout << "Failed to converged after "
                          << maxiter << "iterations, ";
            }
            std::cout << "value is " << R[iter - 1][iter - 1] << "\n";
        }

        return R[iter - 1][iter - 1];
    }

    // Return weights for Newton-Cotes integration.
    template <class T>
    void newton_cotes(size_t n, numcpp::array<T> &weights) {
        numcpp::matrix<T> A(n + 1, n + 1);
        numcpp::array<T> b(n + 1);
        for (size_t i = 0; i <= n; ++i) {
            A[0][i] = 1.0;
            b[i] = n / (i + 1.0);
        }
        for (size_t i = 1; i <= n; ++i) {
            for (size_t j = 0; j <= n; ++j) {
                A[i][j] = A[i - 1][j] * j / n;
            }
        }
        weights = scicpp::solve(A, b);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Multiple integral                                                      //
    ////////////////////////////////////////////////////////////////////////////

    // Compute a double (definite) integral.
    template <
        class T, class Function = T(T, T),
        class LowerLimit = T(T), class UpperLimit = T(T)
    >
    T dblquad(
        Function f, T ax, T bx, LowerLimit ay, UpperLimit by,
        bool show = false,
        T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
    ) {
        T value;
        for (size_t iter = 2; iter <= maxiter; ++iter) {
            numcpp::array<T> points, weights;
            leggauss(iter, points, weights);

            T integral = 0.0;
            for (size_t i = 0; i < iter; ++i) {
                T integral_y_axis = 0.0;
                T x = (bx - ax)/2 * points[i] + (ax + bx)/2;
                T ay_ = ay(x), by_ = by(x);
                for (size_t j = 0; j < iter; ++j) {
                    T y = (by_ - ay_)/2 * points[j] + (ay_ + by_)/2;
                    integral_y_axis += weights[j] * f(x, y);
                }
                integral_y_axis = (by_ - ay_)/2 * integral_y_axis;
                integral += weights[i] * integral_y_axis;
            }
            integral = (bx - ax)/2 * integral;

            if (iter > 2 && numcpp::isclose(integral, value, tol, rtol)) {
                if (show) {
                    std::cout << "Converged after " << iter
                              << " iterations, value is " << integral << "\n";
                }
                return integral;
            }
            value = integral;
        }
        if (show) {
            std::cout << "Failed to converge after " << maxiter
                      << " iterations, value is " << value << "\n";
        }
        return value;
    }

    // Compute a triple (definite) integral.
    template <
        class T, class Function = T(T, T, T),
        class LowerLimit1 = T(T), class UpperLimit1 = T(T),
        class LowerLimit2 = T(T, T), class UpperLimit2 = T(T, T)
    >
    T tplquad(
        Function f,
        T ax, T bx,
        LowerLimit1 ay, UpperLimit1 by,
        LowerLimit2 az, UpperLimit2 bz,
        bool show = false,
        T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
    ) {
        T value;
        for (size_t iter = 2; iter <= maxiter; ++iter) {
            numcpp::array<T> points, weights;
            leggauss(iter, points, weights);

            T integral = 0.0;
            for (size_t i = 0; i < iter; ++i) {
                T integral_y_axis = 0.0;
                T x = (bx - ax)/2 * points[i] + (ax + bx)/2;
                T ay_ = ay(x), by_ = by(x);
                for (size_t j = 0; j < iter; ++j) {
                    T integral_z_axis = 0.0;
                    T y = (by_ - ay_)/2 * points[j] + (ay_ + by_)/2;
                    T az_ = az(x, y), bz_ = bz(x, y);
                    for (size_t k = 0; k < iter; ++k) {
                        T z = (bz_ - az_)/2 * points[k] + (az_ + bz_)/2;
                        integral_z_axis += weights[k] * f(x, y, z);
                    }
                    integral_z_axis = (bz_ - az_)/2 * integral_z_axis;
                    integral_y_axis += weights[j] * integral_z_axis;
                }
                integral_y_axis = (by_ - ay_)/2 * integral_y_axis;
                integral += weights[i] * integral_y_axis;
            }
            integral = (bx - ax)/2 * integral;

            if (iter > 2 && numcpp::isclose(integral, value, tol, rtol)) {
                if (show) {
                    std::cout << "Converged after " << iter
                              << " iterations, value is " << integral << "\n";
                }
                return integral;
            }
            value = integral;
        }
        if (show) {
            std::cout << "Failed to converge after " << maxiter
                      << " iterations, value is " << value << "\n";
        }
        return value;
    }
}

#endif // INTEGRATE_H_INCLUDED
