# Arreglos

## Contenido

- [Constructores](#Constructores)
- [Destructor](#Destructor)
- [Rutinas para la creación de arreglos](#Rutinas-para-la-creación-de-arreglos)
- [Operadores de asignación](#Operadores-de-asignación)
- [Sobrecarga de operadores](#Sobrecarga-de-operadores)
- [Indexado básico](#Indexado-básico)
- [Indexado avanzado](#Indexado-avanzado)
- [Lista de métodos](#Lista-de-métodos)
- [Funciones globales](#Funciones-globales)

<hr>

Los arreglos son contenedores secuenciales de tamaño fijo: guardan un número 
específico de elementos organizados estrictamente en una secuencia lineal.

Para crear un arreglo, incluya primero la biblioteca `numcpp` al inicio del 
programa. Los arreglos están representados mediante la clase `array`.

> NOTA: No confundir `numcpp::array` con `std::array`. El primero corresponde 
> a un arreglo de la biblioteca `numcpp`, el cual está implementado 
> especialmente para realizar distintas operaciones matemáticas, mientras que
> el segundo es un arreglo convencional de la *Standard Template Library*. 
> Para evitar conflictos en compilación, conserve el nombre de espacio `numcpp`.

#### Ejemplo

Calcula la suma de los productos elemento a elemento de dos arreglos (producto
punto).
```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1, 5, 2, 0, 1};
    np::array<double> w = {7, -1, 2, 8, 3};
    cout << np::sum(v * w) << "\n";
    return 0;
}
```

```
[Out] 9
```

## Constructores

1. Constructor por defecto. Construye un arreglo vacío sin elementos.
```cpp
array();
```

2. Constructor por tamaño. Construye un arreglo con `n` elementos.
```cpp
array(size_t n);
```

3. Constructor por relleno. Construye un arreglo con `n` elementos, cada uno 
inicializado a `val`.
```cpp
array(size_t n, const T &val);
```

4. Constructor por rango. Construye un arreglo con tantos elementos haya en el
rango `[first, last)`, cada elemento construido a partir de su elemento 
correspondiente en el rango, en el mismo orden.
```cpp
template <class InputIterator> 
array(InputIterator first, InputIterator last);
```

5. Constructor por copia. Construye un arreglo con una copia de los elementos 
de `v`, en el mismo orden.
```cpp
array(const array &v);
```

6. Constructor por movimiento. Construye un arreglo que adquiere los elementos 
de `v`.
```cpp
array(array &&v);
```

7. Constructor por lista de inicialización. Construye un arreglo con una copia 
de los elementos de `il`, en el mismo orden.
```cpp
array(std::initializer_list<T> il);
```

## Destructor

Destruye el arreglo.
```cpp
~array();
```

## Rutinas para la creación de arreglos

### Ceros y unos

- **empty**: Crea un nuevo arreglo de tamaño `n` sin inicializar.
```cpp
template<class T = double> 
array<T> empty(size_t n);
```

- **zeros**: Crea un nuevo arreglo de tamaño `n` inicializando los valores a 
cero.
```cpp
template<class T = double> 
array<T> zeros(size_t n);
```

- **ones**: Crea un nuevo arreglo de tamaño `n` inicializando los valores a 
uno.
```cpp
template<class T = double> 
array<T> ones(size_t n);
```

- **full**: Crea un nuevo arreglo de tamaño `n` inicializando los valores a 
`val`.
```cpp
template <class T = double> 
array<T> full(size_t n, const T &val);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> vacio = np::empty<int>(5);
    np::array<int> cero = np::zeros<int>(5);
    np::array<int> uno = np::ones<int>(5);
    np::array<int> tres = np::full<int>(5, 3);
    cout << "vacio: " << vacio << "\n";
    cout << "cero: " << cero << "\n";
    cout << "uno: " << uno << "\n";
    cout << "tres: " << tres << "\n";
    return 0;
}
```

```
[Out] vacio: [7609056, 0, 7602512, 0, 0]
      cero: [0, 0, 0, 0, 0]
      uno: [1, 1, 1, 1, 1]
      tres: [3, 3, 3, 3, 3]
```

### Desde un archivo binario

- **load**: Crea un nuevo arreglo desde un archivo binario. La función arroja 
una excepción del tipo `runtime_error` si el archivo pasado como argumento no
existe.
```cpp
template <class T>
array<T> load(const char *file);
```

- **save**: Guarda el contenido de un arreglo en un archivo binario. La función 
arroja una excepción del tipo `runtime_error` si el archivo pasado como 
argumento no se pudo abrir.
```cpp
template <class T>
void save(const char *file, const array<T> &v);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    np::save("numeros-1-al-10.bin", v);
    np::array<int> w = np::load<int>("numeros-1-al-10.bin");
    cout << w << "\n";
    return 0;
}
```

```
[Out] [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

### Rangos numéricos

- **arange**: Crea un nuevo arreglo con valores equi-espaciados dentro de un 
intervalo. Los valores son generados dentro del intervalo semi-abierto 
`[start, stop)` con saltos de tamaño `step`. Por defecto, el valor de 
inicio es 0 y el tamaño de paso es 1.
```cpp
template <class T> 
array<T> arange(const T &stop);

template <class T> 
array<T> arange(const T &start, const T &stop, const T &step = T(1));
```

- **linspace**: Crea un nuevo arreglo con números equi-espaciados sobre un 
intervalo especificado. Devuelve `num` muestras equi-espaciadas, calculadas 
sobre el intervalo cerrado `[start, stop]`. Opcionalmente, el extremo derecho 
del intervalo puede ser excluido.
```cpp
template <class T> 
array<T> linspace(
    const T &start, const T &stop, 
    size_t num = 50, 
    bool endpoint = true
)
```

- **logspace**: Crea un nuevo arreglo con números equi-espaciados en una escala 
logarítmica. En la escala lineal, la secuencia empieza en `pow(base, start)` 
(`base` elevado a la potencia `start`) y termina en `pow(base, stop)`. 
Opcionalmente, el extremo derecho del intervalo puede ser excluido.
```cpp
template <class T>
array<T> logspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true,
    const T &base = 10.0
)
```

- **geomspace**: Crea un nuevo arreglo con números en una progresión geométrica. 
Cada valor de la muestra es un múltiplo constante del anterior. Opcionalmente, 
el extremo derecho del intervalo puede ser excluido.
```cpp
template <class T>
array<T> geomspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true
)
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    cout << "arange:\n";
    cout << np::arange(10) << "\n";
    cout << np::arange(0., 1., 0.1) << "\n";
    cout << np::arange(5, 0, -1) << "\n";

    cout << "linspace:\n";
    cout << np::linspace(0., 1., 5) << "\n";
    cout << np::linspace(0., 1., 5, false) << "\n";

    cout << "logspace:\n";
    cout << np::logspace(1., 5., 5, true, 2.) << "\n";
    cout << np::logspace(0., 5., 5, false, 10.) << "\n";

    cout << "geomspace:\n";
    cout << np::geomspace(1., 256., 9) << "\n";
    cout << np::geomspace(1., 100000., 5, false) << "\n";
    return 0;
}
```

```
[Out] arange:
      [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
      [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
      [5, 4, 3, 2, 1]
      linspace:
      [0, 0.25, 0.5, 0.75, 1]
      [0, 0.2, 0.4, 0.6, 0.8]
      logspace:
      [2, 4, 8, 16, 32]
      [1, 10, 100, 1000, 10000]
      geomspace:
      [1, 2, 4, 8, 16, 32, 64, 128, 256]
      [1, 10, 100, 1000, 10000]
```

## Operadores de asignación

1. Asignación por copia. Asigna el contenido de `v` a `*this` después de 
cambiar el tamaño del arreglo (en caso de ser necesario).
```cpp
array& operator= (const array &v);
```

2. Asignación por movimiento. Adquiere el contenido de `v`.
```cpp
array& operator= (array &&v);
```

3. Asignación por relleno. Asigna `val` a cada elemento.
```cpp
array& operator= (const T &val);
```

## Sobrecarga de operadores

Cada una de estas funciones realiza la operación  respectiva sobre todos los 
elementos del arreglo.

Cuando ambos argumentos izquierdo y derecho son arreglos, la operación se 
realiza entre los elementos correspondientes de cada objeto (el primer elemento 
del arreglo de la izquierda con el primero de la derecha, el segundo con el 
segundo, y así sucesivamente). Si los tamaños de los arreglos no coinciden, 
la función arroja una excepción del tipo `runtime_error`.

Cuando uno de los argumentos es un valor, la operación se realiza entre todos 
los elementos en el arreglo contra ese valor.

### Como función miembro

```cpp
array& operator+= (const array &v);
array& operator-= (const array &v);
array& operator*= (const array &v);
array& operator/= (const array &v);
array& operator%= (const array &v);
array& operator&= (const array &v);
array& operator|= (const array &v);
array& operator^= (const array &v);
array& operator<<= (const array &v);
array& operator>>= (const array &v);

array& operator+= (const T &val);
array& operator-= (const T &val);
array& operator*= (const T &val);
array& operator/= (const T &val);
array& operator%= (const T &val);
array& operator&= (const T &val);
array& operator|= (const T &val);
array& operator^= (const T &val);
array& operator<<= (const T &val);
array& operator>>= (const T &val);
```

### Operadores unitarios

```cpp
template <class T> array<T> operator+ (const array<T> &v);
template <class T> array<T> operator- (const array<T> &v);
template <class T> array<T> operator~ (const array<T> &v);
array<bool> operator! (const array<bool> &v);
```

### Como función no miembro

```cpp
template<class T> array<T> operator+ (const array<T> &v, const array<T> &w);
template<class T> array<T> operator+ (const array<T> &v, const T &val);
template<class T> array<T> operator+ (const T &val, const array<T> &v);

template<class T> array<T> operator- (const array<T> &v, const array<T> &w);
template<class T> array<T> operator- (const array<T> &v, const T &val);
template<class T> array<T> operator- (const T &val, const array<T> &v);

template<class T> array<T> operator* (const array<T> &v, const array<T> &w);
template<class T> array<T> operator* (const array<T> &v, const T &val);
template<class T> array<T> operator* (const T &val, const array<T> &v);

template<class T> array<T> operator/ (const array<T> &v, const array<T> &w);
template<class T> array<T> operator/ (const array<T> &v, const T &val);
template<class T> array<T> operator/ (const T &val, const array<T> &v);

template<class T> array<T> operator% (const array<T> &v, const array<T> &w);
template<class T> array<T> operator% (const array<T> &v, const T &val);
template<class T> array<T> operator% (const T &val, const array<T> &v);

template<class T> array<T> operator& (const array<T> &v, const array<T> &w);
template<class T> array<T> operator& (const array<T> &v, const T &val);
template<class T> array<T> operator& (const T &val, const array<T> &v);

template<class T> array<T> operator| (const array<T> &v, const array<T> &w);
template<class T> array<T> operator| (const array<T> &v, const T &val);
template<class T> array<T> operator| (const T &val, const array<T> &v);

template<class T> array<T> operator^ (const array<T> &v, const array<T> &w);
template<class T> array<T> operator^ (const array<T> &v, const T &val);
template<class T> array<T> operator^ (const T &val, const array<T> &v);

template<class T> array<T> operator<< (const array<T> &v, const array<T> &w);
template<class T> array<T> operator<< (const array<T> &v, const T &val);
template<class T> array<T> operator<< (const T &val, const array<T> &v);

template<class T> array<T> operator>> (const array<T> &v, const array<T> &w);
template<class T> array<T> operator>> (const array<T> &v, const T &val);
template<class T> array<T> operator>> (const T &val, const array<T> &v);

template<class T> array<bool> operator== (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator== (const array<T> &v, const T &val);
template<class T> array<bool> operator== (const T &val, const array<T> &v);

template<class T> array<bool> operator!= (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator!= (const array<T> &v, const T &val);
template<class T> array<bool> operator!= (const T &val, const array<T> &v);

template<class T> array<bool> operator< (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator< (const array<T> &v, const T &val);
template<class T> array<bool> operator< (const T &val, const array<T> &v);

template<class T> array<bool> operator> (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator> (const array<T> &v, const T &val);
template<class T> array<bool> operator> (const T &val, const array<T> &v);

template<class T> array<bool> operator<= (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator<= (const array<T> &v, const T &val);
template<class T> array<bool> operator<= (const T &val, const array<T> &v);

template<class T> array<bool> operator>= (const array<T> &v, const array<T> &w);
template<class T> array<bool> operator>= (const array<T> &v, const T &val);
template<class T> array<bool> operator>= (const T &val, const array<T> &v);
```

### Entrada/Salida
```cpp
template <class T>
std::ostream& operator<< (std::ostream &ostr, const array<T> &v);
```
## Indexado básico

### Tamaño

- **size**: Devuelve el número de elementos en el arreglo.
```cpp
size_t size() const;
```

### Indexado

- **Operador [ ]**: Accede al `i`-ésimo elemento del arreglo. Note que el 
primer elemento tiene la posición 0, no 1. Valores mayores o iguales que el 
tamaño del arreglo tendrán un comportamiento indefinido.
```cpp
T& operator[] (size_t i);
const T& operator[] (size_t i) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = np::zeros(5);
    cout << "v tiene " << v.size() << " elementos.\n";
    v[0] = 1.;
    v[3] = 2.5;
    v[2] = -0.1;
    cout << "Los elementos de v son: " << v << "\n";
    return 0;
}
```

```
[Out] v tiene 5 elementos.
      Los elementos de v son: [1, 0, -0.1, 2.5, 0]
```

## Indexado avanzado

### Sub-arreglos

Un sub-arreglo es una clase intermedia devuelta por el operador `[]` de un 
arreglo. Hace referencia a los elementos en el arreglo que son seleccionados 
por el sub-índice, y sobrecarga los operadores de asignación y asignación 
compuesta, permitiendo acceder directamente a los elementos en la selección. 
El objeto puede ser convertido en un arreglo, generando un nuevo objeto con 
copias de los elementos referenciados.
```cpp
template <class T>
class subarray<T> {
public:
    void operator= (const array<T> &v);
    void operator= (const T &val);

    void operator+= (const array<T> &v);
    void operator-= (const array<T> &v);
    void operator*= (const array<T> &v);
    void operator/= (const array<T> &v);
    void operator%= (const array<T> &v);
    void operator&= (const array<T> &v);
    void operator|= (const array<T> &v);
    void operator^= (const array<T> &v);
    void operator<<= (const array<T> &v);
    void operator>>= (const array<T> &v);

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

    T& operator[] (size_t i);
    const T& operator[] (size_t i) const;

    array<T> copy() const;
    size_t size() const;
};
```

### Slices

Un `slice` describe una selección de elementos a utilizar como índices por 
el operador `[]`. Un slice está definido por un índice de inicio (`start`), 
un índice de fin (`stop`) y un tamaño de paso (`step`). Por ejemplo, 
`slice(3, 20, 5)` selecciona los elementos en las posiciones 3, 8, 13 y 18.
Por defecto, el valor de inicio es 0 y el tamaño de paso es 1.
```cpp
slice(size_t stop);
slice(size_t start, size_t stop, size_t step = 1);
```
Al indexar un arreglo mediante un slice, el operador devuelve un objeto de 
tipo  `subarray<T>` representando un subarreglo con los elementos seleccionados 
por el slice.
```cpp
subarray<T> operator[] (slice slc);
const subarray<T> operator[] (slice slc) const;
```

#### Ejemplo
```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {8, -3, 7, 5, 10, -1, 1, 3, -5, 14};
    np::subarray<int> sub = v[np::slice(3, 10, 2)];
    cout << "Los elementos de v son: " << v << "\n";
    cout << "El subarreglo tiene " << sub.size() << " elementos.\n";
    cout << "Los elementos del subarreglo son: " << sub.copy() << "\n";
    sub = 0;
    cout << "Los elementos de v son ahora: " << v << "\n";
    return 0;
}
```

```
[Out] Los elementos de v son: [8, -3, 7, 5, 10, -1, 1, 3, -5, 14]
      El subarreglo tiene 4 elementos.
      Los elementos del subarreglo son: [5, -1, 3, 14]
      Los elementos de v son ahora: [8, -3, 7, 0, 10, 0, 1, 0, -5, 0]
```

### Arreglos de índices

Al indexar un arreglo mediante un arreglo de índices (de tipo `size_t`), el 
operador devuelve un objeto de tipo  `subarray<T>` representando un subarreglo 
con los elementos especificados por el arreglo de índices.
```cpp
subarray<T> operator[] (const array<size_t> &indices);
const subarray<T> operator[] (const array<size_t> &indices) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {8, -3, 7, 5, 10, -1, 1, 3, -5, 14};
    np::array<size_t> indices = {1, 5, 6, 8, 2};
    np::subarray<int> sub = v[indices];
    cout << "Los elementos de v son: " << v << "\n";
    cout << "El subarreglo tiene " << sub.size() << " elementos.\n";
    cout << "Los elementos del subarreglo son: " << sub.copy() << "\n";
    sub = 0;
    cout << "Los elementos de v son ahora: " << v << "\n";
    return 0;
}
```

```
[Out] Los elementos de v son: [8, -3, 7, 5, 10, -1, 1, 3, -5, 14]
      El subarreglo tiene 5 elementos.
      Los elementos del subarreglo son: [-3, -1, 1, -5, 7]
      Los elementos de v son ahora: [8, 0, 0, 5, 10, 0, 0, 3, 0, 14]
```

### Arreglos booleanos

Al indexar un arreglo mediante un arreglo booleano, el operador devuelve un 
objeto de tipo  `subarray<T>` representando un subarreglo con los elementos 
seleccionados por la máscara booleana.
```cpp
subarray<T> operator[] (const array<bool> &mask);
const subarray<T> operator[] (const array<bool> &mask) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {8, -3, 7, 5, 10, -1, 1, 3, -5, 14};
    np::array<bool> mascara = {1, 1, 0, 0, 1, 0, 1, 0, 1, 0};
    np::subarray<int> sub = v[mascara];
    cout << "Los elementos de v son: " << v << "\n";
    cout << "El subarreglo tiene " << sub.size() << " elementos.\n";
    cout << "Los elementos del subarreglo son: " << sub.copy() << "\n";
    sub = 0;
    cout << "Los elementos de v son ahora: " << v << "\n";
    return 0;
}
```

```
[Out] Los elementos de v son: [8, -3, 7, 5, 10, -1, 1, 3, -5, 14]
      El subarreglo tiene 5 elementos.
      Los elementos del subarreglo son: [8, -3, 10, 1, -5]
      Los elementos de v son ahora: [0, 0, 7, 5, 0, -1, 0, 3, 0, 14]
```

## Lista de métodos

### `apply` 

Sustituye cada elemento del arreglo con el resultado de evaluar una función en 
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
    np::array<double> v = {1., -9., 5., 10., -2., 7.};
    v.apply(square);
    cout << v << "\n";
    return 0;
}
```

```
[Out] [1, 81, 25, 100, 4, 49]
```

### `argmax` 

Devuelve el índice del mayor valor contenido en el arreglo.
```cpp
size_t argmax() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
    cout << v.argmax() << "\n";
    return 0;
}
```

```
[Out] 0
```

### `argmin` 

Devuelve el índice del menor valor contenido en el arreglo.
```cpp
size_t argmin() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
    cout << v.argmin() << "\n";
    return 0;
}
```

```
[Out] 3
```

### `argsort` 

Devuelve el arreglo de índices que ordenan el arreglo.
```cpp
array<size_t> argsort() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -4.1, 2.5, 0., 3.};
    cout << v.argsort() << "\n";
    return 0;
}
```

```
[Out] [1, 3, 0, 2, 4]
```

### `astype` 

Crea una copia del arreglo, haciendo la conversión al tipo de dato 
especificado.
```cpp
template <class U> array<U> astype() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., 2.5, 3., -4.1, 0.};
    np::array<int> w = v.astype<int>();
    cout << w << "\n";
    return 0;
}
```

```
[Out] [1, 2, 3, -4, 0]
```

### `clip` 

Restringe los valores de un arreglo. Dado un intervalo, valores  fuera del 
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
    np::array<double> v = {1., -9., 3., 10., -2., 7.};
    v.clip(0., 5.);
    cout << v << "\n";
    return 0;
}
```

```
[Out] [1, 0, 3, 5, 0, 5]
```

### `cumprod`

Crea un arreglo con los productos acumulados de los elementos de un arreglo.
```cpp
array cumprod() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.cumprod() << "\n";
    return 0;
}
```

```
[Out] [2, 6, 30, 30, 120]
```

### `cumsum` 
Crea un arreglo con las sumas acumuladas de los elementos de un  arreglo.
```cpp
array cumsum() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.cumsum() << "\n";
    return 0;
}
```

```
[Out] [2, 5, 10, 11, 15]
```

### `data`

Devuelve un puntero a la memoria del arreglo utilizado internamente.
```cpp
T* data();
const T* data() const;
```

### `dot` 

Devuelve el producto punto entre dos arreglos. El producto punto está definido
como `sum(v * w)`.
```cpp
T dot(const array &v) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1, 5, 2, 0, 1};
    np::array<double> w = {7, -1, 2, 8, 3};
    cout << v.dot(w) << "\n";
    return 0;
}
```

```
[Out] 9
```

### `max` 

Devuelve el mayor valor contenido en el arreglo.
```cpp
T max() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
    cout << v.max() << "\n";
    return 0;
}
```

```
[Out] 5
```

### `mean` 

Devuelve la media o promedio de los elementos de un arreglo. La media está 
definida como `x.sum() / x.size()`.
```cpp
T mean() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -3., 2., 0., 5., 2.};
    cout << v.mean() << "\n";
    return 0;
}
```

```
[Out] 1.16667
```

### `min` 

Devuelve el menor valor contenido en el arreglo.
```cpp
T min() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {5., 2.5, 3., -4.1, 0., 1.};
   cout << v.min() << "\n";
    return 0;
}
```

```
[Out] -4.1
```

### `prod` 

Devuelve el producto de los elementos de un arreglo.
```cpp
T prod() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.prod() << "\n";
    return 0;
}
```

```
[Out] 120
```

### `resize`

Cambia el tamaño del arreglo a `n` elementos. Si `n` es menor que el tamaño 
actual, el contenido es reducido a los primeros `n` elementos, eliminando 
aquellos más allá de esa posición. Si `n` es mayor que el tamaño actual, el 
contenido es expandido agregando al final tantos elementos sean necesarios para 
alcanzar un tamaño de `n`.
```cpp
void resize(size_t n, const T &val = T());
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -9., 5., 10., -2., 7.};
    cout << "Antes: " << v << "\n";
    v.resize(10, 0.);
    cout << "Tras expandir: " << v << "\n";
    v.resize(3, 0.);
    cout << "Tras recortar: " << v << "\n";
    return 0;
}
```

```
[Out] Antes: [1, -9, 5, 10, -2, 7]
      Tras expandir: [1, -9, 5, 10, -2, 7, 0, 0, 0, 0]
      Tras recortar: [1, -9, 5]
```

### `size`

Devuelve el número de elementos en el arreglo.
```cpp
size_t size() const;
```

### `sort` 

Ordena los elementos de un arreglo.
```cpp
void sort();
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -4.1, 2.5, 0., 3.};
    v.sort();
    cout << v << "\n";
    return 0;
}
```

```
[Out] [-4.1, 0, 1, 2.5, 3]
```

### `stddev` 

Devuelve la desviación estándar de los elementos de un arreglo. La desviación 
estándar está definida como `sqrt(square(x - x.mean()) / (x.size() - ddof))` 
(la raíz cuadrada de la  varianza) donde `ddof` son los grados de libertad. 
Por defecto, `ddof = 0`.
```cpp
T stddev(size_t ddof = 0) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -3., 2., 0., 5., 2.};
    cout << v.stddev() << "\n";
    return 0;
}
```

```
[Out] 2.40947
```

### `sum` 

Devuelve la suma de los elementos de un arreglo.
```cpp
T sum() const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {2, 3, 5, 1, 4};
    cout << v.sum() << "\n";
    return 0;
}
```

```
[Out] 15
```

### `swap` 

Intercambia el contenido del arreglo con otro arreglo `v`. Esta función está 
implementada para funcionar en tiempo constante.
```cpp
void swap(array &v);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -9., 5., 10., -2.};
    np::array<double> w = {1, 2, 3};
    cout << "Antes: " << v << " y " << w << "\n";
    v.swap(w);
    cout << "Despues: " << v << " y " << w << "\n";
    return 0;
}
```

```
[Out] Antes: [1, -9, 5, 10, -2] y [1, 2, 3]
      Despues: [1, 2, 3] y [1, -9, 5, 10, -2]
```


### `var` 

Devuelve la varianza de los elementos de un arreglo. La varianza está definida 
como `square(x - x.mean()) / (x.size() - ddof)` donde `ddof` son los grados de 
libertad. Por defecto, `ddof = 0`.
```cpp
T var(size_t ddof = 0) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {1., -3., 2., 0., 5., 2.};
    cout << v.var() << "\n";
    return 0;
}
```

```
[Out] 5.80556
```

## Funciones globales

### `all`

Devuelve `true` si todos los elementos de un arreglo evaluan a `true`.
```cpp
bool all(const array<bool> &v);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, 3, 2, 5, 7};
    cout << boolalpha << "Son todos positivos? " << np::all(v > 0) << "\n";
    cout << "Son todos pares? " << np::all(v % 2 == 0) << "\n";
    return 0;
}
```

```
[Out] Son todos positivos? true
      Son todos pares? false
```

### `any`

Devuelve `true` si algún elemento de un arreglo evalua a `true`.
```cpp
bool any(const array<bool> &v);
```

### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, 3, 2, 5, 7};
    cout << boolalpha << "Hay algun negativo? " << np::any(v < 0) << "\n";
    cout << "Hay algun par? " << np::any(v % 2 == 0) << "\n";
    return 0;
}
```

```
[Out] Hay algun negativo? false
      Hay algun par? true
```

### `apply`

En su primera versión, crea un arreglo donde cada uno de sus elementos está 
inicializado con el resultado de evaluar una función `f` en los elementos 
correspondientes de `v`. A diferencia del método `v.apply()`, esta función no
modifica los elementos de `v`.

En su segunda versión, crea un arreglo donde cada uno de sus elementos está 
inicializado con el resultado de evaluar una función `f` en los elementos 
correspondientes de `v` y `w`; en los elementos de `v` contra `val`; en `val`
contra los elementos de `v` según sea el caso.
```cpp
template <class T, class Function = T(T)>
array<T> apply(Function f, const array<T> &v);

template <class T, class Function = T(T, T)>
array<T> apply(Function f, const array<T> &v, const array<T> &w);

template <class T, class Function = T(T, T)>
array<T> apply(Function f, const array<T> &v, const T &val);

template <class T, class Function = T(T, T)>
array<T> apply(Function f, const T &val, const array<T> &v);
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
    np::array<double> x = {1, 3, 2, 5, 7};
    np::array<double> y = {-1, 0, 2, 5, 3};
    cout << np::apply(square, x) << "\n";
    cout << np::apply(f, x, y) << "\n";
    return 0;
}
```

```
[Out] [1, 9, 4, 25, 49]
      [-1, 3, 6, 15, 13]
```

### `arange`

Crea un nuevo arreglo con valores equi-espaciados dentro de un intervalo. Los 
valores son generados dentro del intervalo semi-abierto `[start, stop)` con 
saltos de tamaño `step`. Por defecto, el valor de inicio es 0 y el tamaño de 
paso es 1.
```cpp
template <class T> 
array<T> arange(const T &stop);

template <class T> 
array<T> arange(const T &start, const T &stop, const T &step = T(1));
```

### `argmax` 

Devuelve el índice del mayor valor contenido en el arreglo.
```cpp
template <class T>
size_t argmax(const array<T> &v);
```

### `argmin` 

Devuelve el índice del menor valor contenido en el arreglo.
```cpp
template <class T>
size_t argmin(const array<T> &v);
```

### `argsort` 

Devuelve el arreglo de índices que ordenan el arreglo.
```cpp
template <class T>
size_t argsort(const array<T> &v);
```

### `clip` 

Crea un arreglo donde cada uno de sus elementos está inicializado con el 
resultado de restringir los valores de un arreglo. A diferencia del método 
`v.clip()`, esta función no modifica los elementos de `v`.
```cpp
template <class T>
array<T> clip(const array<T> &v, const T &a_min, const T &a_max);
```

### `concatenate`

Concatena (une) dos arreeglos.
```cpp
template <class T>
array<T> concatenate(const array<T> &v, const array<T> &w);
```

#### Ejemplo 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, -9, 5, 10};
    np::array<int> w = {0, 7, -1};
    cout << np::concatenate(v, w) << "\n";
    return 0;
}
```

```
[Out] [1, -9, 5, 10, 0, 7, -1]
```

### `cumprod`

Crea un arreglo con los productos acumulados de los elementos de un arreglo.
```cpp
template <class T>
array<T> cumprod(const array<T> &v);
```

### `cumsum` 
Crea un arreglo con las sumas acumuladas de los elementos de un  arreglo.
```cpp
template <class T>
array<T> cumprod(const array<T> &v);
```

### `dot` 

Devuelve el producto punto entre dos arreglos. 
```cpp
template <class T>
T dot(const array<T> &v, const array<T> &w);
```

### `empty`
Crea un nuevo arreglo de tamaño `n` sin inicializar.
```cpp
template<class T = double> 
array<T> empty(size_t n);
```

### `erase`

Elimina valores de un arreglo en base a su posición o índice.
```cpp
template <class T>
array<T> erase(const array<T> &v, size_t index);

template <class T>
array<T> erase(const array<T> &v, const array<size_t> indices);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {3., -1., 7., 5., 2., 10., -3., 2., 1., 8.};
    cout << v << "\n";
    cout << np::erase(v, 2) << "\n";
    cout << np::erase(v, {0, 5, 2, 9}) << "\n";
    return 0;
}
```

```
[Out] [3, -1, 7, 5, 2, 10, -3, 2, 1, 8]
      [3, -1, 5, 2, 10, -3, 2, 1, 8]
      [-1, 5, 2, -3, 2, 1]
```

### `full` 
Crea un nuevo arreglo de tamaño `n` inicializando los valores a `val`.
```cpp
template <class T = double> 
array<T> full(size_t n, const T &val);
```

### `geomspace` 

Crea un nuevo arreglo con números en una progresión geométrica. 
Cada valor de la muestra es un múltiplo constante del anterior. Opcionalmente, 
el extremo derecho del intervalo puede ser excluido.
```cpp
template <class T>
array<T> geomspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true
)
```

### `insert`

Inserta valores en un arreglo antes de las posiciones o índices indicados.
```cpp
template <class T>
array<T> insert(const array<T> &v, size_t index, const T &value);

template <class T>
array<T> insert(
    const array<T> &v, const array<size_t> &indices, const array<T> &values
);
```

#### Ejemplo 

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> v = {3., -1., 7., 5., 2., 10.};
    cout << v << "\n";
    cout << np::insert(v, 1, 8.1) << "\n";
    cout << np::insert(v, {0, 5, 2, 6}, {0.1, 1.2, -0.5, 3.5}) << "\n";
    return 0;
}
```

```
[Out] [3, -1, 7, 5, 2, 10]
      [3, 8.1, -1, 7, 5, 2, 10]
      [0.1, 3, -1, -0.5, 7, 5, 2, 1.2, 10, 3.5]
```

### `linspace`

Crea un nuevo arreglo con números equi-espaciados sobre un intervalo 
especificado. Devuelve `num` muestras equi-espaciadas, calculadas sobre el 
intervalo cerrado `[start, stop]`. Opcionalmente, el extremo derecho del 
intervalo puede ser excluido.
```cpp
template <class T> 
array<T> linspace(
    const T &start, const T &stop, 
    size_t num = 50, 
    bool endpoint = true
)
```

### `load` 

Crea un nuevo arreglo desde un archivo binario. La función arroja una 
excepción del tipo `runtime_error` si el archivo pasado como argumento no
existe.
```cpp
template <class T>
array<T> load(const char *file);
```

### `logspace` 

Crea un nuevo arreglo con números equi-espaciados en una escala logarítmica. 
En la escala lineal, la secuencia empieza en `pow(base, start)` (`base` 
elevado a la potencia `start`) y termina en `pow(base, stop)`. 
Opcionalmente, el extremo derecho del intervalo puede ser excluido. Por 
defecto, la base es igual a 10.
```cpp
template <class T>
array<T> logspace(
    const T &start, const T &stop,
    size_t num = 50,
    bool endpoint = true,
    const T &base = 10.0
)
```

### `max` 

Devuelve el mayor valor contenido en el arreglo.
```cpp
template <class T>
T max(const array<T> &v);
```

### `mean` 

Devuelve la media o promedio de los elementos de un arreglo.
```cpp
template <class T>
T mean(const array<T> &v);
```

### `min` 

Devuelve el menor valor contenido en el arreglo.
```cpp
template <class T>
T min(const array<T> &v);
```

### `ones` 
Crea un nuevo arreglo de tamaño `n` inicializando los valores a uno.
```cpp
template<class T = double> 
array<T> ones(size_t n);
```

### `prod` 

Devuelve el producto de los elementos de un arreglo.
```cpp
template <class T>
T prod(const array<T> &v); 
```

### `save`

Guarda el contenido de un arreglo en un archivo binario. La función arroja una 
excepción del tipo `runtime_error` si el archivo pasado como argumento no se 
pudo abrir.
```cpp
template <class T>
void save(const char *file, const array<T> &v);
```

### `sort` 

Crea una copia con los elementos ordenados de un arreglo. A diferencia del 
método `v.sort()`, esta función no modifica los elementos de `v`.
```cpp
template <class T>
array<T> sort(const array<T> &v);
```

### `stddev` 

Devuelve la desviación estándar de los elementos de un arreglo con `ddof` 
grados de libertad. Por defecto, `ddof = 0`.
```cpp
template <class T>
T stddev(const array<T> &v, size_t ddof = 0);
```

### `sum` 

Devuelve la suma de los elementos de un arreglo.
```cpp
template <class T>
T sum(const array<T> &v);
```

### `swap` 

Intercambia el contenido entre dos arreglos. Esta función está implementada 
para funcionar en tiempo constante.
```cpp
template <class T>
void swap(array<T> &v, array<T> &w);
```
### `var` 

Devuelve la varianza de los elementos de un arreglo con `ddof` grados de 
libertad. Por defecto, `ddof = 0`.
```cpp
template <class T>
T var(const array<T> &v, size_t ddof = 0);
```

### `where`

En su primera versión, devuelve el arreglo de índices de las posiciones en 
donde `condition` evalua a `true`.

En su segunda versión, devuelve un arreglo escogiendo los elementos de 
`expr_true` que se encuentran en las posiciones en donde `condition` evalua a
`true`.

En su tercera versión, devuelve un arreglo escogiendo entre los elementos de 
`expr_true` que se encuentran en las posiciones en donde `condition` evalua a
`true` y los elementos de `expr_false` que se encuentran en las posiciones en 
donde `condition` evalua a `false`.
```cpp
array<size_t> where(const array<bool> &condition);

template <class T>
array<T> where(const array<bool> &condition, const array<T> &expr_true);

template <class T>
array<T> where(
    const array<bool> &condition,
    const array<T> &expr_true, const array<T> &expr_false
);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {1, -9, 5, 10, -2, 4, -1, 7, 3, 0};
    cout << np::where(v > 0) << "\n";
    cout << np::where(v > 0, v) << "\n";
    cout << np::where(v > 0, v, -v) << "\n";
    return 0;
}
```

```
[Out] [0, 2, 3, 5, 7, 8]
      [1, 5, 10, 4, 7, 3]
      [1, 9, 5, 10, 2, 4, 1, 7, 3, 0]
```

### `zeros` 
Crea un nuevo arreglo de tamaño `n` inicializando los valores a cero.
```cpp
template<class T = double> 
array<T> zeros(size_t n);
```