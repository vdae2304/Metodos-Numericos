# [Mathematical functions](readme.md)

Defined in header [`numcpp/math.h`](/include/numcpp/math.h)

- [Mathematical functions](#mathematical-functions)
  - [Power functions](#power-functions)
    - [`pow`](#pow)
    - [`sqrt`](#sqrt)
    - [`cbrt`](#cbrt)

## Power functions

### `pow`

Return $x$ raised to the power $y$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> pow(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> pow(const tensor<T, Rank> &x,
                    const typename tensor<T, Rank>::value_type &y);

template <class T, size_t Rank>
tensor<T, Rank> pow(const typename tensor<T, Rank>::value_type &x,
                    const tensor<T, Rank> &y);
```

Parameters

* `x` A tensor-like object with the values of the base.
* `y` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the result of rasing `x` to the power `y`, element-wise. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
    std::cin >> x >> y;
    std::cout << np::pow(x, y) << "\n";
    return 0;
}
```

Input

```
[2, 2, 2, 2, 3, 3, 4, 5, 10, 100]
[2, 3, 4, 5, 2, 3, 2, 2, -1, 0.5]
```

Output

```
[  4,   8,  16,  32,   9,  27,  16,  25, 0.1,  10]
```

### `sqrt`

Return the square root, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> sqrt(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose square root is computed.

Returns

* A light-weight object with the square root of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::sqrt(x) << "\n";
    return 0;
}
```

Input

```
[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

Output

```
[ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10]
```

### `cbrt`

Return the cubic root, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cbrt(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose cubic root is computed.

Returns

* A light-weight object with the cubic root of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::cbrt(x) << "\n";
    return 0;
}
```

Input

```
[-64, -27, -8, -1, 0, 1, 8, 27, 64]
```

Output

```
[-4, -3, -2, -1,  0,  1,  2,  3,  4]
```
