# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Numerical ranges](#numerical-ranges)
    - [`arange`](#arange)
    - [`linspace`](#linspace)
    - [`logspace`](#logspace)
    - [`geomspace`](#geomspace)

## Numerical ranges

### `arange`

Return evenly spaced values within a given interval.

Values are generated within the half-open interval $[start, stop)$, i.e., the interval including `start` but excluding `stop`, with spacing between values given by `step`.
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
* `step` Spacing between values. This is the distance between two adjacent values. Defaults to 1 if not provided.

Returns

* A light-weight object with evenly spaced values. This function does not create a new tensor, instead, an expression object is returned with evenly spaced values within a given interval.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return an expression equivalent to vector<int>
    std::cout << "arange(10):\n";
    std::cout << np::arange(10) << "\n";

    // Return an expression equivalent to vector<int>
    std::cout << "arange(10, 20):\n";
    std::cout << np::arange(10, 20) << "\n";

    // Return an expression equivalent to vector<int>
    std::cout << "arange(1, 20, 3):\n";
    std::cout << np::arange(1, 20, 3) << "\n";

    // Return an expression equivalent to vector<int>
    std::cout << "arange(10, 0, -1):\n";
    std::cout << np::arange(10, 0, -1) << "\n";
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
```

### `linspace`

Return evenly spaced numbers over a specified interval.

Return `num` evenly spaced samples, calculated over the interval $[start, stop]$. The endpoint of the interval can optionally be excluded.
```cpp
template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
linspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);
```

Parameters

* `start` The starting value of the sequence.
* `stop` The end value of the sequence.
* `num` Number of samples to generate. Default is 50.
* `endpoint` If true, `stop` is the last sample. Otherwise, it is not included. Default is true.

Returns

* A light-weight object with equally spaced samples. This function does not create a new tensor, instead, an expression object is returned with equally spaced samples over an interval.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return an expression equivalent to vector<double>
    std::cout << "linspace(0., 1., num=10):\n";
    std::cout << np::linspace(0., 1., 10) << "\n";

    // Return an expression equivalent to vector<double>
    std::cout << "linspace(-1., 1., num=10):\n";
    std::cout << np::linspace(-1., 1., 10) << "\n";

    // Return an expression equivalent to vector<double>
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

In linear space, the sequence starts at `pow(base, start)` (`base` to the power of `start`) and ends with `pow(base, stop)`.
```cpp
template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
logspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);

template <class T, class U, class V>
tensor<typename std::common_type<T, U, V>::type, 1>
logspace(const T &start, const U &stop, size_t num, bool endpoint,
         const V &base);
```

Parameters

* `start` `pow(base, start)` is the starting value of the sequence.
* `stop` `pow(base, stop)` is the final value of the sequence.
* `num` Number of samples to generate. Default is 50.
* `endpoint` If true, `stop` is the last sample. Otherwise, it is not included. Default is true.
* `base` The base of the log space. Default is 10.

Returns

* A light-weight object with equally spaced samples on a log scale. This function does not create a new tensor, instead, an expression object is returned with equally spaced samples on a log scale.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return an expression equivalent to vector<double>
    std::cout << "logspace(-3., 6., num=10):\n";
    std::cout << np::logspace(-3., 6., 10) << "\n";

    // Return an expression equivalent to vector<double>
    std::cout << "logspace(-3., 6., num=10, endpoint=false):\n";
    std::cout << np::logspace(-3., 6., 10, false) << "\n";

    // Return an expression equivalent to vector<double>
    std::cout << "logspace(1., 10., num=10, endpoint=true, base=2.):\n";
    std::cout << np::logspace(1., 10., 10, true, 2.) << "\n";
    return 0;
}
```

Output

```
logspace(-3., 6., num=10):
[  0.001,    0.01,     0.1,       1,      10,     100,    1000,   10000, 
  100000, 1000000]
logspace(-3., 6., num=10, endpoint=false):
[       0.001, 0.0079432823,  0.063095734,   0.50118723,    3.9810717, 
    31.622777,    251.18864,    1995.2623,    15848.932,    125892.54]
logspace(1., 10., num=10, endpoint=true, base=2.):
[   2,    4,    8,   16,   32,   64,  128,  256,  512, 1024]
```

### `geomspace`

Return numbers spaced evenly on a log scale (a geometric progression).

This is similar to `logspace`, but with endpoints specified directly. Each output sample is a constant multiple of the previous.

If the inputs are complex, the output will follow a logarithmic spiral in the complex plane.
```cpp
template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
geomspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);
```

Parameters

* `start` The starting value of the sequence.
* `stop` The final value of the sequence.
* `num` Number of samples to generate. Default is 50.
* `endpoint` If true, `stop` is the last sample. Otherwise, it is not included. Default is true.

Returns

* A light-weight object with equally spaced samples on a log scale. This function does not create a new tensor, instead, an expression object is returned with equally spaced samples on a log scale.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return an expression equivalent to vector<double>
    std::cout << "geomspace(1e-3, 1e6, num=10):\n";
    std::cout << np::geomspace(1e-3, 1e6, 10) << "\n";

    // Return an expression equivalent to vector<double>
    std::cout << "geomspace(1e-3, 1e6, num=10, endpoint=false):\n";
    std::cout << np::geomspace(1e-3, 1e6, 10, false) << "\n";

    // Return an expression equivalent to vector<double>
    std::cout << "geomspace(2., 1024., num=10):\n";
    std::cout << np::geomspace(2., 1024., 10) << "\n";
    return 0;
}
```

Output

```
geomspace(1e-3, 1e6, num=10):
[  0.001,    0.01,     0.1,       1,      10,     100,    1000,   10000, 
  100000, 1000000]
geomspace(1e-3, 1e6, num=10, endpoint=false):
[       0.001, 0.0079432823,  0.063095734,   0.50118723,    3.9810717, 
    31.622777,    251.18864,    1995.2623,    15848.932,    125892.54]
geomspace(2., 1024., num=10):
[   2,    4,    8,   16,   32,   64,  128,  256,  512, 1024]
```
