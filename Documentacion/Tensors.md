# Tensor class

Defined in `numcpp/tensor.h`

- [Tensor class](#tensor-class)
  - [Template parameters](#template-parameters)
  - [Aliases](#aliases)
  - [Member types](#member-types)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Size constructor](#size-constructor)
    - [Fill constructor](#fill-constructor)
    - [Range constructor](#range-constructor)
    - [Copy constructor](#copy-constructor)
    - [Move constructor](#move-constructor)
    - [Initializer list constructor](#initializer-list-constructor)
  - [Destructor](#destructor)
  - [Indexing](#indexing)
    - [`tensor::operator()`](#tensoroperator)
    - [`tensor::operator[]`](#tensoroperator-1)
    - [`tensor::ndim`](#tensorndim)
    - [`tensor::shape`](#tensorshape)
    - [`tensor::size`](#tensorsize)
    - [`tensor::empty`](#tensorempty)
    - [`tensor::data`](#tensordata)
    - [`tensor::rowmajor`](#tensorrowmajor)
    - [`tensor::colmajor`](#tensorcolmajor)
  - [Advanced indexing](#advanced-indexing)
    - [Slice indexing](#slice-indexing)
    - [Coordinate tensor indexing](#coordinate-tensor-indexing)
    - [Boolean tensor indexing](#boolean-tensor-indexing)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
    - [Fill assignment](#fill-assignment)
    - [Move assignment](#move-assignment)
    - [Initializer list assignment](#initializer-list-assignment)

```cpp
template <class T, size_t Rank> class tensor<T, Rank>;
```
Tensors are contiguous multidimensional sequence containers: they hold a
specific number of elements arranged in multiple axis. Unlike a `tensor_view`,
a tensor is always owner of its own data and the elements are always stored in
row-major order.

Tensors are designed to easily perform mathematical operations on them. Most
mathematical operations can be applied directly to tensor objects, affecting
all its elements.

## Template parameters

* `T` Type of the elements contained in the tensor. This shall be an arithmetic
type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the tensor. It must be a positive integer.

## Aliases

The main header `numcpp.h` offers the following aliases for `tensor` class.
```cpp
template <class T>
using array = tensor<T, 1>;

template <class T>
using matrix = tensor<T, 2>;
```

## Member types

| Member type       | Definition                            |
| ----------------- | ------------------------------------- |
| `value_type`      | The template parameter (`T`)          |
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

Constructs an empty tensor with no elements.
```cpp
tensor();
```

### Size constructor

Constructs a tensor with given shape, each element is left uninitialized.
```cpp
tensor(const shape_t<Rank> &shape);

template <class... Args,
          typename = detail::RequiresNArguments<Rank, Args...>,
          typename = detail::RequiresIntegral<Args...> >
tensor(Args... args);
```

Parameters

* `shape` Number of elements along each axis. It can be a `shape_t` object or
the elements of the shape passed as separate arguments.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array(10);
    std::cout << array.shape() << "\n";
    np::matrix<int> matrix(3, 4);
    std::cout << matrix.shape() << "\n";
    np::tensor<int, 3> cube(2, 3, 4);
    std::cout << cube.shape() << "\n";
    return 0;
}
```

Output

```
(10,)
(3, 4)
(2, 3, 4)
```

### Fill constructor

Constructs a tensor with given shape, each element initialized to `val`.
```cpp
tensor(const shape_t<Rank> &shape, const T &val);
```

Parameters

* `shape` Number of elements along each axis.
* `val` Value to which each of the elements is initialized.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array(10, 0);
    std::cout << "1 dimensional:\n" << array << "\n";
    np::matrix<int> matrix({3, 4}, 0);
    std::cout << "2 dimensional:\n" << matrix << "\n";
    np::tensor<int, 3> cube({2, 3, 4}, 0);
    std::cout << "3 dimensional:\n" << cube << "\n";
    return 0;
}
```

Output

```
1 dimensional:
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
2 dimensional:
[[0, 0, 0, 0],
 [0, 0, 0, 0],
 [0, 0, 0, 0]]
3 dimensional:
[[[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]],

 [[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]]]
```

### Range constructor

Constructs a tensor with given shape, with each element constructed from its
corresponding element in the range starting at `first`, in the same order.
```cpp
template <class InputIterator,
          typename = detail::RequiresInputIterator<InputIterator> >
tensor(const shape_t<Rank> &shape, InputIterator first);
```

Parameters

* `shape` Number of elements along each axis.
* `first` Input iterator to the initial position in a range.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int ptr[12] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4, 11, 5};
    np::array<int> array(12, ptr);
    std::cout << "1 dimensional:\n" << array << "\n";
    np::matrix<int> matrix({3, 4}, ptr);
    std::cout << "2 dimensional:\n" << matrix << "\n";
    np::tensor<int, 3> cube({2, 2, 3}, ptr);
    std::cout << "3 dimensional:\n" << cube << "\n";
    return 0;
}
```

Output

```
1 dimensional:
[-4, 16, 14,  9, 18,  3,  7,  2,  1,  4, 11,  5]
2 dimensional:
[[-4, 16, 14,  9],
 [18,  3,  7,  2],
 [ 1,  4, 11,  5]]
3 dimensional:
[[[-4, 16, 14],
  [ 9, 18,  3]],

 [[ 7,  2,  1],
  [ 4, 11,  5]]]
```

### Copy constructor

Constructs a tensor with a copy of each of the elements in other, in the same
order.
```cpp
tensor(const tensor &other);
template <class U>
tensor(const tensor<U, Rank> &other);
```

Parameters

* `other` A tensor-like object of the same rank.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

### Move constructor

Constructs a tensor that acquires the elements of other.
```cpp
tensor(tensor &&other);
```

Parameters

* `other` A tensor of the same type and rank. The ownership is directly
transferred from `other`. `other` is left in an empty state.

### Initializer list constructor

Constructs a tensor with a copy of each of the elements in `il`, in the same
order.
```cpp
tensor(detail::RequiresDepthInitializerList<T, Rank> il);
```

Parameters

* `il` An `initializer_list` object.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array{-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    std::cout << "1 dimensional:\n" << array << "\n";
    np::matrix<int> matrix{{0, 10, -4, 5},
                           {6, 10, 8, 12},
                           {2, 11, 0, -1}};
    std::cout << "2 dimensional:\n" << matrix << "\n";
    np::tensor<int, 3> cube{{{1, 18, 11, 10},
                             {9, 19, 12, 10},
                             {13, 8, -4, 16}},
                            {{2, 4, 14, 19},
                             {18, 5, 19, 18},
                             {0, 0, 15, 17}}};
    std::cout << "3 dimensional:\n" << cube << "\n";
    return 0;
}

```

Output

```
1 dimensional:
[-4, 16, 14,  9, 18,  3,  7,  2,  1,  4]
2 dimensional:
[[ 0, 10, -4,  5],
 [ 6, 10,  8, 12],
 [ 2, 11,  0, -1]]
3 dimensional:
[[[ 1, 18, 11, 10],
  [ 9, 19, 12, 10],
  [13,  8, -4, 16]],

 [[ 2,  4, 14, 19],
  [18,  5, 19, 18],
  [ 0,  0, 15, 17]]]
```

## Destructor

```cpp
~tensor();
```

## Indexing

### `tensor::operator()`

Returns a reference to the element at the given position.
```cpp
template <class... Args,
          detail::RequiresNArguments<Rank, Args...> = true,
          detail::RequiresIntegral<Args...> = true>
T& operator()(Args... args);

template <class... Args,
          detail::RequiresNArguments<Rank, Args...> = true,
          detail::RequiresIntegral<Args...> = true>
const T& operator()(Args... args) const;
```

Parameters

* `args...` Index arguments.

Returns

* The element at the specified position. If the tensor is const-qualified, the
function returns a reference to `const T`. Otherwise, it returns a reference to
`T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array(10);
    int value = 0;
    for (unsigned i = 0; i < array.size(); ++i) {
        array(i) = value++;
    }
    std::cout << "1 dimensional:\n" << array << "\n";

    np::matrix<int> matrix(3, 4);
    value = 0;
    for (unsigned i = 0; i < matrix.shape(0); ++i) {
        for (unsigned j = 0; j < matrix.shape(1); ++j) {
            matrix(i, j) = value++;
        }
    }
    std::cout << "2 dimensional:\n" << matrix << "\n";

    np::tensor<int, 3> cube(2, 3, 4);
    value = 0;
    for (unsigned i = 0; i < cube.shape(0); ++i) {
        for (unsigned j = 0; j < cube.shape(1); ++j) {
            for (unsigned k = 0; k < cube.shape(2); ++k) {
                cube(i, j, k) = value++;
            }
        }
    }
    std::cout << "3 dimensional:\n" << cube << "\n";
    return 0;
}
```

Output

```
1 dimensional:
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
2 dimensional:
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
3 dimensional:
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
```

### `tensor::operator[]`

Returns a reference to the element at the given position.
```cpp
T& operator[](const index_t<Rank> &index);
const T& operator[](const index_t<Rank> &index) const;

T& operator[](size_t i);
const T& operator[](size_t i) const;
```

Parameters

* `index` An `index_t` object with the position of an element in the tensor. If
the tensor is one dimensional, an integer can be used instead.

Returns

* The element at the specified position. If the tensor is const-qualified, the
function returns a reference to `const T`. Otherwise, it returns a reference to
`T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array(10);
    int value = 0;
    for (unsigned i = 0; i < array.size(); ++i) {
        array[i] = value++;
    }
    std::cout << "1 dimensional:\n" << array << "\n";

    np::matrix<int> matrix(3, 4);
    value = 0;
    for (unsigned i = 0; i < matrix.shape(0); ++i) {
        for (unsigned j = 0; j < matrix.shape(1); ++j) {
            matrix[{i, j}] = value++;
        }
    }
    std::cout << "2 dimensional:\n" << matrix << "\n";

    np::tensor<int, 3> cube(2, 3, 4);
    value = 0;
    for (unsigned i = 0; i < cube.shape(0); ++i) {
        for (unsigned j = 0; j < cube.shape(1); ++j) {
            for (unsigned k = 0; k < cube.shape(2); ++k) {
                cube[{i, j, k}] = value++;
            }
        }
    }
    std::cout << "3 dimensional:\n" << cube << "\n";
    return 0;
}
```

Output

```
1 dimensional:
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
2 dimensional:
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
3 dimensional:
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
```

### `tensor::ndim`

Return the dimension of the tensor.
```cpp
constexpr size_t ndim() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array(10);
    std::cout << array.ndim() << "\n";
    np::matrix<int> matrix(3, 4);
    std::cout << matrix.ndim() << "\n";
    np::tensor<int, 3> cube(2, 3, 4);
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

### `tensor::shape`

Return the shape of the tensor.
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
    np::array<int> array(10);
    std::cout << "1 dimensional:\n";
    std::cout << "Shape: " << array.shape() << "\n";
    std::cout << "Length: " << array.shape(0) << "\n\n";

    np::matrix<int> matrix(3, 4);
    std::cout << "2 dimensional:\n";
    std::cout << "Shape: " << matrix.shape() << "\n";
    std::cout << "Rows: " << matrix.shape(0) << "\n";
    std::cout << "Columns: " << matrix.shape(1) << "\n\n";

    np::tensor<int, 3> cube(2, 3, 4);
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
Shape: (10,)
Length: 10

2 dimensional:
Shape: (3, 4)
Rows: 3
Columns: 4

3 dimensional:
Shape: (2, 3, 4)
Depth: 2
Rows: 3
Columns: 4
```

### `tensor::size`

Return the number of elements in the tensor (i.e., the product of the sizes
along all the axes).
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
    np::array<int> array(10);
    std::cout << array.size() << "\n";
    np::matrix<int> matrix(3, 4);
    std::cout << matrix.size() << "\n";
    np::tensor<int, 3> cube(2, 3, 4);
    std::cout << cube.size() << "\n";
    return 0;
}
```

Output

```
10
12
24
```

### `tensor::empty`

Returns whether the tensor is empty (i.e., whether its size is 0).
```cpp
bool empty() const;
```

### `tensor::data`

Returns a pointer to the memory array used internally by the tensor. Because
elements in the tensor are stored contiguously and in row-major order, the
pointer retrieved can be offset to access any element in the tensor.
```cpp
T* data();
const T* data() const;
```

Returns

* A pointer to the memory array used internally by the tensor. If the tensor is
const-qualified, the function returns a pointer to `const T`. Otherwise, it
returns a pointer to `T`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array(10);
    int value = 0;
    for (unsigned i = 0; i < array.size(); ++i) {
        array.data()[i] = value++;
    }
    std::cout << "1 dimensional:\n" << array << "\n";

    np::matrix<int> matrix(3, 4);
    value = 0;
    for (unsigned i = 0; i < matrix.size(); ++i) {
        matrix.data()[i] = value++;
    }
    std::cout << "2 dimensional:\n" << matrix << "\n";

    np::tensor<int, 3> cube(2, 3, 4);
    value = 0;
    for (unsigned i = 0; i < cube.size(); ++i) {
        cube.data()[i] = value++;
    }
    std::cout << "3 dimensional:\n" << cube << "\n";
    return 0;
}
```

Output

```
1 dimensional:
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
2 dimensional:
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
3 dimensional:
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
```

### `tensor::rowmajor`

Returns whether the elements are stored in row-major order. For tensor objects,
always returns `true`.
```cpp
bool rowmajor() const;
```

### `tensor::colmajor`

Returns whether the elements are stored in column-major order. For tensor
objects, always returns `false`.
```cpp
bool colmajor() const;
```

## Advanced indexing

### Slice indexing

Returns a `tensor_view` object that selects the elements given by the slices.
```cpp
template <class... Args,
          detail::RequiresNArguments<Rank, Args...> = true,
          detail::RequiresSlice<Args...> = true>
tensor_view<T, detail::slice_index_rank<Args...>::value>
operator()(Args... args);

template <class... Args,
          detail::RequiresNArguments<Rank, Args...> = true,
          detail::RequiresSlice<Args...> = true>
tensor_view<const T, detail::slice_index_rank<Args...>::value>
operator()(Args... args) const;
```

Parameters

* `args...` Index arguments. Each argument can be either an integer or a slice.
If a slice is given, select multiple positions along the corresponding axis. An
empty slice can be used to select all the positions along the axis.

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to
`const T`, which is convertible to a tensor object. Otherwise, the function
returns a `tensor_view` to `T`, which has reference semantics to the original
tensor. The dimension of the `tensor_view` is the number of slices in the index
arguments.

Exceptions

* `std::out_of_range` Thrown if an integral index is out of bounds.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    // Select first 5 positions.
    std::cout << array(np::slice(5)) << "\n";
    // Select 3 positions starting from 7 (i.e., positions 7, 8, 9), and set
    // all of them to zero.
    array(np::slice(7, 3)) = 0;
    std::cout << array << "\n";
    return 0;
}
```

Output

```
[ 7, 13, 19, 11,  5]
[ 7, 13, 19, 11,  5,  8, -2,  0,  0,  0]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> matrix{{15, -4, 3, 18, -2, 7},
                           {8, 11, 19, 0, -5, 14},
                           {16, 19, 9, 12, 12, 18},
                           {-5, 11, 5, 10, 8, 10}};
    // Select all rows.
    // Select 2 columns starting from 4 (i.e., columns 4 and 5).
    std::cout << matrix(np::slice(), np::slice(4, 2)) << "\n";
    // Select row 1 (i.e., second row).
    // Select 3 columns starting from 0 with stride 2 (i.e., columns 0, 2, 4).
    std::cout << matrix(1, np::slice(0, 3, 2)) << "\n";
    // Set third column to zero.
    matrix(np::slice(), 2) = 0;
    std::cout << matrix << "\n";
    return 0;
}
```

Output

```
[[-2,  7],
 [-5, 14],
 [12, 18],
 [ 8, 10]]
[ 8, 19, -5]
[[15, -4,  0, 18, -2,  7],
 [ 8, 11,  0,  0, -5, 14],
 [16, 19,  0, 12, 12, 18],
 [-5, 11,  0, 10,  8, 10]]
 ```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube{{{-5, 19, 5, 18},
                             {13, 1, 9, 14},
                             {15, 12, 14, 16}},
                            {{2, 14, -2, 3},
                             {18, 11, 9, 18},
                             {6, 19, -2, 1}}};
    // Select first matrix block.
    std::cout << cube(0, np::slice(), np::slice()) << "\n";
    // Set second and fourth columns of each matrix block to zero.
    cube(np::slice(), np::slice(), np::slice(1, 2, 2)) = 0;
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[-5, 19,  5, 18],
 [13,  1,  9, 14],
 [15, 12, 14, 16]]
[[[-5,  0,  5,  0],
  [13,  0,  9,  0],
  [15,  0, 14,  0]],

 [[ 2,  0, -2,  0],
  [18,  0,  9,  0],
  [ 6,  0, -2,  0]]]
```

### Coordinate tensor indexing

Returns an `indirect_tensor` that selects the elements specified by the tensor
of indices.
```cpp
template <size_t N,>
indirect_tensor<T, N> operator[](const tensor<index_t<Rank>, N> &index);
template <size_t N,>
tensor<T, N> operator[](const tensor<index_t<Rank>, N> &index) const;

template <class IntegralType, size_t N>
indirect_tensor<T, N> operator[](const tensor<IntegralType, N> &index);
template <class IntegralType, size_t N>
tensor<T, N> operator[](const tensor<IntegralType, N> &index) const;
```

Parameters

* `index` A tensor-like object of `index_t` with its elements identifying which
elements of the tensor are selected. If the tensor is one dimensional, a
tensor-like object of integers can be used instead.

Returns

* If the tensor is const-qualified, the function returns a new tensor object
with a copy of the selection. Otherwise, the function returns an
`indirect_tensor`, which has reference semantics to the original tensor.

Exceptions

* `std::out_of_range` Thrown if the indices references to an element out of
bounds.
* `std::bad_alloc` If the function needs to allocate storage and fails, it may
throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    np::array<int> indices{1, 2, 3, 5, 7};
    std::cout << array[indices] << "\n";
    array[indices] = 0;
    std::cout << array << "\n";
    return 0;
}
```

Output

```
[13, 19, 11,  8,  7]
[ 7,  0,  0,  0,  5,  0, -2,  0, 11,  3]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> matrix{{15, -4, 3, 18, -2, 7},
                           {8, 11, 19, 0, -5, 14},
                           {16, 19, 9, 12, 12, 18},
                           {-5, 11, 5, 10, 8, 10}};
    np::array<np::index_t<2> > indices{{0,0}, {0,1}, {1,1}, {1,2}, {2,2}, {2,3},
                                       {3,3}};
    std::cout << matrix[indices] << "\n";
    matrix[indices] = 0;
    std::cout << matrix;
    return 0;
}
```

Output

```
[15, -4, 11, 19,  9, 12, 10]
[[ 0,  0,  3, 18, -2,  7],
 [ 8,  0,  0,  0, -5, 14],
 [16, 19,  0,  0, 12, 18],
 [-5, 11,  5,  0,  8, 10]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube{{{-5, 19, 5, 18},
                             {13, 1, 9, 14},
                             {15, 12, 14, 16}},
                            {{2, 14, -2, 3},
                             {18, 11, 9, 18},
                             {6, 19, -2, 1}}};
    np::matrix<np::index_t<3> > indices{{{0,0,0}, {0,2,0}, {0,1,2}},
                                        {{1,1,1}, {1,0,2}, {1,2,2}}};
    std::cout << cube[indices] << "\n";
    cube[indices] = 0;
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[-5, 15,  9],
 [11, -2, -2]]
[[[ 0, 19,  5, 18],
  [13,  1,  0, 14],
  [ 0, 12, 14, 16]],

 [[ 2, 14,  0,  3],
  [18,  0,  9, 18],
  [ 6, 19,  0,  1]]]
```

### Boolean tensor indexing

Returns an `indirect_tensor` that selects the elements specified by the boolean
mask.
```cpp
indirect_tensor<T, 1> operator[](const tensor<bool, Rank> &mask);
tensor<T, 1> operator[](const tensor<bool, Rank> &mask) const;
```

Parameters

* `mask` A tensor-like object of `bool` with its elements identifying whether
each element of the tensor is selected or not.

Returns

* If the tensor is const-qualified, the function returns a new tensor object
with a copy of the selection. Otherwise, the function returns an
`indirect_tensor`, which has reference semantics to the original tensor.

Exceptions

* `std::out_of_range` Thrown if the tensor and the boolean mask have different
shapes.
* `std::bad_alloc` If the function needs to allocate storage and fails, it may
throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    np::array<bool> mask = (array > 10);
    std::cout << std::boolalpha << mask << "\n";
    std::cout << array[mask] << "\n";
    array[mask] = 0;
    std::cout << array << "\n";
    return 0;
}
```

Output

```
[false,  true,  true,  true, false, false, false, false,  true, false]
[13, 19, 11, 11]
[ 7,  0,  0,  0,  5,  8, -2,  7,  0,  3]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> matrix{{15, -4, 3, 18, -2, 7},
                           {8, 11, 19, 0, -5, 14},
                           {16, 19, 9, 12, 12, 18},
                           {-5, 11, 5, 10, 8, 10}};
    np::matrix<bool> mask = (matrix <= 0);
    std::cout << std::boolalpha << mask << "\n";
    std::cout << matrix[mask] << "\n";
    matrix[mask] = 0;
    std::cout << matrix;
    return 0;
}
```

Output

```
[[false,  true, false, false,  true, false],
 [false, false, false,  true,  true, false],
 [false, false, false, false, false, false],
 [ true, false, false, false, false, false]]
[-4, -2,  0, -5, -5]
[[15,  0,  3, 18,  0,  7],
 [ 8, 11, 19,  0,  0, 14],
 [16, 19,  9, 12, 12, 18],
 [ 0, 11,  5, 10,  8, 10]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube{{{-5, 19, 5, 18},
                             {13, 1, 9, 14},
                             {15, 12, 14, 16}},
                            {{2, 14, -2, 3},
                             {18, 11, 9, 18},
                             {6, 19, -2, 1}}};
    np::tensor<bool, 3> mask = (cube > 10);
    std::cout << std::boolalpha << mask << "\n";
    std::cout << cube[mask] << "\n";
    cube[mask] = 0;
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[false,  true, false,  true],
  [ true, false, false,  true],
  [ true,  true,  true,  true]],

 [[false,  true, false, false],
  [ true,  true, false,  true],
  [false,  true, false, false]]]
[19, 18, 13, 14, 15, 12, 14, 16, 14, 18, 11, 18, 19]
[[[-5,  0,  5,  0],
  [ 0,  1,  9,  0],
  [ 0,  0,  0,  0]],

 [[ 2,  0, -2,  3],
  [ 0,  0,  9,  0],
  [ 6,  0, -2,  1]]]
```

## Assignment

### Copy assignment

Assigns to each element the value of the corresponding element in `other`,
after resizing the object (if necessary).
```cpp
tensor& operator=(const tensor &other);
template <class U>
tensor& operator=(const base_tensor<U, Rank> &other);
```

Parameters

* `other` A tensor-like object of the same rank.

Returns

* `*this`

Exceptions

* `std::bad_alloc` If the function needs to allocate storage and fails, it may
throw an exception.

Warnings

* When the shapes do not mach, invalidates all iterators, references and views
to elements of the tensor. Otherwise, valid iterators, references and views
keep their validity.

### Fill assignment

Assigns `val` to every element. The size of the tensor is preserved.
```cpp
tensor& operator=(const T &val);
```

Parameters

* `val` Value assigned to all the elements in the tensor.

Returns

* `*this`

### Move assignment

Acquires the contents of `other`, leaving `other` in an empty state.
```cpp
tensor& operator=(tensor &&other);
```

Parameters

* `other` A tensor of the same type and rank. The ownership is directly
transferred from `other`. `other` is left in an empty state.

Returns

* `*this`

### Initializer list assignment

Assigns to each element the value of the corresponding element in `il` after
resizing the object (if necessary).
```cpp
tensor& operator=(detail::RequiresDepthInitializerList<T, Rank> il);
```

Parameters

* `il` An `initializer_list` object.

Returns

* `*this`

Exceptions

* `std::bad_alloc` If the function needs to allocate storage and fails, it may
throw an exception.

Warnings

* When the shapes do not mach, invalidates all iterators, references and views
to elements of the tensor. Otherwise, valid iterators, references and views
keep their validity.