# Sorting and searching

Defined in `numcpp/routines.h`

- [Sorting and searching](#sorting-and-searching)
  - [Sorting](#sorting)
    - [`argsort`](#argsort)
    - [`argsort(axis)`](#argsortaxis)
    - [`sort`](#sort)
    - [`sort(axis)`](#sortaxis)
    - [`argpartition`](#argpartition)
    - [`argpartition(axis)`](#argpartitionaxis)
    - [`partition`](#partition)
    - [`partition(axis)`](#partitionaxis)
  - [Searching](#searching)
    - [`nonzero`](#nonzero)
    - [`where`](#where)
  - [Rearranging elements](#rearranging-elements)
    - [`reverse(axes)`](#reverseaxes)
    - [`rotate(axes)`](#rotateaxes)

## Sorting

### `argsort`

Return the indices that would sort the tensor.
```cpp
template <class T, size_t Rank>
tensor<index_t<Rank>, 1> argsort(const tensor<T, Rank> &a);

template <class T, size_t Rank, class Compare>
tensor<index_t<Rank>, 1> argsort(
    const tensor<T, Rank> &a, Compare comp, bool stable = false
);
```

Parameters

* `a` Tensor-like object to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.
* `stable` If `true`, preserve the relative order of the elements with
equivalent values. Otherwise, equivalent elements are not guaranteed to keep
their original relative order.

Returns

* A one-dimensional tensor of indices that sort the tensor. If `a` is a tensor,
then `a[argsort(a)]` yields a sorted `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::array<int> a;
    std::cin >> a;

    np::array<np::index_t<1>> indices = np::argsort(a);
    std::cout << "Default sort:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted:\n" << a[indices] << "\n";

    indices = np::argsort(a, compareByAbs);
    std::cout << "\nSort by absolute value:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
Default sort:
Indices:
[(7,), (6,), (1,), (3,), (8,), (4,), (0,), (9,), (5,), (2,)]
Sorted:
[-5, -3, -2,  0,  3,  4, 12, 12, 18, 19]

Sort by absolute value:
Indices:
[(3,), (1,), (6,), (8,), (4,), (7,), (0,), (9,), (5,), (2,)]
Sorted:
[ 0, -2, -3,  3,  4, -5, 12, 12, 18, 19]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::matrix<int> a;
    std::cin >> a;

    std::cout << "Default sort:\n";
    np::array<np::index_t<2>> indices = np::argsort(a);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted:\n" << a[indices] << "\n";

    std::cout << "\nSort by absolute value:\n";
    indices = np::argsort(a, compareByAbs);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted:\n" << a[indices] << "\n";
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
Default sort:
Indices:
[(1, 5), (1, 1), (0, 2), (3, 1), (2, 5), (0, 4), (3, 5), (3, 0), (2, 2), (1, 4),
 (1, 3), (3, 3), (2, 4), (2, 1), (2, 3), (2, 0), (1, 0), (0, 0), (3, 2), (0, 3),
 (1, 2), (0, 5), (3, 4), (0, 1)]
Sorted:
[-5, -4, -3, -3, -3, -2, -1, -1, -1,  0,  1,  2,  3,  3,  4,  9, 10, 11, 12, 13,
 15, 15, 18, 18]

Sort by absolute value:
Indices:
[(1, 4), (3, 5), (3, 0), (1, 3), (2, 2), (3, 3), (0, 4), (0, 2), (3, 1), (2, 5),
 (2, 4), (2, 1), (1, 1), (2, 3), (1, 5), (2, 0), (1, 0), (0, 0), (3, 2), (0, 3),
 (1, 2), (0, 5), (3, 4), (0, 1)]
Sorted:
[ 0, -1, -1,  1, -1,  2, -2, -3, -3, -3,  3,  3, -4,  4, -5,  9, 10, 11, 12, 13,
 15, 15, 18, 18]
```

### `argsort(axis)`

Return the indices that would sort the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank> argsort(const tensor<T, Rank> &a, size_t axis);

template <class T, size_t Rank, class Compare>
tensor<size_t, Rank> argsort(
    const tensor<T, Rank> &a, size_t axis, Compare comp, bool stable = false
);
```

Parameters

* `a` Tensor-like object to sort.
* `axis` Axis along which to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.
* `stable` If `true`, preserve the relative order of the elements with
equivalent values. Otherwise, equivalent elements are not guaranteed to keep
their original relative order.

Returns

* A tensor of indices of the same shape as `a` that sort the tensor along the
given axis. If `a` is a tensor, then
`take_along_axis(a, argsort(a, axis), axis)` yields a sorted `a`.

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

    std::cout << "Axis 0:\n";
    np::matrix<size_t> indices = np::argsort(a, 0);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted:\n" << np::take_along_axis(a, indices, 0) << "\n";

    std::cout << "\nAxis 1:\n";
    indices = np::argsort(a, 1);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Sorted:\n" << np::take_along_axis(a, indices, 1) << "\n";
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
Indices:
[[3, 1, 0, 1, 0, 1],
 [2, 3, 2, 3, 1, 2],
 [1, 2, 3, 2, 2, 3],
 [0, 0, 1, 0, 3, 0]]
Sorted:
[[-1, -4, -3,  1, -2, -5],
 [ 9, -3, -1,  2,  0, -3],
 [10,  3, 12,  4,  3, -1],
 [11, 18, 15, 13, 18, 15]]

Axis 1:
Indices:
[[2, 4, 0, 3, 5, 1],
 [5, 1, 4, 3, 0, 2],
 [5, 2, 1, 4, 3, 0],
 [1, 0, 5, 3, 2, 4]]
Sorted:
[[-3, -2, 11, 13, 15, 18],
 [-5, -4,  0,  1, 10, 15],
 [-3, -1,  3,  3,  4,  9],
 [-3, -1, -1,  2, 12, 18]]
```

### `sort`

Return a sorted copy of the flattened tensor.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, 1> sort(const tensor<T, Rank> &a);

template <class T, size_t Rank, class Compare>
tensor<typename tensor<T, Rank>::value_type, 1> sort(
    const tensor<T, Rank> &a, Compare comp, bool stable = false
);
```

Parameters

* `a` Tensor-like object to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.
* `stable` If `true`, preserve the relative order of the elements with
equivalent values. Otherwise, equivalent elements are not guaranteed to keep
their original relative order.

Returns

* A sorted copy of the flattened tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::array<int> a;
    std::cin >> a;
    std::cout << "Default sort:\n";
    std::cout << np::sort(a) << "\n";
    std::cout << "Sort by absolute value:\n";
    std::cout << np::sort(a, compareByAbs) << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
Default sort:
[-5, -3, -2,  0,  3,  4, 12, 12, 18, 19]
Sort by absolute value:
[ 0, -2, -3,  3,  4, -5, 12, 12, 18, 19]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Default sort:\n";
    std::cout << np::sort(a) << "\n";
    std::cout << "Sort by absolute value:\n";
    std::cout << np::sort(a, compareByAbs) << "\n";
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
Default sort:
[-5, -4, -3, -3, -3, -2, -1, -1, -1,  0,  1,  2,  3,  3,  4,  9, 10, 11, 12, 13,
 15, 15, 18, 18]
Sort by absolute value:
[ 0, -1, -1,  1, -1,  2, -2, -3, -3, -3,  3,  3, -4,  4, -5,  9, 10, 11, 12, 13,
 15, 15, 18, 18]
```

### `sort(axis)`

Return a sorted copy of the tensor.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> sort(
    const tensor<T, Rank> &a, size_t axis
);

template <class T, size_t Rank, class Compare>
tensor<typename tensor<T, Rank>::value_type, Rank> sort(
    const tensor<T, Rank> &a, size_t axis, Compare comp, bool stable = false
);
```

Parameters

* `a` Tensor-like object to sort.
* `axis` Axis along which to sort.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.
* `stable` If `true`, preserve the relative order of the elements with
equivalent values. Otherwise, equivalent elements are not guaranteed to keep
their original relative order.

Returns

* A sorted copy of the tensor.

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
    std::cout << "Axis 0:\n" << np::sort(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::sort(a, 1) << "\n";
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
[[-3, -2, 11, 13, 15, 18],
 [-5, -4,  0,  1, 10, 15],
 [-3, -1,  3,  3,  4,  9],
 [-3, -1, -1,  2, 12, 18]]
```

### `argpartition`

Return the indices that would partition the tensor.
```cpp
template <class T, size_t Rank>
tensor<index_t<Rank>, 1> argpartition(const tensor<T, Rank> &a, size_t kth);

template <class T, size_t Rank, class Compare>
tensor<index_t<Rank>, 1> argpartition(
    const tensor<T, Rank> &a, size_t kth, Compare comp
);
```

Parameters

* `a` Tensor-like object to partition.
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
tensor, then `a[argpartition(a, kth)]` yields a partitioned `a`.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::array<int> a;
    int kth;
    std::cin >> a >> kth;

    np::array<np::index_t<1>> indices = np::argpartition(a, kth);
    std::cout << "Default partition:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned:\n" << a[indices] << "\n";

    indices = np::argpartition(a, kth, compareByAbs);
    std::cout << "\nPartition by absolute value:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned:\n" << a[indices] << "\n";
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
Default partition:
Indices:
[(1,), (7,), (6,), (3,), (8,), (4,), (0,), (9,), (5,), (2,)]
Partitioned:
[-2, -5, -3,  0,  3,  4, 12, 12, 18, 19]

Partition by absolute value:
Indices:
[(4,), (1,), (6,), (3,), (8,), (7,), (9,), (0,), (5,), (2,)]
Partitioned:
[ 4, -2, -3,  0,  3, -5, 12, 12, 18, 19]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::matrix<int> a;
    int kth;
    std::cin >> a >> kth;

    np::array<np::index_t<2>> indices = np::argpartition(a, kth);
    std::cout << "Default partition:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned:\n" << a[indices] << "\n";

    indices = np::argpartition(a, kth, compareByAbs);
    std::cout << "\nPartition by absolute value:\n";
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned:\n" << a[indices] << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
10
```

Output

```
Default partition:
Indices:
[(3, 5), (2, 2), (0, 2), (1, 5), (0, 4), (3, 1), (2, 5), (1, 1), (1, 4), (3, 0),
 (1, 3), (3, 3), (2, 4), (2, 1), (2, 0), (2, 3), (0, 0), (1, 2), (1, 0), (0, 5),
 (3, 2), (0, 3), (3, 4), (0, 1)]
Partitioned:
[-1, -1, -3, -5, -2, -3, -3, -4,  0, -1,  1,  2,  3,  3,  9,  4, 11, 15, 10, 15,
 12, 13, 18, 18]

Partition by absolute value:
Indices:
[(2, 5), (3, 5), (2, 2), (3, 3), (0, 4), (2, 1), (3, 0), (2, 4), (1, 4), (1, 3),
 (0, 2), (3, 1), (2, 3), (1, 1), (1, 5), (2, 0), (0, 0), (1, 2), (1, 0), (0, 5),
 (3, 2), (0, 3), (3, 4), (0, 1)]
Partitioned:
[-3, -1, -1,  2, -2,  3, -1,  3,  0,  1, -3, -3,  4, -4, -5,  9, 11, 15, 10, 15,
 12, 13, 18, 18]
```

### `argpartition(axis)`

Return the indices that would partition the tensor along the given axis.
```cpp
template <class T, size_t Rank>
tensor<size_t, Rank> argpartition(
    const tensor<T, Rank> &a, size_t kth, size_t axis
);

template <class T, size_t Rank, class Compare>
tensor<size_t, Rank> argpartition(
    const tensor<T, Rank> &a, size_t kth, size_t axis, Compare comp
);
```

Parameters

* `a` Tensor-like object to partition.
* `kth` Element index to partition by. The element at the `kth` position is the
element that would be in that position in the sorted tensor. The other elements
are left without any specific order, except that none of the elements preceding
`kth` are greater than it, and none of the elements following it are less.
* `axis` Axis along which to partition.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.

Returns

* A tensor of indices of the same shape as `a` that partitions the tensor along
the given axis. If `a` is a tensor, then
`take_along_axis(a, argpartition(a, kth, axis), axis)` yields a partitioned `a`.

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
    int kth;
    std::cin >> a;

    std::cin >> kth;
    std::cout << "Axis 0:\n";
    np::matrix<size_t> indices = np::argpartition(a, kth, 0);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned:\n" << np::take_along_axis(a, indices, 0) << "\n";

    std::cin >> kth;
    std::cout << "\nAxis 1:\n";
    indices = np::argpartition(a, kth, 1);
    std::cout << "Indices:\n" << indices << "\n";
    std::cout << "Partitioned:\n" << np::take_along_axis(a, indices, 1) << "\n";
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
Indices:
[[3, 1, 0, 1, 0, 1],
 [2, 3, 2, 3, 1, 2],
 [0, 2, 3, 2, 2, 0],
 [1, 0, 1, 0, 3, 3]]
Partitioned:
[[-1, -4, -3,  1, -2, -5],
 [ 9, -3, -1,  2,  0, -3],
 [11,  3, 12,  4,  3, 15],
 [10, 18, 15, 13, 18, -1]]

Axis 1:
Indices:
[[4, 2, 0, 3, 5, 1],
 [1, 5, 4, 3, 2, 0],
 [2, 5, 1, 4, 3, 0],
 [1, 5, 0, 3, 4, 2]]
Partitioned:
[[-2, -3, 11, 13, 15, 18],
 [-4, -5,  0,  1, 15, 10],
 [-1, -3,  3,  3,  4,  9],
 [-3, -1, -1,  2, 18, 12]]
```

### `partition`

Return a partitioned copy of the flattened tensor.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, 1> partition(
    const tensor<T, Rank> &a, size_t kth
);

template <class T, size_t Rank, class Compare>
tensor<typename tensor<T, Rank>::value_type, 1> partition(
    const tensor<T, Rank> &a, size_t kth, Compare comp
);
```

Parameters

* `a` Tensor-like object to partition.
* `kth` Element index to partition by. The element at the `kth` position is the
element that would be in that position in the sorted tensor. The other elements
are left without any specific order, except that none of the elements preceding
`kth` are greater than it, and none of the elements following it are less.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.

Returns

* A partitioned copy of the flattened tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::array<int> a;
    int kth;
    std::cin >> a >> kth;
    std::cout << "Default partition:\n";
    std::cout << np::partition(a, kth) << "\n";
    std::cout << "Partition by absolute value:\n";
    std::cout << np::partition(a, kth, compareByAbs) << "\n";
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
Default partition:
[-2, -5, -3,  0,  3,  4, 12, 12, 18, 19]
Partition by absolute value:
[ 4, -2, -3,  0,  3, -5, 12, 12, 18, 19]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

bool compareByAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

int main() {
    np::matrix<int> a;
    int kth;
    std::cin >> a >> kth;
    std::cout << "Default partition:\n";
    std::cout << np::partition(a, kth) << "\n";
    std::cout << "Partition by absolute value:\n";
    std::cout << np::partition(a, kth, compareByAbs) << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
10
```

Output

```
Default partition:
[-1, -1, -3, -5, -2, -3, -3, -4,  0, -1,  1,  2,  3,  3,  9,  4, 11, 15, 10, 15,
 12, 13, 18, 18]
Partition by absolute value:
[-3, -1, -1,  2, -2,  3, -1,  3,  0,  1, -3, -3,  4, -4, -5,  9, 11, 15, 10, 15,
 12, 13, 18, 18]
```

### `partition(axis)`

Return a partitioned copy of the tensor.
```cpp
template <class T, size_t Rank>
tensor<typename tensor<T, Rank>::value_type, Rank> partition(
    const tensor<T, Rank> &a, size_t kth, size_t axis
);

template <class T, size_t Rank, class Compare>
tensor<typename tensor<T, Rank>::value_type, Rank> partition(
    const tensor<T, Rank> &a, size_t kth, size_t axis,
    Compare comp
);
```

Parameters

* `a` Tensor-like object to partition.
* `kth` Element index to partition by. The element at the `kth` position is the
element that would be in that position in the sorted tensor. The other elements
are left without any specific order, except that none of the elements preceding
`kth` are greater than it, and none of the elements following it are less.
* `axis` Axis along which to partition.
* `comp` Custom comparator. A binary function that accepts two elements of type
`T` as arguments, and returns a value convertible to `bool`. The value returned
indicates whether the element passed as first argument is considered to go
before the second.

Returns

* A partitioned copy of the tensor.

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
    int kth;
    std::cin >> a;
    std::cin >> kth;
    std::cout << "Axis 0:\n" << np::partition(a, kth, 0) << "\n";
    std::cin >> kth;
    std::cout << "\nAxis 1:\n" << np::partition(a, kth, 1) << "\n";
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
[[-2, -3, 11, 13, 15, 18],
 [-4, -5,  0,  1, 15, 10],
 [-1, -3,  3,  3,  4,  9],
 [-3, -1, -1,  2, 18, 12]]
```

## Searching

### `nonzero`

Return the indices of the elements that are non-zero.
```cpp
template <class T, size_t Rank>
tensor<index_t<Rank>, 1> nonzero(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

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
    np::array<np::index_t<1>> indices = np::nonzero(a > 0);
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    np::array<np::index_t<2>> indices = np::nonzero(a > 0);
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

Return elements chosen from two tensors depending on `condition`. When only
`condition` is provided, this function is equivalent to `nonzero`.
```cpp
template <size_t Rank>
tensor<index_t<Rank>, 1> where(const tensor<bool, Rank> &condition);

template <class T, size_t Rank>
tensor<T, Rank> where(
    const tensor<bool, Rank> &condition,
    const tensor<T, Rank> &x,
    const tensor<T, Rank> &y
);

template <class T, size_t Rank>
tensor<T, Rank> where(
    const tensor<bool, Rank> &condition,
    const tensor<T, Rank> &x,
    const typename tensor<T, Rank>::value_type &y
);

template <class T, size_t Rank>
tensor<T, Rank> where(
    const tensor<bool, Rank> &condition,
    const typename tensor<T, Rank>::value_type &x,
    const tensor<T, Rank> &y
);
```

Parameters

* `condition` A tensor-like object of `bool`. Where `true`, yield `x`, otherwise
yield `y`.
* `x` A single value or a tensor-like object with the values from which to
choose where `condition` is `true`.
* `y` A single value or a tensor-like object with the values from which to
choose where `condition` is `false`.

Returns

* A light-weight object with the elements chosen from `x` where `condition` is
`true` and from `y` where `condition` is `false`. This function does not create
a new tensor, instead, an expression object is returned.

Exceptions

* `std::invalid_argument` Thrown if `condition`, `x` and `y` shapes are not
compatible and cannot be broadcasted according to broadcasting rules.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> x, y;
    std::cin >> x >> y;
    // Just like ternary operator ?, where(cond, x, y) uses short-circuit
    // evaluation, i.e., exactly one of x or y is evaluated at each position.
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

## Rearranging elements

### `reverse(axes)`

Reverse the order of the elements in a tensor along the given axes.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> reverse(const tensor<T, Rank> &a, size_t axis = Rank - 1);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> reverse(const tensor<T, Rank> &a, const shape_t<N> &axes);
```

Parameters

* `a` A tensor-like object to reverse.
* `axes` Axis or axes along which to reverse over. The default is `Rank - 1`,
which reverses along the last axis.

Returns

* A light-weight object with the elements in the tensor in reversed order. This
function does not create a new tensor, instead, it returns a readonly view of
the tensor with its elements in reversed order over the given axes.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << np::reverse(a, 0) << "\n";
    std::cout << "Axis 1:\n" << np::reverse(a, 1) << "\n";
    std::cout << "Both axes:\n" << np::reverse(a, np::make_shape(0, 1)) << "\n";
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
[[15, -2, 13, -3, 18, 11],
 [-5,  0,  1, 15, -4, 10],
 [-3,  3,  4, -1,  3,  9],
 [-1, 18,  2, 12, -3, -1]]
Both axes:
[[-1, 18,  2, 12, -3, -1],
 [-3,  3,  4, -1,  3,  9],
 [-5,  0,  1, 15, -4, 10],
 [15, -2, 13, -3, 18, 11]]
```

### `rotate(axes)`

Rotate the elements in a tensor along the given axes. The elements are shifted
circularly in such a way that the element at position `shift` becomes the first
element and the element at position `shift - 1` becomes the last element.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> rotate(
    const tensor<T, Rank> &a, size_t shift, size_t axis = Rank - 1
);

template <class T, size_t Rank, size_t N>
tensor<T, Rank> rotate(
    const tensor<T, Rank> &a, const index_t<N> &shift, const shape_t<N> &axes
);
```

Parameters

* `a` A tensor-like object to rotate.
* `shift` Number of positions to shift the elements by along each axis.
* `axes` Axis or axes along which the elements are rotated. The default is
`Rank - 1`, which rotates along the last axis.

Returns

* A light-weight object with the elements in the tensor shifted circularly.
This function does not create a new tensor, instead, it returns a readonly view
of the tensor with its elements shifted circularly over the given axes.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> a;
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
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    np::index_t<2> shift;
    std::cin >> a >> shift[0] >> shift[1];
    std::cout << "Axis 0:\n" << np::rotate(a, shift[0], 0) << "\n";
    std::cout << "Axis 1:\n" << np::rotate(a, shift[1], 1) << "\n";
    std::cout << "Both axes:\n" << np::rotate(a, shift, {0, 1}) << "\n";
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
[[-3, 13, -2, 15, 11, 18],
 [15,  1,  0, -5, 10, -4],
 [-1,  4,  3, -3,  9,  3],
 [12,  2, 18, -1, -1, -3]]
Both axes:
[[12,  2, 18, -1, -1, -3],
 [-3, 13, -2, 15, 11, 18],
 [15,  1,  0, -5, 10, -4],
 [-1,  4,  3, -3,  9,  3]]
```
