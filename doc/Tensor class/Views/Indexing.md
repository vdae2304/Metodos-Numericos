# [Tensor view](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor view](#tensor-view)
  - [Indexing](#indexing)
    - [`tensor_view::operator()`](#tensor_viewoperator)
    - [`tensor_view::operator[]`](#tensor_viewoperator-1)
    - [`tensor_view::shape`](#tensor_viewshape)
    - [`tensor_view::size`](#tensor_viewsize)
    - [`tensor_view::empty`](#tensor_viewempty)
    - [`tensor_view::data`](#tensor_viewdata)
    - [`tensor_view::strides`](#tensor_viewstrides)
    - [`tensor_view::layout`](#tensor_viewlayout)
    - [`tensor_view::is_contiguous`](#tensor_viewis_contiguous)

## Indexing

### `tensor_view::operator()`

Return a reference to the element at the given position. The elements in a `tensor_view` are given by
```
    data[index[0]*stride[0] + ... + index[rank-1]*stride[rank-1]]
```
where `data` is the memory array.
```cpp
template <class... Indices>
T &operator()(Indices... indices);

template <class... Indices>
const T &operator()(Indices... indices) const;
```

Parameters

* `indices...` Position of an element along each axis.

Returns

* The element at the specified position. If the `tensor_view` is const-qualified, the function returns a reference to `const T`. Otherwise, it returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    np::vector_view<int> view(data, /*shape=*/5, /*offset=*/2, /*strides=*/1);
    for (unsigned i = 0; i < view.size(); ++i) {
        // Prints element at position 2 + 1*i
        std::cout << view(i) << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
19, 11, 5, 8, -2,
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data[24] = {15, -4,  3, 18, -2,  7,
                     8, 11, 19,  0, -5, 14,
                    16, 19,  9, 12, 12, 18,
                    -5, 11,  5, 10,  8, 10};
    np::matrix_view<int> view(data, /*shape=*/{3, 4}, /*offset=*/2,
                              /*strides=*/{6, 1});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            // Prints element at position 2 + 6*i + 1*j
            std::cout << view(i, j) << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
3, 18, -2, 7, 
19, 0, -5, 14, 
9, 12, 12, 18, 
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data[24] = {-5, 19,  5, 18,
                    13,  1,  9, 14,
                    15, 12, 14, 16,

                     2, 14, -2 , 3,
                    18, 11 , 9, 18,
                     6, 19, -2,  1};
    np::tensor_view<int, 3> view(data, /*shape=*/{2, 3, 2}, /*offset=*/1,
                                 /*strides=*/{12, 4, 2});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            for (unsigned k = 0; k < view.shape(2); ++k) {
                // Prints element at position 1 + 12*i + 4*j + 2*k
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

Return a reference to the element at the given position.
```cpp
T &operator[](const index_type &index);
const T &operator[](const index_type &index) const;

// Since C++23
template <class... Indices>
T &operator[](Indices... indices);

template <class... Indices>
const T &operator[](Indices... indices) const;
```

Parameters

* `index` An `index_t` object with the position of an element in the `tensor_view`. Since C++23, the elements of the index can be passed as separate arguments.

Returns

* The element at the specified position. If the `tensor_view` is const-qualified, the function returns a reference to `const T`. Otherwise, it returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    np::vector_view<int> view(data, /*shape=*/5, /*offset=*/2, /*strides=*/1);
    for (unsigned i = 0; i < view.size(); ++i) {
        // Prints element at position 2 + 1*i
        std::cout << view[i] << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
19, 11, 5, 8, -2, 
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data[24] = {15, -4,  3, 18, -2,  7,
                     8, 11, 19,  0, -5, 14,
                    16, 19,  9, 12, 12, 18,
                    -5, 11,  5, 10,  8, 10};
    np::matrix_view<int> view(data, /*shape=*/{3, 4}, /*offset=*/2,
                              /*strides=*/{6, 1});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            // Prints element at position 2 + 6*i + 1*j
            // std::cout << view[i, j] << ", "; // Valid since C++23
            std::cout << view[{i, j}] << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
3, 18, -2, 7, 
19, 0, -5, 14, 
9, 12, 12, 18, 
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data[24] = {-5, 19,  5, 18,
                    13,  1,  9, 14,
                    15, 12, 14, 16,

                     2, 14, -2 , 3,
                    18, 11 , 9, 18,
                     6, 19, -2,  1};
    np::tensor_view<int, 3> view(data, /*shape=*/{2, 3, 2}, /*offset=*/1,
                                 /*strides=*/{12, 4, 2});
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            for (unsigned k = 0; k < view.shape(2); ++k) {
                // Prints element at position 1 + 12*i + 4*j + 2*k
                // std::cout << view[i, j, k] << ", "; // Valid since C++23
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

### `tensor_view::shape`

Return the shape of the `tensor_view`.
```cpp
const shape_type &shape() const;
size_type shape(size_type axis) const;
```

Parameters

* `axis` It is an optional parameter that changes the return value. If provided, returns the size along the given axis. Otherwise, returns a `shape_t` object with the shape of the tensor along all axes.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data1[10];
    np::vector_view<int> vec(data1, 10);
    std::cout << "1 dimensional:\n";
    std::cout << "Shape: " << vec.shape() << "\n";
    std::cout << "Length: " << vec.shape(0) << "\n\n";

    int data2[12];
    np::matrix_view<int> mat(data2, 3, 4);
    std::cout << "2 dimensional:\n";
    std::cout << "Shape: " << mat.shape() << "\n";
    std::cout << "Rows: " << mat.shape(0) << "\n";
    std::cout << "Columns: " << mat.shape(1) << "\n\n";

    int data3[24];
    np::tensor_view<int, 3> cube(data3, 2, 3, 4);
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

### `tensor_view::size`

Return the number of elements in the `tensor_view` (i.e., the product of the sizes along all the axes).
```cpp
size_type size() const;
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    int data1[10];
    np::vector_view<int> vec(data1, 10);
    std::cout << vec.size() << "\n";
    int data2[12];
    np::matrix_view<int> mat(data2, 3, 4);
    std::cout << mat.size() << "\n";
    int data3[24];
    np::tensor_view<int, 3> cube(data3, 2, 3, 4);
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

### `tensor_view::empty`

Return whether the `tensor_view` is empty (i.e., whether its size is 0).
```cpp
bool empty() const;
```

### `tensor_view::data`

Return a pointer to the memory array used internally by the `tensor_view`.
```cpp
T *data();
const T *data() const;
```

Returns

* A pointer to the memory array used internally by the `tensor_view`. If the `tensor_view` is const-qualified, the function returns a pointer to `const T`. Otherwise, it returns a pointer to `T`.

### `tensor_view::strides`

Return the span that separates the elements in the memory array.
```cpp
const shape_type &strides() const;
size_type strides(size_type axis) const;
```

Parameters

* `axis` It is an optional parameter that changes the return value. If provided, returns the stride along the given axis. Otherwise, returns a `shape_t` object with the strides of the tensor along all axes.

### `tensor_view::layout`

Return the order in which elements are iterated. It is not necessarily the memory layout in which elements are stored as the elements might not be contiguous in memory.
```cpp
layout_t layout() const;
```

### `tensor_view::is_contiguous`

Return whether the elements in the `tensor_view` are stored contiguously.
```cpp
bool is_contiguous() const;
```
