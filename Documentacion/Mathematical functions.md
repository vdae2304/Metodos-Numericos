# Mathematical functions

Defined in `numcpp/math.h`

- [Mathematical functions](#mathematical-functions)
  - [Template parameters](#template-parameters)
  - [Constants](#constants)
  - [Basic functions](#basic-functions)
    - [`abs`](#abs)
    - [`fmod`](#fmod)
    - [`modf`](#modf)
    - [`fmax`](#fmax)
    - [`fmin`](#fmin)
  - [Trigonometric functions](#trigonometric-functions)
    - [`cos`](#cos)
    - [`sin`](#sin)
    - [`tan`](#tan)
    - [`hypot`](#hypot)
    - [`acos`](#acos)
    - [`asin`](#asin)
    - [`atan`](#atan)
    - [`atan2`](#atan2)
    - [`degrees`](#degrees)
    - [`radians`](#radians)
  - [Hyperbolic functions](#hyperbolic-functions)
    - [`cosh`](#cosh)
    - [`sinh`](#sinh)
    - [`tanh`](#tanh)
    - [`acosh`](#acosh)
    - [`asinh`](#asinh)
    - [`atanh`](#atanh)
  - [Exponential and logarithmic functions](#exponential-and-logarithmic-functions)
    - [`exp`](#exp)
    - [`log`](#log)
    - [`log10`](#log10)
    - [`exp2`](#exp2)
    - [`log2`](#log2)
    - [`expm1`](#expm1)
    - [`log1p`](#log1p)
  - [Power functions](#power-functions)
    - [`pow`](#pow)
    - [`sqrt`](#sqrt)
    - [`cbrt`](#cbrt)
  - [Rounding](#rounding)
    - [`ceil`](#ceil)
    - [`floor`](#floor)
    - [`trunc`](#trunc)
    - [`round`](#round)
  - [Floating-point manipulation functions](#floating-point-manipulation-functions)
    - [`frexp`](#frexp)
    - [`ldexp`](#ldexp)
    - [`copysign`](#copysign)
    - [`nextafter`](#nextafter)
  - [Integer-valued functions](#integer-valued-functions)
    - [`gcd`](#gcd)
    - [`lcm`](#lcm)
  - [Complex numbers](#complex-numbers)
    - [`real`](#real)
    - [`imag`](#imag)
    - [`conj`](#conj)
    - [`abs`](#abs-1)
    - [`arg`](#arg)
  - [Clasification functions](#clasification-functions)
    - [`isfinite`](#isfinite)
    - [`isinf`](#isinf)
    - [`isnan`](#isnan)

## Template parameters

* `T` Type of the elements contained in the tensor. This shall be an arithmetic
type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the tensor. It must be a positive integer.

## Constants

`numcpp/math.h` defines the following mathematical constants.

Floating point representation of (positive) infinity.
```cpp
template <class T>
static constexpr T constants<T>::inf;
```

Floating point representation of quiet NaN (Not a Number).
```cpp
template <class T>
static constexpr T constants<T>::nan;
```

Euler's constant, base of natural logarithm.
```cpp
template <class T>
static constexpr T constants<T>::e;
```

Pi, ratio of a circle's circunferemce to its diameter.
```cpp
template <class T>
static constexpr T constants<T>::pi;
```

Golden ratio.
```cpp
template <class T>
static constexpr T constants<T>::golden;
```

Euler–Mascheroni's constant.
```cpp
template <class T>
static constexpr T constants<T>::egamma;
```

Example
```cpp
#include <iostream>
#include "numcpp/math.h"
namespace np = numcpp;
int main() {
    std::cout << "Infinity: " << np::constants<double>::inf << "\n";
    std::cout << "NaN: " << np::constants<double>::nan << "\n";
    std::cout << "e: " << np::constants<double>::e << "\n";
    std::cout << "Pi: " << np::constants<double>::pi << "\n";
    std::cout << "Golden ratio: " << np::constants<double>::golden << "\n";
    std::cout << "Euler-Mascheroni: " << np::constants<double>::egamma << "\n";
    return 0;
}
```

Output

```
Infinity: inf
NaN: nan
e: 2.71828
Pi: 3.14159
Golden ratio: 1.61803
Euler-Mascheroni: 0.577216
```

## Basic functions

### `abs`

Return the absolute value, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> abs(const tensor<T, Rank> &x);
```

Parameter

* `x` A tensor-like object with the values whose absolute value is computed.

Returns

* A light-weight object with the absolute value of each element in the tensor.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> x;
    std::cin >> x;
    std::cout << np::abs(x) << "\n";
    return 0;
}
```

Input

```
[2, 7, -3, 1, 0, 8, -1, 9, -5, 12]
```

Output

```
[ 2,  7,  3,  1,  0,  8,  1,  9,  5, 12]
```

### `fmod`

Return the floating-point remainder of $x/y$, element-wise. The floating-point
remainder of $x/y$ is $x - ny$, where $n$ is the truncated value (i.e., rounded
towards zero) of $x/y$.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> fmod(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> fmod(
    const tensor<T, Rank> &x, const typename tensor<T, Rank>::value_type &y
);

template <class T, size_t Rank>
tensor<T, Rank> fmod(
    const typename tensor<T, Rank>::value_type &x, const tensor<T, Rank> &y
)
```

Parameters

* `x` A tensor-like object with the values of the quotient numerator.
* `y` A tensor-like object with the values of the quotient denominator.

Returns

* A light-weight object with the remainder of `x/y`, element-wise. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << np::fmod(x, y) << "\n";
    return 0;
}
```

Input

```
[4.14, 12.32, 11.29, 11.57, 5.87, 2.45, 9.74, 9.31, 11.83, 3.87]
[   1,     6,     2,     8,    4,    5,    3,    4,     5,    2]
```

Output

```
[0.14, 0.32, 1.29, 3.57, 1.87, 2.45, 0.74, 1.31, 1.83, 1.87]
```

### `modf`

Descompose $x$ into integral and fractional parts, element-wise.
```cpp
template <class T, size_t Rank>
tensor<std::pair<T, T>, Rank> modf(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to be decomposed.

Returns

* A light-weight object with the integral and fractional parts of each element
in the tensor. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    np::array<std::pair<double, double> > y = np::modf(x);
    np::array<double> intpart(x.size());
    np::array<double> fracpart(x.size());
    for (unsigned i = 0; i < x.size(); ++i) {
        intpart[i] = y[i].first;
        fracpart[i] = y[i].second;
    }
    std::cout << intpart << "\n";
    std::cout << fracpart << "\n";
    return 0;
}
```

Input

```
[4.14, 12.32, 11.29, 11.57, 5.87, 2.45, 9.74, 9.31, 11.83, 3.87]
```

Output

```
[ 4, 12, 11, 11,  5,  2,  9,  9, 11,  3]
[0.14, 0.32, 0.29, 0.57, 0.87, 0.45, 0.74, 0.31, 0.83, 0.87]
```

### `fmax`

Return the maximum value, element-wise. If one of the elements being compared
is a NaN, then the non-NaN element is returned.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> fmax(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> fmax(
    const tensor<T, Rank> &x, const typename tensor<T, Rank>::value_type &y
);

template <class T, size_t Rank>
tensor<T, Rank> fmax(
    const typename tensor<T, Rank>::value_type &x, const tensor<T, Rank> &y
)
```

Parameters

* `x` A tensor-like object with floating point or integral values.
* `y` A tensor-like object with floating point or integral values.

Returns

* A light-weight object with the element-wise maximum. This function does not
create a new tensor, instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << np::fmax(x, y) << "\n";
    return 0;
}
```

Input

```
[6.64, 4.85, 6.71, 0.21, 8.31, 1.85, 9.28, 3.85, 4.93, 1.27]
[7.33, 2.42, 1.32, 9.72, 5.86, 4.91, 8.4 , 9.62, 2.34, 0.91]
```

Output

```
[7.33, 4.85, 6.71, 9.72, 8.31, 4.91, 9.28, 9.62, 4.93, 1.27]
```

### `fmin`

Return the minimum value, element-wise. If one of the elements being compared
is a NaN, then the non-NaN element is returned.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> fmin(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> fmin(
    const tensor<T, Rank> &x, const typename tensor<T, Rank>::value_type &y
);

template <class T, size_t Rank>
tensor<T, Rank> fmin(
    const typename tensor<T, Rank>::value_type &x, const tensor<T, Rank> &y
);
```

Parameters

* `x` A tensor-like object with floating point or integral values.
* `y` A tensor-like object with floating point or integral values.

Returns

* A light-weight object with the element-wise minimum. This function does not
create a new tensor, instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << np::fmin(x, y) << "\n";
    return 0;
}
```

Input

```
[6.64, 4.85, 6.71, 0.21, 8.31, 1.85, 9.28, 3.85, 4.93, 1.27]
[7.33, 2.42, 1.32, 9.72, 5.86, 4.91, 8.4 , 9.62, 2.34, 0.91]
```

Output

```
[6.64, 2.42, 1.32, 0.21, 5.86, 1.85,  8.4, 3.85, 2.34, 0.91]
```

## Trigonometric functions

### `cos`

Return the cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cos(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the cosine of each element in the tensor. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double pi = np::constants<double>::pi;
    np::array<double> x{0, pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6, pi};
    std::cout << "x:\n" << x << "\n";
    std::cout << "cos(x):\n" << np::cos(x) << "\n";
    return 0;
}
```

Output

```
x:
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951,
  2.3561945,  2.6179939,  3.1415927]
cos(x):
[            1,     0.8660254,    0.70710678,           0.5, 6.1230318e-17,
          -0.5,   -0.70710678,    -0.8660254,            -1]
```

### `sin`

Return the sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> sin(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the sine of each element in the tensor. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double pi = np::constants<double>::pi;
    np::array<double> x{0, pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6, pi};
    std::cout << "x:\n" << x << "\n";
    std::cout << "sin(x):\n" << np::sin(x) << "\n";
    return 0;
}
```

Output

```
x:
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951,
  2.3561945,  2.6179939,  3.1415927]
sin(x):
[            0,           0.5,    0.70710678,     0.8660254,             1,
     0.8660254,    0.70710678,           0.5, 1.2246064e-16]
```

### `tan`

Return the tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> tan(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the tangent of each element in the tensor. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double pi = np::constants<double>::pi;
    np::array<double> x{0, pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6, pi};
    std::cout << "x:\n" << x << "\n";
    std::cout << "tan(x):\n" << np::tan(x) << "\n";
    return 0;
}
```

Output

```
x:
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951,
  2.3561945,  2.6179939,  3.1415927]
tan(x):
[             0,     0.57735027,              1,      1.7320508,  1.6331239e+16,
     -1.7320508,             -1,    -0.57735027, -1.2246468e-16]
```

### `hypot`

Return the hypotenuse of a right-angled triangle whose legs are `x` and `y`,
element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> hypot(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> hypot(
    const tensor<T, Rank> &x, const typename tensor<T, Rank>::value_type &y
);

template <class T, size_t Rank>
tensor<T, Rank> hypot(
    const typename tensor<T, Rank>::value_type &x, const tensor<T, Rank> &y
);
```

Parameters

* `x` A tensor-like object with one of the legs of the right-angle triangles.
* `y` A tensor-like object with one of the legs of the right-angle triangles.

Returns

* A light-weight object with the hypotenuse of the triangles. This function
does not create a new tensor, instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << np::hypot(x, y) << "\n";
    return 0;
}
```

Input

```
[1, 3, 3,  5,  7,  8,  9,  9, 11, 12]
[1, 4, 2, 12, 24, 15, 10, 40, 60, 35]
```

Output

```
[1.4142136,         5, 3.6055513,        13,        25,        17, 13.453624,
        41,        61,        37]
```

### `acos`

Return the principal value of the arc cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> acos(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose arc cosine is computed.

Returns

* A light-weight object with the arc cosine, in radians, of each value in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double sqrt2 = np::sqrt(2);
    double sqrt3 = np::sqrt(3);
    np::array<double> x{-1, -sqrt3/2, -1/sqrt2, -0.5, 0, 0.5, 1/sqrt2,
                        sqrt3/2, 1};
    std::cout << "x:\n" << x << "\n";
    std::cout << "acos(x):\n" << np::acos(x) << "\n";
    return 0;
}
```

Output

```
x:
[         -1,  -0.8660254, -0.70710678,        -0.5,           0,         0.5,
  0.70710678,   0.8660254,           1]
acos(x):
[ 3.1415927,  2.6179939,  2.3561945,  2.0943951,  1.5707963,  1.0471976,
 0.78539816, 0.52359878,          0]
```

### `asin`

Return the principal value of the arc sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> asin(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose arc sine is computed.

Returns

* A light-weight object with the arc sine, in radians, of each value in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double sqrt2 = np::sqrt(2);
    double sqrt3 = np::sqrt(3);
    np::array<double> x{-1, -sqrt3/2, -1/sqrt2, -0.5, 0, 0.5, 1/sqrt2,
                        sqrt3/2, 1};
    std::cout << "x:\n" << x << "\n";
    std::cout << "asin(x):\n" << np::asin(x) << "\n";
    return 0;
}
```

Output

```
x:
[         -1,  -0.8660254, -0.70710678,        -0.5,           0,         0.5,
  0.70710678,   0.8660254,           1]
asin(x):
[ -1.5707963,  -1.0471976, -0.78539816, -0.52359878,           0,  0.52359878,
  0.78539816,   1.0471976,   1.5707963]
```

### `atan`

Return the principal value of the arc tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> atan(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose arc tangent is computed.

Returns

* A light-weight object with the arc tangent, in radians, of each value in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double inf = np::constants<double>::inf;
    double sqrt3 = np::sqrt(3);
    np::array<double> x{-inf, -sqrt3, -1, -1/sqrt3, 0, 1/sqrt3, 1, sqrt3, inf};
    std::cout << "x:\n" << x << "\n";
    std::cout << "atan(x):\n" << np::atan(x) << "\n";
    return 0;
}
```

Output

```
x:
[       -inf,  -1.7320508,          -1, -0.57735027,           0,  0.57735027,
           1,   1.7320508,         inf]
atan(x):
[ -1.5707963,  -1.0471976, -0.78539816, -0.52359878,           0,  0.52359878,
  0.78539816,   1.0471976,   1.5707963]
```

### `atan2`

Return the principal value of the arc tangent of $y/x$, element-wise. To
compute the value, the function takes into account the sign of both arguments
in order to determine the quadrant.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> atan2(const tensor<T, Rank> &y, const tensor<T, Rank> &x);

template <class T, size_t Rank>
tensor<T, Rank> atan2(
    const tensor<T, Rank> &y, const typename tensor<T, Rank>::value_type &x
);

template <class T, size_t Rank>
tensor<T, Rank> atan2(
    const typename tensor<T, Rank>::value_type &y, const tensor<T, Rank> &x
);
```

Parameters

* `y` A tensor-like object with the $y$-coordinates.
* `x` A tensor-like object with the $x$-coordinates.

Returns

* A light-weight object with the arc tangent, in radians, of `y/x`. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double sqrt2 = np::sqrt(2);
    double sqrt3 = np::sqrt(3);
    np::array<double> x{1, sqrt3/2, 1/sqrt2,     0.5, 0,    -0.5, -1/sqrt2,
                        -sqrt3/2, -1};
    np::array<double> y{0,     0.5, 1/sqrt2, sqrt3/2, 1, sqrt3/2,  1/sqrt2,
                             0.5,  0};
    std::cout << "x:\n" << x << "\n";
    std::cout << "y:\n" << y << "\n";
    std::cout << "atan2(y, x):\n" << np::atan2(y, x) << "\n";
    return 0;
}
```

Output

```
x:
[          1,   0.8660254,  0.70710678,         0.5,           0,        -0.5,
 -0.70710678,  -0.8660254,          -1]
y:
[         0,        0.5, 0.70710678,  0.8660254,          1,  0.8660254,
 0.70710678,        0.5,          0]
atan2(y, x):
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951,
  2.3561945,  2.6179939,  3.1415927]
```

### `degrees`

Convert angles from radians to degrees.
```cpp
double degrees(double x);
float degrees(float x);
long double degrees(long double x);
template <class T>
double degrees(T x); // Additional overloads for integral types.
```

Parameters

* `x` Angle in radians.

Returns

* The corresponding angle in degrees.

<h3><code>degrees</code></h3>

Convert angles from radians to degrees, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> degrees(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the corresponding angles in degrees. This function
does not create a new tensor, instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double pi = np::constants<double>::pi;
    np::array<double> x{0, pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6, pi};
    std::cout << "x:\n" << x << "\n";
    std::cout << "degrees(x):\n" << np::degrees(x) << "\n";
    return 0;
}
```

Output

```
x:
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951,
  2.3561945,  2.6179939,  3.1415927]
degrees(x):
[  0,  30,  45,  60,  90, 120, 135, 150, 180]
```

### `radians`

Convert angles from degrees to radians.
```cpp
double radians(double x);
float radians(float x);
long double radians(long double x);
template <class T>
double radians(T x); // Additional overloads for integral types.
```

Parameters

* `x` Angle in degrees.

Returns

* The corresponding angle in radians.

<h3><code>radians</code></h3>

Convert angles from degrees to radians, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> radians(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in degrees.

Parameters

* A light-weight object with the corresponding angles in radians. This function
does not create a new tensor, instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x{0, 30, 45, 60, 90, 120, 135, 150, 180};
    std::cout << "x:\n" << x << "\n";
    std::cout << "radians(x):\n" << np::radians(x) << "\n";
    return 0;
}
```

Output

```
x:
[  0,  30,  45,  60,  90, 120, 135, 150, 180]
radians(x):
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951,
  2.3561945,  2.6179939,  3.1415927]
```

## Hyperbolic functions

### `cosh`

Return the hyperbolic cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cosh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the hyperbolic angles.

Returns

* A light-weight object with the hyperbolic cosine of each element in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::cosh(x) << "\n";
    return 0;
}
```

Input

```
[-4, -3, -2, -1, 0, 1, 2, 3, 4]
```

Output

```
[27.308233, 10.067662, 3.7621957, 1.5430806,         1, 1.5430806, 3.7621957,
 10.067662, 27.308233]
```

### `sinh`

Return the hyperbolic sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> sinh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the hyperbolic angles.

Returns

* A light-weight object with the hyperbolic sine of each element in the tensor.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::sinh(x) << "\n";
    return 0;
}
```

Input

```
[-4, -3, -2, -1, 0, 1, 2, 3, 4]
```

Output

```
[-27.289917, -10.017875, -3.6268604, -1.1752012,          0,  1.1752012,
  3.6268604,  10.017875,  27.289917]
```

### `tanh`

Return the hyperbolic tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> tanh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the hyperbolic angles.

Returns

* A light-weight object with the hyperbolic tangent of each element in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::tanh(x) << "\n";
    return 0;
}
```

Input

```
[-5, -1, -0.5, -0.25, 0, 0.25, 0.5, 1, 5]
```

Output

```
[ -0.9999092, -0.76159416, -0.46211716, -0.24491866,           0,  0.24491866,
  0.46211716,  0.76159416,   0.9999092]
```

### `acosh`

Return the inverse hyperbolic cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> acosh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose inverse hyperbolic cosine is
computed.

Returns

* A light-weight object with the inverse hyperbolic cosine of each element in
the tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::acosh(x) << "\n";
    return 0;
}
```

Input

```
[1, 1.13, 1.54, 2.35, 3.76, 6.13, 10.07, 16.57, 27.31]
```

Output

```
[         0,  0.5045336, 0.99737411,  1.4988676,  1.9993944,  2.4996215,
  3.0002334,  3.4998292,  4.0000648]
```

### `asinh`

Return the inverse hyperbolic sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> asinh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose inverse hyperbolic sine is
computed.

Returns

* A light-weight object with the inverse hyperbolic sine of each element in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::asinh(x) << "\n";
    return 0;
}
```

Input

```
[-27.29, -10.02, -3.63, -1.17, 0, 1.17, 3.63, 10.02, 27.29]
```

Output

```
[  -4.000003,  -3.0002111,  -2.0008342, -0.99662501,           0,  0.99662501,
   2.0008342,   3.0002111,    4.000003]
```

### `atanh`

Return the inverse hyperbolic tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> atanh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose inverse hyperbolic tangent is
computed.

Returns

* A light-weight object with the inverse hyperbolic tangent of each element in
the tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::atanh(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[       -inf, -0.97295507, -0.54930614, -0.25541281,           0,  0.25541281,
  0.54930614,  0.97295507,         inf]
```

## Exponential and logarithmic functions

### `exp`

Return the base-$e$ exponential, which is $e$ raised to the power $x$: $e^x$,
element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> exp(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values of the exponents.

Returns

* A light-weight object with the exponential of each element in the tensor.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::exp(x) << "\n";
    return 0;
}
```

Input

```
[-4, -3, -2, -1, 0, 1, 2, 3, 4, 5]
```

Output

```
[0.018315639, 0.049787068,  0.13533528,  0.36787944,           1,   2.7182818,
   7.3890561,   20.085537,    54.59815,   148.41316]
```

### `log`

Return the natural logarithm, which is the inverse of the exponential function
(exp), element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light weight-object with the natural logarithm of each element in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::log(x) << "\n";
    return 0;
}
```

Input

```
[0.01, 0.05, 0.13, 0.37, 1, 2.72, 7.39, 20.09, 54.56, 148.41]
```

Output

```
[ -4.6051702,  -2.9957323,  -2.0402208, -0.99425227,           0,   1.0006319,
   2.0001277,   3.0002222,    3.999301,   4.9999787]
```

### `log10`

Return the common (base-10) logarithm, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log10(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light-weight object with the common logarithm of each element in the
tensor. This function does not create a new tensor, instead, an  expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::log10(x) << "\n";
    return 0;
}
```

Input

```
[0.001, 0.01, 0.1, 1, 10, 100, 1000, 10000, 100000, 1000000]
```

Output

```
[-3, -2, -1,  0,  1,  2,  3,  4,  5,  6]
```

### `exp2`

Return the base-2 exponential, which is 2 raised to the power $x$: $2^x$,
element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> exp2(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values of the exponents.

Returns

* A light-weight object with the base-2 exponential of each element in the
tensor. This function does not create a new tensor, instead, an  expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::exp2(x) << "\n";
    return 0;
}
```

Input

```
[-3, -2, -1,  0,  1,  2,  3,  4,  5,  6]
```

Output

```
[0.125,  0.25,   0.5,     1,     2,     4,     8,    16,    32,    64]
```

### `log2`

Return the binary (base-2) logarithm, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log2(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light-weight object with the binary logarithm of each element in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::log2(x) << "\n";
    return 0;
}
```

Input

```
[0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64]
```

Output

```
[-3, -2, -1,  0,  1,  2,  3,  4,  5,  6]
```

### `expm1`

Return $e$ raised to the power $x$ minus one: $e^x - 1$, element-wise. For
values of $x$ close to zero, `expm1` is more accurate than `exp(x) - 1`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> expm1(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the exponential minus one of each element in the
tensor. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::expm1(x) << "\n";
    return 0;
}
```

Input

```
[-4, -3, -2, -1, 0, 1, 2, 3, 4, 5]
```

Output

```
[-0.98168436, -0.95021293, -0.86466472, -0.63212056,           0,   1.7182818,
   6.3890561,   19.085537,    53.59815,   147.41316]
```

### `log1p`

Return the natural logarithm of one plus $x$, element-wise. For values of $x$
close to zero, `log1p` is more accurate than `log(1 + x)`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log1p(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light-weight object with the natural logarithm of $(1 + x)$ for each
element in the tensor. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::log1p(x) << "\n";
    return 0;
}
```

Input

```
[-0.98, -0.95, -0.86, -0.63, 0, 1.72, 6.39, 19.09, 53.56, 147.41]
```

Output

```
[  -3.912023,  -2.9957323,  -1.9661129, -0.99425227,           0,   1.0006319,
   2.0001277,   3.0002222,    3.999301,   4.9999787]
```

## Power functions

### `pow`

Return $x$ raised to the power $y$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> pow(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> pow(
    const tensor<T, Rank> &x, const typename tensor<T, Rank>::value_type &y
);

template <class T, size_t Rank>
tensor<T, Rank> pow(
    const typename tensor<T, Rank>::value_type &x, const tensor<T, Rank> &y
);
```

Parameters

* `x` A tensor-like object with the values of the base.
* `y` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the result of rasing `x` to the power `y`,
element-wise. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << np::pow(x, y) << "\n";
    return 0;
}
```

Input

```
[2, 2, 2, 2, 3, 3, 4, 5, 10, 100]
[2, 3, 4, 5, 2, 3, 2, 2, -1, 0.5]
```

Output

```
[  4,   8,  16,  32,   9,  27,  16,  25, 0.1,  10]
```

### `sqrt`

Return the square root, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> sqrt(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose square root is computed.

Returns

* A light-weight object with the square root of each element in the tensor.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::sqrt(x) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

Output

```
[        1, 1.4142136, 1.7320508,         2,  2.236068, 2.4494897, 2.6457513,
 2.8284271,         3, 3.1622777]
```

### `cbrt`

Return the cubic root, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cbrt(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose cubic root is computed.

Returns

* A light-weight object with the cubic root of each element in the tensor. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::cbrt(x) << "\n";
    return 0;
}
```

Input

```
[-8, -4, -2, -1, 0, 1, 2, 4, 8]
```

Output

```
[        -2, -1.5874011,  -1.259921,         -1,          0,          1,
   1.259921,  1.5874011,          2]
```

## Rounding

### `ceil`

Rounds $x$ upward. Return the smallest integral value that is not less than
$x$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> ceil(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to round up.

Returns

* A light-weight object with the ceiling of each element in the tensor. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::ceil(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -0, -0, -0,  0,  1,  1,  1,  1]
```

### `floor`

Rounds $x$ downward. Return the largest integral value that is not greater than
$x$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> floor(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to round down.

Returns

* A light-weight object with the floor of each element in the tensor. This
function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::floor(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -1, -1, -1,  0,  0,  0,  0,  1]
```

### `trunc`

Rounds $x$ toward zero. Return the nearest integral value that is not larger in
magnitude than $x$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> trunc(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to truncate.

Returns

* A light-weight object with the truncated value of each element in the tensor.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::trunc(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -0, -0, -0,  0,  0,  0,  0,  1]
```

### `round`

Return the integral value that is nearest to $x$, element-wise. Halfway cases
are rounded away from zero.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> round(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to round.

Returns

* A light-weight object with the rounded value of each element in the tensor.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    std::cout << np::round(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -1, -1, -0,  0,  0,  1,  1,  1]
```

## Floating-point manipulation functions

### `frexp`

Descompose $x$ into its binary significand and an integral power of two, such
that $x = {significand} \times 2^{exponent}$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<std::pair<T, int>, Rank> frexp(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to be decomposed.

Returns

* A light-weight object with the binary significand and the exponent of each
element in the tensor. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    std::cin >> x;
    np::array<std::pair<double, int> > y = np::frexp(x);
    np::array<double> mantissa(x.size());
    np::array<int> exponent(x.size());
    for (unsigned i = 0; i < x.size(); ++i) {
        mantissa[i] = y[i].first;
        exponent[i] = y[i].second;
    }
    std::cout << mantissa << "\n";
    std::cout << exponent << "\n";
    return 0;
}
```

Input

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

Output

```
[     0,    0.5,    0.5,   0.75,    0.5,  0.625,   0.75,  0.875,    0.5, 0.5625]
[0, 1, 2, 2, 3, 3, 3, 3, 4, 4]
```

### `ldexp`

Return the result of multiplying $x$ (the significand) by 2 raised to the power
of `exp` (the exponent), element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> ldexp(const tensor<T, Rank> &x, const tensor<int, Rank> &exp);

template <class T, size_t Rank>
tensor<T, Rank> ldexp(const tensor<T, Rank> &x, int exp);

template <class T, size_t Rank>
tensor<T, Rank> ldexp(const T &x, const tensor<int, Rank> &exp);
```

Parameters

* `x` A tensor-like object with the values of the significand.
* `exp` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the result of $x \times 2^{exp}$, element-wise.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x;
    np::array<int> exp;
    std::cin >> x >> exp;
    std::cout << np::ldexp(x, exp) << "\n";
    return 0;
}
```

Input

```
[0, 0.5, 0.5, 0.75, 0.5, 0.625, 0.75, 0.875, 0.5, 0.5625]
[0, 1, 2, 2, 3, 3, 3, 3, 4, 4]
```

Output

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

### `copysign`

Return a value with the magnitude of $x$ and the sign of $y$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> copysign(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> copysign(
    const typename tensor<T, Rank>::value_type &x, const tensor<T, Rank> &y
);

template <class T, size_t Rank>
tensor<T, Rank> copysign(
    const tensor<T, Rank> &x, const typename tensor<T, Rank>::value_type &y
);
```

Parameters

* `x` A tensor-like object with the values to change the sign of.
* `y` A tensor-like object with the values to copy the sign from.

Returns

* A light-weight object with the values in the first tensor and the signs in
the second tensor. This function does not create a new tensor,  instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << np::copysign(x, y) << "\n";
    return 0;
}
```

Input

```
[5,  3, -1, 0,  0, 2,  8, 10, -7]
[1, -1,  1, 1, -1, 1, -1, -1, -1]
```

Output

```
[  5,  -3,   1,   0,  -0,   2,  -8, -10,  -7]
```

### `nextafter`

Return the next representable value after $x$ in the direction of $y$,
element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> nextafter(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> nextafter(
    const typename tensor<T, Rank>::value_type &x, const tensor<T, Rank> &y
);

template <class T, size_t Rank>
tensor<T, Rank> nextafter(
    const tensor<T, Rank> &x, const typename tensor<T, Rank>::value_type &y
);
```

Parameters

* `x` A tensor-like object with the base values.
* `y` A tensor-like object with the directions where to look for the next
representable values.

Returns

* A light-weight object with the next representable value of each element in
the first tensor in the direction of the second tensor. This function does not
create a new tensor,  instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    precision = fullprecision;
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << np::nextafter(x, y) << "\n";
    return 0;
}
```

Input

```
[-2, -2, -1, -1,  0, 0, 1, 1, 2, 2]
[-3, -1, -2,  0, -1, 1, 0, 2, 1, 3]
```

Output

```
[     -2.0000000000000004,      -1.9999999999999998,      -1.0000000000000002,
     -0.99999999999999989, -4.9406564584124654e-324,  4.9406564584124654e-324,
      0.99999999999999989,       1.0000000000000002,       1.9999999999999998,
       2.0000000000000004]
```

## Integer-valued functions

### `gcd`

Return the greatest common divisor of $|m|$ and $|n|$.
```cpp
template <class T1, class T2>
typename std::common_type<T1, T2>::type gcd(T1 m, T2 n);
```

Parameters

* `m` An integer value.
* `n` An integer value.

Returns

* The greatest common divisor of $|m|$ and $|n|$. If both `m` and `n` are zero,
return zero.

<h3><code>gcd</code></h3>

Return the greatest common divisor, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> gcd(const tensor<T, Rank> &m, const tensor<T, Rank> &n);

template <class T, size_t Rank>
tensor<T, Rank> gcd(
    const tensor<T, Rank> &m, const typename tensor<T, Rank>::value_type &n
);

template <class T, size_t Rank>
tensor<T, Rank> gcd(
    const typename tensor<T, Rank>::value_type &m, const tensor<T, Rank> &n
);
```

Parameters

* `m` A tensor-like object with integer values.
* `n` A tensor-like object with integer values.

Returns

* A light-weight object with the greatest common divisor of $|m|$ and $|n|$,
element-wise. If both `m` and `n` are zero, return zero. This function does not
create a new tensor,  instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> m, n;
    std::cin >> m >> n;
    std::cout << np::gcd(m, n) << "\n";
    return 0;
}
```

Input

```
[24, 36, 45, 20, 20, 120, 10, 0,  0, 30]
[20, 40, 75, 18, 19, 100,  0, 0, 30, 24]
```

Output

```
[ 4,  4, 15,  2,  1, 20, 10,  0, 30,  6]
```

### `lcm`

Return the least common multiple of $|m|$ and $|n|$.
```cpp
template <class T1, class T2>
typename std::common_type<T1, T2>::type lcm(T1 m, T2 n);
```

Parameters

* `m` An integer value.
* `n` An integer value.

Returns

* The least common multiple of $|m|$ and $|n|$. If either `m` or `n` is zero,
return zero.

<h3><code>lcm</code></h3>

Return the least common multiple, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> lcm(const tensor<T, Rank> &m, const tensor<T, Rank> &n);

template <class T, size_t Rank>
tensor<T, Rank> lcm(
    const tensor<T, Rank> &m, const typename tensor<T, Rank>::value_type &n
);

template <class T, size_t Rank>
tensor<T, Rank> lcm(
    const typename tensor<T, Rank>::value_type &m, const tensor<T, Rank> &n
);
```

Parameters

* `m` A tensor-like object with integer values.
* `n` A tensor-like object with integer values.

Returns

* A light-weight object with the least common multiple of $|m|$ and $|n|$,
element-wise. If either `m` or `n` is zero, return zero. This function does not
create a new tensor,  instead, an expression object is returned (see
lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> m, n;
    std::cin >> m >> n;
    std::cout << np::lcm(m, n) << "\n";
    return 0;
}
```

Input

```
[24, 36, 45, 20, 20, 120, 10, 0,  0, 30]
[20, 40, 75, 18, 19, 100,  0, 0, 30, 24]
```

Output

```
[120, 360, 225, 180, 380, 600,   0,   0,   0, 120]
```

## Complex numbers

### `real`

Return the real part, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> real(const tensor<std::complex<T>, Rank> &z);

template <class T, size_t Rank>
tensor<T, Rank> real(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with complex values.

Returns

* A light-weight object with the real part of each element in the tensor.
Non-complex types are treated as complex numbers with zero imaginary part
component. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << np::real(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[ 8, 12,  8,  5,  1, 12,  0, -1,  6,  0]
```

### `imag`

Return the imaginary part, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> imag(const tensor<std::complex<T>, Rank> &z);

template <class T, size_t Rank>
tensor<T, Rank> imag(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with complex values.

Returns

* A light-weight object with the imaginary part of each element in the tensor.
Non-complex types are treated as complex numbers with zero imaginary part
component. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << np::imag(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[-1, -3,  9,  0,  2,  1, -3,  9,  0, 10]
```

### `conj`

Return the complex conjugate, element-wise. The conjugate of a complex number
is obtained by changing the sign of its imaginary part.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> conj(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with the values whose complex conjugate is computed.

Returns

* A light-weight object with the complex conjugate of each element in the
tensor. Non-complex types are treated as complex numbers with zero imaginary
part component. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << np::conj(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[ 8+1i, 12+3i,  8-9i,  5-0i,  1-2i, 12-1i,  0+3i, -1-9i,  6-0i, 0-10i]
```

### `abs`

Return the absolute value, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> abs(const tensor<std::complex<T>, Rank> &z);
```

Parameters

* `z` A tensor-like object with the values whose absolute value is computed.

Returns

* A light-weight object with the absolute value of each element in the tensor.
This function does not create a new tensor, instead, an expression object is
returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << np::abs(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[8.0622577, 12.369317, 12.041595,         5,  2.236068, 12.041595,         3,
 9.0553851,         6,        10]
```

### `arg`

Return the phase angle (in radians) of a complex number, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> arg(const tensor<std::complex<T>, Rank> &z);

template <class T, size_t Rank>
tensor<T, Rank> arg(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with the values whose phase angle is computed.

Returns

* A light-weight object with the phase angle of each element in the tensor.
Non-complex types are treated as complex numbers with zero imaginary part
component. This function does not create a new tensor, instead, an expression
object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << np::arg(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[-0.12435499, -0.24497866,  0.84415399,           0,   1.1071487, 0.083141232,
  -1.5707963,   1.6814535,           0,   1.5707963]
```

## Clasification functions

### `isfinite`

Return whether $x$ is a finite value (neither infinite nor NaN), element-wise.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isfinite(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with floating point values.

Returns

* A light-weight object with each element set to `true` where `x` is finite
and `false` otherwise. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x{0., 0./0., 1., -1., np::sqrt(-1.), 1./0., -1./0.};
    std::cout << "x:\n" << x << "\n";
    std::cout << "isfinite(x):\n" << std::boolalpha << np::isfinite(x) << "\n";
    return 0;
}
```

Output

```
x:
[   0,  nan,    1,   -1,  nan,  inf, -inf]
isfinite(x):
[ true, false,  true,  true, false, false, false]
```

### `isinf`

Return whether $x$ is an infinity value (either positive infinity or negative
infinity), element-wise.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isinf(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with floating point values.

Returns

* A light-weight object with each element set to `true` where `x` is infinity
and `false` otherwise. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x{0., 0./0., 1., -1., np::sqrt(-1.), 1./0., -1./0.};
    std::cout << "x:\n" << x << "\n";
    std::cout << "isinf(x):\n" << std::boolalpha << np::isinf(x) << "\n";
    return 0;
}
```

Output

```
x:
[   0,  nan,    1,   -1,  nan,  inf, -inf]
isinf(x):
[false, false, false, false, false,  true,  true]
```

### `isnan`

Return whether $x$ is a NaN (Not a Number) value, element-wise. The NaN values
are used to identify undefined or non-representable values for floating-point
numbers, such as the square root of negative numbers or the result of $0/0$.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isnan(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with floating point values.

Returns

* A light-weight object with each element set to `true` where `x` is NaN and
`false` otherwise. This function does not create a new tensor, instead, an
expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x{0., 0./0., 1., -1., np::sqrt(-1.), 1./0., -1./0.};
    std::cout << "x:\n" << x << "\n";
    std::cout << "isnan(x):\n" << std::boolalpha << np::isnan(x) << "\n";
    return 0;
}
```

Output

```
x:
[   0,  nan,    1,   -1,  nan,  inf, -inf]
isnan(x):
[false,  true, false, false,  true, false, false]
```
