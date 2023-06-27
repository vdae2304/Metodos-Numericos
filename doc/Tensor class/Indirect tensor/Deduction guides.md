# [Indirect tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Indirect tensor](#indirect-tensor)
  - [Deduction guides (since C++17)](#deduction-guides-since-c17)

## Deduction guides (since C++17)

One deduction guide is provided for `indirect_tensor` to allow deduction of the value type and rank from a pointer.
```cpp
#if __cplusplus >= 201703L
template <class T, class... Sizes>
indirect_tensor(T *data, Sizes... sizes)
    -> indirect_tensor<T, sizeof...(sizes)>;

template <class T, size_t Rank>
indirect_tensor(T *data, const shape_t<Rank> &shape,
                layout_t order = default_layout) -> indirect_tensor<T, Rank>;
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    np::indirect_tensor vec(data, 10);
    std::cout << vec << "\n";
    return 0;
}
```

Output

```
[-4, 16, 14,  9, 18,  3,  7,  2,  1,  4]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
    np::indirect_tensor mat1(data, 3, 4);
    std::cout << "Row-major order:\n" << mat1 << "\n";
    np::indirect_tensor mat2(data, np::shape_t(3, 4), np::column_major);
    std::cout << "Column-major order:\n" << mat2 << "\n";
    return 0;
}
```

Output

```
Row-major order:
[[ 0, 10, -4,  5],
 [ 6, 10,  8, 12],
 [ 2, 11,  0, -1]]
Column-major order:
[[ 0,  5,  8, 11],
 [10,  6, 12,  0],
 [-4, 10,  2, -1]]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    int data[24] = { 1, 18, 11, 10,  9, 19, 12, 10, 13,  8,
                    -4, 16 , 2,  4, 14, 19, 18,  5, 19, 18,
                     0,  0, 15, 17};
    np::indirect_tensor cube1(data, 2, 3, 4);
    std::cout << "Row-major order:\n" << cube1 << "\n";
    np::indirect_tensor cube2(data, np::shape_t(2, 3, 4), np::column_major);
    std::cout << "Column-major order:\n" << cube2 << "\n";
    return 0;
}
```

Output

```
Row-major order:
[[[ 1, 18, 11, 10],
  [ 9, 19, 12, 10],
  [13,  8, -4, 16]],

 [[ 2,  4, 14, 19],
  [18,  5, 19, 18],
  [ 0,  0, 15, 17]]]
Column-major order:
[[[ 1, 12,  2, 19],
  [11, 13, 14,  0],
  [ 9, -4, 18, 15]],

 [[18, 10,  4, 18],
  [10,  8, 19,  0],
  [19, 16,  5, 17]]]
```
