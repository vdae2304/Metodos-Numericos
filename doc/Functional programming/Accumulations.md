# [Functional programming](readme.md)

Defined in header [`numcpp/functional.h`](/include/numcpp/functional.h)

- [Functional programming](#functional-programming)
  - [Accumulations](#accumulations)
    - [`accumulate`](#accumulate)

## Accumulations

### `accumulate`

Accumulate the result of applying a function along an axis.
```cpp
template <class Function, class T, size_t Rank>
tensor<T, Rank> accumulate(Function &&f, const tensor<T, Rank> &a,
                           size_t axis = 0);
```

Parameters

* `f` The function to apply. A binary function taking the current accumulated value as first argument and an element in the tensor as second argument, and returning a value.
* `a` A tensor-like object with the values where the accumulation will be performed.
* `axis` Axis along which to apply the accumulation. Default is zero.

Returns

* A new tensor with the accumulated values along the given axis.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;

int add(int a, int b) {
    return a + b;
}

int main() {
    np::vector<int> vec;
    std::cin >> vec;
    std::cout << np::accumulate(add, vec) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
[ 4,  7, 15, 16, 31, 34, 39, 40, 42, 44]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;

int add(int a, int b) {
    return a + b;
}

int main() {
    np::matrix<int> mat;
    std::cin >> mat;
    // For axis=0, accumulate values over all rows (column-wise).
    // For axis=1, accumulate values over all columns (row-wise).
    std::cout << "Sum along axis 0:\n";
    std::cout << np::accumulate(add, mat, 0) << "\n";
    std::cout << "Sum along axis 1:\n";
    std::cout << np::accumulate(add, mat, 1) << "\n";
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
Sum along axis 0:
[[ 8,  3,  9,  5,  3,  6],
 [15,  5, 14, 12,  6, 15],
 [18,  6, 16, 17, 13, 22],
 [20, 15, 21, 23, 18, 32]]
Sum along axis 1:
[[ 8, 11, 20, 25, 28, 34],
 [ 7,  9, 14, 21, 24, 33],
 [ 3,  4,  6, 11, 18, 25],
 [ 2, 11, 16, 22, 27, 37]]
```
