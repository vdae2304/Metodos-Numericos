# Clase `matrix`.

Biblioteca: `matrix.h`

Requisitos: C++11

```cpp
template <class T> class matrix;
```

Una matriz es un arreglo bidimensional de números. Las matrices tienen múltiples aplicaciones y sirven, por ejemplo, para representar los coeficientes de los sistemas de ecuaciones lineales. Pueden sumarse, multiplicarse y descomponerse de varias formas, lo que las hace un concepto clave en álgebra lineal.

## Tipos miembro

```cpp
typedef T value_type;
typedef T& reference;
typedef const T& const_reference;
typedef T* iterator;
typedef const T* const_iterator;
typedef unsigned int size_type;
```

## Funciones miembro

### Constructores

```cpp
(1) matrix();
(2) matrix(size_type m, size_type n);
    matrix(size_type m, size_type n, const T &val);
(3) template <class InputIterator> 
    matrix(size_type m, size_type n, InputIterator first);
(4) matrix(const matrix &A);
(5) matrix(std::initializer_list< std::initializer_list<T> > il);
```

Construye una matriz de alguna de las siguientes formas:
1. Por defecto. Construye una matriz vacía, sin elementos.
2. Construye una matriz de `m` renglones y `n` columnas. Cada elemento es una copia de `val` (cuando se proporciona).
3. Construye una matriz de `m` renglones y `n` columnas. Cada elemento es una copia de los elementos apuntados por `first`.
4. Construye una matriz como una copia de los elementos de `A`.
5. Construye una matriz como una copia de los elementos de `il`.

Ejemplo:

```cpp
#include "matrix.h"
using namespace num_met;
int main() {
    int init[] = {1, 2, 3, 4};
    matrix<int> first;                         // (vacio)

    matrix<int> second(2, 3, 0);               // 0 0 0
                                               // 0 0 0

    matrix<int> third(2, 2, init);             // 1 2
                                               // 3 4

    matrix<int> fourth(third);                 // 1 2
                                               // 3 4

    matrix<int> fifth({{1, 3, 5}, {2, 4, 6}}); // 1 3 5
                                               // 2 4 6
    return 0;
}
```

### Destructor

```cpp
~matrix();
```

Destruye la matriz.

### Asignación

```cpp
(1) matrix& operator=(const matrix &A);
(2) matrix& operator=(std::initializer_list< std::initializer_list<T> > il);
```

Sobrecarga el operador `=` para asignar nuevo contenido a la matriz, reemplazando el contenido actual.

### Iteradores

```cpp
(1) iterator begin();
    const_iterator begin() const;
(2) iterator end();
    const_iterator end() const;
```

Los iteradores sirven para recorrer sobre los elementos de una matriz. El recorrido siempre es primero por renglones y luego por columnas.
1. Regresa un iterador al primer elemento de la matriz.
2. Regresa un iterador posterior al último elemento de la matriz.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> mymatrix = {{4, 3, 1},
                            {2, 5, 6}};
    for (matrix<int>::iterator it = mymatrix.begin(); it != mymatrix.end(); ++it)
        cout << *it << ' ';
    cout << '\n';
    return 0;
}
```

```
4 3 1 2 5 6
```

### Tamaño

```cpp
(1) size_type rows() const;
(2) size_type columns() const;
(3) void resize(size_type m, size_type n, const T &val = T());
```

1. Regresa el número de renglones de la matriz.
2. Regresa el número de columnas de la matriz.
3. Modifica las dimensiones de la matriz. Si el número total de elementos es distinto que el anterior, los elementos son realojados en memoria. Si además el nuevo tamaño es mayor que el anterior, los nuevos elementos son inicializados como copias de `val`.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    int init[] = {3, 4, 2, 1};
    matrix<int> mymatrix(2, 2, init); // 3 4
                                      // 2 1
    cout << "Antes: " << mymatrix.rows() << ", " << mymatrix.columns() << '\n';
    
    mymatrix.resize(2, 3, 0);         // 3 4 2
                                      // 1 0 0
    cout << "Despues: " << mymatrix.rows() << ", " << mymatrix.columns() << '\n';
    return 0;
}
```

```
Antes: 2, 2
Despues: 2, 3
```

### Acceso a los elementos

```cpp
(1) T& at(size_type i, size_type j);
    const T& at(size_type i, size_type j) const;
(2) T* operator[](size_type i);
    const T* operator[](size_type i) const;
```

Tenemos dos formas para acceder individualmente a los elementos de una matriz:
1. Regresa una referencia al elemento en el renglón `i` y columna `j`.
2. Regresa un apuntador al primer elemento del renglón `i`.

Si bien ambos métodos cumplen al final el mismo propósito, la diferencia está en que `at` arroja una excepción cuando los índices están fuera de rango.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> mymatrix(2, 3, 0);
    mymatrix.at(0, 0) = 1;
    mymatrix[1][2] = -1;
    for (int i = 0; i < mymatrix.rows(); ++i) {
        for (int j = 0; j < mymatrix.columns(); ++j)
            cout << mymatrix[i][j] << ' ';
        cout << '\n';
    }
    return 0;
}
```

```
1 0 0
0 0 -1
```

### swap

```cpp
void swap(matrix &A);
```

Intercambia el contenido con otra matriz. Esta función está optimizada para funcionar en tiempo constante.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> foo = {{2, 6, 1},
                       {5, 3, 4}};
    matrix<int> bar = {{3, 1, 2}};
    foo.swap(bar);

    cout << "foo:\n";
    for (int i = 0; i < foo.rows(); ++i) {
        for (int j = 0; j < foo.columns(); ++j)
            cout << foo[i][j] << ' ';
        cout << '\n';
    }
    
    cout << "bar:\n";
    for (int i = 0; i < bar.rows(); ++i) {
        for (int j = 0; j < bar.columns(); ++j)
            cout << bar[i][j] << ' ';
        cout << '\n';
    }
    return 0;
}
```

```
foo:
3 1 2
bar:
2 6 1
5 3 4
```

## Funciones no miembro

### Matriz identidad

```cpp
template <class T>
matrix<T> identity(unsigned int n);
```

Regresa una matriz de `n` renglones y `n` columnas con 1s en la diagonal y 0s en las demás posiciones.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> I = identity<int>(3);
    for (int i = 0; i < I.rows(); ++i) {
        for (int j = 0; j < I.columns(); ++j)
            cout << I[i][j] << ' ';
        cout << '\n';
    }
    return 0;
}
```

```
1 0 0
0 1 0
0 0 1
```

### Matriz diagonal

```cpp
(1) template <class T>
    matrix<T> diagonal(unsigned int n, const T &val);
(2) template <class InputIterator, class T = typename std::iterator_traits<InputIterator>::value_type>
    matrix<T> diagonal(InputIterator first, InputIterator last);
```

1. Regresa una matriz de `n` renglones y `n` columnas con copias de `val` sobre la diagonal y 0s en las demás posiciones.
2. Regresa una matriz cuadrada con los elementos apuntados por `first` sobre la diagonal y 0s en las demás posiciones. El rango usado es [`first`, `last`).

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    int init[] = {1, 2, 3};
    matrix<int> foo = diagonal(3, -1);
    matrix<int> bar = diagonal(init, init + 3);
    
    cout << "foo:\n";
    for (int i = 0; i < foo.rows(); ++i) {
        for (int j = 0; j < foo.columns(); ++j)
            cout << foo[i][j] << ' ';
        cout << '\n';
    }

    cout << "bar:\n";
    for (int i = 0; i < bar.rows(); ++i) {
        for (int j = 0; j < bar.columns(); ++j)
            cout << bar[i][j] << ' ';
        cout << '\n';
    }
    return 0;
}
```

```cpp
foo:
-1 0 0
0 -1 0
0 0 -1
bar:
1 0 0
0 2 0
0 0 3
```

### Matriz triangular

```cpp
template <class T>
matrix<T> triangular(const matrix<T> &A, bool lower, bool unit_diagonal = false);
```

Regresa una matriz triangular con la mitad de los elementos de otra matriz. Si `lower` es `true`, entonces la matriz será triangular inferior, de lo contrario, será triangular superior. Si `unit_diagonal` es `false`, utiliza los elementos de la diagonal de `A`, de lo contrario, construye la matriz con una diagonal de 1's. La función arroja una excepción si la matriz no es cuadrada.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> mymatrix = {{1, 2, 3},
                            {4, 5, 6},
                            {7, 8, 9}};
    mymatrix = triangular(mymatrix, true);

    for (int i = 0; i < mymatrix.rows(); ++i) {
        for (int j = 0; j < mymatrix.columns(); ++j)
            cout << mymatrix[i][j] << ' ';
        cout << '\n';
    }

    return 0;
}
```

```
1 0 0
4 5 0
7 8 9
```

### Matriz transpuesta

```cpp
template <class T>
matrix<T> transpose(const matrix<T> &A);
```

Regresa una matriz con los renglones y columnas intercambiados.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> mymatrix = {{2, 6, 1},
                            {5, 3, 4}};
    mymatrix = transpose(mymatrix);
    for (int i = 0; i < mymatrix.rows(); ++i) {
        for (int j = 0; j < mymatrix.columns(); ++j)
            cout << mymatrix[i][j] << ' ';
        cout << '\n';
    }
    return 0;
}
```

```
2 5
6 3
1 4
```

### Traza de una matriz

```cpp
template <class T>
T trace(const matrix<T> &A);
```

Regresa la suma de los elementos en la diagonal de una matriz. La función arroja una excepción si la matriz no es cuadrada.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> mymatrix = {{1, 3},
                            {4, 2}};
    cout << trace(mymatrix) << '\n';
    return 0;
}
```

```
3
```

### Producto punto

```cpp
template <class T>
T dot(const matrix<T> &A, const matrix<T> &B);
```

Regresa el producto punto de los datos aplanados de dos matrices. El producto punto de dos vectores se define como 

![v \cdot w = \sum_{i=1}^n v_iw_i = v_1w_1 + \ldots + v_nw_n](https://render.githubusercontent.com/render/math?math=v%20%5Ccdot%20w%20%3D%20%5Csum_%7Bi%3D1%7D%5En%20v_iw_i%20%3D%20v_1w_1%20%2B%20%5Cldots%20%2B%20v_nw_n)

La función arroja una excepción si el número total de elementos en cada matriz es distinto.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> foo = {{10}, {20}, {30}};
    matrix<int> bar = {{1}, {2}, {3}};
    cout << dot(foo, bar) << '\n';
    return 0;
}
```

```
140 
```

### Producto matricial

```cpp
template <class T>
matrix<T> matmul(const matrix<T> &A, const matrix<T> &B);
```

Regresa el producto de dos matrices. La multiplicación de matrices solo está definida cuando el número de columnas de `A` es igual al número de renglones de `B`, y se define como la matriz `C` cuyos elementos están dados por

![c_{ij} = \sum_{k=1}^n a_{ik}b_{kj} = a_{i,1}b_{1,j} + \ldots + a_{i,n}b_{n,j}](https://render.githubusercontent.com/render/math?math=c_%7Bij%7D%20%3D%20%5Csum_%7Bk%3D1%7D%5En%20a_%7Bik%7Db_%7Bkj%7D%20%3D%20a_%7Bi%2C1%7Db_%7B1%2Cj%7D%20%2B%20%5Cldots%20%2B%20a_%7Bi%2Cn%7Db_%7Bn%2Cj%7D)

La función arroja una excepción si `A.columns() != B.rows()`.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> foo = {{5, 3, 0}, 
                       {2, 0, 1},
                       {3, 1, 0}};
    matrix<int> bar = {{0}, 
                       {1}, 
                       {-2}};
    matrix<int> baz = matmul(foo, bar);

    for (int i = 0; i < baz.rows(); ++i) {
        for (int j = 0; j < baz.columns(); ++j)
            cout << baz[i][j] << ' ';
        cout << '\n';
    }
    return 0;
}
```

```
3
-2
1
```


### Igualdad de matrices

```cpp
template <class T>
bool allclose(const matrix<T> &A, const matrix<T> &B, const T &tol = 1e-6);
```

Compara las dimensiones y los elementos de dos matrices y verifica que estos últimos sean iguales hasta una cierta tolerancia. Esta función es útil cuando las matrices guardan números de punto flotante.

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<double> foo(3, 3, 1.0);
    matrix<double> bar(foo);
    cout << boolalpha << allclose(foo, bar) << '\n';
    foo[0][0] += 0.001;
    cout << allclose(foo, bar) << '\n';
    return 0;
}
```

```
true
false
```

## Sobrecarga de operadores

### Asignación compuesta

```cpp
matrix& operator+=(const T &x);
matrix& operator+=(const matrix &A);

matrix& operator-=(const T &x);
matrix& operator-=(const matrix &A);

matrix& operator*=(const T &x);
matrix& operator*=(const matrix &A);

matrix& operator/=(const T &x);
matrix& operator/=(const matrix &A);
```

Asigna a la matriz el resultado de la operación correspondiente. Todas las operaciones, incluyendo multiplicación y división, son realizadas elemento por elemento. En el caso de operaciones entre dos matrices, el operador arroja una excepción cuando los tamaños de las matrices son distintos.

### Operaciones aritméticas

```cpp
template <class T> matrix<T> operator+(const matrix<T> &A, const matrix<T> &B);
template <class T> matrix<T> operator+(const matrix<T> &A, const T &x);
template <class T> matrix<T> operator+(const T &x, const matrix<T> &A);

template <class T> matrix<T> operator-(const matrix<T> &A, const matrix<T> &B);
template <class T> matrix<T> operator-(const matrix<T> &A, const T &x);
template <class T> matrix<T> operator-(const T &x, const matrix<T> &A);

template <class T> matrix<T> operator*(const matrix<T> &A, const matrix<T> &B);
template <class T> matrix<T> operator*(const matrix<T> &A, const T &x);
template <class T> matrix<T> operator*(const T &x, const matrix<T> &A);

template <class T> matrix<T> operator/(const matrix<T> &A, const matrix<T> &B);
template <class T> matrix<T> operator/(const matrix<T> &A, const T &x);
template <class T> matrix<T> operator/(const T &x, const matrix<T> &A);
```

Regresa una matriz con el resultado de la operación correspondiente. De igual manera, todas las operaciones son realizadas elemento por elemento. En el caso de operaciones entre dos matrices, el operador arroja una excepción cuando los tamaños de las matrices son distintos.


### Prefijos

```cpp
template <class T> matrix<T> operator+(const matrix<T> &A);
template <class T> matrix<T> operator-(const matrix<T> &A);
```

Regresa una copia de la matriz o el negativo de esta, elemento por elemento.


# Clase `submatrix`

```cpp
template <class T> class submatrix;
```

Una submatriz es una clase adicional que nos permite modificar regiones específicas de una matriz de manera efectiva.

## Constructor

```cpp
submatrix<T> operator()(size_type i_start, size_type i_end, size_type j_start, size_type j_end);
```

La clase `matrix` sobrecarga el operador `()` que regresa la submatriz cuyos renglones están en el rango [`i_start`, `i_end`) y cuyas columnas están en el rango [`j_start`, `j_end`).

Ejemplo:

```cpp
#include <iostream>
#include "matrix.h"
using namespace std;
using namespace num_met;
int main() {
    matrix<int> mymatrix = {{1, 2, 3},
                            {4, 5, 6},
                            {7, 8, 9}};
    mymatrix(0, 2, 1, 3) = 0;

    for (int i = 0; i < mymatrix.rows(); ++i) {
        for (int j = 0; j < mymatrix.columns(); ++j)
            cout << mymatrix[i][j] << ' ';
        cout << '\n';
    } 

    return 0;
}
```

```
1 0 0
4 0 0
7 8 9
```

## Funciones miembro

```cpp
matrix<T> to_matrix();
```

Regresa una copia de los elementos en la submatriz.

### Sobrecarga de operadores

```cpp
void operator=(const T &val);
void operator=(const matrix<T> &A);

void operator+=(const T &val);
void operator+=(const matrix<T> &A);

void operator-=(const T &val);
void operator-=(const matrix<T> &A);

void operator*=(const T &val);
void operator*=(const matrix<T> &A);

void operator/=(const T &val);
void operator/=(const matrix<T> &A);
```

Asigna a los elementos de la submatriz el resultado de la operación correspondiente. Todas las operaciones, incluyendo multiplicación y división, son realizadas elemento por elemento. En el caso donde se opera con otra matriz, el operador arroja una excepción si la matriz proporcionada es más pequeña.