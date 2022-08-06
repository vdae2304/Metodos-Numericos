# Tensor operators

Defined in `numcpp/tensor.h`

- [Tensor operators](#tensor-operators)
  - [Template parameters](#template-parameters)
  - [Compound assignment](#compound-assignment)
  - [Operators (non-member functions)](#operators-non-member-functions)
    - [Unary operators](#unary-operators)
    - [Arithmetic operators](#arithmetic-operators)
    - [Bitwise operators](#bitwise-operators)
    - [Logical operators](#logical-operators)
    - [Relational operators](#relational-operators)
  - [Matrix multiplication](#matrix-multiplication)
    - [`matmul`](#matmul)

## Template parameters

* `T` Type of the elements contained in the tensor. This shall be an arithmetic
type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the tensor. It must be a positive integer.

## Compound assignment

Assigns to `*this` the result of performing the respective operation on all the
elements in the tensor.

When the right-hand side argument is a tensor object, the operation is
performed between the corresponding elements in each object (the first element
of the left one with the right one, the second with the second, and so on...)

When the right-hand side argument is a value, the operation is applied to all
the elements in the tensor against that value.
```cpp
template <class U>
tensor& operator+=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator-=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator*=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator/=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator%=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator&=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator|=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator^=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator<<=(const tensor<U, Rank> &rhs);
template <class U>
tensor& operator>>=(const tensor<U, Rank> &rhs);

tensor& operator+=(const T &val);
tensor& operator-=(const T &val);
tensor& operator*=(const T &val);
tensor& operator/=(const T &val);
tensor& operator%=(const T &val);
tensor& operator&=(const T &val);
tensor& operator|=(const T &val);
tensor& operator^=(const T &val);
tensor& operator<<=(const T &val);
tensor& operator>>=(const T &val);
```

Parameters

* `rhs` Right-hand side tensor-like object.
* `val` Value to use as right-hand operand.

Returns

* `*this`

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules (see `numcpp::broadcast_shapes`).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a{11, 8, 16, 17, -2, 16, 18, -5, -4, 15};
    np::array<int> b{ 0, 1, 14,  0,  8,  3,  2,  2, 18,  0};
    std::cout << "a:\n" << a << "\nb:\n" << b << "\n";
    // Sum a and b element-wise and assign the result to a.
    a += b;
    std::cout << "a + b:\n" << a << "\n";
    return 0;
}
```

Output

```
a:
[11,  8, 16, 17, -2, 16, 18, -5, -4, 15]
b:
[ 0,  1, 14,  0,  8,  3,  2,  2, 18,  0]
a + b:
[11,  9, 30, 17,  6, 19, 20, -3, 14, 15]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a{11, 8, 16, 17, -2, 16, 18, -5, -4, 15};
    std::cout << "a:\n" << a << "\n";
    // Multiply every element of a by 2.
    a *= 2;
    std::cout << "2*a:\n" << a << "\n";
    return 0;
}
```

Output

```
a:
[11,  8, 16, 17, -2, 16, 18, -5, -4, 15]
2*a:
[ 22,  16,  32,  34,  -4,  32,  36, -10,  -8,  30]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a{{ 7, 12, 18,  8,  4, 15},
                      {15,  8, -1, -2, -2,  3},
                      {12, 19, 15,  5, -3,  2},
                      { 3, -5, 10, 13,  7, 14}};
    np::array<int> b{1, 3, 4, 0};
    std::cout << "a:\n" << a << "\nb:\n" << b << "\n";

    // Compilation error: a is 2-dimensional and b is 1-dimensional.
    // a -= b;

    // OK. a's shape is (4, 6) and c's shape is (4, 1).
    np::matrix_view<int> c = b.reshape(4, 1);

    // c is matched to (4, 6) by creating copies of c for each column.
    // No copies are created actually. Broadcasting handles this efficiently.
    a -= c;

    std::cout << "a - b:\n" << a << "\n";
    return 0;
}
```

Output

```
a:
[[ 7, 12, 18,  8,  4, 15],
 [15,  8, -1, -2, -2,  3],
 [12, 19, 15,  5, -3,  2],
 [ 3, -5, 10, 13,  7, 14]]
b:
[1, 3, 4, 0]
a - b:
[[ 6, 11, 17,  7,  3, 14],
 [12,  5, -4, -5, -5,  0],
 [ 8, 15, 11,  1, -7, -2],
 [ 3, -5, 10, 13,  7, 14]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a{{ 7, 12, 18,  8,  4, 15},
                      {15,  8, -1, -2, -2,  3},
                      {12, 19, 15,  5, -3,  2},
                      { 3, -5, 10, 13,  7, 14}};
    np::matrix<int> b{{15, 16},
                      { 3, -1},
                      { 6,  9},
                      {16,  9}};
    // Throws an exception, a and b cannot be broadcasted into a common shape.
    // Two dimensions are compatible if they are equal or one of them is 1.
    a += b;
    return 0;
}
```

Output

```
terminate called after throwing an instance of 'std::invalid_argument'
  what():  operands could not be broadcast together with shapes (4, 6) (4, 2)
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a{{{17}, { 6}, {16}, {18}},
                         {{18}, {19}, {13}, {10}},
                         {{ 1}, { 3}, {-1}, {-1}}};
    np::tensor<int, 3> b{{{13, 13, 13, -1, 17, 16},
                          {11, 11,  0,  0,  9, 18},
                          { 3, 18, 15,  3, 10, 3},
                          { 8,  5, 17, -1,  0, 16}}};
    std::cout << a.shape() << "\n" << b.shape() << "\n";
    std::cout << np::broadcast_shapes(a.shape(), b.shape()) << "\n";
    // Throws an exception.
    // a's shape is (3, 4, 1) and b's shape is (1, 4, 6).
    // The result's shape is (3, 4, 6), which can not be assigned to a.
    a += b;
    return 0;
}
```

Output

```
(3, 4, 1)
(1, 4, 6)
(3, 4, 6)
terminate called after throwing an instance of 'std::invalid_argument'
  what():  non-broadcastable output operand with shape (3, 4, 1) doesn't match
           the broadcast shape (3, 4, 6)
```

## Operators (non-member functions)

Each of this functions performs their respective operation on all the elements
in the tensor.

When both the left-hand side and right-hand side arguments are tensor objects,
the operation is performed between the corresponding elements in each object
(the first element of the left one with the right one, the second with the
second, and so on...)

When one of the arguments is a value, the operation is applied to all the
elements in the tensor against that value.

Parameters

* `lhs` Left-hand side tensor-like object.
* `rhs` Right-hand side tensor-like object.
* `val` Value to use either as left-hand or right-hand operand.

Returns

* A light-weight object which stores the result of performing the operation on
each element. None of these operators creates a new tensor object, instead, an
expression object is returned. The returned object uses lazy-evaluation, which
means that the result of each operation is computed only when required, i.e.,
when the whole expression is evaluated or assigned to a tensor object.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules (see `numcpp::broadcast_shapes`).

### Unary operators

```cpp
template <class T, size_t Rank>
tensor<T, Rank> operator+(const tensor<T, Rank> &arg);

template <class T, size_t Rank>
tensor<T, Rank> operator-(const tensor<T, Rank> &arg);

template <class T, size_t Rank>
tensor<T, Rank> operator~(const tensor<T, Rank> &arg);

template <class T, size_t Rank>
tensor<bool, Rank> operator!(const tensor<T, Rank> &arg);
```

### Arithmetic operators

```cpp
template <class T, size_t Rank>
tensor<T, Rank> operator+(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator+(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator+(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator-(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator-(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator-(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator*(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator*(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator*(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator/(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator/(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator/(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator%(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator%(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator%(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);
```

### Bitwise operators

```cpp
template <class T, size_t Rank>
tensor<T, Rank> operator&(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator&(
    const tensor<T, Rank>  &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator&(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank>  &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator|(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator|(
    const tensor<T, Rank>  &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator|(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank>  &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator^(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator^(
    const tensor<T, Rank>  &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator^(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank>  &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator<<(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator<<(
    const tensor<T, Rank>  &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator<<(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank>  &rhs
);

template <class T, size_t Rank>
tensor<T, Rank> operator>>(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<T, Rank> operator>>(
    const tensor<T, Rank>  &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<T, Rank> operator>>(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank>  &rhs
);
```

### Logical operators

```cpp
template <class T, size_t Rank>
tensor<bool, Rank> operator&&(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator&&(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator&&(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<bool, Rank> operator||(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator||(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator||(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);
```

### Relational operators

```cpp
template <class T, size_t Rank>
tensor<bool, Rank> operator==(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator==(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator==(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<bool, Rank> operator!=(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator!=(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator!=(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<bool, Rank> operator<(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator<(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator<(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<bool, Rank> operator>(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator>(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator>(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<bool, Rank> operator<=(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator<=(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator<=(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);

template <class T, size_t Rank>
tensor<bool, Rank> operator>=(
    const tensor<T, Rank> &lhs, const tensor<T, Rank> &rhs
);
template <class T, size_t Rank>
tensor<bool, Rank> operator>=(
    const tensor<T, Rank> &lhs, const typename tensor<T, Rank>::value_type &val
);
template <class T, size_t Rank>
tensor<bool, Rank> operator>=(
    const typename tensor<T, Rank>::value_type &val, const tensor<T, Rank> &rhs
);
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> x{-4,  4, 16,  1,  9, -4, 13,  8,  8, -1};

    // The following line is equivalent to:
    //
    // np::array<int> y(x.size());
    // for (unsigned i = 0; i < y.size(); ++i) {
    //     y[i] = x[i]*x[i] - 3*x[i] + 2;
    // }
    //
    // Notice that no temporary arrays are created to store intermediate
    // results. The whole expression is evaluated only when assigned to y.
    np::array<int> y = x*x - 3*x + 2;

    std::cout << "x:\n" << x << "\n";
    std::cout << "y:\n" << y << "\n";
    return 0;
}
```

Output

```
x:
[-4,  4, 16,  1,  9, -4, 13,  8,  8, -1]
y:
[ 30,   6, 210,   0,  56,  30, 132,  42,  42,   6]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> x{-4,  4, 16,  1,  9, -4, 13,  8,  8, -1};

    // The following variables seems like temporary arrays. However, each of
    // these variables are not tensors but expression objects. As such, they
    // should not be treated like usual tensors. They can not be re-assigned
    // and only a limited amount of methods are available.
    auto tmp1 = x*x;
    auto tmp2 = 3*x;
    // Neither tmp1 nor tmp2 are evaluated yet. The operation is delayed once
    // more. To force evaluation, assign the variables to a tensor or call
    // tmp1.copy() and tmp2.copy()
    auto tmp3 = tmp1 - tmp2;
    auto tmp4 = tmp3 + 2;
    np::array<int> y = tmp4; // y = x*x - 3*x + 2

    std::cout << "x:\n" << x << "\n";
    std::cout << "y:\n" << y << "\n";
    return 0;
}
```

Output

```
x:
[-4,  4, 16,  1,  9, -4, 13,  8,  8, -1]
y:
[ 30,   6, 210,   0,  56,  30, 132,  42,  42,   6]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a{{{17}, { 6}, {16}, {18}},
                         {{18}, {19}, {13}, {10}},
                         {{ 1}, { 3}, {-1}, {-1}}};
    np::tensor<int, 3> b{{{13, 13, 13, -1, 17, 16},
                          {11, 11,  0,  0,  9, 18},
                          { 3, 18, 15,  3, 10, 3},
                          { 8,  5, 17, -1,  0, 16}}};
    // Broadcasting works as usual:
    // (3, 4, 1) + (1, 4, 6) -> (3, 4, 6)
    np::tensor<int, 3> c = a + b;
    std::cout << "a " << a.shape() << ":\n" << a << "\n\n";
    std::cout << "b " << b.shape() << ":\n" << b << "\n\n";
    std::cout << "a + b " << c.shape() << ":\n" << c << "\n";
    return 0;
}
```

Output

```
a (3, 4, 1):
[[[17],
  [ 6],
  [16],
  [18]],

 [[18],
  [19],
  [13],
  [10]],

 [[ 1],
  [ 3],
  [-1],
  [-1]]]

b (1, 4, 6):
[[[13, 13, 13, -1, 17, 16],
  [11, 11,  0,  0,  9, 18],
  [ 3, 18, 15,  3, 10,  3],
  [ 8,  5, 17, -1,  0, 16]]]

a + b (3, 4, 6):
[[[30, 30, 30, 16, 34, 33],
  [17, 17,  6,  6, 15, 24],
  [19, 34, 31, 19, 26, 19],
  [26, 23, 35, 17, 18, 34]],

 [[31, 31, 31, 17, 35, 34],
  [30, 30, 19, 19, 28, 37],
  [16, 31, 28, 16, 23, 16],
  [18, 15, 27,  9, 10, 26]],

 [[14, 14, 14,  0, 18, 17],
  [14, 14,  3,  3, 12, 21],
  [ 2, 17, 14,  2,  9,  2],
  [ 7,  4, 16, -2, -1, 15]]]
```

## Matrix multiplication

### `matmul`

Return the matrix multiplication of two matrices.
```cpp
template <class T>
T matmul(const tensor<T, 1> &a, const tensor<T, 1> &b);

template <class T>
tensor<T, 2> matmul(const tensor<T, 2> &a, const tensor<T, 2> &b);

template <class T, size_t Rank>
tensor<T, Rank - 1> matmul(const tensor<T, Rank> &a, const tensor<T, 1> &b);

template <class T, size_t Rank>
tensor<T, Rank - 1> matmul(const tensor<T, 1> &a, const tensor<T, Rank> &b);

template <class T, size_t Rank>
tensor<T, Rank> matmul(const tensor<T, Rank> &a, const tensor<T, Rank> &b);
```

The behaviour depends on the arguments in the following way.

* If both arguments are one-dimensional, it is the inner product of vectors
(without complex conjugation).
* If both arguments are two-dimensional, it is the conventional matrix
multiplication.
* If the first argument is one-dimensional, it is promoted to a matrix by
prepending a 1 to its dimensions. After matrix multiplication the prepended 1
is removed.
* If the second argument is one-dimensional, it is promoted to a matrix by
appending a 1 to its dimensions. After matrix multiplication the appended 1 is
removed.
* If both arguments are $n$-dimensional, $n > 2$, it is treated as a stack of
matrices residing in the last two indexes and broadcast accordingly.

The inner product of two vectors $a = [a_1, a_2, \ldots, a_n]$ and
$b = [b_1, b_2, \ldots, b_n]$ is defined as

$$a \cdot b = a_1b_1 + a_2b_2 + \ldots + a_nb_n$$

The matrix multiplication of a $m \times p$ matrix $A = (a_{ij})$ and a
$p \times n$ matrix $B = (b_{ij})$ is the $m \times n$ matrix $C = (c_{ij})$
such that

$$c_{ij} = a_{i1}b_{1j} + a_{i2}b_{2j} + \ldots + a_{ip}b_{pj}$$

Parameters

* `a` First tensor-like argument.
* `b` Second tensor-like argument.

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
    // Prints the inner product of a and b.
    std::cout << np::matmul(a, b) << "\n";
    return 0;
}
```

Input

```
[-3, 10,  9,  7,  5, 13]
[-3,  0, 10, -5,  5,  6]
```

Output

```
167
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
[[ 4, -2, 10, -5, 11,  1],
 [ 6,  5,  9,  0, -1, 10],
 [ 6,  8,  0,  7,  7,  0],
 [ 1,  7,  4,  8,  0, -2]]

[[-2, -4, -4],
 [ 4,  6,  0],
 [-5,  8,  1],
 [-1,  2, 11],
 [ 6, -1,  3],
 [11, 10, -3]]
```

Output

```
[[ 16,  41, -31],
 [ 67, 179, -48],
 [ 55,  31,  74],
 [-24,  66,  94]]
```

Input

```
[[ 4, -2, 10, -5, 11,  1],
 [ 6,  5,  9,  0, -1, 10],
 [ 6,  8,  0,  7,  7,  0],
 [ 1,  7,  4,  8,  0, -2]]

[[ 8,  4, 12,  6],
 [ 7, -4, 14,  3],
 [13, -3, 13,  3],
 [ 0, -4, 14, 14]]
```

Output

```
terminate called after throwing an instance of 'std::invalid_argument'
  what():  shapes (4, 6) and (4, 4) not aligned: 6 (dim 1) != 4 (dim 0)
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
[[ 4, -2, 10, -5, 11,  1],
 [ 6,  5,  9,  0, -1, 10],
 [ 6,  8,  0,  7,  7,  0],
 [ 1,  7,  4,  8,  0, -2]]

[-3,  0, 10, -5,  5,  6]
```

Output

```
[174, 127, -18, -15]
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
[-3, 10,  9,  7,  5, 13]

[[-2, -4, -4],
 [ 4,  6,  0],
 [-5,  8,  1],
 [-1,  2, 11],
 [ 6, -1,  3],
 [11, 10, -3]]
```

Output

```
[167, 283,  74]
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
    // is the element-wise matrix multiplication, with the matrices residing
    // in the last two axes.
    std::cout << "matmul(a, b):\n" << np::matmul(a, b) << "\n";
    // Prints the multiplications one by one to check the previous statement.
    for (unsigned i = 0; i < a.shape(0); ++i) {
        std::cout << "Block " << i << "\n";
        std::cout << np::matmul(a(i, np::slice(), np::slice()),
                                b(i, np::slice(), np::slice())) << "\n";
    }
    return 0;
}
```

Input

```
[[[-5, -2,  1, 10, -3],
  [ 9,  8,  9, 11,  6],
  [11, -2, -5,  7,  8]],
 [[10,  6, 12,  8,  6],
  [ 6, -3,  7, -2,  8],
  [-3,  5,  2,  9, 14]],
 [[ 4,  6,  5,  3,  0],
  [ 8,  7,  5,  0,  8],
  [10,  3, 12, 12, -4]]]

[[[10,  4],
  [14, 13],
  [ 0, 13],
  [14, 10],
  [ 7,  1]],
 [[13,  5],
  [-1, 10],
  [11, 10],
  [ 5,  4],
  [-2,  2]],
 [[-3, -5],
  [ 7,  8],
  [-3,  5],
  [ 1, -1],
  [-4,  6]]]
```

Output

```
matmul(a, b):
[[[ 41,  64],
  [398, 373],
  [236,  31]],

 [[284, 274],
  [132,  78],
  [ -5, 119]],

 [[ 18,  50],
  [-22,  89],
  [-17,  -2]]]
Block 0
[[ 41,  64],
 [398, 373],
 [236,  31]]
Block 1
[[284, 274],
 [132,  78],
 [ -5, 119]]
Block 2
[[ 18,  50],
 [-22,  89],
 [-17,  -2]]
```