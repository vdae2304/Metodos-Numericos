# [Mathematical functions](readme.md)

Defined in header [`numcpp/math.h`](/include/numcpp/math.h)

- [Mathematical functions](#mathematical-functions)
  - [Classification functions](#classification-functions)
    - [`isfinite`](#isfinite)
    - [`isinf`](#isinf)
    - [`isnan`](#isnan)
    - [`signbit`](#signbit)

## Classification functions

### `isfinite`

Return whether $x$ is a finite value (neither infinite nor NaN), element-wise.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isfinite(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with floating point values.

Returns

* A light-weight object with each element set to true where `x` is finite and false otherwise. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x{0., 0./0., 1., -1., np::sqrt(-1.), 1./0., -1./0.};
    std::cout << "x:\n" << x << "\n";
    std::cout << "isfinite(x):\n" << std::boolalpha << np::isfinite(x) << "\n";
    return 0;
}
```

Output

```
x:
[   0,  nan,    1,   -1,  nan,  inf, -inf]
isfinite(x):
[ true, false,  true,  true, false, false, false]
```

### `isinf`

Return whether $x$ is an infinity value (either positive infinity or negative infinity), element-wise.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isinf(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with floating point values.

Returns

* A light-weight object with each element set to true where `x` is infinity and false otherwise. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x{0., 0./0., 1., -1., np::sqrt(-1.), 1./0., -1./0.};
    std::cout << "x:\n" << x << "\n";
    std::cout << "isinf(x):\n" << std::boolalpha << np::isinf(x) << "\n";
    return 0;
}
```

Output

```
x:
[   0,  nan,    1,   -1,  nan,  inf, -inf]
isinf(x):
[false, false, false, false, false,  true,  true]
```

### `isnan`

Return whether $x$ is a NaN (Not a Number) value, element-wise. The NaN values are used to identify undefined or non-representable values for floating-point numbers, such as the square root of negative numbers or the result of $0/0$.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isnan(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with floating point values.

Returns

* A light-weight object with each element set to true where `x` is NaN and false otherwise. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x{0., 0./0., 1., -1., np::sqrt(-1.), 1./0., -1./0.};
    std::cout << "x:\n" << x << "\n";
    std::cout << "isnan(x):\n" << std::boolalpha << np::isnan(x) << "\n";
    return 0;
}
```

Output

```
x:
[   0,  nan,    1,   -1,  nan,  inf, -inf]
isnan(x):
[false,  true, false, false,  true, false, false]
```

### `signbit`

Return whether the sign of $x$ is negative, element-wise.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> signbit(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with floating point or integer values.

Returns

* A light-weight object with each element set to true where `x` is negative and false otherwise. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> x;
    std::cin >> x;
    std::cout << std::boolalpha << np::signbit(x) << "\n";
    return 0;
}
```

Input

```
[2, 7, -3, 1, 0, 8, -1, 9, -5, 12]
```

Output

```
[false, false,  true, false, false, false,  true, false,  true, false]
```
