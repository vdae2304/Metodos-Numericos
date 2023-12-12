# [Functional programming](readme.md)

Defined in header [`numcpp/functional.h`](/include/numcpp/functional.h)

- [Functional programming](#functional-programming)
  - [Element-wise functions](#element-wise-functions)
    - [`apply`](#apply)
    - [`apply2`](#apply2)

## Element-wise functions

### `apply`

Apply a function element-wise.
```cpp
template <class Function, class T, size_t Rank>
tensor</*Return type*/, Rank> apply(Function &&f, const tensor<T, Rank> &a);
```

Parameters

* `f` The function to apply.
* `a` A tensor-like object with the values where the function will be invoked.

Returns

* A light-weight object which stores the result of invoking the function on each element. This function does not create a new tensor, instead, an expression object is returned. The returned object uses lazy-evaluation, which means that the function is called only when required, i.e., when the whole expression is evaluated or assigned to a tensor object.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;

int square(int x) {
    return x*x;
}

double inverse(int x) {
    return 1.0/x;
}

int main() {
    np::vector<int> x;
    std::cin >> x;
    // Returns an expression equivalent to vector<int>
    std::cout << "square:\n" << np::apply(square, x) << "\n";
    // Returns an expression equivalent to vector<double>
    std::cout << "inverse:\n" << np::apply(inverse, x) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

Output

```
square:
[  1,   4,   9,  16,  25,  36,  49,  64,  81, 100]
inverse:
[         1,        0.5, 0.33333333,       0.25,        0.2, 0.16666667, 
 0.14285714,      0.125, 0.11111111,        0.1]
```

<h3><code>apply</code></h3>

Apply a function element-wise.
```cpp
template <class R, class Function, class T, size_t Rank>
void apply(tensor<R, Rank> &out, Function &&f, const tensor<T, Rank> &a);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply.
* `a` A tensor-like object with the values where the function will be invoked.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `out` does not match the shape of `a`.

### `apply2`

Apply a binary function element-wise.
```cpp
template <class Function, class T, class U, size_t Rank>
tensor</*Result type*/, Rank> apply2(Function &&f, const tensor<T, Rank> &a,
                                     const tensor<U, Rank> &b);

template <class Function, class T, class U, size_t Rank>
tensor</*Result type*/, Rank> apply2(Function &&f, const tensor<T, Rank> &a,
                                     const U &val);

template <class Function, class T, class U, size_t Rank>
tensor</*Result type*/, Rank> apply2(Function &&f, const T &val,
                                     const tensor<U, Rank> &b);
```

Parameters

* `f` The function to apply.
* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.
* `val` Value to use either as first argument or second argument. Values are broadcasted to an appropriate shape.

Returns

* A light-weight object which stores the result of invoking the function on each element. This function does not create a new tensor, instead, an expression object is returned. The returned object uses lazy-evaluation, which means that the function is called only when required, i.e., when the whole expression is evaluated or assigned to a tensor object.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;

double true_divide(int x, int y) {
    return (double)x / (double)y;
}

int integer_pow(int x, int y) {
    if (y == 0) {
        return 1;
    } else if (y % 2 == 1) {
        return x * integer_pow(x, y - 1);
    } else {
        int p = integer_pow(x, y / 2);
        return p * p;
    }
}

int main() {
    np::vector<int> x, y;
    std::cin >> x >> y;
    // Returns an expression equivalent to vector<double>
    std::cout << "true_divide:\n" << np::apply2(true_divide, x, y) << "\n";
    // Returns an expression equivalent to vector<int>
    std::cout << "integer_pow:\n" << np::apply2(integer_pow, x, y) << "\n";
    return 0;
}
```

Input

```
[1, 1, 2, 1, 2, 3, 1, 2, 3, 4]
[2, 3, 3, 4, 4, 4, 5, 5, 5, 5]
```

Output

```
true_divide:
[       0.5, 0.33333333, 0.66666667,       0.25,        0.5,       0.75, 
        0.2,        0.4,        0.6,        0.8]
integer_pow:
[   1,    1,    8,    1,   16,   81,    1,   32,  243, 1024]
```

<h3><code>apply2</code></h3>

Apply a binary function element-wise.
```cpp
template <class R, class Function, class T, class U, size_t Rank>
void apply2(tensor<R, Rank> &out, Function &&f, const tensor<T, Rank> &a,
            const tensor<U, Rank> &b);

template <class R, class Function, class T, class U, size_t Rank>
void apply2(tensor<R, Rank> &out, Function &&f, const tensor<T, Rank> &a,
            const U &val);

template <class R, class Function, class T, class U, size_t Rank>
void apply2(tensor<R, Rank> &out, Function &&f, const T &val,
            const tensor<U, Rank> &b);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply.
* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.
* `val` Value to use either as first argument or second argument. Values are broadcasted to an appropriate shape.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes), or if the shape of `out` does not match the broadcasting shape.
