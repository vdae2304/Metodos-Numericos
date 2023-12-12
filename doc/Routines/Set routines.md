# [Routines](readme.md)

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Set routines](#set-routines)
    - [`unique`](#unique)
    - [`contains`](#contains)
    - [`is_subset`](#is_subset)
    - [`set_union`](#set_union)
    - [`set_intersection`](#set_intersection)
    - [`set_difference`](#set_difference)
    - [`set_symmetric_difference`](#set_symmetric_difference)

## Set routines

### `unique`

Find the sorted unique elements of a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, 1> unique(const tensor<T, Rank> &a);
```

Parameters

* `a` A tensor-like object.

Returns

* A new tensor with the sorted unique elements of `a`.

Notes

* The elements of `a` are not required to be sorted beforehand. However, this function might perform faster if the elements of `a` are already sorted.

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
    std::cout << np::unique(a) << "\n";
    return 0;
}
```

Input

```
[2, 2, 3, 3, 3, 4, 5, 8, 9, 9]
```

Output

```
[2, 3, 4, 5, 8, 9]
```

### `contains`

Test whether a value is present in a tensor.
```cpp
template <class T>
bool contains(const tensor<T, 1> &a,
              const typename tensor<T, 1>::value_type &val);
```

Parameters

* `a` A 1-dimensional tensor-like object. This function tests whether `a` contains `val`. The elements in `a` must be sorted.
* `val` The value to test.

Returns

* true if `val` is present in `a` and false otherwise.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a, val;
    std::cin >> a >> val;
    for (unsigned i = 0; i < val.size(); ++i) {
        if (np::contains(a, val[i])) {
            std::cout << val[i] << " is in a\n";
        } else {
            std::cout << val[i] << " is not in a\n";
        }
    }
    return 0;
}
```

Input

```
[2, 3, 4, 5, 8, 9, 10, 12, 15, 18]
[1, 2, 3, 5, 7]
```

Output

```
1 is not in a
2 is in a
3 is in a
5 is in a
7 is not in a
```

### `is_subset`

Test whether all the elements in a tensor are also present in another tensor.
```cpp
template <class T>
bool is_subset(const tensor<T, 1> &a, const tensor<T, 1> &b);
```

Parameters

* `a` A 1-dimensional tensor-like object. This function test whether `b` contains all the elements of `a`. The elements in `a` must be sorted.
* `b` A 1-dimensional tensor-like object. The elements in `b` must be sorted.

Returns

* true if `a` is a subset of `b` and false otherwise.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a, b;
    std::cin >> a >> b;
    if (np::is_subset(a, b)) {
        std::cout << "a is a subset of b\n";
    } else {
        std::cout << "a is not a subset of b\n";
    }
    return 0;
}
```

Input

```
[1, 2, 3, 5, 7]
[2, 3, 4, 5, 8, 9, 10, 12, 15, 18]
```

Output

```
a is not a subset of b
```

Input

```
[2, 3, 5, 10, 12]
[2, 3, 4, 5, 8, 9, 10, 12, 15, 18]
```

Output

```
a is a subset of b
```

### `set_union`

Find the set union of two tensors. Return the unique sorted elements that are present in either one of two tensors, or in both.
```cpp
template <class T>
tensor<T, 1> set_union(const tensor<T, 1> &a, const tensor<T, 1> &b);
```

Parameters

* `a` A 1-dimensional tensor-like object. The elements in `a` must be sorted.
* `b` A 1-dimensional tensor-like object. The elements in `b` must be sorted.

Returns

* A new tensor with the set union of both tensors.

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
    np::vector<int> a, b;
    std::cin >> a >> b;
    std::cout << np::set_union(a, b) << "\n";
    return 0;
}
```

Input

```
[2, 3, 5, 7, 8, 9, 10]
[5, 6, 7, 8, 9, 12, 15]
```

Output

```
[ 2,  3,  5,  6,  7,  8,  9, 10, 12, 15]
```

### `set_intersection`

Find the set intersection of two tensors. Return the unique sorted elements that are present in both tensors.
```cpp
template <class T>
tensor<T, 1> set_intersection(const tensor<T, 1> &a, const tensor<T, 1> &b);
```

Parameters

* `a` A 1-dimensional tensor-like object. The elements in `a` must be sorted.
* `b` A 1-dimensional tensor-like object. The elements in `b` must be sorted.

Returns

* A new tensor with the set intersection of both tensors.

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
    np::vector<int> a, b;
    std::cin >> a >> b;
    std::cout << np::set_intersection(a, b) << "\n";
    return 0;
}
```

Input

```
[2, 3, 5, 7, 8, 9, 10]
[5, 6, 7, 8, 9, 12, 15]
```

Output

```
[5, 7, 8, 9]
```

### `set_difference`

Find the set difference of two tensors. Return the unique sorted elements that are present in the first tensor, but not in the second.
```cpp
template <class T>
tensor<T, 1> set_difference(const tensor<T, 1> &a, const tensor<T, 1> &b);
```

Parameters

* `a` A 1-dimensional tensor-like object. The elements in `a` must be sorted.
* `b` A 1-dimensional tensor-like object. The elements in `b` must be sorted.

Returns

* A new tensor with the set difference of both tensors.

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
    np::vector<int> a, b;
    std::cin >> a >> b;
    std::cout << np::set_difference(a, b) << "\n";
    return 0;
}
```

Input

```
[2, 3, 5, 7, 8, 9, 10]
[5, 6, 7, 8, 9, 12, 15]
```

Output

```
[ 2,  3, 10]
```

### `set_symmetric_difference`

Find the set symmetric difference of two tensors. Return the unique sorted elements that are present in one of the tensors, but not in the other.
```cpp
template <class T>
tensor<T, 1> set_symmetric_difference(const tensor<T, 1> &a,
                                      const tensor<T, 1> &b);
```

Parameters

* `a` A 1-dimensional tensor-like object. The elements in `a` must be sorted.
* `b` A 1-dimensional tensor-like object. The elements in `b` must be sorted.

Returns

* A new tensor with the set symmetric difference of both tensors.

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
    np::vector<int> a, b;
    std::cin >> a >> b;
    std::cout << np::set_symmetric_difference(a, b) << "\n";
    return 0;
}
```

Input

```
[2, 3, 5, 7, 8, 9, 10]
[5, 6, 7, 8, 9, 12, 15]
```

Output

```
[ 2,  3,  6, 10, 12, 15]
```
