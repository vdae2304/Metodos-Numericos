# Álgebra Lineal

## Contenido

- [Funciones básicas](#Funciones-básicas)
- [Factorizaciones](#Factorizaciones)

## Funciones básicas

### `solve_triangular`

Resuelve la ecuación ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
para *x* suponiendo que *A* es una matriz triangular. 
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
Si `lower` es `true`, utiliza únicamente los elementos en la parte inferior de 
*A*. De lo contrario, utiliza únicamente los elementos en la parte superior de 
*A*. Si `transpose` es `true`, resuelve en su lugar la ecuación 
![$A^Tx = b$](https://render.githubusercontent.com/render/math?math=A%5ETx%20%3D%20b). 
Por defecto, `transpose = false`. Si `unit_diagonal` es `true`, supone que los 
elementos en la diagonal de *A* son unos y por lo tanto no serán referenciados. 
Por defecto, `unit_diagonal = false`.

La función arroja una excepción del tipo `LinAlgError` si la matriz es 
singular.

#### Ejemplo

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

Resuelve la ecuación ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
para *x*. 

Si se conoce que la matriz tiene una determinada estructura, entonces 
proporcionando un valor al parámetro `assume_a` se indicará la manera en cómo 
se debe resolver la ecuación. Los valores permitidos para `assume_a` son: 
`"gen"` representando una matriz genérica (valor por defecto); `"sym"` 
representando una matriz simétrica; y `"pos"` representando una matriz 
simétrica definida positiva. 

La función arroja una excepción del tipo `LinAlgError` si la matriz es singular.
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

#### Ejemplo 

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

Calcula la inversa de una matriz.
```cpp
template <class T>
numcpp::matrix<T> inv(const numcpp::matrix<T> &A);
```

#### Ejemplo

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

Calcula el determinante de una matriz.
```cpp
template <class T>
T det(const numcpp::matrix<T> &A);
```

#### Ejemplo

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

## Factorizaciones

### `lu`

Calcula la descomposición LU con pivoteo de una matriz. La descomposición es

![$PA = LU$](https://render.githubusercontent.com/render/math?math=PA%20%3D%20LU) 

donde *P* es una matriz de permutación, *L* es una matriz triangular inferior 
con unos en la diagonal, y *U* es una matriz triangular superior.
```cpp
template <class T>
void lu(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &P,
    numcpp::matrix<T> &L,
    numcpp::matrix<T> &U
)
```

#### Ejemplo 

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

Calcula la descomposición LU con pivoteo de una matriz. Los elementos de las 
matrices *L* y *U* son guardados en una misma matriz `LU`. Los índices de los pivotes representando la matriz de permutación *P* son guardados en un arreglo 
`piv`.
```cpp
template <class T>
void lu_factor(
    const numcpp::matrix<T> &A,
    numcpp::array<size_t> &piv,
    numcpp::matrix<T> &LU
)
```

### `lu_solve`


Resuelve la ecuación ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
para *x* dada la descomposición LU de *A* (dada por `lu_factor`). La función 
arroja una excepción del tipo `LinAlgError` si la matriz es 
singular.
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

#### Ejemplo

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

Calcula la descomposición LDL de una matriz simétrica. La descomposición es

![$A = LDL^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20LDL%5ET) 

donde *L* es una matriz triangular inferior con unos en la diagonal y *D* es una matriz diagonal.
```cpp
template <class T>
void ldl(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &L,
    numcpp::array<T> &D
);
```

#### Ejemplo

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

Resuelve la ecuación ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
para *x* dada la descomposición LDL de *A* (dada por `ldl`). La función arroja 
una excepción del tipo `LinAlgError` si la matriz es singular.
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

#### Ejemplo

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

Calcula la descomposición de Cholesky de una matriz simétrica definida-positva. 
La descomposición es

![$A = LL^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20LL%5ET) 

donde *L* es una matriz triangular inferior. La función arroja una excepción 
del tipo `LinAlgError` si la descomposición falla.
```cpp
template <class T>
void cholesky(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &L
);
```

#### Ejemplo

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

Resuelve la ecuación ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
para *x* dada la descomposición de Cholesky de *A* (dada por `cholesky`).
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

#### Ejemplo

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