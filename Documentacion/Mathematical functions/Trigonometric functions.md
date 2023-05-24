# [Mathematical functions](./readme.md)

Defined in `numcpp/math.h`

- [Mathematical functions](#mathematical-functions)
  - [Trigonometric functions](#trigonometric-functions)
    - [`cos`](#cos)
    - [`sin`](#sin)
    - [`tan`](#tan)
    - [`hypot`](#hypot)
    - [`acos`](#acos)
    - [`asin`](#asin)
    - [`atan`](#atan)
    - [`atan2`](#atan2)
    - [`degrees`](#degrees)
    - [`radians`](#radians)

## Trigonometric functions

### `cos`

Return the cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> cos(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the cosine of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    const double pi = np::constants<double>::pi;
    np::vector<double> x = np::linspace(0, 2*pi);
    np::matrix<double> df = np::stack<1>(x, np::cos(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[         0,          1],
 [0.12822827, 0.99179001],
 [0.25645654, 0.96729486],
 [0.38468481, 0.92691676],
 [0.51291309,  0.8713187],
 ...,
 [ 5.7702722,  0.8713187],
 [ 5.8985005, 0.92691676],
 [ 6.0267288, 0.96729486],
 [  6.154957, 0.99179001],
 [ 6.2831853,          1]]
```

![Cosine](../Figures/Mathematical%20Functions/cos.png)

### `sin`

Return the sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> sin(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the sine of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    const double pi = np::constants<double>::pi;
    np::vector<double> x = np::linspace(0, 2*pi);
    np::matrix<double> df = np::stack<1>(x, np::sin(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[             0,              0],
 [    0.12822827,     0.12787716],
 [    0.25645654,     0.25365458],
 [    0.38468481,       0.375267],
 [    0.51291309,     0.49071755],
 ...,
 [     5.7702722,    -0.49071755],
 [     5.8985005,      -0.375267],
 [     6.0267288,    -0.25365458],
 [      6.154957,    -0.12787716],
 [     6.2831853, -2.4492936e-16]]
```

![Sine](../Figures/Mathematical%20Functions/sin.png)

### `tan`

Return the tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> tan(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the tangent of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    const double pi = np::constants<double>::pi;
    np::vector<double> x = np::linspace(-pi/2, pi/2);
    np::matrix<double> df = np::stack<1>(x, np::tan(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[    -1.5707963, -1.6331239e+16],
 [    -1.5066822,     -15.575807],
 [    -1.4425681,     -7.7558025],
 [    -1.3784539,     -5.1347886],
 [    -1.3143398,     -3.8134334],
 ...,
 [     1.3143398,      3.8134334],
 [     1.3784539,      5.1347886],
 [     1.4425681,      7.7558025],
 [     1.5066822,      15.575807],
 [     1.5707963,  1.6331239e+16]]
```

![Tangent](../Figures/Mathematical%20Functions/tan.png)

### `hypot`

Return the hypotenuse of a right-angled triangle whose legs are $x$ and $y$, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> hypot(const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> hypot(const tensor<T, Rank> &x,
                      const typename tensor<T, Rank>::value_type &y);

template <class T, size_t Rank>
tensor<T, Rank> hypot(const typename tensor<T, Rank>::value_type &x,
                      const tensor<T, Rank> &y);
```

Parameters

* `x` A tensor-like object with one of the legs of the right-angle triangles.
* `y` A tensor-like object with one of the legs of the right-angle triangles.

Returns

* A light-weight object with the hypotenuse of the triangles. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x, y;
    std::cin >> x >> y;
    std::cout << np::hypot(x, y) << "\n";
    return 0;
}
```

Input

```
[1, 3,  5,  7,  8,  9, 11, 12]
[1, 4, 12, 24, 15, 40, 60, 35]
```

Output

```
[1.4142136,         5,        13,        25,        17,        41,        61, 
        37]
```

### `acos`

Return the principal value of the arc cosine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> acos(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose arc cosine is computed.

Returns

* A light-weight object with the arc cosine, in radians, of each value in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

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
    np::matrix<double> df = np::stack<1>(x, np::acos(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[           -1,     3.1415927],
 [  -0.95918367,     2.8548975],
 [  -0.91836735,     2.7347312],
 [  -0.87755102,     2.6415269],
 [  -0.83673469,     2.5620893],
 ...,
 [   0.83673469,     0.5795034],
 [   0.87755102,    0.50006579],
 [   0.91836735,    0.40686148],
 [   0.95918367,    0.28669514],
 [            1, 2.1073424e-08]]
```

![Arc cosine](../Figures/Mathematical%20Functions/acos.png)

### `asin`

Return the principal value of the arc sine, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> asin(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose arc sine is computed.

Returns

* A light-weight object with the arc sine, in radians, of each value in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

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
    np::matrix<double> df = np::stack<1>(x, np::asin(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[         -1,  -1.5707963],
 [-0.95918367,  -1.2841012],
 [-0.91836735,  -1.1639348],
 [-0.87755102,  -1.0707305],
 [-0.83673469, -0.99129292],
 ...,
 [ 0.83673469,  0.99129292],
 [ 0.87755102,   1.0707305],
 [ 0.91836735,   1.1639348],
 [ 0.95918367,   1.2841012],
 [          1,   1.5707963]]
```

![Arc sine](../Figures/Mathematical%20Functions/asin.png)

### `atan`

Return the principal value of the arc tangent, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> atan(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the values whose arc tangent is computed.

Returns

* A light-weight object with the arc tangent, in radians, of each value in the tensor. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    threshold = 10;
    edgeitems = 5;

    np::vector<double> x = np::linspace(-5., 5.);
    np::matrix<double> df = np::stack<1>(x, np::atan(x));
    std::cout << df << "\n";

    np::savetxt("data.csv", df, /*delimiter=*/',', /*newline=*/'\n', 
                /*header=*/"x,y");
    return 0;
}
```

Output

```
[[        -5, -1.3734008],
 [-4.7959184,  -1.365231],
 [-4.5918367, -1.3563666],
 [-4.3877551, -1.3467167],
 [-4.1836735,  -1.336174],
 ...,
 [ 4.1836735,   1.336174],
 [ 4.3877551,  1.3467167],
 [ 4.5918367,  1.3563666],
 [ 4.7959184,   1.365231],
 [         5,  1.3734008]]
```

![Arc tangent](../Figures/Mathematical%20Functions/atan.png)

### `atan2`

Return the principal value of the arc tangent of $y/x$, element-wise. To compute the value, the function takes into account the sign of both arguments in order to determine the quadrant.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> atan2(const tensor<T, Rank> &y, const tensor<T, Rank> &x);

template <class T, size_t Rank>
tensor<T, Rank> atan2(const tensor<T, Rank> &y,
                      const typename tensor<T, Rank>::value_type &x);

template <class T, size_t Rank>
tensor<T, Rank> atan2(const typename tensor<T, Rank>::value_type &y,
                      const tensor<T, Rank> &x);
```

Parameters

* `y` A tensor-like object with the $y$-coordinates.
* `x` A tensor-like object with the $x$-coordinates.

Returns

* A light-weight object with the arc tangent, in radians, of $y/x$. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    const double pi = np::constants<double>::pi;
    np::vector<double> t = np::linspace(-pi, pi);
    np::vector<double> x = np::cos(t);
    np::vector<double> y = np::sin(t);
    std::cout << std::boolalpha << np::allclose(t, np::atan2(y, x)) << "\n";
    return 0;
}
```

Output

```
true
```

### `degrees`

Convert angles from radians to degrees.
```cpp
double degrees(double x);
float degrees(float x);
long double degrees(long double x);
template <class T>
double degrees(T x); // Additional overloads for integral types.
```

Parameters

* `x` Angle in radians.

Returns

* The corresponding angle in degrees.

<h3><code>degrees</code></h3>

Convert angles from radians to degrees, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> degrees(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in radians.

Returns

* A light-weight object with the corresponding angles in degrees. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    const double pi = np::constants<double>::pi;
    np::vector<double> x{0, pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6, pi};
    std::cout << "x:\n" << x << "\n";
    std::cout << "degrees(x):\n" << np::degrees(x) << "\n";
    return 0;
}
```

Output

```
x:
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951, 
  2.3561945,  2.6179939,  3.1415927]
degrees(x):
[  0,  30,  45,  60,  90, 120, 135, 150, 180]
```

### `radians`

Convert angles from degrees to radians.
```cpp
double radians(double x);
float radians(float x);
long double radians(long double x);
template <class T>
double radians(T x); // Additional overloads for integral types.
```

Parameters

* `x` Angle in degrees.

Returns

* The corresponding angle in radians.

<h3><code>radians</code></h3>

Convert angles from degrees to radians, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> radians(const tensor<T, Rank> &x);
```

Parameters

* `x` A tensor-like object with the angles in degrees.

Parameters

* A light-weight object with the corresponding angles in radians. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<double> x{0, 30, 45, 60, 90, 120, 135, 150, 180};
    std::cout << "x:\n" << x << "\n";
    std::cout << "radians(x):\n" << np::radians(x) << "\n";
    return 0;
}
```

Output

```
x:
[  0,  30,  45,  60,  90, 120, 135, 150, 180]
radians(x):
[         0, 0.52359878, 0.78539816,  1.0471976,  1.5707963,  2.0943951, 
  2.3561945,  2.6179939,  3.1415927]
```
