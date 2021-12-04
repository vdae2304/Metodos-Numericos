# Optimization and root finding

## Content

- [Root finding (Scalar functions)](#Root-finding-Scalar-functions)
- [Local (multivariate) optimization](#Local-multivariate-optimization)
- [Least squares](#Least-squares)

## Root finding (Scalar functions)

### `RootResults`

Class returned by the root finding methods.
```cpp
template <class T>
class RootResults {
public:
    T root;
    size_t iterations;
    size_t function_calls;
    bool converged;
    std::string status;
};
```

#### Attributes

- `root`: Estimated root location.
- `iterations`: Number of iterations needed to find the root.
- `function_calls`: Number of times the function was called.
- `converged`: `true` if the routine converged.
- `status`: Description of the cause of termination.

### `bisect`

Find root of a function within an interval using bisection. 
```cpp
template <class T, class Function = T(T)>
RootResults<T> bisect(
    Function f, T a, T b, T tol = 1e-9, size_t maxiter = 100
);
```

#### Arguments

- `f`: The function whose root is wanted. Must be continuous and `f(a)` and 
`f(b)` must have opposite signs.
- `a`: One end of the bracketing interval *[a, b]*.
- `b`: The other end of the bracketing interval *[a, b]*.
- `tol`: The desired tolerance.
- `maxiter`: Maximum number of iterations.

#### Example 

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

int main() {
    scicpp::RootResults<double> output = scicpp::bisect(f, 0.0, 2.0);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 30 iterations, value is 1.25992
```

### `newton`

Find a root of a function using the Newton-Raphson method.
```cpp
template <class T, class Function = T(T), class Derivative = T(T)>
RootResults<T> newton(
    Function f, T x0, Derivative df, T tol = 1e-9, size_t maxiter = 50
);
```

#### Arguments:

- `f`: The function whose root is wanted.
- `x0`: An initial estimate of the root that should be somewhere near the 
actual root.
- `df`: The derivative of the function.
- `tol`: The desired tolerance.
- `maxiter`: Maximum number of iterations.

#### Example

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

double df(double x) {
    return 3*x*x;
}

int main() {
    scicpp::RootResults<double> output = scicpp::newton(f, 3.0, df);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 7 iterations, value is 1.25992
```

### `secant`

Find a root of a function using the secant method.
```cpp
template <class T, class Function = T(T)>
RootResults<T> secant(
    Function f, T x0, T x1, T tol = 1e-9, size_t maxiter = 50
);
```

#### Arguments:

- `f`: The function whose root is wanted.
- `x0`: An initial estimate of the root that should be somewhere near the 
actual root.
- `x1`: Another estimate of the root that should be somewhere near the actual 
root.
- `tol`: The desired tolerance.
- `maxiter`: Maximum number of iterations.

#### Example

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

int main() {
    scicpp::RootResults<double> output = scicpp::secant(f, 3.0, 2.9);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 9 iterations, value is 1.25992
```

### `halley`

Find a root of a function using the Halley's method.
```cpp
template <
    class T, class Function = T(T),
    class Derivative = T(T), class Derivative2 = T(T)
>
RootResults<T> halley(
    Function f, T x0, Derivative df, Derivative2 df2,
    T tol = 1e-9, size_t maxiter = 50
);
```

#### Arguments:

- `f`: The function whose root is wanted.
- `x0`: An initial estimate of the root that should be somewhere near the 
actual root.
- `df`: The first derivative of the function.
- `df2`: The second derivative of the function.
- `tol`: The desired tolerance.
- `maxiter`: Maximum number of iterations.

#### Example

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

double df(double x) {
    return 3*x*x;
}

double df2(double x) {
    return 6*x;
}

int main() {
    scicpp::RootResults<double> output = scicpp::halley(f, 3.0, df, df2);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 5 iterations, value is 1.25992
```

## Local (multivariate) optimization

### `OptimizeResult`

Class returned by the optimization methods.
```cpp
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
```

#### Attributes 

- `fun`: Value of the objective function.
- `x`: The solution of the optimization.
- `jac`: Value of the objective function's gradient.
- `hess`: Value of the objective function's Hessian.
- `hess_inv`: Inverse of the objective function’s Hessian; may be an 
approximation.
- `success`: Whether or not the optimizer exited successfully.
- `status`: Description of the cause of termination.
- `niter`: Number of iterations performed by the optimizer.
- `nfev`, `njev`, `nhev`: Number of evaluations of the objective function and 
of its gradient and Hessian.

### `minimize_cg`

Minimize a function using a nonlinear conjugate gradient algorithm. This 
implementation uses Polak and Ribiere formula.
```cpp
template <class T, class Function, class Jacobian>
OptimizeResult<T> minimize_cg(
    Function fun, const numcpp::array<T> x0, Jacobian jac,
    T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
);
```

#### Arguments

- `fun`: Objective function to be minimized.
- `x0`: An initial estimate for the optimal value of *x*.
- `jac`: A function that returns the gradient vector of *f* at *x*.
- `gtol`: Stop when the norm of the gradient is less than `gtol`.
- `ordnorm`: Order to use for the norm of the gradient. 
- `maxiter`: Maximum number of iterations to perform.

#### Example

Find a local minimum of the Rosenbrock function. The Rosenbrock function is 
defined by

![$f(\mathbf{x}) = \sum_{i=1}^{n-1} 100(x_{i+1}-x_i^2)^2 + (1-x_i)^2$](https://render.githubusercontent.com/render/math?math=f(%5Cmathbf%7Bx%7D)%20%3D%20%5Csum_%7Bi%3D1%7D%5E%7Bn-1%7D%20100(x_%7Bi%2B1%7D-x_i%5E2)%5E2%20%2B%20(1-x_i)%5E2)

It has a global minimum at ![$\mathbf{x} = (1, 1, \ldots, 1)$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bx%7D%20%3D%20(1%2C%201%2C%20%5Cldots%2C%201)).
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Rosenbrock function.
double rosen(const np::array<double> &x) {
    double f = 0;
    for (int i = 0; i < x.size() - 1; ++i) {
        f += 100*(x[i + 1] - x[i]*x[i])*(x[i + 1] - x[i]*x[i]) +
             (1 - x[i])*(1 - x[i]);
    }
    return f;
}

// Derivative (gradient) of Rosenbrock function.
np::array<double> rosen_der(const np::array<double> &x) {
    np::array<double> jac = np::zeros<double>(x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        jac[i] += -400*x[i]*(x[i + 1] - x[i]*x[i]) - 2*(1 - x[i]);
        jac[i + 1] += 200*(x[i + 1] - x[i]*x[i]);
    }
    return jac;
}

int main() {
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    scicpp::OptimizeResult<double> result;
    result = scicpp::minimize_cg(rosen, x0, rosen_der);
    cout << result;
    return 0;
}
```

```
[Out] fun: 2.3138263e-11
      x: [0.99999896, 0.99999791, 0.99999582, 0.99999159]
      jac: [9.3998262e-07, -2.4901198e-06, 7.1508592e-06, -8.5579434e-06]
      success: true
      status: Optimization terminated successfully.
      niter: 95
      nfev: 1228
      njev: 337
```

### `minimize_ncg`

Minimize a function using the Newton-CG algorithm (also known as the truncated 
Newton method). It uses a conjugate gradient method to compute the search 
direction.
```cpp
template <class T, class Function, class Jacobian, class Hessian>
OptimizeResult<T> minimize_ncg(
    Function fun, const numcpp::array<T> &x0, Jacobian jac, Hessian hess,
    T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
);
```

#### Arguments

- `fun`: Objective function to be minimized.
- `x0`: An initial estimate for the optimal value of *x*.
- `jac`: A function that returns the gradient vector of *f* at *x*.
- `hess`: A function that returns the Hessian matrix of *f* at *x*.
- `gtol`: Stop when the norm of the gradient is less than `gtol`.
- `ordnorm`: Order to use for the norm of the gradient. 
- `maxiter`: Maximum number of iterations to perform.

#### Example

Find a local minimum of the Rosenbrock function.
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Rosenbrock function.
double rosen(const np::array<double> &x) {
    double f = 0;
    for (int i = 0; i < x.size() - 1; ++i) {
        f += 100*(x[i + 1] - x[i]*x[i])*(x[i + 1] - x[i]*x[i]) +
             (1 - x[i])*(1 - x[i]);
    }
    return f;
}

// Derivative (gradient) of Rosenbrock function.
np::array<double> rosen_der(const np::array<double> &x) {
    np::array<double> jac = np::zeros<double>(x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        jac[i] += -400*x[i]*(x[i + 1] - x[i]*x[i]) - 2*(1 - x[i]);
        jac[i + 1] += 200*(x[i + 1] - x[i]*x[i]);
    }
    return jac;
}

// Hessian of Rosenbrock function.
np::matrix<double> rosen_hess(const np::array<double> &x) {
    np::matrix<double> hess = np::zeros<double>(x.size(), x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        hess[i][i] += -400*x[i + 1] + 1200*x[i]*x[i] + 2;
        hess[i][i + 1] += -400*x[i];
        hess[i + 1][i + 1] += 200;
        hess[i + 1][i] += -400*x[i];
    }
    return hess;
}

int main() {
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    scicpp::OptimizeResult<double> result;
    result = scicpp::minimize_ncg(rosen, x0, rosen_der, rosen_hess);
    cout << result;
    return 0;
}
```

```
[Out] fun: 1.37688e-12
      x: [0.99999975, 0.99999951, 0.999999, 0.99999798]
      jac: [7.7789427e-07, 1.4612809e-06, 7.4658088e-06, -5.4988463e-06]
      hess:
      [[801.99961, -399.9999,         0,         0]
       [-399.9999, 1001.9992, -399.9998,         0]
       [        0, -399.9998, 1001.9984, -399.9996]
       [        0,         0, -399.9996,       200]]
      success: true
      status: Optimization terminated successfully.
      niter: 89
      nfev: 290
      njev: 186
      nhev: 89
```

### `minimize_bfgs`

Minimize a function using the quasi-Newton method of Broyden, Fletcher, 
Goldfarb, and Shanno (BFGS).
```cpp
template <class T, class Function, class Jacobian>
OptimizeResult<T> minimize_bfgs(
    Function fun, const numcpp::array<T> &x0, Jacobian jac,
    const numcpp::matrix<T> &B0,
    T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
);
```

#### Arguments

- `fun`: Objective function to be minimized.
- `x0`: An initial estimate for the optimal value of *x*.
- `jac`: A function that returns the gradient vector of *f* at *x*.
- `B0`: An initial estimate for the inverse of the Hessian matrix of *f* at 
`x0`.
- `gtol`: Stop when the norm of the gradient is less than `gtol`.
- `ordnorm`: Order to use for the norm of the gradient. 
- `maxiter`: Maximum number of iterations to perform.


#### Example

Find a local minimum of the Rosenbrock function.
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Rosenbrock function.
double rosen(const np::array<double> &x) {
    double f = 0;
    for (int i = 0; i < x.size() - 1; ++i) {
        f += 100*(x[i + 1] - x[i]*x[i])*(x[i + 1] - x[i]*x[i]) +
             (1 - x[i])*(1 - x[i]);
    }
    return f;
}

// Derivative (gradient) of Rosenbrock function.
np::array<double> rosen_der(const np::array<double> &x) {
    np::array<double> jac = np::zeros<double>(x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        jac[i] += -400*x[i]*(x[i + 1] - x[i]*x[i]) - 2*(1 - x[i]);
        jac[i + 1] += 200*(x[i + 1] - x[i]*x[i]);
    }
    return jac;
}

int main() {
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    np::matrix<double> B0 = np::eye<double>(4, 4);
    scicpp::OptimizeResult<double> result;
    result = scicpp::minimize_bfgs(rosen, x0, rosen_der, B0);
    cout << result;
    return 0;
}
```

```
[Out] fun: 2.75892e-13
      x: [0.9999999, 0.9999998, 0.99999959, 0.99999915]
      jac: [4.5691454e-07, 1.7022435e-06, 6.768782e-06, -4.4084408e-06]
      hess_inv:
      [[0.026480784, 0.050239632, 0.099123274, 0.19728892]
       [0.050239632,  0.10058458,  0.19838138, 0.39491722]
       [0.099123274,  0.19838138,  0.39714857,  0.7905933]
       [ 0.19728892,  0.39491722,   0.7905933,  1.5796873]]
      success: true
      status: Optimization terminated successfully.
      niter: 65
      nfev: 230
      njev: 132
```

### `line_search`

Given a starting point ![$\mathbf{x}_k$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bx%7D_k) and a search direction ![$\mathbf{p}_k$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bp%7D_k), 
find a step length ![$\alpha$](https://render.githubusercontent.com/render/math?math=%5Calpha) that satisfies the Wolfe conditions:

i) ![$f(\mathbf{x}_k + \alpha\mathbf{p}_k) \leq f(\mathbf{x}_k) + c_1\alpha\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k)$](https://render.githubusercontent.com/render/math?math=f(%5Cmathbf%7Bx%7D_k%20%2B%20%5Calpha%5Cmathbf%7Bp%7D_k)%20%5Cleq%20f(%5Cmathbf%7Bx%7D_k)%20%2B%20c_1%5Calpha%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k))

ii) ![$-\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k + \alpha\mathbf{p}_k) \leq -c_2\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k)$](https://render.githubusercontent.com/render/math?math=-%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k%20%2B%20%5Calpha%5Cmathbf%7Bp%7D_k)%20%5Cleq%20-c_2%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k))

 ```cpp
template <class T, class Function, class Jacobian>
RootResults<T> line_search(
    Function fun, Jacobian jac,
    const numcpp::array<T> &xk, const numcpp::array<T> &pk,
    const numcpp::array<T> &gfk, T fk,
    T c1 = 0.0001, T c2 = 0.9, T amax = 1.0, size_t maxiter = 20
);
 ```

#### Arguments

- `fun`: Objective function.
- `jac`: Objective function gradient.
- `xk`: Starting point.
- `pk`: Search direction.
- `gfk`: Gradient value for *x = xk*.
- `fk`: Function value for *x = xk*.
- `c1`: Parameter for Armijo condition rule.
- `c2`: Parameter for curvature condition rule.
- `amax`: Maximum step size.
- `maxiter`: Maximum number of iterations.

#### Example

 ```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Rosenbrock function.
double rosen(const np::array<double> &x) {
    double f = 0;
    for (int i = 0; i < x.size() - 1; ++i) {
        f += 100*(x[i + 1] - x[i]*x[i])*(x[i + 1] - x[i]*x[i]) +
             (1 - x[i])*(1 - x[i]);
    }
    return f;
}

// Derivative (gradient) of Rosenbrock function.
np::array<double> rosen_der(const np::array<double> &x) {
    np::array<double> jac = np::zeros<double>(x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        jac[i] += -400*x[i]*(x[i + 1] - x[i]*x[i]) - 2*(1 - x[i]);
        jac[i + 1] += 200*(x[i + 1] - x[i]*x[i]);
    }
    return jac;
}

int main() {
    // Move in the opposite direction of the gradient.
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    double f0 = rosen(x0);
    np::array<double> gf0 = rosen_der(x0);
    np::array<double> p0 = -gf0;

    // Line search method.
    double c1 = 0.0001, c2 = 0.9;
    scicpp::RootResults<double> result;
    result = scicpp::line_search(rosen, rosen_der, x0, p0, gf0, f0, c1, c2);

    // Check for Wolfe conditions.
    double alpha = result.root;
    bool cond1 = rosen(x0 + alpha*p0) <= f0 + c1*alpha*p0.dot(gf0);
    bool cond2 = -p0.dot(rosen_der(x0 + alpha*p0)) <= -c2*p0.dot(gf0);
    cout << "alpha: " << alpha << "\n";
    cout << boolalpha << "i) " << cond1 << "\nii) " << cond2 << "\n";

    return 0;
}
 ```

 ```
 [Out] alpha: 0.00195312
       i) true
       ii) true
 ```

 ## Least squares

 ### `least_squares`

 Solve a nonlinear least-squares problem using Levenberg-Marquardt algorithm.

 Given the residuals ![$\mathbf{r}(\mathbf{x})$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Br%7D(%5Cmathbf%7Bx%7D)) 
 (an *m*-dimensional function of *n* variables), finds a local minimum of the
 cost function

 ![$f(x) = \frac{1}{2}\sum_{i=1}^{m} r_i(\mathbf{x})^2$](https://render.githubusercontent.com/render/math?math=f(x)%20%3D%20%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D1%7D%5E%7Bm%7D%20r_i(%5Cmathbf%7Bx%7D)%5E2) 
```cpp
template <class T, class Residual, class Jacobian>
OptimizeResult<T> least_squares(
    Residual res, const numcpp::array<T> &x0, Jacobian jac,
    T ftol = 1e-8, T xtol = 1e-8, T gtol = 1e-8, size_t maxiter = 1000
);
```

#### Arguments

- `res`: Function wich computes the vector of residuals.
- `x0`: Initial guess on independent variables.
- `jac`: Method of computing the Jacobian matrix (an *m* by *n* matrix, where 
element *(i, j)* is the partial derivative of ![$r_i$](https://render.githubusercontent.com/render/math?math=r_i) 
with respect to  ![$x_j$](https://render.githubusercontent.com/render/math?math=x_j)).
- `ftol`: Tolerance for termination by the change of the cost function.
- `xtol`: Tolerance for termination by the change of the independent variables.
- `gtol`: Tolerance for termination by the norm of the gradient.
- `maxiter`: Maximum number of iterations to perform.

#### Example

Find a local minimum of the Rosenbrock function. The Rosenbrock function can be 
reformulated as a least squares problem. In this formulation, we don't need to 
write down the cost function as it is defined implicitly by the residuals.
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Rosenbrock function residuals.
np::array<double> rosen_res(const np::array<double> &x) {
    np::array<double> r(2*(x.size() - 1));
    for (int i = 0; i < x.size() - 1; ++i) {
        r[2*i] = 10*(x[i + 1] - x[i]*x[i]);
        r[2*i + 1] = 1 - x[i];
    }
    return r;
}

// Jacobian matrix of Rosenbrock residuals.
np::matrix<double> rosen_jac(const np::array<double> &x) {
    np::matrix<double> J = np::zeros<double>(2*(x.size() - 1), x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        J[2*i][i] = -20*x[i];
        J[2*i][i + 1] = 10;
        J[2*i + 1][i] = -1;
    }
    return J;
}

int main() {
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    scicpp::OptimizeResult<double> result;
    result = scicpp::least_squares(rosen_res, x0, rosen_jac);
    cout << result;
    return 0;
}
```

```
[Out] fun: 9.56782e-17
      x: [1, 0.99999999, 0.99999999, 0.99999998]
      jac: [-7.940204e-11, 3.0103722e-09, -1.667888e-10, 6.0286846e-09, -3.6381786e-10, 1.2074048e-08]
      hess:
      [[-20,  10,   0,  0]
       [ -1,   0,   0,  0]
       [  0, -20,  10,  0]
       [  0,  -1,   0,  0]
       [  0,   0, -20, 10]
       [  0,   0,  -1,  0]]
      success: true
      status: "ftol" termination condition is satisfied.
      niter: 18
      nfev: 37
      njev: 37
      nhev: 19
```

### `curve_fit`

Use nonlinear least squares to fit a function *f* to data.
```cpp
template <class T, class Function, class Jacobian>
OptimizeResult<T> curve_fit(
    Function f,
    const numcpp::array<T> &xdata, const numcpp::array<T> &ydata,
    const numcpp::array<T> &p0, Jacobian jac,
    T ftol = 1e-8, T xtol = 1e-8, T gtol = 1e-8, size_t maxiter = 1000
);
```

#### Arguments

- `f`: The model function. It must take the independent variable as the first 
argument and the parameters to fit as the second argument.
- `xdata`: The independent variable where the data is measured.
- `ydata`: The dependent data.
- `p0`: Initial guess for the parameters.
- `jac`: Function that computes the gradient vector of *f* with respect to the 
parameters.
- `ftol`: Tolerance for termination by the change of the cost function.
- `xtol`: Tolerance for termination by the change of the parameters.
- `gtol`: Tolerance for termination by the norm of the gradient.
- `maxiter`: Maximum number of iterations to perform.

#### Example

Estimate the parameters of the Gompertz function over a sample of points. The 
Gompertz function is defined by

![$f(x; \alpha, \beta, k) = \alpha\exp\left(-\beta\exp\left(-kx\right)\right))$](https://render.githubusercontent.com/render/math?math=f(x%3B%20%5Calpha%2C%20%5Cbeta%2C%20k)%20%3D%20%5Calpha%5Cexp%5Cleft(-%5Cbeta%5Cexp%5Cleft(-kx%5Cright)%5Cright))
```cpp
#include <iostream>
#include <random>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Gompertz function.
double gompertz(double x, const np::array<double> &param) {
    double a = param[0], b = param[1], k = param[2];
    return a*np::exp(-b*np::exp(-k*x));
}

// Derivative (gradient) of Gompertz function.
np::array<double> gompertz_der(double x, const np::array<double> &param) {
    double a = param[0], b = param[1], k = param[2];
    return {
        np::exp(-b*np::exp(-k*x)),
        -a*np::exp(-k*x - b*np::exp(-k*x)),
        a*b*x*np::exp(-k*x - b*np::exp(-k*x))
    };
}

int main() {
    default_random_engine seed(0);
    normal_distribution<double> error(0., 7.);
    np::printoptions::threshold = 10;

    // Sample n points around the function.
    size_t n = 30;
    np::array<double> xdata = np::linspace(0., 10., n);
    np::array<double> ydata(n);
    for (int i = 0; i < n; ++i) {
        ydata[i] = gompertz(xdata[i], {89., 29., 0.9}) + error(seed);
    }

    // Estimate the parameters using least squares.
    np::array<double> p0 = {100., 10., 1.};
    scicpp::OptimizeResult<double> result;
    result = scicpp::curve_fit(gompertz, xdata, ydata, p0, gompertz_der);
    cout << result;

    // Save points and estimated function into a .csv file for later plotting.
    np::matrix<double> df(n, 3);
    for (int i = 0; i < n; ++i) {
        df[i][0] = xdata[i];
        df[i][1] = ydata[i];
        df[i][2] = gompertz(xdata[i], result.x);
    }
    np::save_txt("gompertz.csv", df, ',', {"x", "y", "f"});

    return 0;
}
```

```
[Out] fun: 510.402
      x: [85.083969, 20.086815, 0.85627287]
      jac: [0.85376065, 7.6077536, -4.7887953, ..., 0.84324265, -3.4041416, -9.9317724]
      hess:
      [[1.8897626e-09,  -1.607885e-07,             0]
       [3.2117399e-07, -2.0340213e-05, 0.00014088623]
       [1.4684235e-05, -0.00069220358,  0.0095890793]
       ...
       [   0.99309559,   -0.029144607,     5.4504837]
       [   0.99485627,   -0.021731759,     4.2146934]
       [   0.99616882,   -0.016197011,     3.2534635]]
      success: true
      status: "ftol" termination condition is satisfied.
      niter: 10
      nfev: 11
      njev: 11
      nhev: 11
```
<img src="../Figuras/gompertz.png" alt="Gompertz function" height="auto" width="500">