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

/// Base class.

/**
 * @brief Base class for all expressions and tensors. All subclasses inherits
 * from this class.
 *
 * @tparam Container Tensor subclass.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Container, class T, size_t Rank> class expression {
public:
  /**
   * @brief Return the element at the given position.
   */
  T operator[](const index_t<Rank> &index) const {
    return static_cast<const Container &>(*this)[index];
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  shape_t<Rank> shape() const {
    return static_cast<const Container &>(*this).shape();
  }

  size_t shape(size_t axis) const {
    return static_cast<const Container &>(*this).shape(axis);
  }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_t size() const { return static_cast<const Container &>(*this).size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const {
    return static_cast<const Container &>(*this).layout();
  }

  /**
   * @brief Return the derived subclass.
   */
  Container &self() { return static_cast<Container &>(*this); }

  const Container &self() const {
    return static_cast<const Container &>(*this);
  }
};

/**
 * @brief Base class for dense tensors such as tensor, tensor_view and
 * indirect_tensor.
 */
template <class Container, class T, size_t Rank> class dense_tensor;

/// Iterators.

/**
 * @brief An index_sequence is a class that identifies the indices of a tensor.
 */
template <size_t Rank> class index_sequence;

/**
 * @brief A generic random access iterator for tensor subclasses.
 */
template <class Container, class T, size_t Rank, class Pointer, class Reference>
class flat_iterator;

/// Subclases.

/**
 * @brief Tensors are contiguous multidimensional sequence containers: they hold
 * a variable number of elements arranged in multiple axis.
 */
template <class T, size_t Rank> class tensor;

/**
 * @brief A tensor_view is just a view of a multidimensional array. It
 * references the elements in the original array. The view itself does not own
 * the data.
 */
template <class T, size_t Rank> class tensor_view;

/**
 * @brief An indirect_tensor is a view of a subset of elements from a
 * multidimensional array. It references the elements in the original array
 * through an array of indices.
 */
template <class T, size_t Rank> class indirect_tensor;

/**
 * @brief A light-weight object which stores the result of applying an unary
 * function to each element in a tensor object.
 */
template <class Function, class Container, class T, size_t Rank>
class unary_expr;

/**
 * @brief A light-weight object which stores the result of applying a binary
 * function to each element in two tensor objects.
 */
template <class Function, class Container1, class T, class Container2, class U,
          size_t Rank>
class binary_expr;

/**
 * @brief Placeholder for reduction operations.
 */
struct keepdims_t {
} keepdims;
struct dropdims_t {
} dropdims;

/// Namespace for implementation details.
namespace detail {
#if __cplusplus >= 201703L
using std::conjunction;
using std::disjunction;
using std::void_t;
#else
/**
 * @brief Variadic logical AND.
 */
template <class... Bn> struct conjunction;

template <> struct conjunction<> : std::true_type {};

template <class B1> struct conjunction<B1> : B1 {};

template <class B1, class... Bn>
struct conjunction<B1, Bn...>
    : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {};

/**
 * @brief Variadic logical OR.
 */
template <class... Bn> struct disjunction;

template <> struct disjunction<> : std::false_type {};

template <class B1> struct disjunction<B1> : B1 {};

template <class B1, class... Bn>
struct disjunction<B1, Bn...>
    : std::conditional<bool(B1::value), B1, disjunction<Bn...>>::type {};

/**
 * @brief Always yields void.
 */
template <class...> using void_t = void;
#endif

/**
 * @brief Check whether a type is a tensor or an expression.
 */
template <class T, typename = void, typename = void>
struct is_expression : std::false_type {};

template <class T>
struct is_expression<T, void_t<typename T::value_type>,
                     void_t<decltype(T::rank)>>
    : std::is_base_of<expression<T, typename T::value_type, T::rank>, T> {};

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
#if __cplusplus >= 201703L
template <class Function, class... Args>
using result_of_t = std::decay_t<std::invoke_result_t<Function, Args...>>;
#else
template <class Function, class... Args>
using result_of_t =
    typename std::decay<typename std::result_of<Function(Args...)>::type>::type;
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

/// Constraints.

/**
 * @brief Type constraint to request N arguments.
 */
template <size_t N, class... Args>
using RequiresNArguments =
    typename std::enable_if<sizeof...(Args) == N, int>::type;

/**
 * @brief Type constraint to request integer arguments.
 */
template <class... T>
using RequiresIntegral =
    typename std::enable_if<conjunction<std::is_integral<T>...>::value,
                            int>::type;

/**
 * @brief Type constraint to request input iterator.
 */
template <class Iterator>
using RequiresInputIterator = typename std::enable_if<
    std::is_convertible<
        typename std::iterator_traits<Iterator>::iterator_category,
        std::input_iterator_tag>::value,
    int>::type;

/**
 * @brief Type constraint to request a scalar (non-expression) argument.
 */
template <class T>
using RequiresScalar =
    typename std::enable_if<!is_expression<T>::value, int>::type;

/**
 * @brief Type constraint to request at least one slice argument.
 */
template <class... Indices>
using RequiresSlicing =
    typename std::enable_if<disjunction<std::is_same<Indices, slice>...>::value,
                            int>::type;

/**
 * @brief Type constraint to request callable type.
 */
template <class F, class... Args>
using RequiresCallable =
    typename std::enable_if<is_callable<F, Args...>::value, int>::type;
} // namespace detail
} // namespace numcpp

#endif // C++11
#endif // NUMCPP_CONFIG_H_INCLUDED
