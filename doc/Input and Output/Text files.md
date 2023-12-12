# [Input and Output](readme.md)

Defined in header [`numcpp/io.h`](/include/numcpp/io.h)

- [Input and Output](#input-and-output)
  - [Text files](#text-files)
    - [`loadtxt`](#loadtxt)
    - [`savetxt`](#savetxt)

## Text files

### `loadtxt`

Load tensor data from a text file.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> loadtxt(const std::string &filename, char delimiter = ' ',
                        char newline = '\n', size_t skiprows = 0,
                        size_t max_rows = SIZE_MAX,
                        std::initializer_list<size_t> usecols = {});

template <class T, size_t Rank>
tensor<T, Rank> loadtxt(std::istream &file, char delimiter = ' ',
                        char newline = '\n', size_t skiprows = 0,
                        size_t max_rows = SIZE_MAX,
                        std::initializer_list<size_t> usecols = {});
```

Template parameters

* `T` Type of the elements contained in the tensor.
* `Rank` Dimension of the tensor. Must be 1 or 2.

Parameters

* `filename` A string representing the name of the file to load.
* `file` File object to read.
* `delimiter` Character used to separate values. Defaults to whitespace.
* `newline` Character used to separate lines. Defaults to end of line.
* `skiprows` Skip the first `skiprows` lines. Default is 0.
* `max_rows` Read `max_rows` lines of content after `skiprows` lines. The default is to read all the lines.
* `usecols` An `initializer_list` indicating which columns to read, with 0 being the first. If not provided, read all the columns.

Returns

* A new tensor with its contents loaded from the file.

Exceptions

* `std::ios_base::failure` Thrown if the input file doesn't exist or cannot be read.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    std::string filename = "example.csv";
    char delimiter = ',', newline = '\n';

    np::vector<int> ids =
        np::loadtxt<int, 1>(filename, delimiter, newline, 1, 10, {0});
    std::cout << "Ids:\n" << ids << "\n";

    np::vector<std::string> names =
        np::loadtxt<std::string, 1>(filename, delimiter, newline, 1, 10, {1});
    std::cout << "Names:\n" << names << "\n";

    np::matrix<int> grades =
        np::loadtxt<int, 2>(filename, delimiter, newline, 1, 10, {2, 3, 4});
    std::cout << "Grades:\n" << grades << "\n";

    return 0;
}
```

*example.csv*

```
id,name,math,english,programming
7391,Noelle,8,7,9
9481,Mark,7,9,6
7491,Steve,8,7,7
5986,David,6,9,6
9172,Andrew,9,6,8
```

Output

```
Ids:
[7391, 9481, 7491, 5986, 9172]
Names:
["Noelle",   "Mark",  "Steve",  "David", "Andrew"]
Grades:
[[8, 7, 9],
 [7, 9, 6],
 [8, 7, 7],
 [6, 9, 6],
 [9, 6, 8]]
```

### `savetxt`

Save a tensor to a text file.
```cpp
template <class T, size_t Rank>
void savetxt(const std::string &filename, const tensor<T, Rank> &data,
             char delimiter = ' ', char newline = '\n',
             const std::string &header = "", const std::string &footer = "");

template <class T, size_t Rank>
void savetxt(std::ostream &file, const tensor<T, Rank> &data,
             char delimiter = ' ', char newline = '\n',
             const std::string &header = "", const std::string &footer = "");
```

Parameters

* `filename` A string representing the name of the file destination.
* `file` File object to write.
* `data` A 1-dimensional or 2-dimensional tensor with the data to be saved to a text file.
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
#include <numcpp/tensor.h>
#include <numcpp/routines.h>
#include <numcpp/broadcasting.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    std::string filename = "example.csv";
    std::string header = "x,x²,x³";
    char delimiter = ',', newline = '\n';
    np::vector<int> x = np::arange(1, 11);
    np::matrix<int> data = np::stack<1>(x, x*x, x*x*x);
    np::savetxt(filename, data, delimiter, newline, header);
    return 0;
}
```

*example.csv*

```
x,x²,x³
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
