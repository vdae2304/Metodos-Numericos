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

 /** @file include/numcpp/routines/lazy_where.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_WHERE_H_INCLUDED
#define NUMCPP_LAZY_WHERE_H_INCLUDED

#include <type_traits>

namespace numcpp {
    template <class TagCond, class TagTrue, class TagFalse>
    struct lazy_where_tag;

/// Namespace for implementation details.
namespace detail {
    /// Checks whether a tensor subclass is an expression object.
    template <class T, size_t Rank,
              class TagCond, class TagTrue, class TagFalse>
    struct is_expression<
        base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse> >
    > : std::true_type {};
}

    /**
     * @brief A lazy_where is a light-weight object which selects elements from
     * two tensor objects depending on condition. A lazy_where is convertible
     * to a tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     * @tparam TagCond Type of the base_tensor container representing the
     *     condition.
     * @tparam TagTrue Type of the base_tensor container from which to choose
     *     where condition is true.
     * @tparam TagFalse Type of the base_tensor container from which to choose
     *     where condition is false.
     */
    template <class T, size_t Rank,
              class TagCond, class TagTrue, class TagFalse>
    class base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse>
        > iterator;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_where which selects elements from two
         * tensor objects depending on condition.
         *
         * @param condition Where true, yield x, otherwise yield y.
         * @param x Values from which to choose where condition is true.
         * @param y Values from which to choose where condition is false.
         */
        base_tensor(
            const base_tensor<bool, Rank, TagCond> &condition,
            const base_tensor<T, Rank, TagTrue> &x,
            const base_tensor<T, Rank, TagFalse> &y
        ) : m_cond(condition), m_true(x), m_false(y),
            m_shape(broadcast_shapes(condition.shape(), x.shape(), y.shape())),
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
         * @return A random access iterator to the beginning of the tensor.
         */
        const_iterator begin() const {
            return this->begin(this->rowmajor());
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
         *     tensor.
         */
        const_iterator end() const {
            return this->end(this->rowmajor());
        }

        const_iterator end(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param args... Index arguments.
         *
         * @return The element at the specified position.
         */

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        T operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor. If the tensor is one dimensional, an integer can be
         *     used instead.
         *
         * @return The element at the specified position.
         */
        T operator[](const index_t<Rank> &index) const {
            return m_cond[detail::broadcast_index(index, m_cond.shape())]
                ? m_true[detail::broadcast_index(index, m_true.shape())]
                : m_false[detail::broadcast_index(index, m_false.shape())];
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
         * @brief Returns whether the elements are stored in row-major order.
         */
        bool rowmajor() const {
            return (m_cond.rowmajor() ||
                    m_true.rowmajor() ||
                    m_false.rowmajor());
        }

        /**
         * @brief Returns whether the elements are stored in column-major
         * order.
         */
        bool colmajor() const {
            return (m_cond.colmajor() &&
                    m_true.colmajor() &&
                    m_false.colmajor());
        }

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        // Condition tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<bool, Rank, TagCond> >
        m_cond;

        // First tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<T, Rank, TagTrue> >
        m_true;

        // Second tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<T, Rank, TagFalse> >
        m_false;

        // Common shape.
        shape_t<Rank> m_shape;

        // Common size.
        size_t m_size;
    };

    /**
     * @brief Partial specialization of lazy_where class when the false
     * argument is a value. Values are broadcasted to an appropriate size. A
     * lazy_where is convertible to a tensor object.
     */
    template <class T, size_t Rank, class TagCond, class TagTrue>
    class base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag>
        > iterator;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_tensor(
            const base_tensor<bool, Rank, TagCond> &condition,
            const base_tensor<T, Rank, TagTrue> &x,
            const T &y
        ) : m_cond(condition), m_true(x), m_false(y),
            m_shape(broadcast_shapes(condition.shape(), x.shape())),
            m_size(m_shape.size()) {}

        /// Destructor.
        ~base_tensor() {}

        /// Iterators.

        const_iterator begin() const {
            return this->begin(this->rowmajor());
        }

        const_iterator begin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        const_iterator end() const {
            return this->end(this->rowmajor());
        }

        const_iterator end(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /// Indexing.

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        T operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        T operator[](const index_t<Rank> &index) const {
            return m_cond[detail::broadcast_index(index, m_cond.shape())]
                ? m_true[detail::broadcast_index(index, m_true.shape())]
                : m_false;
        }

        constexpr size_t ndim() const {
            return Rank;
        }

        const shape_t<Rank>& shape() const {
            return m_shape;
        }

        size_t shape(size_t axis) const {
            return m_shape[axis];
        }

        size_t size() const {
            return m_size;
        }

        bool rowmajor() const {
            return (m_cond.rowmajor() || m_true.rowmajor());
        }

        bool colmajor() const {
            return (m_cond.colmajor() && m_true.colmajor());
        }

        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        // Condition tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<bool, Rank, TagCond> >
        m_cond;

        // First tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<T, Rank, TagTrue> >
        m_true;

        // Second argument.
        T m_false;

        // Common shape.
        shape_t<Rank> m_shape;

        // Common size.
        size_t m_size;
    };

    /**
     * @brief Partial specialization of lazy_where class when the true
     * argument is a value. Values are broadcasted to an appropriate size. A
     * lazy_where is convertible to a tensor object.
     */
    template <class T, size_t Rank, class TagCond, class TagFalse>
    class base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse>
        > iterator;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_tensor(
            const base_tensor<bool, Rank, TagCond> &condition,
            const T &x,
            const base_tensor<T, Rank, TagFalse> &y
        ) : m_cond(condition), m_true(x), m_false(y),
            m_shape(broadcast_shapes(condition.shape(), y.shape())),
            m_size(m_shape.size()) {}

        /// Destructor.
        ~base_tensor() {}

        /// Iterators.

        const_iterator begin() const {
            return this->begin(this->rowmajor());
        }

        const_iterator begin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        const_iterator end() const {
            return this->end(this->rowmajor());
        }

        const_iterator end(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /// Indexing.

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        T operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        T operator[](const index_t<Rank> &index) const {
            return m_cond[detail::broadcast_index(index, m_cond.shape())]
                ? m_true
                : m_false[detail::broadcast_index(index, m_false.shape())];
        }

        constexpr size_t ndim() const {
            return Rank;
        }

        const shape_t<Rank>& shape() const {
            return m_shape;
        }

        size_t shape(size_t axis) const {
            return m_shape[axis];
        }

        size_t size() const {
            return m_size;
        }

        bool rowmajor() const {
            return (m_cond.rowmajor() || m_false.rowmajor());
        }

        bool colmajor() const {
            return (m_cond.colmajor() && m_false.colmajor());
        }

        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        // Condition tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<bool, Rank, TagCond> >
        m_cond;

        // First tensor argument.
        T m_true;

        // Second argument.
        detail::ConstRefIfNotExpression<base_tensor<T, Rank, TagFalse> >
        m_false;

        // Common shape.
        shape_t<Rank> m_shape;

        // Common size.
        size_t m_size;
    };

    /**
     * @brief Partial specialization of lazy_where class when both the true and
     * false arguments are values. Values are broadcasted to an appropriate
     * size. A lazy_where is convertible to a tensor object.
     */
    template <class T, size_t Rank, class TagCond>
    class base_tensor<
        T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag>
    > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag>
        > iterator;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_tensor(
            const base_tensor<bool, Rank, TagCond> &condition,
            const T &x,
            const T &y
        ) : m_cond(condition), m_true(x), m_false(y) {}

        /// Destructor.
        ~base_tensor() {}

        /// Iterators.

        const_iterator begin() const {
            return this->begin(this->rowmajor());
        }

        const_iterator begin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        const_iterator end() const {
            return this->end(this->rowmajor());
        }

        const_iterator end(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
        }

        /// Indexing.

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        T operator()(Args... args) const {
            return this->operator[](make_index(args...));
        }

        T operator[](const index_t<Rank> &index) const {
            return m_cond[index] ? m_true : m_false;
        }

        constexpr size_t ndim() const {
            return Rank;
        }

        auto shape() const
         -> decltype(std::declval<base_tensor<bool,Rank,TagCond> >().shape()) {
            return m_cond.shape();
        }

        size_t shape(size_t axis) const {
            return m_cond.shape(axis);
        }

        size_t size() const {
            return m_cond.size();
        }

        bool rowmajor() const {
            return m_cond.rowmajor();
        }

        bool colmajor() const {
            return m_cond.colmajor();
        }

        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        // Condition tensor argument.
        detail::ConstRefIfNotExpression<base_tensor<bool, Rank, TagCond> >
        m_cond;

        // First tensor argument.
        T m_true;

        // Second argument.
        T m_false;
    };
}

#endif // NUMCPP_LAZY_WHERE_H_INCLUDED
