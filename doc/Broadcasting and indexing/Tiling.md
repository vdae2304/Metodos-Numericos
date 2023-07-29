# [Broadcasting and indexing](readme.md)

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Tiling](#tiling)
    - [`tile`](#tile)
    - [`repeat`](#repeat)

## Tiling

### `tile`

Construct a tensor by repeating `a` the number of times given by `reps`.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> tile(const tensor<T, Rank> &a, const shape_t<Rank> &reps);
```

Parameters

* `a` The input tensor.
* `reps` The number of repetitions of `a` along each axis.

Returns

* The tiled tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> a;
    np::shape_t<1> reps;
    std::cin >> a >> reps;
    std::cout << np::tile(a, reps) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
2
```

Output

```
[1, 2, 3, 4, 5, 1, 2, 3, 4, 5]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    np::shape_t<2> reps;
    std::cin >> a >> reps;
    std::cout << np::tile(a, reps) << "\n";
    return 0;
}
```

Input

```
[[1, 2, 3],
 [4, 5, 6]]
(3, 2)
```

Output

```
[[1, 2, 3, 1, 2, 3],
 [4, 5, 6, 4, 5, 6],
 [1, 2, 3, 1, 2, 3],
 [4, 5, 6, 4, 5, 6],
 [1, 2, 3, 1, 2, 3],
 [4, 5, 6, 4, 5, 6]]
```

### `repeat`

Repeat elements of a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> repeat(const tensor<T, Rank> &a, size_t reps, size_t axis = 0);

template <class T, size_t Rank, class IntegralType>
tensor<T, Rank> repeat(const tensor<T, Rank> &a,
                       const tensor<IntegralType, 1> &reps, size_t axis = 0);
```

Parameters

* `a` The input tensor.
* `reps` The number of repetitions of each element.
* `axis` The axis along which to repeat values. Default is 0.

Returns

* A new tensor which has the same shape as `a`, except along the given axis.

Exceptions

* `std::invalid_argument` Thrown if the size of `reps` does not match the size of `a` along the given axis.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec;
    int reps;
    std::cin >> vec >> reps;
    std::cout << np::repeat(vec, reps) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
2
```

Output

```
[1, 1, 2, 2, 3, 3, 4, 4, 5, 5]
```

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> vec, reps;
    std::cin >> vec >> reps;
    std::cout << np::repeat(vec, reps) << "\n";
    return 0;
}
```

Input

```
[1, 2, 3, 4, 5]
[4, 0, 1, 3, 2]
```

Output

```
[1, 1, 1, 1, 3, 4, 4, 4, 5, 5]
```
