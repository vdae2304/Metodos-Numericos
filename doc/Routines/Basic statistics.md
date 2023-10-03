# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Basic statistics](#basic-statistics)
    - [`mean`](#mean)
    - [`median`](#median)
    - [`var`](#var)
    - [`stddev`](#stddev)
    - [`quantile`](#quantile)
    - [`cov`](#cov)
    - [`corrcoef`](#corrcoef)

## Basic statistics

### `mean`

Return the average of the tensor elements.
```cpp
template <class T, size_t Rank>
T mean(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The average of the tensor elements.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << np::mean(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
4.4
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << np::mean(a) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
5.375
```

<h3><code>mean</code></h3>

Return the average of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> mean(const tensor<T, Rank> &a, const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> mean(const tensor<T, Rank> &a, const shape_t<N> &axes,
                     keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> mean(const tensor<T, Rank> &a, const shape_t<N> &axes,
                         dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the average is computed.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the average over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, compute the mean over all rows (column-wise).
    // For axis=1, compute the mean over all columns (row-wise).
    std::cout << np::mean(a, axis) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
0
```

Output

```
[   5, 3.75, 5.25, 5.75,  4.5,    8]
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
1
```

Output

```
[5.6666667,       5.5, 4.1666667, 6.1666667]
```

### `median`

Return the median of the tensor elements.
```cpp
template <class T, size_t Rank>
T median(const tensor<T, Rank> &a);
```

The median is defined as the middle value of a sorted copy of the tensor. If the size is even, the average of the two middle values is returned.

Parameters

* `a` A tensor-like object.

Returns

* The median of the tensor elements.


Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << np::median(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
3
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << np::median(a) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
5
```

<h3><code>median</code></h3>

Return the median of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> median(const tensor<T, Rank> &a, const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> median(const tensor<T, Rank> &a, const shape_t<N> &axes,
                       keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> median(const tensor<T, Rank> &a, const shape_t<N> &axes,
                           dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the median is computed.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the median over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, compute the median over all rows (column-wise).
    // For axis=1, compute the median over all columns (row-wise).
    std::cout << np::median(a, axis) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
0
```

Output

```
[  5, 2.5,   5, 5.5,   4,   8]
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
1
```

Output

```
[5.5,   6,   4, 5.5]
```

### `var`

Return the variance of the tensor elements.
```cpp
template <class T, size_t Rank>
T var(const tensor<T, Rank> &a, bool bias = true);
```

The variance is defined as the average of the squared deviations from the mean
```
    var(a) = mean(x),    x = abs(a - mean(a))**2
```

The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias` is false, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1` provides an unbiased estimator of the sample variance; while `n` provides the maximum likelihood estimator of the variance for normally distributed variables.

Parameters

* `a` A tensor-like object.
* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Defaults to true.

Returns

* The variance of the tensor elements.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << np::var(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
16.44
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << np::var(a) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
6.48438
```

<h3><code>var</code></h3>

Return the variance of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> var(const tensor<T, Rank> &a, const shape_t<N> &axes,
                        bool bias = true);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> var(const tensor<T, Rank> &a, const shape_t<N> &axes,
                    bool bias, keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> var(const tensor<T, Rank> &a, const shape_t<N> &axes,
                        bool bias, dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the variance is computed.
* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Defaults to true.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to  , the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the variance over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, compute the variance over all rows (column-wise).
    // For axis=1, compute the variance over all columns (row-wise).
    std::cout << np::var(a, axis) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
0
```

Output

```
[   6.5, 9.6875, 6.1875, 0.6875,   2.75,    2.5]
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
1
```

Output

```
[5.2222222, 5.9166667, 5.4722222, 7.1388889]
```

### `stddev`

Return the standard deviation of the tensor elements.
```cpp
template <class class T, size_t Rank>
T stddev(const tensor<T, Rank> &a, bool bias = true);
```

The standard deviation is defined as the square root of the average of the squared deviations from the mean
```
    stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
```

The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias` is false, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1` provides an unbiased estimator of the sample variance; while `n` provides the maximum likelihood estimator of the variance for normally distributed variables.

Parameters

* `a` A tensor-like object.
* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Defaults to true.

Returns

* The standard deviation of the tensor elements.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << np::stddev(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
4.05463
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << np::stddev(a) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
2.54644
```

<h3><code>stddev</code></h3>

Return the standard deviation of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> stddev(const tensor<T, Rank> &a, const shape_t<N> &axes,
                           bool bias = true);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> stddev(const tensor<T, Rank> &a, const shape_t<N> &axes,
                       bool bias, keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> stddev(const tensor<T, Rank> &a, const shape_t<N> &axes,
                           bool bias, dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the standard deviation is computed.
* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Defaults to true.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the standard deviation over the given axes.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, compute the standard deviation over all rows (column-wise).
    // For axis=1, compute the standard deviation over all columns (row-wise).
    std::cout << np::stddev(a, axis) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
0
```

Output

```
[2.5495098, 3.1124749, 2.4874686, 0.8291562, 1.6583124, 1.5811388]
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
1
```

Output

```
[2.2852182, 2.4324199, 2.3392781, 2.6718699]
```

### `quantile`

Return the q-th quantile of the tensor elements.
```cpp
template <class T, size_t Rank>
T quantile(const tensor<T, Rank> &a, double q,
           const std::string &method = "linear");
```

Given a tensor of size `n`, the `q`-th quantile is the `(n - 1)*q`-th value of a sorted copy of the tensor. If `(n - 1)*q` is not an integer, the quantile is a weighted average of the two nearest neighbors. The different methods work as follows:

* `"lower"` Takes the lowest neighbor.
* `"higher"` Takes the highest neighbor.
* `"midpoint"` Takes the average of both neighbors.
* `"nearest"` Takes the nearest neighbor.
* `"linear"` Takes a linear interpolation between both neighbors.

Parameters

* `a` A tensor-like object.
* `q` Quantile to compute, which must be between 0 and 1 (inclusive).
* `method` This parameter specifies the method to use for estimating the quantile. Must be one of `"lower"`, `"higher"`, `"nearest"`, `"midpoint"` or `"linear"`.

Returns

* The `q`-th quantile of the tensor elements.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << "1st quartile: " << np::quantile(a, 0.25) << "\n";
    std::cout << "Median: " << np::quantile(a, 0.5) << "\n";
    std::cout << "3rd quartile: " << np::quantile(a, 0.75) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
1st quartile: 2
Median: 3
3rd quartile: 4.75
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "1st quartile: " << np::quantile(a, 0.25) << "\n";
    std::cout << "Median: " << np::quantile(a, 0.5) << "\n";
    std::cout << "3rd quartile: " << np::quantile(a, 0.75) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
1st quartile: 3
Median: 5
3rd quartile: 7
```

<h3><code>quantile</code></h3>

Return the q-th quantile of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> quantile(const tensor<T, Rank> &a, double q,
                             const shape_t<N> &axes,
                             const std::string &method = "linear");

template <class T, size_t Rank, size_t N>
tensor<T, Rank> quantile(const tensor<T, Rank> &a, double q,
                         const shape_t<N> &axes, const std::string &method,
                         keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> quantile(const tensor<T, Rank> &a, double q,
                             const shape_t<N> &axes, const std::string &method,
                             dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `q` Quantile to compute, which must be between 0 and 1 (inclusive).
* `axes` A `shape_t` object with the axes along which the quantile is computed.
* `method` This parameter specifies the method to use for estimating the quantile. Must be one of `"lower"`, `"higher"`, `"nearest"`, `"midpoint"` or `"linear"`.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the quantile over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, compute the quantiles over all rows (column-wise).
    // For axis=1, compute the quantiles over all columns (row-wise).
    std::cout << "1st quartile:\n" << np::quantile(a, 0.25, axis) << "\n";
    std::cout << "Median:\n" << np::quantile(a, 0.5, axis) << "\n";
    std::cout << "3rd quartile:\n" << np::quantile(a, 0.75, axis) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
0
```

Output

```
1st quartile:
[2.75, 1.75, 4.25,    5,    3, 6.75]
Median:
[  5, 2.5,   5, 5.5,   4,   8]
3rd quartile:
[7.25,  4.5,    6, 6.25,  5.5, 9.25]
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
1
```

Output

```
1st quartile:
[ 3.5,  3.5, 2.25,    5]
Median:
[5.5,   6,   4, 5.5]
3rd quartile:
[ 7.5,    7,  6.5, 8.25]
```

### `cov`

Return the covariance of two 1-dimensional tensors.
```cpp
template <class T>
T cov(const tensor<T, 1> &x, const tensor<T, 1> &y, bool bias = false);

template <class T>
T cov(const tensor<std::complex<T>, 1> &x, const tensor<std::complex<T>, 1> &y,
      bool bias = false);
```

The covariance is defined as the average of the element-wise products of the deviations from the mean
```
    cov(x, y) = mean(a),    a = (x - mean(x))*(y - mean(y))
```

For complex types, the complex conjugate of `y` is used
```
    cov(x, y) = mean(a),    a = (x - mean(x))*conj(y - mean(y))
```

The mean is calculated as `sum(a)/n`, where `n = a.size()`. However, if `bias` is false, the divisor `n - 1` is used instead of `n`.

Parameters

* `x` First tensor-like argument.
* `y` Second tensor-like argument.
* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Default is false.

Returns

* The covariance of the two tensors.

Exceptions

* `std::invalid_argument` Thrown if the tensor arguments have different sizes.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
    std::cin >> x >> y;
    std::cout << np::cov(x, y) << "\n";
    return 0;
}
```

Input

```
[ 8, 11,  5,  5,  3,  8,  4,  4,  9,  3]
[25,  5,  4,  6, 12,  8, 20,  7,  3, 13]
```

Output

```
-4.33333
```

<h3><code>cov</code></h3>

Return the covariance matrix of given data.
```cpp
template <class T>
tensor<T, 2> cov(const tensor<T, 2> &a, bool rowvar = true, bool bias = false);

template <class T>
tensor<std::complex<T>, 2> cov(const tensor<std::complex<T>, 2> &a,
                               bool rowvar = true, bool bias = false);
```

Parameters

* `a` A 2-dimensional tensor-like object containing multiple variables and observations.
* `rowvar` If true, then each row represents a variable, with observations in the columns. Otherwise, each column represents a variable, with observations in the rows. Default is true.
* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Default is false.

Returns

* The covariance matrix of the variables. The element at position `(i, j)` is equal to the covariance of the `i`-th and `j`-th variables.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << np::cov(a) << "\n";
    return 0;
}
```

Input

```
[[ 8, 11,  5,  5,  3,  8,  4,  4,  9,  3],
 [25,  5,  4,  6, 12,  8, 20,  7,  3, 13],
 [ 9,  7, 11,  3,  5,  8,  7, 13,  9, 14],
 [ 1, 18, 15, 14, 14, 10, 17, 15, 19,  4]]
```

Output

```
[[  7.7777778,  -4.3333333,  -1.6666667,   2.4444444],
 [ -4.3333333,        52.9, 0.022222222,  -27.455556],
 [ -1.6666667, 0.022222222,        11.6,  -6.5777778],
 [  2.4444444,  -27.455556,  -6.5777778,   35.566667]]
```

### `corrcoef`

Return the Pearson's correlation coefficient of two 1-dimensional tensors.
```cpp
template <class T>
T corrcoef(const tensor<T, 1> &x, const tensor<T, 1> &y);
```

The correlation coefficient is defined as the covariance of the variables divided by the product of their standard deviations
```
    corr(x, y) = cov(x, y) / (stddev(x) * stddev(y))
```

Parameters

* `x` First tensor-like argument.
* `y` Second tensor-like argument.

Returns

* The correlation coefficient of the two tensors.

Exceptions

* `std::invalid_argument` Thrown if the tensor arguments have different sizes.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
    std::cin >> x >> y;
    std::cout << np::corrcoef(x, y) << "\n";
    return 0;
}
```

Input

```
[ 8, 11,  5,  5,  3,  8,  4,  4,  9,  3]
[25,  5,  4,  6, 12,  8, 20,  7,  3, 13]
```

Output

```
-0.213632
```

<h3><code>corrcoef</code></h3>

Return the correlation matrix of given data.
```cpp
template <class T>
tensor<T, 2> corrcoef(const tensor<T, 2> &a, bool rowvar = true);
```

Parameters

* `a` A 2-dimensional tensor-like object containing multiple variables and observations.
* `rowvar` If true, then each row represents a variable, with observations in the columns. Otherwise, each column represents a variable, with observations in the rows. Default is true.

Returns

* The correlation matrix of the variables. The element at position `(i, j)` is equal to the correlation coefficient of the `i`-th and `j`-th variables.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << np::corrcoef(a) << "\n";
    return 0;
}
```

Input

```
[[ 8, 11,  5,  5,  3,  8,  4,  4,  9,  3],
 [25,  5,  4,  6, 12,  8, 20,  7,  3, 13],
 [ 9,  7, 11,  3,  5,  8,  7, 13,  9, 14],
 [ 1, 18, 15, 14, 14, 10, 17, 15, 19,  4]]
```

Output

```
[[            1,   -0.21363209,    -0.1754656,    0.14697072],
 [  -0.21363209,             1, 0.00089707893,   -0.63296663],
 [   -0.1754656, 0.00089707893,             1,   -0.32383861],
 [   0.14697072,   -0.63296663,   -0.32383861,             1]]
```
