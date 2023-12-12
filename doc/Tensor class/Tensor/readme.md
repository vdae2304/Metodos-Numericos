# Tensor

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Template parameters](#template-parameters)
  - [Aliases](#aliases)
  - [Member types](#member-types)
  - [Member constants](#member-constants)
  - [Constructors](#constructors)
  - [Indexing](#indexing)
  - [Advanced indexing](#advanced-indexing)
  - [Iterators](#iterators)
  - [Assignment](#assignment)
  - [Compound assignment](#compound-assignment)
  - [Operators](#operators)
  - [Public methods](#public-methods)
  - [Specializations](#specializations)
    - [`tensor<std::complex<T>>`](#tensorstdcomplext)
  - [Deduction guides (since C++17)](#deduction-guides-since-c17)

```cpp
template <class T, size_t Rank> class tensor;
```

Tensors are contiguous multidimensional sequence containers: they hold a variable number of elements arranged in multiple axis. Unlike [`tensor_view`](../Views/readme.md), `tensor` is always owner of its own data, which means that the storage of the tensor is handled automatically.

Tensors are designed to easily perform mathematical operations on them. Most mathematical operations can be applied directly to tensor objects, including arithmetic and comparison operators, affecting all its elements. It also supports various forms of generalized subscript operators, slicing and indirect access.

## Template parameters

* `T` Type of the elements contained in the tensor. This shall be an arithmetic type or a class that behaves like one (such as `std::complex`).
 * `Rank` Dimension of the tensor. It must be a positive integer.

## Aliases

```cpp
template <class T>
using vector = tensor<T, 1>;

template <class T>
using matrix = tensor<T, 2>;
```

## Member types

| Member type       | Definition                                                      |
| ----------------- | --------------------------------------------------------------- |
| `value_type`      | The template parameter (`T`)                                    |
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

| Function                                                   | Description                                                         |
| ---------------------------------------------------------- | ------------------------------------------------------------------- |
| [`tensor::operator()`](Indexing.md#tensoroperator)         | Return a reference to the element at the given position.            |
| [`tensor::operator[]`](Indexing.md#tensoroperator-1)       | Return a reference to the element at the given position.            |
| [`tensor::shape`](Indexing.md#tensorshape)                 | Return the shape of the tensor.                                     |
| [`tensor::size`](Indexing.md#tensorsize)                   | Return the number of elements in the tensor.                        |
| [`tensor::empty`](Indexing.md#tensorempty)                 | Return whether the tensor is empty.                                 |
| [`tensor::data`](Indexing.md#tensordata)                   | Return a pointer to the memory array used internally by the tensor. |
| [`tensor::layout`](Indexing.md#tensorlayout)               | Return the memory layout in which elements are stored.              |
| [`tensor::is_contiguous`](Indexing.md#tensoris_contiguous) | Return whether the elements in the tensor are stored contiguously.  |

## [Advanced indexing](Advanced%20indexing.md)

## [Iterators](Iterators.md)

| Function                                      | Description                                                                   |
| --------------------------------------------- | ----------------------------------------------------------------------------- |
| [`tensor::begin`](Iterators.md#tensorbegin)   | Return an iterator pointing to the first element in the tensor.               |
| [`tensor::end`](Iterators.md#tensorend)       | Return an iterator pointing to the past-the-end element in the tensor.        |
| [`tensor::cbegin`](Iterators.md#tensorcbegin) | Return a `const_iterator` pointing to the first element in the tensor.        |
| [`tensor::cend`](Iterators.md#tensorcend)     | Return a `const_iterator` pointing to the past-the-end element in the tensor. |

## [Assignment](Assignment.md)

## [Compound assignment](Compound%20assignment.md)

| Function                                                      | Description                                                                               |
| ------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| [`operator+=`](Compound%20assignment.md#compound-assignment)  | Assigns to `*this` the result of performing the respective operation on all the elements. |
| [`operator-=`](Compound%20assignment.md#compound-assignment)  |                                                                                           |
| [`operator*=`](Compound%20assignment.md#compound-assignment)  |                                                                                           |
| [`operator/=`](Compound%20assignment.md#compound-assignment)  |                                                                                           |
| [`operator%=`](Compound%20assignment.md#compound-assignment)  |                                                                                           |
| [`operator&=`](Compound%20assignment.md#compound-assignment)  |                                                                                           |
| [`operator\|=`](Compound%20assignment.md#compound-assignment) |                                                                                           |
| [`operator^=`](Compound%20assignment.md#compound-assignment)  |                                                                                           |
| [`operator<<=`](Compound%20assignment.md#compound-assignment) |                                                                                           |
| [`operator>>=`](Compound%20assignment.md#compound-assignment) |                                                                                           |

## [Operators](Operators.md)

| Function                                          | Description          |
| ------------------------------------------------- | -------------------- |
| [`operator+`](Operators.md#unary-operators)       | Unary operators      |
| [`operator-`](Operators.md#unary-operators)       |                      |
| [`operator~`](Operators.md#unary-operators)       |                      |
| [`operator!`](Operators.md#unary-operators)       |                      |
| [`operator+`](Operators.md#arithmetic-operators)  | Arithmetic operators |
| [`operator-`](Operators.md#arithmetic-operators)  |                      |
| [`operator*`](Operators.md#arithmetic-operators)  |                      |
| [`operator/`](Operators.md#arithmetic-operators)  |                      |
| [`operator%`](Operators.md#arithmetic-operators)  |                      |
| [`operator&`](Operators.md#bitwise-operators)     | Bitwise operators    |
| [`operator\|`](Operators.md#bitwise-operators)    |                      |
| [`operator^`](Operators.md#bitwise-operators)     |                      |
| [`operator<<`](Operators.md#bitwise-operators)    |                      |
| [`operator>>`](Operators.md#bitwise-operators)    |                      |
| [`operator&&`](Operators.md#logical-operators)    | Logical operators    |
| [`operator\|\|`](Operators.md#logical-operators)  |                      |
| [`operator==`](Operators.md#relational-operators) | Relational operators |
| [`operator!=`](Operators.md#relational-operators) |                      |
| [`operator<`](Operators.md#relational-operators)  |                      |
| [`operator>`](Operators.md#relational-operators)  |                      |
| [`operator<=`](Operators.md#relational-operators) |                      |
| [`operator>=`](Operators.md#relational-operators) |                      |

## [Public methods](Public%20methods.md)

| Function                                                   | Description                                                       |
| ---------------------------------------------------------- | ----------------------------------------------------------------- |
| [`tensor::all`](Public%20methods.md#tensorall)             | Test whether all tensor elements evaluate to true.                |
| [`tensor::any`](Public%20methods.md#tensorany)             | Test whether any tensor element evaluate to true.                 |
| [`tensor::apply`](Public%20methods.md#tensorapply)         | Assigns to each element the result of applying a function.        |
| [`tensor::argmax`](Public%20methods.md#tensorargmax)       | Return the index of the maximum value in the tensor.              |
| [`tensor::argmin`](Public%20methods.md#tensorargmin)       | Return the index of the minimum value in the tensor.              |
| [`tensor::astype`](Public%20methods.md#tensorastype)       | Cast each element to a specified type.                            |
| [`tensor::clamp`](Public%20methods.md#tensorclamp)         | Clamp the values in the tensor.                                   |
| [`tensor::diagonal`](Public%20methods.md#tensordiagonal)   | Return a view of the diagonal.                                    |
| [`tensor::flatten`](Public%20methods.md#tensorflatten)     | Return a view of the tensor collapsed into one dimension.         |
| [`tensor::max`](Public%20methods.md#tensormax)             | Return the maximum value contained in the tensor.                 |
| [`tensor::mean`](Public%20methods.md#tensormean)           | Return the average of the tensor elements.                        |
| [`tensor::min`](Public%20methods.md#tensormin)             | Return the minimum value contained in the tensor.                 |
| [`tensor::partition`](Public%20methods.md#tensorpartition) | Partition the elements in-place.                                  |
| [`tensor::prod`](Public%20methods.md#tensorprod)           | Return the product of the tensor elements.                        |
| [`tensor::reshape`](Public%20methods.md#tensorreshape)     | Return a `tensor_view` containing the same data with a new shape. |
| [`tensor::resize`](Public%20methods.md#tensorresize)       | Resizes the tensor in-place to a given shape                      |
| [`tensor::reverse`](Public%20methods.md#tensorreverse)     | Reverse the order of the elements in-place.                       |
| [`tensor::rotate`](Public%20methods.md#tensorrotate)       | Rotate the elements in-place.                                     |
| [`tensor::sort`](Public%20methods.md#tensorsort)           | Sort the elements in-place.                                       |
| [`tensor::stddev`](Public%20methods.md#tensorstddev)       | Return the standard deviation of the tensor elements.             |
| [`tensor::sum`](Public%20methods.md#tensorssum)            | Return the sum of the tensor elements.                            |
| [`tensor::t`](Public%20methods.md#tensort)                 | Return a view of the tensor with its axes in reversed order.      |
| [`tensor::var`](Public%20methods.md#tensorvar)             | Return the variance of the tensor elements.                       |
| [`tensor::view`](Public%20methods.md#tensorview)           | Return a view of the tensor with the same data.                   |

## [Specializations](Specializations.md)

### [`tensor<std::complex<T>>`](Specializations.md#specializations-tensorstdcomplext)

| Function                                        | Description                                     |
| ----------------------------------------------- | ----------------------------------------------- |
| [`tensor::real`](Specializations.md#tensorreal) | Return or set the real part, element-wise.      |
| [`tensor::imag`](Specializations.md#tensorimag) | Return or set the imaginary part, element-wise. |
| [`tensor::conj`](Specializations.md#tensorconj) | Return the complex conjugate, element-wise.     |

## [Deduction guides (since C++17)](Deduction%20guides.md)
