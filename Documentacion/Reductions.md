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
    std::cout << std::boolalpha;
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
    std::cout << "Last 2 axes:\n" << std::boolalpha;
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
