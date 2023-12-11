# [Input and Output](readme.md)

Defined in header [`numcpp/io.h`](/include/numcpp/io.h)

- [Input and Output](#input-and-output)
  - [Input/output streams](#inputoutput-streams)
    - [`operator>>`](#operator)
    - [`operator<<`](#operator-1)
    - [`to_string`](#to_string)

## Input/output streams

### `operator>>`

Extracts a tensor from the input stream.
```cpp
template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, tensor<T, Rank> &a);
```

The format for input extraction is a list of values separated by commas and delimited by brackets:
```
[a1, a2, a3, ..., an]
```

Each value in the list can be a list as well. However, the depth of nested lists must be equal to the dimension of the tensor to extract. For example, a 1-dimensional tensor is formatted as
```
[a1, a2, a3, ..., an]
```

while a 2-dimensional is formatted as
```
[[a11, a12, a13, ..., a1n],
 [a21, a22, a23, ..., a2n],
 ...,
 [am1, am2, am3, ..., amn]]
```

String values must be delimited by either single quotes (') or double quotes (").

If extraction fails, an empty tensor is written and failbit flag is set.

Parameters

* `istr` Input stream object.
* `a` Tensor to be extracted from the input stream.

Returns

* `istr`

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

### `operator<<`

Inserts a tensor into the output stream.
```cpp
template <class charT, class traits, class T, size_t Rank>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr, const tensor<T, Rank> &a);
```

The format for output insertion is the same as input extraction, i.e., a list of values separated by commas and delimited by brackets.

Floating-point values are formatted according to `precision` and `floatmode` options (see [printoptions](Printoptions.md)).

String values are enclosed with double quotes (").

Parameters

* `ostr` Output stream object.
* `a` A tensor-like object to be inserted into the output stream.

Returns

* `ostr`

### `to_string`

Return a string representation of a tensor.
```cpp
template <class T, size_t Rank>
std::string to_string(const tensor<T, Rank> &a,
                      size_t precision = printoptions::precision,
                      size_t threshold = printoptions::threshold,
                      size_t edgeitems = printoptions::edgeitems,
                      size_t linewidth = printoptions::linewidth,
                      bool sign = printoptions::sign,
                      printoptions::floatmode_t floatmode = printoptions::floatmode,
                      const std::string &prefix = "[",
                      const std::string &suffix = "]",
                      const std::string &separator = ", ");

template <class T, size_t Rank, class Function>
std::string to_string(const tensor<T, Rank> &a,
                      Function formatter,
                      size_t threshold = printoptions::threshold,
                      size_t edgeitems = printoptions::edgeitems,
                      size_t linewidth = printoptions::linewidth,
                      const std::string &prefix = "[",
                      const std::string &suffix = "]",
                      const std::string &separator = ", ");
```

Parameters

* `a` Input tensor.
* `formatter` Custom formatter. A function that accepts an element of type `T` as argument, and returns a value convertible to `string`.
* `precision` Floating point precision. Defaults to [`printoptions::precision`](Printoptions.md)
* `threshold` Total number of array elements which trigger summarization rather than full representation. Defaults to [`printoptions::threshold`](Printoptions.md)
* `edgeitems` Number of array items in summary at beginning and end of each dimension. Defaults to [`printoptions::edgeitems`](Printoptions.md)
* `linewidth` Insert newlines if text is longer than `linewidth`. Defaults to [`printoptions::linewidth`](Printoptions.md)
* `sign` Controls the sign of numeric values. If true, always print the sign of positive values. If false, omit the sign of positive values. Default is [`printoptions::sign`](Printoptions.md).
* `floatmode` Controls the interpretation of the precision option for floating-point types. Defaults to [`printoptions::floatmode`](Printoptions.md)
* `prefix` String to insert at the beginning of each dimension.
* `suffix` String to insert the end of each dimension.
* `separator` String to insert bewtween elements.

Returns

* String representation of the tensor.

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
    np::matrix<double> x = rng.random(np::make_shape(10, 10));

    std::cout << "Default:\n";
    std::cout << np::to_string(x) << "\n";

    std::cout << "\nDelimited by curly braces with summarization:\n";
    std::cout << np::to_string(x, precision, 8, 3, linewidth, sign, floatmode,
                              "{", "}", " ") << "\n";

    return 0;
}
```

Possible output

```
Default:
[[  0.13153779,   0.45865013,   0.21895919,   0.67886472,    0.9346929, 
    0.51941637,   0.03457211,   0.52970019, 0.0076981861,  0.066842237], 
 [  0.68677271,   0.93043649,   0.52692878,   0.65391896,   0.70119059, 
    0.76219804,  0.047464513,   0.32823423,   0.75641049,   0.36533867], 
 [  0.98255029,   0.75335584,  0.072685883,   0.88470713,   0.43641141, 
    0.47773177,   0.27490684,    0.1665072,   0.89765629,  0.060564328], 
 [  0.50452289,   0.31903294,   0.49397669,  0.090732895,  0.073749075, 
    0.38414215,   0.91381744,   0.46444582,  0.050083984,   0.77020455], 
 [  0.12536538,    0.6884553,   0.62954342,     0.725412,   0.88857221, 
    0.30632183,    0.5132737,   0.84598156,   0.84151064,   0.41539462], 
 [  0.46791737,    0.1783277,   0.57165481,  0.033053754,   0.49848012, 
    0.74829265,   0.89073748,   0.84203961,   0.21275151,   0.13042726], 
 [  0.27458815,   0.41429326,   0.70981959,    0.2399108,   0.31753954, 
    0.65205869,   0.68134621,   0.38772534,     0.147533,   0.84557566], 
 [  0.95540883,   0.14815156,   0.40876669,   0.56489868,   0.48851455, 
    0.96109514,    0.1997572,   0.62926916,   0.65125374,     0.803073], 
 [   0.4764318,   0.20325033,    0.9016735,   0.14202103,   0.41031304, 
    0.88564837,   0.16219855,   0.36533903,   0.13510937,   0.45530728], 
 [  0.45230017,    0.9316744,   0.21524838,   0.90892189,   0.86085984, 
    0.50595588,   0.81756148,   0.46224498,    0.6327387,   0.82469739]]

Delimited by curly braces with summarization:
{{  0.13153779   0.45865013   0.21895919 ...   0.52970019 0.0076981861 
   0.066842237} 
 {  0.68677271   0.93043649   0.52692878 ...   0.32823423   0.75641049 
    0.36533867} 
 {  0.98255029   0.75335584  0.072685883 ...    0.1665072   0.89765629 
   0.060564328} 
 ... 
 {  0.95540883   0.14815156   0.40876669 ...   0.62926916   0.65125374 
      0.803073} 
 {   0.4764318   0.20325033    0.9016735 ...   0.36533903   0.13510937 
    0.45530728} 
 {  0.45230017    0.9316744   0.21524838 ...   0.46224498    0.6327387 
    0.82469739}}
```

Example

```cpp
#include <iostream>
#include <complex>
#include <sstream>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
#include <numcpp/io.h>
namespace np = numcpp;

std::string formatter(const std::complex<double> &z) {
    std::ostringstream ostr;
    ostr << z.real() << std::showpos << z.imag() << "i";
    return ostr.str();
}

int main() {
    using namespace np::printoptions;
    np::default_rng rng;
    np::vector<std::complex<double>> z(10);

    z.real(rng.random(z.shape()));
    z.imag(rng.random(z.shape()));

    std::cout << "Default:\n";
    std::cout << np::to_string(z) << "\n";

    std::cout << "Custom formatter:\n";
    std::cout << np::to_string(z, formatter) << "\n";

    return 0;
}
```

Possible output

```
Default:
[  (0.13153779,0.68677271),   (0.45865013,0.93043649),   (0.21895919,0.52692878), 
   (0.67886472,0.65391896),    (0.9346929,0.70119059),   (0.51941637,0.76219804), 
  (0.03457211,0.047464513),   (0.52970019,0.32823423), (0.0076981861,0.75641049), 
  (0.066842237,0.36533867)]
Custom formatter:
[  0.131538+0.686773i,    0.45865+0.930436i,   0.218959+0.526929i, 
   0.678865+0.653919i,   0.934693+0.701191i,   0.519416+0.762198i, 
 0.0345721+0.0474645i,     0.5297+0.328234i,  0.00769819+0.75641i, 
  0.0668422+0.365339i]
```
