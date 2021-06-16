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

- **shape**: Devuelve el número de renglones y columnas en la matriz.
```cpp
std::pair<size_t, size_t> shape() const;
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
La segunda forma es utilizando el operador `[]` con un objeto de tipo 
`std::pair`, lo cual devuelve una referencia al elemento en el renglón y 
columna indicados.
```cpp
T& operator[] (std::pair<size_t, size_t> index);
const T& operator[] (std::pair<size_t, size_t> index) const;
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
    return 0;
}
```

```
[Out] 
```

## Indexado avanzado

## Lista de métodos

## Funciones globales
