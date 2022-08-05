# Tensor methods

Defined in `numcpp/tensor.h`

- [Tensor methods](#tensor-methods)
  - [Template parameters](#template-parameters)
  - [Iterators](#iterators)
    - [`tensor::begin`](#tensorbegin)
    - [`tensor::end`](#tensorend)
    - [`tensor::cbegin`](#tensorcbegin)
    - [`tensor::cend`](#tensorcend)
    - [`tensor::iterator::base`](#tensoriteratorbase)
    - [`tensor::iterator::index`](#tensoriteratorindex)
    - [`tensor::iterator::coords`](#tensoriteratorcoords)
    - [`tensor::iterator::rowmajor`](#tensoriteratorrowmajor)
    - [`tensor::iterator::colmajor`](#tensoriteratorcolmajor)
  - [Public methods](#public-methods)
    - [`tensor::all`](#tensorall)
    - [`tensor::all(axes)`](#tensorallaxes)
    - [`tensor::any`](#tensorany)
    - [`tensor::any(axes)`](#tensoranyaxes)
    - [`tensor::apply`](#tensorapply)
    - [`tensor::argmax`](#tensorargmax)
    - [`tensor::argmax(axis)`](#tensorargmaxaxis)
    - [`tensor::argmin`](#tensorargmin)
    - [`tensor::argmin(axis)`](#tensorargminaxis)
    - [`tensor::argpartition`](#tensorargpartition)
    - [`tensor::argsort`](#tensorargsort)
    - [`tensor::astype`](#tensorastype)
    - [`tensor::clamp`](#tensorclamp)
    - [`tensor::conj`](#tensorconj)
    - [`tensor::copy`](#tensorcopy)
    - [`tensor::imag`](#tensorimag)
    - [`tensor::max`](#tensormax)
    - [`tensor::max(axes)`](#tensormaxaxes)
    - [`tensor::mean`](#tensormean)
    - [`tensor::mean(axes)`](#tensormeanaxes)
    - [`tensor::min`](#tensormin)
    - [`tensor::min(axes)`](#tensorminaxes)
    - [`tensor::nonzero`](#tensornonzero)
    - [`tensor::partition`](#tensorpartition)
    - [`tensor::prod`](#tensorprod)
    - [`tensor::prod(axes)`](#tensorprodaxes)
    - [`tensor::real`](#tensorreal)
    - [`tensor::reverse`](#tensorreverse)
    - [`tensor::sort`](#tensorsort)
    - [`tensor::stddev`](#tensorstddev)
    - [`tensor::stddev(axes)`](#tensorstddevaxes)
    - [`tensor::sum`](#tensorsum)
    - [`tensor::sum(axes)`](#tensorsumaxes)
    - [`tensor::var`](#tensorvar)
    - [`tensor::var(axes)`](#tensorvaraxes)

## Template parameters

* `T` Type of the elements contained in the tensor. This shall be an arithmetic
type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the tensor. It must be a positive integer.

## Iterators

### `tensor::begin`

<h3><code>tensor_view::begin</code></h3>

<h3><code>indirect_tensor::begin</code></h3>

Returns an iterator pointing to the first element in the tensor.
```cpp
iterator begin();
const_iterator begin() const;

iterator begin(bool row_major);
const_iterator begin(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A random access iterator to the beginning of the tensor. If the tensor is
const-qualified, the function returns a `const_iterator`. Otherwise, it returns
an `iterator`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array(10);
    int value = 0;
    np::array<int>::iterator it;
    for (it = array.begin(); it != array.end(); ++it) {
        *it = value++;
    }
    std::cout << array << "\n";
    return 0;
}
```

Output

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> matrix(3, 4);
    int value = 0;
    np::matrix<int>::iterator it;
    for (it = matrix.begin(); it != matrix.end(); ++it) {
        *it = value++;
    }
    std::cout << "Row-major:\n" << matrix << "\n";
    value = 0;
    for (it = matrix.begin(false); it != matrix.end(false); ++it) {
        *it = value++;
    }
    std::cout << "Column-major:\n" << matrix << "\n";
    return 0;
}
```

Output

```
Row-major:
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
Column-major:
[[ 0,  3,  6,  9],
 [ 1,  4,  7, 10],
 [ 2,  5,  8, 11]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 4);
    int value = 0;
    np::tensor<int, 3>::iterator it;
    for (it = cube.begin(); it != cube.end(); ++it) {
        *it = value++;
    }
    std::cout << "Row-major:\n" << cube << "\n";
    value = 0;
    for (it = cube.begin(false); it != cube.end(false); ++it) {
        *it = value++;
    }
    std::cout << "Column-major:\n" << cube << "\n";
    return 0;
}
```

Output

```
Row-major:
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
Column-major:
[[[ 0,  6, 12, 18],
  [ 2,  8, 14, 20],
  [ 4, 10, 16, 22]],

 [[ 1,  7, 13, 19],
  [ 3,  9, 15, 21],
  [ 5, 11, 17, 23]]]
```

### `tensor::end`

<h3><code>tensor_view::end</code></h3>

<h3><code>indirect_tensor::end</code></h3>

Returns an iterator pointing to the past-the-end element in the tensor. It does
not point to any element, and thus shall not be dereferenced.
```cpp
iterator end();
const_iterator end() const;

iterator end(bool row_major);
const_iterator end(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A random access iterator to the element past the end of the tensor. If the
tensor is const-qualified, the function returns a `const_iterator`. Otherwise,
it returns an `iterator`.

### `tensor::cbegin`

<h3><code>tensor_view::cbegin</code></h3>

<h3><code>indirect_tensor::cbegin</code></h3>

Returns a `const_iterator` pointing to the first element in the tensor.
```cpp
const_iterator cbegin() const;
const_iterator cbegin(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A `const_iterator` to the beginning of the tensor.

### `tensor::cend`

<h3><code>tensor_view::cend</code></h3>

<h3><code>indirect_tensor::cend</code></h3>

Returns a `const_iterator` pointing to the past-the-end element in the tensor.
```cpp
const_iterator cend() const;
const_iterator cend(bool row_major) const;
```

Parameters

* `row_major` It is an optional parameter that changes the order in which
elements are iterated. If provided, the elements are iterated in row-major
order (i.e., from first axis to last axis) or column-major order (i.e., from
last axis to first axis) as specified by `row_major`. Otherwise, the elements
are iterated in the same order as stored in memory.

Returns

* A `const_iterator` to the element past the end of the tensor.

### `tensor::iterator::base`

<h3><code>tensor_view::iterator::base</code></h3>

<h3><code>indirect_tensor::iterator::base</code></h3>

Accesses the underlying tensor.

```cpp
/// For iterator
tensor<T, Rank>* base() const;
tensor_view<T, Rank>* base() const;
indirect_tensor<T, Rank>* base() const;

/// For const_iterator
const tensor<T, Rank>* base() const;
const tensor_view<T, Rank>* base() const;
const indirect_tensor<T, Rank>* base() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> array{18, 13, 17, 17, 13, 14, -1, 2, 11, -3};
    np::array<int>::iterator it = array.begin();
    std::cout << *it.base() << "\n";
    return 0;
}

```

Output

```
[18, 13, 17, 17, 13, 14, -1,  2, 11, -3]
```

### `tensor::iterator::index`

<h3><code>tensor_view::iterator::index</code></h3>

<h3><code>indirect_tensor::iterator::index</code></h3>

Returns the underlying flat index.
```cpp
/// For both iterator and const_iterator
size_t index() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 2);
    np::tensor<int, 3>::iterator it;
    for (it = cube.begin(); it != cube.end(); ++it) {
        std::cout << it.index() << "\n";
    }
    return 0;
}

```

Output

```
0
1
2
3
4
5
6
7
8
9
10
11
```

### `tensor::iterator::coords`

<h3><code>tensor_view::iterator::coords</code></h3>

<h3><code>indirect_tensor::iterator::coords</code></h3>

Returns an `index_t` object with the current coordinates.
```cpp
/// For both iterator and const_iterator
index_t<Rank> coords() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> cube(2, 3, 2);
    np::tensor<int, 3>::iterator it;
    std::cout << "Row-major order:\n";
    for (it = cube.begin(); it != cube.end(); ++it) {
        std::cout << it.coords() << "\n";
    }
    std::cout << "Column-major order:\n";
    for (it = cube.begin(false); it != cube.end(false); ++it) {
        std::cout << it.coords() << "\n";
    }
    return 0;
}
```

Output

```
Row-major order:
(0, 0, 0)
(0, 0, 1)
(0, 1, 0)
(0, 1, 1)
(0, 2, 0)
(0, 2, 1)
(1, 0, 0)
(1, 0, 1)
(1, 1, 0)
(1, 1, 1)
(1, 2, 0)
(1, 2, 1)
Column-major order:
(0, 0, 0)
(1, 0, 0)
(0, 1, 0)
(1, 1, 0)
(0, 2, 0)
(1, 2, 0)
(0, 0, 1)
(1, 0, 1)
(0, 1, 1)
(1, 1, 1)
(0, 2, 1)
(1, 2, 1)
```

### `tensor::iterator::rowmajor`

<h3><code>tensor_view::iterator::rowmajor</code></h3>

<h3><code>indirect_tensor::iterator::rowmajor</code></h3>

Returns whether the elements are iterated in row-major order.
```cpp
/// For both iterator and const_iterator
bool rowmajor() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> matrix(3, 4);
    np::matrix<int>::iterator it1 = matrix.begin();
    np::matrix<int>::iterator it2 = matrix.begin(false);
    if (it1.rowmajor()) {
        std::cout << "it1 iterates in row-major order\n";
    }
    else {
        std::cout << "it1 iterates in column-major order\n";
    }
    if (it2.rowmajor()) {
        std::cout << "it2 iterates in row-major order\n";
    }
    else {
        std::cout << "it2 iterates in column-major order\n";
    }
    return 0;
}
```

Output

```
it1 iterates in row-major order
it2 iterates in column-major order
```

### `tensor::iterator::colmajor`

<h3><code>tensor_view::iterator::colmajor</code></h3>

<h3><code>indirect_tensor::iterator::colmajor</code></h3>

Returns whether the elements are iterated in column-major order.
```cpp
/// For both iterator and const_iterator
bool colmajor() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> matrix(3, 4);
    np::matrix<int>::iterator it1 = matrix.begin();
    np::matrix<int>::iterator it2 = matrix.begin(false);
    if (it1.colmajor()) {
        std::cout << "it1 iterates in column-major order\n";
    }
    else {
        std::cout << "it1 iterates in row-major order\n";
    }
    if (it2.colmajor()) {
        std::cout << "it2 iterates in column-major order\n";
    }
    else {
        std::cout << "it2 iterates in row-major order\n";
    }
    return 0;
}
```

Output

```
it1 iterates in row-major order
it2 iterates in column-major order
```

## Public methods

### `tensor::all`

<h3><code>tensor_view::all</code></h3>

<h3><code>indirect_tensor::all</code></h3>

Test whether all tensor elements evaluates to `true`.
```cpp
bool all() const;
```

Returns

* `true` if all the elements evaluates to `true` and `false` otherwise.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    np::array<bool> is_positive = (a > 0);
    std::cout << std::boolalpha << is_positive.all() << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
true
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
false
```

### `tensor::all(axes)`

<h3><code>tensor_view::all(axes)</code></h3>

<h3><code>indirect_tensor::all(axes)</code></h3>

Test whether all tensor elements over the given axes evaluates to `true`.
```cpp
tensor<bool, Rank> all(size_t axes) const;
template <size_t N>
tensor<bool, Rank> all(const shape_t<N> &axes) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the logical AND reduction
is performed.

Returns

* A new tensor with the results of the test over the axes. The output tensor
will have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::matrix<bool> is_positive = (a > 0);
    std::cout << std::boolalpha;
    std::cout << "Axis 0:\n" << is_positive.all(0) << "\n";
    std::cout << "Axis 1:\n" << is_positive.all(1) << "\n";
    return 0;
}
```

Input

```
[[13,  3, -1, 1,  3, -3],
 [13,  0, 11, 5, 12,  5],
 [ 1,  5, 13, 2, -3,  8],
 [ 5, 13, 11, 0,  4,  9]]
```

Output

```
Axis 0:
[[ true, false, false, false, false, false]]
Axis 1:
[[false],
 [false],
 [false],
 [false]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    np::tensor<bool, 3> is_positive = (a > 0);
    std::cout << std::boolalpha;
    std::cout << "Last 2 axes:\n" << is_positive.all(np::make_shape(1, 2));
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[11, 11 , 4,  0],
  [14,  9 , 9,  13],
  [10,  3, -1,  8]],
 [[14,  2, 10,  7],
  [ 5,  3 , 9,  1],
  [ 1,  1, 13, 10]]]
```

Output

```
Last 2 axes:
[[[false]],

 [[ true]]]
```

### `tensor::any`

<h3><code>tensor_view::any</code></h3>

<h3><code>indirect_tensor::any</code></h3>

Test whether any tensor element evaluates to `true`.
```cpp
bool any() const;
```

Returns

* `true` if any element evaluates to `true` and `false` otherwise.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    np::array<bool> is_negative = (a < 0);
    std::cout << std::boolalpha << is_negative.any() << "\n";
    return 0;
}
```

Input

```
[13, 17, 7, 5, 19, 6, 18, 5, 5, 17]
```

Output

```
false
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
true
```

### `tensor::any(axes)`

<h3><code>tensor_view::any(axes)</code></h3>

<h3><code>indirect_tensor::any(axes)</code></h3>

Test whether any tensor element over the given axes evaluates to `true`.
```cpp
tensor<bool, Rank> any(size_t axes) const;
template <size_t N>
tensor<bool, Rank> any(const shape_t<N> &axes) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the logical OR reduction
is performed.

Returns

* A new tensor with the results of the test over the axes. The output tensor
will have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::matrix<bool> is_negative = (a < 0);
    std::cout << std::boolalpha;
    std::cout << "Axis 0:\n" << is_negative.any(0) << "\n";
    std::cout << "Axis 1:\n" << is_negative.any(1) << "\n";
    return 0;
}
```

Input

```
[[13,  3, -1, 1,  3, -3],
 [13,  0, 11, 5, 12,  5],
 [ 1,  5, 13, 2, -3,  8],
 [ 5, 13, 11, 0,  4,  9]]
```

Output

```
Axis 0:
[[false, false,  true, false,  true,  true]]
Axis 1:
[[ true],
 [false],
 [ true],
 [false]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    np::tensor<bool, 3> is_negative = (a < 0);
    std::cout << std::boolalpha;
    std::cout << "Last 2 axes:\n" << is_negative.any(np::make_shape(1, 2));
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[11, 11 , 4,  0],
  [14,  9 , 9,  13],
  [10,  3, -1,  8]],
 [[14,  2, 10,  7],
  [ 5,  3 , 9,  1],
  [ 1,  1, 13, 10]]]
```

Output

```
Last 2 axes:
[[[ true]],

 [[false]]]
```

### `tensor::apply`

<h3><code>tensor_view::apply</code></h3>

<h3><code>indirect_tensor::apply</code></h3>

Assigns to each element the result of applying a function to the corresponding
elements in `*this`.
```cpp
void apply(T f(T));
void apply(T f(const T&));
template <class Function>
void apply(Function f);
```

Parameters

* `f` A function that accepts one element of type `T` as argument, and returns
a value convertible to `T`.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

int square(int x) {
    return x*x;
}

int main() {
    np::array<int> a;
    std::cin >> a;
    a.apply(square);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
[4, 2, -4, -2, 8, -5, 7, -4, 6, 3]
```

Output

```
[16,  4, 16,  4, 64, 25, 49, 16, 36,  9]
```

### `tensor::argmax`

<h3><code>tensor_view::argmax</code></h3>

<h3><code>indirect_tensor::argmax</code></h3>

Return the indices of the maximum value in the tensor.
```cpp
index_t<Rank> argmax() const;
```

Returns

* The indices of the maximum value in the tensor.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << a.argmax() << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
(2,)
```

### `tensor::argmax(axis)`

<h3><code>tensor_view::argmax(axis)</code></h3>

<h3><code>indirect_tensor::argmax(axis)</code></h3>

Return the indices of the maximum value in the tensor along the given axis.
```cpp
tensor<size_t, Rank> argmax(size_t axis) const;
```

Parameters

* `axis` Axis along which the maximum value is selected.

Returns

* A new tensor with the indices of the maximum value along an axis. The output
tensor will have the same dimension and the same shape, except that the axis
which is reduced is left as a dimension of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.argmax(0) << "\n";
    std::cout << "Axis 1:\n" << a.argmax(1) << "\n";
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
[[0, 3, 0, 0, 0, 2]]
Axis 1:
[[0],
 [0],
 [5],
 [1]]
```

### `tensor::argmin`

<h3><code>tensor_view::argmin</code></h3>

<h3><code>indirect_tensor::argmin</code></h3>

Return the indices of the minimum value in the tensor.
```cpp
index_t<Rank> argmin() const;
```

Returns

* The indices of the minimum value in the tensor.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << a.argmin() << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
(7,)
```

### `tensor::argmin(axis)`

<h3><code>tensor_view::argmin(axis)</code></h3>

<h3><code>indirect_tensor::argmin(axis)</code></h3>

Return the indices of the minimum value in the tensor along the given axis.
```cpp
tensor<size_t, Rank> argmin(size_t axis) const;
```

Parameters

* `axis` Axis along which the minimum value is selected.

Returns

* A new tensor with the indices of the minimum value along an axis. The output
tensor will have the same dimension and the same shape, except that the axis
which is reduced is left as a dimension of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.argmin(0) << "\n";
    std::cout << "Axis 1:\n" << a.argmin(1) << "\n";
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
[[2, 1, 2, 3, 1, 3]]
Axis 1:
[[2],
 [4],
 [2],
 [5]]
```

### `tensor::argpartition`

<h3><code>tensor_view::argpartition</code></h3>

<h3><code>indirect_tensor::argpartition</code></h3>

Return the indices that would partition the tensor.
```cpp
tensor<index_t<Rank>, 1> argpartition(size_t kth) const;
template <class Compare>
tensor<index_t<Rank>, 1> argpartition(size_t kth, Compare comp) const;
```

Parameters

* `kth` Element index to partition by. The element at the `kth` position is the
element that would be in that position in the sorted tensor. The other elements
are left without any specific order, except that none of the elements preceding
`kth` are greater than it, and none of the elements following it are less.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.

Returns

* A one-dimensional tensor of indices that partitions the tensor. If `a` is a
tensor, then `a[a.argpartition(kth)]` yields a partitioned `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    int kth;
    std::cin >> a >> kth;
    np::array<np::index_t<1> > indices = a.argpartition(kth);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned array:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
5
```

Output

```
Indices:
[(1,), (7,), (6,), (3,), (8,), (4,), (0,), (9,), (5,), (2,)]
Partitioned array:
[-2, -5, -3,  0,  3,  4, 12, 12, 18, 19]
```

### `tensor::argsort`

<h3><code>tensor_view::argsort</code></h3>

<h3><code>indirect_tensor::argsort</code></h3>

Return the indices that would sort the tensor.
```cpp
tensor<index_t<Rank>, 1> argsort(bool stable = false) const;
template <class Compare>
tensor<index_t<Rank>, 1>
argsort(Compare comp, bool stable = false) const;
```

Parameters

* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.
* `stable` If `true`, preserve the relative order of the elements with
equivalent values. Otherwise, equivalent elements are not guaranteed to keep
their original relative order.

Returns

* A one-dimensional tensor of indices that sort the tensor. If `a` is a tensor,
then `a[a.argsort()]` yields a sorted `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    np::array<np::index_t<1> > indices = a.argsort();
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted array:\n" << a[indices] << "\n";
    return 0;
}
```


Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
Indices:
[(7,), (6,), (1,), (3,), (8,), (4,), (0,), (9,), (5,), (2,)]
Sorted array:
[-5, -3, -2,  0,  3,  4, 12, 12, 18, 19]
```

### `tensor::astype`

<h3><code>tensor_view::astype</code></h3>

<h3><code>indirect_tensor::astype</code></h3>

Cast each element to a specified type.
```cpp
template <class U>
tensor<U, Rank> astype() const;
```

Template parameters

* `U` Data type to which the elements are casted.

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
    std::cout << a.astype<int>() << "\n";
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

### `tensor::clamp`

<h3><code>tensor_view::clamp</code></h3>

<h3><code>indirect_tensor::clamp</code></h3>

Clamp the values in the tensor. Given an interval [`a_min`, `a_max`], values
smaller than `a_min` become `a_min`, and values larger than `a_max` become
`a_max`. For complex types, the real and imaginary parts are clamped
separately.
```cpp
void clamp(const T &a_min, const T &a_max);
```

Parameters

* `a_min` The lower boundary to clamp.
* `a_max` The upper boundary to clamp.

Notes

* The behavior is undefined if `a_min` is greater than `a_max`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
    int a_min, a_max;
    std::cin >> a >> a_min >> a_max;
    a.clamp(a_min, a_max);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
[4, 12, 2, 0, 2, 10, -4, 6, 2, -4]
1 10
```

Output

```
[ 4, 10,  2,  1,  2, 10,  1,  6,  2,  1]
```

### `tensor::conj`

<h3><code>tensor_view::conj</code></h3>

<h3><code>indirect_tensor::conj</code></h3>

Return the complex conjugate, element-wise.
```cpp
tensor<T, Rank> conj() const;
```

Returns

* A light-weight object with the complex conjugate of each element in the
tensor. This function does not create a new tensor, instead, it returns a
readonly view with the complex conjugate of each element.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > a;
    std::cin >> a;
    std::cout << a.conj() << "\n";
    return 0;
}
```

Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[ 8+1i, 12+3i,  8-9i,  5-0i,  1-2i, 12-1i,  0+3i, -1-9i,  6-0i, 0-10i]
```

### `tensor::copy`

<h3><code>tensor_view::copy</code></h3>

<h3><code>indirect_tensor::copy</code></h3>

Return a copy of the tensor.
```cpp
tensor<typename std::remove_cv<T>::type, Rank> copy() const;
```

### `tensor::imag`

<h3><code>tensor_view::imag</code></h3>

<h3><code>indirect_tensor::imag</code></h3>

Return or set the imaginary part, element-wise. Non-complex types are treated
as complex numbers with zero imaginary part component.
```cpp
/// If T = std::complex<U>.
tensor<U, Rank> imag() const;
void imag(const tensor<U, Rank> &arg);
void imag(const U &val);

/// Non-complex types
tensor<T, Rank> imag() const;
```

Parameters

* `arg` A tensor-like object with the values to set the imaginary part to.
* `val` Value to set the imaginary part to.

Returns

* A light-weight object with the imaginary part of each element in the tensor.
This function does not create a new tensor, instead, it returns a readonly view
with the imaginary part of each element.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > a;
    std::cin >> a;
    std::cout << a.imag() << "\n";
    a.imag(0);
    std::cout << a << "\n";
    return 0;
}
```
Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[-1, -3,  9,  0,  2,  1, -3,  9,  0, 10]
[ 8+0i, 12+0i,  8+0i,  5+0i,  1+0i, 12+0i,  0+0i, -1+0i,  6+0i,  0+0i]
```

### `tensor::max`

<h3><code>tensor_view::max</code></h3>

<h3><code>indirect_tensor::max</code></h3>

Return the maximum value contained in the tensor.
```cpp
T max() const;
```

Returns

* The maximum value in the tensor.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << a.max() << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
19
```

### `tensor::max(axes)`

<h3><code>tensor_view::max(axes)</code></h3>

<h3><code>indirect_tensor::max(axes)</code></h3>

Return the maximum value contained in the tensor over the given axes.
```cpp
tensor<T, Rank> max(size_t axes) const;
template <size_t N>
tensor<T, Rank> max(const shape_t<N> &axes) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the maximum value is
selected.

Returns

* A new tensor with the maximum value over the axes. The output tensor will
have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.max(0) << "\n";
    std::cout << "Axis 1:\n" << a.max(1) << "\n";
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
[[13, 14,  2,  7,  7, 14]]
Axis 1:
[[13],
 [ 8],
 [14],
 [14]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.max(np::make_shape(1, 2));
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[ 6, 13, -5, -3],
  [14, -1, -3, 14],
  [ 8, 12, -5, -2]],
 [[ 1,  7, -4,  0],
  [13, 14,  8,  5],
  [ 1, -3, -4,  0]]]
```

Output

```
Last 2 axes:
[[[14]],

 [[14]]
```

### `tensor::mean`

<h3><code>tensor_view::mean</code></h3>

<h3><code>indirect_tensor::mean</code></h3>

Return the average of the tensor elements.
```cpp
T mean() const;
```

Returns

* The average of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
    std::cin >> a;
    std::cout << a.mean() << "\n";
    return 0;
}
```

Input

```
[12.83, -4.69, 2.15, -3.72, 9.76, 14.81, -1.39, 7.57, -0.85, -1.75]
```

Output

```
3.472
```

### `tensor::mean(axes)`

<h3><code>tensor_view::mean(axes)</code></h3>

<h3><code>indirect_tensor::mean(axes)</code></h3>

Return the average of the tensor elements over the given axes.
```cpp
tensor<T, Rank> mean(size_t axes) const;
template <size_t N>
tensor<T, Rank> mean(const shape_t<N> &axes) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the average is computed.

Returns

* A new tensor with the average over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.mean(0) << "\n";
    std::cout << "Axis 1:\n" << a.mean(1) << "\n";
    return 0;
}
```
Input

```
[[-3.45, -0.45,  0.24, -3.54, -2.92, 12.56],
 [-0.77,  3.49, 14.81,  3.38,  7.12, 11.98],
 [ 1.95, -0.84, 10.37,  5.75,  2.47,  9.41],
 [-0.81, -4.54, 11.09,  0.37,  8.12,  0.05]]
```

Output

```
Axis 0:
[[ -0.77, -0.585, 9.1275,   1.49, 3.6975,    8.5]]
Axis 1:
[[0.40666667],
 [ 6.6683333],
 [ 4.8516667],
 [      2.38]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.mean(np::make_shape(1, 2));
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[ 8.04,  8.93, -4.34, -1.15],
  [10.37, -3.23, 14.25,  5.74],
  [10.24, -4.75, -2.35,  3.02]],
 [[-3.24, 10.34, 11.09,  6.01],
  [14.1 , -4.23, -2.39,  5.94],
  [ 2.72,  8.91,  2.32,  8.76]]]
```

Output

```
Last 2 axes:
[[[3.7308333]],

 [[   5.0275]]]
```

### `tensor::min`

<h3><code>tensor_view::min</code></h3>

<h3><code>indirect_tensor::min</code></h3>

Return the minimum value contained in the tensor.
```cpp
T min() const;
```

Returns

* The minimum value in the tensor.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << a.min() << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
-5
```

### `tensor::min(axes)`

<h3><code>tensor_view::min(axes)</code></h3>

<h3><code>indirect_tensor::min(axes)</code></h3>

Return the minimum value contained in the tensor over the given axes.
```cpp
tensor<T, Rank> min(size_t axes) const;
template <size_t N>
tensor<T, Rank> min(const shape_t<N> &axes) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the minimum value is
selected.

Returns

* A new tensor with the minimum value over the axes. The output tensor will
have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.min(0) << "\n";
    std::cout << "Axis 1:\n" << a.min(1) << "\n";
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
[[ 3,  2, -2,  1, -5, -3]]
Axis 1:
[[ 2],
 [-5],
 [-2],
 [-3]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.min(np::make_shape(1, 2));
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[ 6, 13, -5, -3],
  [14, -1, -3, 14],
  [ 8, 12, -5, -2]],
 [[ 1,  7, -4,  0],
  [13, 14,  8,  5],
  [ 1, -3, -4,  0]]]
```

Output

```
Last 2 axes:
[[[-5]],

 [[-4]]]
```

### `tensor::nonzero`

<h3><code>tensor_view::nonzero</code></h3>

<h3><code>indirect_tensor::nonzero</code></h3>

Return the indices of the elements that are non-zero.
```cpp
tensor<index_t<Rank>, 1> nonzero() const;
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    np::array<bool> is_negative = (a < 0);
    np::array<np::index_t<1> > indices = is_negative.nonzero();
    std::cout << "Indices: " << indices << "\n";
    std::cout << "Values: " << a[indices] << "\n";
    return 0;
}
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
Indices: [(4,), (6,), (8,)]
Values: [-3, -3, -1]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::matrix<bool> is_negative = (a < 0);
    np::array<np::index_t<2> > indices = is_negative.nonzero();
    std::cout << "Indices: " << indices << "\n";
    std::cout << "Values: " << a[indices] << "\n";
    return 0;
}
```

Input

```
[[13,  3, -1, 1,  3, -3],
 [13,  0, 11, 5, 12,  5],
 [ 1,  5, 13, 2, -3,  8],
 [ 5, 13, 11, 0,  4,  9]]
```

Output

```
Indices: [(0, 2), (0, 5), (2, 4)]
Values: [-1, -3, -3]
```

### `tensor::partition`

<h3><code>tensor_view::partition</code></h3>

<h3><code>indirect_tensor::partition</code></h3>

Partition the elements in-place.
```cpp
void partition(size_t kth, size_t axis = Rank - 1);
template <class Compare>
void partition(size_t kth, size_t axis, Compare comp);
```

Parameters

* `kth` Element index to partition by. The element at the `kth` position is the
element that would be in that position in the sorted tensor. The other elements
are left without any specific order, except that none of the elements preceding
`kth` are greater than it, and none of the elements following it are less.
* `axis` Axis along which to partition. Defaults to `Rank - 1`, which means
partition along the last axis.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    int kth;
    std::cin >> a >> kth;
    a.partition(kth);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
5
```

Output

```
[-2, -5, -3,  0,  3,  4, 12, 12, 18, 19]
```

### `tensor::prod`

<h3><code>tensor_view::prod</code></h3>

<h3><code>indirect_tensor::prod</code></h3>

Return the product of the tensor elements.
```cpp
T prod() const;
```

Returns

* The product of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << a.prod() << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
86400
```

### `tensor::prod(axes)`

<h3><code>tensor_view::prod(axes)</code></h3>

<h3><code>indirect_tensor::prod(axes)</code></h3>

Return the product of the tensor elements over the given axes.
```cpp
tensor<T, Rank> prod(size_t axes) const;
template <size_t N>
tensor<T, Rank> prod(const shape_t<N> &axes) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the product is performed.

Returns

* A new tensor with the product over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.prod(0) << "\n";
    std::cout << "Axis 1:\n" << a.prod(1) << "\n";
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
Axis 0:
[[ 336,   54,  450, 1050,  315, 3780]]
Axis 1:
[[19440],
 [13230],
 [ 1470],
 [27000]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.prod(np::make_shape(1, 2));
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
[[[ 204120]],

 [[3628800]]]
```

### `tensor::real`

<h3><code>tensor_view::real</code></h3>

<h3><code>indirect_tensor::real</code></h3>

Return or set the real part, element-wise. Non-complex types are treated as
complex numbers with zero imaginary part component.
```cpp
/// If T = std::complex<U>.
tensor<U, Rank> real() const;
void real(const tensor<U, Rank> &arg);
void real(const U &val);

/// Non-complex types
tensor<T, Rank> real() const;
```

Parameters

* `arg` A tensor-like object with the values to set the real part to.
* `val` Value to set the real part to.

Returns

* A light-weight object with the real part of each element in the tensor. This
function does not create a new tensor, instead, it returns a readonly view with
the real part of each element.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<std::complex<double> > a;
    std::cin >> a;
    std::cout << a.real() << "\n";
    a.real(0);
    std::cout << a << "\n";
    return 0;
}
```
Input

```
[8-1i, 12-3i, 8+9i, 5, 1+2i, 12+1i, -3i, -1+9i, 6, 10i]
```

Output

```
[ 8, 12,  8,  5,  1, 12,  0, -1,  6,  0]
[ 0-1i,  0-3i,  0+9i,  0+0i,  0+2i,  0+1i,  0-3i,  0+9i,  0+0i, 0+10i]
```

### `tensor::reverse`

<h3><code>tensor_view::reverse</code></h3>

<h3><code>indirect_tensor::reverse</code></h3>

Reverse the order of the elements in-place.
```cpp
void reverse(size_t axis = Rank - 1);
```

Parameters

* `axis` Axis along which to reverse. Defaults to `Rank - 1`, which means
reverse along the last axis.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    a.reverse();
    std::cout << a << "\n";
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

### `tensor::sort`

<h3><code>tensor_view::reverse</code></h3>

<h3><code>indirect_tensor::reverse</code></h3>

Sort the elements in-place.
```cpp
void sort(size_t axis = Rank - 1, bool stable = false);
template <class Compare>
void sort(size_t axis, Compare comp, bool stable = false);
```

Parameters

* `axis` Axis along which to sort. Defaults to `Rank - 1`, which means sort
along the last axis.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.
* `stable` If `true`, preserve the relative order of the elements with
equivalent values. Otherwise, equivalent elements are not guaranteed to keep
their original relative order.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    a.sort();
    std::cout << a << "\n";
    return 0;
}
```

nput

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
[-5, -3, -2,  0,  3,  4, 12, 12, 18, 19]
```

### `tensor::stddev`

<h3><code>tensor_view::stddev</code></h3>

<h3><code>indirect_tensor::stddev</code></h3>

Return the standard deviation of the tensor elements.
```cpp
T stddev(size_t ddof = 0) const;
```

The standard deviation is defined as the square root of the average of the
squared deviations from the mean

```
    stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
```

The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `ddof`
is specified, the divisor `n - ddof` is used instead of `n`. In statistics,
`ddof = 1` provides an unbiased estimator of the sample variance; while
`ddof = 0` provides the maximum likelihood estimator of the variance for
normally distributed variables.

Parameters

* `ddof` Delta degrees of freedom.

Returns

* The standard deviation of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
    std::cin >> a;
    std::cout << a.stddev() << "\n";
    return 0;
}
```

Input

```
[12.83, -4.69, 2.15, -3.72, 9.76, 14.81, -1.39, 7.57, -0.85, -1.75]
```

Output

```
6.79804
```

### `tensor::stddev(axes)`

<h3><code>tensor_view::stddev(axes)</code></h3>

<h3><code>indirect_tensor::stddev(axes)</code></h3>

Return the standard deviation of the tensor elements over the given axes.
```cpp
tensor<T, Rank> stddev(size_t axes, size_t ddof) const;
template <size_t N>
tensor<T, Rank> stddev(const shape_t<N> &axes, size_t ddof) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the standard deviation is
computed.
* `ddof` Delta degrees of freedom.

Returns

* A new tensor with the standard deviation over the axes. The output tensor
will have the same dimension and the same shape, except that the axes which are
reduced are left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.stddev(0, 0) << "\n";
    std::cout << "Axis 1:\n" << a.stddev(1, 0) << "\n";
    return 0;
}
```

Input

```
[[-3.45, -0.45,  0.24, -3.54, -2.92, 12.56],
 [-0.77,  3.49, 14.81,  3.38,  7.12, 11.98],
 [ 1.95, -0.84, 10.37,  5.75,  2.47,  9.41],
 [-0.81, -4.54, 11.09,  0.37,  8.12,  0.05]]
```

Output

```
Axis 0:
[[1.9093454, 2.8430134, 5.4007795, 3.4740107, 4.3752164, 5.0205727]]
Axis 1:
[[5.6270468],
 [5.3377318],
 [4.0522521],
 [5.4213037]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.stddev(np::make_shape(1, 2), 0);
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[ 8.04,  8.93, -4.34, -1.15],
  [10.37, -3.23, 14.25,  5.74],
  [10.24, -4.75, -2.35,  3.02]],
 [[-3.24, 10.34, 11.09,  6.01],
  [14.1 , -4.23, -2.39,  5.94],
  [ 2.72,  8.91,  2.32,  8.76]]]
```

Output

```
Last 2 axes:
[[[6.4132551]],

 [[5.7721849]]]
```

### `tensor::sum`

<h3><code>tensor_view::sum</code></h3>

<h3><code>indirect_tensor::sum</code></h3>

Return the sum of the tensor elements.
```cpp
T sum() const;
```

Returns

* The sum of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << a.sum() << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
44
```

### `tensor::sum(axes)`

<h3><code>tensor_view::sum(axes)</code></h3>

<h3><code>indirect_tensor::sum(axes)</code></h3>

Return the sum of the tensor elements over the given axes.
```cpp
tensor<T, Rank> sum(size_t axes) const;
template <size_t N>
tensor<T, Rank> sum(const shape_t<N> &axes) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the sum is performed.

Returns

* A new tensor with the sum over the axes. The output tensor will have the same
dimension and the same shape, except that the axes which are reduced are left
as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.sum(0) << "\n";
    std::cout << "Axis 1:\n" << a.sum(1) << "\n";
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
Axis 0:
[[20, 15, 21, 23, 18, 32]]
Axis 1:
[[34],
 [33],
 [25],
 [37]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.sum(np::make_shape(1, 2));
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[2, 3, 9, 1],
  [3, 2, 1, 5],
  [7, 3, 3, 2]],
 [[8, 3, 2, 4],
  [6, 1, 2, 3],
  [3, 5, 7, 5]]]
```

Output

```
Last 2 axes:
[[[41]],

 [[49]]]
```

### `tensor::var`

<h3><code>tensor_view::var</code></h3>

<h3><code>indirect_tensor::var</code></h3>

Return the variance of the tensor elements.
```cpp
T var(size_t ddof = 0) const;
```

The variance is defined as the average of the squared deviations from the mean

```
    var(a) = mean(x),    x = abs(a - mean(a))**2
```

The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `ddof`
is specified, the divisor `n - ddof` is used instead of `n`. In statistics,
`ddof = 1` provides an unbiased estimator of the sample variance; while
`ddof = 0` provides the maximum likelihood estimator of the variance for
normally distributed variables.

Parameters

* `ddof` Delta degrees of freedom.

Returns

* The variance of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<double> a;
    std::cin >> a;
    std::cout << a.var() << "\n";
    return 0;
}
```

Input

```
[12.83, -4.69, 2.15, -3.72, 9.76, 14.81, -1.39, 7.57, -0.85, -1.75]
```

Output

```
46.2134
```

### `tensor::var(axes)`

<h3><code>tensor_view::var(axes)</code></h3>

<h3><code>indirect_tensor::var(axes)</code></h3>

Return the variance of the tensor elements over the given axes.
```cpp
tensor<T, Rank> var(size_t axes, size_t ddof) const;
template <size_t N>
tensor<T, Rank> var(const shape_t<N> &axes, size_t ddof) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the variance is computed.
* `ddof` Delta degrees of freedom.

Returns

* A new tensor with the variance over the axes. The output tensor will have the
same dimension and the same shape, except that the axes which are reduced are
left as dimensions of size one.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << a.var(0, 0) << "\n";
    std::cout << "Axis 1:\n" << a.var(1, 0) << "\n";
    return 0;
}
```

Input

```
[[-3.45, -0.45,  0.24, -3.54, -2.92, 12.56],
 [-0.77,  3.49, 14.81,  3.38,  7.12, 11.98],
 [ 1.95, -0.84, 10.37,  5.75,  2.47,  9.41],
 [-0.81, -4.54, 11.09,  0.37,  8.12,  0.05]]
```

Output

```
Axis 0:
[[   3.6456,  8.082725, 29.168419,  12.06875, 19.142519,  25.20615]]
Axis 1:
[[31.663656],
 [28.491381],
 [16.420747],
 [29.390533]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.var(np::make_shape(1, 2), 0);
    std::cout << "\n";
    return 0;
}
```

Input

```
[[[ 8.04,  8.93, -4.34, -1.15],
  [10.37, -3.23, 14.25,  5.74],
  [10.24, -4.75, -2.35,  3.02]],
 [[-3.24, 10.34, 11.09,  6.01],
  [14.1 , -4.23, -2.39,  5.94],
  [ 2.72,  8.91,  2.32,  8.76]]]
```

Output

```
Last 2 axes:
[[[41.129841]],

 [[33.318119]]]
```
