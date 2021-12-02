# Optimización y búsqueda de raíces

## Contenido

- [Búsqueda de raíces (Funciones escalares)](#Búsqueda-de-raíces-Funciones-escalares)
- [Optimización local (multivariada)](#Optimización-local-multivariada)

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

## Optimización local (multivariada)

### `OptimizeResult`

Clase devuelta por los optimizadores.
```cpp
template <class T>
class OptimizeResult {
public:
    T fun;
    numcpp::array<T> x;
    numcpp::array<T> jac;
    numcpp::matrix<T> hess;
    bool success;
    std::string status;
    size_t niter;
    size_t nfev, njev, nhev;
};
```

#### Atributos 

- `fun`: Valor de la función objetivo.
- `x`: La solución de la optimización.
- `jac`: Valor del gradiente de la función objetivo.
- `hess`: Valor del Hessiano de la función objetivo.
- `success`: Si el optimizador terminó exitosamente o no.
- `status`: Descripción de la causa de terminación.
- `niter`: Número de iteraciones realizadas por el optimizador.
- `nfev`, `njev`, `nhev`: Número de evaluaciones de la función objetivo y de 
su gradiente y Hessiano.

### `minimize_cg`

Minimiza una función utilizando un algoritmo de gradiente conjugado no lineal. 
Esta implementación utiliza la fórmula de Polak y Ribier.
```cpp
template <class T, class Function, class Jacobian>
OptimizeResult<T> minimize_cg(
    Function f, const numcpp::array<T> x0, Jacobian jac,
    T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
);
```

#### Atributos

- `f`: Función objetivo a minimizar.
- `x0`: Una estimación inicial para el valor óptimo de `x`.
- `jac`: Una función que devuelve el vector gradiente de `f` en `x`.
- `gtol`: El algoritmo se detiene cuando la norma del gradiente es menor que 
`gtol`.
- `ordnorm`: Orden a usar para la norma del gradiente.
- `maxiter`: Máximo número de iteraciones a realizar.

#### Ejemplo

Calcula un mínimo local de la función de Rosenbrock. La función de Rosenbrock 
está definida por

![$f(\mathbf{x}) = \sum_{i=1}^{n-1} 100(x_{i+1}-x_i^2)^2 + (1-x_i)^2$](https://render.githubusercontent.com/render/math?math=f(%5Cmathbf%7Bx%7D)%20%3D%20%5Csum_%7Bi%3D1%7D%5E%7Bn-1%7D%20100(x_%7Bi%2B1%7D-x_i%5E2)%5E2%20%2B%20(1-x_i)%5E2)

Tiene un mínimo global en ![$\mathbf{x} = (1, 1, \ldots, 1)$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bx%7D%20%3D%20(1%2C%201%2C%20%5Cldots%2C%201)).
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Función de Rosenbrock.
double rosen(const np::array<double> &x) {
    double f = 0;
    for (int i = 0; i < x.size() - 1; ++i) {
        f += 100*(x[i + 1] - x[i]*x[i])*(x[i + 1] - x[i]*x[i]) +
             (1 - x[i])*(1 - x[i]);
    }
    return f;
}

// Derivada (gradiente) de la función de Rosenbrock.
np::array<double> rosen_der(const np::array<double> &x) {
    np::array<double> jac = np::zeros<double>(x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        jac[i] += -400*x[i]*(x[i + 1] - x[i]*x[i]) - 2*(1 - x[i]);
        jac[i + 1] += 200*(x[i + 1] - x[i]*x[i]);
    }
    return jac;
}

int main() {
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    scicpp::OptimizeResult<double> result;
    result = scicpp::minimize_cg(rosen, x0, rosen_der);
    cout << result;
    return 0;
}
```

```
[Out] fun: 2.3138263e-11
      x: [0.99999896, 0.99999791, 0.99999582, 0.99999159]
      jac: [9.3998262e-07, -2.4901198e-06, 7.1508592e-06, -8.5579434e-06]
      success: true
      status: Optimization terminated successfully.
      niter: 95
      nfev: 1228
      njev: 337
```

### `minimize_ncg`

Minimiza una función utilizando el algoritmo de Newton-CG (también conocido como 
método de Newton truncado). Utiliza un método de gradiente conjugado para 
calcular la dirección de búsqueda.
```cpp
template <class T, class Function, class Jacobian, class Hessian>
OptimizeResult<T> minimize_ncg(
    Function f, const numcpp::array<T> &x0, Jacobian jac, Hessian hess,
    T gtol = 1e-5, double ordnorm = numcpp::inf, size_t maxiter = 1000
);
```

#### Atributos

- `f`: Función objetivo a minimizar.
- `x0`: Una estimación inicial para el valor óptimo de `x`.
- `jac`: Una función que devuelve el vector gradiente de `f` en `x`.
- `hess`: Una función que devuelve la matriz Hessiana de `f` en `x`.
- `gtol`: El algoritmo se detiene cuando la norma del gradiente es menor que 
`gtol`.
- `ordnorm`: Orden a usar para la norma del gradiente.
- `maxiter`: Máximo número de iteraciones a realizar.

#### Ejemplo

Calcula un mínimo local de la función de Rosenbrock.
```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Rosenbrock function.
double rosen(const np::array<double> &x) {
    double f = 0;
    for (int i = 0; i < x.size() - 1; ++i) {
        f += 100*(x[i + 1] - x[i]*x[i])*(x[i + 1] - x[i]*x[i]) +
             (1 - x[i])*(1 - x[i]);
    }
    return f;
}

// Derivative (gradient) of Rosenbrock function.
np::array<double> rosen_der(const np::array<double> &x) {
    np::array<double> jac = np::zeros<double>(x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        jac[i] += -400*x[i]*(x[i + 1] - x[i]*x[i]) - 2*(1 - x[i]);
        jac[i + 1] += 200*(x[i + 1] - x[i]*x[i]);
    }
    return jac;
}

// Hessian of Rosenbrock function.
np::matrix<double> rosen_hess(const np::array<double> &x) {
    np::matrix<double> hess = np::zeros<double>(x.size(), x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        hess[i][i] += -400*x[i + 1] + 1200*x[i]*x[i] + 2;
        hess[i][i + 1] += -400*x[i];
        hess[i + 1][i + 1] += 200;
        hess[i + 1][i] += -400*x[i];
    }
    return hess;
}

int main() {
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    scicpp::OptimizeResult<double> result;
    result = scicpp::minimize_ncg(rosen, x0, rosen_der, rosen_hess);
    cout << result;
    return 0;
}
```

```
[Out] fun: 1.37688e-12
      x: [0.99999975, 0.99999951, 0.999999, 0.99999798]
      jac: [7.7789427e-07, 1.4612809e-06, 7.4658088e-06, -5.4988463e-06]
      hess:
      [[801.99961, -399.9999,         0,         0]
       [-399.9999, 1001.9992, -399.9998,         0]
       [        0, -399.9998, 1001.9984, -399.9996]
       [        0,         0, -399.9996,       200]]
      success: true
      status: Optimization terminated successfully.
      niter: 89
      nfev: 290
      njev: 186
      nhev: 89
```

### `line_search`

Dado un punto inicial ![$\mathbf{x}_k$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bx%7D_k) y una dirección de búsqueda ![$\mathbf{p}_k$](https://render.githubusercontent.com/render/math?math=%5Cmathbf%7Bp%7D_k), 
encuentra una longitud de paso ![$\alpha$](https://render.githubusercontent.com/render/math?math=%5Calpha) que satisfaga las conidiciones de Wolfe:

i) ![$f(\mathbf{x}_k + \alpha\mathbf{p}_k) \leq f(\mathbf{x}_k) + c_1\alpha\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k)$](https://render.githubusercontent.com/render/math?math=f(%5Cmathbf%7Bx%7D_k%20%2B%20%5Calpha%5Cmathbf%7Bp%7D_k)%20%5Cleq%20f(%5Cmathbf%7Bx%7D_k)%20%2B%20c_1%5Calpha%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k))

ii) ![$-\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k + \alpha\mathbf{p}_k) \leq -c_2\mathbf{p}_k^{\top}\nabla f(\mathbf{x}_k)$](https://render.githubusercontent.com/render/math?math=-%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k%20%2B%20%5Calpha%5Cmathbf%7Bp%7D_k)%20%5Cleq%20-c_2%5Cmathbf%7Bp%7D_k%5E%7B%5Ctop%7D%5Cnabla%20f(%5Cmathbf%7Bx%7D_k))

 ```cpp
template <class T, class Function, class Jacobian>
RootResults<T> line_search(
    Function f, Jacobian jac,
    const numcpp::array<T> &xk, const numcpp::array<T> &pk,
    const numcpp::array<T> &gfk, T fk,
    T c1 = 0.0001, T c2 = 0.9, T amax = 1.0, size_t maxiter = 20
);
 ```

#### Argumentos

- `f`: Función objetivo.
- `jac`: Gradiente de la función objetivo.
- `xk`: Punto inicial.
- `pk`: Dirección de búsqueda.
- `gfk`: Valor del gradiente para *x = xk*.
- `fk`: Valor de la función para *x = xk*.
- `c1`: Parámetro para la condición de Armijo.
- `c2`: Parámetro para la condición de curvatura.
- `amax`: Tamaño de paso máximo.
- `maxiter`: Máximo número de iteraciones.

#### Ejemplo

 ```cpp
#include <iostream>
#include "numcpp.h"
#include "scicpp/optimize.h"
using namespace std;
namespace np = numcpp;

// Función de Rosenbrock.
double rosen(const np::array<double> &x) {
    double f = 0;
    for (int i = 0; i < x.size() - 1; ++i) {
        f += 100*(x[i + 1] - x[i]*x[i])*(x[i + 1] - x[i]*x[i]) +
             (1 - x[i])*(1 - x[i]);
    }
    return f;
}

// Derivada (gradiente) de la función de Rosenbrock.
np::array<double> rosen_der(const np::array<double> &x) {
    np::array<double> jac = np::zeros<double>(x.size());
    for (int i = 0; i < x.size() - 1; ++i) {
        jac[i] += -400*x[i]*(x[i + 1] - x[i]*x[i]) - 2*(1 - x[i]);
        jac[i + 1] += 200*(x[i + 1] - x[i]*x[i]);
    }
    return jac;
}

int main() {
    // Nos movemos en la dirección opuesta al gradiente.
    np::array<double> x0 = {-1.2, 1., -1.2, 1.};
    double f0 = rosen(x0);
    np::array<double> gf0 = rosen_der(x0);
    np::array<double> p0 = -gf0;

    // Método de búsqueda en línea.
    double c1 = 0.0001, c2 = 0.9;
    scicpp::RootResults<double> result;
    result = scicpp::line_search(rosen, rosen_der, x0, p0, gf0, f0, c1, c2);

    // Verifica las condiciones de Wolfe.
    double alpha = result.root;
    bool cond1 = rosen(x0 + alpha*p0) <= f0 + c1*alpha*p0.dot(gf0);
    bool cond2 = -p0.dot(rosen_der(x0 + alpha*p0)) <= -c2*p0.dot(gf0);
    cout << "alpha: " << alpha << "\n";
    cout << boolalpha << "i) " << cond1 << "\nii) " << cond2 << "\n";

    return 0;
}

 ```

 ```
 [Out] alpha: 0.00195312
       i) true
       ii) true
 ```