# [Mathematical functions](readme.md)

Defined in `numcpp/math.h`

## Constants

NumCPP defines the following mathematical constants.

Floating point representation of (positive) infinity.
```cpp
template <class T>
static constexpr T constants<T>::inf;
```

Floating point representation of quiet NaN (Not a Number).
```cpp
template <class T>
static constexpr T constants<T>::nan;
```

Euler's constant, base of natural logarithm.
```cpp
template <class T>
static constexpr T constants<T>::e;
```

Pi, ratio of a circle's circunferemce to its diameter.
```cpp
template <class T>
static constexpr T constants<T>::pi;
```

Golden ratio.
```cpp
template <class T>
static constexpr T constants<T>::golden;
```

Euler–Mascheroni's constant.
```cpp
template <class T>
static constexpr T constants<T>::egamma;
```

Example
```cpp
#include <iostream>
#include <numcpp/math.h>
namespace np = numcpp;
int main() {
    std::cout << "Infinity: " << np::constants<double>::inf << "\n";
    std::cout << "NaN: " << np::constants<double>::nan << "\n";
    std::cout << "e: " << np::constants<double>::e << "\n";
    std::cout << "Pi: " << np::constants<double>::pi << "\n";
    std::cout << "Golden ratio: " << np::constants<double>::golden << "\n";
    std::cout << "Euler-Mascheroni: " << np::constants<double>::egamma << "\n";
    return 0;
}
```

Output

```
Infinity: inf
NaN: nan
e: 2.71828
Pi: 3.14159
Golden ratio: 1.61803
Euler-Mascheroni: 0.577216
```
