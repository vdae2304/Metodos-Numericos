# Routines

Defined in header [`numcpp/random.h`](/include/numcpp/random.h)

- [Routines](#routines)
  - [Permutations](#permutations)
    - [`Generator::shuffle`](#generatorshuffle)
    - [`Generator::permutation`](#generatorpermutation)

## Permutations

### `Generator::shuffle`

Modify a tensor in-place by shuffling its contents.
```cpp
template <class T, size_t Rank>
void shuffle(tensor<T, Rank> &a, size_t axis = Rank - 1);
```

Parameters

* `a` The tensor to be shuffled.
* `axis` Axis along which to shuffle. Defaults to `Rank - 1`, which means
shuffle along the last axis.

Returns

* None

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    np::vector<int> a;
    std::cin >> a;
    rng.shuffle(a);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Possible output

```
[19, -5, -2, -3,  3, 12,  4, 12, 18,  0]
```

### `Generator::permutation`

Return a permuted range.
```cpp
template <class T>
tensor<T, 1> permutation(T n);
```

Parameters

* `n` Randomly permute the range $0, 1, 2, \ldots, n - 1.$

Returns

* The permuted range.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    int n;
    std::cin >> n;
    std::cout << rng.permutation(n) << "\n";
    return 0;
}
```

Input

```
10
```

Possible output

```
[2, 7, 1, 6, 8, 0, 4, 9, 5, 3]
```

<h3><code>Generator::permutation</code></h3>

Randomly permute a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, 1> permutation(const tensor<T, Rank> &a);

template <class T, size_t Rank>
tensor<T, Rank> permutation(const tensor<T, Rank> &a, size_t axis);
```

Parameters

* `a` Make a copy of the tensor and shuffle the elements randomly.
* `axis` Axis along which to permute. If not provided, the flattened tensor is
used.

Returns

* The permuted tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    np::vector<int> a;
    std::cin >> a;
    std::cout << rng.permutation(a) << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Possible output

```
[19, -5, -2, -3,  3, 12,  4, 12, 18,  0]
```

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    np::matrix<int> a;
    size_t axis;
    std::cin >> a >> axis;
    std::cout << rng.permutation(a, axis) << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
0
```

Possible output

```
[[ 9, 18, -1,  2, -2, -5],
 [-1, -3, 15, 13,  0, -3],
 [10, -4, 12,  4,  3, -1],
 [11,  3, -3,  1, 18, 15]]
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
1
```

Possible output

```
[[-3, 13, 18, -2, 15, 11],
 [-5,  0,  1, 10, 15, -4],
 [ 4,  9, -3,  3,  3, -1],
 [ 2, 12, 18, -1, -3, -1]]
```
