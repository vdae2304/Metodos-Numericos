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

 /** @file include/numcpp/functional/lazy_tensor.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/functional.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_TENSOR_H_INCLUDED
#define NUMCPP_LAZY_TENSOR_H_INCLUDED

namespace numcpp {
    /**
     * @brief A lazy_tensor is a light-weight object which stores the result of
     * applying an unary function to each element in a tensor object. The
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
        typedef typename std::decay<R>::type value_type;
        typedef R reference;
        typedef R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_unary_tag<Function, T, Tag>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_unary_tag<Function, T, Tag>
        > const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

    private:
        // Function to apply.
        Function m_fun;

        // Tensor object where the function is applied.
        const base_tensor<T, Rank, Tag> &m_arg;

    public:
        /// Constructors.

        /**
         * @brief Constructs a lazy_tensor which stores the result of applying
         * an unary function on each element in a tensor object.
         *
         * @param f The function to apply.
         * @param arg Tensor-like object.
         */
        base_tensor(Function f, const base_tensor<T, Rank, Tag> &arg)
         : m_fun(f), m_arg(arg) {}

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the beginning of the tensor.
         */
        const_iterator begin() const {
            return const_iterator(this, 0, this->layout());
        }

        const_iterator begin(layout_t order) const {
            return const_iterator(this, 0, order);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor.
         */
        const_iterator end() const {
            return const_iterator(this, this->size(), this->layout());
        }

        const_iterator end(layout_t order) const {
            return const_iterator(this, this->size(), order);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the result of applying the
         * underlying function to an element in the tensor.
         *
         * @param index... Position of an element along each axis.
         *
         * @return The result of the function evaluation at the specified
         *     position in the tensor.
         */
        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        value_type operator()(Index... index) const {
            return m_fun(m_arg(index...));
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
        value_type operator[](const index_type &index) const {
            return m_fun(m_arg[index]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
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
        auto shape() const -> decltype(m_arg.shape()) {
            return m_arg.shape();
        }

        size_type shape(size_type axis) const {
            return m_arg.shape(axis);
        }

        /**
         * @brief Returns the number of elements in the tensor (i.e., the
         * product of the sizes along all the axes).
         */
        size_type size() const {
            return m_arg.size();
        }

        /**
         * @brief Returns the memory layout in which elements are stored.
         */
        layout_t layout() const {
            return m_arg.layout();
        }

        /// Public methods.

        /**
         * @brief Cast each element to a specified type.
         */
        template <class Rt>
        base_tensor<Rt, Rank, lazy_unary_tag<Function, T, Tag> >
        astype() const {
            typedef lazy_unary_tag<Function, T, Tag> Closure;
            return base_tensor<Rt, Rank, Closure>(m_fun, m_arg);
        }

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(*this);
        }
    };

    /**
     * @brief A lazy_tensor is a light-weight object which stores the result of
     * applying a binary function to each element in two tensor objects. The
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
              class Function,
              class T, class TagT, class U, class TagU>
    class base_tensor<R, Rank, lazy_binary_tag<Function, T, TagT, U, TagU> > {
    public:
        /// Member types.
        typedef typename std::decay<R>::type value_type;
        typedef R reference;
        typedef R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, TagT, U, TagU>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, TagT, U, TagU>
        > const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

    private:
        // Function to apply.
        Function m_fun;

        // First tensor argument.
        const base_tensor<T, Rank, TagT> &m_lhs;

        // Second tensor argument.
        const base_tensor<U, Rank, TagU> &m_rhs;

        // Common shape.
        shape_type m_shape;

        // Common size.
        size_type m_size;

    public:
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
            Function f,
            const base_tensor<T, Rank, TagT> &lhs,
            const base_tensor<U, Rank, TagU> &rhs
        ) : m_fun(f), m_lhs(lhs), m_rhs(rhs),
            m_shape(broadcast_shapes(lhs.shape(), rhs.shape())),
            m_size(m_shape.prod()) {}

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the beginning of the tensor.
         */
        const_iterator begin() const {
            return const_iterator(this, 0, this->layout());
        }

        const_iterator begin(layout_t order) const {
            return const_iterator(this, 0, order);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor.
         */
        const_iterator end() const {
            return const_iterator(this, this->size(), this->layout());
        }

        const_iterator end(layout_t order) const {
            return const_iterator(this, this->size(), order);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the result of applying the
         * underlying function to an element in the tensor.
         *
         * @param index... Position of an element along each axis.
         *
         * @return The result of the function evaluation at the specified
         *     position in the tensor.
         */
        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        value_type operator()(Index... index) const {
            return this->operator[](make_index(index...));
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
        value_type operator[](const index_type &index) const {
            return m_fun(
                m_lhs[detail::broadcast_index(index, m_lhs.shape())],
                m_rhs[detail::broadcast_index(index, m_rhs.shape())]
            );
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
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
        const shape_type& shape() const {
            return m_shape;
        }

        size_type shape(size_type axis) const {
            return m_shape[axis];
        }

        /**
         * @brief Returns the number of elements in the tensor (i.e., the
         * product of the sizes along all the axes).
         */
        size_type size() const {
            return m_size;
        }

        /**
         * @brief Returns the memory layout in which elements are stored.
         */
        layout_t layout() const {
            if (m_lhs.layout() == m_rhs.layout()) {
                return m_lhs.layout();
            }
            return row_major;
        }

        /// Public methods.

        /**
         * @brief Cast each element to a specified type.
         */
        template <class Rt>
        base_tensor<Rt, Rank, lazy_binary_tag<Function, T, TagT, U, TagU> >
        astype() const {
            typedef lazy_binary_tag<Function, T, TagT, U, TagU> Closure;
            return base_tensor<Rt, Rank, Closure>(m_fun, m_lhs, m_rhs);
        }

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(*this);
        }
    };

    /**
     * @brief Partial specialization of lazy_tensor class for binary functions
     * when the second argument is a value. Values are broadcasted to an
     * appropriate size. The function is evaluated only when required. A
     * lazy_tensor is convertible to a tensor object.
     */
    template <class R, size_t Rank, class Function, class T, class Tag, class U>
    class base_tensor<
        R, Rank, lazy_binary_tag<Function, T, Tag, U, scalar_tag>
    > {
    public:
        /// Member types.
        typedef typename std::decay<R>::type value_type;
        typedef R reference;
        typedef R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, Tag, U, scalar_tag>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, Tag, U, scalar_tag>
        > const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

    private:
        // Function to apply.
        Function m_fun;

        // First tensor argument.
        const base_tensor<T, Rank, Tag> &m_lhs;

        // Value to use as second argument.
        U m_val;

    public:
        /// Constructors.

        base_tensor(
            Function f,
            const base_tensor<T, Rank, Tag> &lhs,
            const U &val
        ) : m_fun(f), m_lhs(lhs), m_val(val) {}

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        const_iterator begin() const {
            return const_iterator(this, 0, this->layout());
        }

        const_iterator begin(layout_t order) const {
            return const_iterator(this, 0, order);
        }

        const_iterator end() const {
            return const_iterator(this, this->size(), this->layout());
        }

        const_iterator end(layout_t order) const {
            return const_iterator(this, this->size(), order);
        }

        /// Indexing.

        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        value_type operator()(Index... index) const {
            return m_fun(m_lhs(index...), m_val);
        }

        value_type operator[](const index_type &index) const {
            return m_fun(m_lhs[index], m_val);
        }

        static constexpr size_type ndim() {
            return Rank;
        }

        auto shape() const -> decltype(m_lhs.shape()) {
            return m_lhs.shape();
        }

        size_type shape(size_type axis) const {
            return m_lhs.shape(axis);
        }

        size_type size() const {
            return m_lhs.size();
        }

        layout_t layout() const {
            return m_lhs.layout();
        }

        /// Public methods.

        template <class Rt>
        base_tensor<
            Rt, Rank, lazy_binary_tag<Function, T, Tag, U, scalar_tag>
        > astype() const {
            typedef lazy_binary_tag<Function, T, Tag, U, scalar_tag> Closure;
            return base_tensor<Rt, Rank, Closure>(m_fun, m_lhs, m_val);
        }

        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(*this);
        }
    };

    /**
     * @brief Partial specialization of lazy_tensor class for binary functions
     * when the first argument is a value. Values are broadcasted to an
     * appropriate size. The function is evaluated only when required. A
     * lazy_tensor is convertible to a tensor object.
     */
    template <class R, size_t Rank, class Function, class T, class U, class Tag>
    class base_tensor<
        R, Rank, lazy_binary_tag<Function, T, scalar_tag, U, Tag>
    > {
    public:
        /// Member types.
        typedef typename std::decay<R>::type value_type;
        typedef R reference;
        typedef R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, scalar_tag, U, Tag>
        > iterator;
        typedef base_tensor_const_iterator<
            R, Rank, lazy_binary_tag<Function, T, scalar_tag, U, Tag>
        > const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

    private:
        // Function to apply.
        Function m_fun;

        // Value to use as first argument.
        T m_val;

        // Second tensor argument.
        const base_tensor<U, Rank, Tag> &m_rhs;

    public:
        /// Constructors.

        base_tensor(
            Function f,
            const T &val,
            const base_tensor<U, Rank, Tag> &rhs
        ) : m_fun(f), m_val(val), m_rhs(rhs) {}

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        const_iterator begin() const {
            return const_iterator(this, 0, this->layout());
        }

        const_iterator begin(layout_t order) const {
            return const_iterator(this, 0, order);
        }

        const_iterator end() const {
            return const_iterator(this, this->size(), this->layout());
        }

        const_iterator end(layout_t order) const {
            return const_iterator(this, this->size(), order);
        }

        /// Indexing.

        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        value_type operator()(Index... index) const {
            return m_fun(m_val, m_rhs(index...));
        }

        value_type operator[](const index_type &index) const {
            return m_fun(m_val, m_rhs[index]);
        }

        static constexpr size_type ndim() {
            return Rank;
        }

        auto shape() const -> decltype(m_rhs.shape()) {
            return m_rhs.shape();
        }

        size_type shape(size_type axis) const {
            return m_rhs.shape(axis);
        }

        size_type size() const {
            return m_rhs.size();
        }

        layout_t layout() const {
            return m_rhs.layout();
        }

        /// Public methods.

        template <class Rt>
        base_tensor<
            Rt, Rank, lazy_binary_tag<Function, T, scalar_tag, U, Tag>
        > astype() const {
            typedef lazy_binary_tag<Function, T, scalar_tag, U, Tag> Closure;
            return base_tensor<Rt, Rank, Closure>(m_fun, m_val, m_rhs);
        }

        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(*this);
        }
    };
}

#endif // NUMCPP_LAZY_TENSOR_H_INCLUDED
