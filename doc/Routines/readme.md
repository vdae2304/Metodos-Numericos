# Routines

Defined in header [`numcpp/routines.h`](/include/numcpp/routines.h)

- [Routines](#routines)
  - [Tensor creation routines](#tensor-creation-routines)
  - [Numerical ranges](#numerical-ranges)
  - [Building matrices](#building-matrices)
  - [Maximums and minimums](#maximums-and-minimums)
  - [Sums and products](#sums-and-products)
  - [Logic functions](#logic-functions)
  - [Sorting and searching](#sorting-and-searching)
  - [Basic statistics](#basic-statistics)

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

## [Maximums and minimums](Maximums%20and%20minimums.md)

| Function                                          | Description                                          |
| ------------------------------------------------- | ---------------------------------------------------- |
| [`argmax`](Maximums%20and%20minimums.md#argmax)   | Return the index of the maximum value in the tensor. |
| [`argmin`](Maximums%20and%20minimums.md#argmin)   | Return the index of the minimum value in the tensor. |
| [`amax`](Maximums%20and%20minimums.md#amax)       | Return the maximum value contained in the tensor.    |
| [`amin`](Maximums%20and%20minimums.md#amin)       | Return the minimum value contained in the tensor.    |
| [`maximum`](Maximums%20and%20minimums.md#maximum) | Return the element-wise maximum of two tensors.      |
| [`minimum`](Maximums%20and%20minimums.md#minimum) | Return the element-wise minimum of two tensors.      |
| [`clamp`](Maximums%20and%20minimums.md#clamp)     | Clamp the values in the tensor.                      |
|                                                   |

## [Sums and products](Sums%20and%20products.md)

| Function                                      | Description                                                              |
| --------------------------------------------- | ------------------------------------------------------------------------ |
| [`sum`](Sums%20and%20products.md#sum)         | Return the sum of the tensor elements.                                   |
| [`prod`](Sums%20and%20products.md#prod)       | Return the product of the tensor elements.                               |
| [`cumsum`](Sums%20and%20products.md#cumsum)   | Return the cumulative sum of the tensor elements along a given axis.     |
| [`cumprod`](Sums%20and%20products.md#cumprod) | Return the cumulative product of the tensor elements along a given axis. |

## [Logic functions](Logic%20functions.md)

| Function                                              | Description                                                         |
| ----------------------------------------------------- | ------------------------------------------------------------------- |
| [`all`](Logic%20functions.md#all)                     | Test whether all tensor elements evaluate to true.                  |
| [`any`](Logic%20functions.md#any)                     | Test whether any tensor element evaluate to true.                   |
| [`count_nonzero`](Logic%20functions.md#count_nonzero) | Count the number of non-zero elements in the tensor.                |
| [`isclose`](Logic%20functions.md#isclose)             | Return whether two numbers are equal within a tolerance.            |
| [`allclose`](Logic%20functions.md#allclose)           | Test whether two tensors are element-wise equal within a tolerance. |

## [Sorting and searching](Sorting%20and%20searching.md)

| Function                                                    | Description                                                       |
| ----------------------------------------------------------- | ----------------------------------------------------------------- |
| [`argsort`](Sorting%20and%20searching.md#argsort)           | Return the indices that would sort the tensor.                    |
| [`sort`](Sorting%20and%20searching.md#sort)                 | Return a sorted copy of the flattened tensor.                     |
| [`argpartition`](Sorting%20and%20searching.md#argpartition) | Return the indices that would partition the tensor.               |
| [`partition`](Sorting%20and%20searching.md#partition)       | Return a partitioned copy of the flattened tensor.                |
| [`nonzero`](Sorting%20and%20searching.md#nonzero)           | Return the indices of the elements that are non-zero.             |
| [`where`](Sorting%20and%20searching.md#where)               | Return elements chosen from two tensors depending on `condition`. |

## [Basic statistics](Basic%20statistics.md)

| Function                                     | Description                                                                |
| -------------------------------------------- | -------------------------------------------------------------------------- |
| [`mean`](Basic%20statistics.md#mean)         | Return the average of the tensor elements.                                 |
| [`median`](Basic%20statistics.md#median)     | Return the median of the tensor elements.                                  |
| [`var`](Basic%20statistics.md#var)           | Return the variance of the tensor elements.                                |
| [`stddev`](Basic%20statistics.md#stddev)     | Return the standard deviation of the tensor elements.                      |
| [`quantile`](Basic%20statistics.md#quantile) | Return the q-th quantile of the tensor elements.                           |
| [`cov`](Basic%20statistics.md#cov)           | Return the covariance of two 1-dimensional tensors.                        |
| [`corrcoef`](Basic%20statistics.md#corrcoef) | Return the Pearson's correlation coefficient of two 1-dimensional tensors. |
