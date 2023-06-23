# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Compound assignment](#compound-assignment)

## Compound assignment

Assigns to `*this` the result of performing the respective operation on all the elements in the tensor.

When the right-hand side argument is a tensor object, the operation is performed between the corresponding elements in each object (the first element of the left one with the right one, the second with the second, and so on...)

When the right-hand side argument is a value, the operation is applied to all the elements in the tensor against that value.
```cpp
tensor &operator+=(const tensor<T, Rank> &rhs);
tensor &operator-=(const tensor<T, Rank> &rhs);
tensor &operator*=(const tensor<T, Rank> &rhs);
tensor &operator/=(const tensor<T, Rank> &rhs);
tensor &operator%=(const tensor<T, Rank> &rhs);
tensor &operator&=(const tensor<T, Rank> &rhs);
tensor &operator|=(const tensor<T, Rank> &rhs);
tensor &operator^=(const tensor<T, Rank> &rhs);
tensor &operator<<=(const tensor<T, Rank> &rhs);
tensor &operator>>=(const tensor<T, Rank> &rhs);

tensor &operator+=(const T &val);
tensor &operator-=(const T &val);
tensor &operator*=(const T &val);
tensor &operator/=(const T &val);
tensor &operator%=(const T &val);
tensor &operator&=(const T &val);
tensor &operator|=(const T &val);
tensor &operator^=(const T &val);
tensor &operator<<=(const T &val);
tensor &operator>>=(const T &val);
```

Parameters

* `rhs` Right-hand side tensor-like object.
* `val` Value to use as right-hand operand.

Returns

* `*this`

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](./Shapes/Non-member%20functions.md#broadcast_shapes).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a{11, 8, 16, 17, -2, 16, 18, -5, -4, 15};
    np::vector<int> b{ 0, 1, 14,  0,  8,  3,  2,  2, 18,  0};
    std::cout << "a:\n" << a << "\n";
    std::cout << "b:\n" << b << "\n";
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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a{11, 8, 16, 17, -2, 16, 18, -5, -4, 15};
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
    a -= b;

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
[[1],
 [3],
 [4],
 [0]]
a - b:
[[ 6, 11, 17,  7,  3, 14],
 [12,  5, -4, -5, -5,  0],
 [ 8, 15, 11,  1, -7, -2],
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
    a += b;
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
    np::matrix<int> a{{ 7, 12, 18,  8,  4, 15}};
    np::matrix<int> b{{1},
                      {3},
                      {4},
                      {0}};
    // Throws an exception.
    // The shape of a is (1, 6) while the shape of b is (4, 1). The resulting
    // shape after broadcasting is (4, 6), which is not assignable to a.
    a += b;
    return 0;
}
```

Possible output

```
terminate called after throwing an instance of 'std::invalid_argument'
  what():  non-broadcastable output operand with shape (1, 6) doesn't match the broadcast shape (4, 6)
```
