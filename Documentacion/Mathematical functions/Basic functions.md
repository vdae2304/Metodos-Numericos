# [Mathematical functions](./readme.md)

Defined in `numcpp/math.h`

- [Mathematical functions](#mathematical-functions)
  - [Basic functions](#basic-functions)
    - [`abs`](#abs)
    - [`fmod`](#fmod)
    - [`modf`](#modf)
    - [`fmax`](#fmax)
    - [`fmin`](#fmin)

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

* A light-weight object with the absolute value of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<int> x;
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

Return the floating-point remainder of $x/y$, element-wise. The floating-point remainder of $x/y$ is $x - ny$, where $n$ is the truncated value (i.e., rounded towards zero) of $x/y$.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> fmod(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> fmod(const tensor<T, Rank> &x,
                     const typename tensor<T, Rank>::value_type &y);

template <class T, size_t Rank>
tensor<T, Rank> fmod(const typename tensor<T, Rank>::value_type &x,
                     const tensor<T, Rank> &y)
```

Parameters

* `x` A tensor-like object with the values of the quotient numerator.
* `y` A tensor-like object with the values of the quotient denominator.

Returns

* A light-weight object with the remainder of $x/y$, element-wise. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
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

* A light-weight object with the integral and fractional parts of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    np::vector<std::pair<double, double>> y = np::modf(x);
    std::cout << np::unzip<0>(y) << "\n";
    std::cout << np::unzip<1>(y) << "\n";
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

Return the maximum value, element-wise. If one of the elements being compared is a NaN, then the non-NaN element is returned.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> fmax(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> fmax(const tensor<T, Rank> &x,
                     const typename tensor<T, Rank>::value_type &y);

template <class T, size_t Rank>
tensor<T, Rank> fmax(const typename tensor<T, Rank>::value_type &x,
                     const tensor<T, Rank> &y);
```

Parameters

* `x` A tensor-like object with floating-point or integer values.
* `y` A tensor-like object with floating-point or integer values.

Returns

* A light-weight object with the element-wise maximum. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
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

Return the minimum value, element-wise. If one of the elements being compared is a NaN, then the non-NaN element is returned.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> fmin(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> fmin(const tensor<T, Rank> &x,
                     const typename tensor<T, Rank>::value_type &y);

template <class T, size_t Rank>
tensor<T, Rank> fmin(const typename tensor<T, Rank>::value_type &x,
                     const tensor<T, Rank> &y);
```

Parameters

* `x` A tensor-like object with floating point or integer values.
* `y` A tensor-like object with floating-point or integer values.

Returns

* A light-weight object with the element-wise minimum. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
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
