# Linear Algebra

## Content

- [Basic functions](#Basic-functions)
- [Decompositions](#Decompositions)

## Basic functions

### `solve_triangular`

Solves the equation ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
for *x* assuming *A* is a triangular matrix. 
```cpp
template <class T>
numcpp::array<T> solve_triangular(
    const numcpp::matrix<T> &A,
    const numcpp::array<T> &b,
    bool lower,
    bool transpose = false,
    bool unit_diagonal = false
);

template <class T>
numcpp::matrix<T> solve_triangular(
    const numcpp::matrix<T> &A,
    const numcpp::matrix<T> &b,
    bool lower,
    bool transpose = false,
    bool unit_diagonal = false
);
```
If `lower` is `true`, uses only the elements in the lower triangle of *A*. 
Otherwise, uses only the elements in the upper triangle of *A*. If `transpose` 
is `true`, solves instead the equation 
![$A^Tx = b$](https://render.githubusercontent.com/render/math?math=A%5ETx%20%3D%20b).
By default, `transpose = false`. If `unit_diagonal` is `true`, the diagonal 
elements of *A* are assumed to be ones and hence will not be referenced. 
By default, `unit_diagonal = false`.

The function throws a `LinAlgError` exception if the matrix is singular.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 2, 0, 0, 0},
                            { 5, 4, 0, 0},
                            {-2, 1, 8, 0},
                            { 7, 0, 5, 2}};
    np::array<double> b = {0, 3, 5, 1};
    np::array<double> x = scicpp::solve_triangular(A, b, true);
    cout << boolalpha << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    A = {{2, 0, -3, 5},
         {0, 4,  2, 0},
         {0, 0,  8, 3},
         {0, 0,  0, 2}};
    b = {0, 3, 5, 1};
    x = scicpp::solve_triangular(A, b, false);
    cout << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] true
      x = [0, 0.75, 0.53125, -0.828125]
      true
      x = [-0.59375, 0.53125, 0.4375, 0.5
```

### `solve`

Solves the equation ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
for *x*. 

If the matrix is known to have a specific structure, then supplying a value for 
the `assume_a` parameter will indicate how the equation should be solved. The 
possible values for `assume_a` are: `"gen"` representing a generic matrix 
(default value); `"sym"` representing a symmetric matrix; and `"pos"` 
representing a symmetric positive-definite matrix.


The function throws a `LinAlgError` if the matrix is singular.
```cpp
template <class T>
numcpp::array<T> solve(
    const numcpp::matrix<T> &A,
    const numcpp::array<T> &b,
    const std::string &assume_a = "gen"
);

template <class T>
numcpp::matrix<T> solve(
    const numcpp::matrix<T> &A,
    const numcpp::matrix<T> &b,
    const std::string &assume_a = "gen"
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
    np::matrix<double> A = {{ 7, 3, -1,  0},
                            { 1, 4,  1, -1},
                            {-2, 3,  0,  8},
                            { 5, 2,  8,  5}};
    np::array<double> b = {-1, 0, 5, 2};
    np::array<double> x = scicpp::solve(A, b);
    cout << boolalpha << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";
    return 0;
}
```

```
[Out] true
      x = [-0.21405604, 0.17409279, 0.023886082, 0.50620119]
```

### `inv`

Computes the inverse of a matrix.
```cpp
template <class T>
numcpp::matrix<T> inv(const numcpp::matrix<T> &A);
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, 1,  5},
                            { 1, 5, 2,  2},
                            {-2, 1, 0, -1},
                            { 5, 2, 4,  0}};
    np::matrix<double> B = scicpp::inv(A);
    np::matrix<double> I = np::eye<double>(4, 4);
    cout << boolalpha << np::allclose(A.dot(B), I) << "\n";
    cout << np::allclose(B.dot(A), I) << "\n";
    cout << B << "\n";
    return 0;
}
```

```
[Out] true
      true
      [[ 0.38095238, -0.47619048, 0.95238095,  0.14285714]
       [ 0.34920635, -0.26984127,  1.2063492, 0.047619048]
       [-0.65079365,  0.73015873, -1.7936508, 0.047619048]
       [-0.41269841,  0.68253968, -1.6984127, -0.23809524]]
```

### `det`

Computes the determinant of a matrix.
```cpp
template <class T>
T det(const numcpp::matrix<T> &A);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, 1,  5},
                            { 1, 5, 2,  2},
                            {-2, 1, 0, -1},
                            { 5, 2, 4,  0}};
    cout << scicpp::det(A) << "\n";
    return 0;
}

```

```
[Out] 63
```

## Decompositions

### `lu`

Computes the pivoted LU decomposition of a matrix. The decomposition is

![$PA = LU$](https://render.githubusercontent.com/render/math?math=PA%20%3D%20LU) 

where *P* is a permutation matrix, *L* is a lower triangular matrix with unit 
diagonal elements, and *U* is an upper triangular matrix.
```cpp
template <class T>
void lu(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &P,
    numcpp::matrix<T> &L,
    numcpp::matrix<T> &U
)
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 2, 0, -3, 5},
                            { 5, 4,  2, 0},
                            {-2, 1,  8, 3},
                            { 7, 0,  5, 2}};
    np::matrix<double> P, L, U;
    scicpp::lu(A, P, L, U);
    cout << boolalpha << np::allclose(P.dot(A), L.dot(U)) << "\n";
    cout << "P:\n" << P << "\nL:\n" << L << "\nU:\n" << U << "\n";
    return 0;
}
```

```
[Out] true
      P:
      [[0, 0, 0, 1]
       [0, 1, 0, 0]
       [0, 0, 1, 0]
       [1, 0, 0, 0]]
      L:
      [[          1,    0,           0, 0]
       [ 0.71428571,    1,           0, 0]
       [-0.28571429, 0.25,           1, 0]
       [ 0.28571429,    0, -0.45090909, 1]]
      U:
      [[7, 0,          5,          2]
       [0, 4, -1.5714286, -1.4285714]
       [0, 0,  9.8214286,  3.9285714]
       [0, 0,          0,        6.2]]
```

### `lu_factor`

Computes the pivoted LU decomposition of a matrix. The matrix elements of *L* 
and *U* are stored in a same matrix `LU`. The pivot indices representing the 
permutation matrix *P* are stored in an array `piv`.
```cpp
template <class T>
void lu_factor(
    const numcpp::matrix<T> &A,
    numcpp::array<size_t> &piv,
    numcpp::matrix<T> &LU
)
```

### `lu_solve`

Solves the equation ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
for *x* given the LU decomposition of *A* (as given by `lu_factor`). The 
function throws a `LinAlgError` exception if the matrix is singular.
```cpp
template <class T>
numcpp::array<T> lu_solve(
    const numcpp::array<size_t> &piv,
    const numcpp::matrix<T> &LU,
    const numcpp::array<T> &b
);

template <class T>
numcpp::matrix<T> lu_solve(
    const numcpp::array<size_t> &piv,
    const numcpp::matrix<T> &LU,
    const numcpp::matrix<T> &b
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
    np::matrix<double> A = {{ 2, 0, -3, 5},
                            { 5, 4,  2, 0},
                            {-2, 1,  8, 3},
                            { 7, 0,  5, 2}};
    np::array<double> b = {0, 3, 5, 1};

    np::array<size_t> piv;
    np::matrix<double> LU;
    scicpp::lu_factor(A, piv, LU);
    
    np::array<double> x = scicpp::lu_solve(piv, LU, b);
    cout << boolalpha << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] true
      x = [-0.2, 0.81935484, 0.36129032, 0.29677419]
```

### `ldl`

Computes the LDL decomposition of a symmetric matrix. The decomposition is

![$A = LDL^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20LDL%5ET) 

where *L* is a lower triangular matrix with unit diagonal elements and *D* is 
a diagonal matrix.
```cpp
template <class T>
void ldl(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &L,
    numcpp::array<T> &D
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
    np::matrix<double> A = {{ 2,  5, -2,  7},
                            { 5,  4,  1, -1},
                            {-2,  1,  8,  3},
                            { 7, -1,  3,  2}};
    np::matrix<double> L;
    np::array<double> D;
    scicpp::ldl(A, L, D);
    cout << boolalpha;
    cout << np::allclose(A, L.dot(np::diagonal(D)).dot(L.transpose())) << "\n";
    cout << "L:\n" << L << "\nD:\n" << D << "\n";
    return 0;
}
```

```
[Out] true
      L:
      [[  1,           0,           0, 0]
       [2.5,           1,           0, 0]
       [ -1, -0.70588235,           1, 0]
       [3.5,   2.1764706, -0.29885057, 1]]
      D:
      [2, -8.5, 10.235294, 16.850575]
```

### `ldl_solve`

Solves the equation ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
for *x* given the LDL decomposition of *A* (as given by `ldl`). The 
function throws a `LinAlgError` exception if the matrix is singular.
```cpp
template <class T>
numcpp::array<T> ldl_solve(
    const numcpp::matrix<T> &L,
    const numcpp::array<T> &D,
    const numcpp::array<T> &b
);

template <class T>
numcpp::matrix<T> ldl_solve(
    const numcpp::matrix<T> &L,
    const numcpp::array<T> &D,
    const numcpp::matrix<T> &b
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
    np::matrix<double> A = {{ 2,  5, -2,  7},
                            { 5,  4,  1, -1},
                            {-2,  1,  8,  3},
                            { 7, -1,  3,  2}};
    np::array<double> b = {0, 3, 5, 1};

    np::matrix<double> L;
    np::array<double> D;
    scicpp::ldl(A, L, D);

    np::array<double> x = scicpp::ldl_solve(L, D, b);
    cout << boolalpha << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] true
      x = [0.0047748977, 0.53478854, 0.63506139, -0.20190996]
```

### `cholesky`

Computes the Cholesky decomposition of a symmetric positive-definite matrix. 
The decomposition is

![$A = LL^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20LL%5ET) 

where *L* is a lower triangular matrix. The function throws a `LinAlgError` 
exception if the decomposition fails.
```cpp
template <class T>
void cholesky(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &L
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
    np::matrix<double> A = {{ 7, 1, -2,  0},
                            { 1, 4,  1,  2},
                            {-2, 1,  8,  3},
                            { 0, 2,  3, 10}};
    np::matrix<double> L;
    scicpp::cholesky(A, L);
    cout << boolalpha << np::allclose(A, L.dot(L.transpose())) << "\n";
    cout << "L:\n" << L << "\n";
    return 0;
}
```

```
[Out] true
      L:
      [[  2.6457513,          0,         0,         0]
       [ 0.37796447,   1.963961,         0,         0]
       [-0.75592895, 0.65465367, 2.6457513,         0]
       [          0,  1.0183502, 0.8819171, 2.8609763]]
```

### `cholesky_solve`

Solves the equation ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
for *x* given the Cholesky decomposition of *A* (as given by `cholesky`).
```cpp
template <class T>
numcpp::array<T> cholesky_solve(
    const numcpp::matrix<T> &L,
    const numcpp::array<T> &b
);

template <class T>
numcpp::matrix<T> cholesky_solve(
    const numcpp::matrix<T> &L,
    const numcpp::matrix<T> &b
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
    np::matrix<double> A = {{ 7, 1, -2,  0},
                            { 1, 4,  1,  2},
                            {-2, 1,  8,  3},
                            { 0, 2,  3, 10}};
    np::array<double> b = {0, 3, 5, 1};

    np::matrix<double> L;
    scicpp::cholesky(A, L);

    np::array<double> x = scicpp::cholesky_solve(L, b);
    cout << boolalpha << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] true
      x = [0.087912088, 0.68131868, 0.64835165, -0.23076923]
```

### `qr`

Computes the QR decomposition of a matrix with linearly independent columns. 
The decomposition is

![$A = QR$](https://render.githubusercontent.com/render/math?math=A%20%3D%20QR) 

where *Q* is an orthonormal matrix (i.e., ![$Q^T Q = Q Q^T = I$](https://render.githubusercontent.com/render/math?math=Q%5ET%20Q%20%3D%20Q%20Q%5ET%20%3D%20I))
and *R* is an upper triangular matrix.

If *A* is a rectangular matrix of shape *m × n*, *m ≥ n*, the decomposition can 
be partitioned as

![$A = QR = \begin{bmatrix}Q_1 & Q_2\end{bmatrix}\begin{bmatrix}R_1\\ 
0\end{bmatrix} = Q_1R_1$](https://render.githubusercontent.com/render/math?math=A%20%3D%20QR%20%3D%20%5Cbegin%7Bbmatrix%7DQ_1%20%26%20Q_2%5Cend%7Bbmatrix%7D%5Cbegin%7Bbmatrix%7DR_1%5C%5C%20%0A0%5Cend%7Bbmatrix%7D%20%3D%20Q_1R_1)

where *Q<sub>1</sub>*, *Q<sub>2</sub>* have orthonormal columns, *R* is an 
upper triangular matrix and *0* is the zero matrix.
```cpp
template <class T>
void qr(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &Q,
    numcpp::matrix<T> &R,
    bool full_matrices = true
);
```
If `full_matrices = true` (default), *Q* and *R* will be of shape *m × m* and 
*m × n*, respectively. Otherwise, *Q* and *R* will be of shape *m × n* and 
*n × n*. The function throws a `LinAlgError` exception if the decomposition 
fails.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, 1,  2},
                            { 1, 5, 2,  2},
                            {-2, 1, 0, -1},
                            { 3, 2, 4,  0}};
    np::matrix<double> Q, R;
    scicpp::qr(A, Q, R);

    cout << boolalpha;
    cout << np::allclose(np::dot(Q, Q.transpose()), np::eye(4, 4)) << "\n";
    cout << np::allclose(np::dot(Q.transpose(), Q), np::eye(4, 4)) << "\n";
    cout << np::allclose(Q.dot(R), A) << "\n";
    cout << "Q:\n" << Q << "\nR:\n" << R << "\n";

    return 0;
}
```

```
[Out] true
      true
      true
      Q:
      [[ -0.8819171, 0.067050916,  -0.39272679, -0.25197632]
       [-0.12598816, -0.90997671,  -0.11494443,  0.37796447]
       [ 0.25197632, -0.39272679, -0.067050916,  -0.8819171]
       [-0.37796447, -0.11494443,   0.90997671, -0.12598816]]
      R:
      [[-7.9372539, -3.7796447, -2.6457513,  -2.2677868]
       [         0, -4.9713465, -2.2126802,  -1.2931248]
       [         0,          0,  3.0172912, -0.94829152]
       [         0,          0,          0,   1.1338934]]
```