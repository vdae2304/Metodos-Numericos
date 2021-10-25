# Funciones matemáticas

## Contenido

- [Constantes](#Constantes)
- [Funciones trigonométricas](#Funciones-trigonométricas)
- [Funciones hiperbólicas](#Funciones-hiperbólicas)
- [Exponenciales y logaritmos](#Exponenciales-y-logaritmos)
- [Funciones potencia](#Funciones-potencia)
- [Redondeo](#Redondeo)
- [Misceláneo](#Misceláneo)

<hr>

La biblioteca `numcpp` define un conjunto de funciones matemáticas, 
sobrecargándolas para ser usadas en arreglos y matrices.

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> x = np::linspace(0.0, np::pi, 10);
    np::array<double> y = x * np::cos(np::sqrt(2.0)*x + 1.0);
    cout << y << "\n";
    return 0;
}
```

```
[Out] [0, 0.026901171, -0.28244468, -0.8268722, -1.3768435, -1.6530264, -1.4283364, -0.62063839, 0.655217, 2.0961889]
```

## Constantes

### `inf`, `infty`, `infinity`

Representación de punto flotante de infinito (positivo).

### `e`

*e = 2.71828182846...*.

### `pi`

*pi = 3.14159265359...*

### `golden`

*golden = 1.61803398875...*

## Funciones trigonométricas

### `cos`

Devuelve el coseno de un ángulo *x* en radianes.
```cpp
double cos(double x);
float cos(float x);
long double cos(long double x);

template <class T>
double cos(T x); 

template <class T>
array<T> cos(const array<T> &x);

template <class T>
matrix<T> cos(const matrix<T> &x);
```

### `sin`

Devuelve el seno de un ángulo *x* en radianes.
```cpp
double sin(double x);
float sin(float x);
long double sin(long double x);

template <class T>
double sin(T x); 

template <class T>
array<T> sin(const array<T> &x);

template <class T>
matrix<T> sin(const matrix<T> &x);
```

### `tan`

Devuelve la tangente de un ángulo *x* en radianes.
```cpp
double tan(double x);
float tan(float x);
long double tan(long double x);

template <class T>
double tan(T x); 

template <class T>
array<T> tan(const array<T> &x);

template <class T>
matrix<T> tan(const matrix<T> &x);
```

### `hypot`

Devuelve la hipotenusa de un triángulo rectángulo cuyos catetos son *x* y *y*.
```cpp
double hypot(double x, double y);
float hypot(float x, float y);
long double hypot(long double x, long double y);

template <class T1, class T2>
double hypot(T1 x, T2 y);

template <class T>
array<T> hypot(const array<T> &x, const array<T> &y);

template <class T>
array<T> hypot(const array<T> &x, const T &y);

template <class T>
array<T> hypot(const T &x, const array<T> &y);

template <class T>
matrix<T> hypot(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> hypot(const matrix<T> &x, const T &y);

template <class T>
matrix<T> hypot(const T &x, const matrix<T> &y);
```

### `acos`

Devuelve el valor principal del arco coseno de *x*, expresado en radianes.
```cpp
double acos(double x);
float acos(float x);
long double acos(long double x);

template <class T>
double acos(T x); 

template <class T>
array<T> acos(const array<T> &x);

template <class T>
matrix<T> acos(const matrix<T> &x);
```

### `asin`

Devuelve el valor principal del arco seno de *x*, expresado en radianes.
```cpp
double asin(double x);
float asin(float x);
long double asin(long double x);

template <class T>
double asin(T x); 

template <class T>
array<T> asin(const array<T> &x);

template <class T>
matrix<T> asin(const matrix<T> &x);
```

### `atan`

Devuelve el valor principal del arco tangente de *x*, expresado en radianes.
```cpp
double atan(double x);
float atan(float x);
long double atan(long double x);

template <class T>
double atan(T x); 

template <class T>
array<T> atan(const array<T> &x);

template <class T>
matrix<T> atan(const matrix<T> &x);
```

### `atan2`

Devuelve el valor principal del arco tangente de *y/x*, expresado en radianes.
Para calcular el valor, la función toma en cuenta el signo de *x* y *y* para
determinar el cuadrante.
```cpp
double atan2(double x, double y);
float atan2(float x, float y);
long double atan2(long double x, long double y);

template <class T1, class T2>
double atan2(T1 x, T2 y);

template <class T>
array<T> atan2(const array<T> &y, const array<T> &x);

template <class T>
array<T> atan2(const array<T> &y, const T &x);

template <class T>
array<T> atan2(const T &y, const array<T> &x);

template <class T>
matrix<T> atan2(const matrix<T> &y, const matrix<T> &x); 

template <class T>
matrix<T> atan2(const matrix<T> &y, const T &x);

template <class T>
matrix<T> atan2(const T &y, const matrix<T> &x);
```

### `degrees`

Convierte un ángulo de radianes a grados.
```cpp
double degrees(double x);
float degrees(float x);
long double degrees(long double x);

template <class T>
double degrees(T x);

template <class T>
array<T> degrees(const array<T> &x);

template <class T>
matrix<T> degrees(const matrix<T> &x);
```

### `radians`

Convierte un ángulo de grados a radianes.
```cpp
double radians(double x);
float radians(float x);
long double radians(long double x);

template <class T>
double radians(T x);

template <class T>
array<T> radians(const array<T> &x);

template <class T>
matrix<T> radians(const matrix<T> &x);
```

## Funciones hiperbólicas

### `cosh`

Devuelve el coseno hiperbólico de *x*.
```cpp
double cosh(double x);
float cosh(float x);
long double cosh(long double x);

template <class T>
double cosh(T x); 

template <class T>
array<T> cosh(const array<T> &x);

template <class T>
matrix<T> cosh(const matrix<T> &x);
```

### `sinh`

Devuelve el seno hiperbólico de *x*.
```cpp
double sinh(double x);
float sinh(float x);
long double sinh(long double x);

template <class T>
double sinh(T x); 

template <class T>
array<T> sinh(const array<T> &x);

template <class T>
matrix<T> sinh(const matrix<T> &x);
```

### `tanh`

Devuelve la tangente hiperbólica de *x*.
```cpp
double tanh(double x);
float tanh(float x);
long double tanh(long double x);

template <class T>
double tanh(T x); 

template <class T>
array<T> tanh(const array<T> &x);

template <class T>
matrix<T> tanh(const matrix<T> &x);
```

### `acosh`

Devuelve el área del coseno hiperbólico de *x*.
```cpp
double acosh(double x);
float acosh(float x);
long double acosh(long double x);

template <class T>
double acosh(T x); 

template <class T>
array<T> acosh(const array<T> &x);

template <class T>
matrix<T> acosh(const matrix<T> &x);
```

### `asinh`

Devuelve el área del seno hiperbólico de *x*.
```cpp
double asinh(double x);
float asinh(float x);
long double asinh(long double x);

template <class T>
double asinh(T x); 

template <class T>
array<T> asinh(const array<T> &x);

template <class T>
matrix<T> asinh(const matrix<T> &x);
```

### `atanh`

Devuelve el área de la tangente hiperbólica de *x*.
```cpp
double atanh(double x);
float atanh(float x);
long double atanh(long double x);

template <class T>
double atanh(T x); 

template <class T>
array<T> atanh(const array<T> &x);

template <class T>
matrix<T> atanh(const matrix<T> &x);
```

## Exponenciales y logaritmos

### `exp`

Devuelve la función exponencial de base *e* de *x*, es decir, *e* elevado a la potencia *x*.
```cpp
double exp(double x);
float exp(float x);
long double exp(long double x);

template <class T>
double exp(T x);

template <class T>
array<T> exp(const array<T> &x);

template <class T>
matrix<T> exp(const matrix<T> &x);
```

### `exp2`

Devuelve la función exponencial de base 2 de *x*, es decir, 2 elevado a la potencia *x*.
```cpp
double exp2(double x);
float exp2(float x);
long double exp2(long double x);

template <class T>
double exp2(T x);

template <class T>
array<T> exp2(const array<T> &x);

template <class T>
matrix<T> exp2(const matrix<T> &x);
```

### `log`

Devuelve el logaritmo natural (base *e*) de *x*.
```cpp
double log(double x);
float log(float x);
long double log(long double x);

template <class T>
double log(T x);

template <class T>
array<T> log(const array<T> &x);

template <class T>
matrix<T> log(const matrix<T> &x);
```

### `log2`

Devuelve el logaritmo binario (base 2) de *x*.
```cpp
double log2(double x);
float log2(float x);
long double log2(long double x);

template <class T>
double log2(T x);

template <class T>
array<T> log2(const array<T> &x);

template <class T>
matrix<T> log2(const matrix<T> &x);
```

### `log10`

Devuelve el logaritmo común (base 10) de *x*.
```cpp
double log10(double x);
float log10(float x);
long double log10(long double x);

template <class T>
double log10(T x);

template <class T>
array<T> log10(const array<T> &x);

template <class T>
matrix<T> log10(const matrix<T> &x);
```

## Funciones potencia

### `pow`

Devuelve *x* elevado a la potencia *y*.
```cpp
double pow(double x, double y);
float pow(float x, float y);
long double pow(long double x, long double y);

template <class T1, class T2>
double pow(T1 x, T2 y);

template <class T>
array<T> pow(const array<T> &x, const array<T> &y);

template <class T>
array<T> pow(const array<T> &x, const T &y);

template <class T>
array<T> pow(const T &x, const array<T> &y);

template <class T>
matrix<T> pow(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> pow(const matrix<T> &x, const T &y);

template <class T>
matrix<T> pow(const T &x, const matrix<T> &y);
```

### `square`

Devuelve *x* elevado al cuadrado.
```cpp
template <class T>
T square(T x);

template <class T>
array<T> square(const array<T> &x);

template <class T>
matrix<T> square(const matrix<T> &x);
```

### `sqrt`

Devuelve la raíz cuadrada de *x*.
```cpp
double sqrt(double x);
float sqrt(float x);
long double sqrt(long double x);

template <class T>
double sqrt(T x);  

template <class T>
array<T> sqrt(const array<T> &x);

template <class T>
matrix<T> sqrt(const matrix<T> &x);
```

### `cbrt`

Devuelve la raíz cúbica de *x*.
```cpp
double cbrt(double x);
float cbrt(float x);
long double cbrt(long double x);

template <class T>
double cbrt(T x);

template <class T>
array<T> cbrt(const array<T> &x);

template <class T>
matrix<T> cbrt(const matrix<T> &x);
```

## Redondeo

### `ceil`

Redondea *x* hacia arriba, devolviendo el menor entero que no es menor que *x*.
```cpp
double ceil(double x);
float ceil(float x);
long double ceil(long double x);

template <class T>
double ceil(T x);

template <class T>
array<T> ceil(const array<T> &x);

template <class T>
matrix<T> ceil(const matrix<T> &x);
```

### `floor`

Redondea *x* hacia abajo, devolviendo el mayor entero que no es mayor que *x*.
```cpp
double floor(double x);
float floor(float x);
long double floor(long double x);

template <class T>
double floor(T x);

template <class T>
array<T> floor(const array<T> &x);

template <class T>
matrix<T> floor(const matrix<T> &x);
```

### `round`

Devuelve el entero más cercano a *x*. 
```cpp
double round(double x);
float round(float x);
long double round(long double x);

template <class T>
double round(T x);

template <class T>
array<T> round(const array<T> &x);

template <class T>
matrix<T> round(const matrix<T> &x);
```

Redondea *x* al número de decimales dados.
```cpp
double round(double x, size_t decimals);
float round(float x, size_t decimals);
long double round(long double x, size_t decimals);

template <class T>
array<T> round(const array<T> &x, size_t decimals);

template <class T>
matrix<T> round(const matrix<T> &x, size_t decimals);
```

### `trunc`

Redondea *x* hacia cero, devolviendo el entero más cercano que no es más grande 
en magnitud que *x*.
```cpp
double trunc(double x);
float trunc(float x);
long double trunc(long double x);

template <class T>
double trunc(T x);

template <class T>
array<T> trunc(const array<T> &x);

template <class T>
matrix<T> trunc(const matrix<T> &x);
```

## Misceláneo

### `abs`

Devuelve el valor absoluto de *x*: *|x|*.
```cpp
double abs(double x);
float abs(float x);
long double abs(long double x);

template <class T>
double abs(T x);

template <class T>
array<T> abs(const array<T> &x);

template <class T>
matrix<T> abs(const matrix<T> &x);
```

### `sign`

La función signo devuelve -1 si *x* es menor que cero, 1 si *x* es mayor que 
cero y *0* si *x* es igual a cero.
```cpp
template <class T>
int sign(T x)

template <class T>
array<int> sign(const array<T> &x);

template <class T>
matrix<int> sign(const matrix<T> &x);
``` 

### `gcd`

Devuelve el máximo común divisor de los enteros *m* y *n*.
```cpp
int gcd(int m, int n);
long gcd(long m, long n);
long long gcd(long long m, long long n);
unsigned gcd(unsigned m, unsigned n);
unsigned long gcd(unsigned long m, unsigned long n);
unsigned long long gcd(unsigned long long m, unsigned long long n);

template <class T>
array<T> gcd(const array<T> &m, const array<T> &n);

template <class T>
array<T> gcd(const array<T> &m, const T &n);

template <class T>
array<T> gcd(const T &m, const array<T> &n);

template <class T>
matrix<T> gcd(const matrix<T> &m, const matrix<T> &n);

template <class T>
matrix<T> gcd(const matrix<T> &m, const T &n);

template <class T>
matrix<T> gcd(const T &m, const matrix<T> &n);
```

### `lcm`

Devuelve el mínimo común múltiplo de los enteros *m* y *n*.
```cpp
int lcm(int m, int n);
long lcm(long m, long n);
long long lcm(long long m, long long n);
unsigned lcm(unsigned m, unsigned n);
unsigned long lcm(unsigned long m, unsigned long n);
unsigned long long lcm(unsigned long long m, unsigned long long n);

template <class T>
array<T> lcm(const array<T> &m, const array<T> &n);

template <class T>
array<T> lcm(const array<T> &m, const T &n);

template <class T>
array<T> lcm(const T &m, const array<T> &n);

template <class T>
matrix<T> lcm(const matrix<T> &m, const matrix<T> &n);

template <class T>
matrix<T> lcm(const matrix<T> &m, const T &n);

template <class T>
matrix<T> lcm(const T &m, const matrix<T> &n);
```

### `maximum`

Devuelve el máximo elemento a elemento de dos arreglos o matrices.
```cpp
template <class T>
array<T> maximum(const array<T> &x, const array<T> &y);

template <class T>
array<T> maximum(const array<T> &x, const T &y);

template <class T>
array<T> maximum(const T &x, const array<T> &y);

template <class T>
matrix<T> maximum(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> maximum(const matrix<T> &x, const T &y);

template <class T>
matrix<T> maximum(const T &x, const matrix<T> &y);
```

### `minimum`

Devuelve el mínimo elemento a elemento de dos arreglos o matrices.
```cpp
template <class T>
array<T> minimum(const array<T> &x, const array<T> &y);

template <class T>
array<T> minimum(const array<T> &x, const T &y);

template <class T>
array<T> minimum(const T &x, const array<T> &y);

template <class T>
matrix<T> minimum(const matrix<T> &x, const matrix<T> &y);

template <class T>
matrix<T> minimum(const matrix<T> &x, const T &y);

template <class T>
matrix<T> minimum(const T &x, const matrix<T> &y);
```