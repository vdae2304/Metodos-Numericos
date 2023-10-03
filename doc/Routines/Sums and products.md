# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Sums and products](#sums-and-products)
    - [`sum`](#sum)
    - [`prod`](#prod)
    - [`cumsum`](#cumsum)
    - [`cumprod`](#cumprod)

## Sums and products

### `sum`

Return the sum of the tensor elements.
```cpp
template <class T, size_t Rank>
T sum(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The sum of the tensor elements.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    std::cout << np::sum(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
44
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << np::sum(a) << "\n";
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
129
```

<h3><code>sum</h3></code>

Return the sum of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> sum(const tensor<T, Rank> &a, const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> sum(const tensor<T, Rank> &a, const shape_t<N> &axes,
                    keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> sum(const tensor<T, Rank> &a, const shape_t<N> &axes,
                        dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the sum is performed.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the sum over the given axes.

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
    np::matrix<int> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, compute the sum over all rows (column-wise).
    // For axis=1, compute the sum over all columns (row-wise).
    std::cout <<  np::sum(a, axis) << "\n";
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
[20, 15, 21, 23, 18, 32]
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
[34, 33, 25, 37]
```

### `prod`

Return the product of the tensor elements.
```cpp
template <class T, size_t Rank>
T prod(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The product of the tensor elements.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    std::cout <<  np::prod(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
86400
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
    std::cout <<  np::prod(a) << "\n";
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
1.02079e+16
```

<h3><code>prod</code></h3>

Return the product of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> prod(const tensor<T, Rank> &a, const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> prod(const tensor<T, Rank> &a, const shape_t<N> &axes,
                     keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> prod(const tensor<T, Rank> &a, const shape_t<N> &axes,
                         dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the product is performed.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the product over the given axes.

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
    np::matrix<int> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, compute the product over all rows (column-wise).
    // For axis=1, compute the product over all columns (row-wise).
    std::cout <<  np::prod(a, axis) << "\n";
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
[ 336,   54,  450, 1050,  315, 3780]
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
[19440, 13230,  1470, 27000]
```

### `cumsum`

Return the cumulative sum of the tensor elements along a given axis.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cumsum(const tensor<T, Rank> &a, size_t axis = 0);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the cumulative sum is computed. Default is zero.

Returns

* The cumulative sum of the tensor elements. The output tensor has the same dimension and the same shape as `a`.

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
    np::vector<int> a;
    std::cin >> a;
    std::cout << np::cumsum(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
[ 4,  7, 15, 16, 31, 34, 39, 40, 42, 44]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t axis;
    std::cin >> a >> axis;
    // For axis=0, accumulate sums over all rows (column-wise).
    // For axis=1, accumulate sums over all columns (row-wise).
    std::cout <<  np::cumsum(a, axis) << "\n";
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
[[ 8,  3,  9,  5,  3,  6],
 [15,  5, 14, 12,  6, 15],
 [18,  6, 16, 17, 13, 22],
 [20, 15, 21, 23, 18, 32]]
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
[[ 8, 11, 20, 25, 28, 34],
 [ 7,  9, 14, 21, 24, 33],
 [ 3,  4,  6, 11, 18, 25],
 [ 2, 11, 16, 22, 27, 37]]
```

### `cumprod`

Return the cumulative product of the tensor elements along a given axis.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cumprod(const tensor<T, Rank> &a, size_t axis = 0);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the cumulative product is computed. Default is zero.

Returns

* The cumulative product of the tensor elements. The output tensor has the same dimension and the same shape as `a`.

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
    np::vector<int> a;
    std::cin >> a;
    std::cout << np::cumprod(a) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
[    4,    12,    96,    96,  1440,  4320, 21600, 21600, 43200, 86400]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t axis;
    std::cin >> a >> axis;
    // For axis=0, accumulate products over all rows (column-wise).
    // For axis=1, accumulate products over all columns (row-wise).
    std::cout <<  np::cumprod(a, axis) << "\n";
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
[[   8,    3,    9,    5,    3,    6],
 [  56,    6,   45,   35,    9,   54],
 [ 168,    6,   90,  175,   63,  378],
 [ 336,   54,  450, 1050,  315, 3780]]
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
[[    8,    24,   216,  1080,  3240, 19440],
 [    7,    14,    70,   490,  1470, 13230],
 [    3,     3,     6,    30,   210,  1470],
 [    2,    18,    90,   540,  2700, 27000]]
```
