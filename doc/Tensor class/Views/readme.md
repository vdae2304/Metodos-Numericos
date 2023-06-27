# Tensor view

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor view](#tensor-view)
  - [Template parameters](#template-parameters)
  - [Aliases](#aliases)
  - [Member types](#member-types)
  - [Member constants](#member-constants)
  - [Constructors](#constructors)
  - [Indexing](#indexing)
  - [Iterators](#iterators)
  - [Assignment](#assignment)
  - [Compound assignment](#compound-assignment)
  - [Operators](#operators)
  - [Public methods](#public-methods)
  - [Specializations](#specializations)
    - [`tensor_view<std::complex<T>>`](#tensor_viewstdcomplext)
  - [Deduction guides (since C++17)](#deduction-guides-since-c17)

```cpp
template <class T, size_t Rank> class tensor_view;
```

A `tensor_view` is just a view of a multidimensional array. It references the elements in the original array. The view itself does not own the data and any changes made to the view will affect the original array, and any changes made to the original array will affect the view.

## Template parameters

* `T` Type of the elements contained in the `tensor_view`. This shall be an arithmetic type or a class that behaves like one (such as `std::complex`).
* `Rank` Dimension of the `tensor_view`. It must be a positive integer.

## Aliases

```cpp
template <class T>
using vector_view = tensor_view<T, 1>;

template <class T>
using matrix_view = tensor_view<T, 2>;
```

## Member types

| Member type       | Definition                                                      |
| ----------------- | --------------------------------------------------------------- |
| `value_type`      | `typename std::remove_cv<T>::type`                              |
| `reference`       | `T&`                                                            |
| `const_reference` | `const T&`                                                      |
| `pointer`         | `T*`                                                            |
| `const_pointer`   | `const T*`                                                      |
| `iterator`        | A [random access iterator](../Iterators/readme.md) to `T`       |
| `const_iterator`  | A [random access iterator](../Iterators/readme.md) to `const T` |
| `size_type`       | An unsigned integral type                                       |
| `difference_type` | A signed integral type                                          |
| `shape_type`      | [`shape_t<Rank>`](../Shapes/readme.md)                          |
| `index_type`      | [`index_t<Rank>`](../Shapes/readme.md)                          |

## Member constants

|                                        |
| -------------------------------------- |
| `static constexpr size_t rank = Rank;` |

## [Constructors](Constructors.md)

## [Indexing](Indexing.md)

| Function                                                             | Description                                                                |
| -------------------------------------------------------------------- | -------------------------------------------------------------------------- |
| [`tensor_view::operator()`](Indexing.md#tensor_viewoperator)         | Return a reference to the element at the given position.                   |
| [`tensor_view::operator[]`](Indexing.md#tensor_viewoperator-1)       | Return a reference to the element at the given position.                   |
| [`tensor_view::shape`](Indexing.md#tensor_viewshape)                 | Return the shape of the `tensor_view`.                                     |
| [`tensor_view::size`](Indexing.md#tensor_viewsize)                   | Return the number of elements in the `tensor_view`.                        |
| [`tensor_view::empty`](Indexing.md#tensor_viewempty)                 | Return whether the `tensor_view` is empty.                                 |
| [`tensor_view::data`](Indexing.md#tensor_viewdata)                   | Return a pointer to the memory array used internally by the `tensor_view`. |
| [`tensor_view::strides`](Indexing.md#tensor_viewstrides)             | Return the span that separates the elements in the memory array.           |
| [`tensor_view::layout`](Indexing.md#tensor_viewlayout)               | Return the order in which elements are iterated.                           |
| [`tensor_view::is_contiguous`](Indexing.md#tensor_viewis_contiguous) | Return whether the elements in the `tensor_view` are stored contiguously.  |

## [Iterators](../Tensor/Iterators.md)

| Function              | Description                                                                   |
| --------------------- | ----------------------------------------------------------------------------- |
| `tensor_view::begin`  | Return an iterator pointing to the first element in the tensor.               |
| `tensor_view::end`    | Return an iterator pointing to the past-the-end element in the tensor.        |
| `tensor_view::cbegin` | Return a `const_iterator` pointing to the first element in the tensor.        |
| `tensor_view::cend`   | Return a `const_iterator` pointing to the past-the-end element in the tensor. |

## [Assignment](Assignment.md)

## [Compound assignment](../Tensor/Compound%20assignment.md)

| Function      | Description                                                                               |
| ------------- | ----------------------------------------------------------------------------------------- |
| `operator+=`  | Assigns to `*this` the result of performing the respective operation on all the elements. |
| `operator-=`  |                                                                                           |
| `operator*=`  |                                                                                           |
| `operator/=`  |                                                                                           |
| `operator%=`  |                                                                                           |
| `operator&=`  |                                                                                           |
| `operator\|=` |                                                                                           |
| `operator^=`  |                                                                                           |
| `operator<<=` |                                                                                           |
| `operator>>=` |                                                                                           |

## [Operators](../Tensor/Operators.md)

| Function       | Description          |
| -------------- | -------------------- |
| `operator+`    | Unary operators      |
| `operator-`    |                      |
| `operator~`    |                      |
| `operator!`    |                      |
| `operator+`    | Arithmetic operators |
| `operator-`    |                      |
| `operator*`    |                      |
| `operator/`    |                      |
| `operator%`    |                      |
| `operator&`    | Bitwise operators    |
| `operator\|`   |                      |
| `operator^`    |                      |
| `operator<<`   |                      |
| `operator>>`   |                      |
| `operator&&`   | Logical operators    |
| `operator\|\|` |                      |
| `operator==`   | Relational operators |
| `operator!=`   |                      |
| `operator<`    |                      |
| `operator>`    |                      |
| `operator<=`   |                      |
| `operator>=`   |                      |

## [Public methods](../Tensor/Public%20methods.md)

| Function                 | Description                                                       |
| ------------------------ | ----------------------------------------------------------------- |
| `tensor_view::all`       | Test whether all tensor elements evaluate to true.                |
| `tensor_view::any`       | Test whether any tensor element evaluate to true.                 |
| `tensor_view::apply`     | Assigns to each element the result of applying a function.        |
| `tensor_view::argmax`    | Return the index of the maximum value in the tensor.              |
| `tensor_view::argmin`    | Return the index of the minimum value in the tensor.              |
| `tensor_view::astype`    | Cast each element to a specified type.                            |
| `tensor_view::clamp`     | Clamp the values in the tensor.                                   |
| `tensor_view::diagonal`  | Return a view of the diagonal.                                    |
| `tensor_view::flatten`   | Return a view of the tensor collapsed into one dimension.         |
| `tensor_view::max`       | Return the maximum value contained in the tensor.                 |
| `tensor_view::mean`      | Return the average of the tensor elements.                        |
| `tensor_view::min`       | Return the minimum value contained in the tensor.                 |
| `tensor_view::partition` | Partition the elements in-place.                                  |
| `tensor_view::prod`      | Return the product of the tensor elements.                        |
| `tensor_view::reshape`   | Return a `tensor_view` containing the same data with a new shape. |
| `tensor_view::reverse`   | Reverse the order of the elements in-place.                       |
| `tensor_view::rotate`    | Rotate the elements in-place.                                     |
| `tensor_view::sort`      | Sort the elements in-place.                                       |
| `tensor_view::stddev`    | Return the standard deviation of the tensor elements.             |
| `tensor_view::sum`       | Return the sum of the tensor elements.                            |
| `tensor_view::swapaxes`  | Interchanges two axes of a `tensor_view` in-place.                |
| `tensor_view::t`         | Return a view of the tensor with its axes in reversed order.      |
| `tensor_view::var`       | Return the variance of the tensor elements.                       |

## [Specializations](../Tensor/Specializations.md)

### [`tensor_view<std::complex<T>>`](../Tensor/Specializations.md#specializations-tensorstdcomplext)

| Function            | Description                                     |
| ------------------- | ----------------------------------------------- |
| `tensor_view::real` | Return or set the real part, element-wise.      |
| `tensor_view::imag` | Return or set the imaginary part, element-wise. |
| `tensor_view::conj` | Return the complex conjugate, element-wise.     |

## [Deduction guides (since C++17)](Deduction%20guides.md)
