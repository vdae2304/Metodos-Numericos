# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Rearranging elements](#rearranging-elements)
    - [`reverse`](#reverse)
    - [`rotate`](#rotate)

## Rearranging elements

### `reverse`

Reverse the order of the elements in a tensor along the given axes.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> reverse(const tensor<T, Rank> &a, size_t axis = Rank - 1);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> reverse(const tensor<T, Rank> &a, const shape_t<N> &axes);
```

Parameters

* `a` A tensor-like object to reverse.
* `axes` Axis or axes along which to reverse over. The default is `Rank - 1`, which reverses along the last axis.

Returns

* A light-weight object with the elements in the tensor in reversed order. This function does not create a new tensor, instead, an expression object is returned with the elements of the tensor in reversed order.

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
    std::cout << np::reverse(a) << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
[12,  3, -5, -3, 18,  4,  0, 19, -2, 12]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int axis;
    std::cin >> a >> axis;
    // For axis=0, reverse row elements for each column.
    // For axis=1, reverse column elements for each row.
    std::cout << np::reverse(a, axis) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
0
```

Output

```
[[ 6, 14,  1,  1,  4, -3],
 [ 3, 10, -2,  3,  4, 14],
 [ 8,  2,  0,  3, -5,  0],
 [13, 11,  2,  7,  7, 13]]
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
1
```

Output

```
[[13,  7,  7,  2, 11, 13],
 [ 0, -5,  3,  0,  2,  8],
 [14,  4,  3, -2, 10,  3],
 [-3,  4,  1,  1, 14,  6]]
```

### `rotate`

Rotate the elements in a tensor along the given axes. The elements are shifted circularly in such a way that the element at position `shift` becomes the first element and the element at position `shift - 1` becomes the last element.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> rotate(const tensor<T, Rank> &a, size_t shift,
                       size_t axis = Rank - 1);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> rotate(const tensor<T, Rank> &a, const index_t<N> &shift,
                       const shape_t<N> &axes);
```

Parameters

* `a` A tensor-like object to rotate.
* `shift` Number of positions to shift the elements by along each axis.
* `axes` Axis or axes along which the elements are rotated. The default is `Rank - 1`, which rotates along the last axis.

Returns

* A light-weight object with the elements in the tensor shifted circularly. This function does not create a new tensor, instead, an expression object is returned with the elements of the tensor shifted circularly.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    int shift;
    std::cin >> a >> shift;
    std::cout << np::rotate(a, shift) << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
3
```

Output

```
[ 0,  4, 18, -3, -5,  3, 12, 12, -2, 19]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int shift, axis;
    std::cin >> a >> shift >> axis;
    std::cout << np::rotate(a, shift, axis) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
2
0
```

Output

```
[[ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3],
 [13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0]]
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
3
1
```

Output

```
[[ 7,  7, 13, 13, 11,  2],
 [ 3, -5,  0,  8,  2,  0],
 [ 3,  4, 14,  3, 10, -2],
 [ 1,  4, -3,  6, 14,  1]]
```
