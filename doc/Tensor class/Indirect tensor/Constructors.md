# [Indirect tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Indirect tensor](#indirect-tensor)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [View constructor](#view-constructor)
    - [Index array constructor](#index-array-constructor)
    - [Copy constructor](#copy-constructor)
    - [Move constructor](#move-constructor)

## Constructors

### Default constructor

Constructs an `indirect_tensor` that does not reference any object.
```cpp
indirect_tensor();
```

### View constructor

Constructs an `indirect_tensor` that references the elements of a multidimensional array.
```cpp
template <class... Sizes>
indirect_tensor(T *data, Sizes... sizes);

indirect_tensor(T *data, const shape_type &shape,
                layout_t order = default_layout);
```

Parameters

* data Pointer to the memory array used by the indirect_tensor.
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
    np::indirect_vector<int> vec(data, 10);
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
    np::indirect_matrix<int> mat1(data, 3, 4);
    std::cout << "Row-major order:\n" << mat1 << "\n";
    np::indirect_matrix<int> mat2(data, {3, 4}, np::column_major);
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
    np::indirect_tensor<int, 3> cube1(data, 2, 3, 4);
    std::cout << "Row-major order:\n" << cube1 << "\n";
    np::indirect_tensor<int, 3> cube2(data, {2, 3, 4}, np::column_major);
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

### Index array constructor

Constructs an `indirect_tensor` that references a subset of elements from a multidimensional array.
```cpp
template <class IntegralType>
indirect_tensor(T *data, const shape_type &shape, IntegralType *indptr,
                layout_t order = default_layout);
```

Parameters

* `data` Pointer to the memory array used by the `indirect_tensor`.
* `shape` Number of elements along each axis.
* `indptr` Pointer to the array of indices with its elements identifying which elements of the memory array are selected.
* `order` Order in which elements shall be iterated. In row-major order, the last index is varying the fastest. In column-major order, the first index is varying the fastest. Defaults to row-major order.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int data[10] = {7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    size_t indptr[5] = {9, 4, 0, 7, 5};
    np::indirect_vector<int> view(data, 5, indptr);
    std::cout << view << "\n";
    return 0;
}
```

Output

```
[3, 5, 7, 7, 8]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int data[24] = {15, -4,  3, 18, -2,  7, 8, 11, 19 , 0,
                    -5, 14, 16, 19,  9, 12, 12, 18, -5, 11,
                     5, 10 , 8, 10};
    size_t indptr[8] = { 0, 1, 7, 8, 14, 15, 21, 22};
    np::indirect_matrix<int> view(data, {4, 2}, indptr);
    std::cout << view << "\n";
    return 0;
}
```

Output

```
[[15, -4],
 [11, 19],
 [ 9, 12],
 [10,  8]]
```

### Copy constructor

Constructs an `indirect_tensor` as a copy of `other`.
```cpp
indirect_tensor(const indirect_tensor &other);
```

Parameters

* `other` An `indirect_tensor` of the same type and rank.

### Move constructor

Constructs an `indirect_tensor` that acquires the elements of `other`.
```cpp
indirect_tensor(indirect_tensor &&other);
```

Parameters

* `other` An `indirect_tensor` of the same type and rank. The ownership of the array of indices is directly transferred from `other`. `other` is left in an empty state.
