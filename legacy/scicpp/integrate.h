#ifndef INTEGRATE_H_INCLUDED
#define INTEGRATE_H_INCLUDED

#include "numcpp.h"

namespace scicpp {

    ////////////////////////////////////////////////////////////////////////////
    // Integration                                                            //
    ////////////////////////////////////////////////////////////////////////////

    // Compute a definite integral.
    template <class T, class Function = T(T)>
    T quad(
        Function f, T a, T b,
        bool show = false,
        T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
    );

    // Compute a definite integral using fixed-order Gaussian quadrature.
    template <class T, class Function = T(T)>
    T fixed_quad(Function f, T a, T b, size_t n);

    // Compute the sample points and weights for Gauss-Legendre quadrature.
    template <class T>
    void leggauss(
        size_t n, numcpp::array<T> &points, numcpp::array<T> &weights
    );

    // Compute a definite integral using Romberg method.
    template <class T, class Function = T(T)>
    T romberg(
        Function f, T a, T b,
        bool show = false,
        T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 10
    );

    // Return weights for Newton-Cotes integration.
    template <class T>
    void newton_cotes(size_t n, numcpp::array<T> &weights);

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
    );

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
    );

    ////////////////////////////////////////////////////////////////////////////
    // Ordinary differential equations                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Class for ODE results.
    template <class T>
    class OdeResult {
    public:
        numcpp::array<T> t;
        numcpp::matrix<T> y;
        numcpp::matrix<T> f;
        size_t nfev;

        numcpp::array<T> operator()(T t);

        numcpp::matrix<T> operator()(const numcpp::array<T> &t);
    };

    // Base class for ODE solvers.
    template <class T, class Function>
    class OdeSolver {
    public:
        size_t n;
        Function f;
        T t;
        numcpp::array<T> y;
        T step_size, max_step;
        T tol, rtol;
        size_t nfev;

        OdeSolver(
            Function f, T t0, const numcpp::array<T> &y0,
            T first_step = 1e-3, T max_step = numcpp::inf,
            T tol = 1e-6, T rtol = 1e-3
        );

        virtual void step();

    protected:
        // Use only for Explicit Runge-Kutta methods.
        numcpp::matrix<T> tableau;

        void ExplicitRKstep();
    };

    // Explicit Runge-Kutta method of order 3(2).
    // This uses the Bogacki-Shampine pair of formulas.
    // The error is controlled assuming accuracy of the second-order method,
    // but steps are taken using the third-order accurate formula.
    template <class T, class Function>
    class RK23 : public OdeSolver<T, Function> {
    public:
        RK23(
            Function f, T t0, const numcpp::array<T> &y0,
            T first_step = 1e-3, T max_step = numcpp::inf,
            T tol = 1e-6, T rtol = 1e-3
        );

        void step();
    };

    // Explicit Runge-Kutta method of order 5(4).
    // This uses the Dormand-Prince pair of formulas.
    // The error is controlled assuming accuracy of the fourth-order method,
    // but steps are taken using the fifth-order accurate formula.
    template <class T, class Function>
    class RK45 : public OdeSolver<T, Function> {
    public:
        RK45(
            Function f, T t0, const numcpp::array<T> &y0,
            T first_step = 1e-3, T max_step = numcpp::inf,
            T tol = 1e-6, T rtol = 1e-3
        );

        void step();
    };

    // Solve an initial value problem for a system of ordinary differential
    // equations.
    //   dy/dt = f(t, y)
    //   y(t0) = y0
    template <class T, class Function>
    OdeResult<T> solve_ivp(
        Function f, T t0, T tf, const numcpp::array<T> &y0,
        std::string method = "RK45",
        T first_step = 1e-3, T max_step = numcpp::inf,
        T tol = 1e-6, T rtol = 1e-3
    );
}

#include "integrate.cpp"

#endif // INTEGRATE_H_INCLUDED
