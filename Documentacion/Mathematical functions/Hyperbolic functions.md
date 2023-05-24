# [Mathematical functions](./readme.md)

Defined in `numcpp/math.h`

- [Mathematical functions](#mathematical-functions)
  - [Hyperbolic functions](#hyperbolic-functions)
    - [`cosh`](#cosh)
    - [`sinh`](#sinh)
    - [`tanh`](#tanh)
    - [`acosh`](#acosh)
    - [`asinh`](#asinh)
    - [`atanh`](#atanh)

## Hyperbolic functions

### `cosh`

Return the hyperbolic cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cosh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the hyperbolic angles.

Returns

* A light-weight object with the hyperbolic cosine of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(-4., 4.);
    np::matrix<double> df = np::stack<1>(x, np::cosh(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[        -4,  27.308233],
 [-3.8367347,  23.197683],
 [-3.6734694,  19.706855],
 [-3.5102041,  16.742493],
 [-3.3469388,  14.225403],
 ...,
 [ 3.3469388,  14.225403],
 [ 3.5102041,  16.742493],
 [ 3.6734694,  19.706855],
 [ 3.8367347,  23.197683],
 [         4,  27.308233]]
```

![Hyperbolic cosine](../Figures/Mathematical%20Functions/cosh.png)

### `sinh`

Return the hyperbolic sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> sinh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the hyperbolic angles.

Returns

* A light-weight object with the hyperbolic sine of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(-4., 4.);
    np::matrix<double> df = np::stack<1>(x, np::sinh(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[        -4, -27.289917],
 [-3.8367347, -23.176119],
 [-3.6734694, -19.681467],
 [-3.5102041, -16.712602],
 [-3.3469388, -14.190211],
 ...,
 [ 3.3469388,  14.190211],
 [ 3.5102041,  16.712602],
 [ 3.6734694,  19.681467],
 [ 3.8367347,  23.176119],
 [         4,  27.289917]]
```

![Hyperbolic sine](../Figures/Mathematical%20Functions/sinh.png)

### `tanh`

Return the hyperbolic tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> tanh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the hyperbolic angles.

Returns

* A light-weight object with the hyperbolic tangent of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(-4., 4.);
    np::matrix<double> df = np::stack<1>(x, np::tanh(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[         -4,  -0.9993293],
 [ -3.8367347, -0.99907043],
 [ -3.6734694, -0.99871171],
 [ -3.5102041, -0.99821467],
 [ -3.3469388, -0.99752612],
 ...,
 [  3.3469388,  0.99752612],
 [  3.5102041,  0.99821467],
 [  3.6734694,  0.99871171],
 [  3.8367347,  0.99907043],
 [          4,   0.9993293]]
```

![Hyperbolic tangent](../Figures/Mathematical%20Functions/tanh.png)

### `acosh`

Return the inverse hyperbolic cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> acosh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose inverse hyperbolic cosine is computed.

Returns

* A light-weight object with the inverse hyperbolic cosine of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(1., 25.);
    np::matrix<double> df = np::stack<1>(x, np::acosh(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[        1,         0],
 [1.4897959, 0.9532404],
 [1.9795918, 1.3050943],
 [2.4693878, 1.5533405],
 [2.9591837, 1.7482048],
 ...,
 [23.040816, 3.8299432],
 [23.530612, 3.8509976],
 [24.020408, 3.8716174],
 [24.510204, 3.8918203],
 [       25, 3.9116228]]
```

![Inverse hyperbolic cosine](../Figures/Mathematical%20Functions/acosh.png)

### `asinh`

Return the inverse hyperbolic sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> asinh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose inverse hyperbolic sine is computed.

Returns

* A light-weight object with the inverse hyperbolic sine of each element in the tensor.This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(-25., 25.);
    np::matrix<double> df = np::stack<1>(x, np::asinh(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[       -25, -3.9124228],
 [-23.979592, -3.8707848],
 [-22.959184, -3.8273391],
 [-21.938776, -3.7819218],
 [-20.918367, -3.7343456],
 ...,
 [ 20.918367,  3.7343456],
 [ 21.938776,  3.7819218],
 [ 22.959184,  3.8273391],
 [ 23.979592,  3.8707848],
 [        25,  3.9124228]]
```

![Inverse hyperbolic cosine](../Figures/Mathematical%20Functions/asinh.png)

### `atanh`

Return the inverse hyperbolic tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> atanh(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose inverse hyperbolic tangent is computed.

Returns

* A light-weight object with the inverse hyperbolic tangent of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(-1., 1.);
    np::matrix<double> df = np::stack<1>(x, np::atanh(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[         -1,        -inf],
 [-0.95918367,  -1.9356005],
 [-0.91836735,  -1.5785002],
 [-0.87755102,  -1.3650146],
 [-0.83673469,  -1.2101841],
 ...,
 [ 0.83673469,   1.2101841],
 [ 0.87755102,   1.3650146],
 [ 0.91836735,   1.5785002],
 [ 0.95918367,   1.9356005],
 [          1,     18.3684]]
```

![Inverse hyperbolic tangent](../Figures/Mathematical%20Functions/atanh.png)
