# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Logic functions](#logic-functions)
    - [`all`](#all)
    - [`any`](#any)
    - [`count_nonzero`](#count_nonzero)
    - [`isclose`](#isclose)
    - [`allclose`](#allclose)

## Logic functions

### `all`

Test whether all tensor elements evaluate to true.
```cpp
template <size_t Rank>
bool all(const tensor<bool, Rank> &a);
```

Parameters

* `a` A tensor-like object of `bool`.

Returns

* true if all the elements evaluate to true and false otherwise.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    std::cout << std::boolalpha << np::all(a > 0) << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
true
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
false
```

<h3><code>all</code></h3>

Test whether all tensor elements over the given axes evaluate to true.
```cpp
template <size_t Rank, size_t N>
tensor<bool, Rank - N> all(const tensor<bool, Rank> &a, const shape_t<N> &axes);

template <size_t Rank, size_t N>
tensor<bool, Rank> all(const tensor<bool, Rank> &a, const shape_t<N> &axes,
                       keepdims_t);

template <size_t Rank, size_t N>
tensor<bool, Rank - N> all(const tensor<bool, Rank> &a, const shape_t<N> &axes,
                           dropdims_t);
```

Parameters

* `a` A tensor-like object of `bool`.
* `axes` A `shape_t` object with the axes along which the logical AND reduction is performed.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the results of the test over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, test over all rows (column-wise).
    // For axis=1, test over all columns (row-wise).
    std::cout << std::boolalpha <<  np::all(a > 0, axis) << "\n";
    return 0;
}
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
0
```

Output

```
[false, false,  true, false,  true, false]
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
1
```

Output

```
[ true, false, false, false]
```

### `any`

Test whether any tensor element evaluate to true.
```cpp
template <size_t Rank>
bool any(const tensor<bool, Rank> &a);
```

Parameters

* `a` A tensor-like object of `bool`.

Returns

* true if any element evaluate to true and false otherwise.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    std::cout << std::boolalpha << np::any(a < 0) << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
false
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
true
```

<h3><code>any</code></h3>

Test whether any tensor element over the given axes evaluate to true.
```cpp
template <size_t Rank, size_t N>
tensor<bool, Rank - N> any(const tensor<bool, Rank> &a, const shape_t<N> &axes);

template <size_t Rank, size_t N>
tensor<bool, Rank> any(const tensor<bool, Rank> &a, const shape_t<N> &axes,
                       keepdims_t);

template <size_t Rank, size_t N>
tensor<bool, Rank - N> any(const tensor<bool, Rank> &a, const shape_t<N> &axes,
                           dropdims_t);
```

Parameters

* `a` A tensor-like object of `bool`.
* `axes` A `shape_t` object with the axes along which the logical OR reduction is performed.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the results of the test over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, test over all rows (column-wise).
    // For axis=1, test over all columns (row-wise).
    std::cout << std::boolalpha <<  np::any(a < 0, axis) << "\n";
    return 0;
}
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
0
```

Output

```
[false,  true, false,  true, false,  true]
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
1
```

Output

```
[false,  true,  true,  true]
```

### `count_nonzero`

Count the number of non-zero elements in the tensor.
```cpp
template <class T, size_t Rank>
size_t count_nonzero(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* The number of non-zero elements.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    std::cout << np::count_nonzero(a > 0) << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
10
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
5
```

<h3><code>count_nonzero</code></h3>

Count the number of non-zero elements over the given axes.
```cpp
template <class T, size_t Rank, size_t N>
tensor<size_t, Rank - N> count_nonzero(const tensor<T, Rank> &a,
                                       const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor<size_t, Rank> count_nonzero(const tensor<T, Rank> &a,
                                   const shape_t<N> &axes, keepdims_t);

template <class T, size_t Rank, size_t N>
tensor<size_t, Rank - N> count_nonzero(const tensor<T, Rank> &a,
                                       const shape_t<N> &axes, dropdims_t);
```

Parameters

* `a` A tensor-like object.
* `axes` A `shape_t` object with the axes along which to count non-zeros.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.

Returns

* A new tensor with the number of non-zero elements over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    np::shape_t<1> axis;
    std::cin >> a >> axis;
    // For axis=0, count values over all rows (column-wise).
    // For axis=1, count over all columns (row-wise).
    std::cout <<  np::count_nonzero(a > 0, axis) << "\n";
    return 0;
}
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
0
```

Output

```
[3, 2, 4, 3, 4, 3]
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
1
```

Output

```
[6, 4, 5, 4]
```

### `isclose`

Return whether two numbers are equal within a tolerance.
```cpp
template <class T, class U>
bool isclose(T a, U b, Promoted rtol = 1e-8, Promoted atol = 0.);

// Additional overloads for complex types.
template <class T, class U>
bool isclose(const std::complex<T> &a, const std::complex<U> &b,
             Promoted rtol = 1e-8, Promoted atol = 0);

template <class T, class U>
bool isclose(const std::complex<T> &a, const U &b,
             Promoted rtol = 1e-8, Promoted atol = 0);

template <class T, class U>
bool isclose(const T &a, const std::complex<U> &b,
             Promoted rtol = 1e-8, Promoted atol = 0);
```

Whether or not two values are considered equal is determined according to given absolute and relative tolerance. The tolerances must be non-negative, typically very small numbers. For floating-point values, the function uses the following equation to test whether two numbers are equivalent:

```
    fabs(a - b) <= fmax(rtol * fmax(fabs(a), fabs(b)), atol)
```

NaN is not considered equal to any other value, including NaN. inf and -inf are only considered equal to themselves.

Parameters

* `a` An integer, floating-point or complex value.
* `b` An integer, floating-point or complex value.
* `rtol` The relative tolerance.
* `atol` The absolute tolerance.

Returns

* true if the values are considered equal and false otherwise.

Notes

* If one of the arguments has integral type, it is cast to `double`.
* If one of the arguments is `long double`, the other argument is promoted to `long double`.
* If one of the arguments is `double` and the other argument is `float`, the `float` argument is promoted to `double`.
* For complex types, the equality is tested on both real and imaginary parts.

<h3><code>isclose</code></h3>

Return whether two tensors are equal, element-wise, within a tolerance.
```cpp
template <class T, size_t Rank>
tensor<bool, Rank> isclose(const tensor<T, Rank> &a, const tensor<T, Rank> &b,
                           const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(const tensor<T, Rank> &a,
                           const typename tensor<T, Rank>::value_type &val,
                           const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(const typename tensor<T, Rank>::value_type &val,
                           const tensor<T, Rank> &b,
                           const T &rtol = 1e-8, const T &atol = 0);

// Additional overloads for complex types.
template <class T, size_t Rank>
tensor<bool, Rank> isclose(const tensor<std::complex<T>, Rank> &a,
                           const tensor<std::complex<T>, Rank> &b,
                           const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(const tensor<std::complex<T>, Rank> &a,
                           const std::complex<T> &val,
                           const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
tensor<bool, Rank> isclose(const std::complex<T> &val,
                           const tensor<std::complex<T>, Rank> &b,
                           const T &rtol = 1e-8, const T &atol = 0);
```

Parameters

* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.
* `val` Value to use either as first argument or second argument. Values are broadcasted to an appropriate shape.
* `rtol` The relative tolerance.
* `atol` The absolute tolerance.

Returns

* A light-weight object of `bool` with each element set to true where `a` and `b` are equal within the given tolerance, and false otherwise. This function does not create a new tensor, instead, an expression object is returned with the element-wise comparison.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h> // np::constants
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    const double inf = np::constants<double>::inf;
    const double nan = np::constants<double>::nan;

    np::vector<double> a{       1., 1. + 1e-8, 0.33333333,     1e7,
                               1e8,     1./0.,     -1./0.,   0./0.};
    np::vector<double> b{1. + 1e-7,        1.,      1./3., 1e7 + 1,
                           1e8 + 1,       inf,        inf,     nan};
    std::cout << "a:\n" << a << "\n";
    std::cout << "b:\n" << b << "\n";

    std::cout << std::boolalpha;
    std::cout << "Using isclose:\n" << np::isclose(a, b) << "\n";
    std::cout << "Using == operator:\n" << (a == b) << "\n";
    return 0;
}
```

Output

```
a:
[         1,          1, 0.33333333,   10000000,      1e+08,        inf, 
       -inf,       -nan]
b:
[ 1.0000001,          1, 0.33333333,   10000001,      1e+08,        inf, 
        inf,        nan]
Using isclose:
[false,  true,  true, false,  true,  true, false, false]
Using == operator:
[false, false, false, false, false,  true, false, false]
```

### `allclose`

Test whether two tensors are element-wise equal within a tolerance.
```cpp
template <class T, size_t Rank>
bool allclose(const tensor<T, Rank> &a, const tensor<T, Rank> &b,
              const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
bool allclose(const tensor<T, Rank> &a,
              const typename tensor<T, Rank>::value_type &val,
              const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
bool allclose(const typename tensor<T, Rank>::value_type &val,
              const tensor<T, Rank> &b,
              const T &rtol = 1e-8, const T &atol = 0);

// Additional overloads for complex types.
template <class T, size_t Rank>
bool allclose(const tensor<std::complex<T>, Rank> &a,
              const tensor<std::complex<T>, Rank> &b,
              const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
bool allclose(const tensor<std::complex<T>, Rank> &a,
              const std::complex<T> &val,
              const T &rtol = 1e-8, const T &atol = 0);

template <class T, size_t Rank>
bool allclose(const std::complex<T> &val,
              const tensor<T, Rank> &b,
              const T &rtol = 1e-8, const T &atol = 0);
```

Parameters

* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.
* `val` Value to use either as first argument or second argument. Values are broadcasted to an appropriate shape.
* `rtol` The relative tolerance.
* `atol` The absolute tolerance.

Returns

* true if all the elements are considered equal and false otherwise.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/math.h> // np::constants
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    const double inf = np::constants<double>::inf;

    np::vector<double> a{1. + 1e-8, 0.33333333,     1e8, 1./0., -1./0.};
    np::vector<double> b{       1.,      1./3., 1e8 + 1,   inf,   -inf};
    std::cout << "a:\n" << a << "\n";
    std::cout << "b:\n" << b << "\n";

    std::cout << std::boolalpha;
    std::cout << "Using allclose:\n" << np::allclose(a, b) << "\n";
    std::cout << "Using == operator:\n" << np::all(a == b) << "\n";
    return 0;
}
```

Output

```
a:
[         1, 0.33333333,      1e+08,        inf,       -inf]
b:
[         1, 0.33333333,      1e+08,        inf,       -inf]
Using allclose:
true
Using == operator:
false
```
