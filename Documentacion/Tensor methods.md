# Tensor methods

Defined in `numcpp/tensor.h`

- [Tensor methods](#tensor-methods)
  - [Template parameters](#template-parameters)
  - [Complex numbers](#complex-numbers)
    - [`tensor::real`](#tensorreal)
    - [`tensor::real(val)`](#tensorrealval)
    - [`tensor::imag`](#tensorimag)
    - [`tensor::imag(val)`](#tensorimagval)
    - [`tensor::conj`](#tensorconj)
  - [Miscellaneous](#miscellaneous)
    - [`tensor::apply`](#tensorapply)
    - [`tensor::astype`](#tensorastype)
    - [`tensor::clamp`](#tensorclamp)
    - [`tensor::copy`](#tensorcopy)
    - [`tensor::diagonal`](#tensordiagonal)
    - [`tensor::view`](#tensorview)
  - [Sorting and searching](#sorting-and-searching)
    - [`tensor::argpartition`](#tensorargpartition)
    - [`tensor::argsort`](#tensorargsort)
    - [`tensor::nonzero`](#tensornonzero)
    - [`tensor::partition(axis)`](#tensorpartitionaxis)
    - [`tensor::reverse(axis)`](#tensorreverseaxis)
    - [`tensor::rotate(axis)`](#tensorrotateaxis)
    - [`tensor::sort(axis)`](#tensorsortaxis)
  - [Reductions](#reductions)
    - [`tensor::all`](#tensorall)
    - [`tensor::all(axes)`](#tensorallaxes)
    - [`tensor::any`](#tensorany)
    - [`tensor::any(axes)`](#tensoranyaxes)
    - [`tensor::argmax`](#tensorargmax)
    - [`tensor::argmax(axis)`](#tensorargmaxaxis)
    - [`tensor::argmin`](#tensorargmin)
    - [`tensor::argmin(axis)`](#tensorargminaxis)
    - [`tensor::max`](#tensormax)
    - [`tensor::max(axes)`](#tensormaxaxes)
    - [`tensor::mean`](#tensormean)
    - [`tensor::mean(axes)`](#tensormeanaxes)
    - [`tensor::min`](#tensormin)
    - [`tensor::min(axes)`](#tensorminaxes)
    - [`tensor::prod`](#tensorprod)
    - [`tensor::prod(axes)`](#tensorprodaxes)
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

## Complex numbers

The following methods are defined only for complex-valued tensors such as
`tensor<std::complex<T>, Rank>`, `tensor_view<std::complex<T>, Rank>` and
`indirect_tensor<std::complex<T>, Rank>`.

### `tensor::real`

<h3><code>tensor_view::real</code></h3>

<h3><code>indirect_tensor::real</code></h3>

Return the real part, element-wise.
```cpp
tensor<T, Rank> real() const;
```

Parameters

* None

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
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << z.real() << "\n";
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
```

### `tensor::real(val)`

<h3><code>tensor_view::real(val)</code></h3>

<h3><code>indirect_tensor::real(val)</code></h3>

Set the real part, element-wise.
```cpp
void real(const tensor<T, Rank> &arg);
void real(const T &val);
```

Parameters

* `arg` A tensor-like object with the values to set the real part to.
* `val` Value to set the real part to.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shapes are different.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<double> x;
    std::cin >> x;
    np::array<std::complex<double> > z(x.size(), 0);
    z.real(x);
    std::cout << z << "\n";
    return 0;
}
```

Input

```
[8, 12, 8, 5, 1, 12, 0, -1, 6, 0]
```

Output

```
[ 8+0i, 12+0i,  8+0i,  5+0i,  1+0i, 12+0i,  0+0i, -1+0i,  6+0i,  0+0i]
```

### `tensor::imag`

<h3><code>tensor_view::imag</code></h3>

<h3><code>indirect_tensor::imag</code></h3>

Return the imaginary part, element-wise.
```cpp
tensor<T, Rank> imag() const;
```

Parameters

* None

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
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << z.imag() << "\n";
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
```

### `tensor::imag(val)`

<h3><code>tensor_view::imag(val)</code></h3>

<h3><code>indirect_tensor::imag(val)</code></h3>

Set the imaginary part, element-wise.
```cpp
void imag(const tensor<T, Rank> &arg);
void imag(const T &val);
```

Parameters

* `arg` A tensor-like object with the values to set the imaginary part to.
* `val` Value to set the imaginary part to.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shapes are different.

Example

```cpp
#include <iostream>
#include <complex>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    complexmode = complexmode_t::algebraic;
    np::array<double> y;
    std::cin >> y;
    np::array<std::complex<double> > z(y.size(), 0);
    z.imag(y);
    std::cout << z << "\n";
    return 0;
}
```

Input

```
[-1, -3, 9, 0, 2, 1, -3, 9, 0, 10]
```

Output

```
[ 0-1i,  0-3i,  0+9i,  0+0i,  0+2i,  0+1i,  0-3i,  0+9i,  0+0i, 0+10i]
```

### `tensor::conj`

<h3><code>tensor_view::conj</code></h3>

<h3><code>indirect_tensor::conj</code></h3>

Return the complex conjugate, element-wise.
```cpp
tensor<std::complex<T>, Rank> conj() const;
```

Parameters

* None

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
    np::array<std::complex<double> > z;
    std::cin >> z;
    std::cout << z.conj() << "\n";
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

## Miscellaneous

### `tensor::apply`

<h3><code>tensor_view::apply</code></h3>

<h3><code>indirect_tensor::apply</code></h3>

Assigns to each element the result of applying a function to the corresponding
elements in `*this`.
```cpp
void apply(T f(T));
void apply(T f(const T&));
template <class Function>
void apply(Function &&f);
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

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

struct iota_t {
    int value;

    iota_t(int value = 0) : value(value) {}

    int operator()(int) {
        return value++;
    }
};

int main() {
    int n;
    std::cin >> n;
    np::array<int> a(n);
    // Unlike function pointers, functors allows to contain internal state of
    // the function.
    a.apply(iota_t());
    std::cout << a << "\n";
    return 0;
}
```

Input

```
10
```

Output

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
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
[2.01, -3.62, -4.97, 6.77, 8.33, 5.93, 0.49, 7.8, 14.8, -2.3]
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

Returns

* None

Notes

* The behavior is undefined if `a_min` is greater than `a_max`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
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

### `tensor::copy`

<h3><code>tensor_view::copy</code></h3>

<h3><code>indirect_tensor::copy</code></h3>

Return a copy of the tensor.
```cpp
tensor<T> copy() const;
```

Parameters

* None

Returns

* A new tensor with a copy of each of the elements in the tensor, in the same
order.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

### `tensor::diagonal`

<h3><code>tensor_view::diagonal</code></h3>

Return a view of the diagonal.
```cpp
tensor_view<T, 1> diagonal(ptrdiff_t k = 0);
tensor_view<const T, 1> diagonal(ptrdiff_t k = 0) const;
```

Parameters

* `k` Offset of the diagonal from the main diagonal. A positive value refers to
an upper diagonal and a negative value refers to a lower diagonal. Defaults to
0 (main diagonal).

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to
`const T`, which is convertible to a tensor object. Otherwise, the function
returns a `tensor_view` to `T`, which has reference semantics to the original
tensor.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Main diagonal: " << a.diagonal() << "\n";
    std::cout << "Sub diagonal: " << a.diagonal(-1) << "\n";
    std::cout << "Sup diagonal: " << a.diagonal(1) << "\n";
    return 0;
}
```

Input

```
[[-3, -3,  0, -5, 15],
 [13,  7,  0, -4, 10],
 [17, -3,  7, -5,  9],
 [ 7, -4, 10, -4,  2],
 [16,  1,  3,  9, 18]]
```

Output

```
Main diagonal: [-3,  7,  7, -4, 18]
Sub diagonal: [13, -3, 10,  9]
Sup diagonal: [-3,  0, -5,  2]
```

### `tensor::view`

Return a view of the tensor with the same data.
```cpp
tensor_view<T, Rank> view();
tensor_view<const T, Rank> view() const;
```

Parameters

* None

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to
`const T`, which is convertible to a tensor object. Otherwise, the function
returns a `tensor_view` to `T`, which has reference semantics to the original
tensor.

## Sorting and searching

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
    // Explicit conversion from index_t<1>.
    np::array<size_t> indices = a.argpartition(kth);
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
[1, 7, 6, 3, 8, 4, 0, 9, 5, 2]
Partitioned array:
[-2, -5, -3,  0,  3,  4, 12, 12, 18, 19]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int kth;
    std::cin >> a >> kth;
    np::array<np::index_t<2> > indices = a.argpartition(kth);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned array:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[[19,  3, -5,  2],
 [ 2, 16, -4,  8],
 [ 3, 13,  9,  7]]
6
```

Output

```
Indices:
[(0, 1), (2, 0), (0, 2), (0, 3), (1, 0), (1, 2), (2, 3), (1, 3), (0, 0), (2, 1),
 (2, 2), (1, 1)]
Partitioned array:
[ 3,  3, -5,  2,  2, -4,  7,  8, 19, 13,  9, 16]
```

### `tensor::argsort`

<h3><code>tensor_view::argsort</code></h3>

<h3><code>indirect_tensor::argsort</code></h3>

Return the indices that would sort the tensor.
```cpp
tensor<index_t<Rank>, 1> argsort() const;
template <class Compare>
tensor<index_t<Rank>, 1> argsort(Compare comp, bool stable = false) const;
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
    // Explicit conversion from index_t<1>.
    np::array<size_t> indices = a.argsort();
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
[7, 6, 1, 3, 8, 4, 0, 9, 5, 2]
Sorted array:
[-5, -3, -2,  0,  3,  4, 12, 12, 18, 19]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::array<np::index_t<2> > indices = a.argsort();
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted array:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[[19,  3, -5,  2],
 [ 2, 16, -4,  8],
 [ 3, 13,  9,  7]]
```

Output

```
Indices:
[(0, 2), (1, 2), (0, 3), (1, 0), (0, 1), (2, 0), (2, 3), (1, 3), (2, 2), (2, 1),
 (1, 1), (0, 0)]
Sorted array:
[-5, -4,  2,  2,  3,  3,  7,  8,  9, 13, 16, 19]
```

### `tensor::nonzero`

<h3><code>tensor_view::nonzero</code></h3>

<h3><code>indirect_tensor::nonzero</code></h3>

Return the indices of the elements that are non-zero.
```cpp
tensor<index_t<Rank>, 1> nonzero() const;
```

Parameters

* None

Returns

* A new tensor with the indices of the elements that are non-zero.

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
    np::array<bool> is_positive = (a > 0);
    // Explicit conversion from index_t<1>.
    np::array<size_t> indices = is_positive.nonzero();
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[7, 0, 5, 11, -3, 0, -3, 8, -1, 14]
```

Output

```
Indices:
[0, 2, 3, 7, 9]
Values:
[ 7,  5, 11,  8, 14]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::matrix<bool> is_positive = (a > 0);
    np::array<np::index_t<2> > indices = is_positive.nonzero();
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
```

Output

```
Indices:
[(0, 0), (0, 1), (0, 2), (0, 3), (0, 4), (0, 5), (1, 2), (1, 3), (1, 4), (1, 5),
 (2, 0), (2, 1), (2, 2), (2, 4), (2, 5), (3, 0), (3, 2), (3, 3), (3, 4)]
Values:
[ 6,  3,  4, 13,  5, 12,  9,  7, 14,  9,  8, 11,  9,  7,  4,  1,  6,  9, 14]
```

### `tensor::partition(axis)`

<h3><code>tensor_view::partition(axis)</code></h3>

<h3><code>indirect_tensor::partition(axis)</code></h3>

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

Returns

* None

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

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int kth;
    std::cin >> a >> kth;
    a.partition(kth, 0);
    std::cout << "Axis 0:\n" << a << "\n";
    std::cin >> kth;
    a.partition(kth, 1);
    std::cout << "Axis 1:\n" << a << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
1
2
```

Output

```
Axis 0:
[[-1, -4, -3,  1, -2, -5],
 [ 9, -3, -1,  2,  0, -3],
 [11,  3, 12,  4,  3, 15],
 [10, 18, 15, 13, 18, -1]]
Axis 1:
[[-4, -5, -3, -2, -1,  1],
 [-3, -3, -1,  0,  2,  9],
 [ 3,  3,  4, 11, 12, 15],
 [-1, 10, 13, 15, 18, 18]]
```

### `tensor::reverse(axis)`

<h3><code>tensor_view::reverse(axis)</code></h3>

<h3><code>indirect_tensor::reverse(axis)</code></h3>

Reverse the order of the elements in-place.
```cpp
void reverse(size_t axis = Rank - 1);
```

Parameters

* `axis` Axis along which to reverse. Defaults to `Rank - 1`, which means
reverse along the last axis.

Returns

* None

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

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    a.reverse(0);
    std::cout << "Axis 0:\n" << a << "\n";
    a.reverse(1);
    std::cout << "Axis 1:\n" << a << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
```

Output

```
Axis 0:
[[-1, -3, 12,  2, 18, -1],
 [ 9,  3, -1,  4,  3, -3],
 [10, -4, 15,  1,  0, -5],
 [11, 18, -3, 13, -2, 15]]
Axis 1:
[[-1, 18,  2, 12, -3, -1],
 [-3,  3,  4, -1,  3,  9],
 [-5,  0,  1, 15, -4, 10],
 [15, -2, 13, -3, 18, 11]]
```

### `tensor::rotate(axis)`

<h3><code>tensor_view::rotate(axis)</code></h3>

<h3><code>indirect_tensor::rotate(axis)</code></h3>

Rotate the elements in-place. The elements are shifted circularly in such a way
that the element at position `shift` becomes the first element and the element
at position `shift - 1` becomes the last element.
```cpp
void rotate(size_t shift, size_t axis = Rank - 1);
```

Parameters

* `shift` Number of positions to shift the elements by.
* `axis` Axis along which to rotate. Defaults to `Rank - 1`, which means rotate
along the last axis.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
    int shift;
    std::cin >> a >> shift;
    a.rotate(shift);
    std::cout << a << "\n";
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int shift;
    std::cin >> a >> shift;
    a.rotate(shift, 0);
    std::cout << "Axis 0:\n" << a << "\n";
    std::cin >> shift;
    a.rotate(shift, 1);
    std::cout << "Axis 1:\n" << a << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
3
2
```

Output

```
Axis 0:
[[-1, -3, 12,  2, 18, -1],
 [11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3]]
Axis 1:
[[12,  2, 18, -1, -1, -3],
 [-3, 13, -2, 15, 11, 18],
 [15,  1,  0, -5, 10, -4],
 [-1,  4,  3, -3,  9,  3]]
```

### `tensor::sort(axis)`

<h3><code>tensor_view::sort(axis)</code></h3>

<h3><code>indirect_tensor::sort(axis)</code></h3>

Sort the elements in-place.
```cpp
void sort(size_t axis = Rank - 1);
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

Returns

* None

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

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
[-5, -3, -2,  0,  3,  4, 12, 12, 18, 19]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    a.sort(0);
    std::cout << "Axis 0:\n" << a << "\n";
    a.sort(1);
    std::cout << "Axis 1:\n" << a << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
```

Output

```
Axis 0:
[[-1, -4, -3,  1, -2, -5],
 [ 9, -3, -1,  2,  0, -3],
 [10,  3, 12,  4,  3, -1],
 [11, 18, 15, 13, 18, 15]]
Axis 1:
[[-5, -4, -3, -2, -1,  1],
 [-3, -3, -1,  0,  2,  9],
 [-1,  3,  3,  4, 10, 12],
 [11, 13, 15, 15, 18, 18]]
```

## Reductions

### `tensor::all`

<h3><code>tensor_view::all</code></h3>

<h3><code>indirect_tensor::all</code></h3>

Test whether all tensor elements evaluates to `true`.
```cpp
bool all() const;
```

Parameters

* None

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
tensor<bool, Rank> all(size_t axis) const;
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
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
```

Output

```
Axis 0:
[[false, false,  true, false,  true, false]]
Axis 1:
[[ true],
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
    std::cout << "Last 2 axes:\n" << std::boolalpha;
    std::cout << is_positive.all(np::make_shape(1, 2)) << "\n";
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

Parameters

* None

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
tensor<bool, Rank> any(size_t axis) const;
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
[[ 6,  3,  4, 13,  5, 12],
 [ 0, -4,  9,  7, 14,  9],
 [ 8, 11,  9, -2,  7,  4],
 [ 1, -2,  6,  9, 14, -2]]
```

Output

```
Axis 0:
[[false,  true, false,  true, false,  true]]
Axis 1:
[[false],
 [ true],
 [ true],
 [ true]]
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
    std::cout << "Last 2 axes:\n" << std::boolalpha;
    std::cout << is_negative.any(np::make_shape(1, 2)) << "\n";
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

### `tensor::argmax`

<h3><code>tensor_view::argmax</code></h3>

<h3><code>indirect_tensor::argmax</code></h3>

Return the indices of the maximum value in the tensor.
```cpp
index_t<Rank> argmax() const;
```

Parameters

* None

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

Parameters

* None

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

### `tensor::max`

<h3><code>tensor_view::max</code></h3>

<h3><code>indirect_tensor::max</code></h3>

Return the maximum value contained in the tensor.
```cpp
T max() const;
```

Parameters

* None

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
tensor<T, Rank> max(size_t axis) const;
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
    std::cout << "Last 2 axes:\n" << a.max(np::make_shape(1, 2)) << "\n";
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

Parameters

* None

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
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
4.4
```

### `tensor::mean(axes)`

<h3><code>tensor_view::mean(axes)</code></h3>

<h3><code>indirect_tensor::mean(axes)</code></h3>

Return the average of the tensor elements over the given axes.
```cpp
tensor<T, Rank> mean(size_t axis) const;
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
[[ 8, 3,  9,  5,  3,  6],
 [ 7, 2,  5,  7,  3,  9],
 [ 3, 1,  2,  5,  7,  7],
 [ 2,  9, 5,  6,  5, 10]]
```

Output

```
Axis 0:
[[   5, 3.75, 5.25, 5.75,  4.5,    8]]
Axis 1:
[[5.6666667],
 [      5.5],
 [4.1666667],
 [6.1666667]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n" << a.mean(np::make_shape(1, 2)) << "\n";
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
[[[3.4166667]],

 [[4.0833333]]]
```

### `tensor::min`

<h3><code>tensor_view::min</code></h3>

<h3><code>indirect_tensor::min</code></h3>

Return the minimum value contained in the tensor.
```cpp
T min() const;
```

Parameters

* None

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
tensor<T, Rank> min(size_t axis) const;
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
    std::cout << "Last 2 axes:\n" << a.min(np::make_shape(1, 2)) << "\n";
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

### `tensor::prod`

<h3><code>tensor_view::prod</code></h3>

<h3><code>indirect_tensor::prod</code></h3>

Return the product of the tensor elements.
```cpp
T prod() const;
```

Parameters

* None

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
tensor<T, Rank> prod(size_t axis) const;
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
    std::cout << "Last 2 axes:\n" << a.prod(np::make_shape(1, 2)) << "\n";
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

### `tensor::stddev`

<h3><code>tensor_view::stddev</code></h3>

<h3><code>indirect_tensor::stddev</code></h3>

Return the standard deviation of the tensor elements.
```cpp
T stddev(bool bias) const;
```

The standard deviation is defined as the square root of the average of the
squared deviations from the mean
```
    stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
```
The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias`
is `false`, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1`
provides an unbiased estimator of the sample variance; while `n` provides the
maximum likelihood estimator of the variance for normally distributed
variables.

Parameters

* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

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
    std::cout << "Unbiased: " << a.stddev(false) << "\n";
    std::cout << "Biased: " << a.stddev(true) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
Unbiased: 4.27395
Biased: 4.05463
```

### `tensor::stddev(axes)`

<h3><code>tensor_view::stddev(axes)</code></h3>

<h3><code>indirect_tensor::stddev(axes)</code></h3>

Return the standard deviation of the tensor elements over the given axes.
```cpp
tensor<T, Rank> stddev(size_t axis, bool bias) const;
template <size_t N>
tensor<T, Rank> stddev(const shape_t<N> &axes, bool bias) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the standard deviation is
computed.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

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
    std::cout << "Axis 0:\n";
    std::cout << "Uniased:\n" << a.stddev(0, false) << "\n";
    std::cout << "Biased:\n" << a.stddev(0, true) << "\n";
    std::cout << "Axis 1:\n";
    std::cout << "Uniased:\n" << a.stddev(1, false) << "\n";
    std::cout << "Biased:\n" << a.stddev(1, true) << "\n";
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
Uniased:
[[ 2.9439203,  3.5939764,  2.8722813, 0.95742711,  1.9148542,  1.8257419]]
Biased:
[[2.5495098, 3.1124749, 2.4874686, 0.8291562, 1.6583124, 1.5811388]]
Axis 1:
Uniased:
[[2.5033311],
 [2.6645825],
 [2.5625508],
 [2.9268869]]
Biased:
[[2.2852182],
 [2.4324199],
 [2.3392781],
 [2.6718699]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << "Unbiased:\n" << a.stddev(np::make_shape(1, 2), false) << "\n";
    std::cout << "Biased:\n" << a.stddev(np::make_shape(1, 2), true) << "\n";
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
Unbiased:
[[[2.4293034]],

 [[2.1514618]]]
Biased:
[[[ 2.325881]],

 [[2.0598678]]]
```

### `tensor::sum`

<h3><code>tensor_view::sum</code></h3>

<h3><code>indirect_tensor::sum</code></h3>

Return the sum of the tensor elements.
```cpp
T sum() const;
```

Parameters

* None

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
tensor<T, Rank> sum(size_t axis) const;
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
    std::cout << "Last 2 axes:\n" << a.sum(np::make_shape(1, 2)) << "\n";
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
T var(bool bias) const;
```

The variance is defined as the average of the squared deviations from the mean
```
    var(a) = mean(x),    x = abs(a - mean(a))**2
```
The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias`
is `false`, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1`
provides an unbiased estimator of the sample variance; while `n` provides the
maximum likelihood estimator of the variance for normally distributed
variables.

Parameters

* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

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
    std::cout << "Unbiased: " << a.var(false) << "\n";
    std::cout << "Biased: " << a.var(true) << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
Unbiased: 18.2667
Biased: 16.44
```

### `tensor::var(axes)`

<h3><code>tensor_view::var(axes)</code></h3>

<h3><code>indirect_tensor::var(axes)</code></h3>

Return the variance of the tensor elements over the given axes.
```cpp
tensor<T, Rank> var(size_t axis, bool bias) const;
template <size_t N>
tensor<T, Rank> var(const shape_t<N> &axes, bool bias) const;
```

Parameters

* `axes` A `shape_t` object with the axes along which the variance is computed.
* `bias` If `bias` is `true`, then normalization is by `n`. Otherwise,
normalization is by `n - 1`.

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
    std::cout << "Axis 0:\n";
    std::cout << "Uniased:\n" << a.var(0, false) << "\n";
    std::cout << "Biased:\n" << a.var(0, true) << "\n";
    std::cout << "Axis 1:\n";
    std::cout << "Uniased:\n" << a.var(1, false) << "\n";
    std::cout << "Biased:\n" << a.var(1, true) << "\n";
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
Uniased:
[[ 8.6666667,  12.916667,       8.25, 0.91666667,  3.6666667,  3.3333333]]
Biased:
[[   6.5, 9.6875, 6.1875, 0.6875,   2.75,    2.5]]
Axis 1:
Uniased:
[[6.2666667],
 [      7.1],
 [6.5666667],
 [8.5666667]]
Biased:
[[5.2222222],
 [5.9166667],
 [5.4722222],
 [7.1388889]]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::tensor<double, 3> a;
    std::cin >> a;
    std::cout << "Last 2 axes:\n";
    std::cout << "Unbiased:\n" << a.var(np::make_shape(1, 2), false) << "\n";
    std::cout << "Biased:\n" << a.var(np::make_shape(1, 2), true) << "\n";
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
Unbiased:
[[[5.9015152]],

 [[4.6287879]]]
Biased:
[[[5.4097222]],

 [[4.2430556]]]
```
