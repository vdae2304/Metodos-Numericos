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

 /** @file include/numcpp/broadcasting/lazy_tensor.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_TENSOR_H_INCLUDED
#define NUMCPP_LAZY_TENSOR_H_INCLUDED

#include <type_traits>

namespace numcpp {
    /**
     * @brief A lazy_tensor is a light-weight object which stores the result of
     * applying an unary function on each element in a tensor object. The
     * function is evaluated only when required. A lazy_tensor is convertible
     * to a tensor object.
     *
     * @tparam R Result type of the function.
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     * @tparam Function Type of the applied function.
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Tag Type of the base_tensor container.
     */
    template <class R, size_t Rank, class Function, class T, class Tag>
    class base_tensor<R, Rank, lazy_unary_tag<Function, T, Tag> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<
            typename std::remove_reference<R>::type
        >::type value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_iterator<
            R, Rank, lazy_unary_tag<Function, T, Tag>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_unary_tag<Function, T, Tag>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_tensor which stores the result of applying
         * an unary function on each element in a tensor object.
         *
         * @param f The function to apply.
         * @param arg Tensor-like object.
         */
        base_tensor(Function &&f, const base_tensor<T, Rank, Tag> &arg)
         : m_fun(std::forward<Function>(f)), m_arg(arg) {}

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
        const R operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        /**
         * @brief Subscript operator. Returns the result of applying the
         * underlying function to an element in the tensor.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor. If the tensor is one dimensional, an integer can be
         *     used instead.
         *
         * @return The result of the function evaluation at the specified
         *     position in the tensor.
         */
        R operator[](const index_t<Rank> &index) {
            return m_fun(m_arg[index]);
        }

        const R operator[](const index_t<Rank> &index) const {
            return m_fun(m_arg[index]);
        }

        R operator[](size_t i) {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            return m_fun(m_arg[i]);
        }

        const R operator[](size_t i) const {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            return m_fun(m_arg[i]);
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
            return m_arg.shape();
        }

        size_t shape(size_t axis) const {
            return m_arg.shape(axis);
        }

        /**
         * @brief Returns the number of elements in the tensor (i.e., the
         * product of the sizes along all the axis).
         */
        size_t size() const {
            return m_arg.size();
        }

        /**
         * @brief Returns whether the tensor is empty (i.e., whether its size
         * is 0).
         */
        bool empty() const {
            return m_arg.empty();
        }

        /**
         * @brief Returns whether the elements are stored in row-major order.
         */
        bool rowmajor() const {
            return m_arg.rowmajor();
        }

        /**
         * @brief Returns whether the elements are stored in column-major
         * order.
         */
        bool colmajor() const {
            return m_arg.colmajor();
        }

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        /// Function to apply.
        Function m_fun;

        /// Tensor object where the function is applied.
        const base_tensor<T, Rank, Tag> &m_arg;
    };

    /**
     * @brief A lazy_tensor is a light-weight object which stores the result of
     * applying a binary function on each element in two tensor objects. The
     * function is evaluated only when required. A lazy_tensor is convertible
     * to a tensor object.
     *
     * @tparam R Result type of the function.
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     * @tparam Function Type of the applied function.
     * @tparam T Type of the elements contained in the first tensor.
     * @tparam TagT Type of the first base_tensor container.
     * @tparam U Type of the elements contained in the second tensor.
     * @tparam TagU Type of the second base_tensor container.
     */
    template <class R, size_t Rank,
              class Function, class T, class TagT, class U, class TagU>
    class base_tensor<R, Rank, lazy_binary_tag<Function, T, TagT, U, TagU> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<
            typename std::remove_reference<R>::type
        >::type value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_iterator<
            R, Rank, lazy_binary_tag<Function, T, TagT, U, TagU>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, TagT, U, TagU>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_tensor which stores the result of applying
         * a binary function on each element in two tensor objects.
         *
         * @param f The function to apply.
         * @param lhs First tensor-like argument.
         * @param rhs Second tensor-like argument.
         */
        base_tensor(
            Function &&f,
            const base_tensor<T, Rank, TagT> &lhs,
            const base_tensor<U, Rank, TagU> &rhs
        ) : m_fun(std::forward<Function>(f)),
            m_shape(broadcast_shapes(lhs.shape(), rhs.shape())),
            m_lhs(lhs), m_rhs(rhs)
        {
            m_size = m_shape.size();
        }

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
        const R operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        /**
         * @brief Subscript operator. Returns the result of applying the
         * underlying function to an element in the tensor.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor. If the tensor is one dimensional, an integer can be
         *     used instead.
         *
         * @return The result of the function evaluation at the specified
         *     position in the tensor.
         */
        R operator[](const index_t<Rank> &index) {
            assert_within_bounds(m_shape, index);
            return m_fun(
                m_lhs[this->broadcast_index(index, m_lhs.shape())],
                m_rhs[this->broadcast_index(index, m_rhs.shape())]
            );
        }

        const R operator[](const index_t<Rank> &index) const {
            assert_within_bounds(m_shape, index);
            return m_fun(
                m_lhs[this->broadcast_index(index, m_lhs.shape())],
                m_rhs[this->broadcast_index(index, m_rhs.shape())]
            );
        }

        R operator[](size_t i) {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            size_t i1 = (m_lhs.size() == 1) ? 0 : i;
            size_t i2 = (m_rhs.size() == 1) ? 0 : i;
            return m_fun(m_lhs[i1], m_rhs[i2]);
        }

        const R operator[](size_t i) const {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            size_t i1 = (m_lhs.size() == 1) ? 0 : i;
            size_t i2 = (m_rhs.size() == 1) ? 0 : i;
            return m_fun(m_lhs[i], m_rhs[i]);
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
         * product of the sizes along all the axis).
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
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        /**
         * @brief Broadcasts an index into the appropriate shape.
         */
        index_t<Rank> broadcast_index(
            const index_t<Rank> &index, const shape_t<Rank> &shape
        ) const {
            index_t<Rank> out;
            for (size_t i = 0; i < Rank; ++i) {
                out[i] = (shape[i] == 1) ? 0 : index[i];
            }
            return out;
        }

        /// Function to apply.
        Function m_fun;

        /// Common size.
        size_t m_size;

        /// Common shape.
        shape_t<Rank> m_shape;

        /// First tensor argument.
        const base_tensor<T, Rank, TagT> &m_lhs;

        /// Second tensor argument.
        const base_tensor<U, Rank, TagU> &m_rhs;
    };

    /**
     * @brief Partial specialization of lazy_tensor class for binary functions
     * when the second argument is a value. Values are broadcasted to an
     * appropriate size. The function is evaluated only when required. A
     * lazy_tensor is convertible to a tensor object.
     */
    template <class R, size_t Rank,
              class Function, class T, class Tag, class U>
    class base_tensor<R, Rank,
                      lazy_binary_tag<Function, T, Tag, U, scalar_tag> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<
            typename std::remove_reference<R>::type
        >::type value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_iterator<
            R, Rank, lazy_binary_tag<Function, T, Tag, U, scalar_tag>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, Tag, U, scalar_tag>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_tensor(
            Function &&f,
            const base_tensor<T, Rank, Tag> &lhs,
            const U &val
        ) : m_fun(std::forward<Function>(f)), m_lhs(lhs), m_val(val) {}

        /// Destructor.
        ~base_tensor() {}

        /// Iterators.

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

        const_iterator cbegin() const {
            return this->cbegin(this->rowmajor());
        }

        const_iterator cbegin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        const_iterator cend() const {
            return this->cend(this->rowmajor());
        }

        const_iterator cend(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /// Indexing.

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        R operator()(Args... args) {
            return this->operator[](make_index(args...));
        }

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        const R operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        R operator[](const index_t<Rank> &index) {
            return m_fun(m_lhs[index], m_val);
        }

        const R operator[](const index_t<Rank> &index) const {
            return m_fun(m_lhs[index], m_val);
        }

        R operator[](size_t i) {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            return m_fun(m_lhs[i], m_val);
        }

        const R operator[](size_t i) const {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            return m_fun(m_lhs[i], m_val);
        }

        constexpr size_t ndim() const {
            return Rank;
        }

        const shape_t<Rank>& shape() const {
            return m_lhs.shape();
        }

        size_t shape(size_t axis) const {
            return m_lhs.shape(axis);
        }

        size_t size() const {
            return m_lhs.size();
        }

        bool empty() const {
            return m_lhs.empty();
        }

        bool rowmajor() const {
            return m_lhs.rowmajor();
        }

        bool colmajor() const {
            return m_lhs.colmajor();
        }

        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        /// Function to apply.
        Function m_fun;

        /// First tensor argument.
        const base_tensor<T, Rank, Tag> &m_lhs;

        /// Value to use as second argument.
        U m_val;
    };

    /**
     * @brief Partial specialization of lazy_tensor class for binary functions
     * when the first argument is a value. Values are broadcasted to an
     * appropriate size. The function is evaluated only when required. A
     * lazy_tensor is convertible to a tensor object.
     */
    template <class R, size_t Rank,
              class Function, class T, class U, class Tag>
    class base_tensor<R, Rank,
                      lazy_binary_tag<Function, T, scalar_tag, U, Tag> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<
            typename std::remove_reference<R>::type
        >::type value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_iterator<
            R, Rank, lazy_binary_tag<Function, T, scalar_tag, U, Tag>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, scalar_tag, U, Tag>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_tensor(
            Function &&f,
            const T &val,
            const base_tensor<U, Rank, Tag> &rhs
        ) : m_fun(std::forward<Function>(f)), m_val(val), m_rhs(rhs) {}

        /// Destructor.
        ~base_tensor() {}

        /// Iterators.

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

        const_iterator cbegin() const {
            return this->cbegin(this->rowmajor());
        }

        const_iterator cbegin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        const_iterator cend() const {
            return this->cend(this->rowmajor());
        }

        const_iterator cend(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /// Indexing.

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        R operator()(Args... args) {
            return this->operator[](make_index(args...));
        }

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        const R operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        R operator[](const index_t<Rank> &index) {
            return m_fun(m_val, m_rhs[index]);
        }

        const R operator[](const index_t<Rank> &index) const {
            return m_fun(m_val, m_rhs[index]);
        }

        R operator[](size_t i) {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            return m_fun(m_val, m_rhs[i]);
        }

        const R operator[](size_t i) const {
            static_assert(Rank == 1, "Unkown conversion from integral type");
            return m_fun(m_val, m_rhs[i]);
        }

        constexpr size_t ndim() const {
            return Rank;
        }

        const shape_t<Rank>& shape() const {
            return m_rhs.shape();
        }

        size_t shape(size_t axis) const {
            return m_rhs.shape(axis);
        }

        size_t size() const {
            return m_rhs.size();
        }

        bool empty() const {
            return m_rhs.empty();
        }

        bool rowmajor() const {
            return m_rhs.rowmajor();
        }

        bool colmajor() const {
            return m_rhs.colmajor();
        }

        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        /// Function to apply.
        Function m_fun;

        /// Value to use as first argument.
        T m_val;

        /// Second tensor argument.
        const base_tensor<U, Rank, Tag> &m_rhs;
    };
}

#endif // NUMCPP_LAZY_TENSOR_H_INCLUDED
