# Slices

Defined in `numcpp/slice.h`

- [Slices](#slices)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Slice constructor](#slice-constructor)
    - [Copy constructor](#copy-constructor)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
  - [Public methods](#public-methods)
    - [`slice::start`](#slicestart)
    - [`slice::last`](#slicelast)
    - [`slice::size`](#slicesize)
    - [`slice::stride`](#slicestride)
  - [Operators](#operators)
    - [`slice::operator[]`](#sliceoperator)
    - [`operator==`](#operator)
    - [`operator!=`](#operator-1)

A `slice` is a class that identifies a subset of elements in an array. It holds
three values: the starting index, the stride and the number of elements in the
subset.
```cpp
class slice;
```

## Constructors

### Default constructor

Construct an empty slice. Equivalent to `slice(0, 0, 0)`
```cpp
slice();
```

### Slice constructor

Constructs a slice object.
```cpp
slice(size_t size);
slice(size_t start, size_t size);
slice(size_t start, size_t size, size_t stride);
```

Parameters

* `start` The position of the first element in the slice. Defaults to 0 if not
provided.
* `size` The number of elements in the slice.
* `stride` The span that separates the elements selected into the slice.
Defaults to 1 if not provided.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr{5, 8, 16, 16, 17, 20, 4, 10, 1, 6};
    // Selects indices 0, 1, 2, 3, 4.
    std::cout << arr(np::slice(5)) << '\n';
    // Selects indices 2, 3, 4, 5, 6.
    std::cout << arr(np::slice(2, 5)) << '\n';
    // Selects indices 1, 3, 5, 7, 9.
    std::cout << arr(np::slice(1, 5, 2)) << '\n';
    return 0;
}
```

Output

```
[ 5,  8, 16, 16, 17]
[16, 16, 17, 20,  4]
[ 8, 16, 20, 10,  6]
```

### Copy constructor

Constructs a slice as a copy of other.
```cpp
slice(const slice& other);
```

Parameters

* `other` A slice object.

## Assignment

### Copy assignment

Assigns a copy of other.
```cpp
slice& operator=(const slice& other)
```

Parameters

* `other` A slice object.

Returns

* `*this`

## Public methods

### `slice::start`

Returns the index of the first element in the slice.
```cpp
size_t start() const
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::slice slc1(5);
    std::cout << slc1.start() << '\n';
    np::slice slc2(2, 5);
    std::cout << slc2.start() << '\n';
    np::slice slc3(1, 5, 2);
    std::cout << slc3.start() << '\n';
    return 0;
}
```

Output

```
0
2
1
```

### `slice::last`

Return the index of the last element in the slice. The behaviour is undefined
if the slice is empty.
```cpp
size_t last() const
```

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::slice slc1(5);
    std::cout << slc1.last() << '\n';
    np::slice slc2(2, 5);
    std::cout << slc2.last() << '\n';
    np::slice slc3(1, 5, 2);
    std::cout << slc3.last() << '\n';
    return 0;
}
```

Output

```
4
6
9
```

### `slice::size`

Returns the number of elements in the slice.
```cpp
size_t size() const
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::slice slc1(5);
    std::cout << slc1.size() << '\n';
    np::slice slc2(2, 5);
    std::cout << slc2.size() << '\n';
    np::slice slc3(1, 5, 2);
    std::cout << slc3.size() << '\n';
    return 0;
}
```

Output

```
5
5
5
```

### `slice::stride`

Returns the separation of the elements in the slice.
```cpp
size_t stride() const
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::slice slc1(5);
    std::cout << slc1.stride() << '\n';
    np::slice slc2(2, 5);
    std::cout << slc2.stride() << '\n';
    np::slice slc3(1, 5, 2);
    std::cout << slc3.stride() << '\n';
    return 0;
}
```

Output

```
1
1
2
```

## Operators

### `slice::operator[]`

Returns the element at position `i` in the slice.
```cpp
size_t operator[](size_t i) const
```

Parameters

* `i` Position of an element in the slice. This function does not check for out
of bounds.

Returns

* `start() + i * stride()`

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    int data[10] = {5, 8, 16, 16, 17, 20, 4, 10, 1, 6};
    np::slice slc(1, 5, 2);
    for (unsigned i = 0; i < slc.size(); ++i) {
        std::cout << data[slc[i]] << ", ";
    }
    return 0;
}
```

Output

```
8, 16, 20, 10, 6,
```

### `operator==`

Compares if two slices are equal.
```cpp
bool operator==(const slice &lhs, const slice &rhs);
```

### `operator!=`

Compares if two slices are different.
```cpp
bool operator!=(const slice &lhs, const slice &rhs);
```