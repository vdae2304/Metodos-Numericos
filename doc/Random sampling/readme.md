# Random sampling

Defined in header [`numcpp/random.h`](/include/numcpp/random.h)

- [Random sampling](#random-sampling)
  - [Classes](#classes)
  - [Sample random data](#sample-random-data)
  - [Permutations](#permutations)
  - [Continuous distributions](#continuous-distributions)
  - [Discrete distributions](#discrete-distributions)

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

## [Continuous distributions](Continuous%20distributions.md)

| Function                                                                               | Description                                                  |
| -------------------------------------------------------------------------------------- | ------------------------------------------------------------ |
| [`Generator::beta`](Continuous%20distributions.md#generatorbeta)                       | Draw samples from a Beta distribution.                       |
| [`Generator::cauchy`](Continuous%20distributions.md#generatorcauchy)                   | Draw samples from a Cauchy distribution.                     |
| [`Generator::chisquare`](Continuous%20distributions.md#generatorchisquare)             | Draw samples from a chi-squared distribution.                |
| [`Generator::exponential`](Continuous%20distributions.md#generatorexponential)         | Draw samples from an exponential distribution.               |
| [`Generator::fisher_f`](Continuous%20distributions.md#generatorfisher_f)               | Draw samples from a Fisher F-distribution.                   |
| [`Generator::gamma`](Continuous%20distributions.md#generatorgamma)                     | Draw samples from a Gamma distribution.                      |
| [`Generator::gumbel`](Continuous%20distributions.md#generatorgumbel)                   | Draw samples from a Gumbel distribution.                     |
| [`Generator::laplace`](Continuous%20distributions.md#generatorlaplace)                 | Draw samples from a Laplace distribution.                    |
| [`Generator::logistic`](Continuous%20distributions.md#generatorlogistic)               | Draw samples from a logistic distribution.                   |
| [`Generator::lognormal`](Continuous%20distributions.md#generatorlognormal)             | Draw samples from a log-normal distribution.                 |
| [`Generator::normal`](Continuous%20distributions.md#generatornormal)                   | Draw samples from a normal distribution.                     |
| [`Generator::pareto`](Continuous%20distributions.md#generatorpareto)                   | Draw samples from a Pareto distribution.                     |
| [`Generator::rayleigh`](Continuous%20distributions.md#generatorrayleigh)               | Draw samples from a Rayleigh distribution.                   |
| [`Generator::standard_normal`](Continuous%20distributions.md#generatorstandard_normal) | Draw samples from a standard normal distribution.            |
| [`Generator::student_t`](Continuous%20distributions.md#generatorstudent_t)             | Draw samples from a Student's t distribution.                |
| [`Generator::uniform`](Continuous%20distributions.md#generatoruniform)                 | Draw samples from an uniform distribution.                   |
| [`Generator::wald`](Continuous%20distributions.md#generatorwald)                       | Draw samples from a Wald, or inverse Gaussian, distribution. |
| [`Generator::weibull`](Continuous%20distributions.md#generatorweibull)                 | Draw samples from a Weibull distribution.                    |

## [Discrete distributions](Discrete%20distributions.md)

| Function                                                                                 | Description                                         |
| ---------------------------------------------------------------------------------------- | --------------------------------------------------- |
| [`Generator::bernoulli`](Discrete%20distributions.md#generatorbernoulli)                 | Draw samples from a Bernoulli distribution.         |
| [`Generator::binomial`](Discrete%20distributions.md#generatorbinomial)                   | Draw samples from a binomial distribution.          |
| [`Generator::geometric`](Discrete%20distributions.md#generatorgeometric)                 | Draw samples from a geometric distribution.         |
| [`Generator::negative_binomial`](Discrete%20distributions.md#generatornegative_binomial) | Draw samples from a negative binomial distribution. |
| [`Generator::poisson`](Discrete%20distributions.md#generatorpoisson)                     | Draw samples from a Poisson distribution.           |
