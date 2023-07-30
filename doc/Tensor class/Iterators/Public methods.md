# [Iterators](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Iterators](#iterators)
  - [Public methods](#public-methods)
    - [`iterator::base`](#iteratorbase)
    - [`iterator::index`](#iteratorindex)
    - [`iterator::coords`](#iteratorcoords)
    - [`iterator::layout`](#iteratorlayout)

## Public methods

### `iterator::base`

Accesses the underlying tensor.
```cpp
container_type *base() const;
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec{-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    np::vector<int>::iterator it = vec.begin();
    // Returns a pointer to vec
    std::cout << *it.base() << "\n";
    return 0;
}
```

Output

```
[-4, 16, 14,  9, 18,  3,  7,  2,  1,  4]
```

### `iterator::index`

Returns the current flat index.
```cpp
size_t index() const;
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec{-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    // Prints 0, 1, 2, ...
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << it.index() << ", ";
    }
    std::cout << "\n";
    return 0;
}
```

Output

```
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
```

### `iterator::coords`

Returns an `index_t` object with the current coordinates.
```cpp
index_t<Rank> coords() const;
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;
int main() {
    np::vector<int> vec{-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << it.coords() << ": " << *it << "\n";
    }
    return 0;
}
```

Output

```
(0,): -4
(1,): 16
(2,): 14
(3,): 9
(4,): 18
(5,): 3
(6,): 7
(7,): 2
(8,): 1
(9,): 4
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;

void print_coords(np::matrix<int> &mat, np::layout_t order) {
    for (auto it = mat.begin(order); it != mat.end(order); ++it) {
        std::cout << it.coords() << ": " << *it << "\n";
    }
}

int main() {
    np::matrix<int> mat{{0, 10, -4,  5},
                        {6, 10,  8, 12},
                        {2, 11,  0, -1}};
    std::cout << "Row-major order:\n";
    print_coords(mat, np::row_major);
    std::cout << "Column-major order:\n";
    print_coords(mat, np::column_major);
    return 0;
}
```

Output

```
Row-major order:
(0, 0): 0
(0, 1): 10
(0, 2): -4
(0, 3): 5
(1, 0): 6
(1, 1): 10
(1, 2): 8
(1, 3): 12
(2, 0): 2
(2, 1): 11
(2, 2): 0
(2, 3): -1
Column-major order:
(0, 0): 0
(1, 0): 6
(2, 0): 2
(0, 1): 10
(1, 1): 10
(2, 1): 11
(0, 2): -4
(1, 2): 8
(2, 2): 0
(0, 3): 5
(1, 3): 12
(2, 3): -1
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
namespace np = numcpp;

void print_coords(np::tensor<int, 3> &cube, np::layout_t order) {
    for (auto it = cube.begin(order); it != cube.end(order); ++it) {
        std::cout << it.coords() << ": " << *it << "\n";
    }
}

int main() {
    np::tensor<int, 3> cube{{{ 1, 18, 11, 10},
                             { 9, 19, 12, 10},
                             {13,  8, -4, 16}},
                            {{ 2,  4, 14, 19},
                             {18,  5, 19, 18},
                             { 0,  0, 15, 17}}};
    std::cout << "Row-major order:\n";
    print_coords(cube, np::row_major);
    std::cout << "Column-major order:\n";
    print_coords(cube, np::column_major);
    return 0;
}
```

Output

```
Row-major order:
(0, 0, 0): 1
(0, 0, 1): 18
(0, 0, 2): 11
(0, 0, 3): 10
(0, 1, 0): 9
(0, 1, 1): 19
(0, 1, 2): 12
(0, 1, 3): 10
(0, 2, 0): 13
(0, 2, 1): 8
(0, 2, 2): -4
(0, 2, 3): 16
(1, 0, 0): 2
(1, 0, 1): 4
(1, 0, 2): 14
(1, 0, 3): 19
(1, 1, 0): 18
(1, 1, 1): 5
(1, 1, 2): 19
(1, 1, 3): 18
(1, 2, 0): 0
(1, 2, 1): 0
(1, 2, 2): 15
(1, 2, 3): 17
Column-major order:
(0, 0, 0): 1
(1, 0, 0): 2
(0, 1, 0): 9
(1, 1, 0): 18
(0, 2, 0): 13
(1, 2, 0): 0
(0, 0, 1): 18
(1, 0, 1): 4
(0, 1, 1): 19
(1, 1, 1): 5
(0, 2, 1): 8
(1, 2, 1): 0
(0, 0, 2): 11
(1, 0, 2): 14
(0, 1, 2): 12
(1, 1, 2): 19
(0, 2, 2): -4
(1, 2, 2): 15
(0, 0, 3): 10
(1, 0, 3): 19
(0, 1, 3): 10
(1, 1, 3): 18
(0, 2, 3): 16
(1, 2, 3): 17
```

### `iterator::layout`

Returns the order in which elements are iterated.
```cpp
layout_t layout() const;
```
