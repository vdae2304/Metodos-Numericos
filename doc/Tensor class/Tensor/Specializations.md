# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Specializations (`tensor<std::complex<T>>`)](#specializations-tensorstdcomplext)
    - [`tensor::real`](#tensorreal)
    - [`tensor::real`](#tensorreal-1)
    - [`tensor::imag`](#tensorimag)
    - [`tensor::imag`](#tensorimag-1)
    - [`tensor::conj`](#tensorconj)

## Specializations (`tensor<std::complex<T>>`)

### `tensor::real`

Return the real part, element-wise.
```cpp
tensor<T, Rank> real() const;
```

Parameters

* None

Returns

* A light-weight object with the real part of each element in the tensor. This function does not create a new tensor, instead, it returns a readonly view with the real part of each element.

Example

```cpp
#include <complex>
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << z.real() << "\n";
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

### `tensor::real`

Set the real part, element-wise.
```cpp
void real(const tensor<T, Rank> &x);
void real(const T &val);
```

Parameters

* `x` A tensor-like object with the values to set the real part to.
* `val` Value to set the real part to.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to broadcasting rules.

Example

```cpp
#include <complex>
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<double> x;
    std::cin >> x;
    np::vector<std::complex<double>> z(x.size(), 0);
    z.real(x);
    std::cout << z << "\n";
    return 0;
}
```

Output

```
[ 8, 12,  8,  5,  1, 12,  0, -1,  6,  0]
```

Output

```
[ 8+0i, 12+0i,  8+0i,  5+0i,  1+0i, 12+0i,  0+0i, -1+0i,  6+0i,  0+0i]
```

### `tensor::imag`

Return the imaginary part, element-wise.
```cpp
tensor<T, Rank> imag() const;
```

Parameters

* None

Returns

* A light-weight object with the imaginary part of each element in the tensor. This function does not create a new tensor, instead, it returns a readonly view with the imaginary part of each element.

Example

```cpp
#include <complex>
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << z.imag() << "\n";
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

### `tensor::imag`

Set the imaginary part, element-wise.
```cpp
void imag(const tensor<T, Rank> &y);
void imag(const T &val);
```

Parameters

* `y` A tensor-like object with the values to set the imaginary part to.
* `val` Value to set the imaginary part to.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to broadcasting rules.

Example

```cpp
#include <complex>
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<double> y;
    std::cin >> y;
    np::vector<std::complex<double>> z(y.size(), 0);
    z.imag(y);
    std::cout << z << "\n";
    return 0;
}
```

Output

```
[-1, -3,  9,  0,  2,  1, -3,  9,  0, 10]
```

Output

```
[ 0-1i,  0-3i,  0+9i,  0+0i,  0+2i,  0+1i,  0-3i,  0+9i,  0+0i, 0+10i]
```

### `tensor::conj`

Return the complex conjugate, element-wise.
```cpp
tensor<std::complex<T>, Rank> conj() const;
```

Parameters

* None

Returns

* A light-weight object with the complex conjugate of each element in the tensor. This function does not create a new tensor, instead, it returns an expression object with the complex conjugate of each element.

Example

```cpp
#include <complex>
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = arithmetic;
    np::vector<std::complex<double>> z;
    std::cin >> z;
    std::cout << z.conj() << "\n";
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
