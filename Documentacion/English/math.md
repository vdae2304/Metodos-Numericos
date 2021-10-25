# Mathematical functions

## Content

- [Constants](#Constants)
- [Trigonometric functions](#Trigonometric-functions)
- [Hyperbolic functions](#Hyperbolic-functions)
- [Exponential and logarithms](#Exponential-and-logarithms)
- [Power functions](#Power-functions)
- [Rounding](#Rounding)
- [Miscellaneous](#Miscellaneous)

<hr>

The `numcpp` header defines a set of mathematical functions, overloading them 
to be used on arrays and matrices.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> x = np::linspace(0.0, np::pi, 10);
    np::array<double> y = x * np::cos(np::sqrt(2.0)*x + 1.0);
    cout << y << "\n";
    return 0;
}
```

```
[Out] [0, 0.026901171, -0.28244468, -0.8268722, -1.3768435, -1.6530264, -1.4283364, -0.62063839, 0.655217, 2.0961889]
```

## Constants

### `inf`, `infty`, `infinity`

Floating point representation of (positive) infinite.

### `e`

*e = 2.71828182846...*.

### `pi`

*pi = 3.14159265359...*

### `golden`

*golden = 1.61803398875...*

## Trigonometric functions

### `cos`

Returns the cosine of an angle of *x* in radians.
```cpp
double cos(double x);
float cos(float x);
long double cos(long double x);

template <class T>
double cos(T x); 

template <class T>
array<T> cos(const array<T> &x);

template <class T>
matrix<T> cos(const matrix<T> &x);
```

### `sin`

Returns the sine of an angle of *x* in radians.
```cpp
double sin(double x);
float sin(float x);
long double sin(long double x);

template <class T>
double sin(T x); 

template <class T>
array<T> sin(const array<T> &x);

template <class T>
matrix<T> sin(const matrix<T> &x);
```

### `tan`

Returns the tangent of an angle of *x* in radians.
```cpp
double tan(double x);
float tan(float x);
long double tan(long double x);

template <class T>
double tan(T x); 

template <class T>
array<T> tan(const array<T> &x);

template <class T>
matrix<T> tan(const matrix<T> &x);
```

### `hypot`

Returns the hypotenuse of a right-angled triangle whose legs are *x* and *y*.
```cpp
double hypot(double x, double y);
float hypot(float x, float y);
long double hypot(long double x, long double y);

template <class T1, class T2>
double hypot(T1 x, T2 y);

template <class T>
array<T> hypot(const array<T> &x, const array<T> &y);

template <class T>
array<T> hypot(const array<T> &x, const T &y);

template <class T>
array<T> hypot(const T &x, const array<T> &y);

template <class T>
matrix<T> hypot(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> hypot(const matrix<T> &x, const T &y);

template <class T>
matrix<T> hypot(const T &x, const matrix<T> &y);
```

### `acos`

Returns the principal value of the arc cosine of *x*, expressed in radians.
```cpp
double acos(double x);
float acos(float x);
long double acos(long double x);

template <class T>
double acos(T x); 

template <class T>
array<T> acos(const array<T> &x);

template <class T>
matrix<T> acos(const matrix<T> &x);
```

### `asin`

Returns the principal value of the arc sine of *x*, expressed in radians.
```cpp
double asin(double x);
float asin(float x);
long double asin(long double x);

template <class T>
double asin(T x); 

template <class T>
array<T> asin(const array<T> &x);

template <class T>
matrix<T> asin(const matrix<T> &x);
```

### `atan`

Returns the principal value of the arc tangent of *x*, expressed in radians.
```cpp
double atan(double x);
float atan(float x);
long double atan(long double x);

template <class T>
double atan(T x); 

template <class T>
array<T> atan(const array<T> &x);

template <class T>
matrix<T> atan(const matrix<T> &x);
```

### `atan2`

Returns the principal value of the arc tangent of *y/x,* expressed in radians.
To compute the value, the function takes into account the sign of both 
arguments in order to determine the quadrant.
```cpp
double atan2(double x, double y);
float atan2(float x, float y);
long double atan2(long double x, long double y);

template <class T1, class T2>
double atan2(T1 x, T2 y);

template <class T>
array<T> atan2(const array<T> &y, const array<T> &x);

template <class T>
array<T> atan2(const array<T> &y, const T &x);

template <class T>
array<T> atan2(const T &y, const array<T> &x);

template <class T>
matrix<T> atan2(const matrix<T> &y, const matrix<T> &x); 

template <class T>
matrix<T> atan2(const matrix<T> &y, const T &x);

template <class T>
matrix<T> atan2(const T &y, const matrix<T> &x);
```

### `degrees`

Convert angles from radians to degrees.
```cpp
double degrees(double x);
float degrees(float x);
long double degrees(long double x);

template <class T>
double degrees(T x);

template <class T>
array<T> degrees(const array<T> &x);

template <class T>
matrix<T> degrees(const matrix<T> &x);
```

### `radians`

Convert angles from degrees to radians.
```cpp
double radians(double x);
float radians(float x);
long double radians(long double x);

template <class T>
double radians(T x);

template <class T>
array<T> radians(const array<T> &x);

template <class T>
matrix<T> radians(const matrix<T> &x);
```

## Hyperbolic functions

### `cosh`

Returns the hyperbolic cosine of *x*.
```cpp
double cosh(double x);
float cosh(float x);
long double cosh(long double x);

template <class T>
double cosh(T x); 

template <class T>
array<T> cosh(const array<T> &x);

template <class T>
matrix<T> cosh(const matrix<T> &x);
```

### `sinh`

Returns the hyperbolic sine of *x*.
```cpp
double sinh(double x);
float sinh(float x);
long double sinh(long double x);

template <class T>
double sinh(T x); 

template <class T>
array<T> sinh(const array<T> &x);

template <class T>
matrix<T> sinh(const matrix<T> &x);
```

### `tanh`

Returns the hyperbolic tangent of *x*.
```cpp
double tanh(double x);
float tanh(float x);
long double tanh(long double x);

template <class T>
double tanh(T x); 

template <class T>
array<T> tanh(const array<T> &x);

template <class T>
matrix<T> tanh(const matrix<T> &x);
```

### `acosh`

Returns the area hyperbolic cosine of *x*.
```cpp
double acosh(double x);
float acosh(float x);
long double acosh(long double x);

template <class T>
double acosh(T x); 

template <class T>
array<T> acosh(const array<T> &x);

template <class T>
matrix<T> acosh(const matrix<T> &x);
```

### `asinh`

Returns the area hyperbolic sine of *x*.
```cpp
double asinh(double x);
float asinh(float x);
long double asinh(long double x);

template <class T>
double asinh(T x); 

template <class T>
array<T> asinh(const array<T> &x);

template <class T>
matrix<T> asinh(const matrix<T> &x);
```

### `atanh`

Returns the area hyperbolic tangent of *x*.
```cpp
double atanh(double x);
float atanh(float x);
long double atanh(long double x);

template <class T>
double atanh(T x); 

template <class T>
array<T> atanh(const array<T> &x);

template <class T>
matrix<T> atanh(const matrix<T> &x);
```

## Exponential and logarithms

### `exp`

Returns the base-*e* exponential function of *x*, which is *e* raised to the 
power *x*.
```cpp
double exp(double x);
float exp(float x);
long double exp(long double x);

template <class T>
double exp(T x);

template <class T>
array<T> exp(const array<T> &x);

template <class T>
matrix<T> exp(const matrix<T> &x);
```

### `exp2`

Returns the base-2 exponential function of *x*, which is 2 raised to the power 
*x*.
```cpp
double exp2(double x);
float exp2(float x);
long double exp2(long double x);

template <class T>
double exp2(T x);

template <class T>
array<T> exp2(const array<T> &x);

template <class T>
matrix<T> exp2(const matrix<T> &x);
```

### `log`

Returns the natural (base *e*) logarithm of *x*.
```cpp
double log(double x);
float log(float x);
long double log(long double x);

template <class T>
double log(T x);

template <class T>
array<T> log(const array<T> &x);

template <class T>
matrix<T> log(const matrix<T> &x);
```

### `log2`

Returns the binary (base-2) logarithm of *x*.
```cpp
double log2(double x);
float log2(float x);
long double log2(long double x);

template <class T>
double log2(T x);

template <class T>
array<T> log2(const array<T> &x);

template <class T>
matrix<T> log2(const matrix<T> &x);
```

### `log10`

Returns the common (base-10) logarithm of *x*.
```cpp
double log10(double x);
float log10(float x);
long double log10(long double x);

template <class T>
double log10(T x);

template <class T>
array<T> log10(const array<T> &x);

template <class T>
matrix<T> log10(const matrix<T> &x);
```

## Power functions

### `pow`

Returns *x* raised to the power *y*.
```cpp
double pow(double x, double y);
float pow(float x, float y);
long double pow(long double x, long double y);

template <class T1, class T2>
double pow(T1 x, T2 y);

template <class T>
array<T> pow(const array<T> &x, const array<T> &y);

template <class T>
array<T> pow(const array<T> &x, const T &y);

template <class T>
array<T> pow(const T &x, const array<T> &y);

template <class T>
matrix<T> pow(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> pow(const matrix<T> &x, const T &y);

template <class T>
matrix<T> pow(const T &x, const matrix<T> &y);
```

### `square`

Returns the square of *x*.
```cpp
template <class T>
T square(T x);

template <class T>
array<T> square(const array<T> &x);

template <class T>
matrix<T> square(const matrix<T> &x);
```

### `sqrt`

Returns the square root of *x*.
```cpp
double sqrt(double x);
float sqrt(float x);
long double sqrt(long double x);

template <class T>
double sqrt(T x);  

template <class T>
array<T> sqrt(const array<T> &x);

template <class T>
matrix<T> sqrt(const matrix<T> &x);
```

### `cbrt`

Returns the cubic root of *x*.
```cpp
double cbrt(double x);
float cbrt(float x);
long double cbrt(long double x);

template <class T>
double cbrt(T x);

template <class T>
array<T> cbrt(const array<T> &x);

template <class T>
matrix<T> cbrt(const matrix<T> &x);
```

## Rounding

### `ceil`

Rounds *x* upward, returning the smallest integral value that is not less than 
*x*.
```cpp
double ceil(double x);
float ceil(float x);
long double ceil(long double x);

template <class T>
double ceil(T x);

template <class T>
array<T> ceil(const array<T> &x);

template <class T>
matrix<T> ceil(const matrix<T> &x);
```

### `floor`

Rounds *x* downward, returning the largest integral value that is not greater 
than *x*.
```cpp
double floor(double x);
float floor(float x);
long double floor(long double x);

template <class T>
double floor(T x);

template <class T>
array<T> floor(const array<T> &x);

template <class T>
matrix<T> floor(const matrix<T> &x);
```

### `round`

Returns the integral value that is nearest to *x*.
```cpp
double round(double x);
float round(float x);
long double round(long double x);

template <class T>
double round(T x);

template <class T>
array<T> round(const array<T> &x);

template <class T>
matrix<T> round(const matrix<T> &x);
```

Rounds *x* to the given number of decimal places.
```cpp
double round(double x, size_t decimals);
float round(float x, size_t decimals);
long double round(long double x, size_t decimals);

template <class T>
array<T> round(const array<T> &x, size_t decimals);

template <class T>
matrix<T> round(const matrix<T> &x, size_t decimals);
```

### `trunc`

Rounds *x* toward zero, returning the nearest integral value that is not larger 
in magnitude than *x*.
```cpp
double trunc(double x);
float trunc(float x);
long double trunc(long double x);

template <class T>
double trunc(T x);

template <class T>
array<T> trunc(const array<T> &x);

template <class T>
matrix<T> trunc(const matrix<T> &x);
```

## Miscellaneous

### `abs`

Returns the absolute value of *x*: *|x|*.
```cpp
double abs(double x);
float abs(float x);
long double abs(long double x);

template <class T>
double abs(T x);

template <class T>
array<T> abs(const array<T> &x);

template <class T>
matrix<T> abs(const matrix<T> &x);
```

### `sign`

The sign function returns -1 if *x* is less than zero, 1 if *x* is greater than
zero and 0 if *x* is equal to zero.
```cpp
template <class T>
int sign(T x)

template <class T>
array<int> sign(const array<T> &x);

template <class T>
matrix<int> sign(const matrix<T> &x);
``` 

### `gcd`

Returns the greatest common divisor of the integers *m* and *n*.
```cpp
int gcd(int m, int n);
long gcd(long m, long n);
long long gcd(long long m, long long n);
unsigned gcd(unsigned m, unsigned n);
unsigned long gcd(unsigned long m, unsigned long n);
unsigned long long gcd(unsigned long long m, unsigned long long n);

template <class T>
array<T> gcd(const array<T> &m, const array<T> &n);

template <class T>
array<T> gcd(const array<T> &m, const T &n);

template <class T>
array<T> gcd(const T &m, const array<T> &n);

template <class T>
matrix<T> gcd(const matrix<T> &m, const matrix<T> &n);

template <class T>
matrix<T> gcd(const matrix<T> &m, const T &n);

template <class T>
matrix<T> gcd(const T &m, const matrix<T> &n);
```

### `lcm`

Returns the least common multiple of the integers *m* and *n*.
```cpp
int lcm(int m, int n);
long lcm(long m, long n);
long long lcm(long long m, long long n);
unsigned lcm(unsigned m, unsigned n);
unsigned long lcm(unsigned long m, unsigned long n);
unsigned long long lcm(unsigned long long m, unsigned long long n);

template <class T>
array<T> lcm(const array<T> &m, const array<T> &n);

template <class T>
array<T> lcm(const array<T> &m, const T &n);

template <class T>
array<T> lcm(const T &m, const array<T> &n);

template <class T>
matrix<T> lcm(const matrix<T> &m, const matrix<T> &n);

template <class T>
matrix<T> lcm(const matrix<T> &m, const T &n);

template <class T>
matrix<T> lcm(const T &m, const matrix<T> &n);
```

### `maximum`

Returns the element-wise maximum of two arrays/matrices.
```cpp
template <class T>
array<T> maximum(const array<T> &x, const array<T> &y);

template <class T>
array<T> maximum(const array<T> &x, const T &y);

template <class T>
array<T> maximum(const T &x, const array<T> &y);

template <class T>
matrix<T> maximum(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> maximum(const matrix<T> &x, const T &y);

template <class T>
matrix<T> maximum(const T &x, const matrix<T> &y);
```

### `minimum`

Returns the element-wise minimum of two arrays/matrices.
```cpp
template <class T>
array<T> minimum(const array<T> &x, const array<T> &y);

template <class T>
array<T> minimum(const array<T> &x, const T &y);

template <class T>
array<T> minimum(const T &x, const array<T> &y);

template <class T>
matrix<T> minimum(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> minimum(const matrix<T> &x, const T &y);

template <class T>
matrix<T> minimum(const T &x, const matrix<T> &y);
```