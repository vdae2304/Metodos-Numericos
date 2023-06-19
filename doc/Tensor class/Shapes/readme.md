# Shapes

Defined in header [`numcpp/shape.h`](/include/numcpp/shape.h)

- [Shapes](#shapes)
  - [Template parameters](#template-parameters)
  - [Member types](#member-types)
  - [Member constants](#member-constants)
  - [Constructors](#constructors)
  - [Assignment](#assignment)
  - [Public methods](#public-methods)
  - [Operators](#operators)
  - [Non-member functions](#non-member-functions)
  - [Deduction guides (since C++17)](#deduction-guides-since-c17)

A `shape_t` is a class that identifies the size of a tensor along each dimension.
```cpp
template <size_t Rank> class shape_t;
```

## Template parameters

* `Rank` Dimension of the shape. It must be a positive integer.

## Member types

| Member type | Definition |
| ----------- | ---------- |
| `size_type` | `size_t`   |

## Member constants

|                                        |
| -------------------------------------- |
| `static constexpr size_t rank = Rank;` |

## [Constructors](Constructors.md)

## [Assignment](Assignment.md)

## [Public methods](Public%20methods.md)

| Function                                           | Description                                                                   |
| -------------------------------------------------- | ----------------------------------------------------------------------------- |
| [`shape_t::prod`](Public%20methods.md#shape_tprod) | Return the product of the sizes along all the axes.                           |
| [`shape_t::data`](Public%20methods.md#shape_tdata) | Return a pointer to the block of memory containing the elements of the shape. |

## [Operators](Operators.md)

| Function                                                                      | Description                                    |
| ----------------------------------------------------------------------------- | ---------------------------------------------- |
| [`shape_t::operator[]`](Operators.md#shape_toperator)                         | Return the size of the i-th axis.              |
| [`shape_t::operator IntegralType`](Operators.md#shape_toperator-integraltype) | Integer conversion. Dimension must be one.     |
| [`operator==`](Operators.md#operator)                                         | Compares if two shapes are equal.              |
| [`operator!=`](Operators.md#operator-1)                                       | Compares if two shapes are not equal.          |
| [`operator>>`](Operators.md#operator-2)                                       | Overloads input stream for `shape_t` objects.  |
| [`operator<<`](Operators.md#operator-3)                                       | Overloads output stream for `shape_t` objects. |

## [Non-member functions](Non-member%20functions.md)

| Function                                                         | Description                                                                                |
| ---------------------------------------------------------------- | ------------------------------------------------------------------------------------------ |
| [`make_shape`](Non-member%20functions.md#make_shape)             | Create a `shape_t` object deducing its dimension from the number of arguments.             |
| [`make_index`](Non-member%20functions.md#make_index)             | Create an `index_t` object deducing its dimension from the number of arguments.            |
| [`make_strides`](Non-member%20functions.md#make_strides)         | Return a tuple of strides to offset a contiguous memory array as a multidimensional array. |
| [`ravel_index`](Non-member%20functions.md#ravel_index)           | Converts a tuple of indices into a flat index.                                             |
| [`unravel_index`](Non-member%20functions.md#unravel_index)       | Converts a flat index into a tuple of indices.                                             |
| [`broadcast_shapes`](Non-member%20functions.md#broadcast_shapes) | Broadcast input shapes into a common shape.                                                |
| [`shape_cat`](Non-member%20functions.md#shape_cat)               | Constructs a shape that is the concatenation of one or more shapes.                        |

## [Deduction guides (since C++17)](Deduction%20guides.md)
