# [Broadcasting and indexing](readme.md)

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Indexing](#indexing)
    - [`ravel_index`](#ravel_index)
    - [`unravel_index`](#unravel_index)
    - [`take`](#take)
    - [`take_along_axis`](#take_along_axis)
    - [`put`](#put)
    - [`put_along_axis`](#put_along_axis)
    - [`compress`](#compress)
    - [`place`](#place)
    - [`putmask`](#putmask)

## Indexing

### `ravel_index`

Converts a tuple of indices into a flat index, element-wise.
```cpp
template <size_t Rank, size_t N>
tensor<size_t, N> ravel_index(const tensor<index_t<Rank>, N> &index,
                              const shape_t<Rank> &shape,
                              layout_t order = default_layout)
```

Parameters

* `index` A tensor-like object with the indices to flatten.
* `shape` The shape of the tensor used for raveling.
* `order` Determines whether the indices should be viewed as indexing in row-major or column-major order. Defaults to row-major order.

Returns

* A light-weight object with the flattened indices. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::shape_t<2> shape;
    np::matrix<np::index_t<2>> indices;
    std::cin >> shape >> indices;
    std::cout << "Row-major order:\n";
    std::cout << np::ravel_index(indices, shape) << "\n";
    std::cout << "Column-major order:\n";
    std::cout << np::ravel_index(indices, shape, np::column_major) << "\n";
    return 0;
}
```

Input

```
(3, 4)
[[(0, 0), (0, 1), (0, 2), (0, 3)],
 [(1, 0), (1, 1), (1, 2), (1, 3)],
 [(2, 0), (2, 1), (2, 2), (2, 3)]]
```

Output

```
Row-major order:
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
Column-major order:
[[ 0,  3,  6,  9],
 [ 1,  4,  7, 10],
 [ 2,  5,  8, 11]]
```

### `unravel_index`

Converts a flat index into a tuple of indices, element-wise.
```cpp
template <class IntegralType, size_t Rank, size_t N>
tensor<index_t<Rank>, N> unravel_index(const tensor<IntegralType, N> &index,
                                       const shape_t<Rank> &shape,
                                       layout_t order = default_layout);
```

Parameters

* `index` A tensor-like object with the indices to unravel.
* `shape` The shape of the tensor used for unraveling.
* `order` Determines whether the indices should be viewed as indexing in row-major or column-major order. Defaults to row-major order.

Returns

* A light-weight object with the unraveled indices. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::shape_t<2> shape;
    np::vector<size_t> indices;
    std::cin >> shape >> indices;
    std::cout << "Row-major order:\n";
    std::cout << np::unravel_index(indices, shape) << "\n";
    std::cout << "Column-major order:\n";
    std::cout << np::unravel_index(indices, shape, np::column_major) << "\n";
    return 0;
}
```

Input

```
(3, 4)
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
```

Output

```
Row-major order:
[(0, 0), (0, 1), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2), (1, 3), (2, 0), (2, 1), 
 (2, 2), (2, 3)]
Column-major order:
[(0, 0), (1, 0), (2, 0), (0, 1), (1, 1), (2, 1), (0, 2), (1, 2), (2, 2), (0, 3), 
 (1, 3), (2, 3)]
```

### `take`

Take elements from a tensor. For [`tensor class`](/doc/Tensor%20class/Tensor/readme.md), a call such as `np::take(a, indices)` is equivalent to `a[indices]`, except that a copy is always returned.
```cpp
template <class T, size_t Rank, size_t N>
tensor<T, N> take(const tensor<T, Rank> &a,
                  const tensor<index_t<Rank>, N> &indices);

template <class T, class IntegralType, size_t N>
tensor<T, N> take(const tensor<T, 1> &a,
                  const tensor<IntegralType, N> &indices);
```

Parameters

* `a` The source tensor.
* `indices` A tensor-like object of `index_t` with the indices of the values to take. If the source tensor is 1-dimensional, a tensor-like object of integers can be used instead.

Returns

* A new tensor with the elements from `a` at the given positions.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::vector<size_t> indices;
    std::cin >> vec >> indices;
    std::cout << np::take(vec, indices) << "\n";
    return 0;
}
```

Input

```
[7, 13, 19, 11, 5, 8, -2, 7, 11, 3]
[9, 4, 0, 7, 5]
```

Output

```
[3, 5, 7, 7, 8]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::vector<np::index_t<2>> indices;
    std::cin >> mat >> indices;
    std::cout << np::take(mat, indices) << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]
[(0, 0), (0, 1), (1, 1), (1, 2), (2, 2), (2, 3), (3, 3), (3, 4)]
```

Output

```
[15, -4, 11, 19,  9, 12, 10,  8]
```

<h3><code>take</code></h3>

Take elements from a tensor along an axis.
```cpp
template <class T, size_t Rank, class IntegralType>
tensor<T, Rank> take(const tensor<T, Rank> &a,
                     const tensor<IntegralType, 1> &indices, size_t axis);

template <class T, size_t Rank>
tensor<T, Rank - 1> take(const tensor<T, Rank> &a, size_t index, size_t axis);
```

Parameters

* `a` The source tensor.
* `indices` A tensor-like object with the indices of the values to take.
* `axis` The axis over which to select values.

Returns

* A new tensor with the elements from `a` at the given positions. If a single index is given, the output tensor will have a dimension less than the source tensor. Otherwise, the output tensor and the source tensor will have the same dimension and the same shape, except for the axis over which the values are taken.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::vector<size_t> indices;
    size_t axis;
    // For axis=0, select rows at the positions given by indices.
    // For axis=1, select columns at the positions given by indices.
    std::cin >> mat >> indices >> axis;
    std::cout << np::take(mat, indices, axis) << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]
[3, 0]
0
```

Output

```
[[-5, 11,  5, 10,  8, 10],
 [15, -4,  3, 18, -2,  7]]
```


Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]
[2, 5, 0]
1
```

Output

```
[[ 3,  7, 15],
 [19, 14,  8],
 [ 9, 18, 16],
 [ 5, 10, -5]]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    size_t index;
    size_t axis;
    std::cin >> mat >> index >> axis;
    // For axis=0, select row numbered index.
    // For axis=1, select column numbered index.
    std::cout << np::take(mat, index, axis) << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]
1
0
```

Output

```
[ 8, 11, 19,  0, -5, 14]
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]
4
1
```

Output

```
[-2, -5, 12,  8]
```

### `take_along_axis`

Take values from the input tensor by matching 1d index and data slices. Functions returning an index along an axis, like `argsort` and `argpartition`, produce suitable indices for this function.
```cpp
template <class T, size_t Rank, class IntegralType>
tensor<T, Rank> take_along_axis(const tensor<T, Rank> &a,
                                const tensor<IntegralType, Rank> &indices,
                                size_t axis);
```

Parameters

* `a` The source tensor.
* `indices` A tensor-like object with the indices to take along each 1d slice of `a`. This must match the shape of `a` for all dimensions other than `axis`.
* `axis` The axis to take 1d slices along.

Returns

* A new tensor with the elements from `a` at the given positions.

Exceptions

* `std::invalid_argument` Thrown if the shape of `indices` is not compatible with the shape of `a`.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::matrix<size_t> indices;
    size_t axis;
    std::cin >> mat >> indices >> axis;
    // For axis=0, select rows at the given positions for each column.
    // For axis=1, select columns at the given positions for each row.
    std::cout << np::take_along_axis(mat, indices, axis) << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]

[[3, 0, 0, 1, 0, 0],
 [2, 2, 1, 0, 2, 2]]

0
```

Output

```
[[-5, -4,  3,  0, -2,  7],
 [16, 19, 19, 18, 12, 18]]
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]

[[4, 3],
 [4, 2],
 [2, 1],
 [0, 1]]

1
```

Output

```
[[-2, 18],
 [-5, 19],
 [ 9, 19],
 [-5, 11]]
```

### `put`

Replaces specified elements of a tensor with given values. For [`tensor`](/doc/Tensor%20class/Tensor/readme.md) class, a call such as `np::put(a, indices, values)` is equivalent to `a[indices] = values`.
```cpp
template <class T, size_t Rank, size_t N>
void put(tensor<T, Rank> &a, const tensor<index_t<Rank>, N> &indices,
         const tensor<T, N> &values);

template <class T, size_t Rank, size_t N>
void put(tensor<T, Rank> &a, const tensor<index_t<Rank>, N> &indices,
         const typename tensor<T, Rank>::value_type &value);

template <class T, class IntegralType, size_t N>
void put(tensor<T, 1> &a, const tensor<IntegralType, N> &indices,
         const tensor<T, N> &values);

template <class T, class IntegralType, size_t N>
void put(tensor<T, 1> &a, const tensor<IntegralType, N> &indices,
         const typename tensor<T, 1>::value_type &value);
```

Parameters

* `a` Target tensor.
* `indices` A tensor-like object of `index_t` with the target indices. If the source tensor is 1-dimensional, a tensor-like object of integers can be used instead.
* `values` A single value or a tensor-like object with the values to put into `a` at target indices. Its shape is broadcast to match the shape of `indices`.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `values` is not compatible with the shape of `indices` according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::vector<size_t> indices;
    np::vector<int> values;
    std::cin >> vec >> indices >> values;
    np::put(vec, indices, values);
    std::cout << vec << "\n";
    return 0;
}
```

Input

```
[7, 13, 19, 11, 5, 8, -2, 7, 11, 3]
[9, 4, 0, 7, 5]
[10, 20, 30, 40, 50]
```

Output

```
[30, 13, 19, 11, 20, 50, -2, 40, 11, 10]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::vector<np::index_t<2>> indices;
    np::vector<int> values;
    std::cin >> mat >> indices >> values;
    np::put(mat, indices, values);
    std::cout << mat << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]
[(0, 0), (0, 1), (1, 1), (1, 2), (2, 2), (2, 3), (3, 3), (3, 4)]
[10, 20, 30, 40, 50, 60, 70, 80]
```

Output

```
[[10, 20,  3, 18, -2,  7],
 [ 8, 30, 40,  0, -5, 14],
 [16, 19, 50, 60, 12, 18],
 [-5, 11,  5, 70, 80, 10]]
```

### `put_along_axis`

Put values into the destination tensor by matching 1d index and data slices. Functions returning an index along an axis, like `argsort` and `argpartition`, produce suitable indices for this function.
```cpp
template <class T, size_t Rank, class IntegralType>
void put_along_axis(tensor<T, Rank> &a,
                    const tensor<IntegralType, Rank> &indices,
                    const tensor<T, Rank> &values, size_t axis);
```

Parameters

* `a` Destination tensor.
* `indices` A tensor-like object with the indices to change along each 1d slice of `a`. This must match the shape of `a` for all dimensions other than `axis`.
* `values` A tensor-like object with the values to insert at those indices. Its shape is broadcast to match the shape of `indices`.
* `axis` The axis to take 1d slices along.

Returns

* `std::invalid_argument` Thrown if the shape of `values` is not compatible with the shape of `indices` according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::matrix<size_t> indices;
    np::matrix<int> values;
    size_t axis;
    std::cin >> mat >> indices >> values >> axis;
    np::put_along_axis(mat, indices, values, axis);
    std::cout << mat << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]

[[3, 0, 0, 1, 0, 0],
 [2, 2, 1, 0, 2, 2]]

[[ 0, 10, 1 , 1,  0, 10],
 [10, 15, 9, 12, 10, 10]]

0
```

Output

```
[[15, 10,  1, 12,  0, 10],
 [ 8, 11,  9,  1, -5, 14],
 [10, 15,  9, 12, 10, 10],
 [ 0, 11,  5, 10,  8, 10]]
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19 , 0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10 , 8, 10]]

[[4, 3],
 [4, 2],
 [2, 1],
 [0, 1]]

[[ 1, 15],
 [ 0, 10],
 [11, 20],
 [ 1,  9]]

1
```

Output

```
[[15, -4,  3, 15,  1,  7],
 [ 8, 11, 10,  0,  0, 14],
 [16, 20, 11, 12, 12, 18],
 [ 1,  9,  5, 10,  8, 10]]
```

### `compress`

Return the elements of a tensor that satisfy some condition. For [`tensor`](/doc/Tensor%20class/Tensor/readme.md) class, a call such as `np::compress(a, condition)` is equivalent to `a[condition]`, except that a copy is always returned.
```cpp
template <class T, size_t Rank>
tensor<T, 1> compress(const tensor<T, Rank> &a,
                      const tensor<bool, Rank> &condition);
```

Parameters

* `a` The input tensor.
* `condition` A tensor-like object of `bool` whose true entries indicate the elements of `a` to return.

Returns

* A new tensor with the elements from `a` where `condition` is true.

Exceptions

* `std::invalid_argument` Thrown if the shape of `condition` does not match the shape of `a`.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    std::cin >> vec;
    // Select all elements greater than 10.
    std::cout << np::compress(vec, vec > 10) << "\n";
    return 0;
}
```

Input

```
[7, 13, 19, 11, 5, 8, -2, 7, 11, 3]
```

Output

```
[13, 19, 11, 11]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    std::cin >> mat;
    // Select all elements less or equal to zero.
    std::cout << np::compress(mat, mat <= 0) << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19,  0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10,  8, 10]]
```

Output

```
[-4, -2,  0, -5, -5]
```

<h3><code>compress</code></h3>

Return the elements of a tensor that satisfy some condition along given axis.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> compress(const tensor<T, Rank> &a,
                         const tensor<bool, 1> &condition,
                         size_t axis);
```

Parameters

* `a` The input tensor.
* `condition` A tensor-like object of `bool` whose true entries indicate the elements of `a` to return.
* `axis` The axis over which to select values.

Returns

* A new tensor with the elements from `a` where `condition` is true. The output tensor and the input tensor will have the same dimension and the same shape, except for the axis over which the values are selected.

Exceptions

* `std::invalid_argument` Thrown if the size of `condition` does not match the shape of `a` along the given axis.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::vector<bool> condition;
    size_t axis;
    // For axis=0, select rows where condition is true.
    // For axis=1, select columns where condition is false.
    std::cin >> mat >> std::boolalpha >> condition >> axis;
    std::cout << np::compress(mat, condition, axis) << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19,  0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10,  8, 10]]
[false, true, true, false]
0
```

Output

```
[[ 8, 11, 19,  0, -5, 14],
 [16, 19,  9, 12, 12, 18]]
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19,  0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10,  8, 10]]
[true, false, true, true, false, false]
1
```

Output

```
[[15,  3, 18],
 [ 8, 19,  0],
 [16,  9, 12],
 [-5,  5, 10]]
```

### `place`

Change elements of a tensor based on conditional and input values.
```cpp
template <class T, size_t Rank>
void place(tensor<T, Rank> &a, const tensor<bool, Rank> &condition,
           const tensor<T, 1> &values);

template <class T, size_t Rank>
void place(tensor<T, Rank> &a, const tensor<bool, Rank> &condition,
           const typename tensor<T, Rank>::value_type &value);
```

Parameters

* `a` Tensor to put data into.
* `condition` Boolean mask tensor.
* `values` A single value or a tensor-like object with the values to place in `a`. Only the first `N` elements are used, where `N` is the number of true values in `condition`.

Exceptions

* `std::invalid_argument` Thrown if the shape of `condition` does not match the shape of `a`.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::vector<int> values;
    std::cin >> vec >> values;
    np::place(vec, vec > 10, values);
    std::cout << vec << "\n";
    return 0;
}
```

Input

```
[7, 13, 19, 11, 5, 8, -2, 7, 11, 3]
[10, 20, 30, 40, 50]
```

Output

```
[ 7, 10, 20, 30,  5,  8, -2,  7, 40,  3]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::vector<int> values;
    std::cin >> mat >> values;
    np::place(mat, mat <= 0, values);
    std::cout << mat << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19,  0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10,  8, 10]]
[10, 20, 30, 40, 50]
```

Output

```
[[15, 10,  3, 18, 20,  7],
 [ 8, 11, 19, 30, 40, 14],
 [16, 19,  9, 12, 12, 18],
 [50, 11,  5, 10,  8, 10]]
```

### `putmask`

Change elements of a tensor based on conditional and input values. Similar to `place`, the difference is that `place` uses the first `N` elements of `values`, where `N` is the number of true values in `condition`, while `putmask` uses the elements where `condition` is true.
```cpp
template <class T, size_t Rank>
void putmask(tensor<T, Rank> &a, const tensor<bool, Rank> &condition,
             const tensor<T, Rank> &values);
```

Parameters

* `a` Tensor to put data into.
* `condition` Boolean mask tensor.
* `values` A tensor-like object with the values to put into `a` where `condition` is true. Its shape is broadcast to match the shape of `a`.

Exceptions

* `std::invalid_argument` Thrown if the shape of `condition` does not match the shape of `a`.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    std::cin >> vec;
    np::putmask(vec, vec > 10, vec % 10);
    std::cout << vec << "\n";
    return 0;
}
```

Input

```
[7, 13, 19, 11, 5, 8, -2, 7, 11, 3]
```

Output

```
[ 7,  3,  9,  1,  5,  8, -2,  7,  1,  3]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    std::cin >> mat;
    np::putmask(mat, mat <= 0, -mat);
    std::cout << mat << "\n";
    return 0;
}
```

Input

```
[[15, -4,  3, 18, -2,  7],
 [ 8, 11, 19,  0, -5, 14],
 [16, 19,  9, 12, 12, 18],
 [-5, 11,  5, 10,  8, 10]]
```

Output

```
[[15,  4,  3, 18,  2,  7],
 [ 8, 11, 19,  0,  5, 14],
 [16, 19,  9, 12, 12, 18],
 [ 5, 11,  5, 10,  8, 10]]
```
