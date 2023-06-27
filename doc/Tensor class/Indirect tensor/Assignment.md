# [Indirect tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Indirect tensor](#indirect-tensor)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
    - [Fill assignment](#fill-assignment)
    - [Move assignment](#move-assignment)

## Assignment

### Copy assignment

Assigns to each element the value of the corresponding element in `other`.
```cpp
indirect_tensor &operator=(const indirect_tensor &other);
template <class U>
indirect_tensor &operator=(const tensor<U, Rank> &other);
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
indirect_tensor &operator=(const T &val);
```

Parameters

* `val` Value assigned to all the elements in the `indirect_tensor`.

Returns

* `*this`

### Move assignment

Acquires the contents of `other`, leaving `other` in an empty state.
```cpp
indirect_tensor &operator=(indirect_tensor &&other);
```

Parameters

* `other` An `indirect_tensor` of the same type and rank. The ownership of the array of indices is directly transferred from `other`. `other` is left in an empty state.

Returns

* `*this`
