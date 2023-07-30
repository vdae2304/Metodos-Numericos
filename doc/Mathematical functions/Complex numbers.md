# [Mathematical functions](readme.md)

Defined in header [`numcpp/math.h`](/include/numcpp/math.h)

- [Mathematical functions](#mathematical-functions)
  - [Complex numbers](#complex-numbers)
    - [`real`](#real)
    - [`imag`](#imag)
    - [`conj`](#conj)
    - [`abs`](#abs)
    - [`arg`](#arg)

## Complex numbers

### `real`

Return the real part, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> real(const tensor<std::complex<T>, Rank> &z);

template <class T, size_t Rank>
tensor<T, Rank> real(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with complex values.

Returns

* A light-weight object with the real part of each element in the tensor. Non-complex types are treated as complex numbers with zero imaginary part component. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << np::real(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[ 8, 12,  8,  5,  1, 12,  0, -1,  6,  0]
```

### `imag`

Return the imaginary part, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> imag(const tensor<std::complex<T>, Rank> &z);

template <class T, size_t Rank>
tensor<T, Rank> imag(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with complex values.

Returns

* A light-weight object with the imaginary part of each element in the tensor. Non-complex types are treated as complex numbers with zero imaginary part component. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << np::imag(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[-1, -3,  9,  0,  2,  1, -3,  9,  0, 10]
```

### `conj`

Return the complex conjugate, element-wise. The conjugate of a complex number is obtained by changing the sign of its imaginary part.
```cpp
template <class T, size_t Rank>
tensor<std::complex<T>, Rank> conj(const tensor<std::complex<T>, Rank> &z);

template <class T, size_t Rank>
tensor<std::complex<T>, Rank> conj(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with the values whose complex conjugate is computed.

Returns

* A light-weight object with the complex conjugate of each element in the tensor. Non-complex types are treated as complex numbers with zero imaginary part component. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << np::conj(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[ 8+1i, 12+3i,  8-9i,  5-0i,  1-2i, 12-1i,  0+3i, -1-9i,  6-0i, 0-10i]
```

### `abs`

Return the absolute value, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> abs(const tensor<std::complex<T>, Rank> &z);
```

Parameters

* `z` A tensor-like object with the values whose absolute value is computed.

Returns

* A light-weight object with the absolute value of each element in the tensor. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << np::abs(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[8.0622577, 12.369317, 12.041595,         5,  2.236068, 12.041595,         3, 
 9.0553851,         6,        10]
```

### `arg`

Return the phase angle (in radians) of a complex number, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> arg(const tensor<std::complex<T>, Rank> &z);

template <class T, size_t Rank>
tensor<T, Rank> arg(const tensor<T, Rank> &z);
```

Parameters

* `z` A tensor-like object with the values whose phase angle is computed.

Returns

* A light-weight object with the phase angle of each element in the tensor. Non-complex types are treated as complex numbers with zero imaginary part component. This function does not create a new tensor, instead, an expression object is returned (see [lazy-evaluation](/doc/Tensor%20class/Tensor/Operators.md)).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << np::arg(z) << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[-0.12435499, -0.24497866,  0.84415399,           0,   1.1071487, 0.083141232, 
  -1.5707963,   1.6814535,           0,   1.5707963]
```
