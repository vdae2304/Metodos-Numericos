# [Broadcasting and indexing](readme.md)

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Concatenation](#concatenation)
    - [`concatenate`](#concatenate)
    - [`stack`](#stack)

## Concatenation

### `concatenate`

Concatenate one or more tensors.
```cpp
template <class T, size_t Rank, class... Tensors>
tensor<T, Rank> concatenate(const tensor<T, Rank> &a, const Tensors &...b);

template <size_t Axis, class T, size_t Rank, class... Tensors>
tensor<T, Rank> concatenate(const tensor<T, Rank> &a, const Tensors &...b);
```

Template parameters

* `Axis` The axis along which the tensors will be concatenated. Default is 0.

Parameters

* `a, b...` Tensor-like objects to concatenate. The tensors must have the same value type and the same shape, except in the dimension corresponding to `axis`.

Returns

* The concatenated tensors.

Exceptions

* `std::invalid_argument` Thrown if the tensors do not have the same shape along all the axes other than `axis`.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a, b;
    std::cin >> a >> b;
    std::cout << np::concatenate(a, b) << "\n";
    return 0;
}
```

Input

```
[10, 2, 9, 1, 4, 2]
[2, 5, 6, 3]
```

Output

```
[10,  2,  9,  1,  4,  2,  2,  5,  6,  3]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a, b;
    int axis;
    std::cin >> a >> b >> axis;
    switch (axis) {
    case 0:
        std::cout << np::concatenate<0>(a, b) << "\n";
        break;
    case 1:
        std::cout << np::concatenate<1>(a, b) << "\n";
        break;
    }
    return 0;
}
```

Input

```
[[-3,  2,  9,  4, 10],
 [ 0,  4, 13, 14,  5],
 [ 3,  8,  7, -5,  3]]

[[ 1, -5,  0, -3,  9],
 [ 8, -4, 14,  2, -4]]

0
```

Output

```
[[-3,  2,  9,  4, 10],
 [ 0,  4, 13, 14,  5],
 [ 3,  8,  7, -5,  3],
 [ 1, -5,  0, -3,  9],
 [ 8, -4, 14,  2, -4]]
```

Input

```
[[-3,  0,  3],
 [ 2,  4,  8],
 [ 9, 13,  7],
 [ 4, 14, -5],
 [10,  5,  3]]

[[ 1,  8],
 [-5, -4],
 [ 0, 14],
 [-3,  2],
 [ 9, -4]]

1
```

Output

```
[[-3,  0,  3,  1,  8],
 [ 2,  4,  8, -5, -4],
 [ 9, 13,  7,  0, 14],
 [ 4, 14, -5, -3,  2],
 [10,  5,  3,  9, -4]]
```

### `stack`

Concatenate one or more tensors along a new axis.
```cpp
template <class T, size_t Rank, class... Tensors>
tensor<T, Rank + 1> stack(const tensor<T, Rank> &a, const Tensors &...b);

template <size_t Axis, class T, size_t Rank, class... Tensors>
tensor<T, Rank + 1> stack(const tensor<T, Rank> &a, const Tensors &...b);
```

Template parameters

* `axis` The axis in the result tensor along which the tensors will be stacked. Default is 0.

Parameters

* `a, b...` Tensor-like objects to stack. The tensors must have the same value type and the same shape.

Returns

* The stacked tensors.

Exceptions

* `std::invalid_argument` Thrown if the tensors do not have the same shape.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a, b;
    std::cin >> a >> b;
    std::cout << "Stack along first axis:\n" << np::stack<0>(a, b) << "\n";
    std::cout << "Stack along second axis:\n" << np::stack<1>(a, b) << "\n";
    return 0;
}
```

Input

```
[0, 1, 8, -3, -2]
[7, -3, 10, 6, 4]
```

Output

```
Stack along first axis:
[[ 0,  1,  8, -3, -2],
 [ 7, -3, 10,  6,  4]]
Stack along second axis:
[[ 0,  7],
 [ 1, -3],
 [ 8, 10],
 [-3,  6],
 [-2,  4]]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a, b;
    std::cin >> a >> b;
    std::cout << "Stack along first axis:\n" << np::stack<0>(a, b) << "\n";
    std::cout << "Stack along second axis:\n" << np::stack<1>(a, b) << "\n";
    std::cout << "Stack along third axis:\n" << np::stack<2>(a, b) << "\n";
    return 0;
}
```

Input

```
[[17,  0,  5,  8],
 [ 3, -4,  6, -3],
 [ 0, 13, -5,  3]]
[[ 8,  6,  2, 16],
 [ 1, 16, 17, -3],
 [-4, 16,  8, 18]]
```

Output

```
Stack along first axis:
[[[17,  0,  5,  8],
  [ 3, -4,  6, -3],
  [ 0, 13, -5,  3]],

 [[ 8,  6,  2, 16],
  [ 1, 16, 17, -3],
  [-4, 16,  8, 18]]]
Stack along second axis:
[[[17,  0,  5,  8],
  [ 8,  6,  2, 16]],

 [[ 3, -4,  6, -3],
  [ 1, 16, 17, -3]],

 [[ 0, 13, -5,  3],
  [-4, 16,  8, 18]]]
Stack along third axis:
[[[17,  8],
  [ 0,  6],
  [ 5,  2],
  [ 8, 16]],

 [[ 3,  1],
  [-4, 16],
  [ 6, 17],
  [-3, -3]],

 [[ 0, -4],
  [13, 16],
  [-5,  8],
  [ 3, 18]]]
```
