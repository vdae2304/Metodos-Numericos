# [Broadcasting and indexing](readme.md)

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Broadcasting](#broadcasting)
    - [`broadcast_to`](#broadcast_to)
    - [`expand_dims`](#expand_dims)
    - [`squeeze`](#squeeze)

## Broadcasting

### `broadcast_to`

Broadcast a tensor to a new shape.

Return a readonly `tensor_view` of given shape constructed by repeating elements along each axis of size one. This function does not create a new tensor, instead, it broadcasts the given tensor to a new shape. Thus, some of the elements in the broadcasted tensor may refer to the same memory location.
```cpp
template <class T, size_t Rank>
tensor_view<const T, Rank> broadcast_to(tensor<T, Rank> &a,
                                        const shape_t<Rank> &shape);

template <class T, size_t Rank>
tensor_view<const T, Rank> broadcast_to(tensor_view<T, Rank> &a,
                                        const shape_t<Rank> &shape);
```

Parameters

* `a` Reference to the `tensor` or `tensor_view` to broadcast.
* `shape` The shape of the desired tensor.

Returns

* A readonly view of the original tensor with the given shape.

Exceptions

* `std::invalid_argument` Thrown if the tensor is not compatible with the new shape according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> shape;
    std::cin >> mat >> shape;
    // Return matrix_view<const int>. Expand axes of size one to match the
    // desired shape.
    std::cout << np::broadcast_to(mat, shape) << "\n";
    return 0;
}
```

Input

```
[[0]]
(3, 5)
```

Output

```
[[0, 0, 0, 0, 0],
 [0, 0, 0, 0, 0],
 [0, 0, 0, 0, 0]]
```

Input

```
[[1, 2, 3, 4, 5]]
(3, 5)
```

Output

```
[[1, 2, 3, 4, 5],
 [1, 2, 3, 4, 5],
 [1, 2, 3, 4, 5]]
```

Input

```
[[1],
 [2],
 [3],
 [4],
 [5]]
(5, 3)
```

Output

```
[[1, 1, 1],
 [2, 2, 2],
 [3, 3, 3],
 [4, 4, 4],
 [5, 5, 5]]
```

### `expand_dims`

Expand the shape of a tensor by inserting new axes.
```cpp
template <class T, size_t Rank, class... Axes>
tensor_view<T, Rank + sizeof...(Axes)> expand_dims(tensor<T, Rank> &a,
                                                   Axes... axes);

template <class T, size_t Rank, class... Axes>
tensor_view<T, Rank + sizeof...(Axes)> expand_dims(tensor_view<T, Rank> &a,
                                                   Axes... axes);

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank + N> expand_dims(tensor<T, Rank> &a,
                                     const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank + N> expand_dims(tensor_view<T, Rank> &a,
                                     const shape_t<N> &axes);
```

Parameters

* `a` Input tensor.
* `axes` Position in the expanded axes where the new axis (or axes) is placed.

Returns

* A view of the original tensor with the number of dimensions increased.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    std::cin >> vec;

    // The following is equivalent to vec.reshape(1, vec.size())
    np::matrix_view<int> view1 = np::expand_dims(vec, 0);
    std::cout << "As a row vector:\n" << view1 << "\n";

    // The following is equivalent to vec.reshape(vec.size(), 1)
    np::matrix_view<int> view2 = np::expand_dims(vec, 1);
    std::cout << "As a column vector:\n" << view2 << "\n";

    // The following is equivalent to vec.reshape(1, 1, vec.size())
    np::tensor_view<int, 3> view3 = np::expand_dims(vec, 0, 1);
    std::cout << "As a tensor of shape " << view3.shape() << "\n";
    std::cout << view3 << "\n";

    // The following is equivalent to vec.reshape(1, vec.size(), 1)
    np::tensor_view<int, 3> view4 = np::expand_dims(vec, 2, 0);
    std::cout << "As a tensor of shape " << view4.shape() << "\n";
    std::cout << view4 << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
```

Output

```
As a row vector:
[[1, 2, 3, 4, 5]]
As a column vector:
[[1],
 [2],
 [3],
 [4],
 [5]]
As a tensor of shape (1, 1, 5)
[[[1, 2, 3, 4, 5]]]
As a tensor of shape (1, 5, 1)
[[[1],
  [2],
  [3],
  [4],
  [5]]]
```

### `squeeze`

Removes axes of length one.
```cpp
template <class T, size_t Rank, class... Axes>
tensor_view<T, Rank - sizeof...(Axes)> squeeze(tensor<T, Rank> &a,
                                               Axes... axes);

template <class T, size_t Rank, class... Axes>
tensor_view<T, Rank - sizeof...(Axes)> squeeze(tensor_view<T, Rank> &a,
                                               Axes... axes);

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank - N> squeeze(tensor<T, Rank> &a, const shape_t<N> &axes);

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank - N> squeeze(tensor_view<T, Rank> &a,
                                 const shape_t<N> &axes);
```

Parameters

* `a` Input tensor.
* `axes` Selects a subset of the entries of length one in the shape.

Returns

* A view of the original tensor with the axes removed.

Exceptions

* `std::invalid_argument` Thrown if an axis with shape entry greater than one is selected.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube;
    std::cin >> cube;
    std::cout << "Original: " << cube.shape() << "\n";
    // Axes 0 and 1 can be squeezed.
    if (cube.shape(0) == 1 && cube.shape(1) == 1) {
        np::vector_view<int> view = np::squeeze(cube, 0, 1);
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    // Axes 0 and 2 can be squeezed.
    } else if (cube.shape(0) == 1 && cube.shape(2) == 1) {
        np::vector_view<int> view = np::squeeze(cube, 0, 2);
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    // Axes 1 and 2 can be squeezed.
    } else if (cube.shape(1) == 1 && cube.shape(2) == 1) {
        np::vector_view<int> view = np::squeeze(cube, 1, 2);
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    // Axis 0 can be squeezed.
    } else if (cube.shape(0) == 1) {
        np::matrix_view<int> view = np::squeeze(cube, 0);
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    // Axis 1 can be squeezed.
    } else if (cube.shape(1) == 1) {
        np::matrix_view<int> view = np::squeeze(cube, 1);
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    // Axis 2 can be squeezed.
    } else if (cube.shape(2) == 1) {
        np::matrix_view<int> view = np::squeeze(cube, 2);
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    }
    return 0;
}
```

Input

```
[[[1, 2, 3, 4, 5]]]
```

Output

```
Original: (1, 1, 5)
Squeezed: (5,)
[1, 2, 3, 4, 5]
```


Input

```
[[[1],
  [2],
  [3],
  [4],
  [5]]]
```

Output

```
Original: (1, 5, 1)
Squeezed: (5,)
[1, 2, 3, 4, 5]
```

Input

```
[[[ 1,  2 , 3,  4],
  [ 5,  6 , 7,  8],
  [ 9, 10, 11, 12]]]
```

Output

```
Original: (1, 3, 4)
Squeezed: (3, 4)
[[ 1,  2,  3,  4],
 [ 5,  6,  7,  8],
 [ 9, 10, 11, 12]]
```
