# [Functional programming](readme.md)

Defined in header [`numcpp/functional.h`](/include/numcpp/functional.h)

- [Functional programming](#functional-programming)
  - [Reductions](#reductions)
    - [`reduce`](#reduce)

## Reductions

### `reduce`

Reduce the tensor's dimension by cumulatively applying a function to all elements.
```cpp
template <class Function, class T, size_t Rank>
T reduce(Function &&f, const tensor<T, Rank> &a);

template <class Function, class T, size_t Rank>
T reduce(Function &&f, const tensor<T, Rank> &a,
         typename tensor<T, Rank>::value_type init);

template <class Function, class T, size_t Rank>
T reduce(Function &&f, const tensor<T, Rank> &a,
         typename tensor<T, Rank>:value_type init,
         const tensor<bool, Rank> &where);
```

Parameters

* `f` The function to apply. A binary function taking the current accumulated value as first argument and an element in the tensor as second argument, and returning a value.
* `a` A tensor-like object with the values where the reduction will be performed.
* `init` Initial value. If not provided, the first element is used.
* `where` A boolean tensor which indicates the elements to include in the reduction.

Returns

* The result of accumulating all the elements in the tensor.

Exceptions

* `std::invalid_argument` Thrown if the tensor is empty and `init` is not provided.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;

int max(int a, int b) {
    return b < a ? a : b;
}

int main() {
    np::vector<int> vec;
    std::cin >> vec;
    std::cout << "Maximum value: " << np::reduce(max, vec) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
Maximum value: 15
```

Input

```
[]
```

Possible output

```
terminate called after throwing an instance of 'std::invalid_argument'
  what():  attempt to reduce on an empty sequence with no initial value
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
    np::vector<int> vec;
    std::cin >> vec;
    std::cout << "Sum all values: ";
    std::cout << np::reduce(add, vec, /*init=*/0) << "\n";
    std::cout << "Sum positive values: ";
    std::cout << np::reduce(add, vec, /*init=*/0, /*where=*/vec > 0) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
Sum all values: 44
Sum positive values: 44
```

Input

```
[ 7, -4,  3, -5,  5,  6,  8,  0,  4, -2]
```

Output

```
Sum all values: 22
Sum positive values: 33
```

<h3><code>reduce</code></h3>

Reduce the tensor's dimension by cumulatively applying a function over multiple axes.
```cpp
template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f, const tensor<T, Rank> &a,
                           const shape_t<N> &axes);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f, const tensor<T, Rank> &a,
                           const shape_t<N> &axes,
                           typename tensor<T, Rank>::value_type init);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f, const tensor<T, Rank> &a,
                           const shape_t<N> &axes,
                           typename tensor<T, Rank>::value_type init,
                           const tensor<bool, Rank> &where);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank> reduce(Function &&f, const tensor<T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank> reduce(Function &&f, const tensor<T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t,
                       typename tensor<T, Rank>::value_type init);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank> reduce(Function &&f, const tensor<T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t,
                       typename tensor<T, Rank>::value_type init,
                       const tensor<bool, Rank> &where);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f, const tensor<T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f, const tensor<T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t,
                           typename tensor<T, Rank>::value_type init);

template <class Function, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f, const tensor<T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t,
                           typename tensor<T, Rank>::value_type init,
                           const tensor<bool, Rank> &where);
```

Parameters

* `f` The function to apply. A binary function taking the current accumulated value as first argument and an element in the tensor as second argument, and returning a value.
* `a` A tensor-like object with the values where the reduction will be performed.
* `axes` Axes along which the reduction is performed.
* `keepdims` If set to `keepdims`, the axes which are reduced are left as dimensions with size one. If set to `dropdims`, the axes which are reduced are dropped. Defaults to `dropdims`.
* `init` Initial value. If not provided, the first element is used.
* `where` A boolean tensor which indicates the elements to include in the reduction.

Returns

* A new tensor with the result of performing the reduction over the given axes.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/functional.h>
#include <numcpp/io.h>
namespace np = numcpp;

int max(int a, int b) {
    return b < a ? a : b;
}

int main() {
    np::matrix<int> mat;
    np::shape_t<1> axis;
    std::cin >> mat >> axis;
    // For axis=0, perform reduction over all rows (column-wise).
    // For axis=1, perform reduction over all columns (row-wise).
    std::cout << "Maximum along axis " << axis << " (drop axis):\n";
    std::cout << np::reduce(max, mat, axis) << "\n";
    std::cout << "Maximum along axis " << axis << " (keep axis):\n";
    std::cout << np::reduce(max, mat, axis, np::keepdims) << "\n";
    return 0;
}
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
0
```

Output

```
Maximum along axis (0,) (drop axis):
[ 8,  9,  9,  7,  7, 10]
Maximum along axis (0,) (keep axis):
[[ 8,  9,  9,  7,  7, 10]]
```

Input

```
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
1
```

Output

```
Maximum along axis (1,) (drop axis):
[ 9,  9,  7, 10]
Maximum along axis (1,) (keep axis):
[[ 9],
 [ 9],
 [ 7],
 [10]]
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
    np::shape_t<1> axis;
    std::cin >> mat >> axis;

    // For axis=0, perform reduction over all rows (column-wise).
    // For axis=1, perform reduction over all columns (row-wise).
    std::cout << "Sum all values along axis " << axis << " (drop axis):\n";
    std::cout << np::reduce(add, mat, axis, /*init=*/0) << "\n";
    std::cout << "Sum positive values along axis " << axis << " (drop axis):\n";
    std::cout << np::reduce(add, mat, axis, /*init=*/0,
                            /*where=*/mat > 0) << "\n";
 
    std::cout << "Sum all values along axis " << axis << " (keep axis):\n";
    std::cout << np::reduce(add, mat, axis, np::keepdims, /*init=*/0) << "\n";
    std::cout << "Sum positive values along axis " << axis << " (keep axis):\n";
    std::cout << np::reduce(add, mat, axis, np::keepdims, /*init=*/0,
                            /*where=*/mat > 0) << "\n";
    return 0;
}
```

Input

```
[[-5,  9,  8,  4, -3,  1],
 [ 8,  2,  6,  8, -2, -5],
 [ 4,  7, -1,  3,  6, -3],
 [ 1,  5,  4,  2,  8, -2]]
0
```

Output

```
Sum all values along axis (0,) (drop axis):
[ 8, 23, 17, 17,  9, -9]
Sum positive values along axis (0,) (drop axis):
[13, 23, 18, 17, 14,  1]
Sum all values along axis (0,) (keep axis):
[[ 8, 23, 17, 17,  9, -9]]
Sum positive values along axis (0,) (keep axis):
[[13, 23, 18, 17, 14,  1]]
```

Input

```
[[-5,  9,  8,  4, -3,  1],
 [ 8,  2,  6,  8, -2, -5],
 [ 4,  7, -1,  3,  6, -3],
 [ 1,  5,  4,  2,  8, -2]]
1
```

Output

```
Sum all values along axis (1,) (drop axis):
[14, 17, 16, 18]
Sum positive values along axis (1,) (drop axis):
[22, 24, 20, 20]
Sum all values along axis (1,) (keep axis):
[[14],
 [17],
 [16],
 [18]]
Sum positive values along axis (1,) (keep axis):
[[22],
 [24],
 [20],
 [20]]
```
