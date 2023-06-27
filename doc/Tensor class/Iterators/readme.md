# Iterators

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Iterators](#iterators)
  - [Member types](#member-types)
  - [Layouts](#layouts)
    - [`row_major`](#row_major)
    - [`column_major`](#column_major)
  - [Operators](#operators)
  - [Operators (non-member functions)](#operators-non-member-functions)
  - [Public methods](#public-methods)

A generic random access iterator for tensor subclasses.

## Member types

| Member type         | Definition                            |
| ------------------- | ------------------------------------- |
| `container_type`    | The type of the underlying container  |
| `difference_type`   | An unsigned integral type             |
| `value_type`        | `typename container_type::value_type` |
| `pointer`           | Implementation defined                |
| `reference`         | Implementation defined                |
| `iterator_category` | `std::random_access_iterator_tag`     |

## Layouts

Layout in which elements are stored or iterated.
```cpp
enum layout_t { row_major, column_major, default_layout = row_major};
```

### `row_major`

Row-major order (C/C++ style).

In row-major order, the last dimension is contiguous, and the memory offset of each axis is a constant multiple of the following axis.

In row-major iteration, the last index is varying the fastest.

### `column_major`

Column-major order (Fortran/Matlab style).

In column-major order, the first dimension is contiguous, and the memory offset of each axis is a constant multiple of the previous axis.

In column-major iteration, the first index is varying the fastest.

## Operators

| Function          | Description                                                   |
| ----------------- | ------------------------------------------------------------- |
| `operator++`      | Pre-increments the iterator by one.                           |
| `operator--`      | Pre-decrements the iterator by one.                           |
| `operator++(int)` | Post-increments the iterator by one.                          |
| `operator--(int)` | Post-decrements the iterator by one.                          |
| `operator+=`      | Advances the iterator by `n`.                                 |
| `operator-=`      | Advances the iterator by `-n`.                                |
| `operator*`       | Return a reference to the current element.                    |
| `operator->`      | Return a pointer to the current element.                      |
| `operator[]`      | Return a reference to the element located `n` positions away. |


## Operators (non-member functions)

| Function     | Description          |
| ------------ | -------------------- |
| `operator+`  | Arithmetic operators |
| `operator-`  |                      |
| `operator==` | Relational operators |
| `operator!=` |                      |
| `operator<`  |                      |
| `operator>`  |                      |
| `operator<=` |                      |
| `operator>=` |                      |

## [Public methods](Public%20methods.md)

| Function                                                  | Description                                               |
| --------------------------------------------------------- | --------------------------------------------------------- |
| [`iterator::base`](Public%20methods.md#iteratorbase)      | Accesses the underlying tensor.                           |
| [`iterator::index`](Public%20methods.md#iteratorindex)    | Returns the current flat index.                           |
| [`iterator::coords` ](Public%20methods.md#iteratorcoords) | Returns an `index_t` object with the current coordinates. |
| [`iterator::layout`](Public%20methods.md#iteratorlayout)  | Returns the order in which elements are iterated.         |
