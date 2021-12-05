#ifndef OPTIMIZE_H_INCLUDED
#define OPTIMIZE_H_INCLUDED

#include "numcpp.h"
#include <utility>

namespace scicpp {

    ////////////////////////////////////////////////////////////////////////////
    // Root finding                                                           //
    ////////////////////////////////////////////////////////////////////////////

    // Class returned by the root finding methods.
    template <class T>
    class RootResults {
    public:
        T root;
        size_t iterations;
        size_t function_calls;
        size_t derivative_calls;
        bool converged;
        std::string status;
    };

    // Find a root of a function within an interval using bisection.
    template <class T, class Function = T(T)>
    RootResults<T> bisect(
        Function f, T a, T b, T tol = 1e-9, size_t maxiter = 100
    );

    // Find a root of a function using the Newton-Raphson method.
    template <class T, class Function = T(T), class Derivative = T(T)>
    RootResults<T> newton(
        Function f, T x0, Derivative df, T tol = 1e-9, size_t maxiter = 50
    );

    // Find a root of a function using the secant method.
    template <class T, class Function = T(T)>
    RootResults<T> secant(
        Function f, T x0, T x1, T tol = 1e-9, size_t maxiter = 50
    );

    // Find a root of a function using the Halley's method.
    template <
        class T, class Function = T(T),
        class Derivative = T(T), class Derivative2 = T(T)
    >
    RootResults<T> halley(
        Function f, T x0, Derivative df, Derivative2 df2,
        T tol = 1e-9, size_t maxiter = 50
    );

    ////////////////////////////////////////////////////////////////////////////
    // Local optimization                                                     //
    ////////////////////////////////////////////////////////////////////////////

    // Class returned by the optimization methods.
    template <class T>
    class OptimizeResult {
    public:
        T fun;
        numcpp::array<T> x;
        numcpp::array<T> jac;
        numcpp::matrix<T> hess, hess_inv;
        bool success;
        std::string status;
        size_t niter;
        size_t nfev, njev, nhev;
    };

    // Outputs results from optimizer.
    template <class T>
    std::ostream&
    operator<< (std::ostream &ostr, const OptimizeResult<T> &result);

    // No callback type.
    class no_callback {
    public:
        template <class T>
        bool operator()(const OptimizeResult<T> &state);
    };

    // Minimize a function using a nonlinear conjugate gradient algorithm.
    template <
        class T, class Function, class Jacobian,
        class Callable = no_callback
    >
    OptimizeResult<T> minimize_cg(
        Function fun, const numcpp::array<T> x0, Jacobian jac,
        T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000,
        Callable callback = no_callback()
    );

    // Minimize a function using the Newton-CG algorithm.
    template <
        class T, class Function, class Jacobian, class Hessian,
        class Callable = no_callback
    >
    OptimizeResult<T> minimize_ncg(
        Function fun, const numcpp::array<T> &x0, Jacobian jac, Hessian hess,
        T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000,
        Callable callback = no_callback()
    );

    // Minimize a function using the quasi-Newton method of Broyden, Fletcher,
    // Goldfarb, and Shanno (BFGS).
    template <
        class T, class Function, class Jacobian,
        class Callable = no_callback
    >
    OptimizeResult<T> minimize_bfgs(
        Function fun, const numcpp::array<T> &x0, Jacobian jac,
        const numcpp::matrix<T> &B0,
        T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000,
        Callable callback = no_callback()
    );

    // Find alpha that satisfies Wolfe conditions.
    template <class T, class Function, class Jacobian>
    RootResults<T> line_search(
        Function fun, Jacobian jac,
        const numcpp::array<T> &xk, const numcpp::array<T> &pk,
        const numcpp::array<T> &gfk, T fk,
        T c1 = 0.0001, T c2 = 0.9, T amax = 1.0, size_t maxiter = 20
    );

    ////////////////////////////////////////////////////////////////////////////
    // Least-squares and curve fitting                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Solve a nonlinear least-squares problem using Levenberg-Marquardt
    // algorithm.
    template <
        class T, class Residual, class Jacobian,
        class Callable = no_callback
    >
    OptimizeResult<T> least_squares(
        Residual res, const numcpp::array<T> &x0, Jacobian jac,
        T ftol = 1e-8, T xtol = 1e-8, T gtol = 1e-8, size_t maxiter = 1000,
        Callable callback = no_callback()
    );

    // Use nonlinear least squares to fit a function f to data.
    template <
        class T, class Function, class Jacobian,
        class Callable = no_callback
    >
    OptimizeResult<T> curve_fit(
        Function f,
        const numcpp::array<T> &xdata, const numcpp::array<T> &ydata,
        const numcpp::array<T> &p0, Jacobian jac,
        T ftol = 1e-8, T xtol = 1e-8, T gtol = 1e-8, size_t maxiter = 1000,
        Callable callback = no_callback()
    );
}

#include "optimize.cpp"

#endif // OPTIMIZE_H_INCLUDED
