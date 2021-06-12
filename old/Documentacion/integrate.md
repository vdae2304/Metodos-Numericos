# Integración y Ecuaciones Diferenciales Ordinarias.

Biblioteca: `integrate.h`

Requisitos: C++11

## Diferencias finitas

```cpp
template <class OutputIterator>
void central_diff_weights(
    unsigned int n,
    OutputIterator coeff,
    unsigned int order = 1
)
```

Calcula los coeficientes de diferencias centrales finitas.

Argumentos:
* `n`: Número de coeficientes. Debe ser impar.
* `coeff`: Iterador correspondiente a los coeficientes. 
* `order`: El orden de la derivada. Debe ser menor que `n`.

La derivada correspondiente puede ser calculada como 

![\frac{1}{h^{order}} \left( c_0 f\left(x - \frac{n-1}{2}h\right) + \ldots + c_{n-1} f\left(x + \frac{n-1}{2}h\right) \right)](https://render.githubusercontent.com/render/math?math=%5Cfrac%7B1%7D%7Bh%5E%7Border%7D%7D%20%5Cleft(%20c_0%20f%5Cleft(x%20-%20%5Cfrac%7Bn-1%7D%7B2%7Dh%5Cright)%20%2B%20%5Cldots%20%2B%20c_%7Bn-1%7D%20f%5Cleft(x%20%2B%20%5Cfrac%7Bn-1%7D%7B2%7Dh%5Cright)%20%5Cright))

Ejemplo:

Calcula los coeficientes de la primera derivada con 5 puntos.

```cpp
#include <cstdio>
#include "integrate.h"
using namespace num_met;
int main() {
    const int n = 5;
    double coef[n];
    
    central_diff_weights(n, coef);
    for (int i = 0; i < n; ++i)
        printf("%.6f ", coef[i]);
    printf("\n");
    
    return 0;
}
```

```
0.083333 -0.666667 0.000000 0.666667 -0.083333 
```

## Diferenciación

```cpp
template <class Function, class T>
T derivative(
    Function f,
    T x,
    T h = 1e-6,
    unsigned int n = 1,
    unsigned int npts = 3
)
```

Calcula una derivada en un punto.

Argumentos:
* `f`: La función a derivar.
* `x`: El punto sobre el cual derivar. 
* `h`: Tamaño del paso. Utilizar valores muy pequeños de `h` podría generar errores de precisión.
* `n`: Orden de la derivada. 
* `npts`: Número de puntos para aproximar la derivada. Debe ser impar y mayor que `n`.

Regresa:
* Una aproximación a la `n`-ésima derivada de `f` en `x`. 

Ejemplo: 

Calcula ![\left. \frac{d}{dx} x^2e^x \right|_{x=1}](https://render.githubusercontent.com/render/math?math=%5Cleft.%20%5Cfrac%7Bd%7D%7Bdx%7D%20x%5E2e%5Ex%20%5Cright%7C_%7Bx%3D1%7D) y ![\left. \frac{d^2}{dx^2} x^2e^x \right|_{x=1}](https://render.githubusercontent.com/render/math?math=%5Cleft.%20%5Cfrac%7Bd%5E2%7D%7Bdx%5E2%7D%20x%5E2e%5Ex%20%5Cright%7C_%7Bx%3D1%7D).

```cpp
#include <cstdio>
#include <cmath>
#include "integrate.h"
using namespace num_met;

double f(double x) {
    return x*x*exp(x);
}

int main() {
    printf("%.6f\n", derivative(f, 1.0));
    printf("%.6f\n", derivative(f, 1.0, 1e-3, 2));
    return 0;
}
```

```
8.154845
19.027978
```

## Integral definida

```cpp
template <class Function, class T>
T integrate(
    Function f,
    T a,
    T b,
    unsigned int n = 1024
)
```

Evalua una integral definida.

Argumentos:
* `f`: La función a integrar.
* `a`: Límite inferior de integración.
* `b`: Límite superior de integración.
* `n`: Tamaño de la partición.

Regresa:
* El valor de la integral.

Ejemplo:

Calcula ![\int_0^1 e^{-x^2}dx](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20e%5E%7B-x%5E2%7Ddx)

```cpp
#include <cstdio>
#include <cmath>
#include "integrate.h"
using namespace num_met;

double f(double x) {
    return exp(-x*x);
}

int main() {
    printf("%.6f\n", integrate(f, 0.0, 1.0));
    return 0;
}
```

```
0.746824
```

## Integral doble

```cpp
template <class Function, class T, class LowerLimit, class UpperLimit>
T double_integrate(
    Function f,
    T a,
    T b,
    LowerLimit c,
    UpperLimit d,
    unsigned int m = 256,
    unsigned int n = 256
)
```

Evalua una integral doble.

Argumentos:

* `f`: La función a integrar. Se integrará primero respecto a la segunda variable.
* `a`, `b`: Límites inferior y superior respecto a la primera variable.
* `c`, `d`: Límites inferior y superior respecto a la segunda variable. Ambos como función de la primera variable.
* `m`: Tamaño de la partición sobre el primer eje.
* `n`: Tamaño de la partición sobre el segundo eje.

Regresa:
* El valor de la integral. 

Ejemplo:

Calcula ![\int_0^1 \int_{x-1}^{x+1} xy^2 dydx](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20%5Cint_%7Bx-1%7D%20%5E%7Bx%20%2B%201%7D%20xy%5E2%20dydx).

```cpp
#include <cstdio>
#include "integrate.h"
using namespace num_met;

double f(double x, double y) {
    return x*y*y;
}

double c(double x) {
    return x - 1;
}

double d(double x) {
    return x + 1;
}

int main() {
    printf("%.6f\n", double_integrate(f, 0.0, 1.0, c, d));
    return 0;
}
```

```
0.833333
```

## Integral triple

```cpp
template <
    class Function,
    class T,
    class LowerLimit1,
    class UpperLimit1,
    class LowerLimit2,
    class UpperLimit2
>
T triple_integrate(
    Function f,
    T a,
    T b,
    LowerLimit1 c,
    UpperLimit1 d,
    LowerLimit2 g,
    UpperLimit2 h,
    unsigned int l = 128,
    unsigned int m = 128,
    unsigned int n = 128
)
```

Evalua una integral triple.

Argumentos:

* `f`: La función a integrar. Se integrará primero respecto a la tercera variable y posteriormente respecto a la segunda variable.
* `a`, `b`: Límites inferior y superior respecto a la primera variable.
* `c`, `d`: Límites inferior y superior respecto a la segunda variable. Ambos como función de la primera variable.
* `g`, `h`: Límites inferior y superior respecto a la tercera variable. Ambos como función de la primera y segunda variable.
* `l`: Tamaño de la partición sobre el primer eje.
* `m`: Tamaño de la partición sobre el segundo eje.
* `n`: Tamaño de la partición sobre el tercer eje.

Regresa:
* El valor de la integral. 

Ejemplo:

Calcula ![\int_0^1 \int_0^x \int_{x-y}^{x+y} xe^{y-z}dzdydx](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1+%5Cint_0%5Ex+%5Cint_%7Bx-y%7D%5E%7Bx%2By%7D+xe%5E%7By-z%7Ddzdydx).

```cpp
#include <cstdio>
#include <cmath>
#include "integrate.h"
using namespace num_met;

double f(double x, double y, double z) {
    return x*exp(y - z);
}

double c(double x) {
    return 0;
}

double d(double x) {
    return x;
}

double g(double x, double y) {
    return x - y;
}

double h(double x, double y) {
    return x + y;
}

int main() {
    printf("%.6f\n", triple_integrate(f, 0.0, 1.0, c, d, g, h));
    return 0;
}
```

```
0.207277
```

## Ecuaciones diferenciales ordinarias

```cpp
template <
    class Function,
    class InputIterator,
    class InitialValue,
    class OutputIterator
>
void solve_ode(
    Function f,
    InputIterator xfirst,
    InputIterator xlast,
    InitialValue y0,
    OutputIterator yfirst
)
```

Resuelve el problema de valor inicial:

![\begin{align*} \frac{dy}{dx} &= f(x, y)\\ y(x_0) &= y_0 \end{align*}](https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign*%7D%20%5Cfrac%7Bdy%7D%7Bdx%7D%20%26%3D%20f(x%2C%20y)%5C%5C%20y(x_0)%20%26%3D%20y_0%20%5Cend%7Balign*%7D)

donde ![y](https://render.githubusercontent.com/render/math?math=y) puede ser una función escalar o una función vectorial.

Argumentos:
* `f`: La función que define la ecuación.
* `xfirst`, `xlast`: Iteradores correspondientes a los valores del dominio de la ecuacion. El rango utilizado es [`xfirst`, `xlast`).
* `y0`: La condición inicial.
* `yfirst`: Iterador correspondiente a los valores de la solución. 

Comentarios:
* Cuando se trate de un sistema de ecuaciones diferenciales, el tipo asociado a los elementos de la solución debe tener sobrecargado los operadores de suma y división por escalar.

Ejemplo:

Resuelve ![\frac{dy}{dx} = 6x - 3y + 5](https://render.githubusercontent.com/render/math?math=%5Cfrac%7Bdy%7D%7Bdx%7D%20%3D%206x%20-%203y%20%2B%205), ![y(0) = 3](https://render.githubusercontent.com/render/math?math=y(0)%20%3D%203) sobre ![\[0, 1\]](https://render.githubusercontent.com/render/math?math=%5B0%2C%201%5D). La solución analítica es ![y(x) = 2e^{-3x} + 2x + 1](https://render.githubusercontent.com/render/math?math=y(x)%20%3D%202e%5E%7B-3x%7D%20%2B%202x%20%2B%201).

```cpp
#include <cstdio>
#include "interpolate.h" // linspace
#include "integrate.h"
using namespace num_met;

double f(double x, double y) {
    return 6*x - 3*y + 5;
}

int main() {
    const int n = 11;
    double a = 0, b = 1, y0 = 3;
    double x[n], y[n];

    linspace(a, b, x, x + n);
    solve_ode(f, x, x + n, y0, y);
    
    for (int i = 0; i < n; ++i)
        printf("%.6f\t%.6f\n", x[i], y[i]);
    return 0;
}
```

```
0.000000        3.000000
0.100000        2.681675
0.200000        2.497680
0.300000        2.413203
0.400000        2.402451
0.500000        2.446318
0.600000        2.530649
0.700000        2.644957
0.800000        2.781474
0.900000        2.934443
1.000000        3.099600
```

Resuelve 

![\begin{align*} \frac{dx}{dt} &= x + 2y, & x(0) &= 6\\ \frac{dy}{dt} &= 3x + 2y, & y(0) &= 4 \end{align*}](https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign*%7D%20%5Cfrac%7Bdx%7D%7Bdt%7D%20%26%3D%20x%20%2B%202y%2C%20%26%20x(0)%20%26%3D%206%5C%5C%20%5Cfrac%7Bdy%7D%7Bdt%7D%20%26%3D%203x%20%2B%202y%2C%20%26%20y(0)%20%26%3D%204%20%5Cend%7Balign*%7D)

sobre ![\[0, 1\]](https://render.githubusercontent.com/render/math?math=%5B0%2C%201%5D). La solución analítica es ![x(t) = 4e^{4t} + 2e^{-t}](https://render.githubusercontent.com/render/math?math=x(t)%20%3D%204e%5E%7B4t%7D%20%2B%202e%5E%7B-t%7D) y ![y(t) = 6e^{4t} - 2e^{-t}](https://render.githubusercontent.com/render/math?math=y(t)%20%3D%206e%5E%7B4t%7D%20-%202e%5E%7B-t%7D).

```cpp
#include <cstdio>
#include <valarray>
#include "interpolate.h" // linspace
#include "integrate.h"
using namespace std;
using namespace num_met;

valarray<double> f(double t, valarray<double> y) {
    return valarray<double> {
        y[0] + 2*y[1],
        3*y[0] + 2*y[1]
    };
}

int main() {
    const int n = 11;
    double a = 0, b = 1, x[n];
    valarray<double> y0 = {6, 4}, y[n];

    linspace(a, b, x, x + n);
    solve_ode(f, x, x + n, y0, y);
    
    for (int i = 0; i < n; ++i)
        printf("%.6f\t%.6f\t%.6f\n", x[i], y[i][0], y[i][1]);
    return 0;
}
```

```
0.000000        6.000000        4.000000
0.100000        7.776608        7.140725
0.200000        10.538535       11.714148
0.300000        14.759665       18.435405
0.400000        21.147917       28.370274
0.500000        30.760237       43.107700
0.600000        45.174130       65.017135
0.700000        66.743564       97.632418
0.800000        98.980711       146.224420
0.900000        147.125407      218.655261
1.000000        218.994646      326.652570
```