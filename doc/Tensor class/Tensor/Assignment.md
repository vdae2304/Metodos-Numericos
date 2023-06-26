# [Tensor](readme.md)

Defined in header [`numcpp/tensor.h`](/include/numcpp/tensor.h)

- [Tensor](#tensor)
  - [Assignment](#assignment)
    - [Copy assignment](#copy-assignment)
    - [Fill assignment](#fill-assignment)
    - [Move assignment](#move-assignment)
    - [Initializer list assignment](#initializer-list-assignment)

## Assignment

### Copy assignment

Assigns to each element the value of the corresponding element in `other`, after resizing the object (if necessary).
```cpp
tensor &operator=(const tensor &other);
template <class U>
tensor &operator=(const tensor<U, Rank> &other);
```

Parameters

* `other` A tensor-like object of the same rank.

Returns

* `*this`

Exceptions

* `std::bad_alloc` If the function needs to allocate storage and fails, it may throw an exception.

Warnings

* When the shapes do not mach, invalidates all iterators, references and views to elements of the tensor. Otherwise, valid iterators, references and views keep their validity.

### Fill assignment

Assigns `val` to every element. The size of the tensor is preserved.
```cpp
tensor &operator=(const T &val);
```

Parameters

* `val` Value assigned to all the elements in the tensor.

Returns

* `*this`

### Move assignment

Acquires the contents of `other`, leaving `other` in an empty state.
```cpp
tensor &operator=(tensor &&other);
```

Parameters

* `other` A tensor of the same type and rank. The ownership is directly transferred from `other`. `other` is left in an empty state.

Returns

* `*this`

Warnings

* Invalidates all iterators, references and views to elements of the tensor.

### Initializer list assignment

Assigns to each element the value of the corresponding element in `il` after resizing the object (if necessary).
```cpp
tensor &operator=(/* Nested initializer_list */ il);
```

Parameters

* `il` An `initializer_list` object.

Returns

* `*this`

Exceptions

* `std::bad_alloc` If the function needs to allocate storage and fails, it may throw an exception.

Warnings

* When the shapes do not mach, invalidates all iterators, references and views to elements of the tensor. Otherwise, valid iterators, references and views keep their validity.