# Functional programming

Defined in header [`numcpp/functional.h`](/include/numcpp/functional.h)

- [Functional programming](#functional-programming)
  - [Element-wise functions](#element-wise-functions)
  - [Reductions](#reductions)
  - [Accumulations](#accumulations)
  - [Outer product](#outer-product)
  - [Vectorized functions](#vectorized-functions)

## [Element-wise functions](Element-wise%20functions.md)

| Function                                       | Description                           |
| ---------------------------------------------- | ------------------------------------- |
| [`apply`](Element-wise%20functions.md#apply)   | Apply a function element-wise.        |
| [`apply2`](Element-wise%20functions.md#apply2) | Apply a binary function element-wise. |

## [Reductions](Reductions.md)

| Function                         | Description                                                                        |
| -------------------------------- | ---------------------------------------------------------------------------------- |
| [`reduce`](Reductions.md#reduce) | Reduce the tensor's dimension by cumulatively applying a function to all elements. |

## [Accumulations](Accumulations.md)

| Function                                    | Description                                                 |
| ------------------------------------------- | ----------------------------------------------------------- |
| [`accumulate`](Accumulations.md#accumulate) | Accumulate the result of applying a function along an axis. |

## [Outer product](Outer%20product.md)

| Function                            | Description                                                                    |
| ----------------------------------- | ------------------------------------------------------------------------------ |
| [`outer`](Outer%20product.md#outer) | Apply a function to all pairs $(a_i, b_j)$ with $a_i$ in $a$ and $b_j$ in $b$. |

## [Vectorized functions](Vectorized%20functions.md)

| Function                                                                   | Description                              |
| -------------------------------------------------------------------------- | ---------------------------------------- |
| [`vectorize`](Vectorized%20functions.md#vectorize)                         | Vectorize a function.                    |
| [`<vectorized>::target`](Vectorized%20functions.md#vectorizedtarget)       | Return a pointer to the stored function. |
| [`<vectorized>::operator()`](Vectorized%20functions.md#vectorizedoperator) | Invokes the function.                    |
