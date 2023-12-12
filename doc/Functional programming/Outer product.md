# [Functional programming](readme.md)

Defined in header [`numcpp/functional.h`](/include/numcpp/functional.h)

- [Functional programming](#functional-programming)
  - [Outer product](#outer-product)
    - [`outer`](#outer)

## Outer product

### `outer`

Apply a function to all pairs $(a_i, b_j)$ with $a_i$ in $a$ and $b_j$ in $b$.

Let $a$ and $b$ be tensors of dimension $M$ and $N$, respectively. The result of `outer` is a tensor of dimension $M + N$ such that

$$out(i_0, ..., i_{M-1}, j_0, ..., j_{N-1})
    = f(a(i_0, ..., i_{M-1}), b(j_0, ..., j_{N-1}))$$

```cpp
template <class Function, class T, size_t Rank1, class U, size_t Rank2>
tensor</*Result type*/, Rank1 + Rank2>
outer(Function &&f, const tensor<T, Rank1> &a, const tensor<U, Rank2> &b);
```

Parameters

* `f` The function to apply.
* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.

Returns

* A light-weight object which stores the result of invoking the function to all pairs of elements. This function does not create a new tensor, instead, an expression object is returned. The returned object uses lazy-evaluation, which means that the function is called only when required, i.e., when the whole expression is evaluated or assigned to a tensor object.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;

int multiply(int a, int b) {
    return a * b;
}

np::shape_t<2> cartesian_prod(int i, int j) {
    return {i, j};
}

int main() {
    np::vector<int> a, b;
    std::cin >> a >> b;
    // Returns an expression equivalent to matrix<int>
    std::cout << "multiply:\n" << np::outer(multiply, a, b) << "\n";
    // Returns an expression equivalent to matrix<shape_t<2>>
    std::cout << "cartesian_prod:\n" << np::outer(cartesian_prod, a, b) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4]
[1, 2, 3, 4, 5, 6]
```

Output

```
multiply:
[[ 1,  2,  3,  4,  5,  6],
 [ 2,  4,  6,  8, 10, 12],
 [ 3,  6,  9, 12, 15, 18],
 [ 4,  8, 12, 16, 20, 24]]
cartesian_prod:
[[(1, 1), (1, 2), (1, 3), (1, 4), (1, 5), (1, 6)],
 [(2, 1), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6)],
 [(3, 1), (3, 2), (3, 3), (3, 4), (3, 5), (3, 6)],
 [(4, 1), (4, 2), (4, 3), (4, 4), (4, 5), (4, 6)]]
```

<h3><code>outer</code></h3>

Apply a function to all pairs $(a_i, b_j)$ with $a_i$ in $a$ and $b_j$ in $b$.

```cpp
template <class R, class Function, class T, size_t Rank1, class U, size_t Rank2>
void outer(tensor<R, Rank1 + Rank2> &out, Function &&f,
           const tensor<T, Rank1> &a, const tensor<U, Rank2> &b);
```

Parameters

* `out` A location into which the result is stored.
* `f` The function to apply.
* `a` A tensor-like object with the values to pass as first argument.
* `b` A tensor-like object with the values to pass as second argument.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `out` does not match the concatenated shape of `a` and `b`.
