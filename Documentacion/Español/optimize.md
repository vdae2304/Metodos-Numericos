# Optimización y búsqueda de raíces

## Contenido

- [Búsqueda de raíces (Funciones escalares)](#Búsqueda-de-raíces-Funciones-escalares)

## Búsqueda de raíces (Funciones escalares)

### `RootResults`

Clase devuelta por los métodos de búsqueda de raíces.

```cpp
template <class T>
class RootResults {
public:
    T root;
    size_t iterations;
    size_t function_calls;
    bool converged;
    std::string status;
};
```

#### Atributos

- `root`: Ubicación estimada de la raíz.
- `iterations`: Número de iteraciones requeridas para encontrar la raíz.
- `function_calls`: Número de veces que la función fue llamada.
- `converged`: `true` si el método convergió.
- `status`: Descripción de la causa de terminación.


### `bisect`

Calcula una raíz de una función dentro de un intervalo utilizando bisección.
```cpp
template <class T, class Function = T(T)>
RootResults<T> bisect(
    Function f, T a, T b, T tol = 1e-9, size_t maxiter = 100
);
```

#### Argumentos

- `f`: La función cuya raíz se quiere calcular. Debe ser continua y `f(a)` y 
`f(b)` deben tener signos opuestos.
- `a`: Un extremo del intervalo delimitador *[a, b]*.
- `b`: El otro extremo del intervalo delimitador *[a, b]*.
- `tol`: La tolerancia deseada.
- `maxiter`: Máximo número de iteraciones.

#### Ejemplo

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

int main() {
    scicpp::RootResults<double> output = scicpp::bisect(f, 0.0, 2.0);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 30 iterations, value is 1.25992
```

### `newton`

Calcula una raíz de una función usando el método de Newton-Raphson.
```cpp
template <class T, class Function = T(T), class Derivative = T(T)>
RootResults<T> newton(
    Function f, T x0, Derivative df, T tol = 1e-9, size_t maxiter = 50
);
```

#### Argumentos:

- `f`: La función cuya raíz se quiere calcular.
- `x0`: Una estimación inicial de la raíz que debe estar cerca de la verdadera 
raíz.
- `df`: La derivada de la función.
- `tol`: La tolerancia deseada.
- `maxiter`: Máximo número de iteraciones.

#### Ejemplo

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

double df(double x) {
    return 3*x*x;
}

int main() {
    scicpp::RootResults<double> output = scicpp::newton(f, 3.0, df);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 7 iterations, value is 1.25992
```

### `secant`

Calcula una raíz de una función usando el método de la secante.
```cpp
template <class T, class Function = T(T)>
RootResults<T> secant(
    Function f, T x0, T x1, T tol = 1e-9, size_t maxiter = 50
);
```

#### Argumentos:

- `f`: La función cuya raíz se quiere calcular.
- `x0`: Una estimación inicial de la raíz que debe estar cerca de la verdadera 
raíz.
- `x1`: Otra estimación de la raíz que debe estar cerca de la verdadera raíz.
- `tol`: La tolerancia deseada.
- `maxiter`: Máximo número de iteraciones.

#### Ejemplo

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

int main() {
    scicpp::RootResults<double> output = scicpp::secant(f, 3.0, 2.9);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 9 iterations, value is 1.25992
```

### `halley`

Calcula una raíz de una función usando el método de Halley.
```cpp
template <
    class T, class Function = T(T),
    class Derivative = T(T), class Derivative2 = T(T)
>
RootResults<T> halley(
    Function f, T x0, Derivative df, Derivative2 df2,
    T tol = 1e-9, size_t maxiter = 50
);
```

#### Argumentos:

- `f`: La función cuya raíz se quiere calcular.
- `x0`: Una estimación inicial de la raíz que debe estar cerca de la verdadera 
raíz.
- `df`: La primera derivada de la función.
- `df2`: La segunda derivada de la función.
- `tol`: La tolerancia deseada.
- `maxiter`: Máximo número de iteraciones.

#### Ejemplo

```cpp
#include <iostream>
#include "scicpp/optimize.h"
using namespace std;

double f(double x) {
    return x*x*x - 2;
}

double df(double x) {
    return 3*x*x;
}

double df2(double x) {
    return 6*x;
}

int main() {
    scicpp::RootResults<double> output = scicpp::halley(f, 3.0, df, df2);
    cout << output.status << "\n";
    return 0;
}
```

```
[Out] Converged after 5 iterations, value is 1.25992
```