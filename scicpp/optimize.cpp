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
        if (result.hess_inv.rows()*result.hess_inv.columns() != 0) {
            ostr << "hess_inv:\n" << result.hess_inv << "\n";
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
        Function fun, const numcpp::array<T> x0, Jacobian jac,
        T gtol, double ordnorm, size_t maxiter
    ) {
        OptimizeResult<T> result;
        result.x = x0;
        result.fun = fun(x0);
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
                fun, jac, result.x, pk, result.jac, result.fun,
                T(1e-4), T(0.1), T(1.0), 50
            );
            T alpha = step_selector.root;
            result.x += alpha*pk;
            result.fun = fun(result.x);
            numcpp::array<T> jac_old = result.jac;
            result.jac = jac(result.x);
            T beta = (result.jac).dot(result.jac - jac_old) /
                     (jac_old).dot(jac_old);
            pk = beta*pk - result.jac;
            result.nfev += step_selector.function_calls + 1;
            result.njev += step_selector.derivative_calls + 1;
        }
        if (!result.success) {
            result.status = "Maximum number of iterations has been exceeded.";
        }

        return result;
    }

    // Minimize a function using the Newton-CG algorithm.
    template <class T, class Function, class Jacobian, class Hessian>
    OptimizeResult<T> minimize_ncg(
        Function fun, const numcpp::array<T> &x0, Jacobian jac, Hessian hess,
        T gtol, double ordnorm, size_t maxiter
    ) {
        OptimizeResult<T> result;
        result.x = x0;
        result.fun = fun(x0);
        result.jac = jac(x0);
        result.hess = hess(x0);
        result.success = false;
        result.nfev = 1;
        result.njev = 1;
        result.nhev = 1;

        for (result.niter = 1; result.niter < maxiter; ++result.niter) {
            T error = norm(result.jac, ordnorm);
            if (error <= gtol) {
                result.success = true;
                result.status = "Optimization terminated successfully.";
                break;
            }

            numcpp::array<T> pk(result.x.size(), T(0));
            numcpp::array<T> r = result.jac;
            numcpp::array<T> d = -r;
            T tol = numcpp::min(T(0.5), numcpp::sqrt(error))*error;
            for (size_t i = 0; i < pk.size(); ++i) {
                numcpp::array<T> hess_d = (result.hess).dot(d);
                if (norm(r) <= tol || d.dot(hess_d) <= 0) {
                    if (i == 0) {
                        pk = -result.jac;
                    }
                    break;
                }
                T alpha = r.dot(r) / d.dot(hess_d);
                pk += alpha*d;
                numcpp::array<T> r_old = r;
                r += alpha*hess_d;
                T beta = r.dot(r) / r_old.dot(r_old);
                d = beta*d - r;
            }

            RootResults<T> step_selector = line_search(
                fun, jac, result.x, pk, result.jac, result.fun,
                T(1e-4), T(0.9), T(1.0), 50
            );
            T alpha = step_selector.root;
            result.x += alpha*pk;
            result.fun = fun(result.x);
            result.jac = jac(result.x);
            result.hess = hess(result.x);
            result.nfev += step_selector.function_calls + 1;
            result.njev += step_selector.derivative_calls + 1;
            ++result.nhev;
        }
        if (!result.success) {
            result.status = "Maximum number of iterations has been exceeded.";
        }

        return result;
    }

    // Minimize a function using the quasi-Newton method of Broyden, Fletcher,
    // Goldfarb, and Shanno (BFGS).
    template <class T, class Function, class Jacobian>
    OptimizeResult<T> minimize_bfgs(
        Function fun, const numcpp::array<T> &x0, Jacobian jac,
        const numcpp::matrix<T> &B0,
        T gtol, double ordnorm, size_t maxiter
    ) {
        OptimizeResult<T> result;
        result.x = x0;
        result.fun = fun(x0);
        result.jac = jac(x0);
        result.hess_inv = B0;
        result.success = false;
        result.nfev = 1;
        result.njev = 1;
        result.nhev = 0;

        for (result.niter = 1; result.niter < maxiter; ++result.niter) {
            if (norm(result.jac, ordnorm) <= gtol) {
                result.success = true;
                result.status = "Optimization terminated successfully.";
                break;
            }

            numcpp::array<T> pk = (result.hess_inv).dot(-result.jac);
            RootResults<T> step_selector = line_search(
                fun, jac, result.x, pk, result.jac, result.fun,
                T(1e-4), T(0.9), T(1.0), 50
            );
            T alpha = step_selector.root;
            numcpp::array<T> sk = alpha*pk;
            result.x += sk;
            result.fun = fun(result.x);
            numcpp::array<T> yk = result.jac;
            result.jac = jac(result.x);
            yk = result.jac - yk;

            numcpp::matrix<T> Bk = result.hess_inv;
            T skTyk = sk.dot(yk);
            T ykTBkyk = yk.dot(Bk).dot(yk);
            for (size_t i = 0; i < Bk.rows(); ++i) {
                for (size_t j = 0; j < Bk.columns(); ++j) {
                    result.hess_inv[i][j] = 0;
                    for (size_t k = 0; k < Bk.rows(); ++k) {
                        result.hess_inv[i][j] += Bk[i][k]*yk[k]*sk[j] +
                                                 sk[i]*yk[k]*Bk[k][j];
                    }
                    result.hess_inv[i][j] = Bk[i][j] +
                    ((skTyk + ykTBkyk)*sk[i]*sk[j]) / (skTyk*skTyk) -
                    result.hess_inv[i][j] / skTyk;
                }
            }

            result.nfev += step_selector.function_calls + 1;
            result.njev += step_selector.derivative_calls + 1;
        }
        if (!result.success) {
            result.status = "Maximum number of iterations has been exceeded.";
        }

        return result;
    }

    // Find alpha that satisfies Wolfe conditions.
    template <class T, class Function, class Jacobian>
    RootResults<T> line_search(
        Function fun, Jacobian jac,
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
        T a_hi = amax, phi_hi = fun(xk + a_hi*pk);

        bool stage1 = true;
        while (result.iterations < maxiter) {
            T a = (a_lo + a_hi)/2;
            T phi = fun(xk + a*pk);
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

    ////////////////////////////////////////////////////////////////////////////
    // Least-squares and curve fitting                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Solve a nonlinear least-squares problem using Levenberg-Marquardt
    // algorithm.
    template <class T, class Residual, class Jacobian>
    OptimizeResult<T> least_squares(
        Residual res, const numcpp::array<T> &x0, Jacobian jac,
        T ftol, T xtol, T gtol, size_t maxiter
    ) {
        OptimizeResult<T> result;
        result.x = x0;
        result.jac = res(x0);
        result.hess = jac(x0);
        result.fun = 0.5*(result.jac).dot(result.jac);
        result.success = false;
        result.nfev = 1;
        result.njev = 1;
        result.nhev = 1;

        T mu = 3e-3;
        for (result.niter = 1; result.niter < maxiter; ++result.niter) {
            size_t m = result.hess.rows(), n = result.hess.columns();
            numcpp::matrix<T> A(n, n, T(0));
            numcpp::array<T> b(n, T(0));
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    for (size_t k = 0; k < m; ++k) {
                        A[i][j] += result.hess[k][i] * result.hess[k][j];
                    }
                }
                A[i][i] *= 1 + mu;
                for (size_t k = 0; k < m; ++k) {
                    b[i] -= result.hess[k][i] * result.jac[k];
                }
            }
            if (norm(result.jac) <= gtol || norm(b) <= gtol) {
                result.success = true;
                result.status = "\"gtol\" termination condition is satisfied.";
                break;
            }

            mu /= 3;
            numcpp::array<T> pk;
            numcpp::array<T> x_old = result.x;
            T fun_old = result.fun;
            for (size_t ndamps = 0; ndamps < 20; ++ndamps) {
                pk = solve(A, b, "sym");
                result.x = x_old + pk;
                result.jac = res(result.x);
                result.fun = 0.5*(result.jac).dot(result.jac);
                ++result.njev;
                ++result.nfev;
                if (result.fun < fun_old) {
                    break;
                }
                else {
                    for (size_t i = 0; i < n; ++i) {
                        A[i][i] = (1 + 2*mu)*(A[i][i]/(1 + mu));
                    }
                    mu *= 2;
                }
            }
            result.hess = jac(result.x);
            ++result.nhev;
            if (result.fun <= fun_old && fun_old - result.fun < ftol) {
                result.success = true;
                result.status = "\"ftol\" termination condition is satisfied.";
                break;
            }
            if (norm(pk) <= xtol) {
                result.success = true;
                result.status = "\"xtol\" termination condition is satisfied.";
                break;
            }
        }
        if (!result.success) {
            result.status = "Maximum number of iterations has been exceeded.";
        }

        return result;
    }

    // Use non-linear least squares to fit a function f to data.
    template <class T, class Function, class Jacobian>
    OptimizeResult<T> curve_fit(
        Function f,
        const numcpp::array<T> &xdata, const numcpp::array<T> &ydata,
        const numcpp::array<T> &p0, Jacobian jac,
        T ftol, T xtol, T gtol, size_t maxiter
    ) {
        if (xdata.size() != ydata.size()) {
            std::ostringstream error;
            error << "x and y must have same dimension, but have shapes ("
                  << xdata.size() << ",) and (" << ydata.size() << ",)";
            throw std::invalid_argument(error.str());
        }
        OptimizeResult<T> result = least_squares(
            [&](const numcpp::array<T> &param) {
                numcpp::array<T> r(xdata.size());
                for (size_t i = 0; i < xdata.size(); ++i) {
                    r[i] = f(xdata[i], param) - ydata[i];
                }
                return r;
            },
            p0,
            [&](const numcpp::array<T> &param) {
                numcpp::matrix<T> J(xdata.size(), param.size());
                for (size_t i = 0; i < xdata.size(); ++i) {
                    numcpp::array<T> Ji = jac(xdata[i], param);
                    for (size_t j = 0; j < param.size(); ++j) {
                        J[i][j] = Ji[j];
                    }
                }
                return J;
            },
            ftol, xtol, gtol, maxiter
        );
        result.nfev *= xdata.size();
        result.njev *= xdata.size();
        result.nhev *= xdata.size();
        return result;
    }
}
