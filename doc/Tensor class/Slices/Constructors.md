# [Slice](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Slice](#slice)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Slice constructor](#slice-constructor)
    - [Placeholders](#placeholders)
    - [Copy constructor](#copy-constructor)

## Constructors

### Default constructor

Construct an empty slice. Equivalent to `slice(0, 0, 0)`.
```cpp
slice();
```

### Slice constructor

Constructs a slice with values within the half-open interval `[start, stop)`.
```cpp
explicit slice(size_t stop);
explicit slice(size_t start, size_t stop);
explicit slice(size_t start, size_t stop, size_t stride);
```

Parameters

* `start` The position of the first element selected by the slice. Defaults to 0 if not provided.
* `stop` The position at which the slice ends. The slice does not include this position.
* `stride` The span that separates the elements selected by the slice. Defaults to 1 if not provided.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    // Select first 5 positions.
    std::cout << vec(np::slice(5)) << "\n";
    // Select positions 2, 3, 4, 5, 6.
    std::cout << vec(np::slice(2, 7)) << "\n";
    // Select positions 1, 3, 5, 7, 9.
    std::cout << vec(np::slice(1, 10, 2)) << "\n";
    return 0;
}
```

Output

```
[ 7, 13, 19, 11,  5]
[19, 11,  5,  8, -2]
[13, 11,  8,  7,  3]
```

### Placeholders

Additional constructors when either one of `start` or `stop` is omitted.

The placeholder `_`, defined in the `placeholders` namespace, can be used where `start` and `stop` arguments is expected. If a placeholder is used in the place of `start`, its value is deduced to 0. If a placeholder is used in the place of `stop`, its value is deduced to the size of the tensor along the indexed axis.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::placeholders;
    np::vector<int> vec{7, 13, 19, 11, 5, 8, -2, 7, 11, 3};
    // Select all positions.
    std::cout << vec(np::slice(_, _)) << "\n";
    // Select positions 0, 1, 2, 3, 4.
    std::cout << vec(np::slice(_, 5)) << "\n";
    // Select positions 5, 6, 7, 8, 9.
    std::cout << vec(np::slice(5, _)) << "\n";
    // Select positions 0, 2, 4, 6, 8.
    std::cout << vec(np::slice(_, _, 2)) << "\n";
    return 0;
}
```

Output

```
[ 7, 13, 19, 11,  5,  8, -2,  7, 11,  3]
[ 7, 13, 19, 11,  5]
[ 8, -2,  7, 11,  3]
[ 7, 19,  5, -2, 11]
```

### Copy constructor

Constructs a slice as a copy of `other`.
```cpp
slice(const slice &other);
```

Parameters

* `other` A slice object.
