# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Building matrices](#building-matrices)
    - [`eye`](#eye)
    - [`diag`](#diag)
    - [`tril`](#tril)
    - [`triu`](#triu)
    - [`vander`](#vander)

## Building matrices

### `eye`

Return a matrix with ones on the diagonal and zeros elsewhere.
```cpp
template <class T = double>
tensor<T, 2> eye(size_t n);

template <class T = double>
tensor<T, 2> eye(size_t m, size_t n, ptrdiff_t k = 0);
```

Template parameters

* `T` Type of the elements contained in the tensor. Default is `double`.

Parameters

* `m` Number of rows. If not provided, defaults to `n`.
* `n` Number of columns.
* `k` Offset of the diagonal from the main diagonal. A positive value refers to an upper diagonal and a negative value refers to a lower diagonal. Defaults to main diagonal (0).

Returns

* A light-weight object with ones on the diagonal and zeros elsewhere. This function, does not create a new tensor, instead, an expression object is returned with ones on the diagonal and zeros elsewhere.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    int m, n;
    std::cin >> m >> n;
    std::cout << "Main diagonal:\n" << np::eye(m, n) << "\n";
    std::cout << "Sub diagonal:\n" << np::eye(m, n, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::eye(m, n, 1) << "\n";
    return 0;
}
```

Input

```
4 4
```

Output

```
Main diagonal:
[[1, 0, 0, 0],
 [0, 1, 0, 0],
 [0, 0, 1, 0],
 [0, 0, 0, 1]]
Sub diagonal:
[[0, 0, 0, 0],
 [1, 0, 0, 0],
 [0, 1, 0, 0],
 [0, 0, 1, 0]]
Sup diagonal:
[[0, 1, 0, 0],
 [0, 0, 1, 0],
 [0, 0, 0, 1],
 [0, 0, 0, 0]]
```

### `diag`

Extract a diagonal or construct a diagonal matrix.
```cpp
template <class T, size_t Rank>
tensor<T, Rank - 1> diag(const tensor<T, Rank> &a, ptrdiff_t k = 0);

template <class T>
tensor<T, 2> diag(const tensor<T, 1> &a, ptrdiff_t k = 0);
```

If `a` is a 1-dimensional tensor, returns a matrix with `a` on the `k`-th diagonal and zeros elsewhere.

If `a` is a 2-dimensional tensor, returns the diagonal of `a` with the given offset, i.e., an array with the elements of the form `a(i, i+k)`.

If `a` has more than two dimensions, then the last two axes are used to determine the 2-dimensional sub-tensor whose diagonal is returned.

Parameters

* `a` An $n$-dimensional tensor from which the diagonals are taken or an 1-dimensional tensor with the elements that will be on the diagonal.
* `k` Offset of the diagonal from the main diagonal. A positive value refers to an upper diagonal and a negative value refers to a lower diagonal. Defaults to main diagonal (0).

Returns

* The extracted diagonal or the constructed diagonal matrix. This function does not create a new tensor, instead, an expression object is returned.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // For 1-dimensional tensors, return a matrix with a on the diagonal.
    np::vector<int> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << np::diag(a) << "\n";
    std::cout << "Sub diagonal:\n" << np::diag(a, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::diag(a, 1) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4]
```

Output

```
Main diagonal:
[[1, 0, 0, 0],
 [0, 2, 0, 0],
 [0, 0, 3, 0],
 [0, 0, 0, 4]]
Sub diagonal:
[[0, 0, 0, 0, 0],
 [1, 0, 0, 0, 0],
 [0, 2, 0, 0, 0],
 [0, 0, 3, 0, 0],
 [0, 0, 0, 4, 0]]
Sup diagonal:
[[0, 1, 0, 0, 0],
 [0, 0, 2, 0, 0],
 [0, 0, 0, 3, 0],
 [0, 0, 0, 0, 4],
 [0, 0, 0, 0, 0]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // For 2-dimensional tensors, return the diagonal of a.
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << np::diag(a) << "\n";
    std::cout << "Sub diagonal:\n" << np::diag(a, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::diag(a, 1) << "\n";
    return 0;
}
```

Input

```
[[ 3, 19,  6,  9, 14],
 [17,  7, 11, -2, -4],
 [ 5,  5, 18,  2,  2],
 [17, -2, -1, 11,  1],
 [12,  6,  2, 11, -3]]
```

Output

```
Main diagonal:
[ 3,  7, 18, 11, -3]
Sub diagonal:
[17,  5, -1, 11]
Sup diagonal:
[19, 11,  2,  1]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // For 3-dimensional tensors, return the diagonal of each matrix determined
    // by the last two axes of a.
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << np::diag(a) << "\n";
    std::cout << "Sub diagonal:\n" << np::diag(a, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::diag(a, 1) << "\n";
    return 0;
}
```

Input

```
[[[ 2,  5, -4, 15],
  [-1, -1, 15,  2],
  [ 6,  5, 18, -3],
  [ 6,  5, 18, 14]],

 [[18, 19, 10, -3],
  [ 1,  0, 15, 18],
  [10, 18,  4,  7],
  [10, 10,  9, -1]]]
```

Output

```
Main diagonal:
[[ 2, -1, 18, 14],
 [18,  0,  4, -1]]
Sub diagonal:
[[-1,  5, 18],
 [ 1, 18,  9]]
Sup diagonal:
[[ 5, 15, -3],
 [19, 15,  7]]
```

### `tril`

Return the lower triangle of a matrix. If the tensor has more than two dimensions, then the last two axes are used to determine the 2-dimensional sub-tensor whose lower triangle is returned.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> tril(const tensor<T, Rank> &a, ptrdiff_t k = 0);
```

Parameters

* `a` Matrix from which the lower triangle is taken.
* `k` Diagonal above which to zero elements. A positive value refers to an upper diagonal and a negative value refers to a lower diagonal. Defaults to main diagonal (0).

Returns

* Lower triangle of `a`. This function does not create a new tensor, instead, an expression object is returned with the elements above the `k`-th diagonal zeroed.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << np::tril(a) << "\n";
    std::cout << "Sub diagonal:\n" << np::tril(a, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::tril(a, 1) << "\n";
    return 0;
}
```

Input

```
[[ 3, 19,  6,  9, 14],
 [17,  7, 11, -2, -4],
 [ 5,  5, 18,  2,  2],
 [17, -2, -1, 11,  1],
 [12,  6,  2, 11, -3]]
```

Output

```
Main diagonal:
[[ 3,  0,  0,  0,  0],
 [17,  7,  0,  0,  0],
 [ 5,  5, 18,  0,  0],
 [17, -2, -1, 11,  0],
 [12,  6,  2, 11, -3]]
Sub diagonal:
[[ 0,  0,  0,  0,  0],
 [17,  0,  0,  0,  0],
 [ 5,  5,  0,  0,  0],
 [17, -2, -1,  0,  0],
 [12,  6,  2, 11,  0]]
Sup diagonal:
[[ 3, 19,  0,  0,  0],
 [17,  7, 11,  0,  0],
 [ 5,  5, 18,  2,  0],
 [17, -2, -1, 11,  1],
 [12,  6,  2, 11, -3]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << np::tril(a) << "\n";
    std::cout << "Sub diagonal:\n" << np::tril(a, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::tril(a, 1) << "\n";
    return 0;
}
```

Input

```
[[[ 2,  5, -4, 15],
  [-1, -1, 15,  2],
  [ 6,  5, 18, -3],
  [ 6,  5, 18, 14]],

 [[18, 19, 10, -3],
  [ 1,  0, 15, 18],
  [10, 18,  4,  7],
  [10, 10,  9, -1]]]
```

Output

```
Main diagonal:
[[[ 2,  0,  0,  0],
  [-1, -1,  0,  0],
  [ 6,  5, 18,  0],
  [ 6,  5, 18, 14]],

 [[18,  0,  0,  0],
  [ 1,  0,  0,  0],
  [10, 18,  4,  0],
  [10, 10,  9, -1]]]
Sub diagonal:
[[[ 0,  0,  0,  0],
  [-1,  0,  0,  0],
  [ 6,  5,  0,  0],
  [ 6,  5, 18,  0]],

 [[ 0,  0,  0,  0],
  [ 1,  0,  0,  0],
  [10, 18,  0,  0],
  [10, 10,  9,  0]]]
Sup diagonal:
[[[ 2,  5,  0,  0],
  [-1, -1, 15,  0],
  [ 6,  5, 18, -3],
  [ 6,  5, 18, 14]],

 [[18, 19,  0,  0],
  [ 1,  0, 15,  0],
  [10, 18,  4,  7],
  [10, 10,  9, -1]]]
```

### `triu`

Return the upper triangle of a matrix. If the tensor has more than two dimensions, then the last two axes are used to determine the 2-dimensional sub-tensor whose upper triangle is returned.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> triu(const tensor<T, Rank> &a, ptrdiff_t k = 0);
```

Parameters

* `a` Matrix from which the upper triangle is taken.
* `k` Diagonal below which to zero elements. A positive value refers to an upper diagonal and a negative value refers to a lower diagonal. Defaults to main diagonal (0).

Returns

* Upper triangle of `a`. This function does not create a new tensor, instead, an expression object is returned with the elements below the `k`-th diagonal zeroed.


Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << np::triu(a) << "\n";
    std::cout << "Sub diagonal:\n" << np::triu(a, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::triu(a, 1) << "\n";
    return 0;
}
```

Input

```
[[ 3, 19,  6,  9, 14],
 [17,  7, 11, -2, -4],
 [ 5,  5, 18,  2,  2],
 [17, -2, -1, 11,  1],
 [12,  6,  2, 11, -3]]
```

Output

```
Main diagonal:
[[ 3, 19,  6,  9, 14],
 [ 0,  7, 11, -2, -4],
 [ 0,  0, 18,  2,  2],
 [ 0,  0,  0, 11,  1],
 [ 0,  0,  0,  0, -3]]
Sub diagonal:
[[ 3, 19,  6,  9, 14],
 [17,  7, 11, -2, -4],
 [ 0,  5, 18,  2,  2],
 [ 0,  0, -1, 11,  1],
 [ 0,  0,  0, 11, -3]]
Sup diagonal:
[[ 0, 19,  6,  9, 14],
 [ 0,  0, 11, -2, -4],
 [ 0,  0,  0,  2,  2],
 [ 0,  0,  0,  0,  1],
 [ 0,  0,  0,  0,  0]]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << np::triu(a) << "\n";
    std::cout << "Sub diagonal:\n" << np::triu(a, -1) << "\n";
    std::cout << "Sup diagonal:\n" << np::triu(a, 1) << "\n";
    return 0;
}
```

Input

```
[[[ 2,  5, -4, 15],
  [-1, -1, 15,  2],
  [ 6,  5, 18, -3],
  [ 6,  5, 18, 14]],

 [[18, 19, 10, -3],
  [ 1,  0, 15, 18],
  [10, 18,  4,  7],
  [10, 10,  9, -1]]]
```

Output

```
Main diagonal:
[[[ 2,  5, -4, 15],
  [ 0, -1, 15,  2],
  [ 0,  0, 18, -3],
  [ 0,  0,  0, 14]],

 [[18, 19, 10, -3],
  [ 0,  0, 15, 18],
  [ 0,  0,  4,  7],
  [ 0,  0,  0, -1]]]
Sub diagonal:
[[[ 2,  5, -4, 15],
  [-1, -1, 15,  2],
  [ 0,  5, 18, -3],
  [ 0,  0, 18, 14]],

 [[18, 19, 10, -3],
  [ 1,  0, 15, 18],
  [ 0, 18,  4,  7],
  [ 0,  0,  9, -1]]]
Sup diagonal:
[[[ 0,  5, -4, 15],
  [ 0,  0, 15,  2],
  [ 0,  0,  0, -3],
  [ 0,  0,  0,  0]],

 [[ 0, 19, 10, -3],
  [ 0,  0, 15, 18],
  [ 0,  0,  0,  7],
  [ 0,  0,  0,  0]]]
```

### `vander`

Generate a Vandermonde matrix.

The columns of the output matrix are powers of the input vector. The order of the powers is determined by the `increasing` boolean argument.
```cpp
template <class T>
tensor<T, 2> vander(const tensor<T, 1> &x);

template <class T>
tensor<T, 2> vander(const tensor<T, 1> &x, size_t N, bool increasing = false);
```

Parameters

* `x` 1-dimensional input tensor.
* `N` Number of columns in the output. If `N` is not specified, a square matrix is returned (`N = x.size()`).
* `increasing` Order of the powers of the columns. If true, the powers increase from left to right. If false, they are reversed. Default is false.

Returns

* Vandermonde matrix.

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
    np::vector<int> x;
    std::cin >> x;
    std::cout << "vander(x):\n";
    std::cout << np::vander(x) << "\n";
    std::cout << "vander(x, N=3):\n";
    std::cout << np::vander(x, 3) << "\n";
    std::cout << "vander(x, N=3, increasing=true):\n";
    std::cout << np::vander(x, 3, true) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
```

Output

```
vander(x):
[[  1,   1,   1,   1,   1],
 [ 16,   8,   4,   2,   1],
 [ 81,  27,   9,   3,   1],
 [256,  64,  16,   4,   1],
 [625, 125,  25,   5,   1]]
vander(x, N=3):
[[ 1,  1,  1],
 [ 4,  2,  1],
 [ 9,  3,  1],
 [16,  4,  1],
 [25,  5,  1]]
vander(x, N=3, increasing=true):
[[ 1,  1,  1],
 [ 1,  2,  4],
 [ 1,  3,  9],
 [ 1,  4, 16],
 [ 1,  5, 25]]
```
