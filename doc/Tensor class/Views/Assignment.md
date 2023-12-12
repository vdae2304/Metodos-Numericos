# [Tensor view](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor view](#tensor-view)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
    - [Fill assignment](#fill-assignment)
    - [Move assignment](#move-assignment)

## Assignment

### Copy assignment

Assigns to each element the value of the corresponding element in `other`.
```cpp
tensor_view &operator=(const tensor_view &other);
template <class U>
tensor_view &operator=(const tensor<U, Rank> &other);
```

Parameters

* `other` A tensor-like object of the same rank.

Returns

* `*this`

Exceptions

* `std::invalid_argument` Thrown if the shapes are not compatible and cannot be broadcasted according to [broadcasting rules](../Shapes/Non-member%20functions.md#broadcast_shapes).

### Fill assignment

Assigns `val` to every element.
```cpp
tensor_view &operator=(const T &val);
```

Parameters

* `val` Value assigned to all the elements in the `tensor_view`.

Returns

* `*this`

### Move assignment

Acquires the contents of `other`, leaving `other` in an empty state.
```cpp
tensor_view &operator=(tensor_view &&other);
```

Parameters

* `other` A `tensor_view` of the same type and rank. `other` is left in an empty state.

Returns

* `*this`
