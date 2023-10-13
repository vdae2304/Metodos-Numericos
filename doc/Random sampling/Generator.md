# Generator

Defined in header [`numcpp/random.h`](/include/numcpp/random.h)

- [Generator](#generator)
  - [Aliases](#aliases)
  - [Template parameters](#template-parameters)
  - [Constructors](#constructors)
    - [Default constructor](#default-constructor)
    - [Seed constructor](#seed-constructor)
  - [Destructor](#destructor)
  - [Public methods](#public-methods)
    - [`Generator::seed`](#generatorseed)

```cpp
template <class bit_generator> class Generator
```
A Generator is a wrapper class of a random number engine that transforms
sequence of random bits into a sequence of numbers that follow a specific
probability distribution (such as uniform, normal or binomial).

## Aliases

```cpp
using default_rng = Generator<std::default_random_engine>;
```

## Template parameters

* `bit_generator` Random number engine to use. Must meet the requirements
listed by [Uniform Random Bit Generator](https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator)
and [Random Number Engine](https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine)
Common examples are `std::default_random_engine` and `std::random_device`.

## Constructors

### Default constructor

Constructs a `Generator` with default seed.
```cpp
Generator();
```

### Seed constructor

Constructs a `Generator` from a seed value or a seed sequence.
```cpp
template <class Sseq>
Generator(Sseq &s);
```

Parameters

* `s` Seed value or seed sequence to use.

## Destructor

```cpp
~Generator() = default;
```

## Public methods

### `Generator::seed`

Reinitializes the internal state of the random-number engine using a new seed value.
```cpp
template <class Sseq>
void seed(Sseq &s);
```

Parameters

* `s` Seed value or seed sequence to use.

Returns

* None
