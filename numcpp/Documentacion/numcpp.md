# Numcpp

## Arrays

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

### Constructores

1. Constructor por defecto. Construye un arreglo vacío sin elementos.
```cpp
array();
```

2. Constructor por tamaño. Construye un arreglo con *n* elementos.
```cpp
array(size_t n);
```

3. Constructor por relleno. Construye un arreglo con *n* elementos, cada uno 
inicializado a *val*.
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
de *v*, en el mismo orden.
```cpp
array(const array &v);
```

6. Constructor por movimiento. Construye un arreglo que adquiere los elementos 
de *v*.
```cpp
array(array &&v);
```

7. Constructor por lista de inicialización. Construye un arreglo con una copia 
de los elementos de *il*, en el mismo orden.
```cpp
array(std::initializer_list<T> il);
```

### Destructor

Destruye el arreglo.
```cpp
~array();
```

### Rutinas para la creación de arreglos

- **empty**: Crea un nuevo arreglo de tamaño *n* sin inicializar.
```cpp
array<T>::empty(size_t n);
```

- **zeros**: Crea un nuevo arreglo de tamaño *n* inicializando los valores a 
cero.
```cpp
array<T>::zeros(size_t n);
```

- **ones**: Crea un nuevo arreglo de tamaño *n* inicializando los valores a 
unos.
```cpp
array<T>::ones(size_t n);
```

- **full**: Crea un nuevo arreglo de tamaño *n* inicializando los valores a 
*val*
```cpp
array<T>::full(size_t n, const T &val);
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> vacio = np::array<int>::empty(5);
    np::array<int> cero = np::array<int>::zeros(5);
    np::array<int> uno = np::array<int>::ones(5);
    np::array<int> tres = np::array<int>::full(5, 3);
    cout << "vacio: " << vacio << "\n";
    cout << "cero: " << cero << "\n";
    cout << "uno: " << uno << "\n";
    cout << "tres: " << tres << "\n";
    return 0;
}
```

```
[Out] vacio: [ 7412480 0 7405904 0 0 ]
      cero: [ 0 0 0 0 0 ]
      uno: [ 1 1 1 1 1 ]
      tres: [ 3 3 3 3 3 ]
```

### Manejo de archivos

- **load**: Crea un nuevo arreglo desde un archivo binario.
```cpp
array<T>::load(const char *file);
```

- **save**: Guarda el contenido de un arreglo en un archivo binario.
```cpp
void save(const char *file) const;
```

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<int> v(10);
    for (int i = 0; i < 10; ++i) {
        v[i] = i + 1;
    }
    v.save("numeros-1-al-10.bin");
    np::array<int> w = np::array<int>::load("numeros-1-al-10.bin");
    cout << w << "\n";
    return 0;
}
```

```
[Out] [ 1 2 3 4 5 6 7 8 9 10 ]
```

### Operadores de asignación

1. Asignación por copia. Asigna el contenido de *v* a **this* después de 
cambiar el tamaño del arreglo (en caso de ser necesario).
```cpp
array& operator= (const array &v);
```

2. Asignación por movimiento. Adquiere el contenido de *v*.
```cpp
array& operator= (array &&v);
```

3. Asignación por relleno. Asigna *val* a cada elemento.
```cpp
array& operator= (const T &val);
```

### Sobrecarga de operadores

Cada una de estas funciones realiza la operación  respectiva sobre todos los 
elementos del arreglo.

Cuando ambos argumentos izquierdo y derecho son arreglos, la operación se 
realiza entre los elementos correspondientes de cada objeto (el primer elemento 
del arreglo de la izquierda con el primero de la derecha, el segundo con el 
segundo, y así sucesivamente). Si los tamaños de los arreglos no coinciden, 
la función arroja una excepción de tipo `runtime_error`.

Cuando uno de los argumentos es un valor, la operación se realiza entre todos 
los elementos en el arreglo contra ese valor.

#### Como función miembro

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

#### Operadores unitarios

```cpp
template <class T> array<T> operator+ (const array<T> &v);
template <class T> array<T> operator- (const array<T> &v);
template <class T> array<T> operator~ (const array<T> &v);
array<bool> operator! (const array<bool> &v);
```

#### Como función no miembro

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

#### Salida estándar
```cpp
template <class T>
std::ostream& operator<< (std::ostream &ostr, const array<T> &v);
```