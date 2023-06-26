# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Operators](#operators)
    - [Unary operators](#unary-operators)
    - [Arithmetic operators](#arithmetic-operators)
    - [Bitwise operators](#bitwise-operators)
    - [Logical operators](#logical-operators)
    - [Relational operators](#relational-operators)

## Operators

Each of this functions performs their respective operation on all the elements in the tensor.

When both the left-hand side and right-hand side arguments are tensor objects, the operation is performed between the corresponding elements in each object (the first element of the left one with the right one, the second with the second, and so on...)

When one of the arguments is a value, the operation is applied to all the elements in the tensor against that value.

Parameters

* `lhs` Left-hand side tensor-like object.
* `rhs` Right-hand side tensor-like object.
* `val` Value to use either as left-hand or right-hand operand.

Returns

* A light-weight object which stores the result of performing the operation on each element. None of these operators creates a new tensor object, instead, an expression object is returned. The returned object uses lazy-evaluation, which means that the result of each operation is computed only when required, i.e., when the whole expression is evaluated or assigned to a tensor object.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](../Shapes/Non-member%20functions.md#broadcast_shapes).

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
tensor<T, Rank> operator+(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator+(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator+(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator-(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator-(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator-(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator*(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator*(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator*(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator/(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator/(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator/(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator%(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator%(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator%(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);
```

### Bitwise operators

```cpp
template <class T, size_t Rank>
tensor<T, Rank> operator&(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator&(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator&(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator|(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator|(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator|(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator^(const tensor<T, Rank> &lhs,
                          const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator^(const tensor<T, Rank> &lhs,
                          const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator^(const typename tensor<T, Rank>::value_type &val,
                          const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator<<(const tensor<T, Rank> &lhs,
                           const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator<<(const tensor<T, Rank> &lhs,
                           const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator<<(const typename tensor<T, Rank>::value_type &val,
                           const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<T, Rank> operator>>(const tensor<T, Rank> &lhs,
                           const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<T, Rank> operator>>(const tensor<T, Rank> &lhs,
                           const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<T, Rank> operator>>(const typename tensor<T, Rank>::value_type &val,
                           const tensor<T, Rank> &rhs);
```

### Logical operators

```cpp
template <class T, size_t Rank>
tensor<bool, Rank> operator&&(const tensor<T, Rank> &lhs,
                              const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator&&(const tensor<T, Rank> &lhs,
                              const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator&&(const typename tensor<T, Rank>::value_type &val,
                              const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<bool, Rank> operator||(const tensor<T, Rank> &lhs,
                              const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator||(const tensor<T, Rank> &lhs,
                              const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator||(const typename tensor<T, Rank>::value_type &val,
                              const tensor<T, Rank> &rhs);
```

### Relational operators

```cpp
template <class T, size_t Rank>
tensor<bool, Rank> operator==(const tensor<T, Rank> &lhs,
                              const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator==(const tensor<T, Rank> &lhs,
                              const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator==(const typename tensor<T, Rank>::value_type &val,
                              const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<bool, Rank> operator!=(const tensor<T, Rank> &lhs,
                              const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator!=(const tensor<T, Rank> &lhs,
                              const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator!=(const typename tensor<T, Rank>::value_type &val,
                              const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<bool, Rank> operator<(const tensor<T, Rank> &lhs,
                             const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator<(const tensor<T, Rank> &lhs,
                             const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator<(const typename tensor<T, Rank>::value_type &val,
                             const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<bool, Rank> operator>(const tensor<T, Rank> &lhs,
                             const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator>(const tensor<T, Rank> &lhs,
                             const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator>(const typename tensor<T, Rank>::value_type &val,
                             const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<bool, Rank> operator<=(const tensor<T, Rank> &lhs,
                              const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator<=(const tensor<T, Rank> &lhs,
                              const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator<=(const typename tensor<T, Rank>::value_type &val,
                              const tensor<T, Rank> &rhs);

template <class T, size_t Rank>
tensor<bool, Rank> operator>=(const tensor<T, Rank> &lhs,
                              const tensor<T, Rank> &rhs);
template <class T, size_t Rank>
tensor<bool, Rank> operator>=(const tensor<T, Rank> &lhs,
                              const typename tensor<T, Rank>::value_type &val);
template <class T, size_t Rank>
tensor<bool, Rank> operator>=(const typename tensor<T, Rank>::value_type &val,
                              const tensor<T, Rank> &rhs);
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> x{-4,  4, 16,  1,  9, -4, 13,  8,  8, -1};

    // Thanks to lazy-evaluation, the following line is equivalent to:
    //
    // np::vector<int> y(x.size());
    // for (unsigned i = 0; i < y.size(); ++i) {
    //     y[i] = x[i]*x[i] - 3*x[i] + 2;
    // }
    //
    // No temporary arrays are created to store intermediate results. The whole
    // expression is evaluated only when assigned to y.
    np::vector<int> y = x*x - 3*x + 2;

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
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    double a = 1;
    np::vector<double> b{ 0, -2, -3, -4, -5, -6, -7, -8, -9, -10};
    np::vector<double> c{-1,  1,  2,  4,  6,  9, 12, 12, 18,  21};

    np::vector<double> x1 = (-b - np::sqrt(b*b - 4*a*c)) / (2*a);
    np::vector<double> x2 = (-b + np::sqrt(b*b - 4*a*c)) / (2*a);

    for (unsigned i = 0; i < x1.size(); ++i) {
        std::cout << "(x - " << x1[i] << ")(x - " << x2[i] << ") = ";
        std::cout << a << "x² + " << b[i] << "x + " << c[i] << "\n";
    }
    return 0;
}
```

Output

```
(x - -1)(x - 1) = 1x² + 0x + -1
(x - 1)(x - 1) = 1x² + -2x + 1
(x - 1)(x - 2) = 1x² + -3x + 2
(x - 2)(x - 2) = 1x² + -4x + 4
(x - 2)(x - 3) = 1x² + -5x + 6
(x - 3)(x - 3) = 1x² + -6x + 9
(x - 3)(x - 4) = 1x² + -7x + 12
(x - 2)(x - 6) = 1x² + -8x + 12
(x - 3)(x - 6) = 1x² + -9x + 18
(x - 3)(x - 7) = 1x² + -10x + 21
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a{{ 7, 12, 18,  8,  4, 15},
                      {15,  8, -1, -2, -2,  3},
                      {12, 19, 15,  5, -3,  2},
                      { 3, -5, 10, 13,  7, 14}};
    np::matrix<int> b{{1},
                      {3},
                      {4},
                      {0}};
    std::cout << "a:\n" << a << "\n";
    std::cout << "b:\n" << b << "\n";

    // The shape of a is (4, 6) while the shape of b is (4, 1).
    // b is broadcasted to match (4, 6) by creating copies of b for each column.
    // No copies are created in memory. Broadcasting handles this efficiently.
    std::cout << "a + b:\n" << a + b << "\n";
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
[[1],
 [3],
 [4],
 [0]]
a + b:
[[ 8, 13, 19,  9,  5, 16],
 [18, 11,  2,  1,  1,  6],
 [16, 23, 19,  9,  1,  6],
 [ 3, -5, 10, 13,  7, 14]]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
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
    std::cout << a + b << "\n";
    return 0;
}
```

Possible output

```
terminate called after throwing an instance of 'std::invalid_argument'
  what():  operands could not be broadcast together with shapes (4, 6) (4, 2)
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    np::matrix<int> b{{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}};
    std::cout << "a:\n" << a << "\n";
    std::cout << "b:\n" << b << "\n";

    // The shape of a is (1, 10) while the shape of b is (10, 1).
    // The resulting shape after broadcasting is (10, 10).
    // a is broadcasted to match (10, 10) by creating copies of a for each row.
    // b is broadcasted to match (10, 10) by creating copies of b for each
    // column.
    // In neither case a copy is created in memory. Broadcasting handles this
    // efficently.
    std::cout << "a * b:\n" << a * b << "\n";
    return 0;
}
```

Output

```
a:
[[ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10]]
b:
[[ 1],
 [ 2],
 [ 3],
 [ 4],
 [ 5],
 [ 6],
 [ 7],
 [ 8],
 [ 9],
 [10]]
a * b:
[[  1,   2,   3,   4,   5,   6,   7,   8,   9,  10],
 [  2,   4,   6,   8,  10,  12,  14,  16,  18,  20],
 [  3,   6,   9,  12,  15,  18,  21,  24,  27,  30],
 [  4,   8,  12,  16,  20,  24,  28,  32,  36,  40],
 [  5,  10,  15,  20,  25,  30,  35,  40,  45,  50],
 [  6,  12,  18,  24,  30,  36,  42,  48,  54,  60],
 [  7,  14,  21,  28,  35,  42,  49,  56,  63,  70],
 [  8,  16,  24,  32,  40,  48,  56,  64,  72,  80],
 [  9,  18,  27,  36,  45,  54,  63,  72,  81,  90],
 [ 10,  20,  30,  40,  50,  60,  70,  80,  90, 100]]
```
