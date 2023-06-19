# [Shapes](./readme.md)

Defined in header [`numcpp/shape.h`](/include/numcpp/shape.h)

- [Shapes](#shapes)
  - [Deduction guides (since C++17)](#deduction-guides-since-c17)

## Deduction guides (since C++17)

One deduction guide is provided for `shape_t` to allow deduction of the rank from the number of arguments.
```cpp
template <class... Sizes>
shape_t(Sizes... sizes) -> shape_t<sizeof...(Sizes)>;
```

Example

```cpp
#include <iostream>
#include "numcpp/shape.h"
namespace np = numcpp;
int main() {
    // Deduces to shape_t<1>.
    np::shape_t shape1(10);
    std::cout << shape1 << "\n";
    // Deduces to shape_t<2>.
    np::shape_t shape2(4, 6);
    std::cout << shape2 << "\n";
    // Deduces to shape_t<3>.
    np::shape_t shape3(3, 4, 6);
    std::cout << shape3 << "\n";
    return 0;
}
```

Output

```
(10,)
(4, 6)
(3, 4, 6)
```
