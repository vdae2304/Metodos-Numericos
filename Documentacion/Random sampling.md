# Random sampling

Defined in `numcpp/random.h`

- [Random sampling](#random-sampling)
  - [Generators](#generators)
    - [Template parameters](#template-parameters)
    - [Constructors](#constructors)
    - [Destructor](#destructor)
    - [Seeding](#seeding)
  - [Sample random data](#sample-random-data)
    - [`Generator::integers`](#generatorintegers)
    - [`Generator::random`](#generatorrandom)
    - [`Generator::choice`](#generatorchoice)
  - [Permutations](#permutations)
    - [`shuffle`](#shuffle)
    - [`permutation`](#permutation)

## Generators

```cpp
template <class bit_generator> class Generator
```
A Generator is a wrapper class of a random number engine that transforms
sequence of random bits into a sequence of numbers that follow a specific
probability distribution (such as uniform, normal or binomial).

### Template parameters

* `bit_generator` Random number engine to use. Must meet the requirements
listed by [Uniform Random Bit Generator](https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator)
and [Random Number Engine](https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine)
Common examples are `std::default_random_engine` and `std::random_device`.

### Constructors

Default constructor. Constructs a `Generator` with default seed.
```cpp
Generator();
```

Seed constructor. Constructs a `Generator` from a seed value or a seed
sequence.
```cpp
Generator(typename bit_generator::result_type s);

template <class Sseq>
Generator(Sseq &s);
```

Parameters

* `s` Seed value or seed sequence to use.

### Destructor

```cpp
~Generator() = default;
```

### Seeding

Reinitializes the internal state of the random-number engine using a new seed
value.
```cpp
void seed(typename bit_generator::result_type s);

template <class Sseq>
void seed(Sseq &s);
```

Parameters

* `s` Seed value or seed sequence to use.

Returns

* None

## Sample random data

### `Generator::integers`

Return random integers from `low` to `high` (inclusive).
```cpp
template <class T>
T integers(T low, T high);
```

Template parameters

* `T` An integer type.

Parameters

* `low` Lowest integer to be drawn.
* `high` Largest integer to be drawn.

Returns

* A random integer.

<h3><code>Generator::integers</code></h3>

Return a tensor of random integers from `low` to `high` (inclusive).
```cpp
template <class T>
tensor<T, 1> integers(T low, T high, size_t size);

template <class T, size_t Rank>
tensor<T, Rank> integers(T low, T high, const shape_t<Rank> &size);
```

Template parameters

* `T` An integer type.
* `Rank` Dimension of the tensor.

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
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    int low, high, size;
    std::cin >> low >> high >> size;
    std::cout << rng.integers(low, high, size) << "\n";
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
template <class T>
T random();
```

Template parameters

* `T` A floating-point type.

Parameters

* None

Returns

* A random floating point number.

<h3><code>Generator::random</code></h3>

Return a tensor of random floating point numbers in the half-open interval
$[0, 1)$.
```cpp
template <class T>
tensor<T, 1> random(size_t size);

template <class T, size_t Rank>
tensor<T, Rank> random(const shape_t<Rank> &size);
```

Template parameters

* `T` A floating-point type.
* `Rank` Dimension of the tensor.

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
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    int size;
    std::cin >> size;
    std::cout << rng.random<double>(size) << "\n";
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

template <class T, class W>
T choice(const tensor<T, 1> &population, const tensor<W, 1> &weights);
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
tensor<T, 1> choice(
    const tensor<T, 1> &population, size_t size,
    bool replace = true, bool shuffle = true
);
template <class T, size_t Rank>
tensor<T, Rank> choice(
    const tensor<T, 1> &population, const shape_t<Rank> &size,
    bool replace = true, bool shuffle = true
);

template <class T, class W>
tensor<T, 1> choice(
    const tensor<T, 1> &population, size_t size,
    const tensor<W, 1> &weights, bool replace = true
);
template <class T, size_t Rank, class W>
tensor<T, Rank> choice(
    const tensor<T, 1> &population, const shape_t<Rank> &size,
    const tensor<W, 1> &weights, bool replace = true
);
```

Parameters

* `population` A 1-dimensional tensor with the values to sample from.
* `size` Output shape.
* `weights` A 1-dimensional tensor with the weights associated to each entry of
`population`. If not provided, the sample assumes a uniform distribution over
all entries of `population`.
* `replace` Whether the sample is with or without replacement. Default is
`true`, meaning that a value can be selected multiple times.
* `shuffle` Whether the sample is shuffled when sampling without replacement.
Default is `true`.

Returns

* A tensor with the generated random samples.

Exceptions

* `std::invalid_argument` Thrown if the population size is 0, if `population`
and `weights` have different sizes or if `replace = false` and the sample size
is greater than the population size.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    int size;
    bool replace;
    std::cin >> size >> std::boolalpha >> replace;
    // Fill population with alpha-numeric characters.
    char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                      "abcdefghijklmnopqrstuvwxyz"
                      "0123456789";
    np::array_view<char> population(62, alphanum);
    // Sample characters with equal probabilities.
    np::array<char> sample = rng.choice(population, size, replace);
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
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    np::array<std::string> population, sample;
    int size;
    bool replace;
    std::cin >> population >> size >> std::boolalpha >> replace;
    // Generate weights decreasing exponentially.
    double pop_sz = population.size();
    np::array<double> weights = np::exp(-np::arange(pop_sz) / (pop_sz - 1));
    std::cout << "weights:\n" << weights << "\n";
    // Sample values with the given weights. Each weight is proportional to the
    // probability of that element to be chosen.
    sample = rng.choice(population, size, weights, replace);
    std::cout << "sample:\n" << sample << "\n";
    return 0;
}
```

Input

```
["red", "orange", "yellow", "green", "blue", "purple", "brown", "white",
 "gray", "black"]
7
true
```

Possible output

```
weights:
[         1, 0.89483932,  0.8007374, 0.71653131, 0.64118039, 0.57375342,
 0.51341712, 0.45942582, 0.41111229, 0.36787944]
sample:
[   "red",  "green", "orange", "purple",   "gray",  "green",    "red"]
```

Input

```
["red", "orange", "yellow", "green", "blue", "purple", "brown", "white",
 "gray", "black"]
7
false
```

Possible output

```
weights:
[         1, 0.89483932,  0.8007374, 0.71653131, 0.64118039, 0.57375342,
 0.51341712, 0.45942582, 0.41111229, 0.36787944]
sample:
[   "red",   "blue", "yellow",  "brown",  "black",  "green", "orange"]
```

## Permutations

### `shuffle`

Modify a tensor in-place by shuffling its contents.
```cpp
template <class T, size_t Rank>
void shuffle(tensor<T, Rank> &arg, size_t axis = Rank - 1);
```

Parameters

* `arg` The tensor to be shuffled.
* `axis` Axis along which to shuffle. Defaults to `Rank - 1`, which means
shuffle along the last axis.

Returns

* None

Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    np::array<int> a;
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

Output

```
[19, -5, -2, -3,  3, 12,  4, 12, 18,  0]
```

### `permutation`

Return a permuted range.
```cpp
template <class T>
tensor<T, 1> permutation(T n);
```

Parameters

* `n` Randomly permute the range 0, 1, 2, ..., `n` - 1.

Returns

* The permuted range.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    int n;
    std::cin >> n;
    // Returns array<int>
    std::cout << rng.permutation(n) << "\n";
    return 0;
}
```

Input

```
10
```

Output

```
[2, 7, 1, 6, 8, 0, 4, 9, 5, 3]
```

<h3><code>permutation</code></h3>

Randomly permute a tensor.
```cpp
template <class T, size_t Rank>
tensor<T, 1> permutation(const tensor<T, Rank> &arg);

template <class T, size_t Rank>
tensor<T, Rank> permutation(const tensor<T, Rank> &arg, size_t axis);
```

Parameters

* `arg` Make a copy of the tensor and shuffle the elements randomly.
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
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    np::array<int> a;
    std::cin >> a;
    std::cout << rng.permutation(a) << "\n";
    return 0;
}
```

Input

```
[12, -2, 19, 0, 4, 18, -3, -5, 3, 12]
```

Output

```
[19, -5, -2, -3,  3, 12,  4, 12, 18,  0]
```

Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    // Set seed to 0 to replicate results.
    np::default_rng rng(0);
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Axis 0:\n" << rng.permutation(a, 0) << "\n";
    std::cout << "Axis 1:\n" << rng.permutation(a, 1) << "\n";
    return 0;
}
```

Input

```
[[11, 18, -3, 13, -2, 15],
 [10, -4, 15,  1,  0, -5],
 [ 9,  3, -1,  4,  3, -3],
 [-1, -3, 12,  2, 18, -1]]
```

Output

```
Axis 0:
[[-1, -4, 12,  2, 18, 15],
 [10,  3, 15, 13, -2, -5],
 [11, -3, -3,  1,  3, -3],
 [ 9, 18, -1,  4,  0, -1]]
Axis 1:
[[-2, -3, 15, 18, 11, 13],
 [ 1, 15, -4, -5, 10,  0],
 [ 9, -3, -1,  3,  4,  3],
 [ 2, 18, 12, -1, -1, -3]]
```
