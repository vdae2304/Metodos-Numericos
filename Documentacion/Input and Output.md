# Input and Output

Defined in `numcpp/io.h`

- [Input and Output](#input-and-output)
  - [Printoptions](#printoptions)
  - [Binary data](#binary-data)
    - [`load`](#load)
    - [`save`](#save)
  - [Text files](#text-files)
    - [`loadtxt`](#loadtxt)
    - [`savetxt`](#savetxt)
  - [Input/output streams](#inputoutput-streams)
    - [`operator>>(shape_t)`](#operatorshape_t)
    - [`operator<<(shape_t)`](#operatorshape_t-1)
    - [`operator>>(tensor)`](#operatortensor)
    - [`operator<<(tensor)`](#operatortensor-1)

## Printoptions

```cpp
namespace printoptions {
    size_t precision = 8;
    constexpr size_t fullprecision = /* implementation defined */;

    size_t threshold = 1000;
    constexpr size_t nothreshold = /* implementation defined */;

    size_t edgeitems = 3;

    size_t linewidth = 80;

    enum floatmode_t { defaultfloat, fixed, scientific };
    floatmode_t floatmode = defaultfloat;

    enum complexmode_t { defaultcomplex, arithmetic };
    complexmode_t complexmode = defaultcomplex;
}
```

Context manager for setting print options.

* `precision` Number of digits of precision for floating point output. To
always use the maximum precission of each floating point type, set to
`fullprecision`. Default is 8.
* `threshold` Total number of array elements which trigger summarization rather
than full representation. To always use the full representation without
summarization, set to `nothreshold`. Default is 1000.
* `edgeitems` Number of array items in summary at beginning and end of each
dimension. Default is 3.
* `linewidth` Number of characters per line for the purpose of inserting line
breaks. Default is 80.
* `floatmode` Controls the interpretation of the precision option for
floating-point types. Can take one of the following values:
    - `defaultfloat` (Default) Write floating-point values in default notation.
    - `fixed` Write floating-point values in fixed-point notation.
    - `scientific` Write floating-point values in scientific notation.
* `complexmode` Controls the input and output format of complex types. Can take
one of the following values:
    - `defaultcomplex` (Default) Read and write complex values in default
    notation, i.e., as a pair `(real,imag)`.
    - `arithmetic` Read and write complex values in arithmetic notation, i.e.,
    as `real + imag i`.

Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    np::default_rng rng;
    np::array<double> arr = rng.random<double>(10);
    std::cout << "Default printoptions:\n" << arr << "\n";
    precision = fullprecision;
    threshold = 8;
    floatmode = scientific;
    std::cout << "Custom printoptions:\n" << arr << "\n";
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

Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "numcpp/random.h"
namespace np = numcpp;
int main() {
    using namespace np::printoptions;
    np::default_rng rng;
    np::array<std::complex<double>> arr(10);
    arr.real(rng.random<double>(10));
    arr.imag(rng.random<double>(10));
    std::cout << "Default printoptions:\n" << arr << "\n";
    complexmode = arithmetic;
    std::cout << "Custom printoptions:\n" << arr << "\n";
    return 0;
}
```

Possible output

```
Default printoptions:
[  (0.13153779,0.68677271),   (0.45865013,0.93043649),
   (0.21895919,0.52692878),   (0.67886472,0.65391896),
    (0.9346929,0.70119059),   (0.51941637,0.76219804),
  (0.03457211,0.047464513),   (0.52970019,0.32823423),
 (0.0076981861,0.75641049),  (0.066842237,0.36533867)]
Custom printoptions:
[  0.13153779+0.68677271i,   0.45865013+0.93043649i,   0.21895919+0.52692878i,
   0.67886472+0.65391896i,    0.9346929+0.70119059i,   0.51941637+0.76219804i,
  0.03457211+0.047464513i,   0.52970019+0.32823423i, 0.0076981861+0.75641049i,
  0.066842237+0.36533867i]
```

## Binary data

### `load`

Load tensor contents from a binary file in NumPy *.npy* format.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> load(const std::string &filename);
```

Template parameters

* `T` Type of the elements contained in the tensor. Must be an arithmetic type
(either integer or floating-point) or a complex type.
* `Rank` Dimension of the tensor.

Parameters

* `filename` A string representing the name of the file to load.

Returns

* A new tensor with its contents loaded from the file.

Exceptions

* `std::ios_base::failure` Thrown if the input file doesn't exist or cannot be
read.
* `std::invalid_argument` Thrown if `T` and `Rank` doesn't match the data type
and dimension stored by the input file. Be aware that some data types might
have platform dependent definitions.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

Python

```py
# Generate a .npy file from Python.
import numpy as np
a = np.array([1, -9, 5, 10, -2, 7, 8, 12, 0, 3], dtype=int)
np.save("example.npy", a)
```

C++

```cpp
// Load the .npy file from C++.
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;

template <class T, size_t Rank>
void load_and_print() {
    try {
        np::tensor<T, Rank> data = np::load<T, Rank>("example.npy");
        std::cout << data << "\n";
    }
    catch (std::invalid_argument &error) {
        std::cout << "Exception caught: " << error.what() << "\n";
    }
}

int main() {
    // Try to read as np::array<double>
    load_and_print<double, 1>();
    // Try to read as np::matrix<int>
    load_and_print<int, 2>();
    // Try to read as np::array<int>
    load_and_print<int, 1>();
    return 0;
}
```

Possible output

```
Exception caught: input file dtype '<i4' doesn't match output dtype '<f8'
Exception caught: input file shape (10,) is not a valid shape of rank 2
[ 1, -9,  5, 10, -2,  7,  8, 12,  0,  3]
```

### `save`

Save tensor contents to a binary file in NumPy *.npy* format.
```cpp
template <class T, size_t Rank>
void save(const std::string &filename, const tensor<T, Rank> &data);
```

Parameters

* `filename` A string representing the name of the file destination.
* `data` Tensor data to be saved. Only arithmetic types (either integer or
floating-point) and complex types are supported.

Returns

* None

Exceptions

* `std::ios_base::failure` Thrown if the output file cannot be written.

Example

C++

```cpp
// Generate a .npy file from C++.
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> arr{1, -9, 5, 10, -2, 7, 8, 12, 0, 3};
    np::save("example.npy", arr);
    return 0;
}
```

Python

```py
# Load the .npy file from Python.
import numpy as np
a = np.load("example.npy")
print(a)
print(a.dtype)
```

Possible output

```
[ 1 -9  5 10 -2  7  8 12  0  3]
int32
```

## Text files

### `loadtxt`

Load tensor data from a text file.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> loadtxt(
    const std::string &filename, char delimiter = ' ', char newline = '\n',
    size_t skiprows = 0, size_t max_rows = SIZE_MAX,
    std::initializer_list<size_t> usecols = {}
);
```

Template parameters

* `T` Type of the elements contained in the tensor.
* `Rank` Dimension of the tensor. Must be 1 or 2.

Parameters

* `filename` A string representing the name of the file to load.
* `delimiter` Character used to separate values. Defaults to whitespace.
* `newline` Character used to separate lines. Defaults to end of line.
* `skiprows` Skip the first `skiprows` lines. Default is 0.
* `max_rows` Read `max_rows` lines of content after `skiprows` lines. The
default is to read all the lines.
* `usecols` An `initializer_list` indicating which columns to read, with 0
being the first. If not provided, read all the columns.

Returns

* A new tensor with its contents loaded from the file.

Exceptions

* `std::ios_base::failure` Thrown if the input file doesn't exist or cannot be
read.
* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::matrix<std::string> raw;
    np::matrix<int> data;
    np::array<int> x;

    raw = np::loadtxt<std::string, 2>("example.csv",
                                      /*delimiter=*/',', /*newline=*/'\n');
    std::cout << "Unformatted data:\n";
    std::cout << raw << "\n";

    data = np::loadtxt<int, 2>("example.csv",
                               /*delimiter=*/',', /*newline=*/'\n',
                               /*skiprows=*/1);
    std::cout << "Read all file:\n";
    std::cout << data << "\n";

    data = np::loadtxt<int, 2>("example.csv",
                               /*delimiter=*/',', /*newline=*/'\n',
                               /*skiprows=*/1, /*max_rows=*/100,
                               /*usecols=*/{0, 1});
    std::cout << "Read first two columns:\n";
    std::cout << data << "\n";

    x = np::loadtxt<int, 1>("example.csv",
                            /*delimiter=*/',', /*newline=*/'\n',
                            /*skiprows=*/1, /*max_rows=*/100,
                            /*usecols=*/{2});
    std::cout << "Read third column:\n";
    std::cout << x << "\n";
    return 0;
}
```

*example.csv*

```
x,x**2,x**3
1,1,1
2,4,8
3,9,27
4,16,64
5,25,125
6,36,216
7,49,343
8,64,512
9,81,729
10,100,1000
```

Output

```
Unformatted data:
[[   "x", "x**2", "x**3"],
 [   "1",    "1",    "1"],
 [   "2",    "4",    "8"],
 [   "3",    "9",   "27"],
 [   "4",   "16",   "64"],
 [   "5",   "25",  "125"],
 [   "6",   "36",  "216"],
 [   "7",   "49",  "343"],
 [   "8",   "64",  "512"],
 [   "9",   "81",  "729"],
 [  "10",  "100", "1000"]]
Read all file:
[[   1,    1,    1],
 [   2,    4,    8],
 [   3,    9,   27],
 [   4,   16,   64],
 [   5,   25,  125],
 [   6,   36,  216],
 [   7,   49,  343],
 [   8,   64,  512],
 [   9,   81,  729],
 [  10,  100, 1000]]
Read first two columns:
[[  1,   1],
 [  2,   4],
 [  3,   9],
 [  4,  16],
 [  5,  25],
 [  6,  36],
 [  7,  49],
 [  8,  64],
 [  9,  81],
 [ 10, 100]]
Read third column:
[   1,    8,   27,   64,  125,  216,  343,  512,  729, 1000]
```

### `savetxt`

Save a tensor to a text file.
```cpp
template <class T, size_t Rank>
void savetxt(
    const std::string &filename, const tensor<T, Rank> &data,
    char delimiter = ' ', char newline = '\n',
    const std::string &header = "", const std::string &footer = ""
);
```

Parameters

* `filename` A string representing the name of the file destination.
* `data` A 1-dimensional or 2-dimensional tensor with the data to be saved to a
text file.
* `delimiter` Character used to separate values. Defaults to whitespace.
* `newline` Character used to separate lines. Defaults to end of line.
* `header` String that will be written at the beginning of the file.
* `footer` String that will be written at the end of the file.

Returns

* None

Exceptions

* `std::ios_base::failure` Thrown if the output file cannot be written.

Example

```cpp
#include <iostream>
#include "numcpp.h"
namespace np = numcpp;
int main() {
    np::array<int> x = np::arange(1, 11);
    np::matrix<int> data = np::stack(/*axis=*/1, x, x*x, x*x*x);
    np::savetxt("example1d.csv", x);
    np::savetxt("example2d.csv", data,
                /*delimiter=*/',', /*newline=*/'\n');
    np::savetxt("example2dheader.csv", data,
                /*delimiter=*/',', /*newline=*/'\n',
                /*header=*/"x,x**2,x**3");
    return 0;
}
```

*example1d.csv*

```
1
2
3
4
5
6
7
8
9
10

```

*example2d.csv*

```
1,1,1
2,4,8
3,9,27
4,16,64
5,25,125
6,36,216
7,49,343
8,64,512
9,81,729
10,100,1000

```

*example2dheader.csv*

```
x,x**2,x**3
1,1,1
2,4,8
3,9,27
4,16,64
5,25,125
6,36,216
7,49,343
8,64,512
9,81,729
10,100,1000

```

## Input/output streams

### `operator>>(shape_t)`

Overloads input stream for `shape_t` objects.
```cpp
template <class charT, class traits, size_t Rank>
std::basic_istream<charT, traits>& operator>>(
    std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape
);

template <class charT, class traits>
std::basic_istream<charT, traits>& operator>>(
    std::basic_istream<charT, traits> &istr, shape_t<1> &shape
);
```

For 1-dimensional shapes, the supported formats are
* `(size,)`
* `size`

For n-dimensional shapes, n > 1, the supported formats are
* `(size_1, size_2, ..., size_n)`

If an error occurs, calls `istr.setstate(std::ios_base::failbit)`.

Parameters

* `istr` Input stream object.
* `shape` Shape to be extracted from the input stream.

Returns

* `istr`

### `operator<<(shape_t)`

Overloads output stream for `shape_t` objects.
```cpp
template <class charT, class traits, size_t Rank>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits> &ostr, const shape_t<Rank> &shape
);

template <class charT, class traits>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits> &ostr, const shape_t<1> &shape
);
```

For 1-dimensional shapes, the format used is
* `(size,)`

For n-dimensional shapes, n > 1, the format used is
* `(size_1, size_2, ..., size_n)`

Parameters

* `ostr` Output stream object.
* `shape` Shape to be inserted into the output stream.

Returns

* `ostr`

### `operator>>(tensor)`

Extracts a tensor from an input stream.
```cpp
template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits>& operator>>(
    std::basic_istream<charT, traits> &istr, tensor<T, Rank> &arg
);
```

The format for input extraction is a list of values separated by commas and
delimited by brackets:
```
    [arg[0], arg[1], arg[2], ... , arg[n - 1]]
```

Each value in the list might be a list as well. However, the number of nested
lists must be equal to the dimension of the tensor to extract. If extraction
fails, an empty tensor is written and failbit flag is set.

Parameters

* `istr` Input stream object.
* `arg` Tensor to be extracted from the input stream.

Returns

* `istr`

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an
exception.

### `operator<<(tensor)`

Inserts a tensor into the output stream.
```cpp
template <class charT, class traits, class T, size_t Rank>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits> &ostr, const tensor<T, Rank> &arg
);
```

The format for output insertion is the same as input extraction, i.e., a list
of values separated by commas and delimited by brackets.

Parameters

* `ostr` Output stream object.
* `arg` A tensor-like object to be inserted into the output stream.

Returns

* `ostr`
