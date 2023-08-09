# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Tensor creation routines](#tensor-creation-routines)
    - [`empty`](#empty)
    - [`empty_like`](#empty_like)
    - [`zeros`](#zeros)
    - [`zeros_like`](#zeros_like)
    - [`ones`](#ones)
    - [`ones_like`](#ones_like)
    - [`full`](#full)
    - [`full_like`](#full_like)

## Tensor creation routines

### `empty`

Return a new tensor of given shape without initializing entries.
```cpp
template <class T = double, class... Sizes>
tensor<T, sizeof...(Sizes)> empty(Sizes... sizes);

template <class T = double, size_t Rank>
tensor<T, Rank> empty(const shape_t<Rank> &shape);
```

Template parameters

* `T` Type of the elements contained in the tensor. Default is `double`.

Parameters

* `shape` Shape of the tensor. It can be a `shape_t` object or the elements of the shape passed as separate arguments.

Returns

* A tensor of uninitialized data with the given shape.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return vector<int> of shape(10,).
    auto vec = np::empty<int>(10);
    std::cout << vec.shape() << "\n";
    return 0;
}
```

Output

```
(10,)
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return matrix<int> of shape (3, 4).
    auto mat = np::empty<int>(3, 4);
    std::cout << mat.shape() << "\n";
    return 0;
}
```

Output

```
(3, 4)
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return tensor<int, 3> of shape (2, 3, 4).
    auto cube = np::empty<int>(2, 3, 4);
    std::cout << cube.shape() << "\n";
    return 0;
}
```

Output

```
(2, 3, 4)
```

### `empty_like`

Return a new tensor with the same shape and type as a given tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> empty_like(const tensor<T, Rank> &like);
```

Parameters

* `like` Tensor-like object defining the shape and data type.

Returns

* A tensor of uninitialized data with the same shape and type as `like`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::vector<int> like{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto vec = np::empty_like(like);
    std::cout << vec.shape() << "\n";
    return 0;
}
```

Output

```
(10,)
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like{{ 0,  1,  2,  3},
                         { 4,  5,  6,  7},
                         { 8,  9, 10, 11}};
    auto mat = np::empty_like(like);
    std::cout << mat.shape() << "\n";
    return 0;
}
```

Output

```
(3, 4)
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (2, 3, 4).
    np::tensor<int, 3> like{{{ 0,  1,  2,  3},
                             { 4,  5,  6,  7},
                             { 8,  9, 10, 11}},
                            {{12, 13, 14, 15},
                             {16, 17, 18, 19},
                             {20, 21, 22, 23}}};
    auto cube = np::empty_like(like);
    std::cout << cube.shape() << "\n";
    return 0;
}
```

Output

```
(2, 3, 4)
```

### `zeros`

Return a tensor of given shape filled with zeros.
```cpp
template <class T = double, class... Sizes>
tensor<T, sizeof...(Sizes)> zeros(Sizes... sizes);

template <class T = double, size_t Rank>
tensor<T, Rank> zeros(const shape_t<Rank> &shape);
```

Template parameters

* `T` Type of the elements contained in the tensor. Default is `double`.

Parameters

* `shape` Shape of the tensor. It can be a `shape_t` object or the elements of the shape passed as separate arguments.

Returns

* A tensor of zeros with the given shape. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return vector<int> of shape (10,) filled with zeros.
    np::vector<int> vec = np::zeros(10);
    std::cout << vec << "\n";
    return 0;
}
```

Output

```
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return matrix<int> of shape (3, 4) filled with zeros.
    np::matrix<int> mat = np::zeros(3, 4);
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[[0, 0, 0, 0],
 [0, 0, 0, 0],
 [0, 0, 0, 0]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return tensor<int, 3> of shape (2, 3, 4) filled with zeros.
    np::tensor<int, 3> cube = np::zeros(2, 3, 4);
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]],

 [[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]]]
```

### `zeros_like`

Return a tensor of zeros with the same shape and type as a given tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> zeros_like(const tensor<T, Rank> &like);
```

Parameters

* `like` Tensor-like object defining the shape and data type.

Returns

* A tensor of zeros with the same shape and type as `like`. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::vector<int> like{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << np::zeros_like(like) << "\n";
    return 0;
}
```

Output

```
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like{{ 0,  1,  2,  3},
                         { 4,  5,  6,  7},
                         { 8,  9, 10, 11}};
    std::cout << np::zeros_like(like) << "\n";
    return 0;
}
```

Output

```
[[0, 0, 0, 0],
 [0, 0, 0, 0],
 [0, 0, 0, 0]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (2, 3, 4).
    np::tensor<int, 3> like{{{ 0,  1,  2,  3},
                             { 4,  5,  6,  7},
                             { 8,  9, 10, 11}},
                            {{12, 13, 14, 15},
                             {16, 17, 18, 19},
                             {20, 21, 22, 23}}};
    std::cout << np::zeros_like(like) << "\n";
    return 0;
}
```

Output

```
[[[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]],

 [[0, 0, 0, 0],
  [0, 0, 0, 0],
  [0, 0, 0, 0]]]
```

### `ones`

Return a tensor of given shape filled with ones.
```cpp
template <class T = double, class... Sizes>
tensor<T, sizeof...(Sizes)> ones(Sizes... sizes);

template <class T = double, size_t Rank>
tensor<T, Rank> ones(const shape_t<Rank> &shape);
```

Template parameters

* `T` Type of the elements contained in the tensor. Default is `double`.

Parameters

* `shape` Shape of the tensor. It can be a `shape_t` object or the elements of the shape passed as separate arguments.

Returns

* A tensor of ones with the given shape. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return vector<int> of shape (10,) filled with ones.
    np::vector<int> vec = np::ones(10);
    std::cout << vec << "\n";
    return 0;
}
```

Output

```
[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return matrix<int> of shape (3, 4) filled with ones.
    np::matrix<int> mat = np::ones(3, 4);
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[[1, 1, 1, 1],
 [1, 1, 1, 1],
 [1, 1, 1, 1]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return tensor<int, 3> of shape (2, 3, 4) filled with ones.
    np::tensor<int, 3> cube = np::ones(2, 3, 4);
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]],

 [[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]]]
```

### `ones_like`

Return a tensor of ones with the same shape and type as a given tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> ones_like(const tensor<T, Rank> &like);
```

Parameters

* `like` Tensor-like object defining the shape and data type.

Returns

* A tensor of ones with the same shape and type as `like`. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::vector<int> like{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << np::ones_like(like) << "\n";
    return 0;
}
```

Output

```
[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like{{ 0,  1,  2,  3},
                         { 4,  5,  6,  7},
                         { 8,  9, 10, 11}};
    std::cout << np::ones_like(like) << "\n";
    return 0;
}
```

Output

```
[[1, 1, 1, 1],
 [1, 1, 1, 1],
 [1, 1, 1, 1]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (2, 3, 4).
    np::tensor<int, 3> like{{{ 0,  1,  2,  3},
                             { 4,  5,  6,  7},
                             { 8,  9, 10, 11}},
                            {{12, 13, 14, 15},
                             {16, 17, 18, 19},
                             {20, 21, 22, 23}}};
    std::cout << np::ones_like(like) << "\n";
    return 0;
}
```

Output

```
[[[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]],

 [[1, 1, 1, 1],
  [1, 1, 1, 1],
  [1, 1, 1, 1]]]
```

### `full`

Return a tensor of given shape filled with `val`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> full(const shape_t<Rank> &shape, const T &val);
```

Parameters

* `shape` Shape of the tensor.
* `val` Fill value.

Returns

* A tensor of `val` with the given shape. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return vector<int> of shape (10,) filled with 10.
    np::vector<int> vec = np::full(np::make_shape(10), 10);
    std::cout << vec << "\n";
    return 0;
}
```

Output

```
[10, 10, 10, 10, 10, 10, 10, 10, 10, 10]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return matrix<int> of shape (3, 4) filled with 10.
    np::matrix<int> mat = np::full(np::make_shape(3, 4), 10);
    std::cout << mat << "\n";
    return 0;
}
```

Output

```
[[10, 10, 10, 10],
 [10, 10, 10, 10],
 [10, 10, 10, 10]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Return tensor<int, 3> of shape (2, 3, 4) filled with 10.
    np::tensor<int, 3> cube = np::full(np::make_shape(2, 3, 4), 10);
    std::cout << cube << "\n";
    return 0;
}
```

Output

```
[[[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]],

 [[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]]]
```

### `full_like`

Return a tensor filled with `val` with the same shape and type as a given tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> full_like(const tensor<T, Rank> &like,
                          const typename tensor<T, Rank>::value_type &val);
```

Parameters

* `like` Tensor-like object defining the shape and data type.
* `val` Fill value.

Returns

* A tensor of `val` with the same shape and type as `like`. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (10,).
    np::vector<int> like{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << np::full_like(like, 10) << "\n";
    return 0;
}
```

Output

```
[10, 10, 10, 10, 10, 10, 10, 10, 10, 10]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (3, 4).
    np::matrix<int> like{{ 0,  1,  2,  3},
                         { 4,  5,  6,  7},
                         { 8,  9, 10, 11}};
    std::cout << np::full_like(like, 10) << "\n";
    return 0;
}
```

Output

```
[[10, 10, 10, 10],
 [10, 10, 10, 10],
 [10, 10, 10, 10]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Type deduced to int. Shape deduced to (2, 3, 4).
    np::tensor<int, 3> like{{{ 0,  1,  2,  3},
                             { 4,  5,  6,  7},
                             { 8,  9, 10, 11}},
                            {{12, 13, 14, 15},
                             {16, 17, 18, 19},
                             {20, 21, 22, 23}}};
    std::cout << np::full_like(like, 10) << "\n";
    return 0;
}
```

Output

```
[[[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]],

 [[10, 10, 10, 10],
  [10, 10, 10, 10],
  [10, 10, 10, 10]]]
```
