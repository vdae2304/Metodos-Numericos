# Shapes

Defined in `numcpp/shape.h`

- [Shapes](#shapes)
  - [Template parameters](#template-parameters)
  - [Member types](#member-types)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [From a list of sizes](#from-a-list-of-sizes)
    - [Copy constructor](#copy-constructor)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
  - [Public methods](#public-methods)
    - [`shape_t::ndim`](#shape_tndim)
    - [`shape_t::prod`](#shape_tprod)
    - [`shape_t::data`](#shape_tdata)
  - [Operators](#operators)
    - [`shape_t::operator[]`](#shape_toperator)
    - [`shape_t::operator IntegralType`](#shape_toperator-integraltype)
    - [`operator==`](#operator)
    - [`operator!=`](#operator-1)
  - [Routines](#routines)
    - [`make_shape`](#make_shape)
    - [`make_index`](#make_index)
    - [`make_strides`](#make_strides)
    - [`ravel_index`](#ravel_index)
    - [`unravel_index`](#unravel_index)
    - [`broadcast_shapes`](#broadcast_shapes)
    - [`shape_cat`](#shape_cat)

A `shape_t` is a class that identifies the size of a tensor along each
dimension.
```cpp
template <size_t Rank> class shape_t;
```

## Template parameters

* `Rank` Dimension of the shape. It must be a positive integer.

## Member types

| Member type | Definition |
| ----------- | ---------- |
| `size_type` | `size_t`   |

## Constructors

### Default constructor

Set all sizes to zero.
```cpp
shape_t();
```

### From a list of sizes

Construct a new `shape_t` object from a list of sizes.

```cpp
template <class... Sizes>
shape_t(Sizes... sizes);
```

Parameters

* `sizes...` Size along each axis.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<1> shape1(10);
    std::cout << shape1 << "\n";
    np::shape_t<2> shape2(4, 6);
    std::cout << shape2 << "\n";
    np::shape_t<3> shape3(3, 4, 6);
    std::cout << shape3 << "\n";
    return 0;
}
```

Output

```
(10,)
(4, 6)
(3, 4, 6)
```

### Copy constructor

```cpp
shape_t(const shape_t &other);
```

Parameters

* `other` A `shape_t` object with the same dimension to copy.

## Assignment

### Copy assignment

```cpp
shape_t& operator=(const shape_t &other);
```

Parameters

* `other` A `shape_t` object with the same dimension to copy.

Returns

* `*this`

## Public methods

### `shape_t::ndim`

Return the dimension of the shape.
```cpp
static constexpr size_t ndim();
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    std::cout << np::shape_t<1>::ndim() << "\n";
    std::cout << np::shape_t<2>::ndim() << "\n";
    std::cout << np::shape_t<3>::ndim() << "\n";
    return 0;
}
```

Output

```
1
2
3
```

### `shape_t::prod`

Return the product of the sizes along all the axes.
```cpp
size_t prod() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<1> shape1(10);
    std::cout << shape1.prod() << "\n";
    np::shape_t<2> shape2(4, 6);
    std::cout << shape2.prod() << "\n";
    np::shape_t<3> shape3(3, 4, 6);
    std::cout << shape3.prod() << "\n";
    return 0;
}
```

Output

```
10
24
72
```

### `shape_t::data`

Return a pointer to the block of memory containing the elements of the shape.
```cpp
size_t* data();
const size_t* data() const;
```

Example

```cpp
#include <iostream>
#include <algorithm> // std::reverse
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<3> shape(3, 4, 6);
    std::reverse(shape.data(), shape.data() + 3);
    std::cout << shape << "\n";
    return 0;
}
```

Output

```
(6, 4, 3)
```

## Operators

### `shape_t::operator[]`

Return the size of the i-th axis.
```cpp
size_t& operator[](size_t i);
const size_t& operator[](size_t i) const;
```

Parameters

* `i` Axis index.

Returns

* The size along the i-th axis. If the `shape_t` is const-qualified, the
function returns a reference to `const size_t`. Otherwise, it returns a
reference to `size_t`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<2> shape(4, 6);
    std::cout << "Rows: " << shape[0] << "\n";
    std::cout << "Columns: " << shape[1] << "\n";
    return 0;
}
```

Output

```
Rows: 4
Columns: 6
```

### `shape_t::operator IntegralType`

Integer conversion. Dimension must be one.
```cpp
template <class IntegralType>
explicit operator IntegralType() const;
```

### `operator==`

Compares two shapes. Returns `true` if they have the same dimension and the
same size along each axis.
```cpp
template <size_t Rank1, size_t Rank2>
bool operator==(const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2);
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<3> shape1, shape2;
    std::cin >> shape1 >> shape2;
    if (shape1 == shape2) {
        std::cout << "They are equal\n";
    }
    else {
        std::cout << "They are not equal\n";
    }
    return 0;
}
```

Input

```
(3, 4, 6)
(3, 4, 6)
```

Output

```
They are equal
```

Input

```
(1, 4, 1)
(3, 4, 6)
```

Output

```
They are not equal
```

### `operator!=`

Compares two shapes. Returns `true` if they have different dimension or if they
have different size in one axis.
```cpp
template <size_t Rank1, size_t Rank2>
bool operator!=(const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2);
```

## Routines

### `make_shape`

Create a `shape_t` object deducing its dimension from the number of arguments.
```cpp
template <class... Sizes>
shape_t<sizeof...(Sizes)> make_shape(Sizes... sizes);
```

Parameters

* `sizes...` Size along each axis.

Return

* A shape with the given values.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    std::cout << np::make_shape(10) << "\n";
    std::cout << np::make_shape(4, 6) << "\n";
    std::cout << np::make_shape(3, 4, 5) << "\n";
    return 0;
}
```

Output

```
(10,)
(4, 6)
(3, 4, 5)
```

### `make_index`

Create an `index_t` object deducing its dimension from the number of arguments.

`index_t` is just an alias of `shape_t` defined to distinguish between shapes
and indices, improving readability.
```cpp
template <class... Indices>
index_t<sizeof...(Indices)> make_index(Indices... indices);
```

Parameters

* `indices...` Index along each axis.

Returns

* An index with the given values.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    std::cout << np::make_index(10) << "\n";
    std::cout << np::make_index(4, 6) << "\n";
    std::cout << np::make_index(3, 4, 5) << "\n";
    return 0;
}
```

Output

```
(10,)
(4, 6)
(3, 4, 5)
```

### `make_strides`

Return a tuple of strides to offset a contiguous memory array as a
multidimensional array. The elements in the array can be offset by
```
    data[index[0]*stride[0] + ... + index[N-1]*stride[N-1]]
```
where `data` is the memory array.
```cpp
template <size_t Rank>
shape_t<Rank> make_strides(
    const shape_t<Rank> &shape, layout_t order = row_major
);
```

Parameters

* `shape` The shape of the tensor.
* `order` Determines whether the strides should be computed for row-major or
column-major order. Defaults to row-major order.

Returns

* The strides for each dimension.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<2> shape(3, 4);
    int data[12] = {-5, -3, 10, 4, 6, -1, -5, 9, 9, 14, 3, 5};

    std::cout << "Row-major order:\n";
    np::shape_t<2> strides = np::make_strides(shape, np::row_major);
    for (unsigned i = 0; i < shape[0]; ++i) {
        for (unsigned j = 0; j < shape[1]; ++j) {
            std::cout.width(2);
            std::cout << data[i*strides[0] + j*strides[1]] << ", ";
        }
        std::cout << "\n";
    }

    std::cout << "Column-major order:\n";
    strides = np::make_strides(shape, np::column_major);
    for (unsigned i = 0; i < shape[0]; ++i) {
        for (unsigned j = 0; j < shape[1]; ++j) {
            std::cout.width(2);
            std::cout << data[i*strides[0] + j*strides[1]] << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
Row-major order:
-5, -3, 10,  4,
 6, -1, -5,  9,
 9, 14,  3,  5,
Column-major order:
-5,  4, -5, 14,
-3,  6,  9,  3,
10, -1,  9,  5,
```

### `ravel_index`

Converts a tuple of indices into a flat index.
```cpp
template <size_t Rank>
size_t ravel_index(
    const index_t<Rank> &index, const shape_t<Rank> &shape,
    layout_t order = row_major
);
```

Parameters

* `index` A tuple of indices to flatten.
* `shape` The shape of the tensor used for raveling.
* `order` Determines whether the indices should be viewed as indexing in
row-major or column-major order. Defaults to row-major order.

Returns

* The flattened index.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<2> shape(3, 4);
    int data[12] = {-5, -3, 10, 4, 6, -1, -5, 9, 9, 14, 3, 5};

    std::cout << "Row-major order:\n";
    for (unsigned i = 0; i < shape[0]; ++i) {
        for (unsigned j = 0; j < shape[1]; ++j) {
            std::cout.width(2);
            std::cout << data[np::ravel_index({i, j}, shape, np::row_major)];
            std::cout << ", ";
        }
        std::cout << "\n";
    }

    std::cout << "Column-major order:\n";
    for (unsigned i = 0; i < shape[0]; ++i) {
        for (unsigned j = 0; j < shape[1]; ++j) {
            std::cout.width(2);
            std::cout << data[np::ravel_index({i, j}, shape, np::column_major)];
            std::cout << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
Row-major order:
-5, -3, 10,  4,
 6, -1, -5,  9,
 9, 14,  3,  5,
Column-major order:
-5,  4, -5, 14,
-3,  6,  9,  3,
10, -1,  9,  5,
```

### `unravel_index`

Converts a flat index into a tuple of indices.
```cpp
template <size_t Rank>
index_t<Rank> unravel_index(
    size_t index, const shape_t<Rank> &shape, layout_t order = row_major
);
```

Parameters

* `index` Index to unravel.
* `shape` The shape of the tensor used for unraveling.
* `order` Determines whether the indices should be viewed as indexing in
row-major or column-major order. Defaults to row-major order.

Returns

* The unraveled index.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<2> shape(3, 4);
    int data[3][4] = {{-5, -3, 10, 4},
                      {6, -1, -5, 9},
                      {9, 14, 3, 5}};

    std::cout << "Row-major iteration:\n";
    for (unsigned flat = 0; flat < 12; ++flat) {
        np::index_t<2> index = np::unravel_index(flat, shape, np::row_major);
        std::cout << data[index[0]][index[1]] << ", ";
    }
    std::cout << "\n";

    std::cout << "Column-major iteration:\n";
    for (unsigned flat = 0; flat < 12; ++flat) {
        np::index_t<2> index = np::unravel_index(flat, shape, np::column_major);
        std::cout << data[index[0]][index[1]] << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
Row-major iteration:
-5, -3, 10, 4, 6, -1, -5, 9, 9, 14, 3, 5,
Column-major iteration:
-5, 6, 9, -3, -1, 14, 10, -5, 3, 4, 9, 5,
```

### `broadcast_shapes`

Broadcast input shapes into a common shape.
```cpp
template <size_t Rank, class... Shapes>
shape_t<Rank> broadcast_shapes(
    const shape_t<Rank> &shape1, const Shapes&... shape2
);
```

Two dimensions are said to be compatible if

* they are equal or,
* one of them is 1

The size of the resulting broadcasting is the size that is not 1 along each
axis of the shapes.

Parameters

* `shape1, shape2...` The shapes to be broadcast against each other. The shapes
must have the same dimension.

Returns

* Broadcasted shape.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<3> shape1, shape2;
    std::cin >> shape1 >> shape2;
    std::cout << np::broadcast_shapes(shape1, shape2) << "\n";
    return 0;
}
```

Input

```
(3, 4, 1)
(1, 4, 6)
```

Output

```
(3, 4, 6)
```

Input

```
(3, 4, 6)
(2, 1, 6)
```

Possible output

```
terminate called after throwing an instance of 'std::invalid_argument'
  what():  operands could not be broadcast together with shapes (3, 4, 6) (2, 1, 6)
```

### `shape_cat`

Constructs a shape that is the concatenation of one or more shapes.
```cpp
template <size_t Rank, class... Shapes>
shape_t</* Rank of concatenation */> shape_cat(
    const shape_t<Rank> &shape1, const Shapes&... shape2
);
```

Parameters

* `shape1, shape2...` The shapes to concatenate.

Returns

* The concatenated shape.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<3> shape1, shape2;
    std::cin >> shape1 >> shape2;
    std::cout << np::shape_cat(shape1, shape2) << "\n";
    return 0;
}
```

Input

```
(1, 4, 2)
(3, 5, 6)
```

Output

```
(1, 4, 2, 3, 5, 6)
```
