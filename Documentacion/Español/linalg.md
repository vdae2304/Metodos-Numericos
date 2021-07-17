# Álgebra Lineal

## Contenido

- [Funciones básicas](#Funciones-básicas)
- [Sistemas de ecuaciones lineales](#Sistemas-de-ecuaciones-lineales)
- [Descomposición LU](#Descomposición-LU)
- [Descomposición LDL](#Descomposición-LDL)
- [Descomposición de Cholesky](#Descomposición-de-Cholesky)
- [Descomposición QR](#Descomposición-QR)
- [Descomposición de Hessenberg](#Descomposición-de-Hessenberg)
- [Valores y vectores propios](#Valores-y-vectores-propios)
- [Descomposición en valores singulares](#Descomposición-en-valores-singulares)

## Funciones básicas

### `norm`

Calcula la norma de un vector. Esta función es capaz de devolver alguna de las 
siguientes normas:

- `p = 1`: ![$\|v\|_1 = \sum_{i = 1}^{n}|v_i|$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_1%20%3D%20%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%7Cv_i%7C) 

- `p = 2` (por defecto): ![$\|v\|_2 = \sqrt{\sum_{i = 1}^{n}v_i^2}$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_2%20%3D%20%5Csqrt%7B%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7Dv_i%5E2%7D) 

- `p = inf`: ![$\|v\|_{\infty} = \underset{i}{\max \,} |v_i|$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_%7B%5Cinfty%7D%20%3D%20%5Cunderset%7Bi%7D%7B%5Cmax%20%5C%2C%7D%20%7Cv_i%7C) 

- Cualquier otro valor de `p`: ![$\|v\|_p = \left(\sum_{i = 1}^{n}|v_i|^p\right)^{1/p}$](https://render.githubusercontent.com/render/math?math=%5C%7Cv%5C%7C_p%20%3D%20%5Cleft(%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%7Cv_i%7C%5Ep%5Cright)%5E%7B1%2Fp%7D)

```cpp
template <class T>
T norm(const numcpp::array<T> &v, double p = 2);
```

#### Ejemplo 

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

Calcula el determinante de una matriz.
```cpp
template <class T>
T det(const numcpp::matrix<T> &A);
```

#### Ejemplo

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

Calcula la inversa de una matriz.
```cpp
template <class T>
numcpp::matrix<T> inv(const numcpp::matrix<T> &A);
```

#### Ejemplo

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

Calcula la pseudo-inversa de Moore-Penrose de una matriz.
```cpp
template <class T>
numcpp::matrix<T> pinv(const numcpp::matrix<T> &A, const T &cond = 1e-8);
```

#### Ejemplo

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

## Sistemas de ecuaciones lineales

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

Resuelve por mínimos cuadrados la ecuación ![$Ax = b$](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) 
para *x*. 

Calcula el *x* tal que la norma ![$\|Ax - b\|$](https://render.githubusercontent.com/render/math?math=%5C%7CAx%20-%20b%5C%7C) 
es mínima.

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
Si `method = "svd"`, calcula la solución utilizando la descomposición en 
valores singulares de *A*. En tal caso, valores singulares menores o iguales 
que `cond*max(S)` son considerados cero. Si `method = "qr"`, calcula la 
solución utilizando la descomposición QR de *A*. El método `"qr"` es más 
rápido pero funciona solamente en sistemas sobredeterminados (más 
ecuaciones que incógnitas), mientras que el método `"svd"` es más lento pero
funciona tanto en sistemas sobredeterminados como subdeterminados. Por 
defecto, `method = "svd"`. La función arroja una excepción del tipo 
`LinAlgError` si la factorización falla.

#### Ejemplo

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
    np::array<double> b = {1, 0, 3, -2, 5};
    np::array<double> x = scicpp::lstsq(A, b, "svd");
    np::array<double> y = scicpp::lstsq(A, b, "qr");
    cout << "x = " << x << "\n";
    cout << "y = " << y << "\n";
    cout << "|Ax - b|: " << scicpp::norm(A.dot(x) - b) << "\n";
    return 0;
}
```

```
[Out] x = [-0.27561683, 0.39230793, 0.57941608]
      y = [-0.27561683, 0.39230793, 0.57941608]
      |Ax - b|: 4.8723866
```

## Descomposición LU

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
);
```

#### Ejemplo 

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

Calcula la descomposición LU con pivoteo de una matriz. Los elementos de las 
matrices *L* y *U* son guardados en una misma matriz `LU`. Los índices de los pivotes representando la matriz de permutación *P* son guardados en un arreglo 
`piv`.
```cpp
template <class T>
void lu_factor(
    const numcpp::matrix<T> &A,
    numcpp::array<size_t> &piv,
    numcpp::matrix<T> &LU
);
```

#### Ejemplo

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

## Descomposición LDL

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

## Descomposición de Cholesky

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

## Descomposición QR

### `qr`

Calcula la descomposición QR de una matriz con columnas linealmente 
independientes. La descomposición es

![$A = QR$](https://render.githubusercontent.com/render/math?math=A%20%3D%20QR) 

donde *Q* es una matriz ortonormal (i.e., ![$Q^T Q = Q Q^T = I$](https://render.githubusercontent.com/render/math?math=Q%5ET%20Q%20%3D%20Q%20Q%5ET%20%3D%20I))
y *R* es una matriz triangular superior. 

Cuando *A* es una matriz rectangular de *m × n*, con *m ≥ n*, la descomposición 
se puede particionar como

![$A = QR = \begin{bmatrix}Q_1 & Q_2\end{bmatrix}\begin{bmatrix}R_1\\ 
0\end{bmatrix} = Q_1R_1$](https://render.githubusercontent.com/render/math?math=A%20%3D%20QR%20%3D%20%5Cbegin%7Bbmatrix%7DQ_1%20%26%20Q_2%5Cend%7Bbmatrix%7D%5Cbegin%7Bbmatrix%7DR_1%5C%5C%20%0A0%5Cend%7Bbmatrix%7D%20%3D%20Q_1R_1)

donde *Q<sub>1</sub>*, *Q<sub>2</sub>* tienen columnas ortonormales, *R* es una 
matriz triangular superior y *0* es la matriz de ceros. 
```cpp
template <class T>
void qr(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &Q,
    numcpp::matrix<T> &R,
    bool full_matrices = true
);
```
Si `full_matrices = true` (valor por defecto), *Q* y *R* serán matrices de 
*m × m* y *m × n*, respectivamente. De lo contrario, *Q* y *R* serán matrices 
de *m × n* y *n × n*. La función arroja una excepción del tipo `LinAlgError` si 
la descomposición falla.

#### Ejemplo

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

Calcula la descomposición QR de una matriz con columnas linealmente 
independientes. La matriz *Q* está codificada en la parte inferior de `R` y 
en `tau`. La descomposición es calculada mediante transformaciones de 
Householder.
```cpp
template <class T>
void qr_raw(
    const numcpp::matrix<T> &A,
    numcpp::array<T> &tau,
    numcpp::matrix<T> &R
);
```

#### Ejemplo

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

## Descomposición de Hessenberg

Calcula la forma de Hessenberg de una matriz. La descomposición es

![$A = QHQ^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20QHQ%5ET)

donde *Q* es una matriz ortonormal (i.e., ![$Q^T Q = Q Q^T = I$](https://render.githubusercontent.com/render/math?math=Q%5ET%20Q%20%3D%20Q%20Q%5ET%20%3D%20I))
y *H* tiene ceros debajo de la primera subdiagonal.

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/linalg.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{2,  5,  8, 7},
                            {5,  2, -2, 8},
                            {0,  5,  4, 1},
                            {7, -1,  6, 6}};
    np::matrix<double> I = np::eye<double>(4, 4);

    np::matrix<double> Q, H;
    scicpp::hessenberg(A, Q, H);

    cout << boolalpha;
    cout << "Q Q^T == I: " << np::allclose(np::dot(Q, Q.transpose()), I) << "\n";
    cout << "Q^T Q == I: " << np::allclose(np::dot(Q.transpose(), Q), I) << "\n";
    cout << "A == Q H Q^T: "
         << np::allclose(A, Q.dot(H).dot(Q.transpose())) << "\n";
    cout << "Q:\n" << Q << "\nH:\n" << H << "\n";

    return 0;
}
```

```
[Out] Q Q^T == I: true
      Q^T Q == I: true
      A == Q H Q^T: true
      Q:
      [[1,           0,           0,           0]
       [0, -0.58123819, -0.57719147,  0.57359582]
       [0,           0, -0.70489397, -0.70931269]
       [0, -0.81373347,  0.41227962,  -0.4097113]]
      H:
      [[         2, -8.6023253, -5.6391518, -5.6745015]
       [-8.6023253,  7.9594595, -1.1065319,  6.3440458]
       [         0,  5.2772822,  1.1941814, -2.3265566]
       [         0,          0,  6.2757687,  2.8463591]]
 ```

## Valores y vectores propios

### `eigen_symm`

Calcula la descomposición espectral de una matriz simétrica. La descomposición 
es

![$A = VDV^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20VDV%5ET)

donde *V* es una matriz ortonormal (i.e., ![$V^T V = V V^T = I$](https://render.githubusercontent.com/render/math?math=V%5ET%20V%20%3D%20V%20V%5ET%20%3D%20I))
y *D* es una matriz diagonal. Las columnas de *V* son conocidos como los 
vectores propios y los elementos en la diagonal de *D* son conocidos como los 
valores propios. La descomposición es calculada mediante rotaciones de Givens.

```cpp
template <class T>
void eigen_symm(
    const numcpp::matrix<T> &A,
    numcpp::matrix<T> &V,
    numcpp::array<T> &D
);
```

#### Ejemplo

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

Calcula los valores propios de una matriz simétrica, sin algún orden en 
particular.
```cpp
template <class T>
numcpp::array<T> eigenvals_symm(const numcpp::matrix<T> &A);
```

#### Ejemplo

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

## Descomposición en valores singulares

### `diagsvd`

Construye la matriz de valores singulares de tamaño *m × n*. 

```cpp
template <class T>
numcpp::matrix<T> diagsvd(const numcpp::array<T> &S, size_t m, size_t n);
```

#### Ejemplo

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

Calcula la descomposición en valores singulares de una matriz rectangular de
*m × n*. La descomposición es

![$A = U\Sigma V^T$](https://render.githubusercontent.com/render/math?math=A%20%3D%20U%5CSigma%20V%5ET)

donde *U* y *V* son matrices ortonormales de *m × m*  y *n × n*, 
respectivamente, (i.e., ![$U^T U = U U^T = I$](https://render.githubusercontent.com/render/math?math=U%5ET%20U%20%3D%20U%20U%5ET%20%3D%20I)
y ![$V^T V = V V^T = I$](https://render.githubusercontent.com/render/math?math=V%5ET%20V%20%3D%20V%20V%5ET%20%3D%20I))
y *Σ* es una matriz rectangular diagonal de *m × n* con valores no negativos 
en la diagonal. Las columnas de *U* y *V* son conocidos como los vectores 
singulares izquierdo y derecho, respectivamente, y los elementos en la diagonal 
de *Σ* son conocidos como los valores singulares. Por convención, los valores
singulares se encuentran ordenados de mayor a menor. 
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
Si `full_matrices = true` (valor por defecto), *U* y *V* serán matrices de 
*m × m*  y *n × n*, respectivamente. De lo contrario, *U* y *V* serán matrices 
de *m × k*  y *k × n*, respectivamente, donde *k = min(m, n)*.

#### Ejemplo 

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

Calcula los valores singulares de una matriz, ordenados de mayor a menor.
```cpp
template <class T>
numcpp::array<T> svdvals(const numcpp::matrix<T> &A);
```

#### Ejemplo

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