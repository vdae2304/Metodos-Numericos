#include "linalg.h"
#include <sstream>
#include <stdexcept>

namespace scicpp {

    ////////////////////////////////////////////////////////////////////////////
    // Root finding                                                           //
    ////////////////////////////////////////////////////////////////////////////

    template <class T>
    std::ostream&
    operator<< (std::ostream &ostr, const RootResults<T> &result) {
        ostr << "root: " << result.root << "\n";
        ostr << "niter: " << result.iterations << "\n";
        ostr << "nfev: " << result.function_calls << "\n";
        ostr << "njev: " << result.derivative_calls << "\n";
        ostr << "converged: " << result.converged << "\n";
        ostr << "status: " << result.status << "\n";
        return ostr;
    }

    // Find a root of a function within an interval using bisection.
    template <class T, class Function>
    RootResults<T> bisect(Function f, T a, T b, T tol, size_t maxiter) {
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
            if (numcpp::abs(fm) <= tol) {
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
    template <class T, class Function, class Derivative>
    RootResults<T> newton(
        Function f, T x0, Derivative df, T tol, size_t maxiter
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
            if (numcpp::abs(fx) <= tol) {
                output.converged = true;
                status << "Converged after " << output.iterations
                       << " iterations, value is " << output.root;
                break;
            }
            T dfx = df(output.root);
            if (numcpp::abs(dfx) <= tol) {
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
    template <class T, class Function>
    RootResults<T> secant(Function f, T x0, T x1, T tol, size_t maxiter) {
        RootResults<T> output;
        output.root = x1;
        output.iterations = 0;
        output.function_calls = 1;
        output.converged = false;
        std::ostringstream status;

        T fx0 = f(x0);
        while (output.iterations < maxiter) {
            ++output.iterations;
            ++output.function_calls;

            T fx1 = f(x1);
            if (numcpp::abs(fx1) <= tol) {
                output.converged = true;
                status << "Converged after " << output.iterations
                       << " iterations, value is " << output.root;
                break;
            }
            if (numcpp::abs(fx1 - fx0) <= tol) {
                status << "Derivative was zero. Failed to converge after "
                       << output.iterations << " iterations, value is "
                       << output.root;
                break;
            }

            output.root = x1 - (x1 - x0) / (fx1 - fx0) * fx1;
            x0 = x1;
            x1 = output.root;
            fx0 = fx1;
        }
        if (!output.converged) {
            status << "Failed to converge after " << output.iterations
                   << " iterations, value is " << output.root;
        }
        output.status = status.str();

        return output;
    }

    // Find a root of a function using the Halley's method.
    template <class T, class Function, class Derivative, class Derivative2>
    RootResults<T> halley(
        Function f, T x0, Derivative df, Derivative2 df2, T tol, size_t maxiter
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
            if (numcpp::abs(fx) <= tol) {
                output.converged = true;
                status << "Converged after " << output.iterations
                       << " iterations, value is " << output.root;
                break;
            }
            T dfx = df(output.root);
            T df2x = df2(output.root);
            if (numcpp::abs(2*dfx*dfx - fx*df2x) <= tol) {
                status << "Derivative was zero. Failed to converge after "
                       << output.iterations << " iterations, value is "
                       << output.root;
                break;
            }
            output.root -= (2*fx*dfx) / (2*dfx*dfx - fx*df2x);
        }
        if (!output.converged) {
            status << "Failed to converge after " << output.iterations
                   << " iterations, value is " << output.root;
        }
        output.status = status.str();

        return output;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Local optimization                                                     //
    ////////////////////////////////////////////////////////////////////////////

    template <class T>
    std::ostream&
    operator<< (std::ostream &ostr, const OptimizeResult<T> &result) {
        ostr << "fun: " << result.fun << "\n";
        ostr << "x: " << result.x << "\n";
        if (result.jac.size() != 0) {
            ostr << "jac: " << result.jac << "\n";
        }
        if (result.hess.rows()*result.hess.columns() != 0) {
            ostr << "hess:\n" << result.hess << "\n";
        }
        ostr << "success: " << result.success << "\n";
        ostr << "status: " << result.status << "\n";
        ostr << "niter: " << result.niter << "\n";
        ostr << "nfev: " << result.nfev << "\n";
        if (result.jac.size() != 0) {
            ostr << "njev: " << result.njev << "\n";
        }
        if (result.hess.rows()*result.hess.columns() != 0) {
            ostr << "nhev: " << result.nhev << "\n";
        }
        return ostr;
    }

    // Minimize a function using a nonlinear conjugate gradient algorithm.
    template <class T, class Function, class Jacobian>
    OptimizeResult<T> minimize_cg(
        Function f, const numcpp::array<T> x0, Jacobian jac,
        T gtol, double ordnorm, size_t maxiter
    ) {
        OptimizeResult<T> result;
        result.x = x0;
        result.fun = f(x0);
        result.jac = jac(x0);
        result.success = false;
        result.nfev = 1;
        result.njev = 1;
        result.nhev = 0;

        numcpp::array<T> pk = -result.jac;
        for (result.niter = 1; result.niter < maxiter; ++result.niter) {
            if (norm(result.jac, ordnorm) <= gtol) {
                result.success = true;
                result.status = "Optimization terminated successfully.";
                break;
            }

            RootResults<T> step_selector = line_search(
                f, jac, result.x, pk, result.jac, result.fun, 1e-4, 0.1, 1.0, 50
            );
            T alpha = step_selector.root;
            numcpp::array<T> x = result.x + alpha*pk;
            numcpp::array<T> g = jac(x);
            T beta = numcpp::dot(g, g - result.jac) /
                     numcpp::dot(result.jac, result.jac);
            pk = beta*pk - g;

            result.x = x;
            result.fun = f(result.x);
            result.jac = g;
            result.nfev += step_selector.function_calls + 1;
            result.njev += step_selector.derivative_calls + 1;
        }
        if (!result.success) {
            result.status = "Maximum number of iterations has been exceeded.";
        }

        return result;
    }

    template <class T, class Function, class Jacobian>
    RootResults<T> line_search(
        Function f, Jacobian jac,
        const numcpp::array<T> &xk, const numcpp::array<T> &pk,
        const numcpp::array<T> &gfk, T fk,
        T c1, T c2, T amax, size_t maxiter
    ) {
        RootResults<T> result;
        result.iterations = 0;
        result.function_calls = 1;
        result.derivative_calls = 0;
        result.converged = false;
        std::ostringstream status;

        T mk = gfk.dot(pk);
        T a_lo = 0, phi_lo = fk;
        T a_hi = amax, phi_hi = f(xk + a_hi*pk);

        bool stage1 = true;
        while (result.iterations < maxiter) {
            T a = (a_lo + a_hi)/2;
            T phi = f(xk + a*pk);
            result.root = a;
            ++result.iterations;
            ++result.function_calls;
            if (phi > fk + c1*a*mk || phi >= phi_lo) {
                if (stage1) {
                    stage1 = false;
                }
                else {
                    a_hi = a;
                    phi_hi = phi;
                }
            }
            else {
                T m = jac(xk + a*pk).dot(pk);
                ++result.derivative_calls;
                if (numcpp::abs(m) <= -c2*mk) {
                    result.converged = true;
                    status << "Converged after " << result.iterations
                           << " iterations, value is " << result.root;
                    break;
                }
                if ((stage1 && m >= 0) || (!stage1 && m*(a_hi - a_lo) >= 0)) {
                    a_hi = a_lo;
                    phi_hi = phi_lo;
                }
                a_lo = a;
                phi_lo = phi;
            }
        }
        if (!result.converged) {
            status << "Failed to converge after " << result.iterations
                   << " iterations, value is " << result.root;
        }
        result.status = status.str();

        return result;
    }
}
