# Algebra Lineal

Biblioteca: `linalg.h`

Requisitos: C++11

## Factorización de matrices

### Factorización LU

```cpp
template <class T, class RandomAccessIterator>
void lu_decomposition(const matrix<T> &A, matrix<T> &LU, RandomAccessIterator P, bool sign = false);
```

Calcula la factorización LU con pivoteo de una matriz cuadrada. Esta factorización viene dada por 

![PA = LU](https://render.githubusercontent.com/render/math?math=PA%20%3D%20LU)

donde ![P](https://render.githubusercontent.com/render/math?math=P) es una matriz de permutación, ![L](https://render.githubusercontent.com/render/math?math=L) es una matriz triangular inferior con 1's en la diagonal y ![U](https://render.githubusercontent.com/render/math?math=U) es una matriz triangular superior.

Argumentos:
* `A`: La matriz a factorizar.
* `LU`: Matriz correspondiente a la factorización. Los elementos de ![L](https://render.githubusercontent.com/render/math?math=L) corresponden a la parte inferior de `LU` mientras que los elementos de ![U](https://render.githubusercontent.com/render/math?math=U) a la parte superior. La diagonal de 1's de ![L](https://render.githubusercontent.com/render/math?math=L) es descartada.
* `P`: Iterador correspondiente a la permutación. El renglón `i` de ![LU](https://render.githubusercontent.com/render/math?math=LU) corresponde al renglón `P[i]` de ![A](https://render.githubusercontent.com/render/math?math=A).
* `sign`: Si `sign` es `true`, entonces calcula también el signo de la permutación y lo guarda al final de `P`.

Ejemplo:

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;

matrix<double> permute(int P[], const matrix<double> &A) {
    matrix<double> PA(A.rows(), A.columns());
    for (int i = 0; i < A.rows(); ++i)
        for (int j = 0; j < A.columns(); ++j)
            PA[i][j] = A[P[i]][j];
    return PA;
}

void print(const matrix<double> &A) {
    for (int i = 0; i < A.rows(); ++i) {
        for (int j = 0; j < A.columns(); ++j)
            printf("%.6f ", A[i][j]);
        printf("\n");
    }
}

int main() {
    matrix<double> A = {{ 2,  0, -3},
                        { 5,  4,  2},
                        {-2,  1,  8}};
    int P[3];
    matrix<double> LU;
    lu_decomposition(A, LU, P);

    matrix<double> PA = permute(P, A);
    matrix<double> L = triangular(LU, true, true);
    matrix<double> U = triangular(LU, false, false);

    if (allclose(matmul(L, U), PA)) {
        printf("P:\n");
        for (int i = 0; i < 3; ++i)
            printf("%d ", P[i]);
        printf("\nL:\n");
        print(L);
        printf("U:\n");
        print(U);
    }

    return 0;
}
```

```
P:
1 2 0
L:
1.000000 0.000000 0.000000
-0.400000 1.000000 0.000000
0.400000 -0.615385 1.000000
U:
5.000000 4.000000 2.000000
0.000000 2.600000 8.800000
0.000000 0.000000 1.61538
```

### Factorización de Cholesky

```cpp
template <class T>
void cholesky_decomposition(const matrix<T> &A, matrix<T> &L);
```

Calcula la factorización de Cholesky de una matriz cuadrada. Esta factorización solo existe cuando la matriz es simétrica y positiva definida, y viene dada 

![A = LL^T](https://render.githubusercontent.com/render/math?math=A%20%3D%20LL%5ET)

donde ![L](https://render.githubusercontent.com/render/math?math=L) es una matriz triangular inferior.

Argumentos:
* `A`: La matriz a factorizar.
* `L`: Matriz correspondiente a la factorización. Los elementos de la factorización estarán contenidos tanto en la parte superior como en parte la inferior de `L`.

Esta función solo hace uso de la parte inferior de `A`. La función arroja una excepción si la matriz no es positiva definida.

Ejemplo:

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{ 2, 0, -2},
                        { 5, 4,  0},
                        {-1, 4,  3}};
    A = matmul(A, transpose(A)); // Es simetrica y positiva definida siempre y cuando A no sea singular.

    matrix<double> LLT;
    cholesky_decomposition(A, LLT);

    matrix<double> L = triangular(LLT, true);
    matrix<double> LT = triangular(LLT, false);

    if (allclose(matmul(L, LT), A)) {
        printf("L:\n");
        for (int i = 0; i < L.rows(); ++i) {
            for (int j = 0; j < L.columns(); ++j)
                printf("%.6f ", L[i][j]);
            printf("\n");
        }
    }

    return 0;
}
```

```
L:
2.828427 0.000000 0.000000
3.535534 5.338539 0.000000
-2.828427 3.933660 1.589439
```

### Factorización QR

```cpp
template <class T>
void qr_decomposition(const matrix<T> &A, matrix<T> &Q, matrix<T> &R);
```

Calcula la factorización QR de una matriz rectangular. Esta factorización viene dada por 

![A = QR](https://render.githubusercontent.com/render/math?math=A%20%3D%20QR)

donde ![Q](https://render.githubusercontent.com/render/math?math=Q) es una matriz cuadrada ortonormal, es decir, ![QQ^T = Q^TQ = I](https://render.githubusercontent.com/render/math?math=QQ%5ET%20%3D%20Q%5ETQ%20%3D%20I), y ![R](https://render.githubusercontent.com/render/math?math=R) es una matriz rectangular cuyo bloque superior forma una matriz triangular superior.

Argumentos:
* `A`: La matriz a factorizar. El número de renglones debe ser mayor o igual al número de columnas.
* `Q`, `R`: Las matrices correspondientes a la factorización.

La función arroja una excepción si las columnas de `Q` son linealmente dependientes.

Ejemplo:

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{ 0, -2},
                        {-2,  0},
                        { 1,  7}};

    matrix<double> Q, R;
    qr_decomposition(A, Q, R);

    bool is_orthogonal = allclose(matmul(Q, transpose(Q)), identity<double>(3));
    bool is_decomposition = allclose(matmul(Q, R), A);

    if (is_orthogonal && is_decomposition) {
        printf("Q:\n");
        for (int i = 0; i < Q.rows(); ++i) {
            for (int j = 0; j < Q.columns(); ++j)
                printf("%.6f ", Q[i][j]);
            printf("\n");
        }

        printf("R:\n");
        for (int i = 0; i < R.rows(); ++i) {
            for (int j = 0; j < R.columns(); ++j)
                printf("%.6f ", R[i][j]);
            printf("\n");
        }
    }

    return 0;
}
```

```
Q:
-0.000000 0.304290 -0.952579
0.894427 -0.426006 -0.136083
-0.447214 -0.852013 -0.272166
R:
-2.236068 -3.130495
0.000000 -6.572671
0.000000 0.000000
```

## Sistemas de ecuaciones

```cpp
template <class T>
matrix<T> solve(const matrix<T> &A, const matrix<T> &b);
```

Resuelve la ecuación lineal ![Ax = b](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b). Arroja una excepción si la matriz es singular.

Argumentos:
* `A`: Lado izquierdo de la ecuación. La matriz debe ser cuadrada.
* `b`: Lado derecho de la ecuación.

Regresa: 
* Una matriz con la solución de la ecuación.

Ejemplo:

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{0, 1, -2},
                        {2, 0,  5},
                        {1, 1,  7}};
    matrix<double> b = {{1}, {3}, {0}};
    matrix<double> x = solve(A, b);

    if (allclose(matmul(A, x), b)) {
        for (int i = 0; i < x.rows(); ++i) {
            for (int j = 0; j < x.columns(); ++j)
                printf("%.6f ", x[i][j]);
            printf("\n");
        }
    }

    return 0;
}
```

```
2.461538
0.230769
-0.384615
```

### Matrices triangulares

```cpp
template <class T>
matrix<T> solve_triangular(const matrix<T> &A, const matrix<T> &b, bool lower, bool unit_diagonal = false);
```

Resuelve la ecuación ![Ax = b](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) suponiendo que ![A](https://render.githubusercontent.com/render/math?math=A) es una matriz triangular. Arroja una excepción si la matriz es singular.

Argumentos:
* `A`: Una matriz triangular.
* `b`: Lado derecho de la ecuación.
* `lower`: Escoger `true` si `A` es triangular inferior. Escoger `false` si `A` es triangular superior.
* `unit_diagonal`: Si `unit_diagonal` es `false`, utiliza los valores en la diagonal de `A`. Si es `true`, supone que hay 1's en la diagonal de `A`.

Regresa: 
* Una matriz con la solución de la ecuación.

Esta función solo hace uso de la región correspondiente de `A`.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
#include "linalg.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<double> A = {{5, 0, 0},
                        {2, 4, 0},
                        {7, -1, 1}};
    matrix<double> b = {{4}, {0}, {-1}};
    matrix<double> x = solve_triangular(A, b, true);

    if (allclose(matmul(A, x), b)) {
        for (int i = 0; i < x.rows(); ++i) {
            for (int j = 0; j < x.columns(); ++j)
                cout << x[i][j] << ' ';
            cout << '\n';
        }
    }

    return 0;
}
```

```
0.8
-0.4
-7
```

### Matrices con factorización LU

```cpp
template <class T, class RandomAccessIterator>
matrix<T> solve_lu(const matrix<T> &LU, RandomAccessIterator P, const matrix<T> &b);
```

Resuelve la ecuación ![Ax = b](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) dada la factorización LU de ![A](https://render.githubusercontent.com/render/math?math=A). Arroja una excepción si la matriz es singular.

Argumentos:
* `LU`, `P`: Los elementos de la factorización.
* `b`: Lado derecho de la ecuación.

Regresa: 
* Una matriz con la solución de la ecuación.

Ejemplo: 

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{ 4,  0, 2},
                        { 2, -1, 5},
                        {-1,  0, 7}};
    matrix<double> b = {{-1}, {0}, {3}};
    
    int P[3];
    matrix<double> LU;
    lu_decomposition(A, LU, P);
    matrix<double> x = solve_lu(LU, P, b);

    if (allclose(matmul(A, x), b)) {
        for (int i = 0; i < x.rows(); ++i) {
            for (int j = 0; j < x.columns(); ++j)
                printf("%.6f ", x[i][j]);
            printf("\n");
        }
    }

    return 0;
}
```

```
-0.433333
0.966667
0.366667
```

### Matrices con factorización de Cholesky.

```cpp
template <class T>
matrix<T> solve_cholesky(const matrix<T> &L, const matrix<T> &b);
```

Resuelve la ecuación ![Ax = b](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) dada la factorización de Cholesky de ![A](https://render.githubusercontent.com/render/math?math=A).

Argumentos:
* `L`: Los elementos de la factorización.
* `b`: Lado derecho de la ecuación.

Regresa: 
* Una matriz con la solución de la ecuación.

Ejemplo: 

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{ 0, -2, 4},
                        {-1,  5, 0},
                        { 0, -1, 5}};
    matrix<double> b = {{0}, {-1}, {3}};
    A = matmul(A, transpose(A));  // Es simetrica y positiva definida siempre y cuando A no sea singular.

    matrix<double> L;
    cholesky_decomposition(A, L);
    matrix<double> x = solve_cholesky(L, b);

    if (allclose(matmul(A, x), b)) {
        for (int i = 0; i < x.rows(); ++i) {
            for (int j = 0; j < x.columns(); ++j)
                printf("%.6f ", x[i][j]);
            printf("\n");
        }
    }

    return 0;
}
```

```
-47.666667
-11.000000
38.333333
```

## Mínimos cuadrados lineales

```cpp
template <class T>
matrix<T> linear_lstsq(const matrix<T> &A, const matrix<T> &b);
```

Resuelve el sistema ![Ax = b](https://render.githubusercontent.com/render/math?math=Ax%20%3D%20b) por mínimos cuadrados lineales, es decir, encontrando el ![x](https://render.githubusercontent.com/render/math?math=x) que minimice el error ![\|Ax - b\|^2](https://render.githubusercontent.com/render/math?math=%5C%7CAx%20-%20b%5C%7C%5E2). El sistema puede tener solución única así como ser subdeterminado o sobredeterminado.

Argumentos:
* `A`: Lado izquierdo de la ecuación.
* `b`: Lado derecho de la ecuación.

Regresa: 
* Una matriz con la solución de la ecuación.

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{0, 1}, {1, 2}, {3, -1}, {4, 0}};
    matrix<double> b = {{-1}, {0}, {3}, {5}};
    matrix<double> x = linear_lstsq(A, b);

    printf("x:\n");
    for (int i = 0; i < x.rows(); ++i) {
        for (int j = 0; j < x.columns(); ++j)
            printf("%.6f ", x[i][j]);
        printf("\n");
    }

    matrix<double> dif = matmul(A, x) - b;
    printf("error: %.6f\n", dot(dif, dif));
    return 0;
}
```

```
x:
1.096774
-0.483871
error: 1.258065
```

## Eigenvalores y eigenvectores

```cpp
template <class T, class OutputIterator>
void eigenvalues(const matrix<T> &A, OutputIterator eigvals, unsigned int max_iter = 1000, const T &tol = 1e-6);
```

Calcula los eigenvalores de una matriz cuadrada. Un eigenvalor es un escalar ![\lambda](https://render.githubusercontent.com/render/math?math=%5Clambda) para el cual existe un vector no cero ![v](https://render.githubusercontent.com/render/math?math=v) tal que ![Av = \lambda v](https://render.githubusercontent.com/render/math?math=Av%20%3D%20%5Clambda%20v). Al vector ![v](https://render.githubusercontent.com/render/math?math=v) se le conoce como eigenvector.

Argumentos:
* `A`: Una matriz cuadrada.
* `eigvals`: Un iterador correspondiente a los eigenvalores. En total habrá `A.rows()` eigenvalores y estos pueden ser complejos.
* `max_iter`: Máximo número de iteraciones.
* `tol`: Tolerancia.

Ejemplo:

```cpp
#include <cstdio>
#include <complex>
#include "matrix.h"
#include "linalg.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<double> A = {{2,  0, -3, 7},
                        {5,  1,  2, 0},
                        {0, -1,  5, 3},
                        {4,  8,  1, 2}};
    complex<double> eigvals[4];
    eigenvalues(A, eigvals);
    for (int i = 0; i < A.rows(); ++i)
        printf("%.6f + %.6fi\n", eigvals[i].real(), eigvals[i].imag());
    return 0;
}
```

```
9.631667 + 0.000000i
5.719553 + 0.000000i
-2.675610 + 4.888767i
-2.675610 + -4.888767i
```

### Eigenpares de una matriz simétrica

```cpp
template <class T, class OutputIterator>
void eigen_symmetric(const matrix<T> &A, matrix<T> &V, OutputIterator D, const T &tol = 1e-6);
```

Calcula la descomposición espectral de una matriz cuadrada. En el caso de una matriz simétrica, esta factorización siempre existe y viene dada por

![A = VDV^T](https://render.githubusercontent.com/render/math?math=A%20%3D%20VDV%5ET)

donde ![V](https://render.githubusercontent.com/render/math?math=V) es una matriz ortonormal cuyas columnas corresponden a los eigenvectores de ![A](https://render.githubusercontent.com/render/math?math=A) y ![D](https://render.githubusercontent.com/render/math?math=D) es una matriz diagonal cuyos elementos en la diagonal corresponden a los eigenvalores de ![A](https://render.githubusercontent.com/render/math?math=A).

Argumentos:
* `A`: La matriz a factorizar. Debe ser simétrica
* `V`: La matriz correspondiente a la factorización.
* `D`: Un iterador correspondiente a la diagonal de ![D](https://render.githubusercontent.com/render/math?math=D).
* `tol`: Tolerancia. 

Esta función solo hace uso de la parte inferior de `A`.

Ejemplo:

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{ 0, -2, 1},
                        {-2,  0, 7},
                        { 1,  7, 3}};

    double D[3];
    matrix<double> V;
    eigen_symmetric(A, V, D);

    if (allclose(matmul(A, V), matmul(V, diagonal<double>(D, D + 3)))) {
        printf("Eigenvectores:\n");
        for (int i = 0; i < V.rows(); ++i) {
            for (int j = 0; j < V.columns(); ++j)
                printf("%.6f ", V[i][j]);
            printf("\n");
        }

        printf("Eigenvalores:\n");
        for (int i = 0; i < A.rows(); ++i)
            printf("%.6f ", D[i]);
        printf("\n");
    }

    return 0;
}
```

```
Eigenvectores:
0.944526 0.323396 -0.057324
-0.215656 0.742308 0.634407
0.247716 -0.586851 0.770871
Eigenvalores:
0.718909 -6.405363 8.686455
```

### Factorización en valores singulares

```cpp
template <class InputIterator, class T = typename std::iterator_traits<InputIterator>::value_type>
matrix<T> diag_svd(unsigned int m, unsigned int n, InputIterator svals);
```

Construye la matriz de valores singulares.

Argumentos:
* `m`, `n`: Las dimensiones de la matriz.
* `svals`: Un iterador correspondiente a los valores singulares.

Regresa:
* La matriz con los valores singulares en la diagonal.

```cpp
template <class T, class OutputIterator>
void svd_decomposition(const matrix<T> &A, matrix<T> &U, OutputIterator S, matrix<T> &V, bool full_matrices = true, const T &tol = 1e-6);
```

Calcula la factorización SVD de una matriz rectangular. Esta factorización viene dada por

![A = U\Sigma V^T](https://render.githubusercontent.com/render/math?math=A%20%3D%20U%5CSigma%20V%5ET)

donde ![U](https://render.githubusercontent.com/render/math?math=U) y ![V](https://render.githubusercontent.com/render/math?math=V) son matrices cuadradas ortonormales y ![\Sigma](https://render.githubusercontent.com/render/math?math=%5CSigma) es una matriz rectangular diagonal. Las columnas de ![U](https://render.githubusercontent.com/render/math?math=U) corresponden a los vectores singulares izquierdos, las columnas de ![V](https://render.githubusercontent.com/render/math?math=V) a los vectores singulares derechos y los elementos en la diagonal de ![\Sigma](https://render.githubusercontent.com/render/math?math=%5CSigma) a los valores singulares.

Argumentos:
* `A`: La matriz a factorizar.
* `U`: La matriz de vectores singulares izquierdos.
* `S`: Un iterador correspondiente a los valores singulares. El número de valores singulares será `min(A.rows(), A.columns())` y estarán dados en orden decreciente.
* `V`: La matriz de vectores singulares derechos.
* `full_matrices`: Si `full_matrices` es `true`, entonces `U`, `S` y `V` corresponderán a matrices de tamaño ![m \times m](https://render.githubusercontent.com/render/math?math=m%20%5Ctimes%20m), ![m \times n](https://render.githubusercontent.com/render/math?math=m%20%5Ctimes%20n) y ![n \times n](https://render.githubusercontent.com/render/math?math=n%20%5Ctimes%20n). Si es `false`, entonces corresponderán a matrices de tamaño ![m \times k](https://render.githubusercontent.com/render/math?math=m%20%5Ctimes%20k), ![k \times k](https://render.githubusercontent.com/render/math?math=k%20%5Ctimes%20k) y ![k \times n](https://render.githubusercontent.com/render/math?math=k%20%5Ctimes%20n) donde ![k = \min(m, n)](https://render.githubusercontent.com/render/math?math=k%20%3D%20%5Cmin(m%2C%20n)).
* `tol`: Tolerancia.

Observación: Escoger `full_matrices = false` podría reducir el tiempo de cálculo.

Ejemplo: 

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;

void print(const matrix<double> &A) {
    for (int i = 0; i < A.rows(); ++i) {
        for (int j = 0; j < A.columns(); ++j)
            printf("%.6f ", A[i][j]);
        printf("\n");
    }
}

int main() {
    matrix<double> A = {{3, 0, -1,  1},
                        {5, 2,  0,  7},
                        {0, 4,  2, -3}};

    double svals[3];
    matrix<double> U, S, V;
    svd_decomposition(A, U, svals, V);
    S = diag_svd(A.rows(), A.columns(), svals);

    if (allclose(matmul(A, V), matmul(U, S))) {
        printf("U:\n");
        print(U);
        printf("S:\n");
        print(S);
        printf("V:\n");
        print(V);
    }

    return 0;
}
```

```
U:
0.281622 0.019253 0.959332
0.931626 0.233856 -0.278182
-0.229702 0.972081 0.047923
S:
9.373136 0.000000 0.000000 0.000000
0.000000 5.076763 0.000000 0.000000
0.000000 0.000000 2.090643 0.000000
V:
0.587103 0.241697 0.711306 0.301557
0.100761 0.858034 -0.174431 -0.472439
-0.079058 0.379161 -0.413025 0.824256
0.799317 -0.248189 -0.541321 -0.080415
```

## Funciones adicionales

### Matriz inversa

```cpp
template <class T>
matrix<T> inverse(const matrix<T> &A);
```

Encuentra la inversa de una matriz cuadrada ![A](https://render.githubusercontent.com/render/math?math=A). La matriz inversa se define como la matriz ![A^{-1}](https://render.githubusercontent.com/render/math?math=A%5E%7B-1%7D) tal que ![AA^{-1} = A^{-1}A = I](https://render.githubusercontent.com/render/math?math=AA%5E%7B-1%7D%20%3D%20A%5E%7B-1%7DA%20%3D%20I). Arroja una excepción si la matriz es singular.

Argumentos:
* `A`: La matriz cuya inversa se quiere calcular.

Regresa:
* La inversa de la matriz.

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{ 0,  3, -2},
                        {-2,  0,  5},
                        { 1, -1,  7}};
    matrix<double> inv = inverse(A);

    if (allclose(matmul(A, inv), identity<double>(3))) {
        for (int i = 0; i < inv.rows(); ++i) {
            for (int j = 0; j < inv.columns(); ++j)
                printf("%.6f ", inv[i][j]);
            printf("\n");
        }
    }

    return 0;
}
```

```
0.094340 -0.358491 0.283019
0.358491 0.037736 0.075472
0.037736 0.056604 0.113208
```

### Determinante

```cpp
template <class T>
T determinant(const matrix<T> &A);
```

Calcula el determinante de una matriz. Una matriz es singular si y sólo si su determinante es 0.

Argumentos:
* `A`: Una matriz cuadrada.

Regresa:
* El determinante de la matriz.

Ejemplo:

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{ 0,  3, -2},
                        {-2,  0,  5},
                        { 1, -1,  7}};
    printf("%.6f\n", determinant(A));
    return 0;
}
```

```
53.000000
```

### Matriz pseudo-inversa

```cpp
template <class T>
matrix<T> pseudoinverse(const matrix<T> &A);
```

Calcula la pseudoinversa de Moore–Penrose de una matriz rectangular. La pseudoinversa siempre existe, es única y satisface:

1. ![AA^{\dagger}A = A](https://render.githubusercontent.com/render/math?math=AA%5E%7B%5Cdagger%7DA%20%3D%20A)
2. ![A^{\dagger}AA^{\dagger} = A^{\dagger}](https://render.githubusercontent.com/render/math?math=A%5E%7B%5Cdagger%7DAA%5E%7B%5Cdagger%7D%20%3D%20A%5E%7B%5Cdagger%7D)
3. ![(AA^{\dagger})^T = AA^{\dagger}](https://render.githubusercontent.com/render/math?math=(AA%5E%7B%5Cdagger%7D)%5ET%20%3D%20AA%5E%7B%5Cdagger%7D)
4. ![(A^{\dagger}A)^T = A^{\dagger}A](https://render.githubusercontent.com/render/math?math=(A%5E%7B%5Cdagger%7DA)%5ET%20%3D%20A%5E%7B%5Cdagger%7DA)

Cuando la matriz es cuadrada y no-singular, la pseudoinversa coincide con la inversa: ![A^{\dagger} = A^{-1}](https://render.githubusercontent.com/render/math?math=A%5E%7B%5Cdagger%7D%20%3D%20A%5E%7B-1%7D).

Ejemplo:

```cpp
#include <cstdio>
#include "matrix.h"
#include "linalg.h"
using namespace num_met;
int main() {
    matrix<double> A = {{5, 0, -1,  1},
                        {3, 2,  0, -3},
                        {0, 4,  7,  2}};
    matrix<double> pinv = pseudoinverse(A);

    matrix<double> Apinv = matmul(A, pinv);
    matrix<double> pinvA = matmul(pinv, A);
    bool cond1 = allclose(matmul(Apinv, A), A);
    bool cond2 = allclose(matmul(pinvA, pinv), pinv);
    bool cond3 = allclose(transpose(Apinv), Apinv);
    bool cond4 = allclose(transpose(pinvA), pinvA);

    if (cond1 && cond2 && cond3 && cond4) {
        for (int i = 0; i < pinv.rows(); ++i) {
            for (int j = 0; j < pinv.columns(); ++j)
                printf("%.6f ", pinv[i][j]);
            printf("\n");
        }
    }

    return 0;
}
```

```
0.167684 0.043911 0.010878
-0.037808 0.106792 0.052136
-0.019103 0.001327 0.100027
0.142478 -0.218228 0.045635
```