# Integration and ODEs

## Content

- [Integration](#Integration)
- [Ordinary differential equations](#Ordinary-differential-equations)

## Integration

### `quad`

Compute a definite integral using Gaussian quadrature.
```cpp
template <class T, class Function = T(T)>
T quad(
    Function f, T a, T b,
    bool show = false,
    T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
);
```

Arguments:
- `f`: Function to integrate.
- `a`: Lower limit of integration.
- `b`: Upper limit of integration.
- `show`: If `true`, prints a short description of the cause of termination. 
Default is `false`.
- `tol`: The desired absolute tolerance.
- `rtol`: The desired relative tolerance.
- `maxiter`: Maximum order of Gaussian quadrature.

#### Example

Calculate ![$\int_0^1 e^{-x}\cos(2x^2 + 1)dx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20e%5E%7B-x%7D%5Ccos(2x%5E2%20%2B%201)dx).

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x) {
    return np::exp(-x) * np::cos(2.0*x*x + 1.0);
}

int main() {
    double a = 0, b = 1;
    double integral = scicpp::quad(f, a, b, true);
    cout << "Integral: " << integral << "\n";
    return 0;
}
```

```
[Out] Converged after 7 iterations, value is 0.0508862
      Integral: 0.0508862
```

### `fixed_quad`

Compute a definite integral using fixed-order Gaussian quadrature.
```cpp
template <class T, class Function = T(T)>
T fixed_quad(Function f, T a, T b, size_t n);
```

Arguments:
- `f`: Function to integrate.
- `a`: Lower limit of integration.
- `b`: Upper limit of integration.
- `n`: Order of quadrature integration.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x) {
    return np::exp(-x) * np::cos(2.0*x*x + 1.0);
}

int main() {
    double a = 0, b = 1;
    cout << "5 points approximation: " << scicpp::fixed_quad(f, a, b, 5) 
         << "\n";
    return 0;
}
```

```
[Out] 5 points approximation: 0.0508857
```

### `leggauss`

Compute the sample points and weights for Gauss-Legendre quadrature. These 
sample points and weights will correctly integrate polynomials of degree 
![$2n-1$](https://render.githubusercontent.com/render/math?math=2n-1) or less 
over the interval ![$[-1,1]$](https://render.githubusercontent.com/render/math?math=%5B-1%2C1%5D).
```cpp
template <class T>
void leggauss(
    size_t n, numcpp::array<T> &points, numcpp::array<T> &weights
);
```

Arguments:
- `n`: Number of sample points and weights. Must be greater than 1.
- `points`: Array that will contain the sample points.
- `weights`: Array that will contain the weights.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> points, weights;
    scicpp::leggauss(5, points, weights);
    cout << "Points:\n" << points << "\nWeights:\n" << weights << "\n";
    return 0;
}
```

```
[Out] Points:
      [-0.90617985, -0.53846931, 8.3341178e-18, 0.53846931, 0.90617985]
      Weights:
      [0.23692689, 0.47862867, 0.56888889, 0.47862867, 0.23692689]
```

### `romberg`

Compute a definite integral using Romberg method.
```cpp
template <class T, class Function = T(T)>
T romberg(
    Function f, T a, T b,
    bool show = false,
    T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 10
);
```

Arguments:
- `f`: Function to integrate.
- `a`: Lower limit of integration.
- `b`: Upper limit of integration.
- `show`: If `true`, prints the triangular matrix showing the Richardson 
extrapolations. Default is `false`.
- `tol`: The desired absolute tolerance.
- `rtol`: The desired relative tolerance.
- `maxiter`: Maximum order of extrapolation.


#### Example

Calculate ![$\int_0^1 e^{-x}\cos(2x^2 + 1)dx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20e%5E%7B-x%7D%5Ccos(2x%5E2%20%2B%201)dx).
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x) {
    return np::exp(-x) * np::cos(2.0*x*x + 1.0);
}

int main() {
    double a = 0, b = 1;
    double integral = scicpp::romberg(f, a, b, true);
    cout << "Integral: " << integral << "\n";
    return 0;
}
```

```
[Out] Steps StepSize Results
          1        1 0.0880522
          2      0.5 0.0654782 0.0579536
          4     0.25 0.0545416 0.0508961 0.0504256
          8    0.125 0.0517954   0.05088 0.0508789 0.0508861
         16   0.0625 0.0511131 0.0508857 0.0508861 0.0508862 0.0508862
         32  0.03125 0.0509429 0.0508861 0.0508862 0.0508862 0.0508862 0.0508862
         64 0.015625 0.0509003 0.0508862 0.0508862 0.0508862 0.0508862 0.0508862 0.0508862
      Converged after 7 iterations, value is 0.0508862
      Integral: 0.0508862
```

### `newton_cotes`

Return weights for Newton-Cotes integration. Let ![$a = x_0 < x_1 < \ldots < x_n = b$](https://render.githubusercontent.com/render/math?math=a%20%3D%20x_0%20%3C%20x_1%20%3C%20%5Cldots%20%3C%20x_n%20%3D%20b) 
be equally spaced sample points over the interval ![$[a,b]$](https://render.githubusercontent.com/render/math?math=%5Ba%2Cb%5D). The Newton-Cotes formula 
for the integral is 

![$\int_a^b f(x)dx \approx \frac{b - a}{n}\sum_{i = 0}^{n} w_i f(x_i)$](https://render.githubusercontent.com/render/math?math=%5Cint_a%5Eb%20f(x)dx%20%5Capprox%20%5Cfrac%7Bb%20-%20a%7D%7Bn%7D%5Csum_%7Bi%20%3D%200%7D%5E%7Bn%7D%20w_i%20f(x_i))

#### Example

Calculate ![$\int_0^1 (3xe^x + 5x - 1)dx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20(3xe%5Ex%20%2B%205x%20-%201)dx). Analytic result is ![$4.5$](https://render.githubusercontent.com/render/math?math=4.5).
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> integral(3);

    for (int n = 1; n <= 3; ++n) {
        np::array<double> x = numcpp::linspace(0.0, 1.0, n + 1);
        np::array<double> y = 3.0*x*np::exp(x) + 5.0*x - 1.0;
        np::array<double> weights;
        scicpp::newton_cotes(n, weights);
        integral[n - 1] = np::sum(weights * y) / n;
    }

    cout << "Trapezoidal rule: " << integral[0] << "\n";
    cout << "Simpson's rule: " << integral[1] << "\n";
    cout << "Simpson's 3/8 rule: " << integral[2] << "\n";

    return 0;
}
```

```
[Out] Trapezoidal rule: 5.57742
      Simpson's rule: 4.50786
      Simpson's 3/8 rule: 4.50351
```