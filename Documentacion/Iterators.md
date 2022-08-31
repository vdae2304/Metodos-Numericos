# Iterators

Defined in `numcpp/tensor.h`

- [Iterators](#iterators)
  - [Template parameters](#template-parameters)
  - [Member types](#member-types)
  - [Index sequence](#index-sequence)
    - [`make_indices`](#make_indices)
  - [Accessing to iterators](#accessing-to-iterators)
    - [`tensor::begin`](#tensorbegin)
    - [`tensor::end`](#tensorend)
    - [`tensor::cbegin`](#tensorcbegin)
    - [`tensor::cend`](#tensorcend)
  - [Public methods](#public-methods)
    - [`tensor::iterator::base`](#tensoriteratorbase)
    - [`tensor::iterator::index`](#tensoriteratorindex)
    - [`tensor::iterator::coords`](#tensoriteratorcoords)
    - [`tensor::iterator::rowmajor`](#tensoriteratorrowmajor)
    - [`tensor::iterator::colmajor`](#tensoriteratorcolmajor)

## Template parameters

* `T` Type of the elements contained in the tensor. This shall be an arithmetic
type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the tensor. It must be a positive integer.

## Member types

Member type definitions for `tensor<T, Rank>::iterator`. Equivalent definitions
exists for `tensor_view<T, Rank>::iterator` and
`indirect_tensor<T, Rank>::iterator`.

| Member type         | Definition                             |
| ------------------- | -------------------------------------- |
| `difference_type`   | A signed integral type.                |
| `value_type`        | `typename tensor<T, Rank>::value_type` |
| `pointer`           | `typename tensor<T, Rank>::pointer`    |
| `reference`         | `typename tensor<T, Rank>::reference`  |
| `iterator_category` | `std::random_access_iterator_tag`      |

Member type definitions for `tensor<T, Rank>::const_iterator`. Equivalent
definitions exists for `tensor_view<T, Rank>::const_iterator` and
`indirect_tensor<T, Rank>::const_iterator`.

| Member type         | Definition                                  |
| ------------------- | ------------------------------------------- |
| `difference_type`   | A signed integral type.                     |
| `value_type`        | `typename tensor<T, Rank>::value_type`      |
| `pointer`           | `typename tensor<T, Rank>::const_pointer`   |
| `reference`         | `typename tensor<T, Rank>::const_reference` |
| `iterator_category` | `std::random_access_iterator_tag`           |

## Index sequence

### `make_indices`

An n-dimensional iterator to index tensors.
```cpp
template <size_t Rank>
index_sequence<Rank> make_indices(const shape_t<Rank> &shape);

template <class... Args>
index_sequence<sizeof...(Args)> make_indices(Args... args);
```

Parameters

* `shape` The size of each dimension. It can be a `shape_t` object or the
elements of the shape passed as separate arguments.

Returns

* An `index_sequence` object which iterates over the indices of a tensor. At
each iteration, a new index is returned. The elements are iterated in row-major
order, i.e., from first axis to last axis.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Equivalent (but shorter) to
    // for (size_t i = 0; i < 3; ++i) {
    //     for (size_t j = 0; j < 4; ++j) {
    //         std::cout << np::make_index(i, j) << "\n";
    //     }
    // }
    np::index_sequence<2> indices = np::make_indices(3, 4);
    np::index_sequence<2>::iterator it;
    for (it = indices.begin(); it != indices.end(); ++it) {
        std::cout << *it << "\n";
    }
    return 0;
}
```

Output

```
(0, 0)
(0, 1)
(0, 2)
(0, 3)
(1, 0)
(1, 1)
(1, 2)
(1, 3)
(2, 0)
(2, 1)
(2, 2)
(2, 3)
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    // Using C++11 range-based for loop
    for (np::index_t<2> i : np::make_indices(3, 4)) {
        std::cout << i << "\n";
    }
    return 0;
}
```

## Accessing to iterators

### `tensor::begin`

<h3><code>tensor_view::begin</code></h3>

<h3><code>indirect_tensor::begin</code></h3>

Returns an iterator pointing to the first element in the tensor.
```cpp
iterator begin();
const_iterator begin() const;

iterator begin(bool row_major);
const_iterator begin(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A random access iterator to the beginning of the tensor. If the tensor is
const-qualified, the function returns a `const_iterator`. Otherwise, it returns
an `iterator`.

### `tensor::end`

<h3><code>tensor_view::end</code></h3>

<h3><code>indirect_tensor::end</code></h3>

Returns an iterator pointing to the past-the-end element in the tensor. It does
not point to any element, and thus shall not be dereferenced.
```cpp
iterator end();
const_iterator end() const;

iterator end(bool row_major);
const_iterator end(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A random access iterator to the element past the end of the tensor. If the
tensor is const-qualified, the function returns a `const_iterator`. Otherwise,
it returns an `iterator`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr(10);
    int value = 0;
    np::array<int>::iterator it;
    for (it = arr.begin(); it != arr.end(); ++it) {
        *it = value++;
    }
    std::cout << arr << "\n";
    return 0;
}
```

Output

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat(3, 4);
    int value = 0;
    np::matrix<int>::iterator it;
    for (it = mat.begin(); it != mat.end(); ++it) {
        *it = value++;
    }
    std::cout << "Row-major:\n" << mat << "\n";
    value = 0;
    for (it = mat.begin(false); it != mat.end(false); ++it) {
        *it = value++;
    }
    std::cout << "Column-major:\n" << mat << "\n";
    return 0;
}
```

Output

```
Row-major:
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
Column-major:
[[ 0,  3,  6,  9],
 [ 1,  4,  7, 10],
 [ 2,  5,  8, 11]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 4);
    int value = 0;
    np::tensor<int, 3>::iterator it;
    for (it = cube.begin(); it != cube.end(); ++it) {
        *it = value++;
    }
    std::cout << "Row-major:\n" << cube << "\n";
    value = 0;
    for (it = cube.begin(false); it != cube.end(false); ++it) {
        *it = value++;
    }
    std::cout << "Column-major:\n" << cube << "\n";
    return 0;
}
```

Output

```
Row-major:
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
Column-major:
[[[ 0,  6, 12, 18],
  [ 2,  8, 14, 20],
  [ 4, 10, 16, 22]],

 [[ 1,  7, 13, 19],
  [ 3,  9, 15, 21],
  [ 5, 11, 17, 23]]]
```

### `tensor::cbegin`

<h3><code>tensor_view::cbegin</code></h3>

<h3><code>indirect_tensor::cbegin</code></h3>

Returns a `const_iterator` pointing to the first element in the tensor.
```cpp
const_iterator cbegin() const;
const_iterator cbegin(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A `const_iterator` to the beginning of the tensor.

### `tensor::cend`

<h3><code>tensor_view::cend</code></h3>

<h3><code>indirect_tensor::cend</code></h3>

Returns a `const_iterator` pointing to the past-the-end element in the tensor.
```cpp
const_iterator cend() const;
const_iterator cend(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A `const_iterator` to the element past the end of the tensor.

## Public methods

### `tensor::iterator::base`

<h3><code>tensor_view::iterator::base</code></h3>

<h3><code>indirect_tensor::iterator::base</code></h3>

Accesses the underlying tensor.

```cpp
// tensor<T, Rank>::iterator
tensor<T, Rank>* base() const;
// tensor<T, Rank>::const_iterator
const tensor<T, Rank>* base() const;

// tensor_view<T, Rank>::iterator
tensor_view<T, Rank>* base() const;
// tensor_view<T, Rank>::const_iterator
const tensor_view<T, Rank>* base() const;

// indirect_tensor<T, Rank>::iterator
indirect_tensor<T, Rank>* base() const;
// indirect_tensor<T, Rank>::const_iterator
const indirect_tensor<T, Rank>* base() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr{18, 13, 17, 17, 13, 14, -1, 2, 11, -3};
    np::array<int>::iterator it = arr.begin();
    // Prints arr.
    std::cout << *it.base() << "\n";
    return 0;
}
```

Output

```
[18, 13, 17, 17, 13, 14, -1,  2, 11, -3]
```

### `tensor::iterator::index`

<h3><code>tensor_view::iterator::index</code></h3>

<h3><code>indirect_tensor::iterator::index</code></h3>

Returns the underlying flat index.
```cpp
// For both iterator and const_iterator
size_t index() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 4);
    np::tensor<int, 3>::iterator it;
    // Prints 0, 1, 2, ...
    for (it = cube.begin(); it != cube.end(); ++it) {
        std::cout << it.index() << ", ";
        if (it.index() % 10 == 9) {
            std::cout << "\n";
        }
    }
    return 0;
}
```

Output

```
0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
20, 21, 22, 23,
```

### `tensor::iterator::coords`

<h3><code>tensor_view::iterator::coords</code></h3>

<h3><code>indirect_tensor::iterator::coords</code></h3>

Returns an `index_t` object with the current coordinates.
```cpp
// For both iterator and const_iterator
index_t<Rank> coords() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 4);
    np::tensor<int, 3>::iterator it;
    std::cout << "Row-major order:\n";
    for (it = cube.begin(); it != cube.end(); ++it) {
        std::cout << it.coords() << ", ";
        if (it.index() % 5 == 4) {
            std::cout << "\n";
        }
    }
    std::cout << "\nColumn-major order:\n";
    for (it = cube.begin(false); it != cube.end(false); ++it) {
        std::cout << it.coords() << ", ";
        if (it.index() % 5 == 4) {
            std::cout << "\n";
        }
    }
    return 0;
}
```

Output

```
Row-major order:
(0, 0, 0), (0, 0, 1), (0, 0, 2), (0, 0, 3), (0, 1, 0),
(0, 1, 1), (0, 1, 2), (0, 1, 3), (0, 2, 0), (0, 2, 1),
(0, 2, 2), (0, 2, 3), (1, 0, 0), (1, 0, 1), (1, 0, 2),
(1, 0, 3), (1, 1, 0), (1, 1, 1), (1, 1, 2), (1, 1, 3),
(1, 2, 0), (1, 2, 1), (1, 2, 2), (1, 2, 3),
Column-major order:
(0, 0, 0), (1, 0, 0), (0, 1, 0), (1, 1, 0), (0, 2, 0),
(1, 2, 0), (0, 0, 1), (1, 0, 1), (0, 1, 1), (1, 1, 1),
(0, 2, 1), (1, 2, 1), (0, 0, 2), (1, 0, 2), (0, 1, 2),
(1, 1, 2), (0, 2, 2), (1, 2, 2), (0, 0, 3), (1, 0, 3),
(0, 1, 3), (1, 1, 3), (0, 2, 3), (1, 2, 3),
```

### `tensor::iterator::rowmajor`

<h3><code>tensor_view::iterator::rowmajor</code></h3>

<h3><code>indirect_tensor::iterator::rowmajor</code></h3>

Returns whether the elements are iterated in row-major order.
```cpp
// For both iterator and const_iterator
bool rowmajor() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat(3, 4);
    np::matrix<int>::iterator it1 = mat.begin();
    np::matrix<int>::iterator it2 = mat.begin(false);
    if (it1.rowmajor()) {
        std::cout << "it1 iterates in row-major order\n";
    }
    else {
        std::cout << "it1 iterates in column-major order\n";
    }
    if (it2.rowmajor()) {
        std::cout << "it2 iterates in row-major order\n";
    }
    else {
        std::cout << "it2 iterates in column-major order\n";
    }
    return 0;
}
```

Output

```
it1 iterates in row-major order
it2 iterates in column-major order
```

### `tensor::iterator::colmajor`

<h3><code>tensor_view::iterator::colmajor</code></h3>

<h3><code>indirect_tensor::iterator::colmajor</code></h3>

Returns whether the elements are iterated in column-major order.
```cpp
// For both iterator and const_iterator
bool colmajor() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat(3, 4);
    np::matrix<int>::iterator it1 = mat.begin();
    np::matrix<int>::iterator it2 = mat.begin(false);
    if (it1.colmajor()) {
        std::cout << "it1 iterates in column-major order\n";
    }
    else {
        std::cout << "it1 iterates in row-major order\n";
    }
    if (it2.colmajor()) {
        std::cout << "it2 iterates in column-major order\n";
    }
    else {
        std::cout << "it2 iterates in row-major order\n";
    }
    return 0;
}
```

Output

```
it1 iterates in row-major order
it2 iterates in column-major order
```