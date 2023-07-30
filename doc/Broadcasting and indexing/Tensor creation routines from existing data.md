# [Broadcasting and indexing](readme.md)

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Tensor creation routines from existing data](#tensor-creation-routines-from-existing-data)
    - [`asarray`](#asarray)
    - [`ascontiguousarray`](#ascontiguousarray)
    - [`asfortranarray`](#asfortranarray)
    - [`asarray_chkfinite`](#asarray_chkfinite)

## Tensor creation routines from existing data

### `asarray`

Convert the input to a tensor. The data-type and dimension of the tensor is deduced from the input data.
```cpp
template <class InputIterator>
tensor<typename std::iterator_traits<InputIterator>::value_type, 1>
asarray(InputIterator first, InputIterator last);

template <class InputIterator, class... Sizes>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
asarray(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank>
asarray(InputIterator first, const shape_t<Rank> &shape,
        layout_t order = default_layout);
```

Parameters

* `first` Input iterator to the initial position in a range.
* `last` Input iterator to the final position in a range. The range used is `[first, last)`, which includes all the elements between `first` and `last`, including the element pointed by `first` but not the element pointed by `last`.
* `shape` Number of elements along each axis. It can be a `shape_t` object or the elements of the shape passed as separate arguments.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. Defaults to row-major order.

Returns

* Tensor interpretation of the input data.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int ptr[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    // Return vector<int> of shape (10,)
    auto vec = np::asarray(ptr, 10);
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
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int ptr[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
    // Return matrix<int> of shape (3, 4)
    auto mat1 = np::asarray(ptr, 3, 4);
    std::cout << "Row-major order:\n" << mat1 << "\n";
    // Return matrix<int> of shape (3, 4)
    auto mat2 = np::asarray(ptr, np::make_shape(3, 4), np::column_major);
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
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int ptr[24] = { 1, 18, 11, 10,  9, 19, 12, 10, 13,  8,
                   -4, 16 , 2,  4, 14, 19, 18,  5, 19, 18,
                    0,  0, 15, 17};
    // Return tensor<int, 3> of shape (2, 3, 4)
    auto cube1 = np::asarray(ptr, 2, 3, 4);
    std::cout << "Row-major order:\n" << cube1 << "\n";
    // Return tensor<int, 3> of shape (2, 3, 4)
    auto cube2 = np::asarray(ptr, np::make_shape(2, 3, 4), np::column_major);
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

### `ascontiguousarray`

Return a tensor laid out in C/C++ (row-major) order in memory.
```cpp
template <class InputIterator, class... Sizes>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
ascontiguousarray(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank>
tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
ascontiguousarray(InputIterator first, const shape_t<Rank> &shape);
```

Parameters

* `first` Input iterator to the initial position in a range.
* `shape` Number of elements along each axis. It can be a `shape_t` object or the elements of the shape passed as separate arguments.

Returns

* Tensor interpretation of the input data in row-major order.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int ptr[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
    // Return matrix<int> of shape (3, 4)
    auto mat = np::ascontiguousarray(ptr, 3, 4);
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[[ 0, 10, -4,  5],
 [ 6, 10,  8, 12],
 [ 2, 11,  0, -1]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int ptr[24] = { 1, 18, 11, 10,  9, 19, 12, 10, 13,  8,
                   -4, 16 , 2,  4, 14, 19, 18,  5, 19, 18,
                    0,  0, 15, 17};
    // Return tensor<int, 3> of shape (2, 3, 4)
    auto cube = np::ascontiguousarray(ptr, 2, 3, 4);
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[ 1, 18, 11, 10],
  [ 9, 19, 12, 10],
  [13,  8, -4, 16]],

 [[ 2,  4, 14, 19],
  [18,  5, 19, 18],
  [ 0,  0, 15, 17]]]
```

### `asfortranarray`

Return a tensor laid out in Fortran/Matlab (column-major) order in memory.
```cpp
template <class InputIterator, class... Sizes>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
asfortranarray(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank>
tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asfortranarray(InputIterator first, const shape_t<Rank> &shape);
```

Parameters

* `first` Input iterator to the initial position in a range.
* `shape` Number of elements along each axis. It can be a `shape_t` object or the elements of the shape passed as separate arguments.

Returns

+ Tensor interpretation of the input data in column-major order.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int ptr[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
    // Return matrix<int> of shape (3, 4)
    auto mat = np::asfortranarray(ptr, 3, 4);
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[[ 0,  5,  8, 11],
 [10,  6, 12,  0],
 [-4, 10,  2, -1]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int ptr[24] = { 1, 18, 11, 10,  9, 19, 12, 10, 13,  8,
                   -4, 16 , 2,  4, 14, 19, 18,  5, 19, 18,
                    0,  0, 15, 17};
    // Return tensor<int, 3> of shape (2, 3, 4)
    auto cube = np::asfortranarray(ptr, 2, 3, 4);
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[ 1, 12,  2, 19],
  [11, 13, 14,  0],
  [ 9, -4, 18, 15]],

 [[18, 10,  4, 18],
  [10,  8, 19,  0],
  [19, 16,  5, 17]]]
```

### `asarray_chkfinite`

Convert the input to a tensor, checking for NaNs or Infs.
```cpp
template <class InputIterator>
tensor<typename std::iterator_traits<InputIterator>::value_type, 1>
asarray_chkfinite(InputIterator first, InputIterator last);

template <class InputIterator, class... Sizes>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
asarray_chkfinite(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank>
tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asarray_chkfinite(InputIterator first, const shape_t<Rank> &shape,
                  layout_t order = default_layout);
```

Parameters

* `first` Input iterator to the initial position in a range.
* `last` Input iterator to the final position in a range. The range used is `[first, last)`, which includes all the elements between `first` and `last`, including the element pointed by `first` but not the element pointed by `last`.
* `shape` Number of elements along each axis. It can be a `shape_t` object or the elements of the shape passed as separate arguments.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. Defaults to row-major order.

Returns

* Tensor interpretation of the input data.

Exceptions

* `std::invalid_argument` Thrown if input data contains NaN (Not a Number) or Inf (Infinity).
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;

void print_vector(double *ptr, int n) {
    try {
        // Return vector<double> of shape (n,)
        auto vec = np::asarray_chkfinite(ptr, n);
        std::cout << vec << "\n";
    } catch (std::invalid_argument &error) {
        std::cout << error.what() << "\n";
    }
}

int main() {
    double ptr1[10] = {0., 7., 3., -1., 2., 5., 1., 0., 10., -3.};
    print_vector(ptr1, 10);
    double ptr2[10] = {0., 1./0., 3., -1., 2., 5., 1., 0./0., 10., -3.};
    print_vector(ptr2, 10);
    return 0;
}
```

Possible output

```
[ 0,  7,  3, -1,  2,  5,  1,  0, 10, -3]
array must not contain infs or NaNs
```
