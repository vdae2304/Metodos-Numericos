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
    - [`tensor_view::rowmajor`](#tensor_viewrowmajor)
    - [`tensor_view::colmajor`](#tensor_viewcolmajor)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
    - [Fill assignment](#fill-assignment)
    - [Move assignment](#move-assignment)

```cpp
template <class T, size_t Rank> class tensor_view<T, Rank>;
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
| `difference_type` | A signed integral type.               |
| `size_type`       | An unsigned integral type.            |

## Constructors

### Default constructor

Constructs a `tensor_view` that does not reference any object.
```cpp
tensor_view();
```

### Tensor constructor

Constructs a `tensor_view` that references a multidimensional array.
```cpp
tensor_view(const shape_t<Rank> &shape, T *data, bool order = true);
```

Parameters

* `shape` Number of elements along each axis.
* `data` Pointer to the memory array used by the `tensor_view`.
* `order` If `true` (default), the elements are stored in row-major order (from
first axis to last axis). Otherwise, the elements are stored in column-major
order (from last axis to first axis).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data1d[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    np::array_view<int> array(10, data1d);
    std::cout << "1 dimensional:\n" << array << "\n";
    int data2d[12] = {0, 10, -4, 5,
                      6, 10, 8, 12,
                      2, 11, 0, -1};
    np::matrix_view<int> matrix_rowmajor({3, 4}, data2d);
    std::cout << "2 dimensional (row-major):\n" << matrix_rowmajor << "\n";
    np::matrix_view<int> matrix_colmajor({3, 4}, data2d, false);
    std::cout << "2 dimensional (column-major):\n" << matrix_colmajor << "\n";
    int data3d[24] = {1, 18, 11, 10,
                      9, 19, 12, 10,
                      13, 8, -4, 16,

                      2, 4, 14, 19,
                      18, 5, 19, 18,
                      0, 0, 15, 17};
    np::tensor_view<int, 3> cube_rowmajor({2, 3, 4}, data3d);
    std::cout << "3 dimensional (row-major):\n" << cube_rowmajor << "\n";
    np::tensor_view<int, 3> cube_colmajor({2, 3, 4}, data3d, false);
    std::cout << "3 dimensional (column-major):\n" << cube_colmajor << "\n";
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
    size_t offset, const shape_t<Rank> &strides, bool order = true
);
```

Parameters

* `shape` Number of elements along each axis.
* `data` Pointer to the memory array used by the `tensor_view`.
* `offset` Index of the first element selected by the `tensor_view`.
* `strides` Span that separates the selected elements along each axis.
* `order` If `true` (default), the elements are stored in row-major order (from
first axis to last axis). Otherwise, the elements are stored in column-major
order (from last axis to first axis).

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
    // Sizes are (1, 3), offset is 6, strides are (0, 3).
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
template <class... Args>
T& operator()(Args... args);

template <class... Args>
const T& operator()(Args... args) const;
```

Parameters

* `args...` Index arguments.

Returns

* The element at the specified position. If the `tensor_view` is
const-qualified, the function returns a reference to `const T`. Otherwise, it
returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

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

* `std::out_of_range` Thrown if index is out of bounds.

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
constexpr size_t ndim() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int ptr[12] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4, 11, 5};
    np::array_view<int> array(12, ptr);
    std::cout << array.ndim() << "\n";
    np::matrix_view<int> matrix({3, 4}, ptr);
    std::cout << matrix.ndim() << "\n";
    np::tensor_view<int, 3> cube({2, 2, 3}, ptr);
    std::cout << cube.ndim() << "\n";
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
    np::array_view<int> array(12, ptr);
    std::cout << "1 dimensional:\n";
    std::cout << "Shape: " << array.shape() << "\n";
    std::cout << "Length: " << array.shape(0) << "\n\n";

    np::matrix_view<int> matrix({3, 4}, ptr);
    std::cout << "2 dimensional:\n";
    std::cout << "Shape: " << matrix.shape() << "\n";
    std::cout << "Rows: " << matrix.shape(0) << "\n";
    std::cout << "Columns: " << matrix.shape(1) << "\n\n";

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
    np::array_view<int> array(12, ptr);
    std::cout << array.size() << "\n";
    np::matrix_view<int> matrix({3, 4}, ptr);
    std::cout << matrix.size() << "\n";
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

### `tensor_view::rowmajor`

Returns whether the elements are stored in row-major order.
```cpp
bool rowmajor() const;
```

### `tensor_view::colmajor`

Returns whether the elements are stored in column-major order.
```cpp
bool colmajor() const;
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
