#ifndef OPTIMIZE_H_INCLUDED
#define OPTIMIZE_H_INCLUDED

#include "numcpp.h"

namespace scicpp {

    // Class returned by the root finding methods.
    template <class T>
    class RootResults {
    public:
        T root;
        size_t iterations;
        size_t function_calls;
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
}

#include "optimize.cpp"

#endif // OPTIMIZE_H_INCLUDED
