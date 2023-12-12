# [Shapes](./readme.md)

Defined in header [`numcpp/shape.h`](/include/numcpp/shape.h)

- [Shapes](#shapes)
  - [Public methods](#public-methods)
    - [`shape_t::prod`](#shape_tprod)
    - [`shape_t::data`](#shape_tdata)

## Public methods

### `shape_t::prod`

Return the product of the sizes along all the axes.
```cpp
size_type prod() const;
```

Example

```cpp
#include <iostream>
#include <numcpp/shape.h>
namespace np = numcpp;
int main() {
    np::shape_t<1> shape1(10);
    std::cout << shape1.prod() << "\n";
    np::shape_t<2> shape2(4, 6);
    std::cout << shape2.prod() << "\n";
    np::shape_t<3> shape3(3, 4, 6);
    std::cout << shape3.prod() << "\n";
    return 0;
}
```

Output

```
10
24
72
```

### `shape_t::data`

Return a pointer to the block of memory containing the elements of the shape.
```cpp
size_type *data();
const size_type *data() const;
```

Example

```cpp
#include <iostream>
#include <algorithm> // std::reverse
#include <numcpp/shape.h>
namespace np = numcpp;
int main() {
    np::shape_t<3> shape(3, 4, 6);
    std::reverse(shape.data(), shape.data() + 3);
    std::cout << shape << "\n";
    return 0;
}
```

Output

```
(6, 4, 3)
```
