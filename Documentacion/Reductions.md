# Reductions

Defined in `numcpp/routines.h`

- [Reductions](#reductions)
  - [Maximums and minimums](#maximums-and-minimums)
    - [`argmax`](#argmax)
    - [`argmax(axis)`](#argmaxaxis)
    - [`argmin`](#argmin)
    - [`argmin(axis)`](#argminaxis)
    - [`amax`](#amax)
    - [`amax(axes)`](#amaxaxes)
    - [`amin`](#amin)
    - [`amin(axes)`](#aminaxes)
    - [`maximum`](#maximum)
    - [`minimum`](#minimum)
    - [`clamp`](#clamp)
  - [Sums and products](#sums-and-products)
    - [`sum`](#sum)
    - [`sum(axes)`](#sumaxes)
    - [`prod`](#prod)
    - [`prod(axes)`](#prodaxes)
    - [`cumsum`](#cumsum)
    - [`cumprod`](#cumprod)
  - [Logic functions](#logic-functions)
    - [`all`](#all)
    - [`all(axes)`](#allaxes)
    - [`any`](#any)
    - [`any(axes)`](#anyaxes)
    - [`count_nonzero`](#count_nonzero)
    - [`count_nonzero(axes)`](#count_nonzeroaxes)
    - [`isclose`](#isclose)
    - [`isclose`](#isclose-1)
    - [`allclose`](#allclose)
  - [Basic statistics](#basic-statistics)
    - [`mean`](#mean)
    - [`mean(axes)`](#meanaxes)
    - [`median`](#median)
    - [`median(axes)`](#medianaxes)
    - [`var`](#var)
    - [`var(axes)`](#varaxes)
    - [`stddev`](#stddev)
    - [`stddev(axes)`](#stddevaxes)
    - [`quantile`](#quantile)
    - [`quantile(axes)`](#quantileaxes)
    - [`cov`](#cov)
    - [`cov`](#cov-1)
    - [`corrcoef`](#corrcoef)
    - [`corrcoef`](#corrcoef-1)

## Maximums and minimums

### `argmax`

Return the indices of the maximum value in the tensor.
```cpp
template <class T, size_t Rank>
index_t<Rank> argmax(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The indices of the maximum value in the tensor. In case of multiple
occurrences of the maximum value, return the index corresponding to the first
occurrence.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    np::index_t<1> index = np::argmax(a);
    std::cout << "Index: " << index << "\n";
    std::cout << "Value: " << a[index] << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
Index: (2,)
Value: 19
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::index_t<2> index = np::argmax(a);
    std::cout << "Index: " << index << "\n";
    std::cout << "Value: " << a[index] << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
Index: (2, 5)
Value: 14
```

### `argmax(axis)`

Return the indices of the maximum value in the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank> argmax(const tensor<T, Rank> &a, size_t axis);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the maximum value is selected.

Returns

* A new tensor with the indices of the maximum value along an axis. The output
tensor will have the same dimension and the same shape, except that the axis
which is reduced is left as a dimension of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;

    np::matrix<size_t> indices = np::argmax(a, 0);
    std::cout << "Axis 0:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, 0) << "\n";

    indices = np::argmax(a, 1);
    std::cout << "Axis 1:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, 1) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
Axis 0:
Indices:
[[0, 3, 0, 0, 0, 2]]
Values:
[[13, 14,  2,  7,  7, 14]]
Axis 1:
Indices:
[[0],
 [0],
 [5],
 [1]]
Values:
[[13],
 [ 8],
 [14],
 [14]]
```

### `argmin`

Return the indices of the minimum value in the tensor.
```cpp
template <class T, size_t Rank>
index_t<Rank> argmin(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The indices of the minimum value in the tensor. In case of multiple
occurrences of the minimum value, return the index corresponding to the first
occurrence.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    np::index_t<1> index = np::argmin(a);
    std::cout << "Index: " << index << "\n";
    std::cout << "Value: " << a[index] << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
Index: (7,)
Value: -5
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::index_t<2> index = np::argmin(a);
    std::cout << "Index: " << index << "\n";
    std::cout << "Value: " << a[index] << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
Index: (1, 4)
Value: -5
```

### `argmin(axis)`

Return the indices of the minimum value in the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank> argmin(const tensor<T, Rank> &a, size_t axis);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the minimum value is selected.

Returns

* A new tensor with the indices of the minimum value along an axis. The output
tensor will have the same dimension and the same shape, except that the axis
which is reduced is left as a dimension of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;

    np::matrix<size_t> indices = np::argmin(a, 0);
    std::cout << "Axis 0:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, 0) << "\n";

    indices = np::argmin(a, 1);
    std::cout << "Axis 1:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, 1) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
Axis 0:
Indices:
[[2, 1, 2, 3, 1, 3]]
Values:
[[ 3,  2, -2,  1, -5, -3]]
Axis 1:
Indices:
[[2],
 [4],
 [2],
 [5]]
Values:
[[ 2],
 [-5],
 [-2],
 [-3]]
```

### `amax`

Return the maximum value contained in the tensor.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type amax(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The maximum value in the tensor.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << np::amax(a) << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
19
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << np::amax(a) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
14
```

### `amax(axes)`

Return the maximum value contained in the tensor over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> amax(
    const tensor<T, Rank> &a, size_t axis
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> amax(
    const tensor<T, Rank> &a, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the maximum value is
selected.

Returns

* A new tensor with the maximum value over the axes. The output tensor will
have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::amax(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::amax(a, 1) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
Axis 0:
[[13, 14,  2,  7,  7, 14]]
Axis 1:
[[13],
 [ 8],
 [14],
 [14]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << np::amax(a, np::make_shape(1, 2)) << "\n";
    return 0;
}
```

Input

```
[[[ 6, 13, -5, -3],
  [14, -1, -3, 14],
  [ 8, 12, -5, -2]],
 [[ 1,  7, -4,  0],
  [13, 14,  8,  5],
  [ 1, -3, -4,  0]]]
```

Output

```
Last 2 axes:
[[[14]],

 [[14]]]
```

### `amin`

Return the minimum value contained in the tensor.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type amin(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The minimum value in the tensor.


Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << np::amin(a) << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
-5
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << np::amin(a) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
-5
```

### `amin(axes)`

Return the minimum value contained in the tensor over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> amin(
    const tensor<T, Rank> &a, size_t axis
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> amin(
    const tensor<T, Rank> &a, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the minimum value is
selected.

Returns

* A new tensor with the minimum value over the axes. The output tensor will
have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::amin(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::amin(a, 1) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
Axis 0:
[[ 3,  2, -2,  1, -5, -3]]
Axis 1:
[[ 2],
 [-5],
 [-2],
 [-3]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << np::amin(a, np::make_shape(1, 2)) << "\n";
    return 0;
}
```

Input

```
[[[ 6, 13, -5, -3],
  [14, -1, -3, 14],
  [ 8, 12, -5, -2]],
 [[ 1,  7, -4,  0],
  [13, 14,  8,  5],
  [ 1, -3, -4,  0]]]
```

Output

```
Last 2 axes:
[[[-5]],

 [[-4]]]
```

### `maximum`

Return the element-wise maximum of two tensors.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> maximum(const tensor<T, Rank> &a, const tensor<T, Rank> &b);

template <class T, size_t Rank>
tensor<T, Rank> maximum(
    const tensor<T, Rank> &a,
    const typename tensor<T, Rank>::value_type &val
);

template <class T, size_t Rank>
tensor<T, Rank> maximum(
    const typename tensor<T, Rank>::value_type &val,
    const tensor<T, Rank> &b
);
```

Parameters

* `a` First tensor-like object to compare.
* `b` Second tensor-like object to compare.
* `val` Value to use either as first argument or second argument. Values are
broadcasted to an appropriate size.

Returns

* A light-weight object with the element-wise maximum of two tensors. This
function does not create a new tensor, instead, it returns an expression object
with the element-wise maximum of the tensor arguments.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a, b;
    std::cin >> a >> b;
    std::cout << np::maximum(a, b) << "\n";
    return 0;
}
```

Input

```
[-1, -5, 12,  9,  4, 12, 15, 17, 11, -3]
[-5, -1,  6, 16, 16,  8,  9, 12, 19, 11]
```

Output

```
[-1, -1, 12, 16, 16, 12, 15, 17, 19, 11]
```

### `minimum`

Return the element-wise minimum of two tensors.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> minimum(const tensor<T, Rank> &a, const tensor<T, Rank> &b);

template <class T, size_t Rank>
tensor<T, Rank> minimum(
    const tensor<T, Rank> &a,
    const typename tensor<T, Rank>::value_type &val
);

template <class T, size_t Rank>
tensor<T, Rank> minimum(
    const typename tensor<T, Rank>::value_type &val,
    const tensor<T, Rank> &b
);
```

Parameters

* `a` First tensor-like object to compare.
* `b` Second tensor-like object to compare.
* `val` Value to use either as first argument or second argument. Values are
broadcasted to an appropriate size.

Returns

* A light-weight object with the element-wise minimum of two tensors. This
function does not create a new tensor, instead, it returns an expression object
with the element-wise minimum of the tensor arguments.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a, b;
    std::cin >> a >> b;
    std::cout << np::minimum(a, b) << "\n";
    return 0;
}
```

Input

```
[-1, -5, 12,  9,  4, 12, 15, 17, 11, -3]
[-5, -1,  6, 16, 16,  8,  9, 12, 19, 11]
```

Output

```
[-5, -5,  6,  9,  4,  8,  9, 12, 11, -3]
```

### `clamp`

Clamp the values in the tensor. Given an interval [`a_min`, `a_max`], values
smaller than `a_min` become `a_min`, and values larger than `a_max` become
`a_max`. For complex types, the real and imaginary parts are clamped
separately.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> clamp(
    const tensor<T, Rank> &a,
    const typename tensor<T, Rank>::value_type &a_min,
    const typename tensor<T, Rank>::value_type &a_max
);
```

Parameters

* `a` A tensor-like object with the elements to clamp.
* `a_min` The lower boundary to clamp.
* `a_max` The upper boundary to clamp.

Returns

* A light-weight object with the tensor formed by clamping the values in the
given tensor. This function does not create a new tensor, instead, it returns a
readonly view with the clamped values in the tensor.

Notes

* The behavior is undefined if `a_min` is greater than `a_max`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    int a_min, a_max;
    std::cin >> a >> a_min >> a_max;
    std::cout << np::clamp(a, a_min, a_max) << "\n";
    return 0;
}
```

Input

```
[4, 12, 2, 0, 2, 10, -4, 6, 2, -4]
1 10
```

Output

```
[ 4, 10,  2,  1,  2, 10,  1,  6,  2,  1]
```

##  Sums and products

### `sum`

Return the sum of the tensor elements.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type sum(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The sum of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
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
#include "numcpp.h"
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

### `sum(axes)`

Return the sum of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> sum(
    const tensor<T, Rank> &a, size_t axis
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> sum(
    const tensor<T, Rank> &a, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the sum is performed.

Returns

* A new tensor with the sum over the axes. The output tensor will have the same
dimension and the same shape, except that the axes which are reduced are left
as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::sum(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::sum(a, 1) << "\n";
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
Axis 0:
[[20, 15, 21, 23, 18, 32]]
Axis 1:
[[34],
 [33],
 [25],
 [37]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << np::sum(a, np::make_shape(1, 2)) << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
[[[41]],

 [[49]]]
```

### `prod`

Return the product of the tensor elements.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type prod(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The product of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << np::prod(a) << "\n";
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

### `prod(axes)`

Return the product of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> prod(
    const tensor<T, Rank> &a, size_t axis
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> prod(
    const tensor<T, Rank> &a, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the product is performed.

Returns

* A new tensor with the product over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::prod(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::prod(a, 1) << "\n";
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
Axis 0:
[[ 336,   54,  450, 1050,  315, 3780]]
Axis 1:
[[19440],
 [13230],
 [ 1470],
 [27000]]
```

### `cumsum`

Return the cumulative sum of the tensor elements along a given axis.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> cumsum(
    const tensor<T, Rank> &a, size_t axis = 0
);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the cumulative sum is computed. Default is zero.

Returns

* The cumulative sum of the tensor elements. The output tensor will have the
same dimension and the same shape as `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::cumsum(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::cumsum(a, 1) << "\n";
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
Axis 0:
[[ 8,  3,  9,  5,  3,  6],
 [15,  5, 14, 12,  6, 15],
 [18,  6, 16, 17, 13, 22],
 [20, 15, 21, 23, 18, 32]]
Axis 1:
[[ 8, 11, 20, 25, 28, 34],
 [ 7,  9, 14, 21, 24, 33],
 [ 3,  4,  6, 11, 18, 25],
 [ 2, 11, 16, 22, 27, 37]]
```

### `cumprod`

Return the cumulative product of the tensor elements along a given axis.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> cumprod(
    const tensor<T, Rank> &a, size_t axis = 0
);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the cumulative product is computed. Default is zero.

Returns

* The cumulative product of the tensor elements. The output tensor will have
the same dimension and the same shape as `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::cumprod(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::cumprod(a, 1) << "\n";
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
Axis 0:
[[   8,    3,    9,    5,    3,    6],
 [  56,    6,   45,   35,    9,   54],
 [ 168,    6,   90,  175,   63,  378],
 [ 336,   54,  450, 1050,  315, 3780]]
Axis 1:
[[    8,    24,   216,  1080,  3240, 19440],
 [    7,    14,    70,   490,  1470, 13230],
 [    3,     3,     6,    30,   210,  1470],
 [    2,    18,    90,   540,  2700, 27000]]
```

## Logic functions

### `all`

Test whether all tensor elements evaluates to `true`.
```cpp
template <size_t Rank>
bool all(const tensor<bool, Rank> &a);
```

Parameters

* `a` A tensor-like object of `bool`.

Returns

* `true` if all the elements evaluates to `true` and `false` otherwise.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    // Recall that a > 0 is an expression object. Therefore, all(a > 0)
    // works over a directly, without creating temporaries.
    std::cout << std::boolalpha << np::all(a > 0) << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
true
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
false
```

### `all(axes)`

Test whether all tensor elements over the given axes evaluates to `true`.
```cpp
template <size_t Rank>
tensor<bool, Rank> all(const tensor<bool, Rank> &a, size_t axis);

template <size_t Rank, size_t N>
tensor<bool, Rank> all(const tensor<bool, Rank> &a, const shape_t<N> &axes);
```

Parameters

* `a` A tensor-like object of `bool`.
* `axes` A `shape_t` object with the axes along which the logical AND reduction
is performed.

Returns

* A new tensor with the results of the test over the axes. The output tensor
will have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << std::boolalpha;
    std::cout << "Axis 0:\n" << np::all(a > 0, 0) << "\n";
    std::cout << "Axis 1:\n" << np::all(a > 0, 1) << "\n";
    return 0;
}
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
```

Output

```
Axis 0:
[[false, false,  true, false,  true, false]]
Axis 1:
[[ true],
 [false],
 [false],
 [false]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << std::boolalpha;
    std::cout << np::all(a > 0, np::make_shape(1, 2))  << "\n";
    return 0;
}
```

Input

```
[[[11, 11 , 4,  0],
  [14,  9 , 9,  13],
  [10,  3, -1,  8]],
 [[14,  2, 10,  7],
  [ 5,  3 , 9,  1],
  [ 1,  1, 13, 10]]]
```

Output

```
Last 2 axes:
[[[false]],

 [[ true]]]
```

### `any`

Test whether any tensor element evaluates to `true`.
```cpp
template <size_t Rank>
bool any(const tensor<bool, Rank> &a);
```

Parameters

* `a` A tensor-like object of `bool`.

Returns

* `true` if any element evaluates to `true` and `false` otherwise.


Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    // Recall that a < 0 is an expression object. Therefore, any(a < 0)
    // works over a directly, without creating temporaries.
    std::cout << std::boolalpha << np::any(a < 0) << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
false
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
true
```

### `any(axes)`

Test whether any tensor element over the given axes evaluates to `true`.
```cpp
template <size_t Rank>
tensor<bool, Rank> any(const tensor<bool, Rank> &a, size_t axis);

template <size_t Rank, size_t N>
tensor<bool, Rank> any(const tensor<bool, Rank> &a, const shape_t<N> &axes);
```

Parameters

* `a` A tensor-like object of `bool`.
* `axes` A `shape_t` object with the axes along which the logical OR reduction
is performed.

Returns

* A new tensor with the results of the test over the axes. The output tensor
will have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << std::boolalpha;
    std::cout << "Axis 0:\n" << np::any(a < 0, 0) << "\n";
    std::cout << "Axis 1:\n" << np::any(a < 0, 1) << "\n";
    return 0;
}
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
```

Output

```
Axis 0:
[[false,  true, false,  true, false,  true]]
Axis 1:
[[false],
 [ true],
 [ true],
 [ true]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << std::boolalpha;
    std::cout << np::any(a < 0, np::make_shape(1, 2))  << "\n";
    return 0;
}
```

Input

```
[[[11, 11 , 4,  0],
  [14,  9 , 9,  13],
  [10,  3, -1,  8]],
 [[14,  2, 10,  7],
  [ 5,  3 , 9,  1],
  [ 1,  1, 13, 10]]]
```

Output

```
Last 2 axes:
[[[ true]],

 [[false]]]
```

### `count_nonzero`

Count the number of non-zero elements in the tensor.
```cpp
template <class T, size_t Rank>
size_t count_nonzero(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The number of non-zero elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    // Recall that a > 0 is an expression object. Therefore,
    // count_nonzero(a > 0) works over a directly, without creating temporaries.
    std::cout << np::count_nonzero(a > 0) << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
10
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
5
```

### `count_nonzero(axes)`

Count the number of non-zero elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank> count_nonzero(const tensor<T, Rank> &a, size_t axis);

template <class T, size_t Rank, size_t N>
tensor<size_t, Rank> count_nonzero(
    const tensor<T, Rank> &a, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which to count non-zeros.

Returns

* A new tensor with the number of non-zero elements over the axes. The output
tensor will have the same dimension and the same shape, except that the axes
which are reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::count_nonzero(a > 0, 0) << "\n";
    std::cout << "Axis 1:\n" << np::count_nonzero(a > 0, 1) << "\n";
    return 0;
}
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
```

Output

```
Axis 0:
[[3, 2, 4, 3, 4, 3]]
Axis 1:
[[6],
 [4],
 [5],
 [4]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << np::count_nonzero(a > 0, np::make_shape(1, 2))  << "\n";
    return 0;
}
```

Input

```
[[[11, 11 , 4,  0],
  [14,  9 , 9,  13],
  [10,  3, -1,  8]],
 [[14,  2, 10,  7],
  [ 5,  3 , 9,  1],
  [ 1,  1, 13, 10]]]
```

Output

```
Last 2 axes:
[[[10]],

 [[12]]]
```

### `isclose`

Return if two numbers are equal within a tolerance.
```cpp
bool isclose(float a, float b, float rtol = 1e-4f, float atol = 0.f);

bool isclose(double a, double b, double rtol = 1e-8, double atol = 0.);

bool isclose(long double a, long double b,
             long double rtol = 1e-10L, long double atol = 0.L);

// Additional overloads for arithmetic types.
template <class T, class U>
bool isclose(T a, U b, Promoted rtol = 1e-8, Promoted atol = 0.);

// Additional overloads for complex types.
template <class T>
bool isclose(const std::complex<T> &a, const std::complex<T> &b,
             T rtol = T(1e-8), T atol = T(0));

template <class T>
bool isclose(const std::complex<T> &a, const T &b,
             T rtol = T(1e-8), T atol = T(0));

template <class T>
bool isclose(const T &a, const std::complex<T> &b,
             T rtol = T(1e-8), T atol = T(0));

template <class T, class U>
bool isclose(const std::complex<T> &a, const std::complex<U> &b,
             Promoted rtol = 1e-8, Promoted atol = 0);
```

Whether or not two values are considered equal is determined according to given
absolute and relative tolerance. The tolerances must be non-negative, typically
very small numbers. For floating-point values, the function uses the following
equation to test whether two numbers are equivalent:

```cpp
    abs(a - b) <= fmax(rtol * fmax(abs(a), abs(b)), atol)
```

NaN is not considered equal to any other value, including NaN. inf and -inf are
only considered equal to themselves.

Parameters

* `a` An integer, floating-point or complex value.
* `b` An integer, floating-point or complex value.
* `rtol` The relative tolerance.
* `atol` The absolute tolerance.

Notes

* If any argument has integral type, it is cast to `double`.
* If any argument is `long double`, the other argument is promoted to
`long double`.
* If arguments are `float` and `double` (in any order), the `float` argument is
promoted to `double`.
* For complex types, the equality is tested on both real and imaginary parts.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    std::cout << std::boolalpha;
    std::cout << "isclose(1., 1.+1e-7): ";
    std::cout << np::isclose(1., 1.+1e-7) << "\n";
    std::cout << "isclose(1.+1e-8, 1.): ";
    std::cout << np::isclose(1.+1e-8, 1.) << "\n";
    std::cout << "isclose(0.33333333, 1./3.): ";
    std::cout << np::isclose(0.33333333, 1./3.) << "\n";
    std::cout << "isclose(1./0., inf): ";
    std::cout << np::isclose(1./0., np::constants<double>::inf) << "\n";
    std::cout << "isclose(0./0., nan): ";
    std::cout << np::isclose(0./0., np::constants<double>::nan) << "\n";
    return 0;
}
```

Output

```
isclose(1., 1.+1e-7): false
isclose(1.+1e-8, 1.): true
isclose(0.33333333, 1./3.): true
isclose(1./0., inf): true
isclose(0./0., nan): false
```

### `isclose`

Return if two tensors are equal, element-wise, within a tolerance.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isclose(
    const tensor<T, Rank> &a, const tensor<T, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(
    const tensor<T, Rank> &a, const typename tensor<T, Rank>::value_type &val,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(
    const tensor<std::complex<T>, Rank> &a,
    const tensor<std::complex<T>, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(
    const tensor<std::complex<T>, Rank> &a, const std::complex<T> &val,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(
    const std::complex<T> &val, const tensor<std::complex<T>, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);
```

Parameters

* `a` First tensor-like object to compare.
* `b` Second tensor-like object to compare.
* `val` Value to use either as first argument or second argument. Values are
broadcasted to an appropriate size.
* `rtol` The relative tolerance.
* `atol` The absolute tolerance.

Returns

* A light-weight object of `bool` with each element set to `true` where `a` and
`b` are equal within the given tolerance, and `false` otherwise. This function
does not create a new tensor, instead, it returns an expression object with the
element-wise comparison.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double inf = np::constants<double>::inf;
    double nan = np::constants<double>::nan;
    np::array<double> a{       1., 1. + 1e-8, 0.33333333,     1e7,
                              1e8,     1./0.,     -1./0.,   0./0.};
    np::array<double> b{1. + 1e-7,        1.,      1./3., 1e7 + 1,
                          1e8 + 1,       inf,        inf,     nan};
    std::cout << "a:\n" << a << "\n";
    std::cout << "b:\n" << b << "\n";
    std::cout << std::boolalpha;
    std::cout << "Using isclose:\n" << np::isclose(a, b) << "\n";
    std::cout << "Using == operator:\n" << (a == b) << "\n";
    return 0;
}
```

Output

```
a:
[         1,          1, 0.33333333,   10000000,      1e+08,        inf,
       -inf,        nan]
b:
[ 1.0000001,          1, 0.33333333,   10000001,      1e+08,        inf,
        inf,        nan]
Using isclose:
[false,  true,  true, false,  true,  true, false, false]
Using == operator:
[false, false, false, false, false,  true, false, false]
```

### `allclose`

Test whether all elements in two tensors are element-wise equal within a
tolerance.
```cpp
template <class T, size_t Rank>
bool allclose(
    const tensor<T, Rank> &a, const tensor<T, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
bool allclose(
    const tensor<T, Rank> &a, const typename tensor<T, Rank>::value_type &val,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
bool allclose(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
bool allclose(
    const tensor<std::complex<T>, Rank> &a,
    const tensor<std::complex<T>, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
bool allclose(
    const tensor<std::complex<T>, Rank> &a, const std::complex<T> &val,
    const T &rtol = T(1e-8), const T &atol = T(0)
);

template <class T, size_t Rank>
bool allclose(
    const std::complex<T> &val, const tensor<std::complex<T>, Rank> &b,
    const T &rtol = T(1e-8), const T &atol = T(0)
);
```

Parameters

* `a` First tensor-like object to compare.
* `b` Second tensor-like object to compare.
* `val` Value to use either as first argument or second argument. Values are
broadcasted to an appropriate size.
* `rtol` The relative tolerance.
* `atol` The absolute tolerance.

Returns

* `true` if all the elements are considered equal and `false` otherwise.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double inf = np::constants<double>::inf;
    np::array<double> a{1. + 1e-8, 0.33333333,     1e8, 1./0., -1./0.};
    np::array<double> b{       1.,      1./3., 1e8 + 1,   inf,   -inf};
    std::cout << "a:\n" << a << "\n";
    std::cout << "b:\n" << b << "\n";
    std::cout << std::boolalpha;
    std::cout << "Using allclose:\n" << np::allclose(a, b) << "\n";
    std::cout << "Using == operator:\n" << np::all(a == b) << "\n";
    return 0;
}
```

Output

```
a:
[         1, 0.33333333,      1e+08,        inf,       -inf]
b:
[         1, 0.33333333,      1e+08,        inf,       -inf]
Using allclose:
true
Using == operator:
false
```

## Basic statistics

### `mean`

Return the average of the tensor elements.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type mean(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The average of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
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
#include "numcpp.h"
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

### `mean(axes)`

Return the average of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> mean(
    const tensor<T, Rank> &a, size_t axis
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> mean(
    const tensor<T, Rank> &a, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the average is computed.

Returns

* A new tensor with the average over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::mean(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::mean(a, 1) << "\n";
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
Axis 0:
[[   5, 3.75, 5.25, 5.75,  4.5,    8]]
Axis 1:
[[5.6666667],
 [      5.5],
 [4.1666667],
 [6.1666667]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << np::mean(a, np::make_shape(1, 2)) << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
[[[3.4166667]],

 [[4.0833333]]]
```

### `median`

Return the median of the tensor elements.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type median(const tensor<T, Rank> &a);
```

The median is defined as the middle value of a sorted copy of the tensor. If
the size is even, the average of the two middle values is returned.

Parameters

* `a` A tensor-like object.

Returns

* The median of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
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
#include "numcpp.h"
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

### `median(axes)`

Return the median of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> median(
    const tensor<T, Rank, Tag> &a, size_t axis
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> median(
    const tensor<T, Rank> &a, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the median is computed.

Returns

* A new tensor with the median over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::median(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::median(a, 1) << "\n";
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
Axis 0:
[[  5, 2.5,   5, 5.5,   4,   8]]
Axis 1:
[[5.5],
 [  6],
 [  4],
 [5.5]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << np::median(a, np::make_shape(1, 2)) << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
[[[  3]],

 [[3.5]]]
```

### `var`

Return the variance of the tensor elements.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type var(const tensor<T, Rank> &a, bool bias);
```

The variance is defined as the average of the squared deviations from the mean
```
    var(a) = mean(x),    x = abs(a - mean(a))**2
```
The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias`
is `false`, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1`
provides an unbiased estimator of the sample variance; while `n` provides
the maximum likelihood estimator of the variance for normally distributed
variables.

Parameters

* `a` A tensor-like object.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

Returns

* The variance of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
    std::cin >> a;
    std::cout << "Unbiased: " << np::var(a, false) << "\n";
    std::cout << "Biased: " << np::var(a, true) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
Unbiased: 18.2667
Biased: 16.44
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Unbiased: " << np::var(a, false) << "\n";
    std::cout << "Biased: " << np::var(a, true) << "\n";
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
Unbiased: 6.7663
Biased: 6.48438
```

### `var(axes)`

Return the variance of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> var(
    const tensor<T, Rank> &a, size_t axis, bool bias
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> var(
    const tensor<T, Rank> &a, const shape_t<N> &axes, bool bias
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the variance is computed.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

Returns

* A new tensor with the variance over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n";
    std::cout << "Uniased:\n" << np::var(a, 0, false) << "\n";
    std::cout << "Biased:\n" << np::var(a, 0, true) << "\n";
    std::cout << "Axis 1:\n";
    std::cout << "Uniased:\n" << np::var(a, 1, false) << "\n";
    std::cout << "Biased:\n" << np::var(a, 1, true) << "\n";
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
Axis 0:
Uniased:
[[ 8.6666667,  12.916667,       8.25, 0.91666667,  3.6666667,  3.3333333]]
Biased:
[[   6.5, 9.6875, 6.1875, 0.6875,   2.75,    2.5]]
Axis 1:
Uniased:
[[6.2666667],
 [      7.1],
 [6.5666667],
 [8.5666667]]
Biased:
[[5.2222222],
 [5.9166667],
 [5.4722222],
 [7.1388889]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << "Unbiased:\n";
    std::cout << np::var(a, np::make_shape(1, 2), false) << "\n";
    std::cout << "Biased:\n";
    std::cout << np::var(a, np::make_shape(1, 2), true) << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
Unbiased:
[[[5.9015152]],

 [[4.6287879]]]
Biased:
[[[5.4097222]],

 [[4.2430556]]]
```

### `stddev`

Return the standard deviation of the tensor elements.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type stddev(
    const tensor<T, Rank> &a, bool bias
);
```

The standard deviation is defined as the square root of the average of the
squared deviations from the mean
```
    stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
```
The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias`
is `false`, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1`
provides an unbiased estimator of the sample variance; while `n` provides the
maximum likelihood estimator of the variance for normally distributed
variables.

Parameters

* `a` A tensor-like object.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

Returns

* The standard deviation of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
    std::cin >> a;
    std::cout << "Unbiased: " << np::stddev(a, false) << "\n";
    std::cout << "Biased: " << np::stddev(a, true) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
Unbiased: 4.27395
Biased: 4.05463
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Unbiased: " << np::stddev(a, false) << "\n";
    std::cout << "Biased: " << np::stddev(a, true) << "\n";
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
Unbiased: 2.60121
Biased: 2.54644
```

### `stddev(axes)`

Return the standard deviation of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> stddev(
    const tensor<T, Rank> &a, size_t axis, bool bias
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> stddev(
    const tensor<T, Rank> &a, const shape_t<N> &axes, bool bias
);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the standard deviation is
computed.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

Returns

* A new tensor with the standard deviation over the axes. The output tensor
will have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n";
    std::cout << "Uniased:\n" << np::stddev(a, 0, false) << "\n";
    std::cout << "Biased:\n" << np::stddev(a, 0, true) << "\n";
    std::cout << "Axis 1:\n";
    std::cout << "Uniased:\n" << np::stddev(a, 1, false) << "\n";
    std::cout << "Biased:\n" << np::stddev(a, 1, true) << "\n";
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
Axis 0:
Uniased:
[[ 2.9439203,  3.5939764,  2.8722813, 0.95742711,  1.9148542,  1.8257419]]
Biased:
[[2.5495098, 3.1124749, 2.4874686, 0.8291562, 1.6583124, 1.5811388]]
Axis 1:
Uniased:
[[2.5033311],
 [2.6645825],
 [2.5625508],
 [2.9268869]]
Biased:
[[2.2852182],
 [2.4324199],
 [2.3392781],
 [2.6718699]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << "Unbiased:\n";
    std::cout << np::stddev(a, np::make_shape(1, 2), false) << "\n";
    std::cout << "Biased:\n";
    std::cout << np::stddev(a, np::make_shape(1, 2), true) << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
Unbiased:
[[[2.4293034]],

 [[2.1514618]]]
Biased:
[[[ 2.325881]],

 [[2.0598678]]]
```

### `quantile`

Return the `q`-th quantile of the tensor elements.
```cpp
template <class T, size_t Rank>
typename tensor<T, Rank>::value_type quantile(
    const tensor<T, Rank> &a, double q, const std::string &method = "linear"
);
```

Given a tensor of size `n`, the `q`-th quantile is the `(n - 1)*q`-th value of
a sorted copy of the tensor. If `(n - 1)*q` is not an integer, the quantile is
a weighted average of the two nearest neighbors. The different methods work as
follows:

* `"lower"` Takes the lowest neighbor.
* `"higher"` Takes the highest neighbor.
* `"midpoint"` Takes the average of both neighbors.
* `"nearest"` Takes the nearest neighbor.
* `"linear"` Takes a linear interpolation between both neighbors.

Parameters

* `a` A tensor-like object.
* `q` Quantile to compute, which must be between 0 and 1 (inclusive).
* `method` This parameter specifies the method to use for estimating the
quantile. Must be one of `"lower"`, `"higher"`, `"nearest"`, `"midpoint"` or
`"linear"`.

Returns

* The `q`-th quantile of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
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
#include "numcpp.h"
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

### `quantile(axes)`

Return the `q`-th quantile of the tensor elements over the given axes.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> quantile(
    const tensor<T, Rank> &a, double q, size_t axis,
    const std::string &method = "linear"
);

template <class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> quantile(
    const tensor<T, Rank> &a, double q, const shape_t<N> &axes,
    const std::string &method = "linear"
);
```

Parameters

* `a` A tensor-like object.
* `q` Quantile to compute, which must be between 0 and 1 (inclusive).
* `axes` A `shape_t` object with the axes along which the quantile is computed.
* `method` This parameter specifies the method to use for estimating the
quantile. Must be one of `"lower"`, `"higher"`, `"nearest"`, `"midpoint"` or
`"linear"`.

Returns

* A new tensor with the quantile over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n";
    std::cout << "1st quartile:\n" << np::quantile(a, 0.25, 0) << "\n";
    std::cout << "Median:\n" << np::quantile(a, 0.5, 0) << "\n";
    std::cout << "3rd quartile:\n" << np::quantile(a, 0.75, 0) << "\n";
    std::cout << "Axis 1:\n";
    std::cout << "1st quartile:\n" << np::quantile(a, 0.25, 1) << "\n";
    std::cout << "Median:\n" << np::quantile(a, 0.5, 1) << "\n";
    std::cout << "3rd quartile:\n" << np::quantile(a, 0.75, 1) << "\n";
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
Axis 0:
1st quartile:
[[2.75, 1.75, 4.25,    5,    3, 6.75]]
Median:
[[  5, 2.5,   5, 5.5,   4,   8]]
3rd quartile:
[[7.25,  4.5,    6, 6.25,  5.5, 9.25]]
Axis 1:
1st quartile:
[[ 3.5],
 [ 3.5],
 [2.25],
 [   5]]
Median:
[[5.5],
 [  6],
 [  4],
 [5.5]]
3rd quartile:
[[ 7.5],
 [   7],
 [ 6.5],
 [8.25]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << "1st quartile:\n";
    std::cout << np::quantile(a, 0.25, np::make_shape(1, 2)) << "\n";
    std::cout << "Median:\n";
    std::cout << np::quantile(a, 0.5, np::make_shape(1, 2)) << "\n";
    std::cout << "3rd quartile:\n";
    std::cout << np::quantile(a, 0.75, np::make_shape(1, 2)) << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
1st quartile:
[[[   2]],

 [[2.75]]]
Median:
[[[  3]],

 [[3.5]]]
3rd quartile:
[[[ 3.5]],

 [[5.25]]]
```

### `cov`

Return the covariance of two 1-dimensional tensors.
```cpp
template <class T>
T cov(const tensor<T, 1> &x, const tensor<T, 1> &y, bool bias = false);

template <class T>
std::complex<T> cov(
    const tensor<std::complex<T>, 1> &x, const tensor<std::complex<T>, 1> &y,
    bool bias = false
);
```

The covariance is defined as the average of the element-wise products of the
deviations from the mean
```
   cov(x, y) = mean(a),    a = (x - mean(x))*(y - mean(y))
```
For complex types, the complex conjugate of `y` is used
```
   cov(x, y) = mean(a),    a = (x - mean(x))*conj(y - mean(y))
```
The mean is calculated as `sum(a)/n`, where `n = a.size()`. However, if `bias`
is `false`, the divisor `n - 1` is used instead of `n`.

Parameters

* `x` First tensor-like argument.
* `y` Second tensor-like argument.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`. Default is `false`.

Returns

* The covariance of the two tensors.

Exceptions

* `std::invalid_argument` Thrown if the tensor arguments have different sizes.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
    std::cin >> x >> y;
    std::cout << "Unbiased: " << np::cov(x, y) << "\n";
    std::cout << "Biased: " << np::cov(x, y, true) << "\n";
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
Unbiased: -4.33333
Biased: -3.9
```

### `cov`

Return the covariance matrix of given data.
```cpp
template <class T>
tensor<T, 2> cov(const tensor<T, 2> &a, bool rowvar = true, bool bias = false);

template <class T>
tensor<std::complex<T>, 2> cov(
    const tensor<std::complex<T>, 2> &a, bool rowvar = true, bool bias = false
);
```

Parameters

* `a` A 2-dimensional tensor-like object containing multiple variables and
observations.
* `rowvar` If `true`, then each row represents a variable, with observations in
the columns. Otherwise, each column represents a variable, with observations in
the rows. Default is `true`.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`. Default is `false`.

Returns

* The covariance matrix of the variables. The element `(i, j)` is equal to the
covariance of the `i`-th and `j`-th variables.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Unbiased:\n" << np::cov(a) << "\n";
    std::cout << "Biased:\n" << np::cov(a, true, true) << "\n";
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
Unbiased:
[[  7.7777778,  -4.3333333,  -1.6666667,   2.4444444],
 [ -4.3333333,        52.9, 0.022222222,  -27.455556],
 [ -1.6666667, 0.022222222,        11.6,  -6.5777778],
 [  2.4444444,  -27.455556,  -6.5777778,   35.566667]]
Biased:
[[     7,   -3.9,   -1.5,    2.2],
 [  -3.9,  47.61,   0.02, -24.71],
 [  -1.5,   0.02,  10.44,  -5.92],
 [   2.2, -24.71,  -5.92,  32.01]]
```

### `corrcoef`

Return the Pearson's correlation coefficient of two 1-dimensional tensors.
```cpp
template <class T>
T corrcoef(const tensor<T, 1> &x, const tensor<T, 1> &y);
```

The correlation coefficient is defined as the covariance of the two variables
divided by the product of their standard deviations
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> x, y;
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

### `corrcoef`

Return the correlation matrix of given data.
```cpp
template <class T>
tensor<T, 2> corrcoef(const tensor<T, 2> &a, bool rowvar = true);
```

Parameters

* `a` A 2-dimensional tensor-like object containing multiple variables and
observations.
* `rowvar` If `true`, then each row represents a variable, with observations in
the columns. Otherwise, each column represents a variable, with observations in
the rows. Default is `true`.

Returns

* The correlation matrix of the variables. The element `(i, j)` is equal to the
correlation coefficient of the `i`-th and `j`-th variables.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
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
