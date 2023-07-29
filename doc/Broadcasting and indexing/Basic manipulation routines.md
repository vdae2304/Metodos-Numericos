# [Broadcasting and indexing](readme.md)

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Basic manipulation routines](#basic-manipulation-routines)
    - [`astype`](#astype)
    - [`copyto`](#copyto)
    - [`copy`](#copy)
    - [`flatten`](#flatten)
    - [`zip`](#zip)
    - [`unzip`](#unzip)

## Basic manipulation routines

### `astype`

Cast each element in a tensor to a specified type.
```cpp
template <class U>
tensor<U, Rank> astype(const tensor<T, Rank> &a);
```

Template parameters

* `U` Data type to which the elements are casted.

Parameters

* `a` Tensor-like object to cast.

Returns

* A light-weight object with the elements in the tensor casted to the specified type. This function does not create a new tensor, instead, an expression object is returned with its elements casted to the specified type.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<double> a;
    std::cin >> a;
    std::cout << np::astype<int>(a) << "\n";
    return 0;
}
```

Input

```
[2.01, -3.62, -4.97, 6.77, 8.33, 5.93, 0.49, 7.8, 14.8, -2.3]
```

Output

```
[ 2, -3, -4,  6,  8,  5,  0,  7, 14, -2]
```

### `copyto`

Copies values from one tensor to another, broadcasting as necessary.
```cpp
template <class T, size_t Rank, class U>
void copyto(tensor<T, Rank> &dest, const tensor<U, Rank> &src);
```

Parameters

* `dest` The tensor into which values are copied.
* `src` The tensor from which values are copied.

Returns

* None

Exceptions

* `std::invalid_argument` Thrown if the shape of `src` is not compatible with the shape of `dest` according to [broadcasting rules](/doc/Tensor%20class/Shapes/Non-member%20functions.md#broadcast_shapes).

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::vector<int> x;
    std::cin >> x;
    // Recall x*x - 3*x + 2 is an expression object. By assigning the result to
    // y, we force its evaluation.
    np::vector<int> y(x.shape());
    // The following line is equivalent to y = x*x - 3*x + 2.
    np::copyto(y, x*x - 3*x + 2);
    std::cout << y << "\n";
    return 0;
}
```

Input

```
[-4,  4, 16,  1,  9, -4, 13,  8,  8, -1]
```

Output

```
[ 30,   6, 210,   0,  56,  30, 132,  42,  42,   6]
```

### `copy`

Return a copy of the tensor.
```cpp
template <class T, size_t Rank>
tensor<T, Rank> copy(const tensor<T, Rank> &a);

template <class T, size_t Rank>
tensor<T, Rank> copy(const tensor<T, Rank> &a, layout_t order);
```

Parameters

* `a` Tensor-like object to copy.
* `order` Memory layout in which elements are stored. In row-major order, the last dimension is contiguous. In column-major order, the first dimension is contiguous. The default is to use the same layout as `a`.

Returns

* A new tensor with a copy of each of the elements in the tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> mat;
    std::cin >> mat;
    // Because mat.t() returns a view of the elements in mat,
    //     mat = mat.t();
    // might produce unexpected behaviour. Therefore, a temporary copy needs to
    // be created before assignment. 
    mat = np::copy(mat.t());
    std::cout << mat << "\n";
    return 0;
}
```


Input

```
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
```

Output

```
[[ 1, -5, -1],
 [14, -3, 18],
 [12, 11, -3],
 [-3, 11, -1]]
```

### `flatten`

Return a copy of the tensor flattened into one dimension.
```cpp
template <class T, size_t Rank>
tensor<T, 1> flatten(const tensor<T, Rank> &a);

template <class T, size_t Rank>
tensor<T, 1> flatten(const tensor<T, Rank> &a, layout_t order);
```

Parameters

* `a` Tensor-like object to flatten.
* `order` Whether to flatten the elements in row-major order or column-major order. The default is to use the same layout as stored in memory.

Returns

* The flattened tensor.

Exceptions

* `std::bad_alloc` If the function fails to allocate storage it may throw an exception.


Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    np::matrix<int> a;
    std::cin >> a;
    std::cout << "Row-major order:\n";
    std::cout << np::flatten(a) << "\n";
    std::cout << "Column-major order:\n";
    std::cout << np::flatten(a, np::column_major) << "\n";
    return 0;
}
```

Input

```
[[ 1, 14, 12, -3],
 [-5, -3, 11, 11],
 [-1, 18, -3, -1]]
```

Output

```
Row-major order:
[ 1, 14, 12, -3, -5, -3, 11, 11, -1, 18, -3, -1]
Column-major order:
[ 1, -5, -1, 14, -3, 18, 12, 11, -3, -3, 11, -1]
```

### `zip`

Return a tensor of tuples with each element constructed from taking the corresponding element from all tensors.
```cpp
template <class T, class U, size_t Rank>
tensor<std::pair<T, U>, Rank> zip(const tensor<T, Rank> &a,
                                  const tensor<U, Rank> &b);

// Since C++14
template <class T, class... U, size_t Rank>
tensor<std::tuple<T, U...>, Rank> zip(const tensor<T, Rank> &a,
                                      const tensor<U, Rank> &...b);
```

Parameters

* `a, b...` Tensor-like objects to zip. Since C++14, this function accepts any number of arguments.

Returns

* A light-weight object with the tuples constructed from the elements of each tensor. This function does not create a new tensor, instead, an expression object is returned with a tuple of the elements of each tensor.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;

namespace std {
    // Overload output stream for std::pair
    template <class T, class U>
    ostream& operator<<(ostream &ostr, const pair<T, U> &rhs) {
        ostringstream sstr;
        sstr.flags(ostr.flags());
        sstr.imbue(ostr.getloc());
        sstr.precision(ostr.precision());
        sstr << "(" << rhs.first << ", " << rhs.second << ")";
        return ostr << sstr.str();
    }
}

int main() {
    np::vector<std::string> names{"Noelle", "Mark", "Steve", "David", "Andrew"};
    np::vector<int> ages{20, 24, 18, 26, 21};
    std::cout << np::zip(names, ages) << "\n";
    return 0;
}
```

Output

```
[(Noelle, 20),   (Mark, 24),  (Steve, 18),  (David, 26), (Andrew, 21)]
```

Example

```cpp
#include <iostream>
#include <tuple>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;

namespace std {
    // Overload output stream for std::tuple of 3 elements
    template <class T, class U, class V>
    ostream& operator<<(ostream &ostr, const tuple<T, U, V> &rhs) {
        ostringstream sstr;
        sstr.flags(ostr.flags());
        sstr.imbue(ostr.getloc());
        sstr.precision(ostr.precision());
        sstr << "(" << get<0>(rhs) << ", " << get<1>(rhs) << ", "
             << get<2>(rhs) << ")";
        return ostr << sstr.str();
    }
}

int main() {
    np::vector<std::string> brands{"Ford", "BMW", "Chevrolet", "Honda",
                                   "Nissan"};
    np::vector<std::string> models{"Mustang", "X5", "Corvette", "Civic",
                                   "Tsuru"};
    np::vector<int> years{1969, 1999, 2005, 1998, 1990};
    std::cout << np::zip(brands, models, years) << "\n";
    return 0;
}
```

Output

```
[      (Ford, Mustang, 1969),             (BMW, X5, 1999), 
 (Chevrolet, Corvette, 2005),        (Honda, Civic, 1998), 
       (Nissan, Tsuru, 1990)]
```

### `unzip`

Return a tensor with each element constructed from taking the I-th element of the corresponding tuple in a tensor.
```cpp
template <size_t I, class Tuple, size_t Rank>
tensor<typename std::tuple_element<I, Tuple>::type, Rank>
unzip(const tensor<Tuple, Rank> &a);

// Since C++14
template <class T, class Tuple, size_t Rank>
tensor<T, Rank> unzip(const tensor<Tuple, Rank> &a);
```

Template parameters

* `I` Element index.
* `T` (Since C++14) Element type. Fails to compile unless the tuple has exactly one element of that type.

Parameters

* `a` A tensor-like object of tuple-like values. Any class overloading `std::get` such as `std::pair`, `std::tuple` and `std::array`, is considered a tuple.

Returns

* A light-weight object with the `I`-th element of each tuple in the tensor. This function does not create a new tensor, instead, an expression object is returned with the `I`-th element of each tuple.

Example

```cpp
#include <iostream>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    typedef std::pair<std::string, int> person;
    enum { name, age };
    np::vector<person> a{{"Noelle", 20},
                         {"Mark", 24},
                         {"Steve", 18},
                         {"David", 26},
                         {"Andrew", 21}};
    std::cout << "Name:\n" << np::unzip<name>(a) << "\n";
    std::cout << "Age:\n" << np::unzip<age>(a) << "\n";
    return 0;
}
```

Output

```
Name:
["Noelle",   "Mark",  "Steve",  "David", "Andrew"]
Age:
[20, 24, 18, 26, 21]
```

Example

```cpp
#include <iostream>
#include <tuple>
#include "numcpp/tensor.h"
#include "numcpp/broadcasting.h"
#include "numcpp/io.h"
namespace np = numcpp;
int main() {
    typedef std::tuple<std::string, std::string, int> car;
    enum { brand, model, year };
    np::vector<car> a{{"Ford", "Mustang", 1969},
                      {"BMW", "X5", 1999},
                      {"Chevrolet", "Corvette", 2005},
                      {"Honda", "Civic", 1998},
                      {"Nissan", "Tsuru", 1990}};
    std::cout << "Brand:\n" << np::unzip<brand>(a) << "\n";
    std::cout << "Model:\n" << np::unzip<model>(a) << "\n";
    std::cout << "Year:\n" << np::unzip<year>(a) << "\n";
    return 0;
}
```

Output

```
Brand:
[     "Ford",       "BMW", "Chevrolet",     "Honda",    "Nissan"]
Model:
[ "Mustang",       "X5", "Corvette",    "Civic",    "Tsuru"]
Year:
[1969, 1999, 2005, 1998, 1990]
```
