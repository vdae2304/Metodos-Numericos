# Input and Output

Defined in header [`numcpp/io.h`](/include/numcpp/io.h)

- [Input and Output](#input-and-output)
  - [Printoptions](#printoptions)
  - [Binary data](#binary-data)
  - [Text files](#text-files)
  - [Input/output streams](#inputoutput-streams)

## [Printoptions](Printoptions.md)

## [Binary data](Binary%20data.md)

| Function                        | Description                                                     |
| ------------------------------- | --------------------------------------------------------------- |
| [`load`](Binary%20data.md#load) | Load tensor contents from a binary file in NumPy `.npy` format. |
| [`save`](Binary%20data.md#save) | Save tensor contents to a binary file in NumPy `.npy` format.   |

## [Text files](Text%20files.md)

| Function                             | Description                        |
| ------------------------------------ | ---------------------------------- |
| [`loadtxt`](Text%20files.md#loadtxt) | Load tensor data from a text file. |
| [`savetxt`](Text%20files.md#savetxt) | Save a tensor to a text file.      |

## [Input/output streams](Input%20output%20streams.md)

| Function                                               | Description                              |
| ------------------------------------------------------ | ---------------------------------------- |
| [`operator>>`](Input%20output%20streams.md#operator)   | Extracts a tensor from an input stream.  |
| [`operator<<`](Input%20output%20streams.md#operator-1) | Inserts a tensor into the output stream. |
| [`to_string`](Input%20output%20streams.md#to_string) | Return a string representation of a tensor. |
