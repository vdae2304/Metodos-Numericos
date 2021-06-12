# Arreglos

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
template<class T> 
array<T> empty(size_t n);
```

- **zeros**: Crea un nuevo arreglo de tamaño `n` inicializando los valores a 
cero.
```cpp
template<class T> 
array<T> zeros(size_t n);
```

- **ones**: Crea un nuevo arreglo de tamaño `n` inicializando los valores a 
uno.
```cpp
template<class T> 
array<T> ones(size_t n);
```

- **full**: Crea un nuevo arreglo de tamaño `n` inicializando los valores a 
`val`.
```cpp
template <class T> 
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
[Out] vacio: [ 14621440 0 14614864 0 0 ]
      cero: [ 0 0 0 0 0 ]
      uno: [ 1 1 1 1 1 ]
      tres: [ 3 3 3 3 3 ]
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
[Out] [ 1 2 3 4 5 6 7 8 9 10 ]
```

### Rangos numéricos

- **arange**: Crea un nuevo arreglo con valores equi-espaciados dentro de un 
intervalo. Los valores son generados dentro del intervalo semi-abierto 
`[start, stop)` con saltos de tamaño `step`. Por defecto, el valor de inicio es
0 y el tamaño de paso es 1.
```cpp
template <class T> 
array<T> arange(const T &stop);
template <class T> 
array<T> arange(const T &start, const T &stop, const T &step = T(1));
```

- **linspace**: Crea un nuevo arreglo con números equi-espaciados sobre un 
intervalo especificado. Devuelve `num` muestras equi-espaciadas, calculadas 
sobre el intervalo `[start, stop]`. Opcionalmente, el extremo derecho del 
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
    cout << np::linspace(0.0, 1.0, 5) << "\n";
    cout << np::linspace(0.0, 1.0, 5, false) << "\n";

    cout << "logspace:\n";
    cout << np::logspace(1.0, 5.0, 5, true, 2.0) << "\n";
    cout << np::logspace(0.0, 5.0, 5, false, 10.0) << "\n";

    cout << "geomspace:\n";
    cout << np::geomspace(1.0, 256.0, 9) << "\n";
    cout << np::geomspace(1.0, 100000.0, 5, false) << "\n";
    return 0;
}
```

```
[Out] arange:
      [ 0 1 2 3 4 5 6 7 8 9 ]
      [ 1 4 7 10 13 16 19 ]
      [ 5 4 3 2 1 ]
      linspace:
      [ 0 0.25 0.5 0.75 1 ]
      [ 0 0.2 0.4 0.6 0.8 ]
      logspace:
      [ 2 4 8 16 32 ]
      [ 1 10 100 1000 10000 ]
      geomspace:
      [ 1 2 4 8 16 32 64 128 256 ]
      [ 1 10 100 1000 10000 ]
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

# Funciones matemáticas