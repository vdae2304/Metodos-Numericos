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
sobre el intervalo cerrado `[start, stop]`. Opcionalmente, el extremo derecho del 
intervalo puede ser excluido.
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
    cout << np::arange(1, 20, 3) << "\n";
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
      [1, 4, 7, 10, 13, 16, 19]
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

### Salida estándar
```cpp
template <class T>
std::ostream& operator<< (std::ostream &ostr, const array<T> &v);
```
## Indexado básico

### Métodos básicos

- **size**: Devuelve el número de elementos en el arreglo.
```cpp
size_t size() const;
```

- **data**: Devuelve un puntero a la memoria del arreglo utilizado 
internamente.
```cpp
T* data();
const T* data() const;
```

- **resize**: Cambia el tamaño del arreglo a `n` elementos. Si `n` es menor
que el tamaño actual, el contenido es reducido a los primeros `n` elementos,
eliminando aquellos más allá de esa posición. Si `n` es mayor que el tamaño 
actual, el contenido es expandido agregando al final tantos elementos sean
necesarios para alcanzar un tamaño de `n`.
```cpp
void resize(size_t n, const T &val = T());
```

### Indexado

Accede al `i`-ésimo elemento del arreglo. Note que el primer elemento tiene 
la posición 0, no 1. Valores mayores o iguales que el tamaño del arreglo 
tendrán un comportamiento indefinido.
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
    v.resize(10);
    cout << "v tiene " << v.size() << " elementos.\n";
    cout << "Los elementos de v son:\n" << v << "\n";
    return 0;
}
```

```
[Out] v tiene 5 elementos.
      v tiene 10 elementos.
      Los elementos de v son:
      [1, 0, -0.1, 2.5, 0, 0, 0, 0, 0, 0]
```

## Indexado avanzado

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
tipo  `subarray<T, slice>` representando un subarreglo con los elementos 
seleccionados por el slice.
```cpp
template <class T>
class subarray<T, slice> {
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

    array<T> copy() const;
    size_t size() const;
};
```

#### Ejemplo
```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v = {8, -3, 7, 5, 10, -1, 1, 3, -5, 14};
    np::subarray<int, np::slice> sub = v[np::slice(3, 10, 2)];
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
operador devuelve un objeto de tipo  `subarray<T, size_t>` representando un 
subarreglo con los elementos especificados por el arreglo de índices.
```cpp
template <class T>
class subarray<T, size_t> {
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

    array<T> copy() const;
    size_t size() const;
};
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
    np::subarray<int, size_t> sub = v[indices];
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

Al indexar un arreglo mediante un arreglo booleano, el operador 
devuelve un objeto de tipo  `subarray<T, bool>` representando un 
subarreglo con los elementos seleccionados por la máscara booleana.
```cpp
template <class T>
class subarray<T, bool> {
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

    array<T> copy() const;
    size_t size() const;
};
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
    np::subarray<int, bool> sub = v[mascara];
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

### Memoria y tamaño

- **data**: Devuelve un puntero a la memoria del arreglo utilizado 
internamente.
```cpp
T* data();
const T* data() const;
```

- **resize**: Cambia el tamaño del arreglo a `n` elementos. Si `n` es menor
que el tamaño actual, el contenido es reducido a los primeros `n` elementos,
eliminando aquellos más allá de esa posición. Si `n` es mayor que el tamaño 
actual, el contenido es expandido agregando al final tantos elementos sean
necesarios para alcanzar un tamaño de `n`.
```cpp
void resize(size_t n, const T &val = T());
```

- **size**: Devuelve el número de elementos en el arreglo.
```cpp
size_t size() const;
```

### Conversión de tipo

- **astype**: Crea una copia del arreglo, haciendo la conversión al tipo de 
dato especificado.
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
    cout << w;
    return 0;
}
```

```
[Out] [1, 2, 3, -4, 0]
```

### Ordenamiento

- **argsort**: Devuelve el arreglo de índices que ordenan el arreglo.
```cpp
array<size_t> argsort() const;
```

- **sort**: Ordena los elementos de un arreglo.
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
    np::array<double> v = {1., 2.5, 3., -4.1, 0.};
    cout << v.argsort() << "\n";
    v.sort();
    cout << v << "\n";
    return 0;
}
```

```
[Out] [3, 4, 0, 1, 2]
      [-4.1, 0, 1, 2.5, 3]
```

### Máximos y mínimos

- **argmax**: Devuelve el índice del mayor valor contenido en el arreglo.
```cpp
size_t argmax() const;
```

- **argmin**: Devuelve el índice del menor valor contenido en el arreglo.
```cpp
size_t argmin() const;
```

- **max**: Devuelve el mayor valor contenido en el arreglo.
```cpp
T max() const;
```

- **min**: Devuelve el menor valor contenido en el arreglo.
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
    cout << "v[" << v.argmax() << "] = " << v.max() << "\n";
    cout << "v[" << v.argmin() << "] = " << v.min() << "\n";
    return 0;
}
```

```
[Out] v[0] = 5
      v[3] = -4.1
```

### Sumas y productos

- **cumprod**: Crea un arreglo con los productos acumulados de los elementos de
un arreglo.
```cpp
array cumprod() const;
```

- **cumsum**: Crea un arreglo con las sumas acumuladas de los elementos de un 
arreglo.
```cpp
array cumsum() const;
```

- **prod**: Devuelve el producto de los elementos de un arreglo.
```cpp
T prod() const;
```

- **sum**: Devuelve la suma de los elementos de un arreglo.
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
    cout << "Suma: " << v.sum() << "\n";
    cout << "Sumas acumuladas: " << v.cumsum() << "\n";
    cout << "Producto: " << v.prod() << "\n";
    cout << "Productos acumulados: " << v.cumprod() << "\n";
    return 0;
}
```

```
[Out] Suma: 15
      Sumas acumuladas: [2, 5, 10, 11, 15]
      Producto: 120
      Productos acumulados: [2, 6, 30, 30, 120]
```

### Álgebra lineal

- **dot**: Devuelve el producto punto entre dos arreglos. El producto punto se define como 
![$v^T w = \sum_{i = 1}^{n} v_i w_i$](https://render.githubusercontent.com/render/math?math=v%5ET%20w%20%3D%20%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20v_i%20w_i)
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

### Estadística

- **mean**: Devuelve la media de los elementos de un arreglo. La media se define como 
![$\bar{x} = \frac{1}{n}\sum_{i = 1}^{n} x_i$](https://render.githubusercontent.com/render/math?math=%5Cbar%7Bx%7D%20%3D%20%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20x_i)
```cpp
T mean() const;
```

- **stddev**: Devuelve la desviación estándar de los elementos de un arreglo. 
La desviación estándar se define como la raíz cuadrada de la varianza.
```cpp
T stddev(size_t ddof = 0) const;
```

- **var**: Devuelve la varianza de los elementos de un arreglo. La varianza se 
define como 
![$S^2 = \frac{1}{n - ddof}\sum_{i = 1}^{n} (x_i - \bar{x})^2$](https://render.githubusercontent.com/render/math?math=S%5E2%20%3D%20%5Cfrac%7B1%7D%7Bn%20-%20ddof%7D%5Csum_%7Bi%20%3D%201%7D%5E%7Bn%7D%20(x_i%20-%20%5Cbar%7Bx%7D)%5E2) 
donde `ddof` indica los grados de libertad. Por defecto, `ddof = 0`.
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
    cout << "Media: " << v.mean() << "\n";
    cout << "Varianza: " << v.var() << "\n";
    cout << "Desviacion estandar: " << v.stddev() << "\n";
    return 0;
}
```

```
[Out] Media: 1.16667
      Varianza: 5.80556
      Desviacion estandar: 2.40947
```