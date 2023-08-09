# Routines

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Tensor creation routines](#tensor-creation-routines)
  - [Numerical ranges](#numerical-ranges)
  - [Building matrices](#building-matrices)

## [Tensor creation routines](Tensor%20creation%20routines.md)

| Function                                                   | Description                                                                       |
| ---------------------------------------------------------- | --------------------------------------------------------------------------------- |
| [`empty`](Tensor%20creation%20routines.md#empty)           | Return a new tensor of given                                                      |
| [`empty_like`](Tensor%20creation%20routines.md#empty_like) | Return a new tensor with the same shape and type as a given tensor.               |
| [`zeros`](Tensor%20creation%20routines.md#zeros)           | Return a tensor of given shape filled with zeros.                                 |
| [`zeros_like`](Tensor%20creation%20routines.md#zeros_like) | Return a tensor of zeros with the same shape and type as a given tensor.          |
| [`ones`](Tensor%20creation%20routines.md#ones)             | Return a tensor of given shape filled with ones.                                  |
| [`ones_like`](Tensor%20creation%20routines.md#ones_like)   | Return a tensor of ones with the same shape and type as a given tensor.           |
| [`full`](Tensor%20creation%20routines.md#full)             | Return a tensor of given shape filled with `val`.                                 |
| [`full_like`](Tensor%20creation%20routines.md#full_like)   | Return a tensor filled with `val` with the same shape and type as a given tensor. |

## [Numerical ranges](Numerical%20ranges.md)

| Function                                       | Description                                                            |
| ---------------------------------------------- | ---------------------------------------------------------------------- |
| [`arange`](Numerical%20ranges.md#arange)       | Return evenly spaced values within a given interval.                   |
| [`linspace`](Numerical%20ranges.md#linspace)   | Return evenly spaced numbers over a specified interval.                |
| [`logspace`](Numerical%20ranges.md#logspace)   | Return numbers spaced evenly on a log scale.                           |
| [`geomspace`](Numerical%20ranges.md#geomspace) | Return numbers spaced evenly on a log scale (a geometric progression). |

## [Building matrices](Building%20matrices.md)

| Function                                  | Description                                                    |
| ----------------------------------------- | -------------------------------------------------------------- |
| [`eye`](Building%20matrices.md#eye)       | Return a matrix with ones on the diagonal and zeros elsewhere. |
| [`diag`](Building%20matrices.md#diag)     | Extract a diagonal or construct a diagonal matrix.             |
| [`tril`](Building%20matrices.md#tril)     | Return the lower triangle of a matrix.                         |
| [`triu`](Building%20matrices.md#triu)     | Return the upper triangle of a matrix.                         |
| [`vander`](Building%20matrices.md#vander) | Generate a Vandermonde matrix.                                 |
