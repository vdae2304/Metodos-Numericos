# Creation routines

Defined in `numcpp/routines.h`

- [Creation routines](#creation-routines)
  - [Tensor creation routines](#tensor-creation-routines)
    - [`empty`](#empty)
    - [`empty_like`](#empty_like)
    - [`zeros`](#zeros)
    - [`zeros_like`](#zeros_like)
    - [`ones`](#ones)
    - [`ones_like`](#ones_like)
    - [`full`](#full)
    - [`full_like`](#full_like)
  - [Numerical ranges](#numerical-ranges)
    - [`arange`](#arange)
    - [`linspace`](#linspace)
    - [`logspace`](#logspace)
    - [`geomspace`](#geomspace)
  - [Building matrices](#building-matrices)
    - [`eye`](#eye)
    - [`diag`](#diag)
    - [`tril`](#tril)
    - [`triu`](#triu)
    - [`vander`](#vander)

## Tensor creation routines

### `empty`

Return a new tensor of given shape without initializing entries.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> empty(const shape_t<Rank> &shape);
```

Template parameters

* `T` Type of the elements contained in the tensor.
* `Rank` Dimension of the tensor.

Parameters

* `shape` Shape of the empty tensor.

Returns

* A tensor of uninitialized data with the given shape.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Return array<int> of shape (10,).
    auto arr = np::empty<int, 1>(10);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Return matrix<int> of shape (3, 4).
    auto mat = np::empty<int, 2>({3, 4});
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Return tensor<int, 3> of shape (2, 3, 4).
    auto cube = np::empty<int, 3>({2, 3, 4});
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Possible output

```
(10,)
[491264784,       586, 491258192,       586,         0,         0,         0,
         0,         0,         0]
(3, 4)
[[ 491284272,        586,  491263952,        586],
 [         0,          0,          0,          0],
 [         0,          0, 1375731794,      51002]]
(2, 3, 4)
[[[ 491284272,        586,  491263952,        586],
  [ 491284272,        586,  491258192,        586],
  [1414088771, 1431585605, 1094534482,  875971661]],

 [[1330794496, 1397966147, 1230983759, 1414415684],
  [1162430025, 1850293586,  913073524, 1631985716],
  [2037148013, 1293956640, 1818584175,  943010080]]]
```

### `empty_like`

Return a new tensor with the same shape and type as a given tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> empty_like(const tensor<T, Rank> &like);
```

Parameters

* `like` Tensor-like object defining the shape and data type.

Returns

* A tensor of uninitialized data with the same shape and type as `like`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::array<int> like1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto arr = np::empty_like(like1);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like2{{0, 1, 2, 3},
                          {4, 5, 6, 7},
                          {8, 9, 10, 11}};
    auto mat = np::empty_like(like2);
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Type deduced to int. Shape deduced to (2, 3, 4)
    np::tensor<int, 3> like3{{{0, 1, 2, 3},
                              {4, 5, 6, 7},
                              {8, 9, 10, 11}},
                             {{12, 13, 14, 15},
                              {16, 17, 18, 19},
                              {20, 21, 22, 23}}};
    auto cube = np::empty_like(like3);
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Possible output

```
(10,)
[223373104,       554, 223352784,       554,         0,         0,         0,
         0,         0,         0]
(3, 4)
[[ 223373104,        554,  223352784,        554],
 [1162430025, 1850293586,  913073524, 1631985716],
 [2037148013, 1293956640, 1818584175,  943010080]]
(2, 3, 4)
[[[ 223373104,        554,  223352784,        554],
  [1348221507, 1919381362, 1176530273, 1936026729],
  [ 947398688, 1342187830, 1919381362,  911699297]],

 [[1026700084, 1348221507, 1919381362, 1176530273],
  [1936026729, 1297305600, 1819632751, 1952534629],
  [ 977485160, 1702057308, 1985770354, 1600479588]]]
```

### `zeros`

Return a new tensor of given shape filled with zeros.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> zeros(const shape_t<Rank> &shape);
```

Template parameters

* `T` Type of the elements contained in the tensor.
* `Rank` Dimension of the tensor.

Parameters

* `shape` Shape of the new tensor.

Returns

* A tensor of zeros with the given shape.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Return array<int> of shape (10,) filled with zeros.
    auto arr = np::zeros<int, 1>(10);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Return matrix<int> of shape (3, 4) filled with zeros.
    auto mat = np::zeros<int, 2>({3, 4});
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Return tensor<int, 3> of shape (2, 3, 4) filled with zeros.
    auto cube = np::zeros<int, 3>({2, 3, 4});
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Output

```
(10,)
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
(3, 4)
[[0, 0, 0, 0],
 [0, 0, 0, 0],
 [0, 0, 0, 0]]
(2, 3, 4)
[[[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]],

 [[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]]]
```

### `zeros_like`

Return a new tensor of zeros with the same shape and type as a given tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> zeros_like(const tensor<T, Rank> &like);
```

Parameters

* `like` Tensor-like object defining the shape and data type.

Returns

* A tensor of zeros with the same shape and type as `like`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::array<int> like1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto arr = np::zeros_like(like1);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like2{{0, 1, 2, 3},
                          {4, 5, 6, 7},
                          {8, 9, 10, 11}};
    auto mat = np::zeros_like(like2);
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Type deduced to int. Shape deduced to (2, 3, 4)
    np::tensor<int, 3> like3{{{0, 1, 2, 3},
                              {4, 5, 6, 7},
                              {8, 9, 10, 11}},
                             {{12, 13, 14, 15},
                              {16, 17, 18, 19},
                              {20, 21, 22, 23}}};
    auto cube = np::zeros_like(like3);
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Output

```
(10,)
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
(3, 4)
[[0, 0, 0, 0],
 [0, 0, 0, 0],
 [0, 0, 0, 0]]
(2, 3, 4)
[[[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]],

 [[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]]]
```

### `ones`

Return a new tensor of given shape filled with ones.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> ones(const shape_t<Rank> &shape);
```

Template parameters

* `T` Type of the elements contained in the tensor.
* `Rank` Dimension of the tensor.

Parameters

* `shape` Shape of the new tensor.

Returns

* A tensor of ones with the given shape.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Return array<int> of shape (10,) filled with ones.
    auto arr = np::ones<int, 1>(10);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Return matrix<int> of shape (3, 4) filled with ones.
    auto mat = np::ones<int, 2>({3, 4});
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Return tensor<int, 3> of shape (2, 3, 4) filled with ones.
    auto cube = np::ones<int, 3>({2, 3, 4});
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Output

```
(10,)
[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
(3, 4)
[[1, 1, 1, 1],
 [1, 1, 1, 1],
 [1, 1, 1, 1]]
(2, 3, 4)
[[[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]],

 [[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]]]
```

### `ones_like`

Return a new tensor of ones with the same shape and type as a given tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> ones_like(const tensor<T, Rank> &like);
```

Parameters

* `like` Tensor-like object defining the shape and data type.

Returns

* A tensor of ones with the same shape and type as `like`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::array<int> like1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto arr = np::ones_like(like1);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like2{{0, 1, 2, 3},
                          {4, 5, 6, 7},
                          {8, 9, 10, 11}};
    auto mat = np::ones_like(like2);
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Type deduced to int. Shape deduced to (2, 3, 4)
    np::tensor<int, 3> like3{{{0, 1, 2, 3},
                              {4, 5, 6, 7},
                              {8, 9, 10, 11}},
                             {{12, 13, 14, 15},
                              {16, 17, 18, 19},
                              {20, 21, 22, 23}}};
    auto cube = np::ones_like(like3);
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Output

```
(10,)
[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
(3, 4)
[[1, 1, 1, 1],
 [1, 1, 1, 1],
 [1, 1, 1, 1]]
(2, 3, 4)
[[[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]],

 [[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]]]
```

### `full`

Return a new tensor of given shape filled with `val`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> full(const shape_t<Rank> &shape, const T &val);
```

Template parameters

* `T` Type of the elements contained in the tensor.
* `Rank` Dimension of the tensor.

Parameters

* `shape` Shape of the new tensor.
* `val` Fill value.

Returns

* A tensor of `val` with the given shape.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Return array<int> of shape (10,) filled with 10.
    auto arr = np::full<int, 1>(10, 10);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Return matrix<int> of shape (3, 4) filled with 10.
    auto mat = np::full<int, 2>({3, 4}, 10);
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Return tensor<int, 3> of shape (2, 3, 4) filled with 10.
    auto cube = np::full<int, 3>({2, 3, 4}, 10);
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Output

```
(10,)
[10, 10, 10, 10, 10, 10, 10, 10, 10, 10]
(3, 4)
[[10, 10, 10, 10],
 [10, 10, 10, 10],
 [10, 10, 10, 10]]
(2, 3, 4)
[[[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]],

 [[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]]]
```

### `full_like`

Return a new tensor with the same shape and type as a given tensor filled with
`val`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> full_like(
    const tensor<T, Rank> &like,
    const typename tensor<T, Rank>::value_type &val
);
```

Parameters

* `like` Tensor-like object defining the shape and data type.
* `val` Fill value.

Returns

* A tensor of `val` with the same shape and type as `like`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::array<int> like1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto arr = np::full_like(like1, 10);
    std::cout << arr.shape() << "\n" << arr << "\n";
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like2{{0, 1, 2, 3},
                          {4, 5, 6, 7},
                          {8, 9, 10, 11}};
    auto mat = np::full_like(like2, 10);
    std::cout << mat.shape() << "\n" << mat << "\n";
    // Type deduced to int. Shape deduced to (2, 3, 4)
    np::tensor<int, 3> like3{{{0, 1, 2, 3},
                              {4, 5, 6, 7},
                              {8, 9, 10, 11}},
                             {{12, 13, 14, 15},
                              {16, 17, 18, 19},
                              {20, 21, 22, 23}}};
    auto cube = np::full_like(like3, 10);
    std::cout << cube.shape() << "\n" << cube << "\n";
    return 0;
}
```

Output

```
(10,)
[10, 10, 10, 10, 10, 10, 10, 10, 10, 10]
(3, 4)
[[10, 10, 10, 10],
 [10, 10, 10, 10],
 [10, 10, 10, 10]]
(2, 3, 4)
[[[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]],

 [[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]]]
```

## Numerical ranges

### `arange`

Return evenly spaced values within a given interval.

Values are generated within the half-open interval [`start`, `stop`), i.e., the
interval including `start` but excluding `stop`, with spacing between values
given by `step`.
```cpp
template <class T>
tensor<T, 1> arange(const T &stop);

template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
arange(const T &start, const U &stop);

template <class T, class U, class V>
tensor<typename std::common_type<T, U, V>::type, 1>
arange(const T &start, const U &stop, const V &step);
```

Parameters

* `start` Start of interval. Defaults to 0 if not provided.
* `stop` End of interval.
* `step` Spacing between values. This is the distance between two adjacent
values. Defaults to 1 if not provided.

Returns

* A light-weight object with evenly spaced values. This function does not
create a new tensor, instead, it returns an expression object with evenly
spaced values in a given interval.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Return an expression equivalent to array<int>
    std::cout << "arange(10):\n" << np::arange(10) << "\n";
    // Return an expression equivalent to array<int>
    std::cout << "arange(10, 20):\n" << np::arange(10, 20) << "\n";
    // Return an expression equivalent to array<int>
    std::cout << "arange(1, 20, 3):\n" << np::arange(1, 20, 3) << "\n";
    // Return an expression equivalent to array<int>
    std::cout << "arange(10, 0, -1):\n" << np::arange(10, 0, -1) << "\n";
    // Return an expression equivalent to array<double>
    std::cout << "arange(0, 5, 0.5):\n" << np::arange(0, 5, 0.5) << "\n";
    return 0;
}
```

Output

```
arange(10):
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
arange(10, 20):
[10, 11, 12, 13, 14, 15, 16, 17, 18, 19]
arange(1, 20, 3):
[ 1,  4,  7, 10, 13, 16, 19]
arange(10, 0, -1):
[10,  9,  8,  7,  6,  5,  4,  3,  2,  1]
arange(0, 5, 0.5):
[  0, 0.5,   1, 1.5,   2, 2.5,   3, 3.5,   4, 4.5]
```

### `linspace`

Return evenly spaced numbers over a specified interval.

Return `num` evenly spaced samples, calculated over the interval
[`start`, `stop`]. The endpoint of the interval can optionally be excluded.
```cpp
template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
linspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);
```

Parameters

* `start` The starting value of the sequence.
* `stop` The end value of the sequence.
* `num` Number of samples to generate. Default is 50.
* `endpoint` If `true`, `stop` is the last sample. Otherwise, it is not
included. Default is `true`.

Returns

* A light-weight object with equally spaced samples. This function does not
create a new tensor, instead, it returns an expression object with equally
spaced samples over an interval.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Return an expression equivalent to array<double>
    std::cout << "linspace(0., 1., num=10):\n";
    std::cout << np::linspace(0., 1., 10) << "\n";
    // Return an expression equivalent to array<double>
    std::cout << "linspace(-1., 1., num=10):\n";
    std::cout << np::linspace(-1., 1., 10) << "\n";
    // Return an expression equivalent to array<double>
    std::cout << "linspace(0., 1., num=10, endpoint=false):\n";
    std::cout << np::linspace(0., 1., 10, false) << "\n";
    return 0;
}
```

Output

```
linspace(0., 1., num=10):
[         0, 0.11111111, 0.22222222, 0.33333333, 0.44444444, 0.55555556,
 0.66666667, 0.77777778, 0.88888889,          1]
linspace(-1., 1., num=10):
[         -1, -0.77777778, -0.55555556, -0.33333333, -0.11111111,  0.11111111,
  0.33333333,  0.55555556,  0.77777778,           1]
linspace(0., 1., num=10, endpoint=false):
[  0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
```

### `logspace`

Return numbers spaced evenly on a log scale.

In linear space, the sequence starts at `pow(base, start)` (`base` to the power
of `start`) and ends with `pow(base, stop)`.
```cpp
template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
logspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);

template <class T, class U, class V>
tensor<typename std::common_type<T, U, V>::type, 1>
logspace(
    const T &start, const U &stop, size_t num, bool endpoint, const V &base
);
```

Parameters

* `start` `pow(base, start)` is the starting value of the sequence.
* `stop` `pow(base, stop)` is the final value of the sequence.
* `num` Number of samples to generate. Default is 50.
* `endpoint` If `true`, `stop` is the last sample. Otherwise, it is not
included. Default is `true`.
* `base` The base of the log space. Default is 10.

Returns

* A light-weight object with equally spaced samples on a log scale. This
function does not create a new tensor, instead, it returns an expression object
with equally spaced samples on a log scale.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    // Return an expression equivalent to array<double>
    std::cout << "logspace(-3., 3., num=7):\n";
    std::cout << np::logspace(-3., 3., 7) << "\n";
    // Return an expression equivalent to array<double>
    std::cout << "logspace(-3., 3., num=7, endpoint=false):\n";
    std::cout << np::logspace(-3., 3., 7, false) << "\n";
    // Return an expression equivalent to array<double>
    std::cout << "logspace(1., 10., num=10, endpoint=true, base=2):\n";
    std::cout << np::logspace(1., 10., 10, true, 2) << "\n";
    // Return an expression equivalent to array<std::complex<double>>
    std::complex<double> base = -1;
    std::cout << "logspace(0., 2., num=10, endpoint=false, base=-1.+0i):\n";
    std::cout << np::logspace(0., 2., 10, false, base) << "\n";
    return 0;
}
```

Output

```
logspace(-3., 3., num=7):
[0.001,  0.01,   0.1,     1,    10,   100,  1000]
logspace(-3., 3., num=7, endpoint=false):
[       0.001, 0.0071968567,  0.051794747,   0.37275937,    2.6826958,
    19.306977,    138.94955]
logspace(1., 10., num=10, endpoint=true, base=2):
[   2,    4,    8,   16,   32,   64,  128,  256,  512, 1024]
logspace(0., 2., num=10, endpoint=false, base=-1.+0i):
[                   1+0i,  0.80901699+0.58778525i,  0.30901699+0.95105652i,
 -0.30901699+0.95105652i, -0.80901699+0.58778525i,       -1+1.2246064e-16i,
 -0.80901699-0.58778525i, -0.30901699-0.95105652i,  0.30901699-0.95105652i,
  0.80901699-0.58778525i]
```

### `geomspace`

Return numbers spaced evenly on a log scale (a geometric progression).

This is similar to logspace, but with endpoints specified directly. Each output
sample is a constant multiple of the previous.
```cpp
template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
geomspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);
```

Parameters

* `start` The starting value of the sequence.
* `stop` The final value of the sequence.
* `num` Number of samples to generate. Default is 50.
* `endpoint` If `true`, `stop` is the last sample. Otherwise, it is not
included. Default is `true`.

Returns

* A light-weight object with equally spaced samples on a log scale. This
function does not create a new tensor, instead, it returns an expression object
with equally spaced samples on a log scale.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    // Return an expression equivalent to array<double>
    std::cout << "geomspace(0.001., 1000., num=7):\n";
    std::cout << np::geomspace(0.001, 1000., 7) << "\n";
    // Return an expression equivalent to array<double>
    std::cout << "geomspace(0.001., 1000., num=7, endpoint=false):\n";
    std::cout << np::geomspace(0.001, 1000., 7, false) << "\n";
    // Return an expression equivalent to array<double>
    std::cout << "geomspace(2., 1024., num=10):\n";
    std::cout << np::geomspace(2., 1024., 10) << "\n";
    // Return an expression equivalent to array<std::complex<double>>
    // For complex types, the sequence follows a logarithmic spiral in the
    // complex plane.
    std::complex<double> one = 1;
    std::cout << "geomspace(-1.+0i, 1.+0i, num=10):\n";
    std::cout << np::geomspace(-one, one, 10) << "\n";
    return 0;
}
```

Output

```
geomspace(0.001., 1000., num=7):
[0.001,  0.01,   0.1,     1,    10,   100,  1000]
geomspace(0.001., 1000., num=7, endpoint=false):
[       0.001, 0.0071968567,  0.051794747,   0.37275937,    2.6826958,
    19.306977,    138.94955]
geomspace(2., 1024., num=10):
[   2,    4,    8,   16,   32,   64,  128,  256,  512, 1024]
geomspace(-1.+0i, 1.+0i, num=10):
[      -1-1.2246064e-16i, -0.93969262-0.34202014i, -0.76604444-0.64278761i,
         -0.5-0.8660254i, -0.17364818-0.98480775i,  0.17364818-0.98480775i,
          0.5-0.8660254i,  0.76604444-0.64278761i,  0.93969262-0.34202014i,
                    1+0i]
```

## Building matrices

### `eye`

Return a matrix with ones on the diagonal and zeros elsewhere.
```cpp
template <class T>
tensor<T, 2> eye(size_t n);

template <class T>
tensor<T, 2> eye(size_t m, size_t n, ptrdiff_t k = 0);
```

Template parameters

* `T` Type of the elements contained in the tensor.

Parameters

* `m` Number of rows. If not provided, defaults to `n`.
* `n` Number of columns.
* `k` Offset of the diagonal from the main diagonal. A positive value refers to
an upper diagonal and a negative value refers to a lower diagonal. Defaults to
main diagonal (0).

Returns

* A light-weight object with ones on the diagonal and zeros elsewhere. This
function, does not create a new tensor, instead, it returns an expression
object with ones on the diagonal and zeros elsewhere.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    std::cout << "eye(4):\n" << np::eye<int>(4) << "\n";
    std::cout << "eye(4, 6):\n" << np::eye<int>(4, 6) << "\n";
    std::cout << "eye(4, 4, 1):\n" << np::eye<int>(4, 4, 1) << "\n";
    std::cout << "eye(4, 4, -1):\n" << np::eye<int>(4, 4, -1) << "\n";
    return 0;
}
```

Output

```
eye(4):
[[1, 0, 0, 0],
 [0, 1, 0, 0],
 [0, 0, 1, 0],
 [0, 0, 0, 1]]
eye(4, 6):
[[1, 0, 0, 0, 0, 0],
 [0, 1, 0, 0, 0, 0],
 [0, 0, 1, 0, 0, 0],
 [0, 0, 0, 1, 0, 0]]
eye(4, 4, 1):
[[0, 1, 0, 0],
 [0, 0, 1, 0],
 [0, 0, 0, 1],
 [0, 0, 0, 0]]
eye(4, 4, -1):
[[0, 0, 0, 0],
 [1, 0, 0, 0],
 [0, 1, 0, 0],
 [0, 0, 1, 0]]
```

### `diag`

Extract a diagonal or construct a diagonal matrix.

If `a` is a 2-dimensional tensor, returns the diagonal of `a` with the given
offset, i.e., an array with the elements of the form `a(i, i + k)`. If `a` is a
1-dimensional tensor, returns a matrix with `a` on the `k`-th diagonal.
```cpp
template <class T>
tensor<T, 1> diag(const tensor<T, 2> &a, ptrdiff_t k = 0);

template <class T>
tensor<T, 2> diag(const tensor<T, 1> &a, ptrdiff_t k = 0);
```

Parameters

* `a` Matrix from which the diagonal is taken or array with the elements that
will be on the diagonal.
* `k` Offset of the diagonal from the main diagonal. A positive value refers
to an upper diagonal and a negative value refers to a lower diagonal. Defaults
to main diagonal (0).

Returns

* The extracted diagonal or the constructed diagonal matrix. This function does
not create a new tensor, instead, it returns a readonly view with the extracted
diagonal (when `a` is 2-dimensional) or with the constructed diagonal matrix
(when `a` is 1-dimensional).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "diag(a):\n" << np::diag(a) << "\n";
    std::cout << "diag(a, k=1):\n" << np::diag(a, 1) << "\n";
    std::cout << "diag(a, k=-1):\n" << np::diag(a, -1) << "\n";
    return 0;
}
```

Input

```
[[ 1,  2,  3,  4],
 [ 5,  6,  7,  8],
 [ 9, 10, 11, 12],
 [13, 14, 15, 16]]
```

Output

```
diag(a):
[ 1,  6, 11, 16]
diag(a, k=1):
[ 2,  7, 12]
diag(a, k=-1):
[ 5, 10, 15]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << "diag(a):\n" << np::diag(a) << "\n";
    std::cout << "diag(a, k=1):\n" << np::diag(a, 1) << "\n";
    std::cout << "diag(a, k=-1):\n" << np::diag(a, -1) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4]
```

Output

```
diag(a):
[[1, 0, 0, 0],
 [0, 2, 0, 0],
 [0, 0, 3, 0],
 [0, 0, 0, 4]]
diag(a, k=1):
[[0, 1, 0, 0, 0],
 [0, 0, 2, 0, 0],
 [0, 0, 0, 3, 0],
 [0, 0, 0, 0, 4],
 [0, 0, 0, 0, 0]]
diag(a, k=-1):
[[0, 0, 0, 0, 0],
 [1, 0, 0, 0, 0],
 [0, 2, 0, 0, 0],
 [0, 0, 3, 0, 0],
 [0, 0, 0, 4, 0]]
```

### `tril`

Return the lower triangle of a matrix.
```cpp
template <class T>
tensor<T, 2> tril(const tensor<T, 2> &a, ptrdiff_t k = 0);
```

Parameters

* `a` Matrix from which the lower triangle is taken.
* `k` Diagonal above which to zero elements. A positive value refers to an
upper diagonal and a negative value refers to a lower diagonal. Defaults to
main diagonal (0).

Returns

* Lower triangle of `a`. This function does not create a new tensor, instead,
it returns a readonly view with elements above the `k`-th diagonal zeroed.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "tril(a):\n" << np::tril(a) << "\n";
    std::cout << "tril(a, k=1):\n" << np::tril(a, 1) << "\n";
    std::cout << "tril(a, k=-1):\n" << np::tril(a, -1) << "\n";
    return 0;
}
```

Input

```
[[ 1,  2,  3,  4],
 [ 5,  6,  7,  8],
 [ 9, 10, 11, 12],
 [13, 14, 15, 16]]
```

Output

```
tril(a):
[[ 1,  0,  0,  0],
 [ 5,  6,  0,  0],
 [ 9, 10, 11,  0],
 [13, 14, 15, 16]]
tril(a, k=1):
[[ 1,  2,  0,  0],
 [ 5,  6,  7,  0],
 [ 9, 10, 11, 12],
 [13, 14, 15, 16]]
tril(a, k=-1):
[[ 0,  0,  0,  0],
 [ 5,  0,  0,  0],
 [ 9, 10,  0,  0],
 [13, 14, 15,  0]]
```

### `triu`

Return the upper triangle of a matrix.
```cpp
template <class T>
tensor<T, 2> triu(const tensor<T, 2> &a, ptrdiff_t k = 0);
```

Parameters

* `a` Matrix from which the upper triangle is taken.
* `k` Diagonal below which to zero elements. A positive value refers to an
upper diagonal and a negative value refers to a lower diagonal. Defaults to
main diagonal (0).

Returns

* Upper triangle of `a`. This function does not create a new tensor, instead,
it returns a readonly view with elements below the `k`-th diagonal zeroed.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "triu(a):\n" << np::triu(a) << "\n";
    std::cout << "triu(a, k=1):\n" << np::triu(a, 1) << "\n";
    std::cout << "triu(a, k=-1):\n" << np::triu(a, -1) << "\n";
    return 0;
}
```

Input

```
[[ 1,  2,  3,  4],
 [ 5,  6,  7,  8],
 [ 9, 10, 11, 12],
 [13, 14, 15, 16]]
```

Output

```
triu(a):
[[ 1,  2,  3,  4],
 [ 0,  6,  7,  8],
 [ 0,  0, 11, 12],
 [ 0,  0,  0, 16]]
triu(a, k=1):
[[ 0,  2,  3,  4],
 [ 0,  0,  7,  8],
 [ 0,  0,  0, 12],
 [ 0,  0,  0,  0]]
triu(a, k=-1):
[[ 1,  2,  3,  4],
 [ 5,  6,  7,  8],
 [ 0, 10, 11, 12],
 [ 0,  0, 15, 16]]
```

### `vander`

Generate a Vandermonde matrix.

The columns of the output matrix are powers of the input vector. The order of
the powers is determined by the `increasing` boolean argument.
```cpp
template <class T>
tensor<T, 2> vander(const tensor<T, 1> &x);

template <class T>
tensor<T, 2> vander(const tensor<T, 1> &x, size_t N, bool increasing = false);
```

Parameters

* `x` One-dimensional input tensor.
* `N` Number of columns in the output. If `N` is not specified, a square matrix
is returned (`N = x.size()`).
* `increasing` Order of the powers of the columns. If `true`, the powers
increase from left to right. If `false`, they are reversed. Default is `false`.

Returns

* Vandermonde matrix.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> x;
    std::cin >> x;
    std::cout << "vander(x):\n";
    std::cout << np::vander(x) << "\n";
    std::cout << "vander(x, N=3):\n";
    std::cout << np::vander(x, 3) << "\n";
    std::cout << "vander(x, increasing=true):\n";
    std::cout << np::vander(x, x.size(), true) << "\n";
    std::cout << "vander(x, N=3, increasing=true):\n";
    std::cout << np::vander(x, 3, true) << "\n";
    return 0;
}

```

Input

```
[1, 2, 3, 4, 5]
```

Output

```
vander(x):
[[  1,   1,   1,   1,   1],
 [ 16,   8,   4,   2,   1],
 [ 81,  27,   9,   3,   1],
 [256,  64,  16,   4,   1],
 [625, 125,  25,   5,   1]]
vander(x, N=3):
[[ 1,  1,  1],
 [ 4,  2,  1],
 [ 9,  3,  1],
 [16,  4,  1],
 [25,  5,  1]]
vander(x, increasing=true):
[[  1,   1,   1,   1,   1],
 [  1,   2,   4,   8,  16],
 [  1,   3,   9,  27,  81],
 [  1,   4,  16,  64, 256],
 [  1,   5,  25, 125, 625]]
vander(x, N=3, increasing=true):
[[ 1,  1,  1],
 [ 1,  2,  4],
 [ 1,  3,  9],
 [ 1,  4, 16],
 [ 1,  5, 25]]
```
