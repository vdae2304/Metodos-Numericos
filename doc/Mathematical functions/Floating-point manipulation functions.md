# [Mathematical functions](readme.md)

Defined in header [`numcpp/math.h`](/include/numcpp/math.h)

- [Mathematical functions](#mathematical-functions)
  - [Floating-point manipulation functions](#floating-point-manipulation-functions)
    - [`frexp`](#frexp)
    - [`ldexp`](#ldexp)
    - [`copysign`](#copysign)
    - [`nextafter`](#nextafter)

## Floating-point manipulation functions

### `frexp`

Descompose $x$ into its binary significand and an integral power of two, such that $x = {significand} \times 2^{exponent}$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<std::pair<T, int>, Rank> frexp(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to be decomposed.

Returns

* A light-weight object with the binary significand and the exponent of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h" // np::unzip
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    np::vector<std::pair<double, int>> y = np::frexp(x);
    std::cout << np::unzip<0>(y) << "\n";
    std::cout << np::unzip<1>(y) << "\n";
    return 0;
}
```

Input

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

Output

```
[     0,    0.5,    0.5,   0.75,    0.5,  0.625,   0.75,  0.875,    0.5, 0.5625]
[0, 1, 2, 2, 3, 3, 3, 3, 4, 4]
```

### `ldexp`

Return the result of multiplying $x$ (the significand) by 2 raised to the power of `exp` (the exponent), element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> ldexp(const tensor<T, Rank> &x, const tensor<int, Rank> &exp);

template <class T, size_t Rank>
tensor<T, Rank> ldexp(const tensor<T, Rank> &x, int exp);

template <class T, size_t Rank>
tensor<T, Rank> ldexp(const T &x, const tensor<int, Rank> &exp);
```

Parameters

* `x` A tensor-like object with the values of the significand.
* `exp` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the result of $x \times 2^{exp}$, element-wise. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    np::vector<int> exp;
    std::cin >> x >> exp;
    std::cout << np::ldexp(x, exp) << "\n";
    return 0;
}
```

Input

```
[0, 0.5, 0.5, 0.75, 0.5, 0.625, 0.75, 0.875, 0.5, 0.5625]
[0, 1, 2, 2, 3, 3, 3, 3, 4, 4]
```

Output

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

### `copysign`

Return a value with the magnitude of $x$ and the sign of $y$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> copysign(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> copysign(const typename tensor<T, Rank>::value_type &x,
                         const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> copysign(const tensor<T, Rank> &x,
                         const typename tensor<T, Rank>::value_type &y);
```

Parameters

* `x` A tensor-like object with the values to change the sign of.
* `y` A tensor-like object with the values to copy the sign from.

Returns

* A light-weight object with the values from the first tensor and the signs from the second tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
    std::cin >> x >> y;
    std::cout << np::copysign(x, y) << "\n";
    return 0;
}
```

Input

```
[5,  3, -1, 0,  0, 2,  8, 10, -7]
[1, -1,  1, 1, -1, 1, -1, -1, -1]
```

Output

```
[  5,  -3,   1,   0,  -0,   2,  -8, -10,  -7]
```

### `nextafter`

Return the next representable value after $x$ in the direction of $y$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> nextafter(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> nextafter(const typename tensor<T, Rank>::value_type &x,
                          const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> nextafter(const tensor<T, Rank> &x,
                          const typename tensor<T, Rank>::value_type &y);
```

Parameters

* `x` A tensor-like object with the base values.
* `y` A tensor-like object with the directions where to look for the next representable values.

Returns

* A light-weight object with the next representable value of each element in the first tensor in the direction of the second tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    precision = fullprecision;
    np::vector<double> x, y;
    std::cin >> x >> y;
    std::cout << np::nextafter(x, y) << "\n";
    return 0;
}
```

Input

```
[-2, -2, -1, -1,  0, 0, 1, 1, 2, 2]
[-3, -1, -2,  0, -1, 1, 0, 2, 1, 3]
```

Output

```
[     -2.0000000000000004,      -1.9999999999999998,      -1.0000000000000002, 
     -0.99999999999999989, -4.9406564584124654e-324,  4.9406564584124654e-324, 
      0.99999999999999989,       1.0000000000000002,       1.9999999999999998, 
       2.0000000000000004]
```
