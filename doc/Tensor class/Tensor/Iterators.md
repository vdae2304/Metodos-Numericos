# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Iterators](#iterators)
    - [`tensor::begin`](#tensorbegin)
    - [`tensor::end`](#tensorend)
    - [`tensor::cbegin`](#tensorcbegin)
    - [`tensor::cend`](#tensorcend)

## Iterators

### `tensor::begin`

Return an iterator pointing to the first element in the tensor.
```cpp
iterator begin();
const_iterator begin() const;

iterator begin(layout_t order);
const_iterator begin(layout_t order) const;
```

Parameters

* `order` It is an optional parameter that changes the order in which elements are iterated. In row-major order, the last index is varying the fastest. In column-major order, the first index is varying the fastest. The default is to use the same layout as stored in memory.

Returns

* A random access iterator to the beginning of the tensor. If the tensor is const-qualified, the function returns a `const_iterator`. Otherwise, it returns an `iterator`.

### `tensor::end`

Return an iterator pointing to the past-the-end element in the tensor. It does not point to any element, and thus shall not be dereferenced.
```cpp
iterator end();
const_iterator end() const;

iterator end(layout_t order);
const_iterator end(layout_t order) const;
```

Parameters

* `order` It is an optional parameter that changes the order in which elements are iterated. In row-major order, the last index is varying the fastest. In column-major order, the first index is varying the fastest. The default is to use the same layout as stored in memory.

Returns

* A random access iterator to the element past the end of the tensor. If the tensor is const-qualified, the function returns a `const_iterator`. Otherwise, it returns an `iterator`.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a(10);
    int value = 0;
    for (auto it = a.begin(); it != a.end(); ++it) {
        *it = value++;
    }
    std::cout << a << "\n";
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
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a(3, 4);

    int value = 0;
    for (auto it = a.begin(); it != a.end(); ++it) {
        *it = value++;
    }
    std::cout << "Row-major order:\n" << a << "\n";
    
    value = 0;
    for (auto it = a.begin(np::column_major); it != a.end(np::column_major); ++it) {
        *it = value++;
    }
    std::cout << "Column-major order:\n" << a << "\n";
    return 0;
}
```

Output

```
Row-major order:
[[ 0,  1,  2,  3],
 [ 4,  5,  6,  7],
 [ 8,  9, 10, 11]]
Column-major order:
[[ 0,  3,  6,  9],
 [ 1,  4,  7, 10],
 [ 2,  5,  8, 11]]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::tensor<int, 3> a(2, 3, 4);

    int value = 0;
    for (auto it = a.begin(); it != a.end(); ++it) {
        *it = value++;
    }
    std::cout << "Row-major order:\n" << a << "\n";
    
    value = 0;
    for (auto it = a.begin(np::column_major); it != a.end(np::column_major); ++it) {
        *it = value++;
    }
    std::cout << "Column-major order:\n" << a << "\n";
    return 0;
}
```

Output

```
Row-major order:
[[[ 0,  1,  2,  3],
  [ 4,  5,  6,  7],
  [ 8,  9, 10, 11]],

 [[12, 13, 14, 15],
  [16, 17, 18, 19],
  [20, 21, 22, 23]]]
Column-major order:
[[[ 0,  6, 12, 18],
  [ 2,  8, 14, 20],
  [ 4, 10, 16, 22]],

 [[ 1,  7, 13, 19],
  [ 3,  9, 15, 21],
  [ 5, 11, 17, 23]]]
```

### `tensor::cbegin`

Return a `const_iterator` pointing to the first element in the tensor.
```cpp
const_iterator cbegin() const;
const_iterator cbegin(layout_t order) const;
```

Parameters

* `order` It is an optional parameter that changes the order in which elements are iterated. In row-major order, the last index is varying the fastest. In column-major order, the first index is varying the fastest. The default is to use the same layout as stored in memory.

Returns

* A `const_iterator` to the beginning of the tensor.

### `tensor::cend`

Return a `const_iterator` pointing to the past-the-end element in the tensor.
```cpp
const_iterator cend() const;
const_iterator cend(layout_t order) const;
```

Parameters

* `order` It is an optional parameter that changes the order in which elements are iterated. In row-major order, the last index is varying the fastest. In column-major order, the first index is varying the fastest. The default is to use the same layout as stored in memory.

Returns

* A `const_iterator` to the element past the end of the tensor.
