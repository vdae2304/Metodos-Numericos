# [Shapes](./readme.md)

Defined in header [`numcpp/shape.h`](/include/numcpp/shape.h)

- [Shapes](#shapes)
  - [Operators](#operators)
    - [`shape_t::operator[]`](#shape_toperator)
    - [`shape_t::operator IntegralType`](#shape_toperator-integraltype)
    - [`operator==`](#operator)
    - [`operator!=`](#operator-1)
    - [`operator>>`](#operator-2)
    - [`operator<<`](#operator-3)

## Operators

### `shape_t::operator[]`

Return the size of the i-th axis.
```cpp
size_type &operator[](size_type i);
const size_type &operator[](size_type i) const;
```

Parameters

* `i` Axis index.

Returns

* The size along the i-th axis. If the `shape_t` is const-qualified, the function returns a reference to `const size_t`. Otherwise, it returns a reference to `size_t`.

Example

```cpp
#include <iostream>
#include <numcpp/shape.h>
namespace np = numcpp;
int main() {
    np::shape_t<1> shape1(10);
    std::cout << "Shape: " << shape1 << "\n";
    std::cout << "Length: " << shape1[0] << "\n\n";

    np::shape_t<2> shape2(4, 6);
    std::cout << "Shape: " << shape2 << "\n";
    std::cout << "Rows: " << shape2[0] << "\n";
    std::cout << "Columns: " << shape2[1] << "\n\n";

    np::shape_t<3> shape3(3, 4, 6);
    std::cout << "Shape: " << shape3 << "\n";
    std::cout << "Depth: " << shape3[0] << "\n";
    std::cout << "Rows: " << shape3[1] << "\n";
    std::cout << "Columns: " << shape3[2] << "\n";
    return 0;
}
```

Output

```
Shape: (10,)
Length: 10

Shape: (4, 6)
Rows: 4
Columns: 6

Shape: (3, 4, 6)
Depth: 3
Rows: 4
Columns: 6
```

### `shape_t::operator IntegralType`

Integer conversion. Dimension must be one.

```cpp
template <class IntegralType>
explicit operator IntegralType() const;
```

### `operator==`

Compares if two shapes are equal. Returns true if they have the same dimension and the same size along each axis.
```cpp
template <size_t Rank1, size_t Rank2>
bool operator==(const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2);
```

Example

```cpp
#include <iostream>
#include <numcpp/shape.h>
namespace np = numcpp;
int main() {
    np::shape_t<3> shape1, shape2;
    std::cin >> shape1 >> shape2;
    if (shape1 == shape2) {
        std::cout << "They are equal\n";
    } else {
        std::cout << "They are not equal\n";
    }
    return 0;
}
```

Input

```
(3, 4, 6)
(3, 4, 6)
```

Output

```
They are equal
```

Input

```
(1, 4, 1)
(3, 4, 6)
```

Output

```
They are not equal
```

### `operator!=`

Compares if two shapes are not equal. Returns true if they have different dimensions or if they have different sizes along an axis.
```cpp
template <size_t Rank1, size_t Rank2>
bool operator!=(const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2);
```

### `operator>>`

Overloads input stream for `shape_t` objects.
```cpp
template <class charT, class traits, size_t Rank>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape);

template <class charT, class traits>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, shape_t<1> &shape);
```

For 1-dimensional shapes, the supported formats are

- `(size,)`
- `size`

For $n$-dimensional shapes, $n > 1$, the supported formats are

- `(size_1, size_2, ..., size_n)`

If an error occurs, calls `istr.setstate(std::ios_base::failbit)`.

Parameters

* `istr` Input stream object.
* `shape` Shape to be extracted from the input stream.

Returns

* `istr`

### `operator<<`

Overloads output stream for `shape_t` objects.
```cpp
template <class charT, class traits, size_t Rank>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr, const shape_t<Rank> &shape);

template <class charT, class traits>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr, const shape_t<1> &shape);
```

For 1-dimensional shapes, the format used is

- `(size,)`

For $n$-dimensional shapes, $n > 1$, the format used is

- `(size_1, size_2, ..., size_n)`

Parameters

* `ostr` Output stream object.
* `shape` Shape to be inserted into the output stream.

Returns

* `ostr`

