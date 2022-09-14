# Tensor view

Defined in `numcpp/tensor.h`

- [Tensor view](#tensor-view)
  - [Template parameters](#template-parameters)
  - [Aliases](#aliases)
  - [Member types](#member-types)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Tensor constructor](#tensor-constructor)
    - [Slice constructor](#slice-constructor)
    - [Copy constructor](#copy-constructor)
    - [Move constructor](#move-constructor)
  - [Destructor](#destructor)
  - [Indexing](#indexing)
    - [`tensor_view::operator()`](#tensor_viewoperator)
    - [`tensor_view::operator[]`](#tensor_viewoperator-1)
    - [`tensor_view::ndim`](#tensor_viewndim)
    - [`tensor_view::shape`](#tensor_viewshape)
    - [`tensor_view::size`](#tensor_viewsize)
    - [`tensor_view::empty`](#tensor_viewempty)
    - [`tensor_view::data`](#tensor_viewdata)
    - [`tensor_view::offset`](#tensor_viewoffset)
    - [`tensor_view::strides`](#tensor_viewstrides)
    - [`tensor_view::layout`](#tensor_viewlayout)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
    - [Fill assignment](#fill-assignment)
    - [Move assignment](#move-assignment)
  - [Resizing](#resizing)
    - [`tensor_view::squeeze`](#tensor_viewsqueeze)
  - [Transpose](#transpose)
    - [`tensor_view::swapaxes`](#tensor_viewswapaxes)
    - [`tensor_view::t`](#tensor_viewt)

```cpp
template <class T, size_t Rank> class tensor_view;
```
A `tensor_view` is just a view of a multidimensional array. It references the
elements in the original array. The view itself does not own the data and any
changes made to the view will affect the original array, and any changes made
to the original array will affect the view.

## Template parameters

* `T` Type of the elements contained in the `tensor_view`. This shall be an
arithmetic type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the `tensor_view`. It must be a positive integer.

## Aliases

The main header `numcpp.h` offers the following aliases for `tensor_view` class.
```cpp
template <class T>
using array_view = tensor<T, 1>;

template <class T>
using matrix_view = tensor<T, 2>;
```

## Member types

| Member type       | Definition                            |
| ----------------- | ------------------------------------- |
| `value_type`      | `typename std::remove_cv<T>::type`    |
| `reference`       | `T&`                                  |
| `const_reference` | `const T&`                            |
| `pointer`         | `T*`                                  |
| `const_pointer`   | `const T*`                            |
| `iterator`        | A random access iterator to `T`       |
| `const_iterator`  | A random access iterator to `const T` |
| `size_type`       | An unsigned integral type.            |
| `difference_type` | A signed integral type.               |
| `shape_type`      | `shape_t<Rank>`                       |
| `index_type`      | `index_t<Rank>`                       |

## Constructors

### Default constructor

Constructs a `tensor_view` that does not reference any object.
```cpp
tensor_view();
```

### Tensor constructor

Constructs a `tensor_view` that references a multidimensional array.
```cpp
tensor_view(const shape_t<Rank> &shape, T *data, layout_t order = row_major);
```

Parameters

* `shape` Number of elements along each axis.
* `data` Pointer to the memory array used by the `tensor_view`.
* `order` Memory layout in which elements are stored. In row-major order, the
last dimension is contiguous. In column-major order, the first dimension is
contiguous. Defaults to row-major order.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data1d[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    np::array_view<int> arr(10, data1d);
    std::cout << "1 dimensional:\n" << arr << "\n";

    int data2d[12] = {0, 10, -4, 5,
                      6, 10, 8, 12,
                      2, 11, 0, -1};
    np::matrix_view<int> mat1({3, 4}, data2d);
    std::cout << "2 dimensional (row-major):\n" << mat1 << "\n";
    np::matrix_view<int> mat2({3, 4}, data2d, np::col_major);
    std::cout << "2 dimensional (column-major):\n" << mat2 << "\n";

    int data3d[24] = {1, 18, 11, 10,
                      9, 19, 12, 10,
                      13, 8, -4, 16,

                      2, 4, 14, 19,
                      18, 5, 19, 18,
                      0, 0, 15, 17};
    np::tensor_view<int, 3> cube1({2, 3, 4}, data3d);
    std::cout << "3 dimensional (row-major):\n" << cube1 << "\n";
    np::tensor_view<int, 3> cube2({2, 3, 4}, data3d, np::col_major);
    std::cout << "3 dimensional (column-major):\n" << cube2 << "\n";

    return 0;
}
```

Output

```
1 dimensional:
[-4, 16, 14,  9, 18,  3,  7,  2,  1,  4]
2 dimensional (row-major):
[[ 0, 10, -4,  5],
 [ 6, 10,  8, 12],
 [ 2, 11,  0, -1]]
2 dimensional (column-major):
[[ 0,  5,  8, 11],
 [10,  6, 12,  0],
 [-4, 10,  2, -1]]
3 dimensional (row-major):
[[[ 1, 18, 11, 10],
  [ 9, 19, 12, 10],
  [13,  8, -4, 16]],

 [[ 2,  4, 14, 19],
  [18,  5, 19, 18],
  [ 0,  0, 15, 17]]]
3 dimensional (column-major):
[[[ 1, 12,  2, 19],
  [11, 13, 14,  0],
  [ 9, -4, 18, 15]],

 [[18, 10,  4, 18],
  [10,  8, 19,  0],
  [19, 16,  5, 17]]]
```

### Slice constructor

Constructs a `tensor_view` that references a subset of elements from a
multidimensional array.
```cpp
tensor_view(
    const shape_t<Rank> &shape, T *data,
    size_t offset, const shape_t<Rank> &strides, layout_t order = row_major
);
```

Parameters

* `shape` Number of elements along each axis.
* `data` Pointer to the memory array used by the `tensor_view`.
* `offset` Index of the first element selected by the `tensor_view`.
* `strides` Span that separates the selected elements along each axis.
* `order` Order in which elements shall be iterated. In row-major order, the
last index is varying the fastest. In column-major order, the first index is
varying the fastest. Defaults to row-major order.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    // Size is 3, offset is 7, stride is 1.
    // Select positions 7, 8, 9.
    np::array_view<int> view1(3, data, 7, 1);
    std::cout << view1 << "\n";
    // Size is 5, offset is 0, stride is 2.
    // Select positions 0, 2, 4, 6, 8.
    np::array_view<int> view2(5, data, 0, 2);
    std::cout << view2 << "\n";
    return 0;
}
```

Output

```
[ 7, 11,  3]
[ 7, 19,  5, -2, 11]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[4*6] = {15, -4, 3, 18, -2, 7,
                     8, 11, 19, 0, -5, 14,
                     16, 19, 9, 12, 12, 18,
                     -5, 11, 5, 10, 8, 10};
    // Sizes are (4, 2), offset is 4, strides are (6, 1).
    // Select positions
    //   4 + 0*6 + 0*1 = 4,  4 + 0*6 + 1*1 = 5,
    //   4 + 1*6 + 0*1 = 10, 4 + 1*6 + 1*1 = 11,
    //   4 + 2*6 + 0*1 = 16, 4 + 2*6 + 1*1 = 17,
    //   4 + 3*6 + 0*1 = 22, 4 + 3*6 + 1*1 = 23
    np::matrix_view<int> view1({4, 2}, data, 4, {6, 1});
    std::cout << view1 << "\n";
    // Sizes are (1, 3), offset is 6, strides are (0, 2).
    // Select positions
    //   6 + 0*0 + 0*2 = 6, 6 + 0*0 + 1*2 = 8, 6 + 0*0 + 2*2 = 10
    np::matrix_view<int> view2({1, 3}, data, 6, {0, 2});
    std::cout << view2 << "\n";
    return 0;
}
```

Output

```
[[-2,  7],
 [-5, 14],
 [12, 18],
 [ 8, 10]]
[[ 8, 19, -5]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[2*3*4] = {-5, 19, 5, 18,
                       13, 1, 9, 14,
                       15, 12, 14, 16,

                       2, 14, -2, 3,
                       18, 11, 9, 18,
                       6, 19, -2, 1};
    // Sizes are (2, 3, 2), offset is 1, strides are (12, 4, 1).
    // Select positions
    //   1 + 0*12 + 0*4 + 0*2 = 1,  1 + 0*12 + 0*4 + 1*2 = 3,
    //   1 + 0*12 + 1*4 + 0*2 = 5,  1 + 0*12 + 1*4 + 1*2 = 7,
    //   1 + 0*12 + 2*4 + 0*2 = 9,  1 + 0*12 + 2*4 + 1*2 = 11,
    //
    //   1 + 1*12 + 0*4 + 0*2 = 13, 1 + 1*12 + 0*4 + 1*2 = 15,
    //   1 + 1*12 + 1*4 + 0*2 = 17, 1 + 1*12 + 1*4 + 1*2 = 19,
    //   1 + 1*12 + 2*4 + 0*2 = 21, 1 + 1*12 + 2*4 + 1*2 = 23
    np::tensor_view<int, 3> view({2, 3, 2}, data, 1, {12, 4, 2});
    std::cout << view << "\n";
    return 0;
}
```

Output

```
[[[19, 18],
  [ 1, 14],
  [12, 16]],

 [[14,  3],
  [11, 18],
  [19,  1]]]
```

### Copy constructor

Constructs a `tensor_view` as a copy of other.
```cpp
tensor_view(const tensor_view &other);
```

Parameters

* `other` A `tensor_view` of the same type and rank.

### Move constructor

Constructs a `tensor_view` that acquires the elements of other.
```cpp
tensor_view(tensor_view &&other);
```

Parameters

* `other` A `tensor_view` of the same type and rank. `other` is left in an
empty state.

## Destructor

```cpp
~tensor_view();
```

## Indexing

### `tensor_view::operator()`

Returns a reference to the element at the given position. The elements in a
`tensor_view` are given by

```
    data[offset + index[0]*stride[0] + ... + index[N-1]*stride[N-1]]
```

where `data` is the memory array.
```cpp
template <class... Index>
T& operator()(Index... index);

template <class... Index>
const T& operator()(Index... index) const;
```

Parameters

* `index...` Position of an element along each axis.

Returns

* The element at the specified position. If the `tensor_view` is
const-qualified, the function returns a reference to `const T`. Otherwise, it
returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if `index` is out of bounds.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    np::array_view<int> view(3, data, 7, 1);
    for (unsigned i = 0; i < view.size(); ++i) {
        // Prints data[7 + i*1]
        std::cout << view(i) << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
7, 11, 3,
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[4*6] = {15, -4, 3, 18, -2, 7,
                     8, 11, 19, 0, -5, 14,
                     16, 19, 9, 12, 12, 18,
                     -5, 11, 5, 10, 8, 10};
    np::matrix_view<int> view({4, 2}, data, 4, {6, 1});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            // Prints data[4 + i*6 + j*1]
            std::cout << view(i, j) << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
-2, 7,
-5, 14,
12, 18,
8, 10,
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[2*3*4] = {-5, 19, 5, 18,
                       13, 1, 9, 14,
                       15, 12, 14, 16,

                       2, 14, -2, 3,
                       18, 11, 9, 18,
                       6, 19, -2, 1};
    np::tensor_view<int, 3> view({2, 3, 2}, data, 1, {12, 4, 2});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            for (unsigned k = 0; k < view.shape(2); ++k) {
                // Prints data[1 + i*12 + j*4 + k*2]
                std::cout << view(i, j, k) << ", ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
19, 18,
1, 14,
12, 16,

14, 3,
11, 18,
19, 1,

```

### `tensor_view::operator[]`

Returns a reference to the element at the given position.
```cpp
T& operator[](const index_t<Rank> &index);
const T& operator[](const index_t<Rank> &index) const;

T& operator[](size_t i);
const T& operator[](size_t i) const;
```

Parameters

* `index` An `index_t` object with the position of an element in the
`tensor_view`. If the tensor is one dimensional, an integer can be used
instead.

Returns

* The element at the specified position. If the `tensor_view` is
const-qualified, the function returns a reference to `const T`. Otherwise, it
returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if `index` is out of bounds.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    np::array_view<int> view(3, data, 7, 1);
    for (unsigned i = 0; i < view.size(); ++i) {
        // Prints data[7 + i*1]
        std::cout << view[i] << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
7, 11, 3,
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[4*6] = {15, -4, 3, 18, -2, 7,
                     8, 11, 19, 0, -5, 14,
                     16, 19, 9, 12, 12, 18,
                     -5, 11, 5, 10, 8, 10};
    np::matrix_view<int> view({4, 2}, data, 4, {6, 1});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            // Prints data[4 + i*6 + j*1]
            std::cout << view[{i, j}] << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
-2, 7,
-5, 14,
12, 18,
8, 10,
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[2*3*4] = {-5, 19, 5, 18,
                       13, 1, 9, 14,
                       15, 12, 14, 16,

                       2, 14, -2, 3,
                       18, 11, 9, 18,
                       6, 19, -2, 1};
    np::tensor_view<int, 3> view({2, 3, 2}, data, 1, {12, 4, 2});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            for (unsigned k = 0; k < view.shape(2); ++k) {
                // Prints data[1 + i*12 + j*4 + k*2]
                std::cout << view[{i, j, k}] << ", ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
19, 18,
1, 14,
12, 16,

14, 3,
11, 18,
19, 1,

```

### `tensor_view::ndim`

Return the dimension of the `tensor_view`.
```cpp
static constexpr size_t ndim();
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    std::cout << np::array_view<int>::ndim() << "\n";
    std::cout << np::matrix_view<int>::ndim() << "\n";
    std::cout << np::tensor_view<int, 3>::ndim() << "\n";
    return 0;
}
```

Output

```
1
2
3
```

### `tensor_view::shape`

Return the shape of the `tensor_view`.
```cpp
const shape_t<Rank>& shape() const;
size_t shape(size_t axis) const;
```

Parameters

* `axis` It is an optional parameter that changes the return value. If
provided, returns the size along the given axis. Otherwise, return a `shape_t`
object with the shape of the tensor along all axes.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int ptr[12] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4, 11, 5};
    np::array_view<int> arr(12, ptr);
    std::cout << "1 dimensional:\n";
    std::cout << "Shape: " << arr.shape() << "\n";
    std::cout << "Length: " << arr.shape(0) << "\n\n";

    np::matrix_view<int> mat({3, 4}, ptr);
    std::cout << "2 dimensional:\n";
    std::cout << "Shape: " << mat.shape() << "\n";
    std::cout << "Rows: " << mat.shape(0) << "\n";
    std::cout << "Columns: " << mat.shape(1) << "\n\n";

    np::tensor_view<int, 3> cube({2, 2, 3}, ptr);
    std::cout << "3 dimensional:\n";
    std::cout << "Shape: " << cube.shape() << "\n";
    std::cout << "Depth: " << cube.shape(0) << "\n";
    std::cout << "Rows: " << cube.shape(1) << "\n";
    std::cout << "Columns: " << cube.shape(2) << "\n";
    return 0;
}
```

Output

```
1 dimensional:
Shape: (12,)
Length: 12

2 dimensional:
Shape: (3, 4)
Rows: 3
Columns: 4

3 dimensional:
Shape: (2, 2, 3)
Depth: 2
Rows: 2
Columns: 3
```

### `tensor_view::size`

Return the number of elements in the `tensor_view` (i.e., the product of the
sizes along all the axes).
```cpp
size_t size() const;
```

Notes

* Time complexity: $O(1)$

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int ptr[12] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4, 11, 5};
    np::array_view<int> arr(12, ptr);
    std::cout << arr.size() << "\n";
    np::matrix_view<int> mat({3, 4}, ptr);
    std::cout << mat.size() << "\n";
    np::tensor_view<int, 3> cube({2, 2, 3}, ptr);
    std::cout << cube.size() << "\n";
    return 0;
}
```

Output

```
12
12
12
```

### `tensor_view::empty`

Returns whether the `tensor_view` is empty (i.e., whether its size is 0).
```cpp
bool empty() const;
```

### `tensor_view::data`

Returns a pointer to the memory array used internally by the `tensor_view`.
```cpp
T* data();
const T* data() const;
```

Returns

* A pointer to the memory array used internally by the `tensor_view`. If the
`tensor_view` is const-qualified, the function returns a pointer to `const T`.
Otherwise, it returns a pointer to `T`.

### `tensor_view::offset`

Returns the position in the memory array of the first element.
```cpp
size_t offset() const;
```

### `tensor_view::strides`

Returns the span that separates the elements in the memory array.
```cpp
const shape_t<Rank>& strides() const;
size_t strides(size_t axis) const;
```

Parameters

* `axis` It is an optional parameter that changes the return value. If
provided, returns the stride along the given axis. Otherwise, return a `shape_t`
object with the strides of the tensor along all axes.

### `tensor_view::layout`

Returns the order in which elements are iterated. It is not necessarily the
memory layout in which elements are stored as the elements might not be
continuous in memory.
```cpp
layout_t layout() const;
```

## Assignment

### Copy assignment

Assigns to each element the value of the corresponding element in `other`.
```cpp
template <class U>
tensor_view& operator=(const tensor<U, Rank> &other);
```

Parameters

* `other` A tensor-like object of the same shape.

Returns

* `*this`

Exceptions

* `std::invalid_argument` Thrown if the shapes are different.

### Fill assignment

Assigns `val` to every element.
```cpp
tensor_view& operator=(const T &val);
```

Parameters

* `val` Value assigned to all the elements in the `tensor_view`.

Returns

* `*this`

### Move assignment

Acquires the contents of `other`, leaving `other` in an empty state.
```cpp
tensor_view& operator=(base_tensor &&other);
```

Parameters

* `other` A `tensor_view` of the same type and rank. `other` is left in an
empty state.

Returns

* `*this`

## Resizing

### `tensor_view::squeeze`

Removes axes of length one.
```cpp
template <size_t N>
tensor_view<T, Rank - N> squeeze(const shape_t<N> &axes);
template <class... Axes>
tensor_view<T, Rank - sizeof...(Axes)> squeeze(Axes... axes);

template <size_t N>
tensor_view<const T, Rank - N> squeeze(const shape_t<N> &axes) const;
template <class... Axes>
tensor_view<const T, Rank - sizeof...(Axes)> squeeze(Axes... axes) const;
```

Parameters

* `axes` Selects a subset of the entries of length one in the shape. It can be
a `shape_t` object or the elements of the shape passed as separate arguments.

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to
`const T`, which is convertible to a tensor object. Otherwise, the function
returns a `tensor_view` to `T`, which has reference semantics to the original
tensor.

Exceptions

* `std::invalid_argument` Thrown if an axis with shape entry greater than one
is selected.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    np::tensor_view<int, 4> view4d({1, 3, 1, 4}, data);
    std::cout << "4 dimensional:\n";
    std::cout << view4d.shape() << "\n";
    std::cout << view4d << "\n";
    np::matrix_view<int> view2d = view4d.squeeze(0, 2);
    std::cout << "2 dimensional:\n";
    std::cout << view2d.shape() << "\n";
    std::cout << view2d << "\n";
    return 0;
}
```

Output

```
4 dimensional:
(1, 3, 1, 4)
[[[[ 0,  1,  2,  3]],

  [[ 4,  5,  6,  7]],

  [[ 8,  9, 10, 11]]]]
2 dimensional:
(3, 4)
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
```

## Transpose

### `tensor_view::swapaxes`

Interchanges two axes of a `tensor_view` in-place.
```cpp
void swapaxes(size_t axis1, size_t axis2);
```

Parameters

* `axis1` First axis.
* `axis2` Second axis.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data2d[3*4] = {1, 14, 12, -3,
                       -5, -3, 11, 11,
                       -1, 18, -3, -1};
    np::matrix_view<int> mat({3, 4}, data2d);
    std::cout << "2 dimensional:\n";
    std::cout << mat << "\n";
    mat.swapaxes(0, 1);
    std::cout << mat << "\n";

    int data3d[2*3*4] = {16, 15, 14, -1,
                         5, 14, 9, 10,
                         18, 15, 2, 5,

                         11, 6, 19, -2,
                         7, 10, 1, -2,
                         14, 7, -2, 11};
    np::tensor_view<int, 3> cube({2, 3, 4}, data3d);
    std::cout << "3 dimensional:\n";
    std::cout << cube << "\n";
    cube.swapaxes(1, 2); // Swap last 2 axes.
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
2 dimensional:
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
[[ 1, -5, -1],
 [14, -3, 18],
 [12, 11, -3],
 [-3, 11, -1]]
3 dimensional:
[[[16, 15, 14, -1],
  [ 5, 14,  9, 10],
  [18, 15,  2,  5]],

 [[11,  6, 19, -2],
  [ 7, 10,  1, -2],
  [14,  7, -2, 11]]]
[[[16,  5, 18],
  [15, 14, 15],
  [14,  9,  2],
  [-1, 10,  5]],

 [[11,  7, 14],
  [ 6, 10,  7],
  [19,  1, -2],
  [-2, -2, 11]]]
```

### `tensor_view::t`

Return a view of the tensor with its axes in reversed order.
```cpp
tensor_view<T, Rank> t();
tensor_view<const T, Rank> t() const;
```

Parameters

* None

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to
`const T`, which is convertible to a tensor object. Otherwise, the function
returns a `tensor_view` to `T`, which has reference semantics to the original
tensor.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data2d[3*4] = {1, 14, 12, -3,
                       -5, -3, 11, 11,
                       -1, 18, -3, -1};
    np::matrix_view<int> mat({3, 4}, data2d);
    std::cout << "2 dimensional:\n";
    std::cout << mat << "\n";
    std::cout << mat.t() << "\n";

    int data3d[2*3*4] = {16, 15, 14, -1,
                         5, 14, 9, 10,
                         18, 15, 2, 5,

                         11, 6, 19, -2,
                         7, 10, 1, -2,
                         14, 7, -2, 11};
    np::tensor_view<int, 3> cube({2, 3, 4}, data3d);
    std::cout << "3 dimensional:\n";
    std::cout << cube << "\n";
    std::cout << cube.t() << "\n";
    return 0;
}
```

Output

```
2 dimensional:
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
[[ 1, -5, -1],
 [14, -3, 18],
 [12, 11, -3],
 [-3, 11, -1]]
3 dimensional:
[[[16, 15, 14, -1],
  [ 5, 14,  9, 10],
  [18, 15,  2,  5]],

 [[11,  6, 19, -2],
  [ 7, 10,  1, -2],
  [14,  7, -2, 11]]]
[[[16, 11],
  [ 5,  7],
  [18, 14]],

 [[15,  6],
  [14, 10],
  [15,  7]],

 [[14, 19],
  [ 9,  1],
  [ 2, -2]],

 [[-1, -2],
  [10, -2],
  [ 5, 11]]]
```
