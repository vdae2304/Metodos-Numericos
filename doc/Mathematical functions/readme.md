# Mathematical functions

Defined in header [`numcpp/math.h`](/include/numcpp/math.h)

- [Mathematical functions](#mathematical-functions)
  - [Constants](#constants)
  - [Basic functions](#basic-functions)
  - [Trigonometric functions](#trigonometric-functions)
  - [Hyperbolic functions](#hyperbolic-functions)
  - [Exponential and logarithmic functions](#exponential-and-logarithmic-functions)
  - [Power functions](#power-functions)
  - [Rounding](#rounding)
  - [Floating-point manipulation functions](#floating-point-manipulation-functions)
  - [Integer-valued functions](#integer-valued-functions)
  - [Complex numbers](#complex-numbers)
  - [Classification functions](#classification-functions)

## [Constants](Constants.md)

## [Basic functions](Basic%20functions.md)

| Function                            | Description                                                      |
| ----------------------------------- | ---------------------------------------------------------------- |
| [`abs`](Basic%20functions.md#abs)   | Return the absolute value, element-wise.                         |
| [`fmod`](Basic%20functions.md#fmod) | Return the floating-point remainder of $x/y$, element-wise.      |
| [`modf`](Basic%20functions.md#modf) | Descompose $x$ into integral and fractional parts, element-wise. |
| [`fmax`](Basic%20functions.md#fmax) | Return the maximum value, element-wise.                          |
| [`fmin`](Basic%20functions.md#fmin) | Return the minimum value, element-wise.                          |

## [Trigonometric functions](Trigonometric%20functions.md)

| Function                                          | Description                                                                                |
| ------------------------------------------------- | ------------------------------------------------------------------------------------------ |
| [`cos`](Trigonometric%20functions.md#cos)         | Return the cosine, element-wise.                                                           |
| [`sin`](Trigonometric%20functions.md#sin)         | Return the sine, element-wise.                                                             |
| [`tan`](Trigonometric%20functions.md#tan)         | Return the tangent, element-wise.                                                          |
| [`hypot`](Trigonometric%20functions.md#hypot)     | Return the hypotenuse of a right-angled triangle whose legs are $x$ and $y$, element-wise. |
| [`acos`](Trigonometric%20functions.md#acos)       | Return the principal value of the arc cosine, element-wise.                                |
| [`asin`](Trigonometric%20functions.md#asin)       | Return the principal value of the arc sine, element-wise.                                  |
| [`atan`](Trigonometric%20functions.md#atan)       | Return the principal value of the arc tangent, element-wise.                               |
| [`atan2`](Trigonometric%20functions.md#atan2)     | Return the principal value of the arc tangent of $y/x$, element-wise.                      |
| [`degrees`](Trigonometric%20functions.md#degrees) | Convert angles from radians to degrees, element-wise.                                      |
| [`radians`](Trigonometric%20functions.md#radians) | Convert angles from degrees to radians, element-wise.                                      |

## [Hyperbolic functions](Hyperbolic%20functions.md)

| Function                                   | Description                                          |
| ------------------------------------------ | ---------------------------------------------------- |
| [`cosh`](Hyperbolic%20functions.md#cosh)   | Return the hyperbolic cosine, element-wise.          |
| [`sinh`](Hyperbolic%20functions.md#sinh)   | Return the hyperbolic sine, element-wise.            |
| [`tanh`](Hyperbolic%20functions.md#tanh)   | Return the hyperbolic tangent, element-wise.         |
| [`acosh`](Hyperbolic%20functions.md#acosh) | Return the inverse hyperbolic cosine, element-wise.  |
| [`asinh`](Hyperbolic%20functions.md#asinh) | Return the inverse hyperbolic sine, element-wise.    |
| [`atanh`](Hyperbolic%20functions.md#atanh) | Return the inverse hyperbolic tangent, element-wise. |

## [Exponential and logarithmic functions](Exponential%20and%20logarithmic%20functions.md)

| Function                                                        | Description                                                            |
| --------------------------------------------------------------- | ---------------------------------------------------------------------- |
| [`exp`](Exponential%20and%20logarithmic%20functions.md#exp)     | Return the base- $e$ exponential, element-wise.                        |
| [`log`](Exponential%20and%20logarithmic%20functions.md#log)     | Return the natural logarithm, element-wise.                            |
| [`log10`](Exponential%20and%20logarithmic%20functions.md#log10) | Return the common (base-10) logarithm, element-wise.                   |
| [`exp2`](Exponential%20and%20logarithmic%20functions.md#exp2)   | Return the base-2 exponential, element-wise.                           |
| [`log2`](Exponential%20and%20logarithmic%20functions.md#log2)   | Return the binary (base-2) logarithm, element-wise.                    |
| [`expm1`](Exponential%20and%20logarithmic%20functions.md#expm1) | Return $e$ raised to the power $x$ minus one: $e^x - 1$, element-wise. |
| [`log1p`](Exponential%20and%20logarithmic%20functions.md#log1p) | Return the natural logarithm of one plus $x$, element-wise.            |

## [Power functions](Power%20functions.md)

| Function                            | Description                                       |
| ----------------------------------- | ------------------------------------------------- |
| [`pow`](Power%20functions.md#pow)   | Return $x$ raised to the power $y$, element-wise. |
| [`sqrt`](Power%20functions.md#sqrt) | Return the square root, element-wise.             |
| [`cbrt`](Power%20functions.md#cbrt) | Return the cubic root, element-wise.              |

## [Rounding](Rounding.md)

| Function                     | Description                                                     |
| ---------------------------- | --------------------------------------------------------------- |
| [`ceil`](Rounding.md#ceil)   | Rounds $x$ upward, element-wise.                                |
| [`floor`](Rounding.md#floor) | Rounds $x$ downward, element-wise.                              |
| [`trunc`](Rounding.md#trunc) | Rounds $x$ toward zero, element-wise.                           |
| [`round`](Rounding.md#round) | Return the integral value that is nearest to $x$, element-wise. |

## [Floating-point manipulation functions](Floating-point%20manipulation%20functions.md)

| Function                                                              | Description                                                                            |
| --------------------------------------------------------------------- | -------------------------------------------------------------------------------------- |
| [`frexp`](Floating-point%20manipulation%20functions.md#frexp)         | Descompose $x$ into its binary significand and an integral power of two, element-wise. |
| [`ldexp`](Floating-point%20manipulation%20functions.md#ldexp)         | Return the result of multiplying $x$ by 2 raised to the power of `exp`, element-wise.  |
| [`copysign`](Floating-point%20manipulation%20functions.md#copysign)   | Return a value with the magnitude of $x$ and the sign of $y$, element-wise.            |
| [`nextafter`](Floating-point%20manipulation%20functions.md#nextafter) | Return the next representable value after $x$ in the direction of $y$, element-wise.   |

## [Integer-valued functions](Integer-valued%20functions.md)

| Function                                   | Description                                       |
| ------------------------------------------ | ------------------------------------------------- |
| [`gcd`](Integer-valued%20functions.md#gcd) | Return the greatest common divisor, element-wise. |
| [`lcm`](Integer-valued%20functions.md#lcm) | Return the least common multiple, element-wise.   |

## [Complex numbers](Complex%20numbers.md)

| Function                            | Description                                                            |
| ----------------------------------- | ---------------------------------------------------------------------- |
| [`real`](Complex%20numbers.md#real) | Return the real part, element-wise.                                    |
| [`imag`](Complex%20numbers.md#imag) | Return the imaginary part, element-wise.                               |
| [`conj`](Complex%20numbers.md#conj) | Return the complex conjugate, element-wise.                            |
| [`abs`](Complex%20numbers.md#abs)   | Return the absolute value, element-wise.                               |
| [`arg`](Complex%20numbers.md#arg)   | Return the phase angle (in radians) of a complex number, element-wise. |

## [Classification functions](Classification%20functions.md)

| Function                                            | Description                                                                    |
| --------------------------------------------------- | ------------------------------------------------------------------------------ |
| [`isfinite`](Clasification%20functions.md#isfinite) | Return whether $x$ is a finite value (neither infinite nor NaN), element-wise. |
| [`isinf`](Clasification%20functions.md#isinf)       | Return whether $x$ is an infinity value, element-wise.                         |
| [`isnan`](Clasification%20functions.md#isnan)       | Return whether $x$ is a NaN (Not a Number) value, element-wise.                |
| [`signbit`](Clasification%20functions.md#signbit)   | Return whether the sign of $x$ is negative, element-wise.                      |
