# Optimización y Búsqueda de raíces.

Biblioteca: `optimize.h`

Requisitos: C++11

## Raíces de polinomios

```cpp
template <class InputIterator, class OutputIterator, class T>
void poly_root(
   unsigned int n,
   InputIterator coef,
   OutputIterator roots,
   unsigned int max_iter = 1000,
   T tol = 1e-6
)
```

Calcula las raíces de un polinomio con coeficientes reales.

Argumentos:
* `n`: Grado del polinomio.
* `coef`: Iterador correspondiente a los coeficientes del polinomio.
* `roots`: Iterador correspondiente a las raíces del polinomio. Las raíces pueden ser complejas.
* `max_iter`: Máximo número de iteraciones.
* `tol`: Tolerancia. 

Ejemplo:

Calcula las raíces de ![p(x) = -x^5 + 2x^3 - 5x^2 + 1](https://render.githubusercontent.com/render/math?math=p(x)%20%3D%20-x%5E5%20%2B%202x%5E3%20-%205x%5E2%20%2B%201).

```cpp
#include <cstdio>
#include <complex>
#include "optimize.h"
using namespace std;
using namespace num_met;
int main() {
    const int n = 5;
    double coef[] = {-1, 0, 2, -5, 0, 1};
    complex<double> roots[n];
    poly_root(n, coef, roots);
    for (int i = 0; i < n; ++i)
        printf("%.6f + %.6fi\n", roots[i].real(), roots[i].imag());
    return 0;
}
```

```
-2.073463 + 0.000000i
0.999175 + -1.164333i
0.999175 + 1.164333i
-0.416632 + 0.000000i
0.491745 + 0.000000i
```

## Método de bisección

```cpp
template <class Function, class T>
std::pair<T, unsigned int> bisection(
    Function f,
    T a,
    T b,
    unsigned int max_iter = 1000,
    T tol = 1e-6
)
```

Encuentra una raíz de una función usando el método de bisección.

Argumentos:
* `f`: La función cuya raíz queremos encontrar.
* `a`: Extremo izquiero del intervalo.
* `b`: Extremo derecho del intervalo. Note que `f(a)` y `f(b)` deben tener signos opuestos.
* `max_iter`: Máximo número de iteraciones permitidas.
* `tol`: Tolerancia.

Regresa:
* Un par con la estimación de la raíz y el número de iteraciones realizadas.

Excepciones:
* La función arrojará una excepción cuando el método no converja.

Ejemplo:

Calcula ![\sqrt{2}](https://render.githubusercontent.com/render/math?math=\sqrt{2}).

```cpp
#include <cstdio>
#include <utility>
#include "optimize.h"
using namespace std;
using namespace num_met;

double f(double x) {
    return x*x - 2;
}

int main() {
    pair<double, int> root = bisection(f, 1.0, 2.0);
    printf("Raiz: %.6f\nIteraciones: %d\n", root.first, root.second);
    return 0;
}
```

```
Raiz: 1.414214
Iteraciones: 20
```

## Método de Newton y de la Secante

```cpp
template <class Function1, class Function2, class T>
std::pair<T, unsigned int> newton(
    Function1 f,
    Function2 df,
    T x0,
    T x1,
    unsigned int max_iter = 1000,
    T tol = 1e-6
)
```

Encuentra una raíz de una función usando el método de Newton o el método de la secante.

Argumentos:
* `f`: La función cuya raíz queremos encontrar.
* `df`: La derivada de la función. Si `df` es igual a `nullptr`, utiliza el método de la secante.
* `x0`: Una aproximación inicial a la raíz.
* `x1`: Una segunda aproximación a la raíz. Si `df` no es `nullptr`, se ignora este valor.
* `max_iter`: Máximo número de iteraciones permitidas.
* `tol`: Tolerancia.

Regresa:
* Un par con la estimación de la raíz y el número de iteraciones realizadas.

Excepciones:
* La función arrojará una excepción cuando el método no converja.

Ejemplo:


Calcula ![\sqrt{2}](https://render.githubusercontent.com/render/math?math=\sqrt{2}).

```cpp
#include <cstdio>
#include <utility>
#include "optimize.h"
using namespace std;
using namespace num_met;

double f(double x) {
    return x*x - 2;
}

double df(double x) {
    return 2*x;
}

int main() {
    pair<double, int> root = newton(f, df, 2.0, 1.0);
    printf("Metodo de Newton:\n");
    printf("Raiz: %.6f\nIteraciones: %d\n", root.first, root.second);
    root = newton(f, nullptr, 2.0, 1.0);
    printf("\nMetodo de la secante:\n");
    printf("Raiz: %.6f\nIteraciones: %d\n", root.first, root.second);
    return 0;
}

```

```
Metodo de Newton:
Raiz: 1.414214
Iteraciones: 4

Metodo de la secante:
Raiz: 1.414214
Iteraciones: 6
```