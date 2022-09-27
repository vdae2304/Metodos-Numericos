# Linear algebra

Defined in `numcpp/routines.h`

- [Linear algebra](#linear-algebra)
  - [Basic linear algebra](#basic-linear-algebra)
    - [`dot`](#dot)
    - [`vdot`](#vdot)
    - [`cross`](#cross)
    - [`matmul`](#matmul)
    - [`tensordot`](#tensordot)
    - [`transpose`](#transpose)
    - [`conj_transpose`](#conj_transpose)
    - [`norm`](#norm)
    - [`norm(axis)`](#normaxis)
    - [`trace`](#trace)

## Basic linear algebra

### `dot`

Return the dot product of two vectors.
```cpp
template <class T>
T dot(const tensor<T, 1> &a, const tensor<T, 1> &b);
```

The dot product of two vectors $a = [a_1, a_2, \ldots, a_n]$ and
$b = [b_1, b_2, \ldots, b_n]$ is defined as

$$a \cdot b = a_1b_1 + a_2b_2 + \ldots + a_nb_n$$

Unlike Numpy's dot, `dot` only supports computing the dot product of two
1-dimensional tensors with the same number of elements.

Parameters

* `a` First tensor-like argument.
* `b` Second tensor-like argument.

Returns

* The dot product of the inputs.

Exceptions

* `std::invalid_argument` Thrown if the tensor arguments have different sizes.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a, b;
    std::cin >> a >> b;
    std::cout << np::dot(a, b) << "\n";
    return 0;
}
```

Input

```
[-4,  6, -1,  1,  6,  7,  4]
[ 3,  0, -5,  2,  6,  1, -2]
```

Output

```
30
```

### `vdot`

Return the dot product of two vectors.

The `vdot` function handles complex numbers differently than `dot`. For complex
numbers, the complex conjugate of the first argument is used for the
calcualtion of the dot product.
```cpp
template <class T>
T vdot(const tensor<T, 1> &a, const tensor<T, 1> &b);

template <class T>
std::complex<T> vdot(
    const tensor<std::complex<T>, 1> &a, const tensor<std::complex<T>, 1> &b
);
```

Parameters

* `a` First tensor-like argument.
* `b` Second tensor-like argument.

Returns

* The dot product of the inputs.

Exceptions

* `std::invalid_argument` Thrown if the tensor arguments have different sizes.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<std::complex<double>> a, b;
    std::cin >> a >> b;
    std::cout << "vdot(a, b):      " << np::vdot(a, b) << "\n";
    std::cout << "dot(conj(a), b): " << np::dot(np::conj(a), b) << "\n";
    std::cout << "vdot(b, a):      " << np::vdot(b, a) << "\n";
    std::cout << "dot(a, conj(b)): " << np::dot(a, np::conj(b)) << "\n";
    return 0;
}
```

Input

```
[(-4,7), (6,0), (-1,6), (1,7), (6,3), (7,9),  (0,4)]
[ (3,4), (0,9), (-5,1), (2,3), (4,0), (1,3), (-2,1)]
```

Output

```
vdot(a, b):      (112,43)
dot(conj(a), b): (112,43)
vdot(b, a):      (112,-43)
dot(a, conj(b)): (112,-43)
```

### `cross`

Return the cross product of two vectors.

The cross product of two vectors $a$ and $b$ (in $\mathbb{R}^3$) is a vector
perpendicular to both $a$ and $b$.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cross(
    const tensor<T, Rank> &a, const tensor<T, Rank> &b,
    size_t axis = Rank - 1
);
```

Parameters

* `a` First tensor-like argument.
* `b` Second tensor-like argument.
* `axis` Axis along which to take cross product. Default is `Rank - 1`. Input
tensors need to be broadcastable to some shape along the other axes.

Returns

* The cross product of the inputs.

Exceptions

* `std::invalid_argument` Thrown if the size of the vectors is not 3 along the
given axis.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a, b;
    std::cin >> a >> b;
    std::cout << np::cross(a, b) << "\n";
    return 0;
}
```

Input

```
[2, 7, 3]
[1, 8, 7]
```

Output

```
[ 25, -11,   9]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a, b;
    std::cin >> a >> b;
    // For higher dimensions, a and b can be interpreted as stacks of vectors.
    std::cout << np::cross(a, b) << "\n";
    return 0;
}
```

Input

```
[[2,  7, 3],
 [2,  1, 8],
 [5,  9, 0],
 [1, -2, 6]]

[[ 1, 8,  7],
 [ 0, 4,  2],
 [-1, 9,  1],
 [-5, 7, -3]]
```

Output

```
[[ 25, -11,   9],
 [-30,  -4,   8],
 [  9,  -5,  54],
 [-36, -27,  -3]]
```

### `matmul`

Return the matrix multiplication of two matrices.
```cpp
template <class T>
T matmul(const tensor<T, 1> &a, const tensor<T, 1> &b);

template <class T>
tensor<T, 2> matmul(const tensor<T, 2> &a, const tensor<T, 2> &b);

template <class T>
tensor<T, 1> matmul(const tensor<T, 1> &a, const tensor<T, 2> &b);

template <class T>
tensor<T, 1> matmul(const tensor<T, 2> &a, const tensor<T, 1> &b);

template <class T>
tensor<T, Rank> matmul(const tensor<T, Rank> &a, const tensor<T, Rank> &b);

template <class T>
tensor<T, Rank> matmul(const tensor<T, Rank> &a, const tensor<T, 2> &b);

template <class T, size_t Rank>
tensor<T, Rank> matmul(const tensor<T, 2> &a, const tensor<T, Rank> &b);
```

The behaviour depends on the arguments in the following way

* If both arguments are 1-dimensional, it is the dot product of vectors
(without complex conjugation).
* If both arguments are 2-dimensional, it is the conventional matrix
multiplication.
* If the first argument is 1-dimensional and the second argument is
2-dimensional, it is the matrix multiplication with the first argument treated
as a row-vector.
* If the first argument is 2-dimensional and the second argument is
1-dimensional, it is the matrix multiplication with the second argument treated
as a column-vector.
* If both arguments are $n$-dimensional, $n > 2$, it is treated as a stack of
matrices residing in the last 2 dimensions and broadcast accordingly.

The matrix multiplication of a $m \times p$ matrix $A = (a_{ij})$ and a
$p \times n$ matrix $B = (b_{ij})$ is the $m \times n$ matrix $C = (c_{ij})$
such that

$$c_{ij} = a_{i1}b_{1j} + a_{i2}b_{2j} + \ldots + a_{ip}b_{pj}$$

Parameters

* `a` First tensor-like argument.
* `b` Second tensor-like argument.

Returns

* The matrix multiplication of the inputs.

Exceptions

* `std::invalid_argument` Thrown if the last dimension of `a` is not the same
as the second-to-last dimension of `b`.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a, b;
    std::cin >> a >> b;
    // Prints the dot product of a and b.
    std::cout << np::matmul(a, b) << "\n";
    return 0;
}
```

Input

```
[-4,  6, -1,  1,  6,  7,  4]
[ 3,  0, -5,  2,  6,  1, -2]
```

Output

```
30
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a, b;
    std::cin >> a >> b;
    // Prints the matrix multiplication of a and b.
    // Do not confuse with a * b; a * b is the element-wise product while
    // matmul(a, b) is the matrix multiplication.
    std::cout << np::matmul(a, b) << "\n";
    return 0;
}
```

Input

```
[[ 4, -2, 6, -5],
 [10,  5, 9,  0],
 [ 6,  8, 0,  7],
 [ 1,  7, 4,  8]]

[[-2, -4, 3],
 [ 4,  6, 0],
 [-5,  8, 1],
 [-1,  2, 7]]
```

Output

```
[[-41,  10, -17],
 [-45,  62,  39],
 [ 13,  38,  67],
 [ -2,  86,  63]]
```

Input

```
[[ 4, -2, 6],
 [10,  5, 9],
 [ 6,  8, 0],
 [ 1,  7, 4]]

[[-2, -4, 3],
 [ 4,  6, 0],
 [-5,  8, 1],
 [-1,  2, 7]]
```

Possible output

```
terminate called after throwing an instance of 'std::invalid_argument'
  what():  shapes (4, 3) and (4, 3) not aligned: 3 (dim 1) != 4 (dim 0)
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    np::array<int> b;
    std::cin >> a >> b;
    // Prints the matrix multiplication of a and b. b is treated as a column
    // vector.
    std::cout << np::matmul(a, b) << "\n";
    return 0;
}
```

Input

```
[[ 4, -2, 6, -5],
 [10,  5, 9,  0],
 [ 6,  8, 0,  7],
 [ 1,  7, 4,  8]]

[-3, 0, 10, -5]
```

Output

```
[ 73,  60, -53,  -3]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    np::matrix<int> b;
    std::cin >> a >> b;
    // Prints the matrix multiplication of a and b. a is treated as a row
    // vector.
    std::cout << np::matmul(a, b) << "\n";
    return 0;
}
```

Input

```
[-3, 9, 7, 5]

[[-2, -4, 3],
 [ 4,  6, 0],
 [-5,  8, 1],
 [-1,  2, 7]]
```

Output

```
[  2, 132,  33]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a, b;
    std::cin >> a >> b;
    // a and b can be interpreted as stacks of matrices. Each block along axis
    // 0 is the matrix multiplication of the corresponding blocks of a and b.
    np::tensor<int, 3> c = np::matmul(a, b);
    std::cout << c << "\n";
    // Without broadcasting, matmul is equivalent to:
    np::tensor<int, 3> d(a.shape(0), a.shape(1), b.shape(2));
    for (unsigned i = 0; i < a.shape(0); ++i) {
        np::slice _;
        d(i, _, _) = np::matmul(a(i, _, _), b(i, _, _));
    }
    std::cout << std::boolalpha << np::all(c == d) << "\n";
    return 0;
}
```

Input

```
[[[-5, -2,  1],
  [ 9,  8,  9],
  [11, -2, -5]],
 [[10,  6, 12],
  [ 6, -3,  7],
  [-3,  5,  2]],
 [[ 4,  6,  5],
  [ 8,  7,  5],
  [10,  3, 12]]]

[[[10,  4],
  [ 0, 13],
  [ 7,  1]],
 [[-1,  5],
  [ 5,  4],
  [-2,  3]],
 [[-3, -5],
  [ 7,  8],
  [-4, -1]]]
```

Output

```
[[[-43, -45],
  [153, 149],
  [ 75,  13]],

 [[ -4, 110],
  [-35,  39],
  [ 24,  11]],

 [[ 10,  23],
  [  5,  11],
  [-57, -38]]]
true
```

### `tensordot`

Return a contraction of two tensors over multiple axes.

Given two tensors `a` and `b`, and two tuples of axes `a_axes` and `b_axes`,
sum the products of `a`'s and `b`'s elements over the axes specified by
`a_axes` and `b_axes`.
```cpp
template <class T, size_t Rank>
T tensordot(
    const tensor<T, Rank> &a, const tensor<T, Rank> &b,
    const shape_t<Rank> &a_axes, const shape_t<Rank> &b_axes
);

template <class T, size_t Rank1, size_t Rank2, size_t N>
tensor<T, (Rank1 - N) + (Rank2 - N)> tensordot(
    const tensor<T, Rank1> &a, const tensor<T, Rank2> &b,
    const shape_t<N> &a_axes, const shape_t<N> &b_axes
);
```

Parameters

* `a` First tensor-like argument.
* `b` Second tensor-like argument.
* `a_axes` List of axes to sum over `a`.
* `b_axes` List of axes to sum over `b`.

Returns

* The contraction of the inputs. The shape of the result consists of the
non-contracted axes of the first tensor, followed by the non-contracted axes of
the second.

Exceptions

* `std::invalid_argument` Thrown if the shape of `a` is not equal to the shape
of `b` over the contracted axes.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a, b;
    std::cin >> a >> b;
    // Align axes (1, 0) of a with axes (0, 1) of b, in that order.
    // The shape of the result consists of axis 2 of a followed by axis 2 of b.
    np::shape_t<2> a_axes(1, 0), b_axes(0, 1);
    np::matrix<int> c = np::tensordot(a, b, a_axes, b_axes);
    std::cout << c << "\n";
    // For this example, tensordot is equivalent to:
    np::matrix<int> d(a.shape(2), b.shape(2));
    for (unsigned i = 0; i < a.shape(2); ++i) {
        for (unsigned j = 0; j < b.shape(2); ++j) {
            d(i, j) = 0;
            for (unsigned k = 0; k < a.shape(0); ++k) {
                for (unsigned l = 0; l < a.shape(1); ++l) {
                    d(i, j) += a(k, l, i) * b(l, k, j);
                }
            }
        }
    }
    std::cout << std::boolalpha << np::all(c == d) << "\n";
    return 0;
}
```

Input

```
[[[ 0,  1,  2,  3,  4],
  [ 5,  6,  7,  8,  9],
  [10, 11, 12, 13, 14],
  [15, 16, 17, 18, 19]],
 [[20, 21, 22, 23, 24],
  [25, 26, 27, 28, 29],
  [30, 31, 32, 33, 34],
  [35, 36, 37, 38, 39]],
 [[40, 41, 42, 43, 44],
  [45, 46, 47, 48, 49],
  [50, 51, 52, 53, 54],
  [55, 56, 57, 58, 59]]]

[[[ 0,  1],
  [ 2,  3],
  [ 4,  5]],
 [[ 6,  7],
  [ 8,  9],
  [10, 11]],
 [[12, 13],
  [14, 15],
  [16, 17]],
 [[18, 19],
  [20, 21],
  [22, 23]]]
```

Output

```
[[4400, 4730],
 [4532, 4874],
 [4664, 5018],
 [4796, 5162],
 [4928, 5306]]
true
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a, b;
    std::cin >> a >> b;
    // For 2-dimensional tensors, matrix multiplication can be seen as a
    // particular case of tensordot with a_axes=(1,) and b_axes=(0,)
    np::shape_t<1> a_axes(1), b_axes(0);
    std::cout << np::tensordot(a, b, a_axes, b_axes) << "\n";
    return 0;
}
```

Input

```
[[ 4, -2, 6, -5],
 [10,  5, 9,  0],
 [ 6,  8, 0,  7],
 [ 1,  7, 4,  8]]

[[-2, -4, 3],
 [ 4,  6, 0],
 [-5,  8, 1],
 [-1,  2, 7]]
```

Output

```
[[-41,  10, -17],
 [-45,  62,  39],
 [ 13,  38,  67],
 [ -2,  86,  63]]
```

### `transpose`

Reverse or permute the axes of a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> transpose(const tensor<T, Rank> &a);

template <class T, size_t Rank>
tensor<T, Rank> transpose(const tensor<T, Rank> &a, const shape_t<Rank> &axes);
```

Parameters

* `a` Input tensor.
* `axes` If specified, it must be a permutation of `(0, 1, ..., Rank - 1)`. The
`i`-th axis of the returned tensor will correspond to the axis numbered
`axes[i]` of the input. If not specified, reverses the order of the axes.

Returns

* A light-weight object with the axes of `a` permuted. This function does not
create a new tensor, instead, it returns a readonly view of `a` with its axes
reversed or permuted.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << np::transpose(a) << "\n";
    return 0;
}
```

Input

```
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
```

Output

```
[[ 1, -5, -1],
 [14, -3, 18],
 [12, 11, -3],
 [-3, 11, -1]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Reverse axes:\n" << np::transpose(a) << "\n";
    std::cout << "Swap last 2 axes:\n" << np::transpose(a, {0, 2, 1}) << "\n";
    return 0;
}
```

Input

```
[[[16, 15, 14, -1],
  [ 5, 14,  9, 10],
  [18, 15,  2,  5]],

 [[11,  6, 19, -2],
  [ 7, 10,  1, -2],
  [14,  7, -2, 11]]]
```

Output

```
Reverse axes:
[[[16, 11],
  [ 5,  7],
  [18, 14]],

 [[15,  6],
  [14, 10],
  [15,  7]],

 [[14, 19],
  [ 9,  1],
  [ 2, -2]],

 [[-1, -2],
  [10, -2],
  [ 5, 11]]]
Swap last 2 axes:
[[[16,  5, 18],
  [15, 14, 15],
  [14,  9,  2],
  [-1, 10,  5]],

 [[11,  7, 14],
  [ 6, 10,  7],
  [19,  1, -2],
  [-2, -2, 11]]]
```

### `conj_transpose`

Return the (complex) conjugate transpose of a tensor. For non-complex types,
`conj_transpose` is equivalent to `transpose`.
```cpp
template <class T, size_t Rank>
tensor<std::complex<T>, Rank> conj_transpose(
    const tensor<std::complex<T>, Rank> &a
);

template <class T, size_t Rank>
tensor<std::complex<T>, Rank> conj_transpose(
    const tensor<std::complex<T>, Rank> &a, const shape_t<Rank> &axes
);

template <class T, size_t Rank>
tensor<T, Rank> conj_transpose(const tensor<T, Rank> &a);

template <class T, size_t Rank>
tensor<T, Rank> conj_transpose(
    const tensor<T, Rank> &a, const shape_t<Rank> &axes
);
```

Parameters

* `a` Input tensor.
* `axes` If specified, it must be a permutation of `(0, 1, ..., Rank - 1)`. The
`i`-th axis of the returned tensor will correspond to the axis numbered
`axes[i]` of the input. If not specified, reverses the order of the axes.

Returns

* A light-weight object with the conjugate transpose of `a`. This function does
not create a new tensor, instead, it returns a readonly view of `a` with its
elements complex conjugated and its axes reversed or permuted.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::matrix<std::complex<double>> a;
    std::cin >> a;
    std::cout << np::conj_transpose(a) << "\n";
    return 0;
}
```

Input

```
[[-2+8i, 5+16i,  16+5i,  -1-2i],
 [ 6-5i, -1-4i, -4+19i,  3+19i],
 [ 4+6i, 2+14i,  17-1i, -1+15i]]
```

Output

```
[[ -2-8i,   6+5i,   4-6i],
 [ 5-16i,  -1+4i,  2-14i],
 [ 16-5i, -4-19i,  17+1i],
 [ -1+2i,  3-19i, -1-15i]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::tensor<std::complex<double>, 3> a;
    std::cin >> a;
    std::cout << "Reverse axes:\n";
    std::cout << np::conj_transpose(a) << "\n";
    std::cout << "Swap last 2 axes:\n";
    std::cout << np::conj_transpose(a, {0, 2, 1}) << "\n";
    return 0;
}
```

Input

```
[[[ 12+3i,  5+19i, -4+10i,  9+7i],
  [-1+11i,  10+2i,   5-2i, 11+4i],
  [ 17+4i,   4-3i,   2+5i, 11+5i]],

 [[-1+13i, -5+18i,   5+17i, 8+7i],
  [17+14i,  13+4i,    0+3i, 6+0i],
  [18+16i, -2+11i,   16+8i, 2+18i]]]
```

Output

```
Reverse axes:
[[[ 12-3i, -1-13i],
  [-1-11i, 17-14i],
  [ 17-4i, 18-16i]],

 [[ 5-19i, -5-18i],
  [ 10-2i,  13-4i],
  [  4+3i, -2-11i]],

 [[-4-10i,  5-17i],
  [  5+2i,   0-3i],
  [  2-5i,  16-8i]],

 [[  9-7i,   8-7i],
  [ 11-4i,   6-0i],
  [ 11-5i,  2-18i]]]
Swap last 2 axes:
[[[ 12-3i, -1-11i,  17-4i],
  [ 5-19i,  10-2i,   4+3i],
  [-4-10i,   5+2i,   2-5i],
  [  9-7i,  11-4i,  11-5i]],

 [[-1-13i, 17-14i, 18-16i],
  [-5-18i,  13-4i, -2-11i],
  [ 5-17i,   0-3i,  16-8i],
  [  8-7i,   6-0i,  2-18i]]]
```

### `norm`

Return the vector norm.
```cpp
template <class T>
T norm(const tensor<T, 1> &a, double ord = 2);

template <class T>
T norm(const tensor<std::complex<T>, 1> &a, double ord = 2);
```

This function is able to return one of the following norms:

| Order  | Definition               |
| ------ | ------------------------ |
| `0`    | `sum(x != 0)`            |
| `inf`  | `amax(abs(x))`           |
| `-inf` | `amin(abs(x))`           |
| `p`    | `sum(abs(a)**p)**(1./p)` |

Parameters

* `a` A 1-dimensional tensor-like object.
* `ord` Order of the norm. The default is 2 (Euclidean norm). For values of
`ord < 1`, the result is, strictly speaking, not a mathematical norm, but it may
still be useful for various numerical purposes.

Returns

* Norm of the vector.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double inf = np::constants<double>::inf;
    np::array<double> a;
    std::cin >> a;
    std::cout << "Euclidean norm: " << np::norm(a) << "\n";
    std::cout << "Taxicab norm: " << np::norm(a, 1) << "\n";
    std::cout << "Zero norm: " << np::norm(a, 0) << "\n";
    std::cout << "Maximum norm: " << np::norm(a, inf) << "\n";
    return 0;
}
```

Input

```
[-4,  6, -1,  1,  6,  7,  4]
```

Output

```
Euclidean norm: 12.4499
Taxicab norm: 29
Zero norm: 7
Maximum norm: 7
```

### `norm(axis)`

Return the vector norm along the given axis.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> norm(const tensor<T, Rank> &a, double ord, size_t axis);

template <class T, size_t Rank>
tensor<T, Rank> norm(
    const tensor<std::complex<T>, Rank> &a, double ord, size_t axis
);
```

Parameters

* `a` A tensor-like object.
* `ord` Order of the norm.
* `axis` Axis along which to compute the vector norm.

Returns

* A new tensor with the norm of the vectors along an axis. The output tensor
will have the same dimension and the same shape, except that the axis which is
reduced is left as a dimension of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    double inf = np::constants<double>::inf;
    np::matrix<double> a;
    int axis;
    std::cin >> a >> axis;
    std::cout << "Euclidean norm:\n" << np::norm(a, 2, axis) << "\n";
    std::cout << "Taxicab norm:\n" << np::norm(a, 1, axis) << "\n";
    std::cout << "Zero norm:\n" << np::norm(a, 0, axis) << "\n";
    std::cout << "Maximum norm:\n" << np::norm(a, inf, axis) << "\n";
    return 0;
}
```

Input

```
[[-4,  6, -1,  1,  6,  7,  4],
 [ 3,  0, -5,  2,  6,  1, -2],
 [ 6, -2, -3,  9,  3,  7,  9],
 [ 8,  1, -3,  7,  2,  1,  0]]
1
```

Output

```
Euclidean norm:
[[  12.4499],
 [8.8881944],
 [16.401219],
 [11.313708]]
Taxicab norm:
[[29],
 [19],
 [39],
 [22]]
Zero norm:
[[7],
 [6],
 [7],
 [6]]
Maximum norm:
[[7],
 [6],
 [9],
 [8]]
```

### `trace`

Return the sum along a diagonal of the matrix, i.e., the sum of the elements of
the form `a(i, i + k)`.
```cpp
template <class T>
T trace(const tensor<T, 2> &a, ptrdiff_t k = 0);
```

Parameters

* `a` Matrix from which the diagonal is taken.
* `k` Offset of the diagonal from the main diagonal. A positive value refers to
an upper diagonal and a negative value refers to a lower diagonal. Defaults to
main diagonal (0).

Returns

* The sum along the diagonal.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "trace(a): " << np::trace(a) << "\n";
    std::cout << "trace(a, k=1): " << np::trace(a, 1) << "\n";
    std::cout << "trace(a, k=-1): " << np::trace(a, -1) << "\n";
    return 0;
}
```

Input

```
[[ 1,  2,  3,  4],
 [ 5,  6,  7,  8],
 [ 9, 10, 11, 12],
 [13, 14, 15, 16]]
```

Output

```
trace(a): 34
trace(a, k=1): 21
trace(a, k=-1): 30
```
