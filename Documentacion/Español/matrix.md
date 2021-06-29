# Matrices

## Contenido

- [Constructores](#Constructores)
- [Destructor](#Destructor)
- [Rutinas para la creación de matrices](#Rutinas-para-la-creación-de-matrices)
- [Operadores de asignación](#Operadores-de-asignación)
- [Sobrecarga de operadores](#Sobrecarga-de-operadores)
- [Indexado básico](#Indexado-básico)
- [Indexado avanzado](#Indexado-avanzado)
- [Lista de métodos](#Lista-de-métodos)
- [Funciones globales](#Funciones-globales)

<hr>

Las matrices son contenedores secuenciales de tamaño fijo en dos dimensiones: 
guardan un número específico de elementos organizados por renglones y columnas.

Para crear una matriz, incluya primero la biblioteca `numcpp` al inicio del 
programa. Las matrices están representadas mediante la clase `matrix`.

#### Ejemplo

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

## Constructores

1. Constructor por defecto. Construye una matriz vacía sin elementos.
```cpp
matrix();
```

2. Constructor por tamaño. Construye una matriz con `m` renglones y `n` 
columnas.
```cpp
matrix(size_t m, size_t n);
```

3. Constructor por relleno. Construye una matriz con `m` renglones y `n` 
columnas, cada elemento inicializado a `val`.
```cpp
matrix(size_t m, size_t n, const T &val);
```

4. Constructor por contenedor. Construye una matriz con `m` renglones y `n` 
columnas, cada elemento inicializado a su elemento correspondiente en el 
contenedor, en el mismo orden.
```cpp
template <class InputIterator> 
matrix(size_t m, size_t n, InputIterator first);
```

5. Constructor por copia. Construye una matriz con una copia de los elementos 
de `A`, en el mismo orden.
```cpp
matrix(const matrix &A);
```

6. Constructor por movimiento. Construye una matriz que adquiere los elementos 
de `A`.
```cpp
matrix(matrix &&A);
```

7. Constructor por lista de listas de inicialización. Construye una matriz con 
una copia de los elementos de `ill`, en el mismo orden.
```cpp
matrix(std::initializer_list< std::initializer_list<T> > ill);
```

## Destructor

Destruye la matriz.
```cpp
~matrix();
```

## Rutinas para la creación de matrices

### Ceros y unos

- **empty**: Crea una nueva matriz con `m` renglones y `n` columnas sin 
inicializar.
```cpp
template<class T = double> 
matrix<T> empty(size_t m, size_t n);
```

- **zeros**: Crea una nueva matriz con `m` renglones y `n` columnas 
inicializando los valores a cero.
```cpp
template<class T = double> 
matrix<T> zeros(size_t m, size_t n);
```

- **ones**: Crea una nueva matriz con `m` renglones y `n` columnas 
inicializando los valores a uno.
```cpp
template<class T = double> 
matrix<T> ones(size_t m, size_t n);
```

- **full**: Crea una nueva matriz con `m` renglones y `n` columnas 
inicializando los valores a `val`.
```cpp
template <class T = double> 
matrix<T> full(size_t m, size_t n, const T &val);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> vacio = np::empty<int>(3, 5);
    np::matrix<int> ceros = np::zeros<int>(3, 5);
    np::matrix<int> unos = np::ones<int>(3, 5);
    np::matrix<int> dos = np::full<int>(3, 5, 2);
    cout << "vacio:\n" << vacio << "\n";
    cout << "ceros:\n" << ceros << "\n";
    cout << "unos:\n" << unos << "\n";
    cout << "dos:\n" << dos << "\n";
    return 0;
}
```

```
[Out] vacio:
      [[ 8198880,     0, 8192336, 0,       0]
       [       0,     0,       0, 0,       0]
       [33554434, 29512, 8198880, 0, 8192336]]
      ceros:
      [[0, 0, 0, 0, 0]
       [0, 0, 0, 0, 0]
       [0, 0, 0, 0, 0]]
      unos:
      [[1, 1, 1, 1, 1]
       [1, 1, 1, 1, 1]
       [1, 1, 1, 1, 1]]
      dos:
      [[2, 2, 2, 2, 2]
       [2, 2, 2, 2, 2]
       [2, 2, 2, 2, 2]]
```

### Desde un archivo de texto

- **load_txt**: Crea una nueva matriz desde un archivo de texto. Opcionalmente 
se puede especificar el delimitador que separa los valores en un mismo renglón.
Por defecto, el delimitador es un espacio en blanco. Adicionalmente se puede 
indicar si el primer renglón del archivo corresponde a un encabezado. En caso 
afirmativo, los valores en el encabezado podrán ser guardados en un arreglo de 
tipo `string`. La función arroja una excepción del tipo `runtime_error` si el 
archivo pasado como argumento no existe.
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

- **save_txt**: Guarda el contenido de una matriz en un archivo de texto. 
Opcionalmente se puede especificar un delimitador que separará los valores en 
un mismo renglón. Por defecto, el delimitador es un espacio en blanco. 
Adicionalmente, se puede especificar los valores que irán como encabezado del 
archivo. La función arroja una excepción del tipo `runtime_error` si el archivo 
pasado como argumento no se pudo abrir.
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

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<string> cols = {"x", "y"};
    np::matrix<int> A = {{1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}};
    np::save_txt("cuadrados.csv", A, ',', cols);

    np::array<string> cols2;
    np::matrix<int> B = np::load_txt<int>("cuadrados.csv", ',', true, cols2);
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
[cuadrados.csv]
x,y
1,1
2,4
3,9
4,16
5,25
```

### Desde un archivo binario

- **load_matrix**: Crea una nueva matriz desde un archivo binario. La función 
arroja una excepción del tipo `runtime_error` si el archivo pasado como 
argumento no existe.
```cpp
template <class T>
matrix<T> load_matrix(const char *file);
```

- **save_matrix**: Guarda el contenido de una matriz en un archivo binario. La 
función arroja una excepción del tipo `runtime_error` si el archivo pasado como 
argumento no se pudo abrir.
```cpp
template <class T>
void save_matrix(const char *file, const matrix<T> &A);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{1, 2, 3, 4, 5},
                         {6, 7, 8, 9, 10}};
    np::save_matrix("numeros-1-al-10.bin", A);
    np::matrix<int> B = np::load_matrix<int>("numeros-1-al-10.bin");
    cout << B << "\n";
    return 0;
}
```

```
[Out] [[1, 2, 3, 4,  5]
       [6, 7, 8, 9, 10]]
```

## Operadores de asignación

1. Asignación por copia. Asigna el contenido de `A` a `*this` después de 
cambiar el tamaño de la matriz (en caso de ser necesario).
```cpp
matrix& operator= (const matrix &A);
```

2. Asignación por movimiento. Adquiere el contenido de `A`.
```cpp
matrix& operator= (matrix &&A);
```

3. Asignación por relleno. Asigna `val` a cada elemento.
```cpp
matrix& operator= (const T &val);
```

## Sobrecarga de operadores

Cada una de estas funciones realiza la operación  respectiva sobre todos los 
elementos de la matriz.

Cuando ambos argumentos izquierdo y derecho son matrices, la operación se 
realiza entre los elementos correspondientes de cada objeto (el primer elemento 
de la matriz de la izquierda con el primero de la derecha, el segundo con el 
segundo, y así sucesivamente). Si los tamaños de las matrices no coinciden, 
la función arroja una excepción del tipo `runtime_error`.

Cuando uno de los argumentos es un valor, la operación se realiza entre todos 
los elementos en la matriz contra ese valor.

### Como función miembro

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

### Operadores unitarios

```cpp
template <class T> matrix<T> operator+ (const matrix<T> &A);
template <class T> matrix<T> operator- (const matrix<T> &A);
template <class T> matrix<T> operator~ (const matrix<T> &A);
matrix<bool> operator! (const matrix<bool> &A);
```

### Como función no miembro

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

### Entrada/Salida
```cpp
template <class T>
std::ostream& operator<< (std::ostream &ostr, const matrix<T> &A);
```
## Indexado básico

### Tamaño

- **rows**: Devuelve el número de renglones en la matriz.
```cpp
size_t rows() const;
```

- **columns**: Devuelve el número de columnas en la matriz.
```cpp
size_t columns() const;
```

### Indexado

Existen dos formas principales de acceder a los elementos de una matriz. La 
primera forma es utilizando el operador `[]` con un índice `i`, lo cual 
devuelve un puntero al comienzo del `i`-ésimo renglón. De esta manera, 
utilizando el operador `[]` una segunda vez con un índice `j` devolverá una 
referencia al elemento en el renglón `i`-ésimo y la columna `j`-ésima.
```cpp
T* operator[] (size_t i);
const T* operator[] (size_t i) const;
```
La segunda forma es utilizando el método `at` con dos índices `i` y `j`, lo 
cual devuelve una referencia al elemento en el renglón `i`-ésimo y columna 
`j`-ésima.
```cpp
T& at(size_t i, size_t j);
const T& at(size_t i, size_t j) const;
```
Note que los índices empiezan desde 0, no 1. Acceder a elementos fuera de la 
matriz tendrá comportamiento indefinido.

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = np::zeros(3, 5);
    cout << "A tiene " << A.rows() << " renglones";
    cout << " y " << A.columns() << " columnas.\n";
    A[0][2] = 1.5;
    A[0][3] = 10.0;
    A[1][1] = -2.1;
    A.at(2, 0) = 0.1;
    A.at(2, 4) = 3.7;
    cout << "Los elementos de A son:\n" << A << "\n";
    return 0;
}
```

```
[Out] A tiene 3 renglones y 5 columnas.
      Los elementos de A son:
      [[  0,    0, 1.5, 10,   0]
       [  0, -2.1,   0,  0,   0]
       [0.1,    0,   0,  0, 3.7]]
```

## Indexado avanzado

### Slices y sub-matrices

Una sub-matriz es una clase intermedia devuelta por el método `at` de una 
matriz cuando se utilizan slices. Hace referencia a los elementos en la matriz 
que son seleccionados por los sub-índices, y sobrecarga los operadores de 
asignación y asignación compuesta, permitiendo acceder directamente a los 
elementos en la selección. El objeto puede ser convertido en una matriz, 
generando un nuevo objeto con copias de los elementos referenciados.
```cpp
submatrix<T> at(slice i, slice j);
const submatrix<T> at(slice i, slice j) const;
```

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

#### Ejemplo

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
    cout << "Los elementos de A son:\n" << A << "\n";

    // Selecciona los renglones 0, 1, 2, 3 y las columnas 1, 3, 5.
    np::submatrix<int> sub = A.at(np::slice(4), np::slice(1, 6, 2));
    cout << "La submatriz tiene " << sub.rows() << " renglones";
    cout << " y " << sub.columns() << " columnas.\n";
    cout << "Los elementos de la submatriz son:\n" << sub.copy() << "\n";
    sub = 0;
    cout << "Los elementos de A son ahora:\n" << A << "\n";

    return 0;
}
```

```
[Out] Los elementos de A son:
      [[7, 0, -1, 14,  3,  0]
       [5, 3, 10,  2,  0, -1]
       [0, 1,  7, -5,  9,  3]
       [1, 5,  0,  0, -2,  9]]
      La submatriz tiene 4 renglones y 3 columnas.
      Los elementos de la submatriz son:
      [[0, 14,  0]
       [3,  2, -1]
       [1, -5,  3]
       [5,  0,  9]]
      Los elementos de A son ahora:
      [[7, 0, -1, 0,  3, 0]
       [5, 0, 10, 0,  0, 0]
       [0, 0,  7, 0,  9, 0]
       [1, 0,  0, 0, -2, 0]]
```

### Arreglos de índices

Al indexar una matriz mediante un arreglo de índices (de tipo `size_t`) en cada 
eje, el operador devuelve un objeto de tipo  `index_subarray<T>` representando 
un subarreglo con los elementos especificados por los arreglos de índices.
```cpp
index_subarray<T> at(const array<size_t> &i, const array<size_t> &j);
const index_subarray<T> at(const array<size_t> &i, const array<size_t> &j) const;
```

#### Ejemplo

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
    cout << "Los elementos de A son:\n" << A << "\n";

    // Selecciona las posiciones (0, 0), (0, 2), (0, 5), (2, 1), (3, 0), (3, 4)
    np::array<size_t> i = {0, 0, 0, 2, 3, 3};
    np::array<size_t> j = {0, 2, 5, 1, 0, 4};
    np::index_subarray<int> sub = A.at(i, j);
    cout << "Los elementos seleccionados son:\n" << sub.copy() << "\n";
    sub = 0;
    cout << "Los elementos de A son ahora:\n" << A << "\n";

    return 0;
}
```

```
[Out] Los elementos de A son:
      [[7, 0, -1, 14,  3,  0]
       [5, 3, 10,  2,  0, -1]
       [0, 1,  7, -5,  9,  3]
       [1, 5,  0,  0, -2,  9]]
      Los elementos seleccionados son:
      [7, -1, 0, 1, 1, -2]
      Los elementos de A son ahora:
      [[0, 0,  0, 14, 3,  0]
       [5, 3, 10,  2, 0, -1]
       [0, 0,  7, -5, 9,  3]
       [0, 5,  0,  0, 0,  9]]
```

### Matriz booleana

Al indexar una matriz mediante una matriz booleana, el operador devuelve un 
objeto de tipo  `mask_subarray<T>` representando un subarreglo con los 
elementos seleccionados por la máscara booleana.
```cpp
mask_subarray<T> at(const matrix<bool> &mask);
const mask_subarray<T> at(const matrix<bool> &mask) const;
```

#### Ejemplo

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
    cout << "Los elementos de A son:\n" << A << "\n";

    // Selecciona las posiciones con valores negativos.
    np::matrix<bool> mascara = A < 0;
    np::mask_subarray<int> sub = A.at(mascara);
    cout << "Los elementos seleccionados son:\n" << sub.copy() << "\n";
    sub = 0;
    cout << "Los elementos de A son ahora:\n" << A << "\n";

    return 0;
}
```

```
[Out] Los elementos de A son:
      [[7, 0, -1, 14,  3,  0]
       [5, 3, 10,  2,  0, -1]
       [0, 1,  7, -5,  9,  3]
       [1, 5,  0,  0, -2,  9]]
      Los elementos seleccionados son:
      [-1, -1, -5, -2]
      Los elementos de A son ahora:
      [[7, 0,  0, 14, 3, 0]
       [5, 3, 10,  2, 0, 0]
       [0, 1,  7,  0, 9, 3]
       [1, 5,  0,  0, 0, 9]]
```

## Lista de métodos

### `apply` 

Sustituye cada elemento de la matriz con el resultado de evaluar una función en 
el elemento correspondiente.
```cpp
template <class Function = T(T)>
void apply(Function f);
```

#### Ejemplo

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

En su primera versión, devuelve el índice del mayor valor contenido en la 
matriz. En su segunda versión, devuelve el índice del mayor valor contenido en 
la matriz a lo largo del eje indicado.
```cpp
std::pair<size_t, size_t> argmax() const;
array<size_t> argmax(size_t axis) const;
```

#### Ejemplo

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

En su primera versión, devuelve el índice del menor valor contenido en la 
matriz. En su segunda versión, devuelve el índice del menor valor contenido en 
la matriz a lo largo del eje indicado.
```cpp
std::pair<size_t, size_t> argmin() const;
array<size_t> argmin(size_t axis) const;
```

#### Ejemplo

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

Crea una copia de la matriz, haciendo la conversión al tipo de dato 
especificado.
```cpp
template <class U> matrix<U> astype() const;
```

#### Ejemplo

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

Restringe los valores de una matriz. Dado un intervalo, valores  fuera del 
intervalo son restringidos a los extremos del intervalo.
```cpp
void clip(const T &a_min, const T &a_max);
```

#### Ejemplo

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

Devuelve el número de columnas en la matriz.
```cpp
size_t columns() const;
```

### `data`

Devuelve un puntero a la memoria del arreglo utilizado internamente por la 
matriz.
```cpp
T* data();
const T* data() const;
```

### `dot`

En su primera versión, devuelve la multiplicación matriz-vector entre `*this` y 
`v`. En su segunda versión, devuelve la multiplicación de matrices entre 
`*this` y `A`. 
```cpp
array<T> dot(const array<T> &v) const;
matrix dot(const matrix &A) const;
```
La multiplicación de matrices entre dos matrices *A* y *B* está 
definida como la matriz cuyos elementos son

![$c_{ij} = \sum_k a_{ik}b_{kj}$](https://render.githubusercontent.com/render/math?math=c_%7Bij%7D%20%3D%20%5Csum_k%20a_%7Bik%7Db_%7Bkj%7D)

Esto es, la entrada *(i, j)* corresponde al producto punto entre el renglón 
*i*-ésimo de *A* y la columna *j*-ésima de *B*. 

El método arroja una excepción del tipo `runtime_error` si el número de 
columnas de `*this` no coincide con el número de elementos de `v` o el número 
de renglones de `A`.

#### Ejemplo

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

Devuelve una copia de la matriz colapsada en un arreglo.
```cpp
array<T> flatten() const;
```

#### Ejemplo

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

En su primera versión, devuelve el mayor valor contenido en la matriz. En su 
segunda versión, devuelve el mayor valor contenido en la matriz a lo largo del 
eje indicado.
```cpp
T max() const;
array<T> max(size_t axis) const;
```

#### Ejemplo

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

En su primera versión, devuelve la media o promedio de los elementos de una 
matriz. En su segunda versión, devuelve la media o promedio de los elementos de 
una matriz a lo largo del eje indicado.
```cpp
T mean() const;
array<T> mean(size_t axis) const;
```

#### Ejemplo

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

En su primera versión, devuelve el menor valor contenido en la matriz. En su 
segunda versión, devuelve el menor valor contenido en la matriz a lo largo del 
eje indicado.
```cpp
T min() const;
array<T> min(size_t axis) const;
```

#### Ejemplo

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

En su primera versión, devuelve el producto de los elementos de una matriz. En 
su segunda versión, devuelve el producto de los elementos de una matriz a lo 
largo del eje indicado.
```cpp
T prod() const;
array<T> prod(size_t axis) const;
```

#### Ejemplo 

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

Cambia el tamaño de la matriz a `m` renglones y `n` columnas. 

Si el número total de elementos en la matriz se mantiene constante, el 
contenido de la matriz es reacomodado para ajustarse al nuevo tamaño.

Si el número total de elementos en la matriz disminuye, el contenido de la 
matriz es reducido eliminando del final tantos elementos sean necesarios 
para alcanzar el nuevo tamaño.

Si el número total de elementos en la matriz incrementa, el contenido de la 
matriz es expandido agregando al final tantos elementos sean necesarios para 
alcanzar el nuevo tamaño.
```cpp
void resize(size_t m, size_t n, const T &val = T());
```

#### Ejemplo 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., -9.}, {5., 10.}, {-2., 7.}};
    cout << "Antes:\n" << A << "\n";
    A.resize(1, 6, 0.);
    cout << "Tras reacomodar:\n" << A << "\n";
    A.resize(4, 2, 0.);
    cout << "Tras expandir:\n" << A << "\n";
    A.resize(2, 2, 0.);
    cout << "Tras recortar:\n" << A << "\n";
    return 0;
}
```

```
[Out] Antes:
      [[ 1, -9]
       [ 5, 10]
       [-2,  7]]
      Tras reacomodar:
      [[1, -9, 5, 10, -2, 7]]
      Tras expandir:
      [[ 1, -9]
       [ 5, 10]
       [-2,  7]
       [ 0,  0]]
      Tras recortar:
      [[1, -9]
       [5, 10]]
```

### `rows` 

Devuelve el número de renglones en la matriz.
```cpp
size_t rows() const;
```

### `stddev` 

En su primera versión, devuelve la desviación estándar de los elementos de una 
matriz. En su segunda versión, devuelve la desviación estándar de los elementos 
de una matriz a lo largo del eje indicado.
```cpp
T stddev(size_t ddof = 0) const;
array<T> stddev(size_t ddof, size_t axis) const;
```
La desviación estándar está definida como

![$\sqrt{\frac{1}{n - ddof}\sum_{i = 1}^{n} (x_i - \bar{x})^2}$](https://render.githubusercontent.com/render/math?math=%5Csqrt%7B%5Cfrac%7B1%7D%7Bn%20-%20ddof%7D%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20(x_i%20-%20%5Cbar%7Bx%7D)%5E2%7D)

donde 
![$\bar{x}$](https://render.githubusercontent.com/render/math?math=%5Cbar%7Bx%7D) 
es la media y
![$ddof$](https://render.githubusercontent.com/render/math?math=ddof) 
son los grados de libertad.

#### Ejemplo

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

En su primera versión, devuelve la suma de los elementos de una matriz. En su 
segunda versión, devuelve la suma de los elementos de una matriz a lo largo del 
eje indicado.
```cpp
T sum() const;
array<T> sum(size_t axis) const;
```

#### Ejemplo 

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

### `swap` 

Intercambia el contenido de la matriz con otra matriz `A`. Esta función está 
implementada para funcionar en tiempo constante.
```cpp
void swap(matrix &A);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<double> A = {{1., -9., 5., 10., -2.}};
    np::matrix<double> B = {{1, 2}, {3, 4}};
    cout << "Antes:\n" << A << "\n" << B << "\n";
    A.swap(B);
    cout << "Despues:\n" << A << "\n" << B << "\n";
    return 0;
}
```

```
[Out] Antes:
      [[1, -9, 5, 10, -2]]
      [[1, 2]
       [3, 4]]
      Despues:
      [[1, 2]
       [3, 4]]
      [[1, -9, 5, 10, -2]]
```

### `trace`

Devuelve la suma a lo largo de la diagonal de la matriz con el desplazamiento 
dado, es decir, la suma de los elementos `a[i][i + offset]` para todo `i`. 
`offset` puede ser tanto positivo como negativo. Por defecto, `offset = 0`.
```cpp
T trace(int offset = 0) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{3, 8, -1, 5},
                         {0, 5, 1, 2},
                         {1, -2, 0, 5},
                         {7, 0, 5, 9}};
    cout << A.trace() << "\n";
    cout << A.trace(1) << "\n";
    cout << A.trace(-1) << "\n";
    return 0;
}
```

```
[Out] 17
      14
      3
```

### `transpose`

Devuelve una copia de la matriz transpuesta. 
```cpp
matrix<T> transpose() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{3, 8, -1, 5},
                         {1, -2, 0, 5}};
    cout << A.transpose() << "\n";
    return 0;
}
```

```
[Out] [[ 3,  1]
       [ 8, -2]
       [-1,  0]
       [ 5,  5]]
```

### `var` 

En su primera versión, devuelve la varianza de los elementos de una matriz. En 
su segunda versión, devuelve la varianza de los elementos de una matriz a lo 
largo del eje indicado.
```cpp
T var(size_t ddof = 0) const;
array<T> var(size_t ddof, size_t axis) const;
```
La varianza está definida como

![$S^2 = \frac{1}{n - ddof}\sum_{i = 1}^{n} (x_i - \bar{x})^2$](https://render.githubusercontent.com/render/math?math=S%5E2%20%3D%20%5Cfrac%7B1%7D%7Bn%20-%20ddof%7D%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20(x_i%20-%20%5Cbar%7Bx%7D)%5E2)

donde 
![$\bar{x}$](https://render.githubusercontent.com/render/math?math=%5Cbar%7Bx%7D) 
es la media y
![$ddof$](https://render.githubusercontent.com/render/math?math=ddof) 
son los grados de libertad.

#### Ejemplo

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

## Funciones globales

### `allclose`

Devuelve `true` si los elementos de dos matrices son iguales dentro de una 
cierta tolerancia. Si la siguiente ecuación se cumple para todos los elementos,
la función devuelve `true`: 
`abs(A[i][j] - B[i][j]) <= atol + rtol*abs(B[i][j])`, donde `atol` es la 
tolerancia absoluta y `rtol` es la tolerancia relativa.
```cpp
template <class T>
bool allclose(
    const matrix<T> &A, const matrix<T> &B,
    const T &atol = 1e-8, const T &rtol = 1e-5
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
    np::matrix<double> A = {{1e10}, {1e-7}};
    np::matrix<double> B = {{1.00001e10}, {1e-8}};
    cout << boolalpha << np::allclose(A, B) << "\n";
    A = {{1e10}, {1e-8}};
    B = {{1.00001e10}, {1e-9}};
    cout << np::allclose(A, B) << "\n";
    return 0;
}
```

```
[Out] false
      true
```

### `apply` 

En su primera versión, devuelve una matriz donde cada elemento está 
inicializado con el resultado de evaluar una función `f` en los elementos 
correspondientes de `A`. A diferencia del método `A.apply()`, esta función no
modifica los elementos de `A`.

En su segunda versión, devuelve una matriz donde cada elemento está 
inicializado con el resultado de evaluar una función `f` en los elementos 
correspondientes de `A` y `B`; en los elementos de `A` contra `val`; y en 
`val`contra los elementos de `A` según sea el caso.
```cpp
template <class T, class Function = T(T)>
matrix<T> apply(Function f, const matrix<T> &A);

template <class T, class Function = T(T, T)>
matrix<T> apply(Function f, const matrix<T> &A, const matrix<T> &B);

template <class T, class Function = T(T, T)>
matrix<T> apply(Function f, const matrix<T> &A, const T &val);

template <class T, class Function = T(T, T)>
matrix<T> apply(Function f, const T &val, const matrix<T> &A);
```

#### Ejemplo

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
    np::matrix<double> x = {{1, 3, 2}, {0, 5, 7}};
    np::matrix<double> y = {{-1, 0, 2}, {5, 3, 1}};
    cout << np::apply(square, x) << "\n";
    cout << np::apply(f, x, y) << "\n";
    return 0;
}
```

```
[Out] [[1,  9,  4]
       [0, 25, 49]]
      [[-1,  3, 6]
       [10, 11, 9]]
```

### `argmax`

En su primera versión, devuelve el índice del mayor valor contenido en la 
matriz. En su segunda versión, devuelve el índice del mayor valor contenido en 
la matriz a lo largo del eje indicado.
```cpp
template <class T>
std::pair<size_t, size_t> argmax(const matrix<T> &A);

template <class T>
array<size_t> argmax(const matrix<T> &A, size_t axis);
```

### `argmin`

En su primera versión, devuelve el índice del menor valor contenido en la 
matriz. En su segunda versión, devuelve el índice del menor valor contenido en 
la matriz a lo largo del eje indicado.
```cpp
template <class T>
std::pair<size_t, size_t> argmin(const matrix<T> &A);

template <class T>
array<size_t> argmin(const matrix<T> &A, size_t axis);
```

### `clip` 

Devuelve una matriz donde cada elemento está inicializado con el resultado de 
restringir los valores de una matriz `A`. A diferencia del método `A.clip()`, 
esta función no modifica los elementos de `A`.
```cpp
template <class T>
matrix<T> clip(const matrix<T> &A, const T &a_min, const T &a_max);
```

### `column_stack`

Concatena arreglos y matrices verticalmente.
```cpp
template <class T>
matrix<T> column_stack(const array<T> &v, const array<T> &w);

template <class T>
matrix<T> column_stack(const matrix<T> &A, const array<T> &v);

template <class T>
matrix<T> column_stack(const array<T> &v, const matrix<T> &A);

template <class T>
matrix<T> column_stack(const matrix<T> &A, const matrix<T> &B);
```

#### Ejemplo 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{5, 3}, {0, 1}, {-1, 2}};
    np::matrix<int> B = {{7, 1}, {-2, 5}, {3, 0}};
    np::array<int> v = {1, 2, 3};
    np::array<int> w = {0, -1, 5};
    cout << np::column_stack(v, w) << "\n";
    cout << np::column_stack(A, v) << "\n";
    cout << np::column_stack(v, A) << "\n";
    cout << np::column_stack(A, B) << "\n";
    return 0;
}
```

```
[Out] [[1,  0]
       [2, -1]
       [3,  5]]
      [[ 5, 3, 1]
       [ 0, 1, 2]
       [-1, 2, 3]]
      [[1,  5, 3]
       [2,  0, 1]
       [3, -1, 2]]
      [[ 5, 3,  7, 1]
       [ 0, 1, -2, 5]
       [-1, 2,  3, 0]]
```

### `corrcoef`

En su primera versión, devuelve el coeficiente de correlación entre dos 
arreglos `x` y `y`. En su segunda versión, devuelve la matriz de coeficientes 
de correlación de `X`. Si `rowvar = true`, entonces cada renglón de `X` 
representa una variable, con observaciones en las columnas. De lo contrario, 
cada columna representa una variable, con observaciones en los renglones.
```cpp
template <class T>
T corrcoef(const array<T> &x, const array<T> &y);

template <class T>
matrix<T> corrcoef(const matrix<T> &X, bool rowvar = true);
```
El coeficiente de correlación entre dos variables está definido como

![$\rho_{X, Y} = \frac{\sigma_{X, Y}}{\sigma_X \sigma_Y}$](https://render.githubusercontent.com/render/math?math=%5Crho_%7BX%2C%20Y%7D%20%3D%20%5Cfrac%7B%5Csigma_%7BX%2C%20Y%7D%7D%7B%5Csigma_X%20%5Csigma_Y%7D)

donde 
![$\sigma_{X, Y}$](https://render.githubusercontent.com/render/math?math=%5Csigma_%7BX%2C%20Y%7D)
es la covarianza y 
![$\sigma_X$](https://render.githubusercontent.com/render/math?math=%5Csigma_X)
,
![$\sigma_Y$](https://render.githubusercontent.com/render/math?math=%5Csigma_Y)
son las desviaciones estándar.

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> x = np::arange(0., 10., 0.1);
    np::array<double> y = np::square(x);
    np::matrix<double> X = np::column_stack(x, y);

    cout << "corr(x, y): " << np::corrcoef(x, y) << "\n";
    cout << "Matriz de correlacion:\n" << np::corrcoef(X, false) << "\n";

    return 0;
}
```

```
[Out] corr(x, y): 0.967644
      Matriz de correlacion:
      [[         1, 0.96764439]
       [0.96764439,          1]]
```

### `cov`

En su primera versión, devuelve la covarianza entre dos arreglos `x` y `y`. En 
su segunda versión, devuelve la matriz de varianzas y covarianzas de `X`. 
Si `rowvar = true`, entonces cada renglón de `X` representa una variable, con 
observaciones en las columnas. De lo contrario, cada columna representa una 
variable, con observaciones en los renglones.
```cpp
template <class T>
T cov(const array<T> &x, const array<T> &y, size_t ddof = 0);

template <class T>
matrix<T> cov(const matrix<T> &X, bool rowvar = true, size_t ddof = 0);
```
La covarianza entre dos variables 
![$X = (x_1, x_2, \ldots, x_n)$](https://render.githubusercontent.com/render/math?math=X%20%3D%20(x_1%2C%20x_2%2C%20%5Cldots%2C%20x_n)) 
y 
![$Y = (y_1, y_2, \ldots, y_n)$](https://render.githubusercontent.com/render/math?math=Y%20%3D%20(y_1%2C%20y_2%2C%20%5Cldots%2C%20y_n)) 
está definida como

![$\sigma_{X, Y} = \frac{1}{n - ddof}\sum_{i = 1}^{n} (x_i - \bar{x})(y_i - \bar{y})$](https://render.githubusercontent.com/render/math?math=%5Csigma_%7BX%2C%20Y%7D%20%3D%20%5Cfrac%7B1%7D%7Bn%20-%20ddof%7D%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20(x_i%20-%20%5Cbar%7Bx%7D)(y_i%20-%20%5Cbar%7By%7D))

donde 
![$\bar{x}$](https://render.githubusercontent.com/render/math?math=%5Cbar%7Bx%7D) 
,
![$\bar{y}$](https://render.githubusercontent.com/render/math?math=%5Cbar%7By%7D) 
son las medias y
![$ddof$](https://render.githubusercontent.com/render/math?math=ddof) 
son los grados de libertad.

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> x = np::arange(0., 10., 0.1);
    np::array<double> y = np::square(x);
    np::matrix<double> X = np::column_stack(x, y);

    cout << "cov(x, y): " << np::cov(x, y) << "\n";
    cout << "var(x): " << np::var(x) << "\n";
    cout << "var(y): " << np::var(y) << "\n";
    cout << "Matriz de covarianzas:\n" << np::cov(X, false) << "\n";

    return 0;
}
```

```
[Out] cov(x, y): 82.4918
      var(x): 8.3325
      var(y): 872.196
      Matriz de covarianzas:
      [[  8.3325,  82.49175]
       [82.49175, 872.19611]]
```

### `diagonal`

En su primera versión, devuelve un arreglo con los elementos en la diagonal de 
una matriz. En su segunda versión, devuelve una matriz con los elementos en la 
diagonal inicializados a los valores de un arreglo. Un valor de cero para 
`offset` representa la diagonal principal, un valor positivo representa una 
diagonal superior, y un valor negativo una diagonal inferior. Por defecto, 
`offset = 0`.
```cpp
template <class T>
array<T> diagonal(const matrix<T> &A, int offset = 0);

template <class T>
matrix<T> diagonal(const array<T> &v, int offset = 0);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = {{1, 8, 7, 3},
                         {4, -1, 2, 0},
                         {0, 5, 3, 2},
                         {7, 2, 0, 5}};
    np::array<int> v = {1, 2, 3};
    cout << np::diagonal(A) << "\n";
    cout << np::diagonal(A, -1) << "\n";
    cout << np::diagonal(v) << "\n";
    cout << np::diagonal(v, 1) << "\n";
    return 0;
}
```

```
[Out] [1, -1, 3, 5]
      [4, 5, 0]
      [[1, 0, 0]
       [0, 2, 0]
       [0, 0, 3]]
      [[0, 1, 0, 0]
       [0, 0, 2, 0]
       [0, 0, 0, 3]
       [0, 0, 0, 0]]
```

### `dot`

En su primera versión, devuelve el producto punto entre dos vectores. En su 
segunda versión, devuelve el producto vector-matriz entre un vector renglón y 
una matriz. En su tercera versión, devuelve el producto matriz-vector entre una 
matriz y un vector columna. En su cuarta versión, devuelve la multiplicación de 
matrices entre dos matrices.
```cpp
template <class T>
T dot(const array<T> &v, const array<T> &w);

template <class T>
array<T> dot(const array<T> &v, const matrix<T> &A);

template <class T>
array<T> dot(const matrix<T> &A, const array<T> &v);

template <class T>
matrix<T> dot(const matrix<T> &A, const matrix<T> &B);
```

### `empty` 

Crea una nueva matriz con `m` renglones y `n` columnas sin inicializar.
```cpp
template<class T = double> 
matrix<T> empty(size_t m, size_t n);
```

### `eye`

Devuelve una matriz con unos en la diagonal y ceros en las demás posiciones. Un 
valor de cero para `offset` representa la diagonal principal, un valor positivo 
representa una diagonal superior, y un valor negativo una diagonal inferior. 
Por defecto, `offset = 0`.
```cpp
template <class T = double>
matrix<T> eye(size_t m, size_t n, int offset = 0);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::matrix<int> A = np::eye<int>(3, 3);
    np::matrix<int> B = np::eye<int>(3, 4, 1);
    cout << A << "\n" << B << "\n";
    return 0;
}
```

```
[Out] [[1, 0, 0]
       [0, 1, 0]
       [0, 0, 1]]
      [[0, 1, 0, 0]
       [0, 0, 1, 0]
       [0, 0, 0, 1]]
```

### `full`

Crea una nueva matriz con `m` renglones y `n` columnas inicializando los 
valores a `val`.
```cpp
template <class T = double> 
matrix<T> full(size_t m, size_t n, const T &val);
```

### `load_matrix` 

Crea una nueva matriz desde un archivo binario. La función arroja una excepción 
del tipo `runtime_error` si el archivo pasado como argumento no existe.
```cpp
template <class T>
matrix<T> load_matrix(const char *file);
```

### `load_txt` 

Crea una nueva matriz desde un archivo de texto. Opcionalmente se puede 
especificar el delimitador que separa los valores en un mismo renglón. Por 
defecto, el delimitador es un espacio en blanco. Adicionalmente se puede 
indicar si el primer renglón del archivo corresponde a un encabezado. En caso 
afirmativo, los valores en el encabezado podrán ser guardados en un arreglo de 
tipo `string`. La función arroja una excepción del tipo `runtime_error` si el 
archivo pasado como argumento no existe.
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

### `max`

En su primera versión, devuelve el mayor valor contenido en la matriz. En su 
segunda versión, devuelve el mayor valor contenido en la matriz a lo largo del 
eje indicado.
```cpp
template <class T>
T max(const matrix<T> &A);

template <class T>
array<T> max(const matrix<T> &A, size_t axis);
```

### `mean` 

En su primera versión, devuelve la media o promedio de los elementos de una 
matriz. En su segunda versión, devuelve la media o promedio de los elementos de 
una matriz a lo largo del eje indicado.
```cpp
template <class T>
T mean(const matrix<T> &A);

template <class T>
array<T> mean(const matrix<T> &A, size_t axis);
```

### `min`

En su primera versión, devuelve el menor valor contenido en la matriz. En su 
segunda versión, devuelve el menor valor contenido en la matriz a lo largo del 
eje indicado.
```cpp
template <class T>
T min(const matrix<T> &A);

template <class T>
array<T> min(const matrix<T> &A, size_t axis);
```

### `ones` 

Crea una nueva matriz con `m` renglones y `n` columnas inicializando los 
valores a uno.
```cpp
template<class T = double> 
matrix<T> ones(size_t m, size_t n);
```

### `prod`

En su primera versión, devuelve el producto de los elementos de una matriz. En 
su segunda versión, devuelve el producto de los elementos de una matriz a lo 
largo del eje indicado.
```cpp
template <class T>
T prod(const matrix<T> &A);

template <class T>
array<T> prod(const matrix<T> &A, size_t axis);
```

### `save_matrix` 

Guarda el contenido de una matriz en un archivo binario. La función arroja una 
excepción del tipo `runtime_error` si el archivo pasado como argumento no se 
pudo abrir.
```cpp
template <class T>
void save_matrix(const char *file, const matrix<T> &A);
```

### `save_txt` 

Guarda el contenido de una matriz en un archivo de texto. Opcionalmente se 
puede especificar un delimitador que separará los valores en un mismo renglón. 
Por defecto, el delimitador es un espacio en blanco. Adicionalmente, se puede 
especificar los valores que irán como encabezado del archivo. La función arroja 
una excepción del tipo `runtime_error` si el archivo pasado como argumento no 
se pudo abrir.
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

### `stddev` 

En su primera versión, devuelve la desviación estándar de los elementos de una 
matriz. En su segunda versión, devuelve la desviación estándar de los elementos 
de una matriz a lo largo del eje indicado.
```cpp
template <class T>
T stddev(const matrix<T> &A, size_t ddof = 0);

template <class T>
array<T> stddev(const matrix<T> &A, size_t ddof, size_t axis);
```

### `sum`

En su primera versión, devuelve la suma de los elementos de una matriz. En su 
segunda versión, devuelve la suma de los elementos de una matriz a lo largo del 
eje indicado.
```cpp
template <class T>
T sum(const matrix<T> &A);

template <class T>
array<T> sum(const matrix<T> &A, size_t axis);
```

### `swap` 

Intercambia el contenido de dos matrices. Esta función está implementada para 
funcionar en tiempo constante.
```cpp
template <class T>
void swap(matrix<T> &A, matrix<T> &B);
```

### `trace`

Devuelve la suma a lo largo de la diagonal de la matriz con el desplazamiento 
dado, es decir, la suma de los elementos `A[i][i + offset]` para todo `i`. 
`offset` puede ser tanto positivo como negativo. Por defecto, `offset = 0`.
```cpp
template <class T>
T trace(const matrix<T> &A, int offset = 0);
```

### `transpose`

Devuelve una copia de la matriz transpuesta. 
```cpp
template <class T>
matrix<T> transpose(const matrix<T> &A);
```

### `var` 

En su primera versión, devuelve la varianza de los elementos de una matriz. En 
su segunda versión, devuelve la varianza de los elementos de una matriz a lo 
largo del eje indicado.
```cpp
template <class T>
T var(const matrix<T> &A, size_t ddof = 0);

template <class T>
array<T> var(const matrix<T> &A, size_t ddof, size_t axis);
```

### `zeros` 

Crea una nueva matriz con `m` renglones y `n` columnas inicializando los 
valores a cero.
```cpp
template<class T = double> 
matrix<T> zeros(size_t m, size_t n);
```