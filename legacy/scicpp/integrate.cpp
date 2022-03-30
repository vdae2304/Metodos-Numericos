#include <algorithm>
#include <iostream>
#include <iomanip>
#include "linalg.h"
#include <sstream>
#include <stdexcept>
#include <vector>

namespace scicpp {
    ////////////////////////////////////////////////////////////////////////////
    // Integration                                                            //
    ////////////////////////////////////////////////////////////////////////////

    // Compute a definite integral.
    template <class T, class Function>
    T quad(Function f, T a, T b, bool show, T tol, T rtol, size_t maxiter) {
        T value = (b - a) * f((a + b) / 2);
        size_t neval = 1;

        for (size_t n = 2; n <= maxiter; ++n) {
            T integral = fixed_quad(f, a, b, n);
            neval += n;
            if (numcpp::isclose(value, integral, tol, rtol)) {
                if (show) {
                    std::cout << "The final result is " << integral << " after "
                              << neval << " function evaluations\nwith error "
                              << numcpp::abs(integral - value) << "\n";
                }
                return integral;
            }
            value = integral;
        }
        if (show) {
            std::cout << "Failed to converge after " << neval
                      << " function evaluations, value is " << value << "\n";
        }

        return value;
    }

    // Compute a definite integral using fixed-order Gaussian quadrature.
    template <class T, class Function>
    T fixed_quad(Function f, T a, T b, size_t n) {
        T integral = 0;
        bool isfinite = numcpp::isfinite(a) && numcpp::isfinite(b);
        if (!isfinite) {
            a = numcpp::atan(a);
            b = numcpp::atan(b);
        }

        numcpp::array<T> points, weights;
        leggauss(n, points, weights);
        for (size_t i = 0; i < n; ++i) {
            T x = (b - a)/2 * points[i] + (a + b)/2;
            T y = isfinite ? f(x)
                           : f(numcpp::tan(x))/numcpp::square(numcpp::cos(x));
            integral += weights[i] * y;
        }
        integral = (b - a)/2 * integral;

        return integral;
    }

    // Compute the sample points and weights for Gauss-Legendre quadrature.
    template <class T>
    void leggauss(
        size_t n, numcpp::array<T> &points, numcpp::array<T> &weights
    ) {
        numcpp::matrix<T> A(n, n, T(0));
        numcpp::matrix<T> V;
        numcpp::array<T> D;
        for (size_t i = 1; i < n; ++i) {
            A[i - 1][i] = i / numcpp::sqrt(T(4*i*i - 1));
            A[i][i - 1] = A[i - 1][i];
        }
        eigen_symm(A, V, D);

        numcpp::array<size_t> indices = numcpp::argsort(D);
        points.resize(n);
        weights.resize(n);
        for (size_t i = 0; i < n; ++i) {
            points[i] = D[indices[i]];
            weights[i] = 2*V[0][indices[i]]*V[0][indices[i]];
        }
    }

    // Compute a definite integral using Romberg method.
    template <class T, class Function>
    T romberg(Function f, T a, T b, bool show, T tol, T rtol, size_t maxiter) {
        numcpp::matrix<T> R(maxiter, maxiter);
        R[0][0] = (b - a) * (f(a) + f(b)) / 2;

        size_t iter = 0, neval = 2;
        bool converged = false;
        for (iter = 1; iter < maxiter; ++iter) {
            size_t n = 1 << iter;
            T h = (b - a) / n;
            R[iter][0] = 0;
            for (size_t k = 1; k < n; k += 2) {
                R[iter][0] += f(a + k*h);
                ++neval;
            }
            R[iter][0] = R[iter - 1][0]/2 + h*R[iter][0];

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
                steps[i][1] = 1. / steps[i][0];
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
                T error = numcpp::abs(R[iter-1][iter-1] - R[iter-2][iter-2]);
                std::cout << "The final result is " << R[iter - 1][iter - 1]
                          << " after " << neval << " function evaluations\n"
                          << "with error " << error << "\n";
            }
            else {
                std::cout << "Failed to converge after " << neval
                          << " function evaluations, value is "
                          << R[iter - 1][iter - 1] << "\n";
            }
        }

        return R[iter - 1][iter - 1];
    }

    // Return weights for Newton-Cotes integration.
    template <class T>
    void newton_cotes(size_t n, numcpp::array<T> &weights) {
        numcpp::matrix<T> A(n + 1, n + 1);
        numcpp::array<T> b(n + 1);
        for (size_t i = 0; i <= n; ++i) {
            A[0][i] = 1;
            b[i] = n / (i + 1.);
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
    template <class T, class Function, class LowerLimit, class UpperLimit>
    T dblquad(
        Function f, T ax, T bx, LowerLimit ay, UpperLimit by,
        bool show, T tol, T rtol, size_t maxiter
    ) {
        T value;
        size_t neval = 0;

        for (size_t n = 2; n <= maxiter; ++n) {
            numcpp::array<T> points, weights;
            leggauss(n, points, weights);

            T integral = 0;
            for (size_t i = 0; i < n; ++i) {
                T x = (bx - ax)/2 * points[i] + (ax + bx)/2;
                T ay_ = ay(x), by_ = by(x);
                T integral_y_axis = 0;
                for (size_t j = 0; j < n; ++j) {
                    T y = (by_ - ay_)/2 * points[j] + (ay_ + by_)/2;
                    integral_y_axis += weights[j] * f(x, y);
                }
                integral_y_axis = (by_ - ay_)/2 * integral_y_axis;
                integral += weights[i] * integral_y_axis;
            }
            integral = (bx - ax)/2 * integral;
            neval += n*n;

            if (n > 2 && numcpp::isclose(value, integral, tol, rtol)) {
                if (show) {
                    std::cout << "The final result is " << integral << " after "
                              << neval << " function evaluations\nwith error "
                              << numcpp::abs(integral - value) << "\n";
                }
                return integral;
            }
            value = integral;
        }
        if (show) {
            std::cout << "Failed to converge after " << neval
                      << " function evaluations, value is " << value << "\n";
        }

        return value;
    }

    // Compute a triple (definite) integral.
    template <
        class T, class Function,
        class LowerLimit1, class UpperLimit1,
        class LowerLimit2, class UpperLimit2
    >
    T tplquad(
        Function f,
        T ax, T bx,
        LowerLimit1 ay, UpperLimit1 by,
        LowerLimit2 az, UpperLimit2 bz,
        bool show, T tol, T rtol, size_t maxiter
    ) {
        T value;
        size_t neval = 0;

        for (size_t n = 2; n <= maxiter; ++n) {
            numcpp::array<T> points, weights;
            leggauss(n, points, weights);

            T integral = 0;
            for (size_t i = 0; i < n; ++i) {
                T x = (bx - ax)/2 * points[i] + (ax + bx)/2;
                T ay_ = ay(x), by_ = by(x);
                T integral_y_axis = 0;
                for (size_t j = 0; j < n; ++j) {
                    T y = (by_ - ay_)/2 * points[j] + (ay_ + by_)/2;
                    T az_ = az(x, y), bz_ = bz(x, y);
                    T integral_z_axis = 0;
                    for (size_t k = 0; k < n; ++k) {
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
            neval += n*n*n;

            if (n > 2 && numcpp::isclose(value, integral, tol, rtol)) {
                if (show) {
                    std::cout << "The final result is " << integral << " after "
                              << neval << " function evaluations\nwith error "
                              << numcpp::abs(integral - value) << "\n";
                }
                return integral;
            }
            value = integral;
        }
        if (show) {
            std::cout << "Failed to converge after " << neval
                      << " function evaluations, value is " << value << "\n";
        }
        return value;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Ordinary differential equations                                        //
    ////////////////////////////////////////////////////////////////////////////

    template <class T>
    numcpp::array<T> OdeResult<T>::operator()(T t) {
        size_t m = this->y.rows(), n = this->y.columns();
        if (t < this->t[0] || t > this->t[m - 1]) {
            throw std::domain_error("t is outside the solution domain.");
        }
        size_t i = std::upper_bound(this->t.data(), this->t.data() + m, t) -
                   this->t.data();

        T a = this->t[i - 1], b = this->t[i];
        t = (t - this->t[i - 1]) / (b - a);
        numcpp::array<T> y(n, T(0));
        for (size_t k = 0; k < n; ++k) {
            y[k] += ((2*t - 3)*t*t + 1) * this->y[i - 1][k];
            y[k] += (((t - 2)*t + 1)*t) * (b - a)*this->f[i - 1][k];
            y[k] += ((-2*t + 3)*t*t) * this->y[i][k];
            y[k] += ((t - 1)*t*t) * (b - a)*this->f[i][k];
        }

        return y;
    }

    template <class T>
    numcpp::matrix<T> OdeResult<T>::operator()(const numcpp::array<T> &t) {
        size_t m = this->y.rows(), n = this->y.columns();
        numcpp::matrix<T> y(t.size(), n, T(0));
        for (size_t i = 0; i < t.size(); ++i) {
            numcpp::array<T> yi = (*this)(t[i]);
            for (size_t j = 0; j < n; ++j) {
                y[i][j] = yi[j];
            }
        }
        return y;
    }

    // Base class for ODE solvers.
    template <class T, class Function>
    OdeSolver<T, Function>::OdeSolver(
        Function f, T t0, const numcpp::array<T> &y0,
        T first_step, T max_step, T tol, T rtol
    ) {
        this->n = y0.size();
        this->f = f;
        this->t = t0;
        this->y = y0;
        this->step_size = first_step/2;
        this->max_step = max_step;
        this->tol = tol;
        this->rtol = rtol;
        this->nfev = 0;
    }

    template <class T, class Function>
    void OdeSolver<T, Function>::step() {
        ;
    }

    template <class T, class Function>
    void OdeSolver<T, Function>::ExplicitRKstep() {
        size_t stages = this->tableau.columns();
        numcpp::array<T> k[stages];
        numcpp::array<T> y_low, y_high;

        T h = numcpp::min(2*this->step_size, this->max_step);
        k[0] = this->f(this->t, this->y);
        ++this->nfev;
        do {
            for (size_t i = 1; i < stages; ++i) {
                T t = this->t + h*this->tableau[i][0];
                numcpp::array<T> y = this->y;
                for (size_t j = 1; j <= i; ++j) {
                    y += h*this->tableau[i][j]*k[j - 1];
                }
                k[i] = this->f(t, y);
                ++this->nfev;
            }

            y_low = this->y;
            y_high = this->y;
            for (size_t i = 0; i < stages - 1; ++i) {
                y_low += h*this->tableau[stages][i]*k[i];
            }
            for (size_t i = 0; i < stages; ++i) {
                y_high += h*this->tableau[stages + 1][i]*k[i];
            }

            this->step_size = h;
            h /= 2;
        } while (!numcpp::allclose(y_low, y_high, this->tol, this->rtol));

        this->t += this->step_size;
        this->y = y_high;
    }

    // Explicit Runge-Kutta method of order 3(2).
    // This uses the Bogacki-Shampine pair of formulas.
    // The error is controlled assuming accuracy of the second-order method,
    // but steps are taken using the third-order accurate formula.
    template <class T, class Function>
    RK23<T, Function>::RK23(
        Function f, T t0, const numcpp::array<T> &y0,
        T first_step, T max_step, T tol, T rtol
    ) : OdeSolver<T, Function>(f, t0, y0, first_step, max_step, tol, rtol) {
        this->tableau = {{    0.,    0.,    0.,    0.},
                         { 1./2., 1./2.,    0.,    0.},
                         { 3./4.,    0., 3./4.,    0.},
                         {    1., 2./9., 1./3., 4./9.},
                         { 2./9., 1./3., 4./9.,    0.},
                         {7./24., 1./4., 1./3., 1./8.}};
    }

    template <class T, class Function>
    void RK23<T, Function>::step() {
        this->ExplicitRKstep();
    }

    // Explicit Runge-Kutta method of order 5(4).
    // This uses the Dormand-Prince pair of formulas.
    // The error is controlled assuming accuracy of the fourth-order method,
    // but steps are taken using the fifth-order accurate formula.
    template <class T, class Function>
    RK45<T, Function>::RK45(
        Function f, T t0, const numcpp::array<T> &y0,
        T first_step, T max_step, T tol, T rtol
    ) : OdeSolver<T, Function>(f, t0, y0, first_step, max_step, tol, rtol) {
        this->tableau =
        {{         0.,           0.,            0.,          0.,               0.,            0.,      0.},
         {      1./5.,        1./5.,            0.,          0.,               0.,            0.,      0.},
         {     3./10.,       3./40.,        9./40.,          0.,               0.,            0.,      0.},
         {      4./5.,      44./45.,      -56./15.,      32./9.,               0.,            0.,      0.},
         {      8./9., 19372./6561., -25360./2187., 64448./6561.,      -212./729.,            0.,      0.},
         {         1.,  9017./3168.,     -355./33., 46732./5247.,        49./176., -5103./18656.,      0.},
         {         1.,     35./384.,            0.,   500./1113.,       125./192.,  -2187./6784., 11./84.},
         {    35./384.,          0.,    500./1113.,    125./192.,    -2187./6784.,       11./84.,      0.},
         {5179./57600.,          0.,  7571./16695.,    393./640., -92097./339200.,    187./2100.,  1./40.}};
    }

    template <class T, class Function>
    void RK45<T, Function>::step() {
        this->ExplicitRKstep();
    }

    template <class T, class Function>
    OdeResult<T> solve_ivp(
        Function f, T t0, T tf, const numcpp::array<T> &y0,
        std::string method, T first_step, T max_step, T tol, T rtol
    ) {
        OdeResult<T> result;
        numcpp::array<T> dy0 = f(t0, y0);
        std::vector<T> t(1, t0);
        std::vector<T> y(y0.data(), y0.data() + y0.size());
        std::vector<T> dy(dy0.data(), dy0.data() + dy0.size());
        result.nfev = 1;

        if (method == "RK23") {
            RK23<T, Function> solver(f, t0, y0, first_step, max_step, tol, rtol);
            while (solver.t < tf) {
                solver.max_step = numcpp::min(max_step, tf - solver.t);
                solver.step();
                t.push_back(solver.t);
                dy0 = f(solver.t, solver.y);
                ++result.nfev;
                for (size_t i = 0; i < solver.n; ++i) {
                    y.push_back(solver.y[i]);
                    dy.push_back(dy0[i]);
                }
            }
            result.nfev += solver.nfev;
        }
        else if (method == "RK45") {
            RK45<T, Function> solver(f, t0, y0, first_step, max_step, tol, rtol);
            while (solver.t < tf) {
                solver.max_step = numcpp::min(max_step, tf - solver.t);
                solver.step();
                t.push_back(solver.t);
                dy0 = f(solver.t, solver.y);
                ++result.nfev;
                for (size_t i = 0; i < solver.n; ++i) {
                    y.push_back(solver.y[i]);
                    dy.push_back(dy0[i]);
                }
            }
            result.nfev += solver.nfev;
        }
        else {
            throw std::invalid_argument(
                "\"method\" must be one of \"RK23\" or \"RK45\""
            );
        }

        result.t = numcpp::array<T>(t.begin(), t.end());
        result.y = numcpp::matrix<T>(t.size(), y0.size(), y.begin());
        result.f = numcpp::matrix<T>(t.size(), y0.size(), dy.begin());
        return result;
    }
}
