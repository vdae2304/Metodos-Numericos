# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Indexing](#indexing)
    - [`tensor::operator()`](#tensoroperator)
    - [`tensor::operator[]`](#tensoroperator-1)
    - [`tensor::shape`](#tensorshape)
    - [`tensor::size`](#tensorsize)
    - [`tensor::empty`](#tensorempty)
    - [`tensor::data`](#tensordata)
    - [`tensor::layout`](#tensorlayout)
    - [`tensor::is_contiguous`](#tensoris_contiguous)

## Indexing

### `tensor::operator()`

Return a reference to the element at the given position.
```cpp
template <class... Indices>
T &operator()(Indices... indices);

template <class... Indices>
const T &operator()(Indices... indices) const;
```

Parameters

* `indices...` Position of an element along each axis.

Returns

* The element at the specified position. If the tensor is const-qualified, the function returns a reference to `const T`. Otherwise, it returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec(10);
    int value = 0;
    for (unsigned i = 0; i < vec.size(); ++i) {
        vec(i) = value++;
    }
    std::cout << vec << "\n";
}
```

Output

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> mat(3, 4);
    int value = 0;
    for (unsigned i = 0; i < mat.shape(0); ++i) {
        for (unsigned j = 0; j < mat.shape(1); ++j) {
            mat(i, j) = value++;
        }
    }
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 4);
    int value = 0;
    for (unsigned i = 0; i < cube.shape(0); ++i) {
        for (unsigned j = 0; j < cube.shape(1); ++j) {
            for (unsigned k = 0; k < cube.shape(2); ++k) {
                cube(i, j, k) = value++;
            }
        }
    }
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
```

### `tensor::operator[]`

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

* `index` An `index_t` object with the position of an element in the tensor. Since C++23, the elements of the index can be passed as separate arguments.

Returns

* The element at the specified position. If the tensor is const-qualified, the function returns a reference to `const T`. Otherwise, it returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec(10);
    int value = 0;
    for (unsigned i = 0; i < vec.size(); ++i) {
        vec[i] = value++;
    }
    std::cout << vec << "\n";
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
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> mat(3, 4);
    int value = 0;
    for (unsigned i = 0; i < mat.shape(0); ++i) {
        for (unsigned j = 0; j < mat.shape(1); ++j) {
            // mat[i, j] = value++; // Valid since C++23.
            mat[{i, j}] = value++;
        }
    }
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 4);
    int value = 0;
    for (unsigned i = 0; i < cube.shape(0); ++i) {
        for (unsigned j = 0; j < cube.shape(1); ++j) {
            for (unsigned k = 0; k < cube.shape(2); ++k) {
                // cube[i, j, k] = value++; // Valid since C++23.
                cube[{i, j, k}] = value++;
            }
        }
    }
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
```

### `tensor::shape`

Return the shape of the tensor.
```cpp
const shape_type &shape() const;
size_type shape(size_type axis) const;
```

Parameters

* `axis` It is an optional parameter that changes the return value. If provided, returns the size along the given axis. Otherwise, returns a `shape_t` object with the shape of the tensor along all axes.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec(10);
    std::cout << "1 dimensional:\n";
    std::cout << "Shape: " << vec.shape() << "\n";
    std::cout << "Length: " << vec.shape(0) << "\n\n";

    np::matrix<int> mat(3, 4);
    std::cout << "2 dimensional:\n";
    std::cout << "Shape: " << mat.shape() << "\n";
    std::cout << "Rows: " << mat.shape(0) << "\n";
    std::cout << "Columns: " << mat.shape(1) << "\n\n";

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

Return the number of elements in the tensor (i.e., the product of the sizes along all the axes).
```cpp
size_type size() const;
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec(10);
    std::cout << vec.size() << "\n";
    np::matrix<int> mat(3, 4);
    std::cout << mat.size() << "\n";
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

Return whether the tensor is empty (i.e., whether its size is 0).
```cpp
bool empty() const;
```

### `tensor::data`

Return a pointer to the memory array used internally by the tensor. Because elements in the tensor are stored contiguously, the pointer retrieved can be offset to access any element in the tensor.
```cpp
T *data();
const T *data() const;
```

Returns

* A pointer to the memory array used internally by the tensor. If the tensor is const-qualified, the function returns a pointer to `const T`. Otherwise, it returns a pointer to `T`.

### `tensor::layout`

Return the memory layout in which elements are stored.
```cpp
layout_t layout() const;
```

### `tensor::is_contiguous`

Return whether the elements in the tensor are stored contiguously. For `tensor` class, always returns true.
```cpp
bool is_contiguous() const;
```
