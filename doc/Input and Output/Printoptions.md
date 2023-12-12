# [Input and Output](readme.md)

Defined in header [`numcpp/io.h`](/include/numcpp/io.h)

## Printoptions

```cpp
namespace printoptions {
    size_t precision = 8;
    constexpr size_t fullprecision = /* implementation defined */;

    size_t threshold = 1000;
    constexpr size_t nothreshold = /* implementation defined */;

    size_t edgeitems = 3;

    size_t linewidth = 80;

    bool sign = false;

    enum floatmode_t { defaultfloat, fixed, scientific };
    floatmode_t floatmode = defaultfloat;
}
```

Context manager for setting print options.

* `precision` Number of digits of precision for floating-point output. To always use the maximum precision of each floating-point type, set to `fullprecision`. Default is 8.
* `threshold` Total number of array elements which trigger summarization rather than full representation. To always use the full representation without summarization, set to `nothreshold`. Default is 1000.
* `edgeitems` Number of array items in summary at beginning and end of each dimension. Default is 3.
* `linewidth` Number of characters per line for the purpose of inserting line breaks. Default is 80.
* `sign` Controls printing of the sign of numeric values. If true, always print the sign of positive values. If false, omit the sign of positive values. Default is false.
* `floatmode` Controls the interpretation of the `precision` option for floating-point types. Can take one of the following values:
  * `defaultfloat` (Default) Write floating-point values in default notation.
  * `fixed` Write floating-point values in fixed-point notation.
  * `scientific` Write floating-point values in scientific notation.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    np::default_rng rng;

    np::vector<double> a = rng.random(10);
    std::cout << "Default printoptions:\n" << a << "\n";

    precision = fullprecision;
    threshold = 8;
    floatmode = scientific;
    std::cout << "Custom printoptions:\n" << a << "\n";

    return 0;
}
```

Possible output

```
Default printoptions:
[  0.13153779,   0.45865013,   0.21895919,   0.67886472,    0.9346929, 
   0.51941637,   0.03457211,   0.52970019, 0.0076981861,  0.066842237]
Custom printoptions:
[1.31537787738760653e-01, 4.58650132023219803e-01, 2.18959186212478951e-01, 
 ..., 5.29700193141057207e-01, 7.69818606159917897e-03, 6.68422372625675498e-02]
 ```
