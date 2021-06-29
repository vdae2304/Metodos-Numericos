# Arrays

## Content

- [Constructors](#Constructors)
- [Destructor](#Destructor)
- [Array creation routines](#Array-creation-routines)
- [Assignment operators](#Assignment-operators)
- [Operator overloading](#Operator-overloading)
- [Basic indexing](#Basic-indexing)
- [Advanced indexing](#Advanced-indexing)
- [Methods list](#Methods-list)
- [Global functions](#Global-functions)

<hr>

Arrays are fixed-size sequence containers: they hold a specific number of 
elements ordered in a strict linear sequence.

To create an array, first include the `numcpp` header at the beginning of each 
program. Arrays will be represented by the `array` class.

> NOTE: Do not confuse between `numcpp::array` and `std::array`. The first 
> represents an array from the `numcpp` header, which is specially implemented 
> to handle distinct mathematical computations, whereas the second is a common 
> array from the *Standard Template Library*. To avoid conflicts in compilation, 
> keep the `numcpp` namespace.

#### Example

Compute the sum of the element-wise product of two arrays (dot product).
```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1, 5, 2, 0, 1};
    np::array<double> w = {7, -1, 2, 8, 3};
    cout << np::sum(v * w) << "\n";
    return 0;
}
```

```
[Out] 9
```

## Constructors

1. Default constructor. Constructs an empty array with no elements.
```cpp
array();
```

2. Size constructor. Constructs an array with `n` elements.
```cpp
array(size_t n);
```

3. Fill constructor. Constructs an array with `n` elements, each initialized to 
`val`.
```cpp
array(size_t n, const T &val);
```

4. Range constructor. Constructs an array with as many elements as the range 
`[first, last)`, with each element constructed from its corresponding element 
in that range, in the same order.
```cpp
template <class InputIterator> 
array(InputIterator first, InputIterator last);
```

5. Copy constructor. Constructs an array with a copy of each of the elements in 
`v`, in the same order.
```cpp
array(const array &v);
```

6. Move constructor. Constructs an array that acquires the elements of  `v`.
```cpp
array(array &&v);
```

7. Initializer list. Constructs an array with a copy of each of the elements in 
`il`, in the same order.
```cpp
array(std::initializer_list<T> il);
```

## Destructor

Destroys the array.
```cpp
~array();
```

## Array creation routines

### Ones and zeros

- **empty**: Returns a new uninitialized array with `n` elements.
```cpp
template<class T = double> 
array<T> empty(size_t n);
```

- **zeros**: Returns a new array of length `n` setting values to zero.
```cpp
template<class T = double> 
array<T> zeros(size_t n);
```

- **ones**: Returns a new array of length `n` setting values to one.
```cpp
template<class T = double> 
array<T> ones(size_t n);
```

- **full**: Returns a new array of length `n` setting values to `val`.
```cpp
template <class T = double> 
array<T> full(size_t n, const T &val);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> empty = np::empty<int>(5);
    np::array<int> zeros = np::zeros<int>(5);
    np::array<int> ones = np::ones<int>(5);
    np::array<int> threes = np::full<int>(5, 3);
    cout << "empty: " << empty << "\n";
    cout << "zeros: " << zeros << "\n";
    cout << "ones: " << ones << "\n";
    cout << "threes: " << threes << "\n";
    return 0;
}
```

```
[Out] empty: [17504992, 0, 17498448, 0, 0]
      zeros: [0, 0, 0, 0, 0]
      ones: [1, 1, 1, 1, 1]
      threes: [3, 3, 3, 3, 3]
```

### From a binary file

- **load**: Returns a new array from a binary file. The function throws a 
`runtime_error` exception if the file passed as an argument does not exist.
```cpp
template <class T>
array<T> load(const char *file);
```

- **save**: Save an array's content into a binary file. The function throws a 
`runtime_error` exception if the file passed as an argument could not be 
opened.
```cpp
template <class T>
void save(const char *file, const array<T> &v);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    np::save("numbers-1-to-10.bin", v);
    np::array<int> w = np::load<int>("numbers-1-to-10.bin");
    cout << w << "\n";
    return 0;
}
```

```
[Out] [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

### Numerical ranges

- **arange**: Returns a new array with evenly spaced values within an interval. 
Values are generated within the half-open interval `[start, stop)` with steps 
of size `step`. By default, the start point is 0 and the step size is 1.
```cpp
template <class T> 
array<T> arange(const T &stop);

template <class T> 
array<T> arange(const T &start, const T &stop, const T &step = T(1));
```

- **linspace**: Returns a new array with evenly spaced numbers over a specified 
interval. Returns `num` evenly spaced samples, calculated over the closed 
interval `[start, stop]`. The endpoint of the interval can optionally be 
excluded.
```cpp
template <class T> 
array<T> linspace(
    const T &start, const T &stop, 
    size_t num = 50, 
    bool endpoint = true
)
```

- **logspace**: Returns a new array with evenly spaced numbers on a log scale. 
In the linear space, the sequence starts at `pow(base, start)` (`base` to the 
power of `start`) and ends with `pow(base, stop)`. The endpoint of the interval 
can optionally be excluded.
```cpp
template <class T>
array<T> logspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true,
    const T &base = 10.0
)
```

- **geomspace**: Returns a new array with numbers on a geometric progression. 
Each output sample is a constant multiple of the previous. The endpoint of the 
interval can optionally be excluded.
```cpp
template <class T>
array<T> geomspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true
)
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    cout << "arange:\n";
    cout << np::arange(10) << "\n";
    cout << np::arange(0., 1., 0.1) << "\n";
    cout << np::arange(5, 0, -1) << "\n";

    cout << "linspace:\n";
    cout << np::linspace(0., 1., 5) << "\n";
    cout << np::linspace(0., 1., 5, false) << "\n";

    cout << "logspace:\n";
    cout << np::logspace(1., 5., 5, true, 2.) << "\n";
    cout << np::logspace(0., 5., 5, false, 10.) << "\n";

    cout << "geomspace:\n";
    cout << np::geomspace(1., 256., 9) << "\n";
    cout << np::geomspace(1., 100000., 5, false) << "\n";
    return 0;
}
```

```
[Out] arange:
      [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
      [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
      [5, 4, 3, 2, 1]
      linspace:
      [0, 0.25, 0.5, 0.75, 1]
      [0, 0.2, 0.4, 0.6, 0.8]
      logspace:
      [2, 4, 8, 16, 32]
      [1, 10, 100, 1000, 10000]
      geomspace:
      [1, 2, 4, 8, 16, 32, 64, 128, 256]
      [1, 10, 100, 1000, 10000]
```

## Assignment operators

1. Copy assignment. Assigns the contents of `v` to `*this` after resizing the
array (if necessary).
```cpp
array& operator= (const array &v);
```

2. Move assignment. Acquires the contents of `v`.
```cpp
array& operator= (array &&v);
```

3. Fill assignment. Assigns `val` to every element.
```cpp
array& operator= (const T &val);
```

## Operator overloading

Each of this functions performs their respective operation on all the elements 
in the array.

When both the left-hand side argument and the right-side one are array objects, 
the operation is performed between the corresponding elements in each object 
(the first element of the left one with the first of the right one, the second with the second, and so on). If the array's lenghts differs, the operator 
throws a `runtime_error`.

When one of the arguments is a value, the operation is applied to all the 
elements in the array against that value.

### Member functions

```cpp
array& operator+= (const array &v);
array& operator-= (const array &v);
array& operator*= (const array &v);
array& operator/= (const array &v);
array& operator%= (const array &v);
array& operator&= (const array &v);
array& operator|= (const array &v);
array& operator^= (const array &v);
array& operator<<= (const array &v);
array& operator>>= (const array &v);

array& operator+= (const T &val);
array& operator-= (const T &val);
array& operator*= (const T &val);
array& operator/= (const T &val);
array& operator%= (const T &val);
array& operator&= (const T &val);
array& operator|= (const T &val);
array& operator^= (const T &val);
array& operator<<= (const T &val);
array& operator>>= (const T &val);
```

### Unitary operators

```cpp
template <class T> array<T> operator+ (const array<T> &v);
template <class T> array<T> operator- (const array<T> &v);
template <class T> array<T> operator~ (const array<T> &v);
array<bool> operator! (const array<bool> &v);
```

### Non-member functions

```cpp
template<class T> array<T> operator+ (const array<T> &v, const array<T> &w);
template<class T> array<T> operator+ (const array<T> &v, const T &val);
template<class T> array<T> operator+ (const T &val, const array<T> &v);

template<class T> array<T> operator- (const array<T> &v, const array<T> &w);
template<class T> array<T> operator- (const array<T> &v, const T &val);
template<class T> array<T> operator- (const T &val, const array<T> &v);

template<class T> array<T> operator* (const array<T> &v, const array<T> &w);
template<class T> array<T> operator* (const array<T> &v, const T &val);
template<class T> array<T> operator* (const T &val, const array<T> &v);

template<class T> array<T> operator/ (const array<T> &v, const array<T> &w);
template<class T> array<T> operator/ (const array<T> &v, const T &val);
template<class T> array<T> operator/ (const T &val, const array<T> &v);

template<class T> array<T> operator% (const array<T> &v, const array<T> &w);
template<class T> array<T> operator% (const array<T> &v, const T &val);
template<class T> array<T> operator% (const T &val, const array<T> &v);

template<class T> array<T> operator& (const array<T> &v, const array<T> &w);
template<class T> array<T> operator& (const array<T> &v, const T &val);
template<class T> array<T> operator& (const T &val, const array<T> &v);

template<class T> array<T> operator| (const array<T> &v, const array<T> &w);
template<class T> array<T> operator| (const array<T> &v, const T &val);
template<class T> array<T> operator| (const T &val, const array<T> &v);

template<class T> array<T> operator^ (const array<T> &v, const array<T> &w);
template<class T> array<T> operator^ (const array<T> &v, const T &val);
template<class T> array<T> operator^ (const T &val, const array<T> &v);

template<class T> array<T> operator<< (const array<T> &v, const array<T> &w);
template<class T> array<T> operator<< (const array<T> &v, const T &val);
template<class T> array<T> operator<< (const T &val, const array<T> &v);

template<class T> array<T> operator>> (const array<T> &v, const array<T> &w);
template<class T> array<T> operator>> (const array<T> &v, const T &val);
template<class T> array<T> operator>> (const T &val, const array<T> &v);

template<class T> array<bool> operator== (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator== (const array<T> &v, const T &val);
template<class T> array<bool> operator== (const T &val, const array<T> &v);

template<class T> array<bool> operator!= (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator!= (const array<T> &v, const T &val);
template<class T> array<bool> operator!= (const T &val, const array<T> &v);

template<class T> array<bool> operator< (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator< (const array<T> &v, const T &val);
template<class T> array<bool> operator< (const T &val, const array<T> &v);

template<class T> array<bool> operator> (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator> (const array<T> &v, const T &val);
template<class T> array<bool> operator> (const T &val, const array<T> &v);

template<class T> array<bool> operator<= (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator<= (const array<T> &v, const T &val);
template<class T> array<bool> operator<= (const T &val, const array<T> &v);

template<class T> array<bool> operator>= (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator>= (const array<T> &v, const T &val);
template<class T> array<bool> operator>= (const T &val, const array<T> &v);
```

### Input/Output
```cpp
template <class T>
std::ostream& operator<< (std::ostream &ostr, const array<T> &v);
```
## Basic indexing

### Size

- **size**: Returns the number of elements in the array.
```cpp
size_t size() const;
```

### Indexing

- **[] operator**: Access the `i`-th element in the array. Notice that the 
first element has the position 0, not 1. Values equal or greater than the 
array's size cause undefined behaviour.
```cpp
T& operator[] (size_t i);
const T& operator[] (size_t i) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = np::zeros(5);
    cout << "v has " << v.size() << " elements.\n";
    v[0] = 1.;
    v[3] = 2.5;
    v[2] = -0.1;
    cout << "v's elements are: " << v << "\n";
    return 0;
}
```

```
[Out] v has 5 elements.
      v's elements are: [1, 0, -0.1, 2.5, 0]
```

## Advanced indexing

### Sub-arrays

A sub-array is an intermediate class returned by the array's subscript `[]` 
operator. It references the elements in the array that are selected by the 
subscript, and overloads the assignment and compound assignment operators, 
allowing direct access to the elements in the selection. The object is 
convertible to an array, producing a new object with copies of the referred 
elements.

There are three types of sub-arrays depending on how the subscripts are 
selected: `slice_subarray`, `index_subarray` y 
`mask_subarray`.
```cpp
template <class T>
class slice_subarray {
public:
    void operator= (const array<T> &v);
    void operator= (const T &val);

    void operator+= (const array<T> &v);
    void operator-= (const array<T> &v);
    void operator*= (const array<T> &v);
    void operator/= (const array<T> &v);
    void operator%= (const array<T> &v);
    void operator&= (const array<T> &v);
    void operator|= (const array<T> &v);
    void operator^= (const array<T> &v);
    void operator<<= (const array<T> &v);
    void operator>>= (const array<T> &v);

    void operator+= (const T &val);
    void operator-= (const T &val);
    void operator*= (const T &val);
    void operator/= (const T &val);
    void operator%= (const T &val);
    void operator&= (const T &val);
    void operator|= (const T &val);
    void operator^= (const T &val);
    void operator<<= (const T &val);
    void operator>>= (const T &val);

    T& operator[] (size_t i);
    const T& operator[] (size_t i) const;

    array<T> copy() const;
    size_t size() const;
};
```

```cpp
template <class T>
class index_subarray {
public:
    void operator= (const array<T> &v);
    void operator= (const T &val);

    void operator+= (const array<T> &v);
    void operator-= (const array<T> &v);
    void operator*= (const array<T> &v);
    void operator/= (const array<T> &v);
    void operator%= (const array<T> &v);
    void operator&= (const array<T> &v);
    void operator|= (const array<T> &v);
    void operator^= (const array<T> &v);
    void operator<<= (const array<T> &v);
    void operator>>= (const array<T> &v);

    void operator+= (const T &val);
    void operator-= (const T &val);
    void operator*= (const T &val);
    void operator/= (const T &val);
    void operator%= (const T &val);
    void operator&= (const T &val);
    void operator|= (const T &val);
    void operator^= (const T &val);
    void operator<<= (const T &val);
    void operator>>= (const T &val);

    T& operator[] (size_t i);
    const T& operator[] (size_t i) const;

    array<T> copy() const;
    size_t size() const;
};
```

```cpp
template <class T>
class mask_subarray {
public:
    void operator= (const array<T> &v);
    void operator= (const T &val);

    void operator+= (const array<T> &v);
    void operator-= (const array<T> &v);
    void operator*= (const array<T> &v);
    void operator/= (const array<T> &v);
    void operator%= (const array<T> &v);
    void operator&= (const array<T> &v);
    void operator|= (const array<T> &v);
    void operator^= (const array<T> &v);
    void operator<<= (const array<T> &v);
    void operator>>= (const array<T> &v);

    void operator+= (const T &val);
    void operator-= (const T &val);
    void operator*= (const T &val);
    void operator/= (const T &val);
    void operator%= (const T &val);
    void operator&= (const T &val);
    void operator|= (const T &val);
    void operator^= (const T &val);
    void operator<<= (const T &val);
    void operator>>= (const T &val);

    array<T> copy() const;
    size_t size() const;
};
```

### Slice indexing

A `slice` describes a selection of elements to be used as an index by the 
`[]` operator. A slice is defined by a starting index (`start`), a stop index 
(`stop`) and a step size (`step`). For example, `slice(3, 20, 5)` selects the 
elements with indices 3, 8, 13 and 18. By default, the starting index is 0 and 
the step size is 1.
```cpp
slice(size_t stop);
slice(size_t start, size_t stop, int step = 1);
```
Indexing an array using slices, the operator will return a `slice_subarray<T>` 
object representing a subarray with the elements selected by the slice.
```cpp
slice_subarray<T> operator[] (slice slc);
const slice_subarray<T> operator[] (slice slc) const;
```

#### Example
```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {8, -3, 7, 5, 10, -1, 1, 3, -5, 14};
    np::slice_subarray<int> sub = v[np::slice(3, 10, 2)];
    cout << "v's elements are: " << v << "\n";
    cout << "The subarray has " << sub.size() << " elements.\n";
    cout << "The subarray's elements are: " << sub.copy() << "\n";
    sub = 0;
    cout << "v's elements are now: " << v << "\n";
    return 0;
}
```

```
[Out] v's elements are: [8, -3, 7, 5, 10, -1, 1, 3, -5, 14]
      The subarray has 4 elements.
      The subarray's elements are: [5, -1, 3, 14]
      v's elements are now: [8, -3, 7, 0, 10, 0, 1, 0, -5, 0]
```

### Integer array indexing

Indexing an array using integer arrays (of `size_t` data type), the operator 
will return a `index_subarray<T>` object representing a subarray with the 
elements selected by the integer array.
```cpp
index_subarray<T> operator[] (const array<size_t> &index);
const index_subarray<T> operator[] (const array<size_t> &index) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {8, -3, 7, 5, 10, -1, 1, 3, -5, 14};
    np::array<size_t> indices = {1, 5, 6, 8, 2};
    np::index_subarray<int> sub = v[indices];
    cout << "v's elements are: " << v << "\n";
    cout << "The subarray has " << sub.size() << " elements.\n";
    cout << "The subarray's elements are: " << sub.copy() << "\n";
    sub = 0;
    cout << "v's elements are now: " << v << "\n";
    return 0;
}
```

```
[Out] v's elements are: [8, -3, 7, 5, 10, -1, 1, 3, -5, 14]
      The subarray has 5 elements.
      The subarray's elements are: [-3, -1, 1, -5, 7]
      v's elements are now: [8, 0, 0, 5, 10, 0, 0, 3, 0, 14]
```

### Boolean array indexing

Indexing an array using boolean arrays, the operator will return a 
`mask_subarray<T>` object representing a subarray with the elements selected by 
the boolean mask.
```cpp
mask_subarray<T> operator[] (const array<bool> &mask);
const mask_subarray<T> operator[] (const array<bool> &mask) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {8, -3, 7, 5, 10, -1, 1, 3, -5, 14};
    np::array<bool> mascara = {1, 1, 0, 0, 1, 0, 1, 0, 1, 0};
    np::mask_subarray<int> sub = v[mascara];
    cout << "v's elements are: " << v << "\n";
    cout << "The subarray has " << sub.size() << " elements.\n";
    cout << "The subarray's elements are: " << sub.copy() << "\n";
    sub = 0;
    cout << "v's elements are now: " << v << "\n";
    return 0;
}
```

```
[Out] v's elements are: [8, -3, 7, 5, 10, -1, 1, 3, -5, 14]
      The subarray has 5 elements.
      The subarray's elements are: [8, -3, 10, 1, -5]
      v's elements are now: [0, 0, 7, 5, 0, -1, 0, 3, 0, 14]
```

## Methods list

### `apply` 

Replaces each element in the array with the result of applying a function to 
its corresponding element.
```cpp
template <class Function = T(T)>
void apply(Function f);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;

double square(double x) {
    return x*x;
}

int main() {
    np::array<double> v = {1., -9., 5., 10., -2., 7.};
    v.apply(square);
    cout << v << "\n";
    return 0;
}
```

```
[Out] [1, 81, 25, 100, 4, 49]
```

### `argmax` 

Returns the index of the maximum value contained in the array.
```cpp
size_t argmax() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
    cout << v.argmax() << "\n";
    return 0;
}
```

```
[Out] 0
```

### `argmin` 

Returns the index of the minimum value contained in the array.
```cpp
size_t argmin() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
    cout << v.argmin() << "\n";
    return 0;
}
```

```
[Out] 3
```

### `argsort` 

Returns the indices that would sort the array.
```cpp
array<size_t> argsort() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -4.1, 2.5, 0., 3.};
    cout << v.argsort() << "\n";
    return 0;
}
```

```
[Out] [1, 3, 0, 2, 4]
```

### `astype` 

Returns a copy of the array, cast to a specified type.
```cpp
template <class U> array<U> astype() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., 2.5, 3., -4.1, 0.};
    np::array<int> w = v.astype<int>();
    cout << w << "\n";
    return 0;
}
```

```
[Out] [1, 2, 3, -4, 0]
```

### `clip` 

Clip (limit) the values in the array. Given an interval, values outside the 
interval are clipped to the interval edges.
```cpp
void clip(const T &a_min, const T &a_max);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -9., 3., 10., -2., 7.};
    v.clip(0., 5.);
    cout << v << "\n";
    return 0;
}
```

```
[Out] [1, 0, 3, 5, 0, 5]
```

### `cumprod`

Returns the cumulative product of the elements in an array.
```cpp
array cumprod() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.cumprod() << "\n";
    return 0;
}
```

```
[Out] [2, 6, 30, 30, 120]
```

### `cumsum` 
Returns the cumulative sum of the elements in an array.
```cpp
array cumsum() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.cumsum() << "\n";
    return 0;
}
```

```
[Out] [2, 5, 10, 11, 15]
```

### `data`

Returns a pointer to the memory array used internally.
```cpp
T* data();
const T* data() const;
```

### `dot` 

Returns the dot product of two arrays. The dot product is defined by 
`sum(v * w)`.
```cpp
T dot(const array &v) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1, 5, 2, 0, 1};
    np::array<double> w = {7, -1, 2, 8, 3};
    cout << v.dot(w) << "\n";
    return 0;
}
```

```
[Out] 9
```

### `max` 

Returns the maximum value contained in the array.
```cpp
T max() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
    cout << v.max() << "\n";
    return 0;
}
```

```
[Out] 5
```

### `mean` 

Returns the mean or average of the array elements. The mean is defined by
`x.sum() / x.size()`.
```cpp
T mean() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -3., 2., 0., 5., 2.};
    cout << v.mean() << "\n";
    return 0;
}
```

```
[Out] 1.16667
```

### `min` 

Returns the minimum value contained in the array.
```cpp
T min() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
   cout << v.min() << "\n";
    return 0;
}
```

```
[Out] -4.1
```

### `prod` 

Returns the product of the array elements.
```cpp
T prod() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.prod() << "\n";
    return 0;
}
```

```
[Out] 120
```

### `resize`

Resizes the array, changing its size to `n` elements. If `n` is smaller than 
the current size, the content is reduced to its first `n` elements, removing 
those beyond. If `n` is greater than the current size, the content is expanded 
by inserting at the end as many elements as needed to reach a size of `n`.
```cpp
void resize(size_t n, const T &val = T());
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -9., 5., 10., -2., 7.};
    cout << "Before: " << v << "\n";
    v.resize(10, 0.);
    cout << "After expanding: " << v << "\n";
    v.resize(3, 0.);
    cout << "After reducing: " << v << "\n";
    return 0;
}
```

```
[Out] Before: [1, -9, 5, 10, -2, 7]
      After expanding: [1, -9, 5, 10, -2, 7, 0, 0, 0, 0]
      After reducing: [1, -9, 5]
```

### `size`

Returns the number of elements in the array.
```cpp
size_t size() const;
```

### `sort` 

Sort an array in-place.
```cpp
void sort();
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -4.1, 2.5, 0., 3.};
    v.sort();
    cout << v << "\n";
    return 0;
}
```

```
[Out] [-4.1, 0, 1, 2.5, 3]
```

### `stddev` 

Returns the standard deviation of the array elements. The standard deviation is 
defined by `sqrt(square(x - x.mean()) / (x.size() - ddof))` 
(the square root of the variance) where `ddof` indicates the degrees of freedom.
By default, `ddof = 0`.
```cpp
T stddev(size_t ddof = 0) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -3., 2., 0., 5., 2.};
    cout << v.stddev() << "\n";
    return 0;
}
```

```
[Out] 2.40947
```

### `sum` 

Returns the sum of the array elements.
```cpp
T sum() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.sum() << "\n";
    return 0;
}
```

```
[Out] 15
```

### `swap` 

Swap contents with `v`. This function is implemented to work in constant time.
```cpp
void swap(array &v);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -9., 5., 10., -2.};
    np::array<double> w = {1, 2, 3};
    cout << "Before: " << v << " and " << w << "\n";
    v.swap(w);
    cout << "After: " << v << " and " << w << "\n";
    return 0;
}
```

```
[Out] Before: [1, -9, 5, 10, -2] and [1, 2, 3]
      After: [1, 2, 3] and [1, -9, 5, 10, -2]
```

### `var` 

Returns the variance of the array elements. The variance is defined by 
`square(x - x.mean()) / (x.size() - ddof)` where `ddof` indicates the degrees 
of freedom. By default, `ddof = 0`.
```cpp
T var(size_t ddof = 0) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -3., 2., 0., 5., 2.};
    cout << v.var() << "\n";
    return 0;
}
```

```
[Out] 5.80556
```

## Global functions

### `all`

Returns `true` if all elements evaluate to `true`.
```cpp
bool all(const array<bool> &v);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, 3, 2, 5, 7};
    cout << boolalpha << "Are all positve? " << np::all(v > 0) << "\n";
    cout << "Are all even? " << np::all(v % 2 == 0) << "\n";
    return 0;
}
```

```
[Out] Are all positve? true
      Are all even? false
```

### `allclose`

Returns `true` if two arrays are element-wise equal within a tolerance. If the 
following equation is element-wise true, the function returns `true`:
`abs(v[i] - w[i]) <= atol + rtol*abs(w[i])`, where `atol` is the absoltue 
tolerance and `rtol` is the relative tolerance.
```cpp
template <class T>
bool allclose(
    const array<T> &v, const array<T> &w,
    const T &atol = 1e-8, const T &rtol = 1e-5
);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1e10, 1e-7};
    np::array<double> w = {1.00001e10, 1e-8};
    cout << boolalpha << np::allclose(v, w) << "\n";
    v = {1e10, 1e-8};
    w = {1.00001e10, 1e-9};
    cout << np::allclose(v, w) << "\n";
    return 0;
}
```

```
[Out] false
      true
```

### `any`

Returns `true` if any of the elements evaluate to `true`.
```cpp
bool any(const array<bool> &v);
```

### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, 3, 2, 5, 7};
    cout << boolalpha << "Is there a negative? " << np::any(v < 0) << "\n";
    cout << "Is there an even? " << np::any(v % 2 == 0) << "\n";
    return 0;
}
```

```
[Out] Is there a negative? false
      Is there an even? true
```

### `apply`

In its first version, returns an array with each element initialized to the 
result of applying a function `f` to its corresponding element in `v`. Contrary 
to the `v.apply()` method, this function does not modify the elements in `v`.

In its second version, returns an array with each element initialized to the 
result of applying a function `f` to the corresponding elements in `v` and `w`;
to the elements in `v` against `val`; to `val` against the elements in `v` 
depending on the case.
```cpp
template <class T, class Function = T(T)>
array<T> apply(Function f, const array<T> &v);

template <class T, class Function = T(T, T)>
array<T> apply(Function f, const array<T> &v, const array<T> &w);

template <class T, class Function = T(T, T)>
array<T> apply(Function f, const array<T> &v, const T &val);

template <class T, class Function = T(T, T)>
array<T> apply(Function f, const T &val, const array<T> &v);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;

double square(double x) {
    return x*x;
}

double f(double x, double y) {
    return x + 2*y;
}

int main() {
    np::array<double> x = {1, 3, 2, 5, 7};
    np::array<double> y = {-1, 0, 2, 5, 3};
    cout << np::apply(square, x) << "\n";
    cout << np::apply(f, x, y) << "\n";
    return 0;
}
```

```
[Out] [1, 9, 4, 25, 49]
      [-1, 3, 6, 15, 13]
```

### `arange`

Returns a new array with evenly spaced values within an interval. Values are 
generated within the half-open interval `[start, stop)` with steps of size 
`step`. By default, the start point is 0 and the step size is 1.
```cpp
template <class T> 
array<T> arange(const T &stop);

template <class T> 
array<T> arange(const T &start, const T &stop, const T &step = T(1));
```

### `argmax` 

Returns the index of the maximum value stored in the array.
```cpp
template <class T>
size_t argmax(const array<T> &v);
```

### `argmin` 

Returns the index of the minimum value stored in the array.
```cpp
template <class T>
size_t argmin(const array<T> &v);
```

### `argsort` 

Returns the indices that would sort the array.
```cpp
template <class T>
size_t argsort(const array<T> &v);
```

### `clip` 

Returns an array with each element initialized to the result of clipping the 
values in an array. Contrary to the `v.clip()` method, this function does not 
modify the elements in `v`.
```cpp
template <class T>
array<T> clip(const array<T> &v, const T &a_min, const T &a_max);
```

### `concatenate`

Concatenate (join) two arrays.
```cpp
template <class T>
array<T> concatenate(const array<T> &v, const array<T> &w);
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, -9, 5, 10};
    np::array<int> w = {0, 7, -1};
    cout << np::concatenate(v, w) << "\n";
    return 0;
}
```

```
[Out] [1, -9, 5, 10, 0, 7, -1]
```

### `cumprod`

Returns the cumulative product of the elements in an array.
```cpp
template <class T>
array<T> cumprod(const array<T> &v);
```

### `cumsum` 
Returns the cumulative sum of the elements in an array.
```cpp
template <class T>
array<T> cumprod(const array<T> &v);
```

### `dot` 

Returns the dot product of two arrays.
```cpp
template <class T>
T dot(const array<T> &v, const array<T> &w);
```

### `empty`
Returns a new uninitialized array with `n` elements.
```cpp
template<class T = double> 
array<T> empty(size_t n);
```

### `erase`

Erase values from an array based on its position.
```cpp
template <class T>
array<T> erase(const array<T> &v, size_t index);

template <class T>
array<T> erase(const array<T> &v, const array<size_t> indices);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {3., -1., 7., 5., 2., 10., -3., 2., 1., 8.};
    cout << v << "\n";
    cout << np::erase(v, 2) << "\n";
    cout << np::erase(v, {0, 5, 2, 9}) << "\n";
    return 0;
}
```

```
[Out] [3, -1, 7, 5, 2, 10, -3, 2, 1, 8]
      [3, -1, 5, 2, 10, -3, 2, 1, 8]
      [-1, 5, 2, -3, 2, 1]
```

### `full` 
Returns a new array of length `n` setting values to `val`.
```cpp
template <class T = double> 
array<T> full(size_t n, const T &val);
```

### `geomspace` 

Returns a new array with numbers on a geometric progression. Each output sample 
is a constant multiple of the previous. The endpoint of the interval can 
optionally be excluded.
```cpp
template <class T>
array<T> geomspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true
)
```

### `insert`

Insert values in an array before the given indices.
```cpp
template <class T>
array<T> insert(const array<T> &v, size_t index, const T &value);

template <class T>
array<T> insert(
    const array<T> &v, const array<size_t> &indices, const array<T> &values
);
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {3., -1., 7., 5., 2., 10.};
    cout << v << "\n";
    cout << np::insert(v, 1, 8.1) << "\n";
    cout << np::insert(v, {0, 5, 2, 6}, {0.1, 1.2, -0.5, 3.5}) << "\n";
    return 0;
}
```

```
[Out] [3, -1, 7, 5, 2, 10]
      [3, 8.1, -1, 7, 5, 2, 10]
      [0.1, 3, -1, -0.5, 7, 5, 2, 1.2, 10, 3.5]
```

### `linspace`

Returns a new array with evenly spaced numbers over a specified interval. 
Returns `num` evenly spaced samples, calculated over the closed interval 
`[start, stop]`. The endpoint of the interval can optionally be excluded.
```cpp
template <class T> 
array<T> linspace(
    const T &start, const T &stop, 
    size_t num = 50, 
    bool endpoint = true
)
```

### `load` 

Returns a new array from a binary file. The function throws a `runtime_error` 
exception if the file passed as an argument does not exist.
```cpp
template <class T>
array<T> load(const char *file);
```

### `logspace` 

Returns a new array with evenly spaced numbers on a log scale. In the linear 
space, the sequence starts at `pow(base, start)` (`base` to the power of 
`start`) and ends with `pow(base, stop)`. The endpoint of the interval can 
optionally be excluded. By default, the base is 10.
```cpp
template <class T>
array<T> logspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true,
    const T &base = 10.0
)
```

### `max` 

Returns the maximum value contained in the array.
```cpp
template <class T>
T max(const array<T> &v);
```

### `mean` 

Returns the mean or average of the array elements.
```cpp
template <class T>
T mean(const array<T> &v);
```

### `min` 

Returns the minimum value contained in the array.
```cpp
template <class T>
T min(const array<T> &v);
```

### `ones` 
Returns a new array of length `n` setting values to one.
```cpp
template<class T = double> 
array<T> ones(size_t n);
```

### `prod` 

Returns the product of the array elements.
```cpp
template <class T>
T prod(const array<T> &v); 
```

### `save`

Save an array's content into a binary file. The function throws a 
`runtime_error` exception if the file passed as an argument could not be 
opened.
```cpp
template <class T>
void save(const char *file, const array<T> &v);
```

### `sort` 

Returns a copy with the sorted elements of an array. Contrary to the `v.sort()` 
method, this function does not modify the elements of `v`.
```cpp
template <class T>
array<T> sort(const array<T> &v);
```

### `stddev` 

Returns the standard deviation of the array elements with `ddof` degrees of 
freedom. By default, `ddof = 0`.
```cpp
template <class T>
T stddev(const array<T> &v, size_t ddof = 0);
```

### `sum` 

Returns the sum of the array elements.
```cpp
template <class T>
T sum(const array<T> &v);
```

### `swap` 

Swap contents between two arrays. This function is implemented to work in 
constant time.
```cpp
template <class T>
void swap(array<T> &v, array<T> &w);
```
### `var` 

Returns the variance of the array elements with `ddof` degrees of freedom. By 
default, `ddof = 0`.
```cpp
template <class T>
T var(const array<T> &v, size_t ddof = 0);
```

### `where`

In its first version, returns an array of indices with the positions where 
`condition` evaluate to `true`.

In its second version, returns an array choosing from the elements in 
`expr_true` at the positions where `condition` evaluate to `true`.

In its third version, returns an array choosing from the elements in 
`expr_true` at the positions where `condition` evaluate to `true` and from the 
elements in `expr_false` at the positions where `condition` evaluate to `false`.
```cpp
array<size_t> where(const array<bool> &condition);

template <class T>
array<T> where(const array<bool> &condition, const array<T> &expr_true);

template <class T>
array<T> where(
    const array<bool> &condition,
    const array<T> &expr_true, const array<T> &expr_false
);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, -9, 5, 10, -2, 4, -1, 7, 3, 0};
    cout << np::where(v > 0) << "\n";
    cout << np::where(v > 0, v) << "\n";
    cout << np::where(v > 0, v, -v) << "\n";
    return 0;
}
```

```
[Out] [0, 2, 3, 5, 7, 8]
      [1, 5, 10, 4, 7, 3]
      [1, 9, 5, 10, 2, 4, 1, 7, 3, 0]
```

### `zeros` 
Returns a new array of length `n` setting values to zero.
```cpp
template<class T = double> 
array<T> zeros(size_t n);
```