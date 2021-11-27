# Optimization and root finding

## Content

- [Root finding (Scalar functions)](#Root-finding-(Scalar-functions))

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

### `bisect`

Find root of a function within an interval using bisection. 
```cpp
template <class T, class Function = T(T)>
RootResults<T> bisect(
    Function f,
    T a,
    T b,
    T tol = 1e-9,
    size_t maxiter = 100
);
```

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
[Out] Converged after 32 iterations, value is 1.25992
```

### `newton`

Find a root of a function using the Newton-Raphson method.
```cpp
template <class T, class Function = T(T), class Derivative = T(T)>
RootResults<T> newton(
    Function f,
    T x0,
    Derivative df,
    T tol = 1e-9,
    size_t maxiter = 50
);
```

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
    Function f,
    T x0,
    T x1,
    T tol = 1e-9,
    size_t maxiter = 50
);
```

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
[Out] Converged after 10 iterations, value is 1.25992
```

### `halley`

Find a root of a function using the Halley's method.
```cpp
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
);
```

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