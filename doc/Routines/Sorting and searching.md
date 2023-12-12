# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Sorting and searching](#sorting-and-searching)
    - [`argsort`](#argsort)
    - [`sort`](#sort)
    - [`argpartition`](#argpartition)
    - [`partition`](#partition)
    - [`nonzero`](#nonzero)
    - [`where`](#where)

## Sorting and searching

### `argsort`

Return the indices that would sort the tensor.
```cpp
template <class T, size_t Rank>
tensor<index_t<Rank>, 1> argsort(const tensor<T, Rank> &a);

template <class T, size_t Rank, class Compare>
tensor<index_t<Rank>, 1> argsort(const tensor<T, Rank> &a, Compare comp,
                                 bool stable = false);
```

Parameters

* `a` Tensor-like object to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.
* `stable` If true, preserve the relative order of the elements with equivalent values. Otherwise, equivalent elements are not guaranteed to keep their original relative order.

Returns

* A 1-dimensional tensor of indices that sort the tensor. If `indices` is the returned tensor of indices for `a`, then `a[indices]` yields a sorted `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

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
    np::vector<np::index_t<1>> indices = np::argsort(a);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << a[indices] << "\n";
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
Values:
[-5, -3, -2,  0,  3,  4, 12, 12, 18, 19]
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
    std::cin >> a;
    np::vector<np::index_t<2>> indices = np::argsort(a);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << a[indices] << "\n";
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
Indices:
[(1, 4), (3, 5), (2, 2), (1, 2), (1, 5), (3, 3), (3, 2), (0, 2), (1, 1), (2, 0), 
 (2, 3), (1, 3), (2, 4), (3, 4), (3, 0), (0, 4), (0, 3), (1, 0), (2, 1), (0, 1), 
 (0, 0), (0, 5), (2, 5), (3, 1)]
Values:
[-5, -3, -2,  0,  0,  1,  1,  2,  2,  3,  3,  3,  4,  4,  6,  7,  7,  8, 10, 11, 
 13, 13, 14, 14]
```

<h3><code>argsort</code></h3>

Return the indices that would sort the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank> argsort(const tensor<T, Rank> &a, size_t axis);

template <class T, size_t Rank, class Compare>
tensor<size_t, Rank> argsort(const tensor<T, Rank> &a, size_t axis,
                             Compare comp, bool stable = false);
```

Parameters

* `a` Tensor-like object to sort.
* `axis` Axis along which to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.
* `stable` If true, preserve the relative order of the elements with equivalent values. Otherwise, equivalent elements are not guaranteed to keep their original relative order.

Returns

* A tensor of indices of the same shape as `a` that sort the tensor along the given axis. If `indices` is the returned tensor of indices for `a`, then `take_along_axis(a, indices, axis)` yields a sorted `a` along the given axis.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/broadcasting.h> // np::take_along_axis
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t axis;
    std::cin >> a >> axis;
    // For axis=0, sort row elements for each column.
    // For axis=1, sort column elements for each row.
    np::matrix<size_t> indices = np::argsort(a, axis);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, axis) << "\n";
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
Indices:
[[2, 1, 2, 3, 1, 3],
 [3, 2, 1, 1, 2, 1],
 [1, 0, 3, 2, 3, 0],
 [0, 3, 0, 0, 0, 2]]
Values:
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
Indices:
[[2, 3, 4, 1, 0, 5],
 [4, 2, 5, 1, 3, 0],
 [2, 0, 3, 4, 1, 5],
 [5, 2, 3, 4, 0, 1]]
Values:
[[ 2,  7,  7, 11, 13, 13],
 [-5,  0,  0,  2,  3,  8],
 [-2,  3,  3,  4, 10, 14],
 [-3,  1,  1,  4,  6, 14]]
```

### `sort`

Return a sorted copy of the flattened tensor.
```cpp
template <class T, size_t Rank>
tensor<T, 1> sort(const tensor<T, Rank> &a);

template <class T, size_t Rank, class Compare>
tensor<T, 1> sort(const tensor<T, Rank> &a, Compare comp, bool stable = false);
```

Parameters

* `a` Tensor-like object to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.
* `stable` If true, preserve the relative order of the elements with equivalent values. Otherwise, equivalent elements are not guaranteed to keep their original relative order.

Returns

* A sorted copy of the flattened tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

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
    std::cout << np::sort(a) << "\n";
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
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << np::sort(a) << "\n";
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
[-5, -3, -2,  0,  0,  1,  1,  2,  2,  3,  3,  3,  4,  4,  6,  7,  7,  8, 10, 11, 
 13, 13, 14, 14]
```

<h3><code>sort</code></h3>

Return a sorted copy of the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> sort(const tensor<T, Rank> &a, size_t axis);

template <class T, size_t Rank, class Compare>
tensor<T, Rank> sort(const tensor<T, Rank> &a, size_t axis, Compare comp,
                     bool stable = false);
```

Parameters

* `a` Tensor-like object to sort.
* `axis` Axis along which to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.
* `stable` If true, preserve the relative order of the elements with equivalent values. Otherwise, equivalent elements are not guaranteed to keep their original relative order.

Returns

* A sorted copy of the tensor along the given axis.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t axis;
    std::cin >> a >> axis;
    // For axis=0, sort row elements for each column.
    // For axis=1, sort column elements for each row.
    std::cout << np::sort(a, axis) << "\n";
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

### `argpartition`

Return the indices that would partition the tensor.
```cpp
template <class T, size_t Rank>
tensor<index_t<Rank>, 1> argpartition(const tensor<T, Rank> &a, size_t kth);

template <class T, size_t Rank, class Compare>
tensor<index_t<Rank>, 1> argpartition(const tensor<T, Rank> &a, size_t kth,
                                      Compare comp);
```

Parameters

* `a` Tensor-like object to partition.
* `kth` Element index to partition by. The element at the `kth` position is the element that would be in that position in the sorted tensor. The other elements are left without any specific order, except that none of the elements preceding `kth` are greater than it, and none of the elements following it are less.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.

Returns

* A 1-dimensional tensor of indices that partition the tensor. If `indices` is the returned tensor of indices for `a`, then `a[indices]` yields a partitioned `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    size_t kth;
    std::cin >> a >> kth;
    np::vector<np::index_t<1>> indices = np::argpartition(a, kth);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << a[indices] << "\n";
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
Values:
[-2, -5, -3,  0,  3,  4, 12, 12, 18, 19]
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
    size_t kth;
    std::cin >> a >> kth;
    np::vector<np::index_t<2>> indices = np::argpartition(a, kth);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
10
```

Output

```
Indices:
[(3, 5), (1, 4), (3, 3), (1, 5), (1, 2), (2, 2), (1, 1), (0, 2), (3, 2), (2, 3), 
 (2, 0), (1, 3), (0, 0), (2, 1), (1, 0), (0, 5), (2, 4), (2, 5), (3, 0), (3, 1), 
 (0, 4), (0, 3), (3, 4), (0, 1)]
Values:
[-3, -5,  1,  0,  0, -2,  2,  2,  1,  3,  3,  3, 13, 10,  8, 13,  4, 14,  6, 14, 
  7,  7,  4, 11]
```

<h3><code>argpartition</code></h3>

Return the indices that would partition the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank> argpartition(const tensor<T, Rank> &a, size_t kth,
                                  size_t axis);

template <class T, size_t Rank, class Compare>
tensor<size_t, Rank> argpartition(const tensor<T, Rank> &a, size_t kth,
                                  size_t axis, Compare comp);
```

Parameters

* `a` Tensor-like object to partition.
* `kth` Element index to partition by. The element at the `kth` position is the element that would be in that position in the sorted tensor. The other elements are left without any specific order, except that none of the elements preceding `kth` are greater than it, and none of the elements following it are less.
* `axis` Axis along which to partition.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.

Returns

* A tensor of indices of the same shape as `a` that partition the tensor along the given axis. If `indices` is the returned tensor of indices for `a`, then `take_along_axis(a, indices, axis)` yields a partitioned `a` along the given axis.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/broadcasting.h> // np::take_along_axis
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t kth, axis;
    std::cin >> a >> kth >> axis;
    // For axis=0, partition row elements for each column.
    // For axis=1, partition column elements for each row.
    np::matrix<size_t> indices = np::argpartition(a, kth, axis);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Values:\n" << np::take_along_axis(a, indices, axis) << "\n";
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
Indices:
[[2, 1, 2, 3, 1, 3],
 [3, 2, 1, 2, 3, 1],
 [1, 0, 0, 0, 2, 2],
 [0, 3, 3, 1, 0, 0]]
Values:
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
Indices:
[[3, 2, 4, 1, 0, 5],
 [2, 4, 5, 1, 3, 0],
 [2, 0, 3, 1, 4, 5],
 [3, 5, 2, 4, 0, 1]]
Values:
[[ 7,  2,  7, 11, 13, 13],
 [ 0, -5,  0,  2,  3,  8],
 [-2,  3,  3, 10,  4, 14],
 [ 1, -3,  1,  4,  6, 14]]
```

### `partition`

Return a partitioned copy of the flattened tensor.
```cpp
template <class T, size_t Rank>
tensor<T, 1> partition(const tensor<T, Rank> &a, size_t kth);

template <class T, size_t Rank, class Compare>
tensor<T, 1> partition(const tensor<T, Rank> &a, size_t kth, Compare comp);
```

Parameters

* `a` Tensor-like object to partition.
* `kth` Element index to partition by. The element at the `kth` position is the element that would be in that position in the sorted tensor. The other elements are left without any specific order, except that none of the elements preceding `kth` are greater than it, and none of the elements following it are less.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.

Returns

* A partitioned copy of the flattened tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a;
    size_t kth;
    std::cin >> a >> kth;
    std::cout << np::partition(a, kth) << "\n";
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
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t kth;
    std::cin >> a >> kth;
    std::cout << np::partition(a, kth) << "\n";
    return 0;
}
```

Input

```
[[13, 11,  2,  7,  7, 13],
 [ 8,  2,  0,  3, -5,  0],
 [ 3, 10, -2,  3,  4, 14],
 [ 6, 14,  1,  1,  4, -3]]
10
```

Output

```
[-3, -5,  1,  0,  0, -2,  2,  2,  1,  3,  3,  3, 13, 10,  8, 13,  4, 14,  6, 14, 
  7,  7,  4, 11]
```

<h3><code>partition</code></h3>

Return a partitioned copy of the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> partition(const tensor<T, Rank> &a, size_t kth, size_t axis);

template <class T, size_t Rank, class Compare>
tensor<T, Rank> partition(const tensor<T, Rank> &a, size_t kth, size_t axis,
                          Compare comp);
```

Parameters

* `a` Tensor-like object to partition.
* `kth` Element index to partition by. The element at the `kth` position is the element that would be in that position in the sorted tensor. The other elements are left without any specific order, except that none of the elements preceding `kth` are greater than it, and none of the elements following it are less.
* `axis` Axis along which to partition.
* `comp` Custom comparator. A binary function that accepts two elements of type `T` as arguments, and returns a value convertible to `bool`. The value returned indicates whether the element passed as first argument is considered to go before the second.

Returns

* A partitioned copy of the tensor along the given axis.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    size_t kth, axis;
    std::cin >> a >> kth >> axis;
    // For axis=0, partition row elements for each column.
    // For axis=1, partition column elements for each row.
    std::cout << np::partition(a, kth, axis) << "\n";
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

### `nonzero`

Return the indices of the elements that are non-zero.
```cpp
template <class T, size_t Rank>
tensor<index_t<Rank>, 1> nonzero(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* A 1-dimensional tensor with the indices of the elements that are non-zero.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

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
    np::vector<np::index_t<1>> indices = np::nonzero(a > 0);
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
[(0,), (2,), (3,), (7,), (9,)]
Values:
[ 7,  5, 11,  8, 14]
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
    std::cin >> a;
    np::vector<np::index_t<2>> indices = np::nonzero(a > 0);
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

### `where`

Return elements chosen from two tensors depending on `condition`. When only `condition` is provided, this function is equivalent to `nonzero`.
```cpp
template <size_t Rank>
tensor<index_t<Rank>, 1> where(const tensor<bool, Rank> &condition);

template <class T, size_t Rank>
tensor<T, Rank> where(const tensor<bool, Rank> &condition,
                      const tensor<T, Rank> &x, const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> where(const tensor<bool, Rank> &condition,
                      const tensor<T, Rank> &x,
                      const typename tensor<T, Rank>::value_type &y);

template <class T, size_t Rank>
tensor<T, Rank> where(const tensor<bool, Rank> &condition,
                      const typename tensor<T, Rank>::value_type &x,
                      const tensor<T, Rank> &y);

template <class T, size_t Rank>
tensor<T, Rank> where(const tensor<bool, Rank> &condition,
                      const T &x, const T &y);
```

Parameters

* `condition` A tensor-like object of `bool`. Where true, yield `x`, otherwise yield `y`.
* `x` A single value or a tensor-like object with the values from which to choose where `condition` is true.
* `y` A single value or a tensor-like object with the values from which to choose where `condition` is false.

Returns

* A light-weight object with the elements chosen from `x` where condition is true and from `y` where `condition` is false. This function does not create a new tensor, instead, an expression object is returned.

Exceptions

* `std::invalid_argument` Thrown if `condition`, `x` and `y` shapes are not compatible and cannot be broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> x, y;
    std::cin >> x >> y;
    std::cout << "abs(x):\n" << np::where(x > 0, x, -x) << "\n";
    std::cout << "max(x, y):\n" << np::where(x > y, x, y) << "\n";
    std::cout << "max(x - y, 0):\n" << np::where(x > y, x - y, 0) << "\n";
    return 0;
}
```

Input

```
[-1, -5, 12,  9,  4, 12, 15, 17, 11, -3]
[-5, -1,  6, 16, 16,  8,  9, 12, 19, 11]
```

Output

```
abs(x):
[ 1,  5, 12,  9,  4, 12, 15, 17, 11,  3]
max(x, y):
[-1, -1, 12, 16, 16, 12, 15, 17, 19, 11]
max(x - y, 0):
[4, 0, 6, 0, 0, 4, 6, 5, 0, 0]
```
