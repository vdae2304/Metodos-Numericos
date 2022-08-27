/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library
 * that provides support for multidimensional arrays, and defines an assortment
 * of routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/functional/lazy_outer.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/functional.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_OUTER_H_INCLUDED
#define NUMCPP_LAZY_OUTER_H_INCLUDED

#include <type_traits>

namespace numcpp {
    template <class Function,
              class T, size_t M, class TagT,
              class U, size_t N, class TagU>
    struct lazy_outer_tag;

/// Namespace for implementation details.
namespace detail {
    /// Checks whether a tensor subclass is an expression object.
    template <class R, size_t Rank,
              class Function,
              class T, size_t M, class TagT,
              class U, size_t N, class TagU>
    struct is_expression<
        base_tensor<R, Rank, lazy_outer_tag<Function, T, M, TagT, U, N, TagU> >
    > : std::true_type {};

    /**
     * @brief Split an index into two indices.
     */
    template <size_t Rank1, size_t Rank2>
    void split_indices(
        const index_t<Rank1 + Rank2> &indices,
        index_t<Rank1> &index1, index_t<Rank2> &index2
    ) {
        std::copy_n(static_cast<const size_t*>(indices), Rank1,
                    static_cast<size_t*>(index1));
        std::copy_n(static_cast<const size_t*>(indices) + Rank1, Rank2,
                    static_cast<size_t*>(index2));
    }
}

    /**
     * @brief A lazy_outer is a light-weight object which stores the result of
     * applying a binary function to all pairs of elements from two tensors.
     * The function is evaluated only when required. A lazy_outer is
     * convertible to a tensor object.
     *
     * @tparam R Result type of the function.
     * @tparam Function Type of the applied function.
     * @tparam T Type of the elements contained in the first tensor.
     * @tparam M Dimension of the first tensor. It must be a positive
     *     integer.
     * @tparam TagT Type of the first base_tensor container.
     * @tparam U Type of the elements contained in the second tensor.
     * @tparam N Dimension of the second tensor. It must be a positive
     *     integer.
     * @tparam TagU Type of the second base_tensor container.
     */
    template <class R, size_t Rank,
              class Function,
              class T, size_t M, class TagT,
              class U, size_t N, class TagU>
    class base_tensor<
        R, Rank, lazy_outer_tag<Function, T, M, TagT, U, N, TagU>
    > {
        static_assert(Rank == M + N, "Invalid Rank value");

    public:
        /// Member types.
        typedef typename std::remove_cv<R>::type value_type;
        typedef R reference;
        typedef R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_iterator<
            R, Rank, lazy_outer_tag<Function, T, M, TagT, U, N, TagU>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_outer_tag<Function, T, M, TagT, U, N, TagU>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_outer which stores the result of applying
         * a binary function to all pairs of elements from two tensors.
         *
         * @param f The function to apply.
         * @param lhs First tensor-like argument.
         * @param rhs Second tensor-like argument.
         */
        base_tensor(
            Function f,
            const base_tensor<T, M, TagT> &lhs,
            const base_tensor<U, N, TagU> &rhs
        ) : m_fun(f), m_lhs(lhs), m_rhs(rhs),
            m_shape(lhs.shape() + rhs.shape()),
            m_size(m_shape.size()) {}

        /// Destructor.
        ~base_tensor() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A random access iterator to the beginning of the tensor. If
         *     the tensor is const-qualified, the function returns a
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator begin() {
            return this->begin(this->rowmajor());
        }

        const_iterator begin() const {
            return this->begin(this->rowmajor());
        }

        iterator begin(bool row_major) {
            return make_tensor_iterator(this, 0, row_major);
        }

        const_iterator begin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor. If the tensor is const-qualified, the function returns a
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator end() {
            return this->end(this->rowmajor());
        }

        const_iterator end() const {
            return this->end(this->rowmajor());
        }

        iterator end(bool row_major) {
            return make_tensor_iterator(this, this->size(), row_major);
        }

        const_iterator end(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a const_iterator pointing to the first element in the
         * tensor.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A const_iterator to the beginning of the tensor.
         */
        const_iterator cbegin() const {
            return this->cbegin(this->rowmajor());
        }

        const_iterator cbegin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        /**
         * @brief Returns a const_iterator pointing to the past-the-end element
         * in the tensor.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A const_iterator to the element past the end of the tensor.
         */
        const_iterator cend() const {
            return this->cend(this->rowmajor());
        }

        const_iterator cend(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the result of applying the
         * underlying function to an element in the tensor.
         *
         * @param args... Index arguments.
         *
         * @return The result of the function evaluation at the specified
         *     position in the tensor.
         */
        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        R operator()(Args... args) {
            return this->operator[](make_index(args...));
        }

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        R operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        /**
         * @brief Subscript operator. Returns the result of applying the
         * underlying function to an element in the tensor.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor.
         *
         * @return The result of the function evaluation at the specified
         *     position in the tensor.
         */
        R operator[](const index_t<Rank> &index) {
            assert_within_bounds(m_shape, index);
            index_t<M> i;
            index_t<N> j;
            detail::split_indices(index, i, j);
            return m_fun(m_lhs[i], m_rhs[j]);
        }

        R operator[](const index_t<Rank> &index) const {
            assert_within_bounds(m_shape, index);
            index_t<M> i;
            index_t<N> j;
            detail::split_indices(index, i, j);
            return m_fun(m_lhs[i], m_rhs[j]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        constexpr size_t ndim() const {
            return Rank;
        }

        /**
         * @brief Return the shape of the tensor.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        const shape_t<Rank>& shape() const {
            return m_shape;
        }

        size_t shape(size_t axis) const {
            return m_shape[axis];
        }

        /**
         * @brief Returns the number of elements in the tensor (i.e., the
         * product of the sizes along all the axes).
         */
        size_t size() const {
            return m_size;
        }

        /**
         * @brief Returns whether the tensor is empty (i.e., whether its size
         * is 0).
         */
        bool empty() const {
            return (m_size == 0);
        }

        /**
         * @brief Returns whether the elements are stored in row-major order.
         */
        bool rowmajor() const {
            return (m_lhs.rowmajor() || m_rhs.rowmajor());
        }

        /**
         * @brief Returns whether the elements are stored in column-major
         * order.
         */
        bool colmajor() const {
            return (m_lhs.colmajor() && m_rhs.colmajor());
        }

        /**
         * @brief Cast each element to a specified type.
         */
        template <class Rt>
        base_tensor<Rt, Rank, lazy_outer_tag<Function, T, M, TagT, U, N, TagU> >
        astype() const {
            typedef lazy_outer_tag<Function, T, M, TagT, U, N, TagU> Closure;
            return base_tensor<Rt, Rank, Closure>(m_fun, m_lhs, m_rhs);
        }

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        // Function to apply.
        Function m_fun;

        // First tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<T, M, TagT> > m_lhs;

        // Second tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<U, N, TagU> > m_rhs;

        // Common shape.
        shape_t<Rank> m_shape;

        // Common size.
        size_t m_size;
    };
}

#endif // NUMCPP_LAZY_OUTER_H_INCLUDED
