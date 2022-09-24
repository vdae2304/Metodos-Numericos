# Linear algebra

Defined in `numcpp/routines.h`

- [Linear algebra](#linear-algebra)
  - [Basic linear algebra](#basic-linear-algebra)
    - [`dot`](#dot)
    - [`vdot`](#vdot)
    - [`cross`](#cross)
    - [`matmul`](#matmul)

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
    std::cout << "vdot(a, b): " << np::vdot(a, b) << "\n";
    std::cout << "dot(conj(a), b): " << np::dot(np::conj(a), b) << "\n";
    std::cout << "vdot(b, a): " << np::vdot(b, a) << "\n";
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
vdot(a, b): (112,43)
dot(conj(a), b): (112,43)
vdot(b, a): (112,-43)
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
    // For higher dimensions, it can be seen as a stack of vectors.
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

Output

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
    // a and b can be interpreted as arrays of matrices. Therefore, the output
    // is the "element-wise" matrix multiplication, with the matrices residing
    // in the last two axes.
    std::cout << "matmul(a, b):\n" << np::matmul(a, b) << "\n";
    // Print the first block to compare results.
    std::cout << "Block 0:\n";
    std::cout << np::matmul(a(0, np::slice(), np::slice()),
                            b(0, np::slice(), np::slice())) << "\n";
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
matmul(a, b):
[[[-43, -45],
  [153, 149],
  [ 75,  13]],

 [[ -4, 110],
  [-35,  39],
  [ 24,  11]],

 [[ 10,  23],
  [  5,  11],
  [-57, -38]]]
Block 0:
[[-43, -45],
 [153, 149],
 [ 75,  13]]
```
