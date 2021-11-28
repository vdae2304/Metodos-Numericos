#ifndef OPTIMIZE_H_INCLUDED
#define OPTIMIZE_H_INCLUDED

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace scicpp {

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
        Function f,
        T a,
        T b,
        T tol = 1e-9,
        size_t maxiter = 100
    ) {
        T fa = f(a), fb = f(b);
        if ((fa < 0 && fb < 0) || (fa > 0 && fb > 0)) {
            throw std::invalid_argument(
                "f(a) and f(b) must have different signs"
            );
        }

        RootResults<T> output;
        output.iterations = 0;
        output.function_calls = 2;
        output.converged = false;
        std::ostringstream status;

        while (output.iterations < maxiter) {
            output.root = a + (b - a) / 2;
            ++output.iterations;
            ++output.function_calls;

            T fm = f(output.root);
            if (std::abs(fm) <= tol) {
                output.converged = true;
                status << "Converged after " << output.iterations
                       << " iterations, value is " << output.root;
                break;
            }
            else if ((fa < 0 && fm > 0) || (fa > 0 && fm < 0)) {
                b = output.root;
                fb = fm;
            }
            else {
                a = output.root;
                fa = fm;
            }
        }

        if (!output.converged) {
            status << "Failed to converge after " << output.iterations
                   << " iterations, value is " << output.root;
        }
        output.status = status.str();

        return output;
    }

    // Find a root of a function using the Newton-Raphson method.
    template <class T, class Function = T(T), class Derivative = T(T)>
    RootResults<T> newton(
        Function f,
        T x0,
        Derivative df,
        T tol = 1e-9,
        size_t maxiter = 50
    ) {
        RootResults<T> output;
        output.root = x0;
        output.iterations = 0;
        output.function_calls = 0;
        output.converged = false;
        std::ostringstream status;

        while (output.iterations < maxiter) {
            ++output.iterations;
            ++output.function_calls;

            T fx = f(output.root);
            if (std::abs(fx) <= tol) {
                output.converged = true;
                status << "Converged after " << output.iterations
                       << " iterations, value is " << output.root;
                break;
            }
            T dfx = df(output.root);
            if (std::abs(dfx) <= tol) {
                status << "Derivative was zero. Failed to converge after "
                       << output.iterations << " iterations, value is "
                       << output.root;
                break;
            }
            output.root -= fx / dfx;
        }

        if (!output.converged) {
            status << "Failed to converge after " << output.iterations
                   << " iterations, value is " << output.root;
        }
        output.status = status.str();

        return output;
    }

    // Find a root of a function using the secant method.
    template <class T, class Function = T(T)>
    RootResults<T> secant(
        Function f,
        T x0,
        T x1,
        T tol = 1e-9,
        size_t maxiter = 50
    ) {
        RootResults<T> output;
        output.root = x1;
        output.iterations = 0;
        output.function_calls = 2;
        output.converged = false;
        std::ostringstream status;

        T fx0 = f(x0), fx1 = f(x1);
        while (output.iterations < maxiter) {
            ++output.iterations;
            ++output.function_calls;

            if (std::abs(fx0) <= tol) {
                output.converged = true;
                status << "Converged after " << output.iterations
                       << " iterations, value is " << output.root;
                break;
            }
            if (std::abs(fx1 - fx0) <= tol) {
                status << "Derivative was zero. Failed to converge after "
                       << output.iterations << " iterations, value is "
                       << output.root;
                break;
            }
            output.root = x1 - (x1 - x0) / (fx1 - fx0) * fx1;
            x0 = x1;
            x1 = output.root;
            fx0 = fx1;
            fx1 = f(x1);
        }

        if (!output.converged) {
            status << "Failed to converge after " << output.iterations
                   << " iterations, value is " << output.root;
        }
        output.status = status.str();

        return output;
    }

    // Find a root of a function using the Halley's method.
    template <
        class T,
        class Function = T(T),
        class Derivative = T(T),
        class Derivative2 = T(T)
    >
    RootResults<T> halley(
        Function f,
        T x0,
        Derivative df,
        Derivative2 df2,
        T tol = 1e-9,
        size_t maxiter = 50
    ) {
        RootResults<T> output;
        output.root = x0;
        output.iterations = 0;
        output.function_calls = 0;
        output.converged = false;
        std::ostringstream status;

        while (output.iterations < maxiter) {
            ++output.iterations;
            ++output.function_calls;

            T fx = f(output.root);
            if (std::abs(fx) <= tol) {
                output.converged = true;
                status << "Converged after " << output.iterations
                       << " iterations, value is " << output.root;
                break;
            }
            T dfx = df(output.root);
            T df2x = df2(output.root);
            if (std::abs(2.0*dfx*dfx - fx*df2x) <= tol) {
                status << "Derivative was zero. Failed to converge after "
                       << output.iterations << " iterations, value is "
                       << output.root;
                break;
            }
            output.root -= (2.0*fx*dfx) / (2.0*dfx*dfx - fx*df2x);
        }

        if (!output.converged) {
            status << "Failed to converge after " << output.iterations
                   << " iterations, value is " << output.root;
        }
        output.status = status.str();

        return output;
    }
}

#endif // OPTIMIZE_H_INCLUDED
