# [Input and Output](readme.md)

Defined in header [`numcpp/io.h`](/include/numcpp/io.h)

- [Input and Output](#input-and-output)
  - [Binary data](#binary-data)
    - [`load`](#load)
    - [`save`](#save)

## Binary data

### `load`

Load tensor contents from a binary file in NumPy `.npy` format.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> load(const std::string &filename);

template <class T, size_t Rank>
tensor<T, Rank> load(std::istream &file);
```

Template parameters

* `T` Type of the elements contained in the tensor. Must be an arithmetic type (either integer or floating-point) or a complex type.
* `Rank` Dimension of the tensor.

Parameters

* `filename` A string representing the name of the file to load.
* `file` File object to read.

Returns

* A new tensor with its contents loaded from the file.

Exceptions

* `std::ios_base::failure` Thrown if the input file doesn't exist or cannot be read.
* `std::invalid_argument` Thrown if either `T` or `Rank` doesn't match the data type and dimension stored in the input file. Be aware that some data types might have platform dependent definitions.
* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

Python

```py
# Generate a .npy file from Python.
import numpy as np
a = np.array([1, -9, 5, 10, -2, 7, 8, 12, 0, 3], dtype=np.int32)
np.save("example.npy", a)
```

C++

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;

template <class T, size_t Rank>
void try_load() {
    try {
        np::tensor<T, Rank> data = np::load<T, Rank>("example.npy");
        std::cout << data << "\n";
    } catch (std::invalid_argument &error) {
        std::cout << "Exception caught: " << error.what() << "\n";
    }
}

int main() {
    // Try to read as np::vector<double>
    try_load<double, 1>();
    // Try to read as np::matrix<int>
    try_load<int, 2>();
    // Try to read as np::vector<int>
    try_load<int, 1>();
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

Save tensor contents to a binary file in NumPy `.npy` format.
```cpp
template <class T, size_t Rank>
void save(const std::string &filename, const tensor<T, Rank> &data);

template <class T, size_t Rank>
void save(std::ostream &file, const tensor<T, Rank> &data);
```

Parameters

* `filename` A string representing the name of the file destination.
* `file` File object to write.
* `data` Tensor data to be saved. Only arithmetic types (either integer or floating-point) and complex types are supported.

Returns

* None

Exceptions

* `std::ios_base::failure` Thrown if the output file cannot be written.

Example

C++

```cpp
#include <iostream>
#include <numcpp/tensor.h>
#include <numcpp/io.h>
namespace np = numcpp;
int main() {
    np::vector<int> a{1, -9, 5, 10, -2, 7, 8, 12, 0, 3};
    np::save("example.npy", a);
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
