# Indirect tensor

Defined in `numcpp/tensor.h`

- [Indirect tensor](#indirect-tensor)
  - [Template parameters](#template-parameters)
  - [Aliases](#aliases)
  - [Member types](#member-types)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Index array constructor](#index-array-constructor)
    - [Copy constructor](#copy-constructor)
    - [Move constructor](#move-constructor)
  - [Destructor](#destructor)
  - [Indexing](#indexing)
    - [`indirect_tensor::operator()`](#indirect_tensoroperator)
    - [`indirect_tensor::operator[]`](#indirect_tensoroperator-1)
    - [`indirect_tensor::ndim`](#indirect_tensorndim)
    - [`indirect_tensor::shape`](#indirect_tensorshape)
    - [`indirect_tensor::size`](#indirect_tensorsize)
    - [`indirect_tensor::empty`](#indirect_tensorempty)
    - [`indirect_tensor::data`](#indirect_tensordata)
    - [`indirect_tensor::indices`](#indirect_tensorindices)
    - [`indirect_tensor::layout`](#indirect_tensorlayout)
    - [`indirect_tensor::is_owner`](#indirect_tensoris_owner)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
    - [Fill assignment](#fill-assignment)
    - [Move assignment](#move-assignment)

```cpp
template <class T, size_t Rank> class indirect_tensor;
```
An `indirect_tensor` is a view of a subset of elements from a multidimensional
array. It references the elements in the original array through an array of
indices. The view itself does not own the data and any changes made to the view
will affect the original array, and any changes made to the original array will
affect the view.

## Template parameters

* `T` Type of the elements contained in the `indirect_tensor`. This shall be an
arithmetic type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the `indirect_tensor`. It must be a positive integer.

## Aliases

The main header `numcpp.h` offers the following aliases for `indirect_tensor`
class.
```cpp
template <class T>
using indirect_array = indirect_tensor<T, 1>;

template <class T>
using indirect_matrix = indirect_tensor<T, 2>;
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

Constructs an `indirect_tensor` that does not reference any object.
```cpp
indirect_tensor();
```

### Index array constructor

Constructs an `indirect_tensor` that references a subset of elements from a
multidimensional array.
```cpp
indirect_tensor(
    const shape_t<Rank> &shape, T *data, const size_t *indices,
    layout_t order = row_major, int mode = 1
);
```

Parameters

* `shape` Number of elements along each axis.
* `data` Pointer to the memory array used by the `indirect_tensor`.
* `indices` Pointer to the array of indices with its elements identifying which
elements of `data` are selected.
* `order` Order in which elements shall be iterated. In row-major order, the
last index is varying the fastest. In column-major order, the first index is
varying the fastest. Defaults to row-major order.
* `mode` If positive, creates a copy of `indices`. If zero, stores the pointer
directly without making a copy of it. If negative, acquires the ownership of
`indices`, which will be deleted along with the `indirect_tensor`. Defaults to
make a copy.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data1d[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    size_t index1d[5] = {1, 2, 3, 5, 7};
    np::indirect_array<int> arr(5, data1d, index1d, np::row_major, 0);
    std::cout << "1 dimensional:\n" << arr << "\n";

    int data2d[12] = {0, 10, -4, 5,
                      6, 10, 8, 12,
                      2, 11, 0, -1};
    size_t index2d[6] = {0, 1,
                         5, 6,
                         10, 11};
    np::indirect_matrix<int> mat({3, 2}, data2d, index2d, np::row_major, 0);
    std::cout << "2 dimensional:\n" << mat << "\n";
    return 0;
}
```

Output

```
1 dimensional:
[13, 19, 11,  8,  7]
2 dimensional:
[[ 0, 10],
 [10,  8],
 [ 0, -1]]
```

### Copy constructor

Constructs an `indirect_tensor` as a copy of other. The ownership is also
copied, i.e., if `other` is owner of an array of indices, then `*this` creates
a copy of it. Otherwise, `*this` and `other` shares the same array of indices.
```cpp
indirect_tensor(const indirect_tensor &other);
```

Parameters

* `other` An `indirect_tensor` of the same type and rank.

### Move constructor

Constructs an `indirect_tensor` that acquires the elements of other.
```cpp
indirect_tensor(indirect_tensor &&other);
```

Parameters

* `other` An `indirect_tensor` of the same type and rank. The ownership is
directly transferred from `other`. `other` is left in an empty state.

## Destructor

```cpp
~indirect_tensor();
```

## Indexing

### `indirect_tensor::operator()`

Returns a reference to the element at the given position. The elements in an
`indirect_tensor` are given by

```
    data[indices[ravel_index(index, shape, order)]]
```

where `data` is the memory array and `indices` is the array of indices.
```cpp
template <class... Args>
T& operator()(Args... args);

template <class... Args>
const T& operator()(Args... args) const;
```

Parameters

* `args...` Index arguments.

Returns

* The element at the specified position. If the `indirect_tensor` is
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
    size_t index[5] = {1, 2, 3, 5, 7};
    np::indirect_array<int> arr(5, data, index, np::row_major, 0);
    for (unsigned i = 0; i < arr.size(); ++i) {
        // Prints data[index[i]]
        std::cout << arr(i) << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
13, 19, 11, 8, 7,
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[12] = {0, 10, -4, 5,
                    6, 10, 8, 12,
                    2, 11, 0, -1};
    size_t index[6] = {0, 1,
                       5, 6,
                       10, 11};
    np::indirect_matrix<int> mat({3, 2}, data, index, np::row_major, 0);
    for (unsigned i = 0; i < mat.shape(0); ++i) {
        for (unsigned j = 0; j < mat.shape(1); ++j) {
            // Prints data[index[np::ravel_index({i, j}, mat.shape())]]
            std::cout << mat(i, j) << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
0, 10,
10, 8,
0, -1,
```

### `indirect_tensor::operator[]`

Returns a reference to the element at the given position.
```cpp
T& operator[](const index_t<Rank> &index);
const T& operator[](const index_t<Rank> &index) const;

T& operator[](size_t i);
const T& operator[](size_t i) const;
```

Parameters

* `index` An `index_t` object with the position of an element in the
`indirect_tensor`. If the tensor is one dimensional, an integer can be used
instead.

Returns

* The element at the specified position. If the `indirect_tensor` is
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
    size_t index[5] = {1, 2, 3, 5, 7};
    np::indirect_array<int> arr(5, data, index, np::row_major, 0);
    for (unsigned i = 0; i < arr.size(); ++i) {
        // Prints data[index[i]]
        std::cout << arr[i] << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
13, 19, 11, 8, 7,
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[12] = {0, 10, -4, 5,
                    6, 10, 8, 12,
                    2, 11, 0, -1};
    size_t index[6] = {0, 1,
                       5, 6,
                       10, 11};
    np::indirect_matrix<int> mat({3, 2}, data, index, np::row_major, 0);
    for (unsigned i = 0; i < mat.shape(0); ++i) {
        for (unsigned j = 0; j < mat.shape(1); ++j) {
            // Prints data[index[np::ravel_index({i, j}, mat.shape())]]
            std::cout << mat[{i, j}] << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
0, 10,
10, 8,
0, -1,
```

### `indirect_tensor::ndim`

Return the dimension of the `indirect_tensor`.
```cpp
static constexpr size_t ndim();
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    std::cout << np::indirect_array<int>::ndim() << "\n";
    std::cout << np::indirect_matrix<int>::ndim() << "\n";
    std::cout << np::indirect_tensor<int, 3>::ndim() << "\n";
    return 0;
}
```

Output

```
1
2
```

### `indirect_tensor::shape`

Return the shape of the `indirect_tensor`.
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
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    size_t index[6] = {1, 2, 3, 5, 7, 8};
    np::indirect_array<int> arr(6, data, index, np::row_major, 0);
    std::cout << "1 dimensional:\n";
    std::cout << "Shape: " << arr.shape() << "\n";
    std::cout << "Length: " << arr.shape(0) << "\n\n";

    np::indirect_matrix<int> mat({3, 2}, data, index, np::row_major, 0);
    std::cout << "2 dimensional:\n";
    std::cout << "Shape: " << mat.shape() << "\n";
    std::cout << "Rows: " << mat.shape(0) << "\n";
    std::cout << "Columns: " << mat.shape(1) << "\n";
    return 0;
}
```

Output

```
1 dimensional:
Shape: (6,)
Length: 6

2 dimensional:
Shape: (3, 2)
Rows: 3
Columns: 2
```

### `indirect_tensor::size`

Returns the number of elements in the `indirect_tensor` (i.e., the product of
the sizes along all the axes).
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
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    size_t index[6] = {1, 2, 3, 5, 7, 8};
    np::indirect_array<int> arr(6, data, index, np::row_major, 0);
    std::cout << arr.size() << "\n";
    np::indirect_matrix<int> mat({3, 2}, data, index, np::row_major, 0);
    std::cout << mat.size() << "\n";
    return 0;
}
```

Output

```
6
6
```

### `indirect_tensor::empty`

Returns whether the `indirect_tensor` is empty (i.e., whether its size is 0).
```cpp
bool empty() const;
```

### `indirect_tensor::data`

Returns a pointer to the memory array used internally by the `indirect_tensor`.
```cpp
T* data();
const T* data() const;
```

Returns

* A pointer to the memory array used internally by the `indirect_tensor`. If
the `indirect_tensor` is const-qualified, the function returns a pointer to
`const T`. Otherwise, it returns a pointer to `T`.

### `indirect_tensor::indices`

Returns a pointer to the array of indices used internally by the
`indirect_tensor`.
```cpp
const size_t* indices() const;
```

Returns

* A const pointer to the array of indices used internally by the
`indirect_tensor`.

### `indirect_tensor::layout`

Returns the order in which elements are iterated. It is not necessarily the
memory layout in which elements are stored as the elements might not be
continuous in memory.
```cpp
layout_t layout() const;
```

### `indirect_tensor::is_owner`

Returns whether the `indirect_tensor` is owner of an array of indices.
```cpp
bool is_owner() const;
```

## Assignment

### Copy assignment

Assigns to each element the value of the corresponding element in `other`.
```cpp
template <class U>
indirect_tensor& operator=(const tensor<U, Rank> &other);
```

Parameters

* `other` A tensor-like object of the same shape.

Returns

* `*this`

### Fill assignment

Assigns `val` to every element.
```cpp
indirect_tensor& operator=(const T &val);
```

Parameters

* `val` Value assigned to all the elements in the `indirect_tensor`.

Returns

* `*this`

### Move assignment

Acquires the contents of `other`, leaving `other` in an empty state.
```cpp
indirect_tensor& operator=(indirect_tensor &&other);
```

Parameters

* `other` An `indirect_tensor` of the same type and rank. The ownership is
directly transferred from `other`. `other` is left in an empty state.

Returns

* `*this`
