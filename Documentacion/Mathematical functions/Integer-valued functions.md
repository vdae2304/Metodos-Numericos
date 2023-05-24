# [Mathematical functions](./readme.md)

Defined in `numcpp/math.h`

- [Mathematical functions](#mathematical-functions)
  - [Integer-valued functions](#integer-valued-functions)
    - [`gcd`](#gcd)
    - [`lcm`](#lcm)

## Integer-valued functions

### `gcd`

Return the greatest common divisor of $|m|$ and $|n|$.
```cpp
template <class T1, class T2>
typename std::common_type<T1, T2>::type gcd(T1 m, T2 n);
```

Parameters

* `m` An integer value.
* `n` An integer value.

Returns

* The greatest common divisor of $|m|$ and $|n|$. If both $m$ and $n$ are zero, return zero.

<h3><code>gcd</code></h3>

Return the greatest common divisor, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> gcd(const tensor<T, Rank> &m, const tensor<T, Rank> &n);

template <class T, size_t Rank>
tensor<T, Rank> gcd(const tensor<T, Rank> &m,
                    const typename tensor<T, Rank>::value_type &n);

template <class T, size_t Rank>
tensor<T, Rank> gcd(const typename tensor<T, Rank>::value_type &m,
                    const tensor<T, Rank> &n);
```

Parameters

* `m` A tensor-like object with integer values.
* `n` A tensor-like object with integer values.

Returns

* A light-weight object with the greatest common divisor of $|m|$ and $|n|$, element-wise. If both $m$ and $n$ are zero, return zero. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<int> m, n;
    std::cin >> m >> n;
    std::cout << np::gcd(m, n) << "\n";
    return 0;
}
```

Input

```
[24, 36, 45, 20, 20, 120, 10, 0,  0, 30]
[20, 40, 75, 18, 19, 100,  0, 0, 30, 24]
```

Output

```
[ 4,  4, 15,  2,  1, 20, 10,  0, 30,  6]
```

### `lcm`

Return the least common multiple of $|m|$ and $|n|$.
```cpp
template <class T1, class T2>
typename std::common_type<T1, T2>::type lcm(T1 m, T2 n);
```

Parameters

* `m` An integer value.
* `n` An integer value.

Returns

* The least common multiple of $|m|$ and $|n|$. If either $m$ or $n$ is zero, return zero.

<h3><code>lcm</code></h3>

Return the least common multiple, element-wise.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> lcm(const tensor<T, Rank> &m, const tensor<T, Rank> &n);

template <class T, size_t Rank>
tensor<T, Rank> lcm(const tensor<T, Rank> &m,
                    const typename tensor<T, Rank>::value_type &n);

template <class T, size_t Rank>
tensor<T, Rank> lcm(const typename tensor<T, Rank>::value_type &m,
                    const tensor<T, Rank> &n);
```

Parameters

* `m` A tensor-like object with integer values.
* `n` A tensor-like object with integer values.

Returns

* A light-weight object with the least common multiple of $|m|$ and $|n|$, element-wise. If either $m$ or $n$ is zero, return zero. This function does not create a new tensor, instead, an expression object is returned (see lazy-evaluation).

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::vector<int> m, n;
    std::cin >> m >> n;
    std::cout << np::lcm(m, n) << "\n";
    return 0;
}
```

Input

```
[24, 36, 45, 20, 20, 120, 10, 0,  0, 30]
[20, 40, 75, 18, 19, 100,  0, 0, 30, 24]
```

Output

```
[120, 360, 225, 180, 380, 600,   0,   0,   0, 120]
```
