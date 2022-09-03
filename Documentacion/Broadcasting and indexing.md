# Broadcasting and indexing

Defined in `numcpp/broadcasting.h`

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Broadcasting](#broadcasting)
    - [`broadcast_to`](#broadcast_to)
    - [`broadcast_to`](#broadcast_to-1)
    - [`expand_dims`](#expand_dims)
    - [`squeeze`](#squeeze)
  - [Basic manipulation routines](#basic-manipulation-routines)
    - [`astype`](#astype)
    - [`copy`](#copy)
    - [`flatten`](#flatten)
    - [`zip`](#zip)
    - [`unzip`](#unzip)
  - [Concatenation](#concatenation)
    - [`concatenate`](#concatenate)
    - [`stack`](#stack)
  - [Tiling](#tiling)
    - [`tile`](#tile)
    - [`repeat`](#repeat)
  - [Padding](#padding)
    - [`pad`](#pad)
    - [`pad`](#pad-1)
    - [`pad_constant`](#pad_constant)
    - [`pad_edge`](#pad_edge)
    - [`pad_linear_ramp`](#pad_linear_ramp)
    - [`pad_reflect`](#pad_reflect)
    - [`pad_symmetric`](#pad_symmetric)
    - [`pad_wrap`](#pad_wrap)

## Broadcasting

### `broadcast_to`

Broadcast a value to a new shape.

Return a `tensor_view` of given shape filled with `val`. This function does not
create a new tensor, instead, it broadcasts the given value as a tensor of
given shape. All the elements in the broadcasted tensor refer to the same
memory location, and thus shall not be modified.
```cpp
template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(T &val, const shape_t<Rank> &shape);
```

Parameters

* `val` Reference to the fill value.
* `shape` The shape of the desired tensor.

Returns

* A view of the original value with the given shape.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::shape_t<1> shape1;
    np::shape_t<2> shape2;
    int val;
    std::cin >> shape1 >> val;
    // Return array_view<int>. All the elements are references to val.
    std::cout << np::broadcast_to(val, shape1) << "\n";
    std::cin >> shape2 >> val;
    // Return matrix_view<int>. All the elements are references to val.
    std::cout << np::broadcast_to(val, shape2) << "\n";
    return 0;
}
```

Input

```
(10,)
0
(4, 6)
0
```

Output

```
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[[0, 0, 0, 0, 0, 0],
 [0, 0, 0, 0, 0, 0],
 [0, 0, 0, 0, 0, 0],
 [0, 0, 0, 0, 0, 0]]
```

### `broadcast_to`

Broadcast a tensor to a new shape.

Return a `tensor_view` of given shape constructed by repeating elements along
each axis of size one. This function does not create a new tensor, instead, it
broadcasts the given tensor to a new shape. Some elements in the broadcasted
tensor may refer to the same memory location, and thus shall not be modified.
```cpp
template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(
    tensor<T, Rank> &arg, const shape_t<Rank> &shape
);

template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(
    tensor_view<T, Rank> arg, const shape_t<Rank> &shape
);
```

Parameters

* `arg` Reference to the `tensor` or `tensor_view` to broadcast.
* `shape` The shape of the desired tensor.

Returns

* A view of the original tensor with the given shape.

Exceptions

* `std::invalid_argument` Thrown if the tensor is not compatible with the new
shape according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> shape;
    std::cin >> mat >> shape;
    // Return matrix_view<int>. Expand axes of size one to match the desired
    // shape.
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
template <class T, size_t Rank>
tensor_view<T, Rank + 1> expand_dims(tensor<T, Rank> &arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank + N> expand_dims(
    tensor<T, Rank> &arg, const shape_t<N> &axes
);

template <class T, size_t Rank>
tensor_view<T, Rank + 1> expand_dims(tensor_view<T, Rank> arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank + N> expand_dims(
    tensor_view<T, Rank> arg, const shape_t<N> &axes
);
```

Parameters

* `arg` Input tensor.
* `axes` Position in the expanded axes where the new axis (or axes) is placed.

Returns

* A view of the original tensor with the number of dimensions increased.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr;
    std::cin >> arr;
    std::cout << "Original: " << arr.shape() << "\n";
    np::matrix_view<int> view1 = np::expand_dims(arr, 0);
    std::cout << "Expanded: " << view1.shape() << "\n";
    std::cout << view1 << "\n";
    np::matrix_view<int> view2 = np::expand_dims(arr, 1);
    std::cout << "Expanded: " << view2.shape() << "\n";
    std::cout << view2 << "\n";
    np::tensor_view<int, 3> view3 = np::expand_dims(arr, np::make_shape(0, 1));
    std::cout << "Expanded: " << view3.shape() << "\n";
    std::cout << view3 << "\n";
    np::tensor_view<int, 3> view4 = np::expand_dims(arr, np::make_shape(0, 2));
    std::cout << "Expanded: " << view4.shape() << "\n";
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
Original: (5,)
Expanded: (1, 5)
[[1, 2, 3, 4, 5]]
Expanded: (5, 1)
[[1],
 [2],
 [3],
 [4],
 [5]]
Expanded: (1, 1, 5)
[[[1, 2, 3, 4, 5]]]
Expanded: (1, 5, 1)
[[[1],
  [2],
  [3],
  [4],
  [5]]]
```

### `squeeze`

Removes axes of length one.
```cpp
template <class T, size_t Rank>
tensor_view<T, Rank - 1> squeeze(tensor<T, Rank> &arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank - N> squeeze(
    tensor<T, Rank> &arg, const shape_t<N> &axes
);

template <class T, size_t Rank>
tensor_view<T, Rank - 1> squeeze(tensor_view<T, Rank> arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank - N> squeeze(
    tensor_view<T, Rank> arg, const shape_t<N> &axes
);
```

Parameters

* `arg` Input tensor.
* `axes` Selects a subset of the entries of length one in the shape.

Returns

* A view of the original tensor with the axes removed.

Exceptions

* `std::invalid_argument` Thrown if an axis with shape entry greater than one
is selected.

Example

```cpp
#include <iostream>
#include <vector>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube;
    std::cin >> cube;
    std::cout << "Original: " << cube.shape() << "\n";
    // Find the axes of length one.
    std::vector<size_t> axes;
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (cube.shape(axis) == 1) {
            axes.push_back(axis);
        }
    }
    // Squeeze the axes of length one.
    if (axes.size() == 1) {
        np::matrix_view<int> view = np::squeeze(cube, axes[0]);
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    }
    else if (axes.size() == 2) {
        np::array_view<int> view =
            np::squeeze(cube, np::make_shape(axes[0], axes[1]));
        std::cout << "Squeezed: " << view.shape() << "\n" << view << "\n";
    }
    else if (axes.size() == 3) {
        std::cout << "Squeezed: " << cube(0, 0, 0) << "\n";
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

## Basic manipulation routines

### `astype`

Cast each element in a tensor to a specified type.
```cpp
template <class U, class T, size_t Rank>
tensor<U, Rank> astype(const tensor<T, Rank> &a);
```

Template parameters

* `U` Data type to which the elements are casted.

Parameters

* `a` Tensor-like object to cast.

Returns

* A light-weight object with the elements in the tensor casted to the specified
type. This function does not create a new tensor, instead, it returns a
readonly view of the tensor with its elements casted to the specified type.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
    std::cin >> a;
    std::cout << np::astype<int>(a) << "\n";
    return 0;
}
```

Input

```
[2.01, -3.62, -4.97, 6.77, 8.33, 5.93, 0.49, 7.8, 14.8, -2.3 ]
```

Output

```
[ 2, -3, -4,  6,  8,  5,  0,  7, 14, -2]
```

### `copy`

Return a copy of the tensor.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank>
copy(const tensor<T, Rank> &a);
```

Parameters

* `a` Tensor-like object to copy.

Returns

* A copy of the tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    std::cin >> mat;
    // Because mat.t() is a view of the elements in mat,
    //     mat = mat.t();
    // might give wrong results. Therefore, a temporary copy needs to be
    // created before assignment. Another valid alternative is
    //     mat = mat.t().copy();
    mat = np::copy(mat.t());
    std::cout << mat << "\n";
    return 0;
}
```

Input

```
[[1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
```

Output

```
[[ 1, -5, -1],
 [14, -3, 18],
 [12, 11, -3],
 [-3, 11, -1]]
```

### `flatten`

Return a copy of the tensor flattened into one dimension.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, 1>
flatten(const tensor<T, Rank> &a);

template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, 1>
flatten(const tensor<T, Rank> &a, bool row_major);
```

Parameters

* `a` Tensor-like object to flatten.
* `row_major` Whether to flatten the elements in row-major order (i.e., from
first axis to last axis) or column-major order (i.e., from last axis to first
axis). Defaults to the same order as stored in memory.

Returns

* The flattened tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    std::cin >> mat;
    std::cout << "Row-major (default):\n" << np::flatten(mat) << "\n";
    std::cout << "Column-major:\n" << np::flatten(mat, false) << "\n";
    return 0;
}
```

Input

```
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
```

Output

```
Row-major (default):
[ 1, 14, 12, -3, -5, -3, 11, 11, -1, 18, -3, -1]
Column-major:
[ 1, -5, -1, 14, -3, 18, 12, 11, -3, -3, 11, -1]
```

### `zip`

Return a tensor of pairs with each pair constructed from the corresponding
elements in two tensors.
```cpp
template <class T, class U, size_t Rank>
tensor<std::pair<T, U>, Rank> zip(
    const tensor<T, Rank> &a, const tensor<U, Rank> &b
);
```

Parameters

* `a` First tensor-like argument.
* `b` Second tensor-like argument.

Returns

* A light-weight object with the pairs constructed from the elements of two
tensors. This function does not create a new tensor, instead, it returns a
readonly view with the pairs constructed from both tensors.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

namespace std {
    // Overload output stream for std::pair
    template <class T, class U>
    ostream& operator<<(ostream &ostr, const pair<T, U> &rhs) {
        ostringstream sstr;
        sstr << "(" << rhs.first << ", " << rhs.second << ")";
        return ostr << sstr.str();
    }
}

int main() {
    np::array<int> a;
    np::array<double> b;
    std::cin >> a >> b;
    std::cout << np::zip(a, b) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
[1, 0.5, 0.333333, 0.25, 0.2, 0.166666, 0.142857, 0.125, 0.111111, 0.1]
```

Output

```
[       (1, 1),      (2, 0.5), (3, 0.333333),     (4, 0.25),      (5, 0.2),
 (6, 0.166666), (7, 0.142857),    (8, 0.125), (9, 0.111111),     (10, 0.1)]
```

### `unzip`

Return a tensor with each element constructed from taking the `I`-th element of
the corresponding pair in a tensor.
```cpp
template <size_t I, class T, class U, size_t Rank>
tensor<typename std::enable_if<(I == 0), T>::type, Rank>
unzip(const tensor<std::pair<T, U>, Rank> &a);

template <size_t I, class T, class U, size_t Rank>
tensor<typename std::enable_if<(I == 1), U>::type, Rank>
unzip(const tensor<std::pair<T, U>, Rank> &a);
```

Template parameters

* `I` Element index. It shall be 0 or 1.

Parameters

* `a` A tensor-like object of pairs.

Returns

* A light-weight object with the `I`-th element of each pair in the tensor.
This function does not create a new tensor, instead, it returns a readonly view
with the `I`-th element of each pair.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

namespace std {
    // Overload input stream for std::pair
    template <class T, class U>
    istream& operator>>(istream &istr, pair<T, U> &rhs) {
        char ch;
        bool success = (istr >> ch && ch == '('
            && istr >> rhs.first >> ch && ch == ','
            && istr >> rhs.second >> ch && ch == ')');
        if (!success) {
            istr.setstate(ios_base::failbit);
        }
        return istr;
    }
}

int main() {
    np::array<std::pair<int, double> > a;
    std::cin >> a;
    std::cout << np::unzip<0>(a) << "\n";
    std::cout << np::unzip<1>(a) << "\n";
    return 0;
}
```

Input

```
[(1, 1), (2, 0.5), (3, 0.333333), (4, 0.25), (5, 0.2), (6, 0.166666),
 (7, 0.142857), (8, 0.125), (9, 0.111111), (10, 0.1)]
```

Output

```
[ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10]
[       1,      0.5, 0.333333,     0.25,      0.2, 0.166666, 0.142857,    0.125,
 0.111111,      0.1
```

## Concatenation

### `concatenate`

Concatenate one or more tensors.
```cpp
template <class T, size_t Rank, class... Args>
tensor<T, Rank> concatenate(
    const tensor<T, Rank> &arg1, const Args&... arg2
);

template <class T, size_t Rank, class... Args>
tensor<T, Rank> concatenate(
    size_t axis, const tensor<T, Rank> &arg1, const Args&... arg2
);
```

Parameters

* `axis` The axis along which the tensors will be concatenated. Default is 0.
* `arg1, arg2...` Tensor-like objects to concatenate. The tensors must have the
same data type and the same shape, except in the dimension corresponding to
`axis`.

Returns

* The concatenated tensors.

Exceptions

* `std::invalid_argument` Thrown if the tensors do not have the same shape
along all the axes other than `axis`.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a, b;
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a, b;
    int axis;
    std::cin >> a >> b >> axis;
    std::cout << np::concatenate(axis, a, b) << "\n";
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
template <class T, size_t Rank, class... Args>
tensor<T, Rank + 1> stack(
    const tensor<T, Rank> &arg1, const Args&... arg2
);

template <class T, size_t Rank, class... Args>
tensor<T, Rank + 1> stack(
    size_t axis, const tensor<T, Rank> &arg1, const Args&... arg2
);
```

Parameters

* `axis` The axis in the result tensor along which the tensors will be stacked.
Default is 0.
* `arg1, arg2...` Tensor-like objects to stack. The tensors must have the same
data type and the same shape.

Returns

* The stacked tensors.

Exceptions

* `std::invalid_argument` Thrown if the tensors do not have the same shape.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a, b;
    std::cin >> a >> b;
    std::cout << "Axis 0:\n" << np::stack(0, a, b) << "\n";
    std::cout << "Axis 1:\n" << np::stack(1, a, b) << "\n";
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
Axis 0:
[[ 0,  1,  8, -3, -2],
 [ 7, -3, 10,  6,  4]]
Axis 1:
[[ 0,  7],
 [ 1, -3],
 [ 8, 10],
 [-3,  6],
 [-2,  4]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a, b;
    std::cin >> a >> b;
    std::cout << "Axis 0:\n" << np::stack(0, a, b) << "\n";
    std::cout << "Axis 1:\n" << np::stack(1, a, b) << "\n";
    std::cout << "Axis 2:\n" << np::stack(2, a, b) << "\n";
    return 0;
}
```

Input

```
[[-4,  0,  8],
 [ 2, 13, -1]]
[[-5, 13,  8],
 [-3,  2, -2]]
```

Output

```
Axis 0:
[[[-4,  0,  8],
  [ 2, 13, -1]],

 [[-5, 13,  8],
  [-3,  2, -2]]]
Axis 1:
[[[-4,  0,  8],
  [-5, 13,  8]],

 [[ 2, 13, -1],
  [-3,  2, -2]]]
Axis 2:
[[[-4, -5],
  [ 0, 13],
  [ 8,  8]],

 [[ 2, -3],
  [13,  2],
  [-1, -2]]]
```

## Tiling

### `tile`

Construct a tensor by repeating `a` the number of times given by `reps`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> tile(const tensor<T, Rank> &a, const shape_t<Rank> &reps);
```

Parameters

* `a` The input tensor.
* `reps` The number of repetitions of `a` along each axis.

Returns

* The tiled tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr;
    np::shape_t<1> n;
    std::cin >> arr >> n;
    std::cout << np::tile(arr, n) << "\n";
    np::matrix<int> mat;
    np::shape_t<2> reps;
    std::cin >> mat >> reps;
    std::cout << np::tile(mat, reps) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
2
[[1, 2, 3, 4],
 [5, 6, 7, 8]]
(3, 1)
```

Output

```
[1, 2, 3, 4, 5, 1, 2, 3, 4, 5]
[[1, 2, 3, 4],
 [5, 6, 7, 8],
 [1, 2, 3, 4],
 [5, 6, 7, 8],
 [1, 2, 3, 4],
 [5, 6, 7, 8]]
```

### `repeat`

Repeat elements of a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> repeat(
    const tensor<T, Rank> &a, size_t reps, size_t axis = 0
);

template <class T, size_t Rank, class IntegralType>
tensor<T, Rank> repeat(
    const tensor<T, Rank> &a,
    const tensor<IntegralType, 1> &reps,
    size_t axis = 0
);
```

Parameters

* `a` The input tensor.
* `reps` The number of repetitions of each element.
* `axis` The axis along which to repeat values. Default is 0.

Returns

* A new tensor which has the same shape as `a`, except along the given axis.

Exceptions

* `std::invalid_argument` Thrown if the size of `reps` does not match the size
of `a` along the given axis.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr;
    int n;
    std::cin >> arr >> n;
    std::cout << np::repeat(arr, n) << "\n";
    np::array<int> reps;
    std::cin >> reps;
    std::cout << np::repeat(arr, reps) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
2
[2, 0, 1, 3, 2]
```

Output

```
[1, 1, 2, 2, 3, 3, 4, 4, 5, 5]
[1, 1, 3, 4, 4, 4, 5, 5]
```

## Padding

### `pad`

Pad a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> pad(
    const tensor<T, Rank> &arg,
    const shape_t<Rank> &before, const shape_t<Rank> &after
);
```

Parameters

* `arg` The tensor to pad.
* `before` Number of elements to pad at the beginning of each axis.
* `after` Number of elements to pad at the end of each axis.

Returns

* The padded tensor. Padded elements are left uninitialized.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr;
    np::shape_t<1> before1, after1;
    std::cin >> arr >> before1 >> after1;
    std::cout << "Padding an array:\n";
    std::cout << np::pad(arr, before1, after1) << "\n";
    np::matrix<int> mat;
    np::shape_t<2> before2, after2;
    std::cin >> mat >> before2 >> after2;
    std::cout << "Padding a matrix:\n";
    std::cout << np::pad(mat, before2, after2) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
3
2

[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Possible output

```
Padding an array:
[247995152,       687, 247988560,         1,         2,         3,         4,
         5,         0,        -1]
Padding a matrix:
[[248128656,       687, 247996384,       687,         0,         0],
 [        0,         0,         0,         0,         0,         0],
 [        0,         1,         2,         3,         4,         0],
 [        0,         5,         6,         7,         8,         0],
 [        0,         9,        10,        11,        12,         0],
 [        0,         0,         0,         0,         0,         0],
 [        0,         0,         0,         0,         0,         0]]
```

### `pad`

Pad a tensor.
```cpp
template <class T, size_t Rank, class Function, class... Args>
tensor<T, Rank> pad(
    const tensor<T, Rank> &arg,
    const shape_t<Rank> &before, const shape_t<Rank> &after,
    Function func, Args&&... args
);
```

Parameters

* `arg` The tensor to pad.
* `before` Number of elements to pad at the beginning of each axis.
* `after` Number of elements to pad at the end of each axis.
* `func` Padding function. For tensors with rank greater than 1, the padding of
later axes depends on the padding of previous axes. The signature of the
padding function should be equivalent to the following:
```cpp
void func(tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
          Args&&... args);
```
where
  * `view` A one dimensional view already padded with uninitialized values.
  * `before` Number of elements padded at the beginning of `view`.
  * `after` Number of elements padded at the end of `view`.
  * `axis` The axis currently being calculated.
* `args...` Additional arguments to pass to `func`.

Returns

* The padded tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

### `pad_constant`

Pads with a constant value.
```cpp
struct pad_constant;
```

Parameters

* `args...` The values to set the padded elements for each axis.
  - If no arguments are passed, all the axes are padded with zeros.
  - If a single value is passed, the same constant is used for all the axes.
  - If two values are passed, the same before and after constants are used for
    each axis.
  - If a matrix of values is passed, unique pad constants are used for each
    axis, where `values(i, 0)` and `values(i, 1)` are the before and after
    constants for axis `i`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> before, after;
    std::cin >> mat >> before >> after;
    std::cout << "Pad with zeros:\n";
    std::cout << np::pad(mat, before, after, np::pad_constant()) << "\n";
    std::cout << "Pad with ones:\n";
    std::cout << np::pad(mat, before, after, np::pad_constant(), 1) << "\n";
    std::cout << "Pad with zeros and ones:\n";
    std::cout << np::pad(mat, before, after, np::pad_constant(), 0, 1) << "\n";
    std::cout << "Pad with different values:\n";
    np::matrix<int> values{{0, 1}, {2, 3}};
    std::cout << np::pad(mat, before, after, np::pad_constant(), values) << "\n";
    return 0;
}
```

Input

```
[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Output

```
Pad with zeros:
[[ 0,  0,  0,  0,  0,  0],
 [ 0,  0,  0,  0,  0,  0],
 [ 0,  1,  2,  3,  4,  0],
 [ 0,  5,  6,  7,  8,  0],
 [ 0,  9, 10, 11, 12,  0],
 [ 0,  0,  0,  0,  0,  0],
 [ 0,  0,  0,  0,  0,  0]]
Pad with ones:
[[ 1,  1,  1,  1,  1,  1],
 [ 1,  1,  1,  1,  1,  1],
 [ 1,  1,  2,  3,  4,  1],
 [ 1,  5,  6,  7,  8,  1],
 [ 1,  9, 10, 11, 12,  1],
 [ 1,  1,  1,  1,  1,  1],
 [ 1,  1,  1,  1,  1,  1]]
Pad with zeros and ones:
[[ 0,  0,  0,  0,  0,  1],
 [ 0,  0,  0,  0,  0,  1],
 [ 0,  1,  2,  3,  4,  1],
 [ 0,  5,  6,  7,  8,  1],
 [ 0,  9, 10, 11, 12,  1],
 [ 0,  1,  1,  1,  1,  1],
 [ 0,  1,  1,  1,  1,  1]]
Pad with different values:
[[ 2,  0,  0,  0,  0,  3],
 [ 2,  0,  0,  0,  0,  3],
 [ 2,  1,  2,  3,  4,  3],
 [ 2,  5,  6,  7,  8,  3],
 [ 2,  9, 10, 11, 12,  3],
 [ 2,  1,  1,  1,  1,  3],
 [ 2,  1,  1,  1,  1,  3]]
```

### `pad_edge`

Pads with the edge values of the tensor.
```cpp
struct pad_edge
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> before, after;
    std::cin >> mat >> before >> after;
    std::cout << "Pad with edge values:\n";
    std::cout << np::pad(mat, before, after, np::pad_edge()) << "\n";
    return 0;
}
```

Input

```
[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Output

```
Pad with edge values:
[[ 1,  1,  2,  3,  4,  4],
 [ 1,  1,  2,  3,  4,  4],
 [ 1,  1,  2,  3,  4,  4],
 [ 5,  5,  6,  7,  8,  8],
 [ 9,  9, 10, 11, 12, 12],
 [ 9,  9, 10, 11, 12, 12],
 [ 9,  9, 10, 11, 12, 12]]
```

### `pad_linear_ramp`

Pads with the linear ramp between an end value and the tensor edge value.
```cpp
struct pad_linear_ramp;
```

Parameters

* `args..` The values used for the ending values of the linear ramp.
  - If no arguments are passed, all the end values are set to zero.
  - If a single value is passed, the same value is used for all the axes.
  - If two values are passed, the same before and after end values are used for
    each axis.
  - If a matrix of values is passed, unique end values are used for each axis,
    where `values(i, 0)` and `values(i, 1)` are the before and after end values
    for axis `i`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> mat;
    np::shape_t<2> before, after;
    std::cin >> mat >> before >> after;
    std::cout << "Pad linear ramp towards 0:\n";
    std::cout << np::pad(mat, before, after, np::pad_linear_ramp()) << "\n";
    std::cout << "Pad linear ramp towards 1:\n";
    std::cout << np::pad(mat, before, after, np::pad_linear_ramp(), 1) << "\n";
    std::cout << "Pad linear ramp towards 0 and 1:\n";
    std::cout << np::pad(mat, before, after, np::pad_linear_ramp(), 0, 1) << "\n";
    std::cout << "Pad linear ramp towards different values:\n";
    np::matrix<double> values{{0, 1}, {2, 3}};
    std::cout << np::pad(mat, before, after, np::pad_linear_ramp(), values) << "\n";
    return 0;
}
```

Input

```
[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Output

```
Pad linear ramp towards 0:
[[  0,   0,   0,   0,   0,   0],
 [  0, 0.5,   1, 1.5,   2,   0],
 [  0,   1,   2,   3,   4,   0],
 [  0,   5,   6,   7,   8,   0],
 [  0,   9,  10,  11,  12,   0],
 [  0, 4.5,   5, 5.5,   6,   0],
 [  0,   0,   0,   0,   0,   0]]
Pad linear ramp towards 1:
[[  1,   1,   1,   1,   1,   1],
 [  1,   1, 1.5,   2, 2.5,   1],
 [  1,   1,   2,   3,   4,   1],
 [  1,   5,   6,   7,   8,   1],
 [  1,   9,  10,  11,  12,   1],
 [  1,   5, 5.5,   6, 6.5,   1],
 [  1,   1,   1,   1,   1,   1]]
Pad linear ramp towards 0 and 1:
[[  0,   0,   0,   0,   0,   1],
 [  0, 0.5,   1, 1.5,   2,   1],
 [  0,   1,   2,   3,   4,   1],
 [  0,   5,   6,   7,   8,   1],
 [  0,   9,  10,  11,  12,   1],
 [  0,   5, 5.5,   6, 6.5,   1],
 [  0,   1,   1,   1,   1,   1]]
Pad linear ramp towards different values:
[[  2,   0,   0,   0,   0,   3],
 [  2, 0.5,   1, 1.5,   2,   3],
 [  2,   1,   2,   3,   4,   3],
 [  2,   5,   6,   7,   8,   3],
 [  2,   9,  10,  11,  12,   3],
 [  2,   5, 5.5,   6, 6.5,   3],
 [  2,   1,   1,   1,   1,   3]]
```

### `pad_reflect`

Pads with the reflection of the tensor mirrored on the first and last values.
```cpp
struct pad_reflect;
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> before, after;
    std::cin >> mat >> before >> after;
    std::cout << "Pad reflect:\n";
    std::cout << np::pad(mat, before, after, np::pad_reflect()) << "\n";
    return 0;
}
```

Input

```
[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Output

```
Pad reflect:
[[10,  9, 10, 11, 12, 11],
 [ 6,  5,  6,  7,  8,  7],
 [ 2,  1,  2,  3,  4,  3],
 [ 6,  5,  6,  7,  8,  7],
 [10,  9, 10, 11, 12, 11],
 [ 6,  5,  6,  7,  8,  7],
 [ 2,  1,  2,  3,  4,  3]]
```

### `pad_symmetric`

Pads with the reflection of the tensor mirrored along the edge.
```cpp
struct pad_symmetric;
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> before, after;
    std::cin >> mat >> before >> after;
    std::cout << "Pad symmetric:\n";
    std::cout << np::pad(mat, before, after, np::pad_symmetric()) << "\n";
    return 0;
}
```

Input

```
[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Output

```
Pad symmetric:
[[ 5,  5,  6,  7,  8,  8],
 [ 1,  1,  2,  3,  4,  4],
 [ 1,  1,  2,  3,  4,  4],
 [ 5,  5,  6,  7,  8,  8],
 [ 9,  9, 10, 11, 12, 12],
 [ 9,  9, 10, 11, 12, 12],
 [ 5,  5,  6,  7,  8,  8]]
```

### `pad_wrap`

Pads with the wrap of the tensor along the axis. The first values are used to
pad the end and the end values are used to pad the beginning.
```cpp
struct pad_wrap;
```

Parameters

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    np::shape_t<2> before, after;
    std::cin >> mat >> before >> after;
    std::cout << "Pad wrap:\n";
    std::cout << np::pad(mat, before, after, np::pad_wrap()) << "\n";
    return 0;
}
```

Input

```
[[1,  2 , 3,  4],
 [5,  6 , 7,  8],
 [9, 10, 11, 12]]
(2, 1)
(2, 1)
```

Output

```
Pad wrap:
[[ 8,  5,  6,  7,  8,  5],
 [12,  9, 10, 11, 12,  9],
 [ 4,  1,  2,  3,  4,  1],
 [ 8,  5,  6,  7,  8,  5],
 [12,  9, 10, 11, 12,  9],
 [ 4,  1,  2,  3,  4,  1],
 [ 8,  5,  6,  7,  8,  5]]
```
