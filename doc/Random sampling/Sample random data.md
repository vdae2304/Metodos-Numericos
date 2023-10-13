# Routines

Defined in header [`numcpp/random.h`](/include/numcpp/random.h)

- [Routines](#routines)
  - [Sample random data](#sample-random-data)
    - [`Generator::integers`](#generatorintegers)
    - [`Generator::random`](#generatorrandom)
    - [`Generator::choice`](#generatorchoice)

## Sample random data

### `Generator::integers`

Return random integers from `low` to `high` (inclusive).
```cpp
template <class T, class U>
typename std::common_type<T, U>::type integers(T low, U high);
```

Parameters

* `low` Lowest integer to be drawn.
* `high` Largest integer to be drawn.

Returns

* A random integer.

<h3><code>Generator::integers</code></h3>

Return a tensor of random integers from `low` to `high` (inclusive).
```cpp
template <class T, class U>
tensor<typename std::common_type<T, U>::type, 1>
integers(T low, U high, size_t size);

template <class T, class U, size_t Rank>
tensor<typename std::common_type<T, U>::type, Rank>
integers(T low, U high, const shape_t<Rank> &size);
```

Parameters

* `low` Lowest integer to be drawn.
* `high` Largest integer to be drawn.
* `size` Output shape.

Returns

* A tensor of random integers.

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
    int low, high, size;
    std::cin >> low >> high >> size;
    np::vector<int> a = rng.integers(low, high, size);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
0 100 10
```

Possible output

```
[ 0, 13, 76, 46, 53, 22,  4, 68, 68, 94]
```

### `Generator::random`

Return random floating point numbers in the half-open interval $[0, 1)$.
```cpp
template <class T = double>
T random();
```

Parameters

* None

Returns

* A random floating point number.

<h3><code>Generator::random</code></h3>

Return a tensor of random floating point numbers in the half-open interval
$[0, 1)$.
```cpp
template <class T = double>
tensor<T, 1> random(size_t size);

template <class T = double, size_t Rank>
tensor<T, Rank> random(const shape_t<Rank> &size);
```

Parameters

* `size` Output shape.

Returns

* A tensor of random floating point numbers.

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
    int size;
    std::cin >> size;
    np::vector<double> a = rng.random(size);
    std::cout << a << "\n";
    return 0;
}
```

Input

```
10
```

Possible output

```
[  0.13153779,   0.45865013,   0.21895919,   0.67886472,    0.9346929, 
   0.51941637,   0.03457211,   0.52970019, 0.0076981861,  0.066842237]
```

### `Generator::choice`

Generates a random sample from a given sequence.
```cpp
template <class T>
T choice(const tensor<T, 1> &population);

template <class T, class U>
T choice(const tensor<T, 1> &population, const tensor<U, 1> &weights);
```

Parameters

* `population` A 1-dimensional tensor with the values to sample from.
* `weights` A 1-dimensional tensor with the weights associated to each entry of
`population`. If not provided, the sample assumes a uniform distribution over
all entries of `population`.

Returns

* The generated random sample.

Exceptions

* `std::invalid_argument` Thrown if the population size is 0 or if `population`
and `weights` have different sizes.

<h3><code>Generator::choice</code></h3>

Generates a random sample from a given sequence.
```cpp
template <class T>
tensor<T, 1> choice(const tensor<T, 1> &population, size_t size,
                    bool replace = true, bool shuffle = true);

template <class T, size_t Rank>
tensor<T, Rank> choice(const tensor<T, 1> &population,
                       const shape_t<Rank> &size,
                       bool replace = true, bool shuffle = true);

template <class T, class U>
tensor<T, 1> choice(const tensor<T, 1> &population, size_t size,
                    const tensor<U, 1> &weights, bool replace = true);

template <class T, size_t Rank, class U>
tensor<T, Rank> choice(const tensor<T, 1> &population,
                       const shape_t<Rank> &size,
                       const tensor<U, 1> &weights, bool replace = true);
```

Parameters

* `population` A 1-dimensional tensor with the values to sample from.
* `size` Output shape.
* `weights` A 1-dimensional tensor with the weights associated to each entry of
`population`. If not provided, the sample assumes a uniform distribution over
all entries of `population`.
* `replace` Whether the sample is with or without replacement. Default is
true, meaning that a value can be selected multiple times.
* `shuffle` Whether the sample is shuffled when sampling without replacement.
Default is true.

Returns

* A tensor with the generated random samples.

Exceptions

* `std::invalid_argument` Thrown if the population size is 0, if `population`
and `weights` have different sizes or if `replace=false` and the sample size
is greater than the population size.
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
    int size;
    bool replace;
    std::cin >> size >> std::boolalpha >> replace;
    // Fill population with alpha-numeric characters.
    const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz"
                            "0123456789";
    np::vector_view<const char> population(alphanum, 62);
    // Sample characters with equal probabilities.
    np::vector<char> sample = rng.choice(population, size, replace);
    std::string str(sample.begin(), sample.end());
    std::cout << str << "\n";
    return 0;
}
```

Input

```
24
true
```

Possible output

```
AIuchNCqq5XgzCDgpAXEZqk5
```

Input

```
24
false
```

Possible output

```
XclL5aiHby0pKRFn7IdsJ4qZ
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
    np::vector<std::string> population;
    np::vector<double> weights;
    int size;
    bool replace;
    std::cin >> population >> weights >> size >> std::boolalpha >> replace;
    // Sample values with the given weights. Each weight is proportional to the
    // probability of that element to be chosen.
    np::vector<std::string> sample = rng.choice(population, size, weights,
                                                replace);
    std::cout << sample << "\n";
    return 0;
}
```

Input

```
["red", "orange", "yellow", "green", "blue", "purple", "brown", "white",
 "gray", "black"]
[100, 89, 80, 71, 64, 57, 51, 45, 41, 36]
7
true
```

Possible output

```
[   "red",  "green", "orange", "purple",   "gray",  "green",    "red"]
```

Input

```
["red", "orange", "yellow", "green", "blue", "purple", "brown", "white",
 "gray", "black"]
[100, 89, 80, 71, 64, 57, 51, 45, 41, 36]
7
false
```

Possible output

```
[   "red",   "blue", "yellow",  "brown",  "black",  "green", "orange"]
```
