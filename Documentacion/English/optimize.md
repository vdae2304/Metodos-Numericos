# Optimization and root finding

## Content

- [Root finding (Scalar functions)](#Root-finding-Scalar-functions)

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