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

    template <class T>
    std::ostream&
    operator<< (std::ostream &ostr, const OptimizeResult<T> &result);

    // Minimize a function using a nonlinear conjugate gradient algorithm.
    template <class T, class Function, class Jacobian>
    OptimizeResult<T> minimize_cg(
        Function f, const numcpp::array<T> x0, Jacobian jac,
        T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
    );

    // Minimize a function using the Newton-CG algorithm.
    template <class T, class Function, class Jacobian, class Hessian>
    OptimizeResult<T> minimize_ncg(
        Function f, const numcpp::array<T> &x0, Jacobian jac, Hessian hess,
        T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
    );

    // Minimize a function using the quasi-Newton method of Broyden, Fletcher,
    // Goldfarb, and Shanno (BFGS).
    template <class T, class Function, class Jacobian>
    OptimizeResult<T> minimize_bfgs(
        Function f, const numcpp::array<T> &x0, Jacobian jac,
        const numcpp::matrix<T> &B0,
        T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
    );

    // Find alpha that satisfies Wolfe conditions.
    template <class T, class Function, class Jacobian>
    RootResults<T> line_search(
        Function f, Jacobian jac,
        const numcpp::array<T> &xk, const numcpp::array<T> &pk,
        const numcpp::array<T> &gfk, T fk,
        T c1 = 0.0001, T c2 = 0.9, T amax = 1.0, size_t maxiter = 20
    );
}

#include "optimize.cpp"

#endif // OPTIMIZE_H_INCLUDED
