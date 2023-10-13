# Routines

Defined in header [`numcpp/random.h`](/include/numcpp/random.h)

- [Routines](#routines)
  - [Classes](#classes)
  - [Sample random data](#sample-random-data)
  - [Permutations](#permutations)

## Classes

| Class                       | Description                               |
| --------------------------- | ----------------------------------------- |
| [`Generator`](Generator.md) | a wrapper class of a random number engine |

## [Sample random data](Sample%20random%20data.md)

| Function                                                             | Description                                                              |
| -------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| [`Generator::integers`](Sample%20random%20data.md#generatorintegers) | Return random integers from `low` to `high` (inclusive).                 |
| [`Generator::random`](Sample%20random%20data.md#generatorrandom)     | Return random floating point numbers in the half-open interval $[0, 1)$. |
| [`Generator::choice`](Sample%20random%20data.md#generatorchoice)     | Generates a random sample from a given sequence.                         |

## [Permutations](Permutations.md)

| Function                                                         | Description                                         |
| ---------------------------------------------------------------- | --------------------------------------------------- |
| [`Generator::shuffle`](Permutations.md#generatorshuffle)         | Modify a tensor in-place by shuffling its contents. |
| [`Generator::permutation`](Permutations.md#generatorpermutation) | Randomly permute a tensor.                          |
