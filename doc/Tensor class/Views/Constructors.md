# [Tensor view](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor view](#tensor-view)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [View constructor](#view-constructor)
    - [Stride constructor](#stride-constructor)
    - [Copy constructor](#copy-constructor)
    - [Move constructor](#move-constructor)

## Constructors

### Default constructor

Constructs a `tensor_view` that does not reference any object.
```cpp
tensor_view();
```

### View constructor

Constructs a `tensor_view` that references the elements of a multidimensional array.
```cpp
template <class... Sizes>
tensor_view(T *data, Sizes... sizes);

tensor_view(T *data, const shape_type &shape, layout_t order = default_layout);
```

Parameters

* `data` Pointer to the memory array used by the `tensor_view`.
* `shape` Number of elements along each axis. It can be a `shape_t` object or the elements of the shape passed as separate arguments.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. Defaults to row-major order.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    np::vector_view<int> vec(data, 10);
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
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
    np::matrix_view<int> mat1(data, 3, 4);
    std::cout << "Row-major order:\n" << mat1 << "\n";
    np::matrix_view<int> mat2(data, {3, 4}, np::column_major);
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
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int data[24] = { 1, 18, 11, 10,  9, 19, 12, 10, 13,  8,
                    -4, 16 , 2,  4, 14, 19, 18,  5, 19, 18,
                     0,  0, 15, 17};
    np::tensor_view<int, 3> cube1(data, 2, 3, 4);
    std::cout << "Row-major order:\n" << cube1 << "\n";
    np::tensor_view<int, 3> cube2(data, {2, 3, 4}, np::column_major);
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

### Stride constructor

Constructs a `tensor_view` that references a subset of elements from a multidimensional array.
```cpp
tensor_view(T *data, const shape_type &shape, difference_type offset,
            const shape_type &strides, layout_t order = default_layout);
```

Parameters

* `data` Pointer to the memory array used by the `tensor_view`.
* `shape` Number of elements along each axis.
* `offset` Relative position of the first element selected by the `tensor_view`.
* `strides` Span that separates the selected elements along each axis.
* `order` Order in which elements shall be iterated. In row-major order, the last index is varying the fastest. In column-major order, the first index is varying the fastest. Defaults to row-major order.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    // Select positions 2, 3, 4, 5, 6
    np::vector_view<int> view1(data, /*shape=*/5, /*offset=*/2, /*strides=*/1);
    std::cout << view1 << "\n";
    // Select positions 1, 3, 5, 7, 9.
    np::vector_view<int> view2(data, /*shape=*/5, /*offset=*/1, /*strides=*/2);
    std::cout << view2 << "\n";
    return 0;
}
```

Output

```
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
    int data[24] = {15, -4,  3, 18, -2,  7,
                     8, 11, 19,  0, -5, 14,
                    16, 19,  9, 12, 12, 18,
                    -5, 11,  5, 10,  8, 10};
    // Select positions of the form
    //   2 + 6*i + 1*j,   0 <= i < 3, 0 <= j < 4
    // i.e., select elements at positions
    //    2,  3,  4,  5
    //    8,  9, 10, 11
    //   14, 15, 16, 17
    np::matrix_view<int> view1(data, /*shape=*/{3, 4}, /*offset=*/2,
                               /*strides=*/{6, 1});
    std::cout << view1 << "\n";
    // Select positions of the form
    //   6 + 0*i + 1*j,  0 <= i < 1, 0 <= j < 6
    // i.e., select elements at positions
    //   6, 7, 8, 9, 10, 11
    np::matrix_view<int> view2(data, /*shape=*/{1, 6}, /*offset=*/6,
                               /*strides=*/{0, 1});
    std::cout << view2 << "\n";
    return 0;
}
```

Output

```
[[ 3, 18, -2,  7],
 [19,  0, -5, 14],
 [ 9, 12, 12, 18]]
[[ 8, 11, 19,  0, -5, 14]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int data[24] = {-5, 19,  5, 18,
                    13,  1,  9, 14,
                    15, 12, 14, 16,

                     2, 14, -2 , 3,
                    18, 11 , 9, 18,
                     6, 19, -2,  1};
    // Select positions of the form
    //   1 + 12*i + 4*j + 1*k,  0 <= i < 2, 0 <= j < 3, 0 <= k < 2
    // i.e., select elements at positions
    //    1,  3
    //    5,  7
    //    9, 11
    //
    //   13, 15
    //   17, 19
    //   21, 23
    np::tensor_view<int, 3> view(data, /*shape=*/{2, 3, 2}, /*offset=*/1,
                                 /*strides=*/{12, 4, 2});
    std::cout << view << "\n";
    return 0;
}
```

Output

```
[[[19, 18],
  [ 1, 14],
  [12, 16]],

 [[14,  3],
  [11, 18],
  [19,  1]]]
```

### Copy constructor

Constructs a `tensor_view` as a copy of `other`.
```cpp
tensor_view(const tensor_view &other);
```

Parameters

* `other` A `tensor_view` of the same type and rank.

### Move constructor

Constructs a `tensor_view` that acquires the elements of `other`.
```cpp
tensor_view(tensor_view &&other);
```

Parameters

* `other` A `tensor_view` of the same type and rank. `other` is left in an empty state.
