# [Shapes](./readme.md)

Defined in header [`numcpp/shape.h`](/include/numcpp/shape.h)

- [Shapes](#shapes)
    - [`make_shape`](#make_shape)
    - [`make_index`](#make_index)
    - [`make_strides`](#make_strides)
    - [`ravel_index`](#ravel_index)
    - [`unravel_index`](#unravel_index)
    - [`broadcast_shapes`](#broadcast_shapes)
    - [`shape_cat`](#shape_cat)


### `make_shape`

Create a `shape_t` object deducing its dimension from the number of arguments.
```cpp
template <class... Sizes>
shape_t<sizeof...(Sizes)> make_shape(Sizes... sizes);
```

Parameters

* `sizes...` Size along each axis.

Returns

* A shape with the given values.

Example

```cpp
#include <iostream>
#include "numcpp/shape.h"
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

`index_t` is just an alias of `shape_t` defined to distinguish between shapes and indices, improving readability.
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
#include "numcpp/shape.h"
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

Return a tuple of strides to offset a contiguous memory array as a multidimensional array. The elements in the array can be offset by

```
    data[index[0]*stride[0] + ... + index[N-1]*stride[N-1]]
```

where `data` is the memory array.
```cpp
template <size_t Rank>
shape_t<Rank> make_strides(const shape_t<Rank> &shape,
                           layout_t order = default_layout);
```

Parameters

* `shape` The shape of the tensor.
* `order` Determines whether the strides should be computed for row-major or column-major order. Defaults to row-major order.

Returns

* The strides for each dimension.

Example

```cpp
#include <iostream>
#include "numcpp/shape.h"
namespace np = numcpp;

void print_matrix(int data[], const np::shape_t<2> &shape,
                  const np::shape_t<2> &strides) {
    for (unsigned i = 0; i < shape[0]; ++i) {
        for (unsigned j = 0; j < shape[1]; ++j) {
            std::cout.width(2);
            std::cout << data[i*strides[0] + j*strides[1]] << ", ";
        }
        std::cout << "\n";
    }
}

int main() {
    np::shape_t<2> shape(3, 4);
    int data[12] = {-5, -3, 10, 4, 6, -1, -5, 9, 9, 14, 3, 5};
    std::cout << "Row-major order:\n";
    print_matrix(data, shape, np::make_strides(shape, np::row_major));
    std::cout << "Column-major order:\n";
    print_matrix(data, shape, np::make_strides(shape, np::column_major));
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
size_t ravel_index(const index_t<Rank> &index, const shape_t<Rank> &shape,
                   layout_t order = default_layout);
```

Parameters

* `index` A tuple of indices to flatten.
* `shape` The shape of the tensor used for raveling.
* `order` Determines whether the indices should be viewed as indexing in row-major or column-major order. Defaults to row-major order.

Returns

* The flattened index.

Example

```cpp
#include <iostream>
#include "numcpp/shape.h"
namespace np = numcpp;

void print_matrix(int data[], const np::shape_t<2> &shape, np::layout_t order) {
    for (unsigned i = 0; i < shape[0]; ++i) {
        for (unsigned j = 0; j < shape[1]; ++j) {
            std::cout.width(2);
            std::cout << data[np::ravel_index({i, j}, shape, order)] << ", ";
        }
        std::cout << "\n";
    }
}

int main() {
    np::shape_t<2> shape(3, 4);
    int data[12] = {-5, -3, 10, 4, 6, -1, -5, 9, 9, 14, 3, 5};
    std::cout << "Row-major order:\n";
    print_matrix(data, shape, np::row_major);
    std::cout << "Column-major order:\n";
    print_matrix(data, shape, np::column_major);
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
index_t<Rank> unravel_index(size_t index, const shape_t<Rank> &shape,
                            layout_t order = default_layout);
```

Parameters

* `index` Index to unravel.
* `shape` The shape of the tensor used for unraveling.
* `order` Determines whether the indices should be viewed as indexing in row-major or column-major order. Defaults to row-major order.

Returns

* The unraveled index.

Example

```cpp
#include <iostream>
#include "numcpp/shape.h"
namespace np = numcpp;

void flatten_matrix(int data[3][4], const np::shape_t<2> &shape,
                    np::layout_t order) {
    for (unsigned flat = 0; flat < shape.prod(); ++flat) {
        np::index_t<2> index = np::unravel_index(flat, shape, order);
        unsigned i = index[0], j = index[1];
        std::cout << data[i][j] << ", ";
    }
    std::cout << "\n";
}

int main() {
    np::shape_t<2> shape(3, 4);
    int data[3][4] = {{-5, -3, 10, 4},
                      { 6, -1, -5, 9},
                      { 9, 14 , 3, 5}};
    std::cout << "Flatten in row-major order:\n";
    flatten_matrix(data, shape, np::row_major);
    std::cout << "Flatten in column-major order:\n";
    flatten_matrix(data, shape, np::column_major);
    return 0;
}
```

Output

```
Flatten in row-major order:
-5, -3, 10, 4, 6, -1, -5, 9, 9, 14, 3, 5, 
Flatten in column-major order:
-5, 6, 9, -3, -1, 14, 10, -5, 3, 4, 9, 5, 
```

### `broadcast_shapes`

Broadcast input shapes into a common shape.
```cpp
template <size_t Rank, class... Shapes>
shape_t<Rank> broadcast_shapes(const shape_t<Rank> &shape1,
                               const Shapes &...shape2);
```

Two dimensions are said to be compatible if

- they are equal or,
- one of them is 1

The size of the resulting broadcasting is the size that is not 1 along each axis of the shapes.

Parameters

* `shape1, shape2...` The shapes to be broadcast against each other. The shapes must have the same dimension.

Returns

* Broadcasted shape.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp/shape.h"
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
shape_t</* Rank of concatenation */> shape_cat(const shape_t<Rank> &shape1,
                                               const Shapes &...shape2);
```

Parameters

* `shape1, shape2...` The shapes to concatenate.

Returns

* The concatenated shape.

Example

```cpp
#include <iostream>
#include "numcpp/shape.h"
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
