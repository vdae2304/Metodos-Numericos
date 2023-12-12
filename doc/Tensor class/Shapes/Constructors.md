# [Shapes](./readme.md)

Defined in header [`numcpp/shape.h`](/include/numcpp/shape.h)

- [Shapes](#shapes)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Initialization constructor](#initialization-constructor)
    - [Copy constructor](#copy-constructor)

## Constructors

### Default constructor

Constructs a `shape_t` object with its elements initialized to zero.
```cpp
shape_t();
```

### Initialization constructor

Initializes each element with the corresponding element in `sizes`.
```cpp
template <class... Sizes>
shape_t(Sizes... sizes);
```

Parameters

* `sizes...` Size along each axis.

Example

```cpp
#include <iostream>
#include <numcpp/shape.h>
namespace np = numcpp;
int main() {
    np::shape_t<1> shape1(10);
    std::cout << shape1 << "\n";
    np::shape_t<2> shape2(4, 6);
    std::cout << shape2 << "\n";
    np::shape_t<3> shape3(3, 4, 6);
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

### Copy constructor

The object is initialized with the contents of `other`.
```cpp
shape_t(const shape_t &other);
```

Parameters

* `other` A `shape_t` object with the same dimension to copy.
