# [Mathematical functions](readme.md)

Defined in header [`numcpp/math.h`](/include/numcpp/math.h)

- [Mathematical functions](#mathematical-functions)
  - [Rounding](#rounding)
    - [`ceil`](#ceil)
    - [`floor`](#floor)
    - [`trunc`](#trunc)
    - [`round`](#round)

## Rounding

### `ceil`

Rounds $x$ upward. Return the smallest integral value that is not less than $x$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> ceil(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to round up.

Returns

* A light-weight object with the ceiling of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::ceil(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -0, -0, -0,  0,  1,  1,  1,  1]
```
 
### `floor`

Rounds $x$ downward. Return the largest integral value that is not greater than $x$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> floor(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to round down.

Returns

* A light-weight object with the floor of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::floor(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -1, -1, -1,  0,  0,  0,  0,  1]
```

### `trunc`

Rounds $x$ toward zero. Return the nearest integral value that is not larger in magnitude than $x$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> trunc(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to truncate.

Returns

* A light-weight object with the truncated value of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::trunc(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -0, -0, -0,  0,  0,  0,  0,  1]
```

### `round`

Return the integral value that is nearest to $x$, element-wise. Halfway cases are rounded away from zero.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> round(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values to round.

Returns

* A light-weight object with the rounded value of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::round(x) << "\n";
    return 0;
}
```

Input

```
[-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1]
```

Output

```
[-1, -1, -1, -0,  0,  0,  1,  1,  1]
```
