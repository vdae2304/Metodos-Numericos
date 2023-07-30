# [Indirect tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Indirect tensor](#indirect-tensor)
    - [`indirect_tensor::operator()`](#indirect_tensoroperator)
    - [`indirect_tensor::operator[]`](#indirect_tensoroperator-1)
    - [`indirect_tensor::iat`](#indirect_tensoriat)
    - [`indirect_tensor::shape`](#indirect_tensorshape)
    - [`indirect_tensor::size`](#indirect_tensorsize)
    - [`indirect_tensor::empty`](#indirect_tensorempty)
    - [`indirect_tensor::data`](#indirect_tensordata)
    - [`indirect_tensor::indptr`](#indirect_tensorindptr)
    - [`indirect_tensor::layout`](#indirect_tensorlayout)
    - [`indirect_tensor::is_contiguous`](#indirect_tensoris_contiguous)

### `indirect_tensor::operator()`

Return a reference to the element at the given position. The elements in an `indirect_tensor` are given by
```
    data[indptr[ravel_index(index, shape, order)]]
```
where `data` is the memory array and `indptr` is the array of indices.
```cpp
template <class... Indices>
T &operator()(Indices... indices);

template <class... Indices>
const T &operator()(Indices... indices) const;
```

Parameters

* `indices...` Position of an element along each axis.

Returns

* The element at the specified position. If the `indirect_tensor` is const-qualified, the function returns a reference to `const T`. Otherwise, it returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example


Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    size_t indptr[5] = {9, 4, 0, 7, 5};
    np::indirect_vector<int> view(data, 5, indptr);
    for (unsigned i = 0; i < view.size(); ++i) {
        // Prints element at position indptr[i]
        std::cout << view(i) << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
3, 5, 7, 7, 8, 
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    int data[24] = {15, -4,  3, 18, -2,  7, 8, 11, 19 , 0,
                    -5, 14, 16, 19,  9, 12, 12, 18, -5, 11,
                     5, 10 , 8, 10};
    size_t indptr[8] = { 0, 1, 7, 8, 14, 15, 21, 22};
    np::indirect_matrix<int> view(data, {4, 2}, indptr);
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            // Prints element at position indptr[ravel_index({i, j}, view.shape())]
            std::cout << view(i, j) << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
15, -4, 
11, 19, 
9, 12, 
10, 8, 
```

### `indirect_tensor::operator[]`

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

* `index` An `index_t` object with the position of an element in the `indirect_tensor`. Since C++23, the elements of the index can be passed as separate arguments.

Returns

* The element at the specified position. If the `indirect_tensor` is const-qualified, the function returns a reference to `const T`. Otherwise, it returns a reference to `T`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    size_t indptr[5] = {9, 4, 0, 7, 5};
    np::indirect_vector<int> view(data, 5, indptr);
    for (unsigned i = 0; i < view.size(); ++i) {
        // Prints element at position indptr[i]
        std::cout << view[i] << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
3, 5, 7, 7, 8, 
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    int data[24] = {15, -4,  3, 18, -2,  7, 8, 11, 19 , 0,
                    -5, 14, 16, 19,  9, 12, 12, 18, -5, 11,
                     5, 10 , 8, 10};
    size_t indptr[8] = { 0, 1, 7, 8, 14, 15, 21, 22};
    np::indirect_matrix<int> view(data, {4, 2}, indptr);
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            // Prints element at position indptr[ravel_index({i, j}, view.shape())]
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
15, -4, 
11, 19, 
9, 12, 
10, 8,
```

### `indirect_tensor::iat`

Return a reference to the element at the given position in the array of indices. The elements in an `indirect_tensor` can be accessed as
```
    data[iat(index)]
```
where `data` is the memory array.
```cpp
size_type &iat(const index_type &index);
const size_type &iat(const index_type &index) const;

template <class... Indices>
size_type &iat(Indices... indices);

template <class... Indices>
const size_type &iat(Indices... indices) const;
```

Parameters

* `index` The position of an element in the array of indices. It can be an `index_t` object or the elements of the index passed as separate arguments.

Returns

* The element at the specified position. If the `indirect_tensor` is const-qualified, the function returns a reference to `const size_type`. Otherwise, it returns a reference to `size_type`.

Exceptions

* `std::out_of_range` Thrown if index is out of bounds.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    size_t indptr[5] = {9, 4, 0, 7, 5};
    np::indirect_vector<int> view(data, 5, indptr);
    for (unsigned i = 0; i < view.size(); ++i) {
        // Prints index at position i
        std::cout << view.iat(i) << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
9, 4, 0, 7, 5, 
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    int data[24] = {15, -4,  3, 18, -2,  7, 8, 11, 19 , 0,
                    -5, 14, 16, 19,  9, 12, 12, 18, -5, 11,
                     5, 10 , 8, 10};
    size_t indptr[8] = { 0, 1, 7, 8, 14, 15, 21, 22};
    np::indirect_matrix<int> view(data, {4, 2}, indptr);
    for (unsigned i = 0; i < view.shape(0); ++i) {
        for (unsigned j = 0; j < view.shape(1); ++j) {
            // Prints index at position ravel_index({i, j}, view.shape())
            std::cout << view.iat(i, j) << ", ";
        }
        std::cout << "\n";
    }
    return 0;
}
```

Output

```
0, 1, 
7, 8, 
14, 15, 
21, 22, 
```

### `indirect_tensor::shape`

Return the shape of the `indirect_tensor`.
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
    int data1[10];
    np::indirect_vector<int> vec(data1, 10);
    std::cout << "1 dimensional:\n";
    std::cout << "Shape: " << vec.shape() << "\n";
    std::cout << "Length: " << vec.shape(0) << "\n\n";

    int data2[12];
    np::indirect_matrix<int> mat(data2, 3, 4);
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
Shape: (10,)
Length: 10

2 dimensional:
Shape: (3, 4)
Rows: 3
Columns: 4
```

### `indirect_tensor::size`

Return the number of elements in the `indirect_tensor` (i.e., the product of the sizes along all the axes).
```cpp
size_type size() const;
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    int data1[10];
    np::indirect_vector<int> vec(data1, 10);
    std::cout << vec.size() << "\n";
    int data2[12];
    np::indirect_matrix<int> mat(data2, 3, 4);
    std::cout << mat.size() << "\n";
    return 0;
}
```

Output

```
10
12
```

### `indirect_tensor::empty`

Return whether the indirect_tensor is empty (i.e., whether its size is 0).
```cpp
bool empty() const;
```

### `indirect_tensor::data`

Return a pointer to the memory array used internally by the `indirect_tensor`.
```cpp
T *data();
const T *data() const;
```

Returns

* A pointer to the memory array used internally by the `indirect_tensor`. If the `indirect_tensor` is const-qualified, the function returns a pointer to `const T`. Otherwise, it returns a pointer to `T`.

### `indirect_tensor::indptr`

Return a pointer to the array of indices used internally by the `indirect_tensor`.
```cpp
size_type *indptr();
const size_type *indptr() const;
```

Parameters

* A pointer to the array of indices used internally by the `indirect_tensor`. If the `indirect_tensor` is const-qualified, the function returns a pointer to `const size_type`. Otherwise, it returns a pointer to `size_type`.

### `indirect_tensor::layout`

Return the order in which elements are iterated. It is not necessarily the memory layout in which elements are stored as the elements might not be contiguous in memory.
```cpp
layout_t layout() const;
```

### `indirect_tensor::is_contiguous`

Return whether the elements in the `indirect_tensor` are stored contiguously. For `indirect_tensor` class, always returns `false`.
```cpp
bool is_contiguous() const;
```
