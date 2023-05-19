/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

/** @file include/numcpp/config.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/config.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_CONFIG_H_INCLUDED
#define NUMCPP_CONFIG_H_INCLUDED

#if __cplusplus < 201103L
#error This file requires compiler and library support for the ISO C++ 2011 \
standard. This support must be enabled with the -std=c++11 or -std=gnu++11 \
compiler options.
#else

#include <complex>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace numcpp {
/// Shapes, indices and layouts.

using std::nullptr_t;
using std::ptrdiff_t;
using std::size_t;

/**
 * @brief A shape_t is a class that identifies the size of a tensor along each
 * dimension.
 */
template <size_t Rank> class shape_t;

/**
 * @brief index_t is just an alias of shape_t defined to distinguish between
 * shapes and indices, improving readability.
 */
template <size_t Rank> using index_t = shape_t<Rank>;

/**
 * @brief Layout in which elements are stored or iterated.
 */
enum layout_t {
  /**
   * @brief Row-major order (C/C++ style).
   * In row-major order, the last dimension is contiguous, and the memory offset
   * of each axis is a constant multiple of the following axis.
   * In row-major iteration, the last index is varying the fastest.
   */
  row_major,

  /**
   * @brief Column-major order (Fortran/Matlab style).
   * In column-major order, the first dimension is contiguous, and the memory
   * offset of each axis is a constant multiple of the previous axis.
   * In column-major iteration, the first index is varying the fastest.
   */
  column_major,

  /**
   * @brief Default layout.
   */
  default_layout = row_major
};

/// Slices.

/**
 * @brief A slice is a class that identifies a subset of elements in an array.
 * It holds three values: the starting index, the stop index and the stride.
 */
class slice;

/// Interfaces.

/**
 * @brief Base tensor class.
 */
template <class T, size_t Rank, class Tag> class base_tensor;

/**
 * @brief A tensor_interface is a generic interface for tensor subclasses. It
 * implements an assortment of methods which will be inherited to all the
 * subclasses.
 */
template <class T, size_t Rank, class Tag> class tensor_interface;

/**
 * @brief A complex_interface is an interface with additional methods for
 * complex-valued tensor subclasses. It can be used in conjunction with
 * tensor_interface.
 */
template <class T, size_t Rank, class Tag> class complex_interface;

/// Iterators.

/**
 * @brief An index_sequence is a class that identifies the indices of a tensor.
 */
template <size_t Rank> class index_sequence;

/**
 * @brief A generic random access iterator for base_tensor class.
 */
template <class T, size_t Rank, class Tag> class base_tensor_iterator;

/**
 * @brief A generic random access iterator for const-qualified base_tensor
 * class.
 */
template <class T, size_t Rank, class Tag> class base_tensor_const_iterator;

/**
 * @brief A random access iterator for base_tensor class obtained by fixing some
 * axes and iterating over the remaining axes.
 */
template <class T, size_t Rank, class Tag, size_t N>
class base_tensor_axes_iterator;

/**
 * @brief A random access iterator for const-qualified base_tensor class
 * obtained by fixing some axes and iterating over the remaining axes.
 */
template <class T, size_t Rank, class Tag, size_t N>
class base_tensor_const_axes_iterator;

/// Subclases.

struct scalar_tag;
struct tensor_tag;
struct view_tag;
struct indirect_tag;

/**
 * @brief Tensors are contiguous multidimensional sequence containers: they hold
 * a variable number of elements arranged in multiple axis.
 */
template <class T, size_t Rank> using tensor = base_tensor<T, Rank, tensor_tag>;

/**
 * @brief A tensor_view is just a view of a multidimensional array. It
 * references the elements in the original array. The view itself does not own
 * the data.
 */
template <class T, size_t Rank>
using tensor_view = base_tensor<T, Rank, view_tag>;

/**
 * @brief An indirect_tensor is a view of a subset of elements from a
 * multidimensional array. It references the elements in the original array
 * through an array of indices.
 */
template <class T, size_t Rank>
using indirect_tensor = base_tensor<T, Rank, indirect_tag>;

/**
 * @brief A light-weight object which stores the result of applying an unary
 * function to each element in a tensor object.
 */
template <class Function, class T, class Tag> struct lazy_unary_tag;

/**
 * @brief A light-weight object which stores the result of applying a binary
 * function to each element in two tensor objects.
 */
template <class Function, class T, class TagT, class U, class TagU>
struct lazy_binary_tag;

/**
 * @brief A light-weight object which stores the result of applying a binary
 * function to all pairs of elements from two tensors.
 */
template <class Function, class T, size_t M, class TagT, class U, size_t N,
          class TagU>
struct lazy_outer_tag;

/// Namespace for implementation details.
namespace detail {
#if __cplusplus < 201703L
/**
 * @brief Variadic logical AND.
 */
template <class... Bn> struct conjunction;

template <> struct conjunction<> : std::true_type {};

template <class B1> struct conjunction<B1> : B1 {};

template <class B1, class... Bn>
struct conjunction<B1, Bn...>
    : std::conditional<B1::value, conjunction<Bn...>, B1> {};

/**
 * @brief Variadic logical OR.
 */
template <class... Bn> struct disjunction;

template <> struct disjunction<> : std::false_type {};

template <class B1> struct disjunction<B1> : B1 {};

template <class B1, class... Bn>
    : std::conditional<B1::value, B1, disjunction<Bn...>> {}
#else
using std::conjunction;
using std::disjunction;
#endif

/**
 * @brief Type traits for complex types.
 */
template <class T> struct complex_traits {
  typedef T value_type;
};

template <class T> struct complex_traits<std::complex<T>> {
  typedef T value_type;
};

/**
 * @brief Rank of shape concatenation.
 */
template <class... Shapes> struct concatenation_rank;

template <> struct concatenation_rank<> : std::integral_constant<size_t, 0> {};

template <size_t Rank, class... Shapes>
struct concatenation_rank<shape_t<Rank>, Shapes...>
    : std::integral_constant<size_t,
                             Rank + concatenation_rank<Shapes...>::value> {};

/**
 * @brief Constructs a nested initializer_list of given depth.
 */
template <class T, size_t Depth> struct nested_initializer_list {
  typedef std::initializer_list<
      typename nested_initializer_list<T, Depth - 1>::type>
      type;
};

template <class T> struct nested_initializer_list<T, 0> {
  typedef T type;
};

template <class T, size_t Depth>
using nested_initializer_list_t =
    typename nested_initializer_list<T, Depth>::type;

/**
 * @brief Number of slice arguments in slice indexing.
 */
template <class... Indices> struct slicing_rank;

template <> struct slicing_rank<> : std::integral_constant<size_t, 0> {};

template <class... Indices>
struct slicing_rank<slice, Indices...>
    : std::integral_constant<size_t, 1 + slicing_rank<Indices...>::value> {};

template <class IntegralType, class... Indices>
struct slicing_rank<IntegralType, Indices...>
    : std::integral_constant<size_t, slicing_rank<Indices...>::value> {
  static_assert(std::is_integral<IntegralType>::value,
                "Index must be either an integer or a slice");
};

/**
 * @brief Result type of function call.
 */
#if __cplusplus < 201703L
template <class Function, class... Args>
using result_of_t = typename std::result_of<Function(Args...)>::type;

template <class...> using void_t = void;
#else
template <class Function, class... Args>
using result_of_t = typename std::invoke_result<Function, Args...>::type;

using std::void_t;
#endif // C++17

template <class Signature, typename = void>
struct is_callable_helper : std::false_type {};

template <class F, class... Args>
struct is_callable_helper<F(Args...), void_t<result_of_t<F, Args...>>>
    : std::true_type {};

/**
 * @brief Checks whether a type is callable with given arguments.
 */
template <class F, class... Args>
struct is_callable : is_callable_helper<F(Args...)> {};

/// Type constraint to request N arguments.
template <size_t N, class... Args>
using RequiresNArguments =
    typename std::enable_if<sizeof...(Args) == N, int>::type;

/// Type constraint to request integer arguments.
template <class... T>
using RequiresIntegral =
    typename std::enable_if<conjunction<std::is_integral<T>...>::value,
                            int>::type;

/// Type constraint to request at least one slice argument.
template <class... Indices>
using RequiresSlicing =
    typename std::enable_if<disjunction<std::is_same<Indices, slice>...>::value,
                            int>::type;

/// Type constraint to request input iterator.
template <class Iterator>
using RequiresInputIterator = typename std::enable_if<
    std::is_convertible<
        typename std::iterator_traits<Iterator>::iterator_category,
        std::input_iterator_tag>::value,
    int>::type;

/// Type constraint to request callable type.
template <class F, class... Args>
using RequiresCallable =
    typename std::enable_if<is_callable<F, Args...>::value, int>::type;
} // namespace detail
} // namespace numcpp

#endif // C++11
#endif // NUMCPP_CONFIG_H_INCLUDED
