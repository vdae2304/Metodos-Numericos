# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Public methods](#public-methods)
    - [`tensor::all`](#tensorall)
    - [`tensor::any`](#tensorany)
    - [`tensor::apply`](#tensorapply)
    - [`tensor::argmax`](#tensorargmax)
    - [`tensor::argmin`](#tensorargmin)
    - [`tensor::astype`](#tensorastype)
    - [`tensor::clamp`](#tensorclamp)
    - [`tensor::diagonal`](#tensordiagonal)
    - [`tensor::flatten`](#tensorflatten)
    - [`tensor::max`](#tensormax)
    - [`tensor::mean`](#tensormean)
    - [`tensor::min`](#tensormin)
    - [`tensor::partition`](#tensorpartition)
    - [`tensor::prod`](#tensorprod)
    - [`tensor::reshape`](#tensorreshape)
    - [`tensor::resize`](#tensorresize)
    - [`tensor::reverse`](#tensorreverse)
    - [`tensor::rotate`](#tensorrotate)
    - [`tensor::sort`](#tensorsort)
    - [`tensor::stddev`](#tensorstddev)
    - [`tensor::sum`](#tensorsum)
    - [`tensor::t`](#tensort)
    - [`tensor::var`](#tensorvar)
    - [`tensor::view`](#tensorview)

## Public methods

### `tensor::all`

Test whether all tensor elements evaluate to true.
```cpp
bool all() const;
```

Parameters

* None

Returns

* true if all the elements evaluate to true and false otherwise.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    np::vector<bool> is_positive = (a > 0);
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

### `tensor::any`

Test whether any tensor element evaluate to true.
```cpp
bool any() const;
```

Parameters

* None

Returns

* true if any element evaluate to true and false otherwise.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    np::vector<bool> is_negative = (a < 0);
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

### `tensor::apply`

Assigns to each element the result of applying a function to the corresponding elements in `*this`.
```cpp
void apply(T f(T));
void apply(T f(const T &));
template <class Function>
void apply(Function &&f);
```

Parameters

* `f` A function that accepts one element of type `T` as argument, and returns a value convertible to `T`.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;

int square(int x) {
    return x*x;
}

int main() {
    np::vector<int> a;
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

Return the index of the maximum value in the tensor.
```cpp
index_t<Rank> argmax() const;
```

Parameters

* None

Returns

* The index of the maximum value in the tensor.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << a.argmax() << "\n";
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
(2, 5)
```

### `tensor::argmin`

Return the index of the minimum value in the tensor.
```cpp
index_t<Rank> argmin() const;
```

Parameters

* None

Returns

* The index of the minimum value in the tensor.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << a.argmin() << "\n";
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
(1, 4)
```

### `tensor::astype`

Cast each element to a specified type.
```cpp
template <class U>
tensor<U, Rank> astype() const;
```

Template parameters

* `U` Data type to which the elements are casted.

Parameters

* None

Returns

* A light-weight object with the elements in the tensor casted to the specified type. This function does not create a new tensor, instead, it returns a readonly view of the tensor with its elements casted to the specified type.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> a;
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

Clamp the values in the tensor. Given an interval `[a_min, a_max]`, values smaller than `a_min` become `a_min`, and values larger than `a_max` become `a_max`. For complex types, the real and imaginary parts are clamped separately.
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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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

### `tensor::diagonal`

Return a view of the diagonal. If the tensor has more than two dimensions, then the last two axes are used to determine the 2-dimensional sub-tensor whose diagonal is returned.
```cpp
tensor_view<T, Rank - 1> diagonal(difference_type k = 0);
tensor_view<const T, Rank - 1> diagonal(difference_type k = 0) const;
```

Parameters

* `k` Offset of the diagonal from the main diagonal. A positive value refers to an upper diagonal and a negative value refers to a lower diagonal. Defaults to 0 (main diagonal).

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to `const T`, which is convertible to a tensor object. Otherwise, the function returns a `tensor_view` to `T`, which has reference semantics to the original tensor.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << a.diagonal() << "\n";
    std::cout << "Sub diagonal:\n" << a.diagonal(-1) << "\n";
    std::cout << "Sup diagonal:\n" << a.diagonal(1) << "\n";
    return 0;
}
```

Input

```
[[ 3, 19,  6,  9, 14],
 [17,  7, 11, -2, -4],
 [ 5,  5, 18,  2,  2],
 [17, -2, -1, 11,  1],
 [12,  6,  2, 11, -3]]
```

Output

```
Main diagonal:
[ 3,  7, 18, 11, -3]
Sub diagonal:
[17,  5, -1, 11]
Sup diagonal:
[19, 11,  2,  1]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << "Main diagonal:\n" << a.diagonal() << "\n";
    std::cout << "Sub diagonal:\n" << a.diagonal(-1) << "\n";
    std::cout << "Sup diagonal:\n" << a.diagonal(1) << "\n";
    return 0;
}
```

Input

```
[[[ 2,  5, -4, 15],
  [-1, -1, 15,  2],
  [ 6,  5, 18, -3],
  [ 6,  5, 18, 14]],

 [[18, 19, 10, -3],
  [ 1,  0, 15, 18],
  [10, 18,  4,  7],
  [10, 10,  9, -1]]]
```

Output

```
Main diagonal:
[[ 2, -1, 18, 14],
 [18,  0,  4, -1]]
Sub diagonal:
[[-1,  5, 18],
 [ 1, 18,  9]]
Sup diagonal:
[[ 5, 15, -3],
 [19, 15,  7]]
```

### `tensor::flatten`

Return a view of the tensor collapsed into one dimension.
```cpp
tensor_view<T, 1> flatten();
tensor_view<const T, 1> flatten() const;
```

Parameters

* None

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to `const T`, which is convertible to a tensor object. Otherwise, the function returns a `tensor_view` to `T`, which has reference semantics to the original tensor.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << a.flatten() << "\n";
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
[ 1, 14, 12, -3, -5, -3, 11, 11, -1, 18, -3, -1]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    std::cout << a.flatten() << "\n";
    return 0;
}
```

Input

```
[[[16, 15, 14, -1],
  [ 5, 14,  9, 10],
  [18, 15,  2, 5]],

 [[11,  6, 19, -2],
  [ 7, 10,  1, -2],
  [14,  7, -2, 11]]]
```

Output

```
[16, 15, 14, -1,  5, 14,  9, 10, 18, 15,  2,  5, 11,  6, 19, -2,  7, 10,  1, -2, 
 14,  7, -2, 11]
```

### `tensor::max`

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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << a.max() << "\n";
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
14
```

### `tensor::mean`

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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> a;
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

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << a.mean() << "\n";
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
5.375
```

### `tensor::min`

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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << a.min() << "\n";
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
-5
```

### `tensor::partition`

Partition the elements in-place.
```cpp
void partition(size_t kth, size_t axis = Rank - 1);
template <class Compare>
void partition(size_t kth, size_t axis, Compare comp);
```

Parameters

* `kth` Element index to partition by. The element at the `kth` position is the element that would be in that position in the sorted tensor. The other elements are left without any specific order, except that none of the elements preceding `kth` are greater than it, and none of the elements following it are less.
* `axis` Axis along which to partition. Defaults to `Rank - 1`, which means partition along the last axis.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int kth, axis;
    std::cin >> a >> kth >> axis;
    a.partition(kth, axis);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
1
0
```

Output

```
[[ 3,  2, -2,  1, -5, -3],
 [ 6, 10,  0,  3,  4,  0],
 [ 8, 11,  2,  7,  4, 14],
 [13, 14,  1,  3,  7, 13]]
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
2
1
```

Output

```
[[ 7,  2,  7, 11, 13, 13],
 [ 0, -5,  0,  2,  3,  8],
 [-2,  3,  3, 10,  4, 14],
 [ 1, -3,  1,  4,  6, 14]]
```

### `tensor::prod`

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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << a.prod() << "\n";
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
1.02079e+16
```

### `tensor::reshape`

Return a `tensor_view` containing the same data with a new shape.
```cpp
template <class... Sizes>
tensor_view<T, sizeof...(Sizes)> reshape(Sizes... sizes);
template <class... Sizes>
tensor_view<const T, sizeof...(Sizes)> reshape(Sizes... sizes) const;

template <size_t N>
tensor_view<T, N> reshape(const shape_t<N> &shape);
template <size_t N>
tensor_view<const T, N> reshape(const shape_t<N> &shape) const;

template <size_t N>
tensor_view<T, N> reshape(const shape_t<N> &shape, layout_t order);
template <size_t N>
tensor_view<const T, N> reshape(const shape_t<N> &shape, layout_t order) const;
```

Parameters

* `shape` The new shape should be compatible with the original shape. It can be a `shape_t` object or the elements of the shape passed as separate arguments.
* `order` Memory layout in which elements are read. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. The default is to use the same layout as `*this`.

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to `const T`, which is convertible to a tensor object. Otherwise, the function returns a `tensor_view` to `T`, which has reference semantics to the original tensor.

Exceptions

* `std::invalid_argument` Thrown if the tensor could not reshaped.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
    np::shape_t<2> shape;
    std::cin >> a >> shape;
    std::cout << "As a row vector:\n" << a.reshape(1, a.size()) << "\n";
    std::cout << "As a column vector:\n" << a.reshape(a.size(), 1) << "\n";
    std::cout << "As a matrix of shape " << shape << ":\n"
              << a.reshape(shape) << "\n";
    std::cout << "As a matrix of shape " << shape << " (column-major):\n"
              << a.reshape(shape, np::column_major) << "\n";
    return 0;
}
```

Input

```
[4, 9, 5, 0, 10, 3]
(2, 3)
```

Output

```
As a row vector:
[[ 4,  9,  5,  0, 10,  3]]
As a column vector:
[[ 4],
 [ 9],
 [ 5],
 [ 0],
 [10],
 [ 3]]
As a matrix of shape (2, 3):
[[ 4,  9,  5],
 [ 0, 10,  3]]
As a matrix of shape (2, 3) (column-major):
[[ 4,  5, 10],
 [ 9,  0,  3]]
```

### `tensor::resize`

Resizes the tensor in-place to a given shape. If the new size is different from the number of elements stored in the tensor, a reallocation takes place to fit the new shape, losing the previous contents in the process. Otherwise, the contents of the tensor are preserved, but arranged to match the new shape.
```cpp
template <class... Sizes>
void resize(Sizes... sizes);

void resize(const shape_type &shape);
```

Parameters

* `shape` New shape of the tensor. It can be a `shape_t` object or the elements of the shape passed as separate arguments.

Returns

* None

Warnings

* Invalidates all iterators, references and views to elements of the tensor.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    np::shape_t<2> shape;
    std::cin >> a >> shape;
    if (a.size() != shape.prod()) {
        std::cout << "A reallocation is required!\n";
    }
    a.resize(shape);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
(6, 2)
```

Output

```
[[ 1, 14],
 [12, -3],
 [-5, -3],
 [11, 11],
 [-1, 18],
 [-3, -1]]
```

Input

```
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
(3, 3)
```

Possible output

```
A reallocation is required!
[[1603217806,          5,          0],
 [         0,         -5,         -3],
 [        11,         11,          0]]
```

### `tensor::reverse`

Reverse the order of the elements in-place.
```cpp
void reverse(size_t axis = Rank - 1);
```

Parameters

* `axis` Axis along which to reverse. Defaults to `Rank - 1`, which means reverse along the last axis.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int axis;
    std::cin >> a >> axis;
    a.reverse(axis);
    std::cout << a << "\n";
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

### `tensor::rotate`

Rotate the elements in-place. The elements are shifted circularly in such a way that the element at position `shift` becomes the first element and the element at position `shift - 1` becomes the last element.
```cpp
void rotate(size_t shift, size_t axis = Rank - 1);
```

Parameters

* `shift` Number of positions to shift the elements by.
* `axis` Axis along which to rotate. Defaults to `Rank - 1`, which means rotate along the last axis.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int shift, axis;
    std::cin >> a >> shift >> axis;
    a.rotate(shift, axis);
    std::cout << a << "\n";
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

### `tensor::sort`

Sort the elements in-place.
```cpp
void sort(size_t axis = Rank - 1);
template <class Compare>
void sort(size_t axis, Compare comp, bool stable = false);
```

Parameters

* `axis` Axis along which to sort. Defaults to `Rank - 1`, which means sort along the last axis.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.
* `stable` If true, preserve the relative order of the elements with equivalent values. Otherwise, equivalent elements are not guaranteed to keep their original relative order.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    int axis;
    std::cin >> a >> axis;
    a.sort(axis);
    std::cout << a << "\n";
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
[[ 3,  2, -2,  1, -5, -3],
 [ 6, 10,  0,  3,  4,  0],
 [ 8, 11,  1,  3,  4, 13],
 [13, 14,  2,  7,  7, 14]]
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
[[ 2,  7,  7, 11, 13, 13],
 [-5,  0,  0,  2,  3,  8],
 [-2,  3,  3,  4, 10, 14],
 [-3,  1,  1,  4,  6, 14]]
```

### `tensor::stddev`

Return the standard deviation of the tensor elements.
```cpp
T stddev(bool bias = true) const;
```

The standard deviation is defined as the square root of the average of the squared deviations from the mean

```
    stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
```

The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias` is false, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1` provides an unbiased estimator of the sample variance; while `n` provides the maximum likelihood estimator of the variance for normally distributed variables.

Parameters

* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Defaults to true.

Returns

* The standard deviation of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << a.stddev() << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
4.05463
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << a.stddev() << "\n";
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
2.54644
```

### `tensor::sum`

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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
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

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << a.sum() << "\n";
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
129
```

### `tensor::t`

Return a view of the tensor with its axes in reversed order.
```cpp
tensor_view<T, Rank> t();
tensor_view<const T, Rank> t() const;
```

Parameters

* None

Returns

* If the tensor is const-qualified, the function returns a `tensor_view` to `const T`, which is convertible to a tensor object. Otherwise, the function returns a `tensor_view` to `T`, which has reference semantics to the original tensor.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
    std::cin >> a;
    np::vector_view<int> view = a.t();
    std::cout << view.shape() << "\n";
    std::cout << view << "\n";
    return 0;
}
```

Input

```
[0, 14, -4, 5, 1, 1, -3, 1, 5, 0]
```

Output

```
(10,)
[ 0, 14, -4,  5,  1,  1, -3,  1,  5,  0]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::matrix_view<int> view = a.t();
    std::cout << view.shape() << "\n";
    std::cout << view << "\n";
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
(4, 3)
[[ 1, -5, -1],
 [14, -3, 18],
 [12, 11, -3],
 [-3, 11, -1]]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a;
    std::cin >> a;
    np::tensor_view<int, 3> view = a.t();
    std::cout << view.shape() << "\n";
    std::cout << view << "\n";
    return 0;
}
```

Input

```
[[[16, 15, 14, -1],
  [ 5, 14,  9, 10],
  [18, 15,  2, 5]],

 [[11,  6, 19, -2],
  [ 7, 10,  1, -2],
  [14,  7, -2, 11]]]
```

Output

```
(4, 3, 2)
[[[16, 11],
  [ 5,  7],
  [18, 14]],

 [[15,  6],
  [14, 10],
  [15,  7]],

 [[14, 19],
  [ 9,  1],
  [ 2, -2]],

 [[-1, -2],
  [10, -2],
  [ 5, 11]]]
```

### `tensor::var`

Return the variance of the tensor elements.
```cpp
T var(bool bias = true) const;
```

The variance is defined as the average of the squared deviations from the mean

```
    var(a) = mean(x),    x = abs(a - mean(a))**2
```

The mean is calculated as `sum(x)/n`, where `n = x.size()`. However, if `bias` is false, the divisor `n - 1` is used instead of `n`. In statistics, `n - 1` provides an unbiased estimator of the sample variance; while `n` provides the maximum likelihood estimator of the variance for normally distributed variables.

Parameters

* `bias` If `bias` is true, then normalization is by `n`. Otherwise, normalization is by `n - 1`. Defaults to true.

Returns

* The variance of the tensor elements.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << a.var() << "\n";
    return 0;
}
```

Input

```
[4, 3, 8, 1, 15, 3, 5, 1, 2, 2]
```

Output

```
16.44
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<double> a;
    std::cin >> a;
    std::cout << a.var() << "\n";
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
6.48438
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

* If the tensor is const-qualified, the function returns a `tensor_view` to `const T`, which is convertible to a tensor object. Otherwise, the function returns a `tensor_view` to `T`, which has reference semantics to the original tensor.
