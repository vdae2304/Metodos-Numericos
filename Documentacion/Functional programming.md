# Functional programming

Defined in `numcpp/functional.h`

- [Functional programming](#functional-programming)
    - [`apply(f, arg)`](#applyf-arg)
    - [`apply(f, arg1, arg2)`](#applyf-arg1-arg2)
    - [`apply_along_axis`](#apply_along_axis)
    - [`apply_over_axes`](#apply_over_axes)
    - [`reduce`](#reduce)
    - [`reduce(axes)`](#reduceaxes)
    - [`accumulate(axis)`](#accumulateaxis)
    - [`outer`](#outer)

### `apply(f, arg)`

Apply a function element-wise.
```cpp
// Before C++17
template <class Function, class T, size_t Rank>
tensor<typename std::result_of<Function(T)>::type, Rank> apply(
    Function &&f, const tensor<T, Rank> &arg
);

// Since C++17
template <class Function, class T, size_t Rank>
tensor<typename std::invoke_result<Function, T>::type, Rank> apply(
    Function &&f, const tensor<T, Rank> &arg
);
```

Parameters

* `f` The function to apply.
* `arg` A tensor-like object with the values where the function will be invoked.

Returns

* A light-weight object which stores the result of invoking the function on
each element. This function does not create a new tensor object, instead, an
expression object is returned. The returned object uses lazy-evaluation, which
means that the function is called only when required, i.e., when the whole
expression is evaluated or assigned to a tensor object.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

int square(int x) {
    return x*x;
}

double half(int x) {
    return 0.5*x;
}

struct multiply_by {
    int n;

    multiply_by(int n = 1) : n(n) {}

    int operator()(int x) const {
        return n*x;
    }
};

int main() {
    np::array<int> x;
    std::cin >> x;
    // Returns an expression equivalent to array<int>
    std::cout << "square:\n" << np::apply(square, x) << "\n";
    // Returns an expression equivalent to array<double>
    std::cout << "half:\n" << np::apply(half, x) << "\n";
    // Returns an expression equivalent to array<int>. Unlike function pointers,
    // functors allows to contain internal state of the function.
    std::cout << "multiply_by_2:\n" << np::apply(multiply_by(2), x) << "\n";
    return 0;
}
```

Input

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

Output

```
square:
[ 0,  1,  4,  9, 16, 25, 36, 49, 64, 81]
half:
[  0, 0.5,   1, 1.5,   2, 2.5,   3, 3.5,   4, 4.5]
multiply_by_2:
[ 0,  2,  4,  6,  8, 10, 12, 14, 16, 18]
```

<h3><code>apply(out, f, arg)</code></h3>

Apply a function element-wise.
```cpp
template <class R, size_t Rank, class Function, class T>
void apply(tensor<R, Rank> &out, Function &&f, const tensor<T, Rank> &arg);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply.
* `arg` A tensor-like object with the values where the function will be invoked.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `out` does not match the shape
of `arg` and cannot be resized.

### `apply(f, arg1, arg2)`

Apply a function element-wise.
```cpp
// Before C++17
template <class Function, size_t Rank, class T, class U>
tensor<typename std::result_of<Function(T, U)>::type, Rank> apply(
    Function &&f, const tensor<T, Rank> &arg1, const tensor<U, Rank> &arg2
);

template <class Function, size_t Rank, class T, class U>
tensor<typename std::result_of<Function(T, U)>::type, Rank> apply(
    Function &&f, const tensor<T, Rank> &arg1, const U &val
);

template <class Function, size_t Rank, class T, class U>
tensor<typename std::result_of<Function(T, U)>::type, Rank> apply(
    Function &&f, const T &val, const tensor<U, Rank> &arg2
);

// Since C++17
template <class Function, size_t Rank, class T, class U>
tensor<typename std::invoke_result<Function, T, U>::type, Rank> apply(
    Function &&f, const tensor<T, Rank> &arg1, const tensor<U, Rank> &arg2
);

template <class Function, size_t Rank, class T, class U>
tensor<typename std::invoke_result<Function, T, U>::type, Rank> apply(
    Function &&f, const tensor<T, Rank> &arg1, const U &val
);

template <class Function, size_t Rank, class T, class U>
tensor<typename std::invoke_result<Function, T, U>::type, Rank> apply(
    Function &&f, const T &val, const tensor<U, Rank> &arg2
);
```

Parameters

* `f` The function to apply.
* `arg1` First tensor-like object.
* `arg2` Second tensor-like object.
* `val` Value to use either as first argument or second argument. Values are
broadcasted to an appropriate size.

Returns

* A light-weight object which stores the result of invoking the function on
each element. This function does not create a new tensor object, instead, an
expression object is returned. The returned object uses lazy-evaluation, which
means that the function is called only when required, i.e., when the whole
expression is evaluated or assigned to a tensor object.

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

double true_divide(int x, int y) {
    return (double)x / (double)y;
}

int integer_pow(int x, int y) {
    if (y == 0) return 1;
    if (y % 2 == 1) return x * integer_pow(x, y - 1);
    int p = integer_pow(x, y / 2);
    return p * p;
}

struct linear_combination {
    int a, b;

    linear_combination(int a = 1, int b = 1) : a(a), b(b) {}

    int operator()(int x, int y) const {
        return a*x + b*y;
    }
};

int main() {
    np::array<int> x, y;
    std::cin >> x >> y;
    // Returns an expression equivalent to array<double>
    std::cout << "true_divide:\n" << np::apply(true_divide, x, y) << "\n";
    // Returns an expression equivalent to array<int>
    std::cout << "integer_pow:\n" << np::apply(integer_pow, x, y) << "\n";
    // Returns an expression equivalent to array<int>. Unlike function pointers,
    // functors allows to contain internal state of the function.
    std::cout << "linear_combination:\n"
              << np::apply(linear_combination(2, 3), x, y) << "\n";
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
linear_combination:
[ 8, 11, 13, 14, 16, 18, 17, 19, 21, 23]
```

Input

```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
[2]
```

Output

```
true_divide:
[0.5,   1, 1.5,   2, 2.5,   3, 3.5,   4, 4.5,   5]
integer_pow:
[  1,   4,   9,  16,  25,  36,  49,  64,  81, 100]
linear_combination:
[ 8, 10, 12, 14, 16, 18, 20, 22, 24, 26]
```

Input

```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
[2, 3]
```

Output

```
true_divide:
terminate called after throwing an instance of 'std::invalid_argument'
  what():  operands could not be broadcast together with shapes (10,) (2,)
```

<h3><code>apply(out, f, arg1, arg2)</code></h3>

Apply a function element-wise.
```cpp
template <class R, size_t Rank, class Function, class T, class U>
void apply(
    tensor<R, Rank> &out,
    Function &&f, const tensor<T, Rank> &arg1, const tensor<U, Rank> &arg2
);

template <class R, size_t Rank, class Function, class T, class U>
void apply(
    tensor<R, Rank> &out,
    Function &&f, const tensor<T, Rank> &arg1, const U &val
);

template <class R, size_t Rank, class Function, class T, class U>
void apply(
    tensor<R, Rank> &out,
    Function &&f, const T &val, const tensor<U, Rank> &arg2
);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply.
* `arg1` First tensor-like object.
* `arg2` Second tensor-like object.
* `val` Value to use either as first argument or second argument. Values are
broadcasted to an appropriate size.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be
broadcasted according to broadcasting rules, or if the shape of `out` does not
match the broadcasting shape.

### `apply_along_axis`
### `apply_over_axes`

Apply a function over multiple axes.
```cpp
// Before C++17
template <class Function, class T, size_t Rank>
tensor<typename std::result_of<Function(/*iterator*/, /*iterator*/)>::type,
       Rank>
apply_along_axis(Function &&f, const tensor<T, Rank> &arg, size_t axis);

// Since C++17
template <class Function, class T, size_t Rank>
tensor<typename std::invoke_result<Function, /*iterator*/, /*iterator*/>::type,
       Rank>
apply_along_axis(Function &&f, const tensor<T, Rank> &arg, size_t axis);

// Before C++17
template <class Function, class T, size_t Rank, size_t N>
tensor<typename std::result_of<Function(/*iterator*/, /*iterator*/)>::type,
       Rank>
apply_over_axes(
    Function &&f, const tensor<T, Rank> &arg, const shape_t<N> &axes
);

// Since C++17
template <class Function, class T, size_t Rank, size_t N>
tensor<typename std::invoke_result<Function, /*iterator*/, /*iterator*/>::type,
       Rank>
apply_over_axes(
    Function &&f, const tensor<T, Rank> &arg, const shape_t<N> &axes
);
```

Parameters

* `f` The function to apply. A function that accepts two iterators as arguments
and returns a value. The iterators indicates the initial and final positions of
the flattened dimensions. The range used is [`first`, `last`), which includes
all the elements between `first` and `last`, including the element pointed by
`first` but not the element pointed by `last`.
* `arg` A tensor-like object with the values where the function will be invoked.
* `axes` Axis or axes over which the function is applied.

Returns

* A new tensor with the result of invoking the function over the given axes.
The output tensor has the same dimension as `arg`, but the axes over which the
function is applied are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include <algorithm> // std::max_element, std::min_element
#include <iterator> // std::iterator_traits
#include "numcpp.h"
namespace np = numcpp;

struct minmax_difference {
    // Return the difference between the minimum and maximum values in the
    // range.
    template <class InputIterator>
    typename std::iterator_traits<InputIterator>::value_type operator()(
        InputIterator first, InputIterator last
    ) const {
        return *std::max_element(first, last) - *std::min_element(first, last);
    }
};

int main() {
    np::matrix<int> mat;
    std::cin >> mat;
    minmax_difference f;
    std::cout << "Axis 0:\n" << np::apply_along_axis(f, mat, 0) << "\n";
    std::cout << "Axis 1:\n" << np::apply_along_axis(f, mat, 1) << "\n";
    std::cout << "Axes 0, 1:\n"
              << np::apply_over_axes(f, mat, np::make_shape(0, 1)) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
```

Output

```
Axis 0:
[[10, 12,  4,  6, 12, 17]]
Axis 1:
[[11],
 [13],
 [16],
 [17]]
Axes 0, 1:
[[19]]
```

<h3><code>apply_along_axis</code></h3>

<h3><code>apply_over_axes</code></h3>

Apply a function over multiple axes.
```cpp
template <class R, size_t Rank, class Function, class T>
void apply_along_axis(
    tensor<R, Rank> &out,
    Function &&f, const tensor<T, Rank> &arg, size_t axis
);

template <class R, size_t Rank, class Function, class T, size_t N>
void apply_over_axes(
    tensor<R, Rank> &out,
    Function &&f, const tensor<T, Rank> &arg, const shape_t<N> &axes
);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply. A function that accepts two iterators as arguments
and returns a value. The iterators indicates the initial and final positions of
the flattened dimensions. The range used is [`first`, `last`), which includes
all the elements between `first` and `last`, including the element pointed by
`first` but not the element pointed by `last`.
* `arg` A tensor-like object with the values where the function will be invoked.
* `axes` Axis or axes over which the function is applied.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `out` does not match the
reduction shape and cannot be resized.

### `reduce`

Reduce the tensor's dimension by applying a function to all elements.
```cpp
template <class Function, class T, size_t Rank>
typename tensor<T, Rank>::value_type reduce(
    Function &&f, const tensor<T, Rank> &arg
);
```

Parameters

* `f` The function to apply. A binary function taking the current accumulated
value as first argument and an element in the tensor as second argument, and
returning a value.
* `arg` A tensor-like object.

Returns

* The result of accumulating all the elements in the tensor.

Note

* The initial value for the reduction is `f.identity`. If the function does not
have identity, the first element in the tensor is used, and a
`std::invalid_argument` exception is thrown if the tensor is empty.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

int max(int a, int b) {
    return b < a ? a : b;
}

struct add {
    int identity = 0;

    int operator()(int a, int b) const {
        return a + b;
    }
};

struct multiply {
    int identity = 1;

    int operator()(int a, int b) const {
        return a * b;
    }
};

int main() {
    np::array<int> arr;
    std::cin >> arr;
    // Function pointer (no identity).
    try {
        std::cout << "reduce(max): " << np::reduce(max, arr) << "\n";
    } catch(std::invalid_argument &error) {
        std::cout << error.what() << "\n";
    }
    // Functor with identity.
    std::cout << "reduce(add): " << np::reduce(add(), arr) << "\n";
    std::cout << "reduce(multiply): " << np::reduce(multiply(), arr) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
reduce(max): 15
reduce(add): 44
reduce(multiply): 86400
```

Input

```
[]
```

Output

```
reduce(max): attempt to call reduce on an empty sequence with no identity
reduce(add): 0
reduce(multiply): 1
```

### `reduce(axes)`

Reduce the tensor's dimension by applying a function over multiple axis.
```cpp
template <class Function, class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> reduce(
    Function &&f, const tensor<T, Rank> &arg, size_t axis
);

template <class Function, class T, size_t Rank, size_t N>
tensor<typename tensor<T, Rank>::value_type, Rank> reduce(
    Function &&f, const tensor<T, Rank> &arg, const shape_t<N> &axes
);
```

Parameters

* `f` The function to apply. A binary function taking the current accumulated
value as first argument and an element in the tensor as second argument, and
returning a value.
* `arg` A tensor-like object.
* `axes` Axis or axes along which the reduction is performed.

Returns

* A new tensor with the result of performing the reduction over the given axes.
The output tensor has the same dimension as `arg`, but the axes which are
reduced are left as dimensions of size one.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

int max(int a, int b) {
    return b < a ? a : b;
}

struct add {
    int identity = 0;

    int operator()(int a, int b) const {
        return a + b;
    }
};

struct multiply {
    int identity = 1;

    int operator()(int a, int b) const {
        return a * b;
    }
};

int main() {
    np::matrix<int> mat;
    std::cin >> mat;

    std::cout << "reduce(max):\n";
    std::cout << "Axis 0:\n" << np::reduce(max, mat, 0) << "\n";
    std::cout << "Axis 1:\n" << np::reduce(max, mat, 1) << "\n";

    std::cout << "reduce(add):\n";
    std::cout << "Axis 0:\n" << np::reduce(add(), mat, 0) << "\n";
    std::cout << "Axis 1:\n" << np::reduce(add(), mat, 1) << "\n";

    std::cout << "reduce(multiply):\n";
    std::cout << "Axis 0:\n" << np::reduce(multiply(), mat, 0) << "\n";
    std::cout << "Axis 1:\n" << np::reduce(multiply(), mat, 1) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
reduce(max):
Axis 0:
[[ 8,  9,  9,  7,  7, 10]]
Axis 1:
[[ 9],
 [ 9],
 [ 7],
 [10]]
reduce(add):
Axis 0:
[[20, 15, 21, 23, 18, 32]]
Axis 1:
[[34],
 [33],
 [25],
 [37]]
reduce(multiply):
Axis 0:
[[ 336,   54,  450, 1050,  315, 3780]]
Axis 1:
[[19440],
 [13230],
 [ 1470],
 [27000]]
```

<h3><code>reduce(axes)</code></h3>

Reduce the tensor's dimension by applying a function over multiple axis.
```cpp
template <class R, size_t Rank, class Function, class T>
void reduce(
    tensor<R, Rank> &out,
    Function &&f, const tensor<T, Rank> &arg, size_t axis
);

template <class R, size_t Rank, class Function, class T, size_t N>
void reduce(
    tensor<R, Rank> &out,
    Function &&f, const tensor<T, Rank> &arg, const shape_t<N> &axes
);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply. A binary function taking the current accumulated
value as first argument and an element in the tensor as second argument, and
returning a value.
* `arg` A tensor-like object.
* `axes` Axis or axes along which the reduction is performed.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `out` does not match the
reduction shape and cannot be resized.

### `accumulate(axis)`

Accumulate the result of applying a function to all elements.
```cpp
template <class Function, class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> accumulate(
    Function &&f, const tensor<T, Rank> &arg, size_t axis = 0
);
```

Parameters

* `f` The function to apply. A binary function taking the current accumulated
value as first argument and an element in the tensor as second argument, and
returning a value.
* `arg` A tensor-like object.
* `axis` Axis along which to apply the accumulation. Default is zero.

Returns

* A new tensor with the accumulated values along the given axis. The output
tensor has the same dimension and the same shape as `arg`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

int add(int a, int b) {
    return a + b;
}

int main() {
    np::array<int> arr;
    std::cin >> arr;
    std::cout << "1-dimensional:\n";
    std::cout << np::accumulate(add, arr) << "\n";
    np::matrix<int> mat;
    std::cin >> mat;
    std::cout << "2-dimensional:\n";
    std::cout << "Axis 0:\n" << np::accumulate(add, mat, 0) << "\n";
    std::cout << "Axis 1:\n" << np::accumulate(add, mat, 1) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
1-dimensional:
[ 4,  7, 15, 16, 31, 34, 39, 40, 42, 44]
2-dimensional:
Axis 0:
[[ 8,  3,  9,  5,  3,  6],
 [15,  5, 14, 12,  6, 15],
 [18,  6, 16, 17, 13, 22],
 [20, 15, 21, 23, 18, 32]]
Axis 1:
[[ 8, 11, 20, 25, 28, 34],
 [ 7,  9, 14, 21, 24, 33],
 [ 3,  4,  6, 11, 18, 25],
 [ 2, 11, 16, 22, 27, 37]]
```

<h3><code>accumulate(axis)</code></h3>

Accumulate the result of applying a function to all elements.
```cpp
template <class R, size_t Rank, class Function, class T>
void accumulate(
    base_tensor<R, Rank> &out,
    Function &&f, const tensor<T, Rank> &arg, size_t axis = 0
);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply. A binary function taking the current accumulated
value as first argument and an element in the tensor as second argument, and
returning a value.
* `arg` A tensor-like object.
* `axis` Axis along which to apply the accumulation. Default is zero.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `out` does not match the shape
of `arg` and cannot be resized.

### `outer`

Apply a function to all pairs $(a, b)$ with $a$ in `arg1` and $b$ in `arg2`.

Let `M = arg1.ndim()` and `N = arg2.ndim()`. The result of
`outer(f, arg1, arg2)` is a tensor of dimension `M + N` such that

$$out(i_0, ..., i_{M-1}, j_0, ..., j_{N-1})
    = f(arg1(i_0, ..., i_{M-1}), arg2(j_0, ..., j_{N-1}))$$

```cpp
// Before C++17
template <class Function, class T, size_t M, class U, size_t N>
tensor<typename std::result_of<Function(T, U)>::type, M + N> outer(
    Function &&f, const tensor<T, M> &arg1, const tensor<U, N> &arg2
);

// Since C++17
template <class Function, class T, size_t M, class U, size_t N>
tensor<typename std::invoke_result<Function, T, U>::type, M + N> outer(
    Function &&f, const tensor<T, M> &arg1, const tensor<U, N> &arg2
);
```

Parameters

* `f` The function to apply.
* `arg1` First tensor-like object.
* `arg2` Second tensor-like object.

Returns

* A light-weight object which stores the result of invoking the function to all
pairs of elements. This function does not create a new tensor object, instead,
an expression object is returned. The returned object uses lazy-evaluation,
which means that the function is called only when required, i.e., when the
whole expression is evaluated or assigned to a tensor object.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

int multiply(int a, int b) {
    return a * b;
}

int main() {
    np::array<int> arr1, arr2;
    std::cin >> arr1 >> arr2;
    // Returns an expression equivalent to matrix<int>
    std::cout << "1-dimensional against 1-dimensional:\n";
    std::cout << np::outer(multiply, arr1, arr2) << "\n";
    np::matrix<int> mat1, mat2;
    std::cin >> mat1 >> mat2;
    // Returns an expression equivalent to tensor<int, 4>
    std::cout << "2-dimensional against 2-dimensional:\n";
    np::tensor<int, 4> out = np::outer(multiply, mat1, mat2);
    std::cout << out.shape() << "\n";
    std::cout << out << "\n";
    return 0;
}
```

Input

```
[1, 2, 3]
[1, 2, 3, 4, 5]

[[1, 2, 3],
 [4, 5, 6]]
[[1, 2, 3, 4]]
```

Output

```
1-dimensional against 1-dimensional:
[[ 1,  2,  3,  4,  5],
 [ 2,  4,  6,  8, 10],
 [ 3,  6,  9, 12, 15]]
2-dimensional against 2-dimensional:
(2, 3, 1, 4)
[[[[ 1,  2,  3,  4]],

  [[ 2,  4,  6,  8]],

  [[ 3,  6,  9, 12]]],


 [[[ 4,  8, 12, 16]],

  [[ 5, 10, 15, 20]],

  [[ 6, 12, 18, 24]]]]
```

<h3><code>outer</code></h3>

Apply a function to all pairs $(a, b)$ with $a$ in `arg1` and $b$ in `arg2`.
```cpp
template <class R, class Function, class T, size_t M, class U, size_t N>
void outer(
    tensor<R, M + N> &out,
    Function &&f, const tensor<T, M> &arg1, const tensor<U, N> &arg2
);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply.
* `arg1` First tensor-like object.
* `arg2` Second tensor-like object.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `out` does not match the
concatenated shape of `arg1` and `arg2` and cannot be resized.

