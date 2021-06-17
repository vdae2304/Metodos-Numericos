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
    return 0;
}
```

```
[Out] 
```

## Advanced indexing

## Methods list

## Global functions
