# Linear Algebra

## Content

- [Basic functions](#Basic-functions)
- [Systems of linear equations](#Systems-of-linear-equations)
- [LU decomposition](#LU-decomposition)
- [LDL decomposition](#LDL-decomposition)
- [Cholesky decomposition](#Cholesky-decomposition)
- [QR decomposition](#QR-decomposition)
- [Eigenvalues and eigenvectors](#Eigenvalues-and-eigenvectors)
- [Singular value decomposition](#Singular-value-decomposition)

## Basic functions

### `norm`

Computes the norm of a vector. This function is able to return one of the 
following norms:

- `p = 1`: ![$\|v\|_1 = \sum_{i = 1}^{n}|v_i|$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_1%20%3D%20%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%7Cv_i%7C) 

- `p = 2` (default): ![$\|v\|_2 = \sqrt{\sum_{i = 1}^{n}v_i^2}$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_2%20%3D%20%5Csqrt%7B%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7Dv_i%5E2%7D) 

- `p = inf`: ![$\|v\|_{\infty} = \underset{i}{\max \,} |v_i|$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_%7B%5Cinfty%7D%20%3D%20%5Cunderset%7Bi%7D%7B%5Cmax%20%5C%2C%7D%20%7Cv_i%7C) 

- Any other value of `p`: ![$\|v\|_p = \left(\sum_{i = 1}^{n}|v_i|^p\right)^{1/p}$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_p%20%3D%20%5Cleft(%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%7Cv_i%7C%5Ep%5Cright)%5E%7B1%2Fp%7D)

```cpp
template <class T>
T norm(const numcpp::array<T> &v, double p = 2);
```

#### Example 

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {-4, 0, 1, 5, 2};
    cout << "p = 2: " << scicpp::norm(v) << "\n";
    cout << "p = 1: " << scicpp::norm(v, 1) << "\n";
    cout << "p = inf: " << scicpp::norm(v, np::constants::inf) << "\n";
    cout << "p = 3: " << scicpp::norm(v, 3) << "\n";
    return 0;
}
```

```
[Out] p = 2: 6.78233
      p = 1: 12
      p = inf: 5
      p = 3: 5.82848
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
#include "scicpp/linalg.h"
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
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, 1,  5},
                            { 1, 5, 2,  2},
                            {-2, 1, 0, -1},
                            { 5, 2, 4,  0}};
    np::matrix<double> B = scicpp::inv(A);
    np::matrix<double> I = np::eye<double>(4, 4);
    cout << boolalpha;
    cout << "A A^-1 == I: " << np::allclose(A.dot(B), I) << "\n";
    cout << "A^-1 A == I: " << np::allclose(B.dot(A), I) << "\n";
    cout << "A^-1:\n" << B << "\n";
    return 0;
}

```

```
[Out] A A^-1 == I: true
      A^-1 A == I: true
      A^-1:
      [[ 0.38095238, -0.47619048, 0.95238095,  0.14285714]
       [ 0.34920635, -0.26984127,  1.2063492, 0.047619048]
       [-0.65079365,  0.73015873, -1.7936508, 0.047619048]
       [-0.41269841,  0.68253968, -1.6984127, -0.23809524]]
```

### `pinv`

Computes Moore-Penrose pseudo-inverse of a matrix.
```cpp
template <class T>
numcpp::matrix<T> pinv(const numcpp::matrix<T> &A, const T &cond = 1e-8);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, -2},
                            { 3, 5,  1},
                            {-2, 1,  0},
                            { 5, 0,  2}};
    np::matrix<double> B = scicpp::pinv(A);
    cout << boolalpha;
    cout << "A A^+ A == A: " << np::allclose(A.dot(B).dot(A), A) << "\n";
    cout << "A^+ A A^+ == A^+: " << np::allclose(B.dot(A).dot(B), B) << "\n";
    cout << "A^+:\n" << B << "\n";
    return 0;
}
```

```
[Out] A A^+ A == A: true
      A^+ A A^+ == A^+: true
      A^+:
      [[   0.0755886, -0.034342361, -0.055053992,   0.09275978]
       [0.0061957869,   0.17985484,  0.082138432, -0.083731634]
       [ -0.21313507,   0.12727916, 0.0030093822,   0.22322535]]
```

## Systems of linear equations

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
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 2, 0, 0, 0},
                            { 5, 4, 0, 0},
                            {-2, 1, 8, 0},
                            { 7, 0, 5, 2}};
    np::array<double> b = {0, 3, 5, 1};
    np::array<double> x = scicpp::solve_triangular(A, b, true);
    cout << boolalpha << "Ax == b: " << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    A = {{2, 0, -3, 5},
         {0, 4,  2, 0},
         {0, 0,  8, 3},
         {0, 0,  0, 2}};
    b = {0, 3, 5, 1};
    x = scicpp::solve_triangular(A, b, false);
    cout << "Ax == b: " << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] Ax == b: true
      x = [0, 0.75, 0.53125, -0.828125]
      Ax == b: true
      x = [-0.59375, 0.53125, 0.4375, 0.5]
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
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, -1,  0},
                            { 1, 4,  1, -1},
                            {-2, 3,  0,  8},
                            { 5, 2,  8,  5}};
    np::array<double> b = {-1, 0, 5, 2};
    np::array<double> x = scicpp::solve(A, b);
    cout << boolalpha << "Ax == b: " << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";
    return 0;
}
```

```
[Out] Ax == b: true
      x = [-0.21405604, 0.17409279, 0.023886082, 0.50620119]
```

### `lstsq`

Solves by least squares the equation ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
for *x*. 

Computes the *x* such that the norm ![$\|Ax - b\|$](https://render.githubusercontent.com/render/math?math=%5C%7CAx%20-%20b%5C%7C) 
is minimized.

```cpp
template <class T>
numcpp::array<T> lstsq(
    const numcpp::matrix<T> &A,
    const numcpp::array<T> &b,
    const std::string &method = "svd",
    const T cond = 1e-8
);

template <class T>
numcpp::matrix<T> lstsq(
    const numcpp::matrix<T> &A,
    const numcpp::matrix<T> &b,
    const std::string &method = "svd",
    const T cond = 1e-8
);
```
If `method = "svd"`, computes the solution using the singular value 
decomposition of *A*. In such case, singular values smaller or equal than 
`cond*max(S)` are considered zero. If `method = "qr"`, computes the solution 
using the QR decomposition of *A*. The `"qr"` method is faster but it only 
works on overdetermined systems (more equations than unknowns), while the 
`"svd"` method is slower but it works on both overdetermined and 
underdetermined systems. By default, `method = "svd"`. The function throws a 
`LinAlgError` exception if the decomposition fails.

## LU decomposition

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
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 2, 0, -3, 5},
                            { 5, 4,  2, 0},
                            {-2, 1,  8, 3},
                            { 7, 0,  5, 2}};
    np::matrix<double> P, L, U;
    scicpp::lu(A, P, L, U);
    cout << boolalpha;
    cout << "P A == L U: " << np::allclose(P.dot(A), L.dot(U)) << "\n";
    cout << "P:\n" << P << "\nL:\n" << L << "\nU:\n" << U << "\n";
    return 0;
}
```

```
[Out] P A == L U: true
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

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 2, 0, -3, 5},
                            { 5, 4,  2, 0},
                            {-2, 1,  8, 3},
                            { 7, 0,  5, 2}};
    np::array<size_t> piv;
    np::matrix<double> LU;
    scicpp::lu_factor(A, piv, LU);
    cout << "piv:\n" << piv << "\nLU:\n" << LU << "\n";
    return 0;
}
```

```
[Out] piv:
      [3, 1, 2, 0]
      LU:
      [[          7,    0,           5,          2]
       [ 0.71428571,    4,  -1.5714286, -1.4285714]
       [-0.28571429, 0.25,   9.8214286,  3.9285714]
       [ 0.28571429,    0, -0.45090909,        6.2]]
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
#include "scicpp/linalg.h"
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
    cout << boolalpha << "Ax == b: " << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] Ax == b: true
      x = [-0.2, 0.81935484, 0.36129032, 0.29677419]
```

## LDL decomposition

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
#include "scicpp/linalg.h"
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
    cout << "A == L D L^T: "
         << np::allclose(A, L.dot(np::diagonal(D)).dot(L.transpose())) << "\n";
    cout << "L:\n" << L << "\nD:\n" << D << "\n";
    return 0;
}
```

```
[Out] A == L D L^T: true
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
#include "scicpp/linalg.h"
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
    cout << boolalpha << "Ax == b: " << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] Ax == b: true
      x = [0.0047748977, 0.53478854, 0.63506139, -0.20190996]
```

## Cholesky decomposition

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
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 1, -2,  0},
                            { 1, 4,  1,  2},
                            {-2, 1,  8,  3},
                            { 0, 2,  3, 10}};
    np::matrix<double> L;
    scicpp::cholesky(A, L);
    cout << boolalpha;
    cout << "A == L L^T: " << np::allclose(A, L.dot(L.transpose())) << "\n";
    cout << "L:\n" << L << "\n";
    return 0;
}
```

```
[Out] A == L L^T: true
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
#include "scicpp/linalg.h"
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
    cout << boolalpha << "Ax == b: " << np::allclose(A.dot(x), b) << "\n";
    cout << "x = " << x << "\n";

    return 0;
}
```

```
[Out] Ax == b: true
      x = [0.087912088, 0.68131868, 0.64835165, -0.23076923]
```

## QR decomposition

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
If `full_matrices = true` (default value), *Q* and *R* will be *m × m* and 
*m × n* matrices, respectively. Otherwise, *Q* and *R* will be *m × n* and 
*n × n* matrices, respectively. The function throws a `LinAlgError` exception 
if the decomposition fails.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, 1,  2},
                            { 1, 5, 2,  2},
                            {-2, 1, 0, -1},
                            { 3, 2, 4,  0}};
    np::matrix<double> I = np::eye<double>(4, 4);

    np::matrix<double> Q, R;
    scicpp::qr(A, Q, R);

    cout << boolalpha;
    cout << "Q Q^T == I: " << np::allclose(np::dot(Q, Q.transpose()), I) << "\n";
    cout << "Q^T Q == I: " << np::allclose(np::dot(Q.transpose(), Q), I) << "\n";
    cout << "A == Q R: " << np::allclose(A, Q.dot(R)) << "\n";
    cout << "Q:\n" << Q << "\nR:\n" << R << "\n";

    return 0;
}
```

```
[Out] Q Q^T == I: true
      Q^T Q == I: true
      A == Q R: true
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

### `qr_raw`

Computes the QR decomposition of a matrix with linearly independent columns. 
The matrix *Q* is encoded in the lower triangle of `R` and in `tau`. The 
decomposition is computed using Householder transformations.
```cpp
template <class T>
void qr_raw(
    const numcpp::matrix<T> &A,
    numcpp::array<T> &tau,
    numcpp::matrix<T> &R
);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, 1,  2},
                            { 1, 5, 2,  2},
                            {-2, 1, 0, -1},
                            { 3, 2, 4,  0}};
    np::array<double> tau;
    np::matrix<double> R;
    scicpp::qr_raw(A, tau, R);
    cout << "tau:\n" << tau << "\nR:\n" << R << "\n";
    return 0;
}
```

```
[Out] tau:
      [14.937254, 9.5174716, -3.3246454, -2.2677868]
      R:
      [[-7.9372539, -3.7796447, -2.6457513,  -2.2677868]
       [         1, -4.9713465, -2.2126802,  -1.2931248]
       [        -2,  1.9077498,  3.0172912, -0.94829152]
       [         3, 0.63837528,  3.0015962,   1.1338934]]
```

## Eigenvalues and eigenvectors

### `eigen_symm`

Computes the eigendecomposition of a symmetric matrix. The decomposition is

![$A = VDV^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20VDV%5ET)

where *V* is an orthonormal matrix (i.e., ![$V^T V = V V^T = I$](https://render.githubusercontent.com/render/math?math=V%5ET%20V%20%3D%20V%20V%5ET%20%3D%20I))
and *D* is a diagonal matrix. The columns of *V* are known as the eigenvectors 
and the elements along the diagonal of *D* are known as the eigenvalues. The
decomposition is computed using Givens rotations.

```cpp
template <class T>
void eigen_symm(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &V,
    numcpp::array<T> &D
);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, -2,  5},
                            { 3, 5,  1,  2},
                            {-2, 1,  0, -1},
                            { 5, 2, -1,  0}};
    np::matrix<double> I = np::eye(4, 4);

    np::matrix<double> V;
    np::array<double> D;
    scicpp::eigen_symm(A, V, D);

    cout << boolalpha;
    cout << "V V^T == I: " << np::allclose(np::dot(V, V.transpose()), I) << "\n";
    cout << "V^T V == I: " << np::allclose(np::dot(V.transpose(), V), I) << "\n";
    cout << "A == V D V^T: "
         << np::allclose(A, V.dot(np::diagonal(D)).dot(V.transpose())) << "\n";
    cout << "V:\n" << V << "\nD:\n" << D << "\n";

    return 0;
}
```

```
[Out] V V^T == I: true
      V^T V == I: true
      A == V D V^T: true
      V:
      [[ 0.77811649, -0.34624052,  0.28875493, -0.43734749]
       [ 0.45009962,  0.83195125, -0.31737551,  -0.0673813]
       [-0.12966954,  0.41600906,  0.89943338, 0.033791498]
       [ 0.41848641, -0.12211096, 0.083144242,  0.89612782]]
      D:
      [11.757732, 3.9579505, -1.0873842, -2.6282986]
```

### `eigenvals_symm`

Computes the eigenvalues of a symmetric matrix, in no specific order.
```cpp
template <class T>
numcpp::array<T> eigenvals_symm(const numcpp::matrix<T> &A);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, -2,  5},
                            { 3, 5,  1,  2},
                            {-2, 1,  0, -1},
                            { 5, 2, -1,  0}};
    cout << scicpp::eigenvals_symm(A) << "\n";
    return 0;
}
```

```
[Out] [11.757732, 3.9579505, -1.0873842, -2.6282986]
```

## Singular value decomposition

### `diagsvd`

Constructs the singular values matrix of shape *m × n*. 

```cpp
template <class T>
numcpp::matrix<T> diagsvd(const numcpp::array<T> &S, size_t m, size_t n);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> S = {1, 2, 3, 4};
    cout << scicpp::diagsvd(S, 4, 5) << "\n";
    return 0;
}
```

```
[Out] [[1, 0, 0, 0, 0]
       [0, 2, 0, 0, 0]
       [0, 0, 3, 0, 0]
       [0, 0, 0, 4, 0]]
 ```

 ### `svd`

Computes the singular value decomposition of a *m × n* rectangular matrix. The 
decomposition is

![$A = U\Sigma V^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20U%5CSigma%20V%5ET)

where *U* and *V* are *m × m*  and *n × n* orthonormal matrices, respectively,
(i.e., ![$U^T U = U U^T = I$](https://render.githubusercontent.com/render/math?math=U%5ET%20U%20%3D%20U%20U%5ET%20%3D%20I)
and ![$V^T V = V V^T = I$](https://render.githubusercontent.com/render/math?math=V%5ET%20V%20%3D%20V%20V%5ET%20%3D%20I))
and *Σ* is an *m × n* rectangular diagonal matrix with non-negative values on 
the diagonal. The columns of *U* and *V* are known as the left and right 
singular vectors, respectively, and the elements along the diagonal of *Σ* are 
known as the singular values. By convention, singular values are sorted from 
largest to smallest.
```cpp
template <class T>
void svd(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &U,
    numcpp::array<T> &S,
    numcpp::matrix<T> &V,
    bool full_matrices = true
);
```
If `full_matrices = true` (default value), *U* and *V* will be *m × m* and 
*n × n* matrices, respectively. Otherwise, *U* and *V* will be *m × k* and 
*k × n* matrices, respectively, with *k = min(m, n)*.

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, -2},
                            { 3, 5,  1},
                            {-2, 1,  0},
                            { 5, 0,  2},
                            {-1, -1, 5}};
    np::matrix<double> I = np::eye(5, 5), J = np::eye(3, 3);

    np::matrix<double> U, V;
    np::array<double> S;
    scicpp::svd(A, U, S, V);

    cout << boolalpha;
    cout << "U U^T == I: " << np::allclose(np::dot(U, U.transpose()), I) << "\n";
    cout << "U^T U == I: " << np::allclose(np::dot(U.transpose(), U), I) << "\n";
    cout << "V V^T == I: " << np::allclose(np::dot(V, V.transpose()), J) << "\n";
    cout << "V^T V == I: " << np::allclose(np::dot(V.transpose(), V), J) << "\n";
    cout << "A == U S V^T: "
         << np::allclose(A, U.dot(scicpp::diagsvd(S, 5, 3)).dot(V.transpose()))
         << "\n";
    cout << "U:\n" << U << "\nS:\n" << S << "\nV:\n" << V << "\n";

    return 0;
}
```

```
[Out] U U^T == I: true
      U^T U == I: true
      V V^T == I: true
      V^T V == I: true
      A == U S V^T: true
      U:
      [[ 0.75336436,  -0.18050362, -0.16072167, -0.60994602, -0.044665221]
       [ 0.46475611,   0.16947394,  0.78429388,  0.33014752,  -0.17653913]
       [-0.12823347, -0.078473554,  0.41183273, -0.30557563,   0.84523099]
       [ 0.40804833,    0.4890524, -0.42499216,  0.43667045,   0.47225512]
       [-0.18302088,   0.83268642, 0.093952772, -0.48467609,  -0.17146031]]
      S:
      [10.319722, 5.7937229, 4.235106]
      V:
      [[0.88641426,  0.17508935, -0.42850144]
       [0.44949479, -0.10447534,  0.88715238]
       [-0.1105631,  0.97899368,  0.17131018]]
```

### `svdvals`

Computes the singular values of a matrix, sorted from largest to smallest.
```cpp
template <class T>
numcpp::array<T> svdvals(const numcpp::matrix<T> &A);
```

#### Example

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{ 7, 3, -2},
                            { 3, 5,  1},
                            {-2, 1,  0},
                            { 5, 0,  2},
                            {-1, -1, 5}};
    cout << scicpp::svdvals(A) << "\n";
    return 0;
}
```

```
[Out] [10.319722, 5.7937229, 4.235106]
```