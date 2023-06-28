# Slice

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Slice](#slice)
  - [Constructors](#constructors)
  - [Operators](#operators)
  - [Public methods](#public-methods)

```cpp
class slice;
```

A slice is a class that identifies a subset of elements in an array. It holds three values: the starting index, the stop index and the stride.

## [Constructors](Constructors.md)

## Operators

| Function            | Description                                    |
| ------------------- | ---------------------------------------------- |
| `slice::operator[]` | Return the element at position i in the slice. |
| `operator==`        | Compares if two slices are equal.              |
| `operator!=`        | Compares if two slices are different.          |

## Public methods

| Function        | Description                                         |
| --------------- | --------------------------------------------------- |
| `slice::start`  | Return the first element in the slice.              |
| `slice::last`   | Return the last element in the slice.               |
| `slice::size`   | Return the number of elements in the slice.         |
| `slice::stride` | Return the separation of the elements in the slice. |
