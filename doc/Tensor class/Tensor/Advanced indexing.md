# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Advanced indexing](#advanced-indexing)
    - [Slice indexing](#slice-indexing)
    - [Coordinate tensor indexing](#coordinate-tensor-indexing)
    - [Boolean tensor indexing](#boolean-tensor-indexing)

## Advanced indexing

### Slice indexing

Return a [`tensor_view`](../Views/readme.md) object that selects the elements specified by the slices.
```cpp
template <class... Indices>
tensor_view<T, /* Number of slice arguments */>
operator()(Indices... indices);

template <class... Indices>
tensor_view<const T, /* Number of slice arguments */>
operator()(Indices... indices) const;

// Since C++23
template <class... Indices>
tensor_view<T, /* Number of slice arguments */>
operator[](Indices... indices);

template <class... Indices>
tensor_view<const T, /* Number of slice arguments */>
operator[](Indices... indices) const;
```

Parameters

* `indices...` Each argument can be either an integer or a [`slice`](../Slices/readme.md). If an integer is given, an unique position is selected for the axis and the dimensionality of the returned view is reduced by 1. If a slice is given, a subset of positions is selected along the axis. An empty slice can be used as a placeholder to select all the positions along the axis.

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to `const T`, which is convertible to a tensor object. Otherwise, the function returns a `tensor_view` to `T`, which has reference semantics to the original tensor. The dimension of the returned view will equal the number of slice arguments.

Exceptions

* `std::out_of_range` Thrown if an integral index is out of bounds.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    // Select first 5 positions.
    std::cout << vec(np::slice(5)) << "\n";
    // Select positions 2, 3, 4, 5, 6.
    std::cout << vec(np::slice(2, 7)) << "\n";
    // Select positions 1, 3, 5, 7, 9.
    std::cout << vec(np::slice(1, 10, 2)) << "\n";
    return 0;
}
```

Output

```
[ 7, 13, 19, 11,  5]
[19, 11,  5,  8, -2]
[13, 11,  8,  7,  3]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> mat{{15, -4,  3, 18, -2,  7},
                        { 8, 11, 19,  0, -5, 14},
                        {16, 19,  9, 12, 12, 18},
                        {-5, 11,  5, 10,  8, 10}};
    // Select first 3 rows. Select columns 2, 3, 4, 5.
    std::cout << mat(np::slice(3), np::slice(2, 6)) << "\n";
    // Select row numbered 1 (i.e. second row).
    std::cout << mat(1, np::slice()) << "\n";
    // Select column numbered 2 (i.e. third column).
    std::cout << mat(np::slice(), 2) << "\n";
    return 0;
}
```

Output

```
[[ 3, 18, -2,  7],
 [19,  0, -5, 14],
 [ 9, 12, 12, 18]]
[ 8, 11, 19,  0, -5, 14]
[ 3, 19,  9,  5]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube{{{-5, 19,  5, 18},
                             {13,  1,  9, 14},
                             {15, 12, 14, 16}},
                            {{ 2, 14, -2 , 3},
                             {18, 11 , 9, 18},
                             { 6, 19, -2,  1}}};
    // Select first matrix block. Select all rows. Select columns 1, 2, 3.
    std::cout << cube(0, np::slice(), np::slice(1, 4)) << "\n";
    // Select all matrix blocks. Select row numbered 2. Select first 3 columns.
    std::cout << cube(np::slice(), 2, np::slice(3)) << "\n";
    // Select all matrix blocks. Select all rows. Select columns 1, 3.
    std::cout << cube(np::slice(), np::slice(), np::slice(1, 4, 2)) << "\n";
    return 0;
}
```

Output

```
[[19,  5, 18],
 [ 1,  9, 14],
 [12, 14, 16]]
[[15, 12, 14],
 [ 6, 19, -2]]
[[[19, 18],
  [ 1, 14],
  [12, 16]],

 [[14,  3],
  [11, 18],
  [19,  1]]]
```

### Coordinate tensor indexing

Return an [`indirect_tensor`](../Indirect%20tensor/readme.md) that selects the elements specified by the tensor of indices.
```cpp
template <size_t N>
indirect_tensor<T, N> operator[](const tensor<index_type, N> &indices);
template <size_t N>
tensor<T, N> operator[](const tensor<index_type, N> &indices) const;

template <class IntegralType, size_t N>
indirect_tensor<T, N> operator[](const tensor<IntegralType, N> &indices);
template <class IntegralType, size_t N>
tensor<T, N> operator[](const tensor<IntegralType, N> &indices) const;
```

Parameters

* `indices` A tensor-like object of `index_t` with its elements identifying which elements of the tensor are selected. If the tensor is 1-dimensional, a tensor-like object of integers can be used instead.

Returns

* If the tensor is const-qualified, the function returns a new tensor object with a copy of the selection. Otherwise, the function returns an `indirect_tensor`, which has reference semantics to the original tensor.

Exceptions

* `std::out_of_range` Thrown if the indices references to an element out of bounds.
* `std::bad_alloc` If the function needs to allocate storage and fails, it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    np::vector<int> indices{9, 4, 0, 7, 5};
    std::cout << vec[indices] << "\n";
    vec[indices] = 0;
    std::cout << vec << "\n";
    return 0;
}
```

Output

```
[3, 5, 7, 7, 8]
[ 0, 13, 19, 11,  0,  0, -2,  0, 11,  0]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> mat{{15, -4,  3, 18, -2,  7},
                        { 8, 11, 19 , 0, -5, 14},
                        {16, 19,  9, 12, 12, 18},
                        {-5, 11,  5, 10 , 8, 10}};
    np::vector<np::index_t<2>> indices{{0,0}, {0,1}, {1,1}, {1,2}, {2,2},
                                       {2,3}, {3,3}, {3,4}};
    std::cout << mat[indices] << "\n";
    mat[indices] = 0;
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[15, -4, 11, 19,  9, 12, 10,  8]
[[ 0,  0,  3, 18, -2,  7],
 [ 8,  0,  0,  0, -5, 14],
 [16, 19,  0,  0, 12, 18],
 [-5, 11,  5,  0,  0, 10]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube{{{-5, 19,  5, 18},
                             {13,  1,  9, 14},
                             {15, 12, 14, 16}},
                            {{ 2, 14, -2,  3},
                             {18, 11,  9, 18},
                             { 6, 19, -2,  1}}};
    np::matrix<np::index_t<3>> indices{{{0,0,0}, {0,2,0}, {0,1,2}},
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

Return an [`indirect_tensor`](../Indirect%20tensor/readme.md) that selects the elements specified by the boolean mask.
```cpp
indirect_tensor<T, 1> operator[](const tensor<bool, Rank> &mask);
tensor<T, 1> operator[](const tensor<bool, Rank> &mask) const;
```

Parameters

* `mask` A tensor-like object of `bool` with its elements identifying whether each element of the tensor is selected or not.

Returns

* If the tensor is const-qualified, the function returns a new tensor object with a copy of the selection. Otherwise, the function returns an `indirect_tensor`, which has reference semantics to the original tensor.

Exceptions

* `std::invalid_argument` Thrown if the tensor and the boolean mask have different shapes.
* `std::bad_alloc` If the function needs to allocate storage and fails, it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    // Select all elements greater than 10.
    std::cout << vec[vec > 10] << "\n";
    vec[vec > 10] = 0;
    std::cout << vec << "\n";
    return 0;
}
```

Output

```
[13, 19, 11, 11]
[ 7,  0,  0,  0,  5,  8, -2,  7,  0,  3]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> mat{{15, -4,  3, 18, -2,  7},
                        { 8, 11, 19,  0, -5, 14},
                        {16, 19,  9, 12, 12, 18},
                        {-5, 11,  5, 10,  8, 10}};
    // Select all elements less or equal to zero.
    std::cout << mat[mat <= 0] << "\n";
    mat[mat <= 0] = 0;
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[-4, -2,  0, -5, -5]
[[15,  0,  3, 18,  0,  7],
 [ 8, 11, 19,  0,  0, 14],
 [16, 19,  9, 12, 12, 18],
 [ 0, 11,  5, 10,  8, 10]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube{{{-5, 19,  5, 18},
                             {13,  1,  9, 14},
                             {15, 12, 14, 16}},
                            {{ 2, 14, -2,  3},
                             {18, 11,  9, 18},
                             { 6, 19, -2,  1}}};
    // Select all odd elements.
    std::cout << cube[cube % 2 != 0] << "\n";
    cube[cube % 2 != 0] = 0;
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[-5, 19,  5, 13,  1,  9, 15,  3, 11,  9, 19,  1]
[[[ 0,  0,  0, 18],
  [ 0,  0,  0, 14],
  [ 0, 12, 14, 16]],

 [[ 2, 14, -2,  0],
  [18,  0,  0, 18],
  [ 6,  0, -2,  0]]]
```
