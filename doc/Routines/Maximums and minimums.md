# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Maximums and minimums](#maximums-and-minimums)
    - [`argmax`](#argmax)
    - [`argmin`](#argmin)
    - [`amax`](#amax)
    - [`amin`](#amin)
    - [`maximum`](#maximum)
    - [`minimum`](#minimum)
    - [`clamp`](#clamp)

## Maximums and minimums

### `argmax`

Return the index of the maximum value in the tensor.
```cpp
template <class T, size_t Rank>
index_t<Rank> argmax(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The index of the maximum value in the tensor. In case of multiple occurrences of the maximum value, return the index corresponding to the first occurrence.

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
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
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

<h3><code>argmax</code></h3>

Return the indices of the maximum value in the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank - 1> argmax(const tensor<T, Rank> &a,
                                const shape_t<1> &axis);

template <class T, size_t Rank>
tensor<size_t, Rank> argmax(const tensor<T, Rank> &a,
                            const shape_t<1> &axis, keepdims_t);

template <class T, size_t Rank>
tensor<size_t, Rank - 1> argmax(const tensor<T, Rank> &a,
                                const shape_t<1> &axis, dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the maximum value is selected.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the indices of the maximum value along the given axis.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/broadcasting.h> // np::take_along_axis
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t axis;
    std::cin >> a >> axis;
    // For axis=0, compute maximum over all rows (column-wise).
    // For axis=1, compute maximum over all columns (row-wise).
    np::matrix<size_t> indices = np::argmax(a, axis, np::keepdims);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, axis) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
0
```

Output

```
Indices:
[[0, 3, 0, 0, 0, 2]]
Values:
[[13, 14,  2,  7,  7, 14]]
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
1
```

Output

```
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

Return the index of the minimum value in the tensor.
```cpp
template <class T, size_t Rank>
index_t<Rank> argmin(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The index of the minimum value in the tensor. In case of multiple occurrences of the minimum value, return the index corresponding to the first occurrence.

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
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
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

<h3><code>argmin</code></h3>

Return the indices of the minimum value in the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank - 1> argmin(const tensor<T, Rank> &a,
                                const shape_t<1> &axis);

template <class T, size_t Rank>
tensor<size_t, Rank> argmin(const tensor<T, Rank> &a,
                            const shape_t<1> &axis, keepdims_t);

template <class T, size_t Rank>
tensor<size_t, Rank - 1> argmin(const tensor<T, Rank> &a,
                                const shape_t<1> &axis, dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axis` Axis along which the minimum value is selected.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the indices of the minimum value along the given axis.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/broadcasting.h> // np::take_along_axis
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t axis;
    std::cin >> a >> axis;
    // For axis=0, compute minimum over all rows (column-wise).
    // For axis=1, compute minimum over all columns (row-wise).
    np::matrix<size_t> indices = np::argmin(a, axis, np::keepdims);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, axis) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
0
```

Output

```
Indices:
[[2, 1, 2, 3, 1, 3]]
Values:
[[ 3,  2, -2,  1, -5, -3]]
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
1
```

Output

```
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
T amax(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The maximum value in the tensor.

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
    std::cout <<  np::amax(a) << "\n";
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
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
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

<h3><code>amax</code></h3>

Return the maximum value contained in the tensor over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> amax(const tensor<T, Rank> &a, const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> amax(const tensor<T, Rank> &a, const shape_t<N> &axes,
                     keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> amax(const tensor<T, Rank> &a, const shape_t<N> &axes,
                         dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the maximum value is selected.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the maximum value over the given axes.

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
    // For axis=0, compute maximum over all rows (column-wise).
    // For axis=1, compute maximum over all columns (row-wise).
    std::cout <<  np::amax(a, axis) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
0
```

Output

```
[13, 14,  2,  7,  7, 14]
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
1
```

Output

```
[13,  8, 14, 14]
```

### `amin`

Return the minimum value contained in the tensor.
```cpp
template <class T, size_t Rank>
T amin(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The minimum value in the tensor.

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
    std::cout <<  np::amin(a) << "\n";
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
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
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

<h3><code>amin</code></h3>

Return the minimum value contained in the tensor over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> amin(const tensor<T, Rank> &a, const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> amin(const tensor<T, Rank> &a, const shape_t<N> &axes,
                     keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<T, Rank - N> amin(const tensor<T, Rank> &a, const shape_t<N> &axes,
                         dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which the minimum value is selected.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the minimum value over the given axes.

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
    // For axis=0, compute maximum over all rows (column-wise).
    // For axis=1, compute maximum over all columns (row-wise).
    std::cout <<  np::amin(a, axis) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
0
```

Output

```
[ 3,  2, -2,  1, -5, -3]
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
1
```

Output

```
[ 2, -5, -2, -3]
```

### `maximum`

Return the element-wise maximum of two tensors.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> maximum(const tensor<T, Rank> &a, const tensor<T, Rank> &b);

template <class T, size_t Rank>
tensor<T, Rank> maximum(const tensor<T, Rank> &a,
                        const typename tensor<T, Rank>::value_type &val);

template <class T, size_t Rank>
tensor<T, Rank> maximum(const typename tensor<T, Rank>::value_type &val,
                        const tensor<T, Rank> &b);
```

Parameters

* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.
* `val` Value to use either as first argument or second argument. Values are broadcasted to an appropriate shape.

Returns

* A light-weight object with the element-wise maximum of two tensors. This function does not create a new tensor, instead, an expression object is returned with the element-wise maximum of the arguments.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a, b;
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
tensor<T, Rank> minimum(const tensor<T, Rank> &a,
                        const typename tensor<T, Rank>::value_type &val);

template <class T, size_t Rank>
tensor<T, Rank> minimum(const typename tensor<T, Rank>::value_type &val,
                        const tensor<T, Rank> &b);
```

Parameters

* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.
* `val` Value to use either as first argument or second argument. Values are broadcasted to an appropriate shape.

Returns

* A light-weight object with the element-wise minimum of two tensors. This function does not create a new tensor, instead, an expression object is returned with the element-wise minimum of the arguments.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a, b;
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

Clamp the values in the tensor. Given an interval [`a_min`, `a_max`], values smaller than `a_min` become `a_min`, and values larger than `a_max` become `a_max`. For complex types, the real and imaginary parts are clamped separately.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> clamp(const tensor<T, Rank> &a,
                      const typename tensor<T, Rank>::value_type &a_min,
                      const typename tensor<T, Rank>::value_type &a_max);
```

Parameters

* `a` A tensor-like object with the elements to clamp.
* `a_min` The lower boundary to clamp.
* `a_max` The upper boundary to clamp.

Returns

* A light-weight object with the elements in the tensor clamped within the given interval. This function does not create a new tensor, instead, an expression object is returned with the clamped values in the tensor.

Notes

* The behavior is undefined if `a_min` is greater than `a_max`.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
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
