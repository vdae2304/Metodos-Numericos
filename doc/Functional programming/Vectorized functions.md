# [Functional programming](readme.md)

Defined in header [`numcpp/functional.h`](/include/numcpp/functional.h)

- [Functional programming](#functional-programming)
  - [Vectorized functions](#vectorized-functions)
    - [`vectorize`](#vectorize)
  - [Member functions](#member-functions)
    - [`<vectorized>::target`](#vectorizedtarget)
    - [`<vectorized>::operator()`](#vectorizedoperator)

## Vectorized functions

### `vectorize`

Vectorize a function.

A vectorized function is a wrapper for a function that operates on tensors in an element-by-element fashion, supporting broadcasting.
```cpp
template <class Function>
/*Implementation defined.*/ vectorize(Function &&f);
```

Parameters

* `f` Function to vectorize.

Returns

* A vectorized function that operates on tensors in an element-by-element fashion.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    auto square = np::vectorize([](int x) { return x*x; });
    std::cin >> a;
    std::cout << "Call on single element:\n";
    std::cout << square(10) << "\n";
    std::cout << "Call on a vector:\n";
    std::cout << square(a) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

Output

```
Call on single element:
100
Call on a vector:
[  1,   4,   9,  16,  25,  36,  49,  64,  81, 100]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a, b;
    auto true_divide = np::vectorize([](int x, int y) {
        return (double)x / (double) y;
    });
    std::cin >> a >> b;
    std::cout << "Call on single elements:\n";
    std::cout << true_divide(1, 2) << "\n";
    std::cout << "Call with both arguments as vectors:\n";
    std::cout << true_divide(a, b) << "\n";
    std::cout << "Call with first argument being a vector:\n";
    std::cout << true_divide(a, 1) << "\n";
    std::cout << "Call with second argument being a vector:\n";
    std::cout << true_divide(1, b) << "\n";
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
Call on single elements:
0.5
Call with both arguments as vectors:
[       0.5, 0.33333333, 0.66666667,       0.25,        0.5,       0.75, 
        0.2,        0.4,        0.6,        0.8]
Call with first argument being a vector:
[1, 1, 2, 1, 2, 3, 1, 2, 3, 4]
Call with second argument being a vector:
[       0.5, 0.33333333, 0.33333333,       0.25,       0.25,       0.25, 
        0.2,        0.2,        0.2,        0.2]
```

## Member functions

### `<vectorized>::target`

Return a pointer to the stored function.
```cpp
Function *target();
const Function *target() const;
```

### `<vectorized>::operator()`

Invokes the function.
```cpp
template <class... T>
/*Return type*/ operator()(T &&...args);
```

Parameters

* `args...` Parameters to pass to the stored function.

Returns

* The return value of the invocation.

<h3><code>&lt;vectorized&gt;::operator()</code></h3>

Apply the function element-wise.
```cpp
template <class T, size_t Rank>
tensor</*Return type*/, Rank> operator()(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object with the values where the function will be invoked.

Returns

* A light-weight object which stores the result of invoking the function on each element. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

<h3><code>&lt;vectorized&gt;::operator()</code></h3>

Apply the function element-wise.
```cpp
template <class T, class class U, size_t Rank>
tensor</*Return type*/, Rank> operator()(const tensor<T, Rank> &a,
                                         const tensor<U, Rank> &b);

template <class T, class U, size_t Rank>
tensor</*Return type*/, Rank> operator()(const tensor<T, Rank> &a,
                                         const U &val);

template <class T, class U, size_t Rank>
tensor</*Return type*/, Rank> operator()(const T &val,
                                         const tensor<U, Rank> &b);
```

Parameters

* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.
* `val` Value to use either as first argument or second argument. Values are broadcasted to an appropriate shape.

Returns

* A light-weight object which stores the result of invoking the function on each element. This function does not create a new tensor, instead, an expression object is returned (see lazy-evalaution).

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).

<h3><code>&lt;vectorized&gt;::operator()</code> (Since C++14)</h3>

Apply the function element-wise.
```cpp
template <class T, class... U, size_t Rank>
tensor</*Return type*/, Rank> operator()(const tensor<T, Rank> &a,
                                         const tensor<U, Rank> &...b);
```

Parameters

* `a, b...` Tensor-like objects with the values to pass. Since C++14, this function accepts any number of arguments.

Returns

* A light-weight object which stores the result of invoking the function on each element. This function does not create a new tensor, instead, an expression object is returned (see lazy-evalaution).

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).