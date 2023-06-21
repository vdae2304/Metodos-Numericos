# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Size constructor](#size-constructor)
    - [Fill constructor](#fill-constructor)
    - [Range constructor](#range-constructor)
    - [Copy constructor](#copy-constructor)
    - [Move constructor](#move-constructor)
    - [Initializer list constructor](#initializer-list-constructor)

## Constructors

### Default constructor

Constructs an empty tensor with no elements.
```cpp
tensor();
```

### Size constructor

Constructs a tensor with given shape, each element is left uninitialized.
```cpp
template <class... Sizes>
explicit tensor(Sizes... sizes);

explicit tensor(const shape_type &shape, layout_t order = default_layout);
```

Parameters

* `shape` Number of elements along each axis. It can be a `shape_t` object or the elements of the shape passed as separate arguments.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. Defaults to row-major order.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec(10);
    std::cout << vec.shape() << "\n";
    np::matrix<int> mat(3, 4);
    std::cout << mat.shape() << "\n";
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
tensor(const shape_type &shape, const T &val, layout_t order = default_layout);
```

Parameters

* `shape` Number of elements along each axis.
* `val` Value to which each of the elements is initialized.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. Defaults to row-major order.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec(10, 0);
    std::cout << "1 dimensional:\n" << vec << "\n";
    np::matrix<int> mat({3, 4}, 0);
    std::cout << "2 dimensional:\n" << mat << "\n";
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

Constructs a tensor with given shape, with each element constructed from its corresponding element in the range starting at `first`, in the same order.
```cpp
template <class InputIterator, class... Sizes>
tensor(InputIterator first, Sizes... sizes);

template <class InputIterator>
tensor(InputIterator first, const shape_type &shape,
       layout_t order = default_layout);
```

Parameters

* `first` Input iterator to the initial position in a range.
* `shape` Number of elements along each axis. It can be a `shape_t` object or the elements of the shape passed as separate arguments.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. Defaults to row-major order.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    int ptr[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    np::vector<int> vec(ptr, 10);
    std::cout << vec << "\n";
    return 0;
}
```

Output

```
[-4, 16, 14,  9, 18,  3,  7,  2,  1,  4]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    int ptr[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
    np::matrix<int> mat1(ptr, 3, 4);
    std::cout << "Row-major order:\n" << mat1 << "\n";
    np::matrix<int> mat2(ptr, {3, 4}, np::column_major);
    std::cout << "Column-major order:\n" << mat2 << "\n";
    return 0;
}
```

Output

```
Row-major order:
[[ 0, 10, -4,  5],
 [ 6, 10,  8, 12],
 [ 2, 11,  0, -1]]
Column-major order:
[[ 0,  5,  8, 11],
 [10,  6, 12,  0],
 [-4, 10,  2, -1]]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    int ptr[24] = { 1, 18, 11, 10,  9, 19, 12, 10, 13,  8,
                   -4, 16 , 2,  4, 14, 19, 18,  5, 19, 18,
                    0,  0, 15, 17};
    np::tensor<int, 3> cube1(ptr, 2, 3, 4);
    std::cout << "Row-major order:\n" << cube1 << "\n";
    np::tensor<int, 3> cube2(ptr, {2, 3, 4}, np::column_major);
    std::cout << "Column-major order:\n" << cube2 << "\n";
    return 0;
}
```

Output

```
Row-major order:
[[[ 1, 18, 11, 10],
  [ 9, 19, 12, 10],
  [13,  8, -4, 16]],

 [[ 2,  4, 14, 19],
  [18,  5, 19, 18],
  [ 0,  0, 15, 17]]]
Column-major order:
[[[ 1, 12,  2, 19],
  [11, 13, 14,  0],
  [ 9, -4, 18, 15]],

 [[18, 10,  4, 18],
  [10,  8, 19,  0],
  [19, 16,  5, 17]]]
```

### Copy constructor

Constructs a tensor with a copy of each of the elements in `other`, in the same order.
```cpp
tensor(const tensor &other);

template <class U>
tensor(const tensor<U, Rank> &other);

template <class U>
tensor(const tensor<U, Rank> &other, layout_t order);
```

Parameters

* `other` A tensor-like object of the same rank.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. The default is to use the same layout as `other`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

### Move constructor

Constructs a tensor that acquires the elements of `other`.
```cpp
tensor(tensor &&other);
```

Parameters

* `other` A tensor of the same type and rank. The ownership is directly transferred from `other`. `other` is left in an empty state.

### Initializer list constructor

Constructs a tensor with a copy of each of the elements in `il`, in the same order.
```cpp
tensor(/* Nested initializer_list */ il);
```

Parameters

* `il` An `initializer_list` object.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec{-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    std::cout << "1 dimensional:\n" << vec << "\n";
    np::matrix<int> mat{{0, 10, -4,  5},
                        {6, 10,  8, 12},
                        {2, 11,  0, -1}};
    std::cout << "2 dimensional:\n" << mat << "\n";
    np::tensor<int, 3> cube{{{ 1, 18, 11, 10},
                             { 9, 19, 12, 10},
                             {13,  8, -4, 16}},
                            {{ 2,  4, 14, 19},
                             {18,  5, 19, 18},
                             { 0,  0, 15, 17}}};
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
