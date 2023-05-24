# [Mathematical functions](./readme.md)

Defined in `numcpp/math.h`

- [Mathematical functions](#mathematical-functions)
  - [Exponential and logarithmic functions](#exponential-and-logarithmic-functions)
    - [`exp`](#exp)
    - [`log`](#log)
    - [`log10`](#log10)
    - [`exp2`](#exp2)
    - [`log2`](#log2)
    - [`expm1`](#expm1)
    - [`log1p`](#log1p)

## Exponential and logarithmic functions

### `exp`

Return the base- $e$ exponential, which is $e$ raised to the power $x$: $e^x$,element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> exp(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the exponential of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

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
    np::matrix<double> df = np::stack<1>(x, np::exp(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[         -4, 0.018315639],
 [ -3.8367347, 0.021563899],
 [ -3.6734694, 0.025388235],
 [ -3.5102041, 0.029890814],
 [ -3.3469388,  0.03519192],
 ...,
 [  3.3469388,   28.415614],
 [  3.5102041,   33.455095],
 [  3.6734694,   39.388322],
 [  3.8367347,   46.373802],
 [          4,    54.59815]]
```

![Exponential](../Figures/Mathematical%20Functions/exp.png)

### `log`

Return the natural logarithm, which is the inverse of the exponential function (exp), element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light weight-object with the natural logarithm of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(0., 10.);
    np::matrix<double> df = np::stack<1>(x, np::log(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[          0,        -inf],
 [ 0.20408163,  -1.5892352],
 [ 0.40816327, -0.89608802],
 [  0.6122449, -0.49062292],
 [ 0.81632653, -0.20294084],
 ...,
 [  9.1836735,   2.2174273],
 [  9.3877551,   2.2394062],
 [  9.5918367,   2.2609124],
 [  9.7959184,   2.2819658],
 [         10,   2.3025851]]
```

![Exponential](../Figures/Mathematical%20Functions/log.png)

### `log10`

Return the common (base-10) logarithm, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log10(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light-weight object with the common logarithm of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::log10(x) << "\n";
    return 0;
}
```

Input

```
[0.001, 0.01, 0.1, 1, 10, 100, 1000, 10000, 100000, 1000000]
```

Output

```
[-3, -2, -1,  0,  1,  2,  3,  4,  5,  6]
```

### `exp2`

Return the base-2 exponential, which is 2 raised to the power $x$: $2^x$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> exp2(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the base-2 exponential of each element in the tensor. This function does not create a new tensor, instead, an  expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::exp2(x) << "\n";
    return 0;
}
```

Input

```
[-3, -2, -1,  0,  1,  2,  3,  4,  5,  6]
```

Output

```
[0.125,  0.25,   0.5,     1,     2,     4,     8,    16,    32,    64]
```

### `log2`

Return the binary (base-2) logarithm, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log2(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light-weight object with the binary logarithm of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x;
    std::cin >> x;
    std::cout << np::log2(x) << "\n";
    return 0;
}
```

Input

```
[0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64]
```

Output

```
[-3, -2, -1,  0,  1,  2,  3,  4,  5,  6]
```

### `expm1`

Return $e$ raised to the power $x$ minus one: $e^x - 1$, element-wise. For
values of $x$ close to zero, `expm1` is more accurate than `exp(x) - 1`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> expm1(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values of the exponent.

Returns

* A light-weight object with the exponential minus one of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x = np::linspace(-1., 1.);
    std::cout << std::boolalpha;
    std::cout << np::allclose(np::exp(x) - 1, np::expm1(x)) << "\n";
    return 0;
}
```

Output

```
true
```

### `log1p`

Return the natural logarithm of one plus $x$, element-wise. For values of $x$ close to zero, `log1p` is more accurate than `log(1 + x)`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> log1p(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose logarithm is computed.

Returns

* A light-weight object with the natural logarithm of $(1 + x)$ for each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x = np::linspace(0., 1.);
    std::cout << std::boolalpha;
    std::cout << np::allclose(np::log(1 + x), np::log1p(x)) << "\n";
    return 0;
}
```

Output

```
true
```
