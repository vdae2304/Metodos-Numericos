# Broadcasting and indexing

Defined in header [`numcpp/broadcasting.h`](/include/numcpp/broadcasting.h)

- [Broadcasting and indexing](#broadcasting-and-indexing)
  - [Tensor creation routines from existing data](#tensor-creation-routines-from-existing-data)
  - [Broadcasting](#broadcasting)
  - [Basic manipulation routines](#basic-manipulation-routines)
  - [Concatenation](#concatenation)
  - [Tiling](#tiling)
  - [Padding](#padding)
  - [Indexing](#indexing)

## [Tensor creation routines from existing data](Tensor%20creation%20routines%20from%20existing%20data.md)

| Function                                                                                          | Description                                                                |
| ------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- |
| [`asarray`](Tensor%20creation%20routines%20from%20existing%20data.md#asarray)                     | Convert the input to a tensor.                                             |
| [`ascontiguousarray`](Tensor%20creation%20routines%20from%20existing%20data.md#ascontiguousarray) | Return a tensor laid out in C/C++ (row-major) order in memory.             |
| [`asfortranarray`](Tensor%20creation%20routines%20from%20existing%20data.md#asfortranarray)       | Return a tensor laid out in Fortran/Matlab (column-major) order in memory. |
| [`asarray_chkfinite`](Tensor%20creation%20routines%20from%20existing%20data.md#asarray_chkfinite) | Convert the input to a tensor, checking for NaNs or Infs.                  |

## [Broadcasting](Broadcasting.md)

| Function                                       | Description                                         |
| ---------------------------------------------- | --------------------------------------------------- |
| [`broadcast_to`](Broadcasting.md#broadcast_to) | Broadcast a tensor to a new shape.                  |
| [`expand_dims`](Broadcasting.md#expand_dims)   | Expand the shape of a tensor by inserting new axes. |
| [`squeeze`](Broadcasting.md#squeeze)           | Removes axes of length one.                         |


## [Basic manipulation routines](Basic%20manipulation%20routines.md)

| Function                                                | Description                                                                   |
| ------------------------------------------------------- | ----------------------------------------------------------------------------- |
| [`astype`](Basic%20manipulation%20routines.md#astype)   | Cast each element in a tensor to a specified type.                            |
| [`copyto`](Basic%20manipulation%20routines.md#copyto)   | Copies values from one tensor to another, broadcasting as necessary.          |
| [`copy`](Basic%20manipulation%20routines.md#copy)       | Return a copy of the tensor.                                                  |
| [`flatten`](Basic%20manipulation%20routines.md#flatten) | Return a copy of the tensor flattened into one dimension.                     |
| [`zip`](Basic%20manipulation%20routines.md#zip)         | Return a tensor of tuples constructed from taking elements from all tensors.  |
| [`unzip`](Basic%20manipulation%20routines.md#unzip)     | Return a tensor with the I-th element of the corresponding tuple in a tensor. |

## [Concatenation](Concatenation.md)

| Function                                      | Description                                       |
| --------------------------------------------- | ------------------------------------------------- |
| [`concatenate`](Concatenation.md#concatenate) | Concatenate one or more tensors.                  |
| [`stack`](Concatenation.md#stack)             | Concatenate one or more tensors along a new axis. |

## [Tiling](Tiling.md)

| Function                     | Description                                                              |
| ---------------------------- | ------------------------------------------------------------------------ |
| [`tile`](Tiling.md#tile)     | Construct a tensor by repeating `a` the number of times given by `reps`. |
| [`repeat`](Tiling.md#repeat) | Repeat elements of a tensor.                                             |

## [Padding](Padding.md)

| Function                                                  | Description                                                                   |
| --------------------------------------------------------- | ----------------------------------------------------------------------------- |
| [`pad`](Padding.md#pad)                                   | Pad a tensor.                                                                 |
| [`pad_mode::constant`](Padding.md#pad_modeconstant)       | Pads with a constant value.                                                   |
| [`pad_mode::edge`](Padding.md#pad_modeedge)               | Pads with the edge values of the tensor.                                      |
| [`pad_mode::linear_ramp`](Padding.md#pad_modelinear_ramp) | Pads with the linear ramp between an end value and the tensor edge value.     |
| [`pad_mode::reflect`](Padding.md#pad_modereflect)         | Pads with the reflection of the tensor mirrored on the first and last values. |
| [`pad_mode::symetric`](Padding.md#pad_modesymetric)       | Pads with the reflection of the tensor mirrored along the edge.               |
| [`pad_mode::wrap`](Padding.md#pad_modewrap)               | Pads with the wrap of the tensor along the axis.                              |

## [Indexing](Indexing.md)

| Function                                         | Description                                                                  |
| ------------------------------------------------ | ---------------------------------------------------------------------------- |
| [`ravel_index`](Indexing.md#ravel_index)         | Converts a tuple of indices into a flat index, element-wise.                 |
| [`unravel_index`](Indexing.md#unravel_index)     | Converts a flat index into a tuple of indices, element-wise.                 |
| [`take`](Indexing.md#take)                       | Take elements from a tensor.                                                 |
| [`take_along_axis`](Indexing.md#take_along_axis) | Take values from the input tensor by matching 1d index and data slices.      |
| [`put`](Indexing.md#put)                         | Replaces specified elements of a tensor with given values.                   |
| [`put_along_axis`](Indexing.md#put_along_axis)   | Put values into the destination tensor by matching 1d index and data slices. |
| [`compress`](Indexing.md#compress)               | Return the elements of a tensor that satisfy some condition.                 |
| [`place`](Indexing.md#place)                     | Change elements of a tensor based on conditional and input values.           |
| [`putmask`](Indexing.md#putmask)                 | Change elements of a tensor based on conditional and input values.           |
