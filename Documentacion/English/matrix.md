# Matrices

## Content

- [Constructors](#Constructors)
- [Destructor](#Destructor)
- [Matrix creation routines](#Matrix-creation-routines)
- [Assignment operators](#Assignment-operators)
- [Operator overloading](#Operator-overloading)
- [Basic indexing](#Basic-indexing)
- [Advanced indexing](#Advanced-indexing)
- [Methods list](#Methods-list)
- [Global functions](#Global-functions)

<hr>

Matrices are fixed-size two dimensional sequence containers: they hold a 
specific number of elements arranged in rows and columns.

To create a matrix, first include the `numcpp` header at the beginning of each 
program. Matrices will be represented by the `matrix` class.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    return 0;
}
```

```
[Out] 
```

## Constructors

1. Default constructor. Constructs an empty matrix with no elements.
```cpp
matrix();
```

2. Size constructor. Constructs a matrix with `m` rows and `n` columns.
```cpp
matrix(size_t m, size_t n);
```

3. Fill constructor. Constructs a matrix with `m` rows and `n` columns, each 
element initialized to `val`.
```cpp
matrix(size_t m, size_t n, const T &val);
```

4. Container constructor. Constructs a matrix with `m` rows and `n` columns, 
each element initialized to its corresponding element in the container, in the
same order.
```cpp
template <class InputIterator> 
matrix(size_t m, size_t n, InputIterator first);
```

5. Copy constructor. Constructs a matrix with a copy of each of the elements in 
`A`, in the same order.
```cpp
matrix(const matrix &A);
```

6. Move constructor. Constructs a matrix that acquires the elements of  `A`.
```cpp
matrix(matrix &&v);
```

7. Initializer list of lists. Constructs a matrix with a copy of each of the elements in `ill`, in the same order.
```cpp
matrix(std::initializer_list< std::initializer_list<T> > ill);
```

## Destructor

Destroys the matrix.
```cpp
~matrix();
```

## Matrix creation routines

### Ones and zeros

- **empty**: Returns a new uninitialized matrix with `m` rows and `n` columns.
```cpp
template<class T = double> 
matrix<T> empty(size_t m, size_t n);
```

- **zeros**: Returns a new matrix with `m` rows and `n` columns setting values 
to zero.
```cpp
template<class T = double> 
matrix<T> zeros(size_t m, size_t n);
```

- **ones**: Returns a new matrix with `m` rows and `n` columns setting values 
to one.
```cpp
template<class T = double> 
matrix<T> ones(size_t m, size_t n);
```

- **full**: Returns a new matrix with `m` rows and `n` columns setting values 
to `val`.
```cpp
template <class T = double> 
matrix<T> full(size_t m, size_t n, const T &val);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> empty = np::empty<int>(3, 5);
    np::matrix<int> zeros = np::zeros<int>(3, 5);
    np::matrix<int> ones = np::ones<int>(3, 5);
    np::matrix<int> twos = np::full<int>(3, 5, 2);
    cout << "empty:\n" << empty << "\n";
    cout << "zeros:\n" << zeros << "\n";
    cout << "ones:\n" << ones << "\n";
    cout << "twos:\n" << twos << "\n";
    return 0;
}
```

```
[Out] empty:
      [[ 7477984,     0, 7471440, 0,       0]
       [       0,     0,       0, 0,       0]
       [33554434, 50269, 7477984, 0, 7471440]]
      zeros:
      [[0, 0, 0, 0, 0]
       [0, 0, 0, 0, 0]
       [0, 0, 0, 0, 0]]
      ones:
      [[1, 1, 1, 1, 1]
       [1, 1, 1, 1, 1]
       [1, 1, 1, 1, 1]]
      twos:
      [[2, 2, 2, 2, 2]
       [2, 2, 2, 2, 2]
       [2, 2, 2, 2, 2]]
```

### From a text file

- **load_txt**: Returns a new matrix from a text file. The delimiter that 
separates values in the same row can optionally be specified. By default, the 
delimiter is a white space. Additionally it can be specified if the first row 
of the file is a header. If so, the header's values can be saved into a 
`string` array. The function throws  a `runtime_error` exception if the file 
passed as an argument does not exist.
```cpp
template <class T>
matrix<T> load_txt(
    const char *file,
    char delimiter = ' ',
    bool header = false
);

template <class T>
matrix<T> load_txt(
    const char *file,
    char delimiter,
    bool header,
    array<std::string> &names
);
```

- **save_txt**: Save a matrix's content into a text file. The delimiter that 
will separate values in the same row can optionally be specified. By default, 
the delimiter is a white space. Aditionaly the file's header can be specified. 
The function throws a `runtime_error` exception if the file passed as an argument 
could not be opened.
```cpp
template <class T>
void save_txt(const char *file, const matrix<T> &A, char delimiter = ' ');

template <class T>
void save_txt(
    const char *file,
    const matrix<T> &A,
    char delimiter,
    const array<std::string> &names
);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<string> cols = {"x", "y"};
    np::matrix<int> A = {{1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}};
    np::save_txt("squares.csv", A, ',', cols);

    np::array<string> cols2;
    np::matrix<int> B = np::load_txt<int>("squares.csv", ',', true, cols2);
    cout << cols2 << "\n" << B << "\n";

    return 0;
}
```

```
[Out] [x, y]
      [[1,  1]
       [2,  4]
       [3,  9]
       [4, 16]
       [5, 25]]
```

```
[squares.csv]
x,y
1,1
2,4
3,9
4,16
5,25
```

### From a binary file

- **load_matrix**: Returns a new matrix from a binary file. The function throws 
a `runtime_error` exception if the file passed as an argument does not exist.
```cpp
template <class T>
matrix<T> load_matrix(const char *file);
```

- **save_matrix**: Save a matrix's content into a binary file. The function 
throws a `runtime_error` exception if the file passed as an argument could not 
be opened.
```cpp
template <class T>
void save_matrix(const char *file, const matrix<T> &A);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{1, 2, 3, 4, 5},
                         {6, 7, 8, 9, 10}};
    np::save_matrix("numbers-1-to-10.bin", A);
    np::matrix<int> B = np::load_matrix<int>("numbers-1-to-10.bin");
    cout << B << "\n";
    return 0;
}
```

```
[Out] [[1, 2, 3, 4,  5]
       [6, 7, 8, 9, 10]]
```

## Assignment operators

1. Copy assignment. Assigns the contents of `A` to `*this` after resizing the
matrix (if necessary).
```cpp
matrix& operator= (const matrix &A);
```

2. Move assignment. Acquires the contents of `A`.
```cpp
matrix& operator= (matrix &&A);
```

3. Fill assignment. Assigns `val` to every element.
```cpp
matrix& operator= (const T &val);
```

## Operator overloading

Each of this functions performs their respective operation on all the elements 
in the matrix.

When both the left-hand side argument and the right-side one are matrix objects, 
the operation is performed between the corresponding elements in each object 
(the first element of the left one with the first of the right one, the second with the second, and so on). If the matrix's shapes differs, the operator 
throws a `runtime_error`.

When one of the arguments is a value, the operation is applied to all the 
elements in the matrix against that value.

### Member functions

```cpp
matrix& operator+= (const matrix &A);
matrix& operator-= (const matrix &A);
matrix& operator*= (const matrix &A);
matrix& operator/= (const matrix &A);
matrix& operator%= (const matrix &A);
matrix& operator&= (const matrix &A);
matrix& operator|= (const matrix &A);
matrix& operator^= (const matrix &A);
matrix& operator<<= (const matrix &A);
matrix& operator>>= (const matrix &A);

matrix& operator+= (const T &val);
matrix& operator-= (const T &val);
matrix& operator*= (const T &val);
matrix& operator/= (const T &val);
matrix& operator%= (const T &val);
matrix& operator&= (const T &val);
matrix& operator|= (const T &val);
matrix& operator^= (const T &val);
matrix& operator<<= (const T &val);
matrix& operator>>= (const T &val);
```

### Unitary operators

```cpp
template <class T> matrix<T> operator+ (const matrix<T> &A);
template <class T> matrix<T> operator- (const matrix<T> &A);
template <class T> matrix<T> operator~ (const matrix<T> &A);
matrix<bool> operator! (const matrix<bool> &A);
```

### Non-member functions

```cpp
template<class T> matrix<T> operator+ (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator+ (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator+ (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator- (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator- (const matrix<T> &A, const T &val)
template<class T> matrix<T> operator- (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator* (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator* (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator* (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator/ (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator/ (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator/ (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator% (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator% (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator% (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator& (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator& (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator& (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator| (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator| (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator| (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator^ (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator^ (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator^ (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator<< (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator<< (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator<< (const T &val, const matrix<T> &A);

template<class T> matrix<T> operator>> (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<T> operator>> (const matrix<T> &A, const T &val);
template<class T> matrix<T> operator>> (const T &val, const matrix<T> &A);

template<class T> matrix<bool> operator== (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<bool> operator== (const matrix<T> &A, const T &val);
template<class T> matrix<bool> operator== (const T &val, const matrix<T> &A);

template<class T> matrix<bool> operator!= (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<bool> operator!= (const matrix<T> &A, const T &val);
template<class T> matrix<bool> operator!= (const T &val, const matrix<T> &A);

template<class T> matrix<bool> operator< (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<bool> operator< (const matrix<T> &A, const T &val);
template<class T> matrix<bool> operator< (const T &val, const matrix<T> &A);

template<class T> matrix<bool> operator> (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<bool> operator> (const matrix<T> &A, const T &val);
template<class T> matrix<bool> operator> (const T &val, const matrix<T> &A);

template<class T> matrix<bool> operator<= (const matrix<T> &A, const matrix<T> &B)
template<class T> matrix<bool> operator<= (const matrix<T> &A, const T &val);
template<class T> matrix<bool> operator<= (const T &val, const matrix<T> &A);

template<class T> matrix<bool> operator>= (const matrix<T> &A, const matrix<T> &B);
template<class T> matrix<bool> operator>= (const matrix<T> &A, const T &val);
template<class T> matrix<bool> operator>= (const T &val, const matrix<T> &A);
```

### Input/Output
```cpp
template <class T>
std::ostream& operator<< (std::ostream &ostr, const matrix<T> &A);
```
## Basic indexing

### Size

- **rows**: Returns the number of rows in the matrix.
```cpp
size_t rows() const;
```

- **columns**: Returns the number of columns in the matrix.
```cpp
size_t columns() const;
```

### Indexing

There are two main approaches to access the elements in a matrix. The first 
approach is using the `[]` operator with an index `i`, which returns a pointer 
to the beginning of the `i`-th row. By doing so, using the `[]` operator a 
second time with an index `j` will return a reference to the element at the 
`i`-th row and `j`-th column.
```cpp
T* operator[] (size_t i);
const T* operator[] (size_t i) const;
```
The second approach is using the `at` method with two indices `i` and `j`, 
which returns a reference to the element at the `i`-th row and `j`-th column.
```cpp
T& at(size_t i, size_t j);
const T& at(size_t i, size_t j) const;
```
Notice that the indices starts from 0, no 1. Accessing to elements outside the 
matrix cause undefined behaviour.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = np::zeros(3, 5);
    cout << "A has " << A.rows() << " rows";
    cout << " and " << A.columns() << " columns.\n";
    A[0][2] = 1.5;
    A[0][3] = 10.0;
    A[1][1] = -2.1;
    A.at(2, 0) = 0.1;
    A.at(2, 4) = 3.7;
    cout << "A's elements are:\n" << A << "\n";
    return 0;
}
```

```
[Out] A has 3 rows and 5 columns.
      A's elements are:
      [[  0,    0, 1.5, 10,   0]
       [  0, -2.1,   0,  0,   0]
       [0.1,    0,   0,  0, 3.7]]
```

## Advanced indexing

### Sub-matrices

A sub-matrix is an intermediate class returned by the matrix's `at` method. It 
references the elements in the matrix that are selected by the subscripts, and 
overloads the assignment and compound assignment operators, allowing direct 
access to the elements in the selection. The object is convertible to a matrix, 
producing a new object with copies of the referred elements.
```cpp
template <class T>
class submatrix {
public:
    void operator= (const matrix<T> &A);
    void operator= (const T &val);

    void operator+= (const matrix<T> &A);
    void operator-= (const matrix<T> &A);
    void operator*= (const matrix<T> &A);
    void operator/= (const matrix<T> &A);
    void operator%= (const matrix<T> &A);
    void operator&= (const matrix<T> &A);
    void operator|= (const matrix<T> &A);
    void operator^= (const matrix<T> &A);
    void operator<<= (const matrix<T> &A);
    void operator>>= (const matrix<T> &A);

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

    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;

    size_t columns() const;
    matrix<T> copy() const;
    size_t rows() const;
};
```
The `at` method accepts `size_t`, `slice`, `array<size_t>` and `array<bool>` 
objects on each axe.

The first one respresents a selection of elements over a single row/column 
through an index.

The second one represents a selection of elements through a slice. A slice is 
defined by a starting index (`start`), a stop index (`stop`) and a step size 
(`step`). For example, `slice(3, 20, 5)` selects the elements with indices 3, 
8, 13 and 18. By default, the starting index is 0 and the step size is 1.

The third one represents a selection of elements through an array of indices.

The fourth one represents a selecion of elements through a boolean mask.

#### Example
```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{7, 0, -1, 14, 3, 0},
                         {5, 3, 10, 2, 0, -1},
                         {0, 1, 7, -5, 9, 3},
                         {1, 5, 0, 0, -2, 9}};
    cout << "A's elements are:\n" << A << "\n";

    // Select rows 0, 1, 2, 3 and columns 1, 5.
    np::submatrix<int> sub = A.at(np::slice(4), np::array<size_t> {1, 5});
    cout << "The submatrix has " << sub.rows() << " rows";
    cout << " and " << sub.columns() << " columns.\n";
    cout << "The submatrix elements are:\n" << sub.copy() << "\n";
    sub = 0;
    cout << "A's elements are now:\n" << A << "\n";

    // Select row 1 and columns 1, 3, 5.
    A.at(1, np::slice(1, 6, 2)) += 10;
    cout << "A's elements are now:\n" << A << "\n";

    // Select rows 1, 2 and columns 0, 1, 4.
    A.at(np::array<size_t> {1, 2}, np::array<bool> {1, 1, 0, 0, 1, 0}) *= 3;
    cout << "A's elements are now:\n" << A << "\n";

    return 0;
}
```

```
[Out] A's elements are:
      [[7, 0, -1, 14,  3,  0]
       [5, 3, 10,  2,  0, -1]
       [0, 1,  7, -5,  9,  3]
       [1, 5,  0,  0, -2,  9]]
      The submatrix has 4 rows and 2 columns.
      The submatrix elements are:
      [[0,  0]
       [3, -1]
       [1,  3]
       [5,  9]]
      A's elements are now:
       [[7, 0, -1, 14,  3, 0]
        [5, 0, 10,  2,  0, 0]
        [0, 0,  7, -5,  9, 0]
        [1, 0,  0,  0, -2, 0]]
       A's elements are now:
       [[7,  0, -1, 14,  3,  0]
        [5, 10, 10, 12,  0, 10]
        [0,  0,  7, -5,  9,  0]
        [1,  0,  0,  0, -2,  0]]
       A's elements are now:
       [[ 7,  0, -1, 14,  3,  0]
        [15, 30, 10, 12,  0, 10]
        [ 0,  0,  7, -5, 27,  0]
        [ 1,  0,  0,  0, -2,  0]]
```

## Methods list

### `apply` 

Replaces each element in the matrix with the result of applying a function to 
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
    np::matrix<double> A = {{1., -9., 5.}, {10., -2., 7.}};
    A.apply(square);
    cout << A << "\n";
    return 0;
}
```

```
[Out] [[  1, 81, 25]
       [100,  4, 49]]
```

### `argmax`

In its first version, returns the index of the maximum value contained in the 
matrix. In its second version, returns the index of the maximum value contained  
in the matrix along the given axis.
```cpp
std::pair<size_t, size_t> argmax() const;
array<size_t> argmax(size_t axis) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{3., 0., 5.},
                            {-4.1, 2.5, 1.}};
    cout << "(" << A.argmax().first << ", " << A.argmax().second << ")\n";
    cout << A.argmax(0) << "\n";
    cout << A.argmax(1) << "\n";
    return 0;
}
```

```
[Out] (0, 2)
      [0, 1, 0]
      [2, 1]
```

### `argmin`

In its first version, returns the index of the minimum value contained in the 
matrix. In its second version, returns the index of the minimum value contained 
in the matrix along the given axis.
```cpp
std::pair<size_t, size_t> argmin() const;
array<size_t> argmin(size_t axis) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{3., 0., 5.},
                            {-4.1, 2.5, 1.}};
    cout << "(" << A.argmin().first << ", " << A.argmin().second << ")\n";
    cout << A.argmin(0) << "\n";
    cout << A.argmin(1) << "\n";
    return 0;
}
```

```
[Out] (1, 0)
      [1, 0, 1]
      [1, 0]
```

### `astype` 

Returns a copy of the matrix, cast to a specified type.
```cpp
template <class U> matrix<U> astype() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., 2.5, 3.}, {-4.1, 0., 5.1}};
    np::matrix<int> B = A.astype<int>();
    cout << B << "\n";
    return 0;
}
```

```
[Out] [[ 1, 2, 3]
       [-4, 0, 5]]
```

### `clip` 

Clip (limit) the values in the matrix. Given an interval, values outside the 
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
    np::matrix<double> A = {{1., -9., 3.}, {10., -2., 7.}};
    A.clip(0., 5.);
    cout << A << "\n";
    return 0;
}
```

```
[Out] [[1, 0, 3]
       [5, 0, 5]]
```

### `columns` 

Returns the number of columns in the matrix.
```cpp
size_t columns() const;
```

### `data`

Returns a pointer to the memory array used internally by the matrix.
```cpp
T* data();
const T* data() const;
```

### `dot`

In its first version, returns the matrix-vector multiplication between `*this` 
and `v`. In its second version, returns the matrix multiplication between 
`*this` and `A`.
```cpp
array<T> dot(const array<T> &v) const;
matrix dot(const matrix &A) const;
```
The matrix multiplication between two matrices *A* and *B* is defined as the 
matrix whose elements are

![$c_{ij} = \sum_k a_{ik}b_{kj}$](https://render.githubusercontent.com/render/math?math=c_%7Bij%7D%20%3D%20%5Csum_k%20a_%7Bik%7Db_%7Bkj%7D)

This is, the *(i, j)* entry is the dot product between the *i*-th row of *A* 
and the *j*-th column of *B*.

The method throws a `runtime_error` exception if the number of columns in 
`this` does not match the number of elements in `v` or the number of rows in 
`A`.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., 3.},
                            {-2., 5.},
                            {7., 0.}};
    np::array<double> v = {1., -1.};
    np::matrix<double> B = {{1., 1.},
                            {-1., 1.}};
    cout << A.dot(v) << "\n";
    cout << A.dot(B) << "\n";
    return 0;
}
```

```
[Out] [-2, -7, 7]
      [[-2, 4]
       [-7, 3]
       [ 7, 7]]
```

### `flatten`

Returns a copy of the matrix collapsed into an array.
```cpp
array<T> flatten() const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., 2.5, 3.},
                            {-4.1, 0., 5.1}};
    cout << A.flatten() << "\n";
    return 0;
}
```

```
[Out] [1, 2.5, 3, -4.1, 0, 5.1]
```

### `max`

In its first version, returns the maximum value contained in the matrix. In its 
second version, returns the maximum value contained in the matrix along the 
given axis.
```cpp
T max() const;
array<T> max(size_t axis) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{3., 0., 5.},
                            {-4.1, 2.5, 1.}};
    cout << A.max() << "\n";
    cout << A.max(0) << "\n";
    cout << A.max(1) << "\n";
    return 0;
}
```

```
[Out] 5
      [3, 2.5, 5]
      [5, 2.5]
```

### `mean` 

In its first version, returns the mean or average of the matrix elements. In 
its second version, returns the mean or average of the matrix elements along 
the given axis.
```cpp
T mean() const;
array<T> mean(size_t axis) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., -3., 2., 7.},
                            {0., 5., 2., -1.},
                            {5., 1., 5., 3.}};
    cout << A.mean() << "\n";
    cout << A.mean(0) << "\n";
    cout << A.mean(1) << "\n";
    return 0;
}
```

```
[Out] 2.25
      [2, 1, 3, 3]
      [1.75, 1.5, 3.5]
```

### `min`

In its first version, returns the minimum value contained in the matrix. In its 
second version, returns the minimum value contained in the matrix along the 
given axis.
```cpp
T min() const;
array<T> min(size_t axis) const;
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{3., 0., 5.},
                            {-4.1, 2.5, 1.}};
    cout << A.min() << "\n";
    cout << A.min(0) << "\n";
    cout << A.min(1) << "\n";
    return 0;
}
```

```
[Out] -4.1
      [-4.1, 0, 1]
      [0, -4.1]
```

### `prod`

In its first version, returns the product of the matrix elements. In its second 
version, returns the product of the matrix elements along the given axis.
```cpp
T prod() const;
array<T> prod(size_t axis) const;
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{2, 3, 5},
                         {1, 4, 2}};
    cout << A.prod() << "\n";
    cout << A.prod(0) << "\n";
    cout << A.prod(1) << "\n";
    return 0;
}
```

```
[Out] 240
      [2, 12, 10]
      [30, 8]
```

### `resize`

Resize the matrix, changing its size to `m` rows and `n` columns. 

If the total number of elements in the matrix remains constant, the content of 
the matrix is rearranged to fit the new shape.

If the total number of elements in the matrix decreases, the content of the 
matrix is reduced by removing from the end as many elements as needed to reach 
the new size.

If the total number of elements in the matrix increases, the content of the 
matrix is expanded by inserting at the end as many elements as needed to reach 
the new size.
```cpp
void resize(size_t m, size_t n, const T &val = T());
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., -9.}, {5., 10.}, {-2., 7.}};
    cout << "Before:\n" << A << "\n";
    A.resize(1, 6, 0.);
    cout << "After rearranging:\n" << A << "\n";
    A.resize(4, 2, 0.);
    cout << "After expanding:\n" << A << "\n";
    A.resize(2, 2, 0.);
    cout << "After reducing:\n" << A << "\n";
    return 0;
}
```

```
[Out] Before:
      [[ 1, -9]
       [ 5, 10]
       [-2,  7]]
      After rearranging:
      [[1, -9, 5, 10, -2, 7]]
      After expanding:
      [[ 1, -9]
       [ 5, 10]
       [-2,  7]
       [ 0,  0]]
      After reducing:
      [[1, -9]
       [5, 10]]
```

### `rows` 

Returns the number of rows in the matrix.
```cpp
size_t rows() const;
```

### `stddev` 

In its first version, returns the standard deviation of the matrix elements. In 
its second version, returns the standard deviation of the matrix elements along 
the given axis.
```cpp
T stddev(size_t ddof = 0) const;
array<T> stddev(size_t ddof, size_t axis) const;
```
The standard deviation is defined as

![$\sqrt{\frac{1}{n - ddof}\sum_{i = 1}^{n} (x_i - \bar{x})^2}$](https://render.githubusercontent.com/render/math?math=%5Csqrt%7B%5Cfrac%7B1%7D%7Bn%20-%20ddof%7D%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20(x_i%20-%20%5Cbar%7Bx%7D)%5E2%7D)

where 
![$\bar{x}$](https://render.githubusercontent.com/render/math?math=%5Cbar%7Bx%7D) 
is the mean and 
![$\bar{x}$](https://render.githubusercontent.com/render/math?math=ddof) 
are the degrees of freedom.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., -3., 2., 7.},
                            {0., 5., 2., -1.},
                            {5., 1., 5., 3.}};
    cout << A.stddev(0) << "\n";
    cout << A.stddev(0, 0) << "\n";
    cout << A.stddev(0, 1) << "\n";
    return 0;
}
```

```
[Out] 2.77263
      [2.1602469, 3.2659863, 1.4142136, 3.2659863]
      [3.5619517, 2.2912878, 1.6583124]
```

### `sum`

In its first version, returns the sum of the matrix elements. In its second 
version, returns the sum of the matrix elements along the given axis.
```cpp
T sum() const;
array<T> sum(size_t axis) const;
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{2, 3, 5},
                         {1, 4, 2}};
    cout << A.sum() << "\n";
    cout << A.sum(0) << "\n";
    cout << A.sum(1) << "\n";
    return 0;
}
```

```
[Out] 17
      [3, 7, 7]
      [10, 7]
```

### `var` 

In its first version, returns the variance of the matrix elements. In its 
second version, returns the variance of the matrix elements along the given 
axis.
```cpp
T var(size_t ddof = 0) const;
array<T> var(size_t ddof, size_t axis) const;
```
The variance is defined as

![$S^2 = \frac{1}{n - ddof}\sum_{i = 1}^{n} (x_i - \bar{x})^2$](https://render.githubusercontent.com/render/math?math=S%5E2%20%3D%20%5Cfrac%7B1%7D%7Bn%20-%20ddof%7D%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20(x_i%20-%20%5Cbar%7Bx%7D)%5E2)

where 
![$\bar{x}$](https://render.githubusercontent.com/render/math?math=%5Cbar%7Bx%7D) 
is the mean and 
![$\bar{x}$](https://render.githubusercontent.com/render/math?math=ddof) 
are the degrees of freedom.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., -3., 2., 7.},
                            {0., 5., 2., -1.},
                            {5., 1., 5., 3.}};
    cout << A.var(0) << "\n";
    cout << A.var(0, 0) << "\n";
    cout << A.var(0, 1) << "\n";
    return 0;
}
```

```
[Out] 7.6875
      [4.6666667, 10.666667, 2, 10.666667]
      [12.6875, 5.25, 2.75]
```

## Global functions