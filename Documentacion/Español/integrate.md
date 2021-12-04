# Integración y EDOs

## Content

- [Integración](#Integración)
- [Integral múltiple](Integral-múltiple)
- [Ecuaciones diferenciales ordinarias](#Ecuaciones-diferenciales-ordinarias)

## Integración

### `quad`

Calcula una integral definida usando cuadratura Gaussiana.
```cpp
template <class T, class Function = T(T)>
T quad(
    Function f, T a, T b,
    bool show = false,
    T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
);
```

#### Argumentos:

- `f`: Función a integrar.
- `a`: Límite inferior de integración. Usa `-numcpp::inf` para *-infinito*.
- `b`: Límite superior de integración. Usa `numcpp::inf` para *+infinito*.
- `show`: Si es `true`, imprime una breve descripción de la causa de 
terminación. Por defecto es `false`.
- `tol`: La tolerancia absoluta deseada.
- `rtol`: La tolerancia relativa deseada.
- `maxiter`: Máximo orden de cuadratura.

#### Ejemplo

Calcula ![$\int_0^1 e^{-x}\cos(2x^2 + 1)dx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20e%5E%7B-x%7D%5Ccos(2x%5E2%20%2B%201)dx).

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x) {
    return np::exp(-x) * np::cos(2*x*x + 1);
}

int main() {
    double a = 0, b = 1;
    double integral = scicpp::quad(f, a, b, true);
    cout << "Integral: " << integral << "\n";
    return 0;
}
```

```
[Out] The final result is 0.0508862 after 28 function evaluations
      with error 4.77208e-09
      Integral: 0.0508862
```

### `fixed_quad`

Calcula una integral definida usando cuadratura Gaussiana de orden fijo.
```cpp
template <class T, class Function = T(T)>
T fixed_quad(Function f, T a, T b, size_t n);
```

#### Argumentos:

- `f`: Función a integrar.
- `a`: Límite inferior de integración. Usa `-numcpp::inf` para *-infinito*.
- `b`: Límite superior de integración. Usa `numcpp::inf` para *+infinito*.
- `n`: Orden de cuadratura.

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x) {
    return np::exp(-x) * np::cos(2*x*x + 1);
}

int main() {
    double a = 0, b = 1;
    cout << "Aproximacion de 5 puntos: " << scicpp::fixed_quad(f, a, b, 5) 
         << "\n";
    return 0;
}
```

```
[Out] Aproximacion de 5 puntos: 0.0508857
```

### `leggauss`

Calcula la muestra de puntos y los pesos de una cuadratura de Gauss-Legendre. 
Esta muestra de puntos y pesos integrarán correctamente polinomios de grado 
![$2n-1$](https://render.githubusercontent.com/render/math?math=2n-1) o menos 
sobre el intervalo ![$[-1,1]$](https://render.githubusercontent.com/render/math?math=%5B-1%2C1%5D).
```cpp
template <class T>
void leggauss(
    size_t n, numcpp::array<T> &points, numcpp::array<T> &weights
);
```

#### Argumentos:

- `n`: Número de puntos y pesos. Debe ser mayor a 1.
- `points`: Arreglo que contendrá la muestra de puntos.
- `weights`: Arreglo que contendrá los pesos.

#### Ejemplo

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> puntos, pesos;
    scicpp::leggauss(5, puntos, pesos);
    cout << "Puntos:\n" << puntos << "\nPesos:\n" << pesos << "\n";
    return 0;
}
```

```
[Out] Puntos:
      [-0.90617985, -0.53846931, 8.3341178e-18, 0.53846931, 0.90617985]
      Pesos:
      [0.23692689, 0.47862867, 0.56888889, 0.47862867, 0.23692689]
```

### `romberg`

Calcula una integral definida usando el método de Romberg.
```cpp
template <class T, class Function = T(T)>
T romberg(
    Function f, T a, T b,
    bool show = false,
    T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 10
);
```

#### Argumentos:

- `f`: Función a integrar.
- `a`: Límite inferior de integración. Debe ser finito.
- `b`: Límite superior de integración. Debe ser finito.
- `show`: Si es `true`, imprime la matriz triangular mostrando las 
extrapolaciones de Richardson. Por defecto es `false`.
- `tol`: La tolerancia absoluta deseada.
- `rtol`: La tolerancia relativa deseada.
- `maxiter`: Máximo orden de extrapolación.


#### Ejemplo

Calcula ![$\int_0^1 e^{-x}\cos(2x^2 + 1)dx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20e%5E%7B-x%7D%5Ccos(2x%5E2%20%2B%201)dx).
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x) {
    return np::exp(-x) * np::cos(2*x*x + 1);
}

int main() {
    double a = 0, b = 1;
    double integral = scicpp::romberg(f, a, b, true);
    cout << "Integral: " << integral << "\n";
    return 0;
}
```

```
[Out] Steps StepSize Results
          1        1 0.0880522
          2      0.5 0.0654782 0.0579536
          4     0.25 0.0545416 0.0508961 0.0504256
          8    0.125 0.0517954   0.05088 0.0508789 0.0508861
         16   0.0625 0.0511131 0.0508857 0.0508861 0.0508862 0.0508862
         32  0.03125 0.0509429 0.0508861 0.0508862 0.0508862 0.0508862 0.0508862
         64 0.015625 0.0509003 0.0508862 0.0508862 0.0508862 0.0508862 0.0508862 0.0508862
      The final result is 0.0508862 after 65 function evaluations
      with error 1.61381e-11
      Integral: 0.0508862 
```

### `newton_cotes`

Calcula los pesos de integración por Newton-Cotes. Sean ![$a = x_0 < x_1 < \ldots < x_n = b$](https://render.githubusercontent.com/render/math?math=a%20%3D%20x_0%20%3C%20x_1%20%3C%20%5Cldots%20%3C%20x_n%20%3D%20b) 
una muestra de puntos equi-espaciados sobre el intervalo ![$[a,b]$](https://render.githubusercontent.com/render/math?math=%5Ba%2Cb%5D). La fórmula de 
Newton-Cotes para la integral es

![$\int_a^b f(x)dx \approx \frac{b - a}{n}\sum_{i = 0}^{n} w_i f(x_i)$](https://render.githubusercontent.com/render/math?math=%5Cint_a%5Eb%20f(x)dx%20%5Capprox%20%5Cfrac%7Bb%20-%20a%7D%7Bn%7D%5Csum_%7Bi%20%3D%200%7D%5E%7Bn%7D%20w_i%20f(x_i))

```cpp
template <class T>
void newton_cotes(size_t n, numcpp::array<T> &weights);
```

#### Argumentos:

- `n`: Número de sub-intervalos.
- `weights`:  Arreglo que contendrá los pesos.

#### Ejemplo

Calcula ![$\int_0^1 (3xe^x + 5x - 1)dx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20(3xe%5Ex%20%2B%205x%20-%201)dx). El resultado 
analítico es ![$4.5$](https://render.githubusercontent.com/render/math?math=4.5).
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;
int main() {
    np::array<double> integral(3);

    for (int n = 1; n <= 3; ++n) {
        np::array<double> x = numcpp::linspace(0.0, 1.0, n + 1);
        np::array<double> y = 3.0*x*np::exp(x) + 5.0*x - 1.0;
        np::array<double> pesos;
        scicpp::newton_cotes(n, pesos);
        integral[n - 1] = np::sum(pesos * y) / n;
    }

    cout << "Regla del trapecio: " << integral[0] << "\n";
    cout << "Regla de Simpson: " << integral[1] << "\n";
    cout << "Regla de Simpson 3/8: " << integral[2] << "\n";

    return 0;
}
```

```
[Out] Regla del trapecio: 5.57742
      Regla de Simpson: 4.50786
      Regla de Simpson 3/8: 4.50351
```

## Integral múltiple

### `dblquad`

Calcula una integral doble. Devuelve la integral doble (definida) de 
![$f(x, y)$](https://render.githubusercontent.com/render/math?math=f(x%2C%20y))
sobre el rectángulo ![$a_x \leq x \leq b_x$](https://render.githubusercontent.com/render/math?math=a_x%20%5Cleq%20x%20%5Cleq%20b_x), ![$a_y(x) \leq y \leq b_y(x)$](https://render.githubusercontent.com/render/math?math=a_y(x)%20%5Cleq%20y%20%5Cleq%20b_y(x)).
```cpp
template <
    class T, class Function = T(T, T),
    class LowerLimit = T(T), class UpperLimit = T(T)
>
T dblquad(
    Function f, T ax, T bx, LowerLimit ay, UpperLimit by,
    bool show = false,
    T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
);
```

#### Argumentos:

- `f`: Función a integrar.
- `ax`, `bx`: Límites inferior y superior de integración para *x*.
- `ay`, `by`: Límites inferior y superior de integración para *y*.
- `show`: Si es `true`, imprime una breve descripción de la causa de 
terminación. Por defecto es `false`.
- `tol`: La tolerancia absoluta deseada.
- `rtol`: La tolerancia relativa deseada.
- `maxiter`: Máximo orden de cuadratura.

#### Ejemplo

Calcula ![$\int_0^1 \int_0^x (3xy^2 - x^2 + y)dydx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20%5Cint_0%5Ex%20(3xy%5E2%20-%20x%5E2%20%2B%20y)dydx).

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x, double y) {
    return 3*x*y*y - x*x + y;
}

int main() {
    double ax = 0, bx = 1;
    auto ay = [](double x) { return 0; };
    auto by = [](double x) { return x; };
    double integral = scicpp::dblquad(f, ax, bx, ay, by, true);
    cout << "Integral: " << integral << "\n";
    return 0;
}
```

```
[Out] The final result is 0.116667 after 29 function evaluations
      with error 1.66533e-16
      Integral: 0.116667
```

### `tplquad`

Calcula una integral triple. Devuelve la integral triple (definida) de 
![$f(x, y, z)$](https://render.githubusercontent.com/render/math?math=f(x%2C%20y%2C%20z))
sobre el sólido ![$a_x \leq x \leq b_x$](https://render.githubusercontent.com/render/math?math=a_x%20%5Cleq%20x%20%5Cleq%20b_x), ![$a_y(x) \leq y \leq b_y(x)$](https://render.githubusercontent.com/render/math?math=a_y(x)%20%5Cleq%20y%20%5Cleq%20b_y(x)), ![$a_z(x, y) \leq z \leq b_z(x, y)$](https://render.githubusercontent.com/render/math?math=a_z(x%2C%20y)%20%5Cleq%20z%20%5Cleq%20b_z(x%2C%20y)).
```cpp
template <
    class T, class Function = T(T, T, T),
    class LowerLimit1 = T(T), class UpperLimit1 = T(T),
    class LowerLimit2 = T(T, T), class UpperLimit2 = T(T, T)
>
T tplquad(
    Function f,
    T ax, T bx,
    LowerLimit1 ay, UpperLimit1 by,
    LowerLimit2 az, UpperLimit2 bz,
    bool show = false,
    T tol = 1e-8, T rtol = 1e-8, size_t maxiter = 50
);
```

#### Argumentos:

- `f`: Función a integrar.
- `ax`, `bx`: Límites inferior y superior de integración para *x*.
- `ay`, `by`: Límites inferior y superior de integración para *y*.
- `ay`, `by`: Límites inferior y superior de integración para *z*.
- `show`: Si es `true`, imprime una breve descripción de la causa de 
terminación. Por defecto es `false`.
- `tol`: La tolerancia absoluta deseada.
- `rtol`: La tolerancia relativa deseada.
- `maxiter`: Máximo orden de cuadratura.

#### Ejemplo

Calcula ![$\int_0^1 \int_0^x \int_0^{x + y} \left(3xy^2 - \frac{z^2}{2} + 5xyz\right)dzdydx$](https://render.githubusercontent.com/render/math?math=%5Cint_0%5E1%20%5Cint_0%5Ex%20%5Cint_0%5E%7Bx%20%2B%20y%7D%20%5Cleft(3xy%5E2%20-%20%5Cfrac%7Bz%5E2%7D%7B2%7D%20%2B%205xyz%5Cright)dzdydx).

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

double f(double x, double y, double z) {
    return 3*x*y*y - z*z/2 + 5*x*y*z;
}

int main() {
    double ax = 0, bx = 1;
    auto ay = [](double x) { return 0; };
    auto by = [](double x) { return x; };
    auto az = [](double x, double y) { return 0; };
    auto bz = [](double x, double y) { return x + y; };
    double integral = scicpp::tplquad(f, ax, bx, ay, by, az, bz, true);
    cout << "Integral: " << integral << "\n";
    return 0;
}
```

```
[Out] The final result is 0.756944 after 99 function evaluations
      with error 7.77156e-16
      Integral: 0.756944
```

## Ecuaciones diferenciales ordinarias

### `OdeResult`

Clase devuelta por `solve_ivp`.
```cpp
template <class T>
class OdeResult {
public:
    numcpp::array<T> t;
    numcpp::matrix<T> y;
    size_t nfev;
};
```

#### Atributos:

- `t`: Puntos en el tiempo.
- `y`: Valores de la solución en `t`. Cada renglón de `y` representa el valor 
de la solución en un tiempo fijo.
- `nfev`: Número de evaluaciones del lado derecho del sistema.

### `solve_ivp`

Resuelve un problema de valor inicial para un sistema de ecuaciones diferenciales 
ordinarias.

![$\begin{align*}\frac{dy}{dt} &= f(t, y)\\y(t_0) &= y_0\end{align*}$](https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign*%7D%5Cfrac%7Bdy%7D%7Bdt%7D%20%26%3D%20f(t%2C%20y)%5C%5Cy(t_0)%20%26%3D%20y_0%5Cend%7Balign*%7D)

```cpp
template <class T, class Function>
OdeResult<T> solve_ivp(
    Function f, T t0, T tf, const numcpp::array<T> &y0,
    std::string method = "RK45",
    T first_step = 1e-3, T max_step = numcpp::inf,
    T tol = 1e-6, T rtol = 1e-3
);
```

#### Argumentos:

- `f`: Lado derecho del sistema.
- `t0`: Tiempo inicial.
- `tf`: Tiempo final.
- `y0`: Estado inicial.
- `method`: Método a usar. Debe ser alguno de
    * `"RK23"`: Método explícito de Runge-Kutta de orden 3(2). El error es 
    controlado utilizando un método de segundo orden de precisión, pero los 
    pasos son tomados utilizando un método de tercer orden.
    * `"RK45"` (por defecto): Método explícito de Runge-Kutta de orden 5(4). El 
    error es controlado utilizando un método de cuarto orden de precisión, pero 
    los pasos son tomados utilizando un método de quinto orden.
- `first_step`: Tamaño de paso inicial.
- `max_step`: Tamaño de paso máximo permitido. Por defecto, el tamaño de paso no 
está acotado y es determinado enteramente por el método.
- `tol`: La tolerancia absoluta deseada.
- `rtol`: La tolerancia relativa deseada.

#### Ejemplo 

Resuelve el problema de valor inicial ![$\frac{dy}{dt} = 0.125y(5 - y), y(0) = 0.1$](https://render.githubusercontent.com/render/math?math=%5Cfrac%7Bdy%7D%7Bdt%7D%20%3D%200.125y(5%20-%20y)%2C%20y(0)%20%3D%200.1) 
para ![$0 \leq y \leq 15$](https://render.githubusercontent.com/render/math?math=0%20%5Cleq%20y%20%5Cleq%2015). La solución analítica es
![$y = \frac{5}{1 + 49e^{-0.625t}}$](https://render.githubusercontent.com/render/math?math=y%20%3D%20%5Cfrac%7B5%7D%7B1%20%2B%2049e%5E%7B-0.625t%7D%7D).

```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/integrate.h"
using namespace std;
namespace np = numcpp;

np::array<double> f(double t, const np::array<double> &y) {
    return 0.125*y*(5. - y);
}

int main() {
    double t0 = 0, tf = 15;
    np::array<double> y0 = {0.1};

    // Solucion numerica y analitica.
    scicpp::OdeResult<double> resultado = scicpp::solve_ivp(f, t0, tf, y0);
    np::array<double> logistica = 5./(1. + 49.*np::exp(-0.625*resultado.t));

    // Exporta el resultado en un archivo .csv.
    np::matrix<double> df;
    df = np::column_stack(resultado.t, resultado.y);
    df = np::column_stack(df, logistica);
    np::save_txt("solucion.csv", df, ',', {"t", "y", "logistica"});

    cout << "Evaluaciones del lado derecho: " << resultado.nfev << "\n";

    return 0;
}
```

```
[Out] Evaluaciones del lado derecho: 143
```

```
[solucion.csv]
t,y,logistica
0,0.1,0.1
0.001,0.10006127,0.10006127
0.003,0.10018392,0.10018392
0.007,0.10042965,0.10042965
0.015,0.1009229,0.1009229
0.031,0.10191651,0.10191651
0.063,0.10393257,0.10393257
0.127,0.10808245,0.10808245
0.255,0.1168739,0.1168739
0.511,0.13659879,0.13659878
1.023,0.18619589,0.18619559
2.047,0.34172001,0.34170872
4.095,1.0443452,1.0437759
6.143,2.4362453,2.4344584
8.191,3.8680478,3.8669515
10.239,4.6249673,4.6233487
12.287,4.8909706,4.8892539
15,4.9810645,4.9793055
```

<img src="../Figuras/ode-plot.png" alt="Logistic function" height="auto" width="500" />

### `OdeSolver`

Clase base para los métodos.
```cpp
template <class T, class Function>
class OdeSolver {
public:
    size_t n;
    Function f;
    T t;
    numcpp::array<T> y;
    T step_size, max_step;
    T tol, rtol;
    size_t nfev;
};
```

#### Atributos:

- `n`: Número de ecuaciones.
- `f`: Lado derecho del sistema.
- `t`: Tiempo actual.
- `y`: Estado actual.
- `step_size`: Tamaño del último paso.
- `max_step`: Tamaño de paso máximo permitido.
- `tol`: La tolerancia absoluta deseada.
- `rtol`: La tolerancia relativa deseada.
- `nfev`: Número de evaluaciones del lado derecho del sistema.

#### Constructor

```cp
OdeSolver(
    Function f, T t0, const numcpp::array<T> &y0,
    T first_step = 1e-3, T max_step = numcpp::inf,
    T tol = 1e-6, T rtol = 1e-3
);
```

#### Métodos

Realiza un paso de integración.
```cpp
virtual void step();
```

### `RK23`

 Método explícito de Runge-Kutta de orden 3(2). 
 
 El error es controlado utilizando un método de segundo orden de precisión, 
 pero los pasos son tomados utilizando un método de tercer orden.
```cpp
template <class T, class Function>
class RK23 : public OdeSolver<T, Function> {
public:
    RK23(
        Function f, T t0, const numcpp::array<T> &y0,
        T first_step = 1e-3, T max_step = numcpp::inf,
        T tol = 1e-6, T rtol = 1e-3
    );

    void step();
};
```

### `RK45`

Método explícito de Runge-Kutta de orden 5(4). 

El error es controlado utilizando un método de cuarto orden de precisión, pero 
los pasos son tomados utilizando un método de quinto orden.
```cpp
template <class T, class Function>
class RK45 : public OdeSolver<T, Function> {
public:
    RK45(
        Function f, T t0, const numcpp::array<T> &y0,
        T first_step = 1e-3, T max_step = numcpp::inf,
        T tol = 1e-6, T rtol = 1e-3
    );

    void step();
};
```