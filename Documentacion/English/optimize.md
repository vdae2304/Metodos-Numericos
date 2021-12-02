# Optimization and root finding

## Content

- [Root finding (Scalar functions)](#Root-finding-Scalar-functions)
- [Local (multivariate) optimization](#Local-multivariate-optimization)

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
    numcpp::matrix<T> hess;
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
    Function f, const numcpp::array<T> x0, Jacobian jac,
    T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
);
```

#### Attributes

- `f`: Objective function to be minimized.
- `x0`: An initial estimate for the optimal value of `x`.
- `jac`: A function that returns the gradient vector of `f` at `x`.
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
    }
    for (int i = 1; i < x.size(); ++i) {
        jac[i] += 200*(x[i] - x[i - 1]*x[i - 1]);
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

### `line_search`

Given a starting point ![$\mathbf{x}_k$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bx%7D_k) and a search direction ![$\mathbf{p}_k$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bp%7D_k), 
find a step length ![$\alpha$](https://render.githubusercontent.com/render/math?math=%5Calpha) that satisfies the Wolfe conditions:

i) ![$f(\mathbf{x}_k + \alpha\mathbf{p}_k) \leq f(\mathbf{x}_k) + c_1\alpha\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k)$](https://render.githubusercontent.com/render/math?math=f(%5Cmathbf%7Bx%7D_k%20%2B%20%5Calpha%5Cmathbf%7Bp%7D_k)%20%5Cleq%20f(%5Cmathbf%7Bx%7D_k)%20%2B%20c_1%5Calpha%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k))

ii) ![$-\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k + \alpha\mathbf{p}_k) \leq -c_2\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k)$](https://render.githubusercontent.com/render/math?math=-%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k%20%2B%20%5Calpha%5Cmathbf%7Bp%7D_k)%20%5Cleq%20-c_2%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k))

 ```cpp
template <class T, class Function, class Jacobian>
RootResults<T> line_search(
    Function f, Jacobian jac,
    const numcpp::array<T> &xk, const numcpp::array<T> &pk,
    const numcpp::array<T> &gfk, T fk,
    T c1 = 0.0001, T c2 = 0.9, T amax = 1.0, size_t maxiter = 20
);
 ```

#### Arguments

- `f`: Objective function.
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
    }
    for (int i = 1; i < x.size(); ++i) {
        jac[i] += 200*(x[i] - x[i - 1]*x[i - 1]);
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