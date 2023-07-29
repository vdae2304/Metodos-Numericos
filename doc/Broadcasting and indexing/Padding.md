# [Broadcasting and indexing](readme.md)

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Padding](#padding)
    - [`pad`](#pad)
  - [Padding modes](#padding-modes)
    - [`pad_mode::constant`](#pad_modeconstant)
    - [`pad_mode::edge`](#pad_modeedge)
    - [`pad_mode::linear_ramp`](#pad_modelinear_ramp)
    - [`pad_mode::reflect`](#pad_modereflect)
    - [`pad_mode::symetric`](#pad_modesymetric)
    - [`pad_mode::wrap`](#pad_modewrap)

## Padding

### `pad`

Pad a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> pad(const tensor<T, Rank> &a, const shape_t<Rank> &before,
                    const shape_t<Rank> &after);
```

Parameters

* `a` The tensor to pad.
* `before` Number of elements to pad at the beginning of each axis.
* `after` Number of elements to pad at the end of each axis.

Returns

* The padded tensor. Padded elements are left uninitialized.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::shape_t<1> before, after;
    std::cin >> vec >> before >> after;
    std::cout << np::pad(vec, before, after) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Possible output

```
[524229392,       306, 524222800,         1,         2,         3,         4,
         5,         0,        -1]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> before, after;
    std::cin >> mat >> before >> after;
    std::cout << np::pad(mat, before, after) << "\n";
    return 0;
}
```

Input

```
[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Possible output

```
[[-756127952,        522, -756154032,        522,         -1,          6],
 [         0,          8,          9,         10, 1375731794,       2823],
 [-756127952,          1,          2,          3,          4,         -1],
 [1342177360,          5,          6,          7,          8,        522],
 [1163151688,          9,         10,         11,         12, 1330860869],
 [1145659218, 1230261829, 1380272454, 1953384765,  875981925, 1835091488],
 [ 544828521, 1867325494,  543974756,  540554545, 1885697107, 1735289200]]
```

<h3><code>pad</code></h3>

Pad a tensor.
```cpp
template <class T, size_t Rank, class Function, class... Args>
tensor<T, Rank> pad(const tensor<T, Rank> &a, const shape_t<Rank> &before,
                    const shape_t<Rank> &after, Function func, Args &&...args);
```

Parameters

* `a` The tensor to pad.
* `before` Number of elements to pad at the beginning of each axis.
* `after` Number of elements to pad at the end of each axis.
* `func` Padding function. For tensors with rank greater than 1, the padding of later axes depends on the padding of previous axes. The signature of the padding function should be equivalent to the following:
    ```cpp
    void func(tensor_view<T, 1> &view, size_t before, size_t after, size_t axis, 
              Args&&... args);
    ```
    where
    - `view` A 1-dimensional view already padded with uninitialized values.
    - `before` Number of elements padded at the beginning of `view`.
    - `after` Number of elements padded at the end of `view`.
    - `axis` The axis currently being calculated.
* `args...` Additional arguments to pass to `func`.

Returns

* The padded tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;

void zero_pad(np::vector_view<int> &view, size_t before, size_t after, size_t) {
    for (size_t i = 0; i < before; ++i) {
        view[i] = 0;
    }
    for (size_t i = view.size() - after; i < view.size(); ++i) {
        view[i] = 0;
    }
}

int main() {
    np::vector<int> vec;
    np::shape_t<1> before, after;
    std::cin >> vec >> before >> after;
    std::cout << np::pad(vec, before, after, zero_pad) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Output

```
[0, 0, 0, 1, 2, 3, 4, 5, 0, 0]
```

## Padding modes

### `pad_mode::constant`

Pads with a constant value.
```cpp
namespace pad_mode {
struct constant;
}
```

Parameters

* `args...` The values to set the padded elements for each axis.
  * If no arguments are passed, all the axes are padded with zeros.
  * If a single value is passed, the same constant is used for all the axes.
  * If two values are passed, the same before and after constants are used for each axis.
  * If a matrix of values is passed, unique pad constants are used for each axis, where `values[{i, 0}]` and `values[{i, 1}]` are the before and after constants for axis `i`.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::shape_t<1> before, after;
    np::pad_mode::constant mode;
    std::cin >> vec >> before >> after;
    std::cout << "Pad with zeros:\n";
    std::cout << np::pad(vec, before, after, mode) << "\n";
    std::cout << "Pad with ones:\n";
    std::cout << np::pad(vec, before, after, mode, 1) << "\n";
    std::cout << "Pad with zeros and ones:\n";
    std::cout << np::pad(vec, before, after, mode, 0, 1) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Output

```
Pad with zeros:
[0, 0, 0, 1, 2, 3, 4, 5, 0, 0]
Pad with ones:
[1, 1, 1, 1, 2, 3, 4, 5, 1, 1]
Pad with zeros and ones:
[0, 0, 0, 1, 2, 3, 4, 5, 1, 1]
```

### `pad_mode::edge`

Pads with the edge values of the tensor.
```cpp
namespace pad_mode {
struct edge;
}
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::shape_t<1> before, after;
    np::pad_mode::edge mode;
    std::cin >> vec >> before >> after;
    std::cout << "Pad with edge values:\n";
    std::cout << np::pad(vec, before, after, mode) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Output

```
Pad with edge values:
[1, 1, 1, 1, 2, 3, 4, 5, 5, 5]
```

### `pad_mode::linear_ramp`

Pads with the linear ramp between an end value and the tensor edge value.
```cpp
namespace pad_mode {
struct linear_ramp;
}
```

Parameters

* `args...` The values used for the ending values of the linear ramp.
  * If no arguments are passed, all the end values are set to zero.
  * If a single value is passed, the same value is used for all the axes.
  * If two values are passed, the same before and after end values are used for each axis.
  * If a matrix of values is passed, unique end values are used for each axis, where `values[{i, 0}]` and `values[{i, 1}]` are the before and after end values for axis `i`.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> vec;
    np::shape_t<1> before, after;
    np::pad_mode::linear_ramp mode;
    std::cin >> vec >> before >> after;
    std::cout << "Pad with linear ramp towards zero:\n";
    std::cout << np::pad(vec, before, after, mode) << "\n";
    std::cout << "Pad with linear ramp towards one:\n";
    std::cout << np::pad(vec, before, after, mode, 1) << "\n";
    std::cout << "Pad with linear ramp towards zero and one:\n";
    std::cout << np::pad(vec, before, after, mode, 0, 1) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Output

```
Pad with linear ramp towards zero:
[         0, 0.33333333, 0.66666667,          1,          2,          3, 
          4,          5,        2.5,          0]
Pad with linear ramp towards one:
[1, 1, 1, 1, 2, 3, 4, 5, 3, 1]
Pad with linear ramp towards zero and one:
[         0, 0.33333333, 0.66666667,          1,          2,          3, 
          4,          5,          3,          1]
```

### `pad_mode::reflect`

Pads with the reflection of the tensor mirrored on the first and last values.
```cpp
namespace pad_mode {
struct reflect;
}
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::shape_t<1> before, after;
    np::pad_mode::reflect mode;
    std::cin >> vec >> before >> after;
    std::cout << "Pad reflect:\n";
    std::cout << np::pad(vec, before, after, mode) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Output

```
Pad reflect:
[4, 3, 2, 1, 2, 3, 4, 5, 4, 3]
```

### `pad_mode::symetric`

Pads with the reflection of the tensor mirrored along the edge.
```cpp
namespace pad_mode {
struct symmetric;
}
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::shape_t<1> before, after;
    np::pad_mode::symmetric mode;
    std::cin >> vec >> before >> after;
    std::cout << "Pad symmetric:\n";
    std::cout << np::pad(vec, before, after, mode) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Output

```
Pad symmetric:
[3, 2, 1, 1, 2, 3, 4, 5, 5, 4]
```

### `pad_mode::wrap`

Pads with the wrap of the tensor along the axis. The first values are used to pad the end and the end values are used to pad the beginning.
```cpp
namespace pad_mode {
struct wrap;
}
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    np::shape_t<1> before, after;
    np::pad_mode::wrap mode;
    std::cin >> vec >> before >> after;
    std::cout << "Pad wrap:\n";
    std::cout << np::pad(vec, before, after, mode) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2
```

Output

```
Pad wrap:
[3, 4, 5, 1, 2, 3, 4, 5, 1, 2]
```
