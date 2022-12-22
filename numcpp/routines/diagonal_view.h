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

 /** @file include/numcpp/routines/diagonal_view.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_DIAGONAL_VIEW_H_INCLUDED
#define NUMCPP_DIAGONAL_VIEW_H_INCLUDED

namespace numcpp {
    /// Forward declarations.
    struct eye_tag;

    template <class Tag>
    struct diagonal_tag;

    template <class Tag>
    struct triangular_tag;

    /**
     * @brief A light-weight object with ones on the diagonal and zeros
     * elsewhere. Convertible to a 2-dimensional tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     */
    template <class T>
    class base_tensor<T, 2, eye_tag> {
    public:
        /// Member types.
        typedef T value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<T, 2, eye_tag> iterator;
        typedef base_tensor_const_iterator<T, 2, eye_tag> const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<2> shape_type;
        typedef index_t<2> index_type;

    private:
        // Number of rows and columns.
        shape_type m_shape;

        // Number of elements.
        size_type m_size;

        // Offset from the main diagonal.
        difference_type m_offset;

    public:
        /// Constructors.

        /**
         * @brief Constructs a matrix with ones on the diagonal and zeros
         * elsewhere.
         *
         * @param m Number of rows.
         * @param n Number of columns.
         * @param k Offset of the diagonal from the main diagonal
         */
        base_tensor(size_type m, size_type n, difference_type k = 0)
         : m_shape(m, n), m_size(m * n), m_offset(k) {}

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
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i, j Row and column position of an element in the matrix.
         *
         * @return The element at the specified position.
         */
        value_type operator()(size_type i, size_type j) const {
            if (m_offset >= 0) {
                return (i + m_offset == j) ? T(1) : T();
            }
            else {
                return (i == j - m_offset) ? T(1) : T();
            }
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param index An index_t object with the position of an element in
         *     the matrix.
         *
         * @return The element at the specified position.
         */
        value_type operator[](const index_type &index) const {
            return this->operator()(index[0], index[1]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
            return 2;
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
            return row_major;
        }

        /// Public methods.

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, 2> copy() const {
            return tensor<value_type, 2>(*this);
        }
    };

    /**
     * @brief A light-weight object with its elements extracted from the
     * diagonal of a matrix. Convertible to a 1-dimensional tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Tag Type of the base_tensor container.
     */
    template <class T, class Tag>
    class base_tensor<T, 1, diagonal_tag<Tag> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<T, 1, diagonal_tag<Tag> >
            iterator;
        typedef base_tensor_const_iterator<T, 1, diagonal_tag<Tag> >
            const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<1> shape_type;
        typedef index_t<1> index_type;

    private:
        // Matrix from which the diagonal is taken.
        const base_tensor<T, 2, Tag> &m_arg;

        // Number of elements.
        size_type m_size;

        // Offset from the main diagonal.
        ptrdiff_t m_offset;

    public:
        /// Constructors.

        /**
         * @brief Constructs an array with its elements extracted from the
         * diagonal of a matrix.
         *
         * @param a Matrix from which the diagonal is taken.
         * @param k Offset of the diagonal from the main diagonal
         */
        base_tensor(const base_tensor<T, 2, Tag> &a, difference_type k = 0)
         : m_arg(a), m_size(0), m_offset(k) {
            if (k >= 0 && a.shape(1) > size_type(k)) {
                m_size = std::min(a.shape(0), a.shape(1) - k);
            }
            else if (k < 0 && a.shape(0) > size_type(-k)) {
                m_size = std::min(a.shape(0) + k, a.shape(1));
            }
        }

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @return A random access iterator to the beginning of the tensor.
         */
        const_iterator begin(layout_t = row_major) const {
            return const_iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor.
         */
        const_iterator end(layout_t = row_major) const {
            return const_iterator(this, this->size());
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i Position of an element in the array.
         *
         * @return The element at the specified position.
         */
        value_type operator()(size_type i) const {
            if (m_offset >= 0) {
                return m_arg(i, i + m_offset);
            }
            else {
                return m_arg(i - m_offset, i);
            }
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param index An index_t object with the position of an element in
         *     the array.
         *
         * @return The element at the specified position.
         */
        value_type operator[](const index_type &index) const {
            return this->operator()(index[0]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
            return 1;
        }

        /**
         * @brief Return the shape of the tensor.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        shape_type shape() const {
            return shape_type(m_size);
        }

        size_type shape(size_type axis) const {
            return (axis == 0) ? m_size : 0;
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
            return row_major;
        }

        /// Public methods.

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, 1> copy() const {
            return tensor<value_type, 1>(*this);
        }
    };

    /**
     * @brief A light-weight object with given values on the diagonal and zeros
     * elsewhere. Convertible to a 2-dimensional tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Tag Type of the base_tensor container.
     */
    template <class T, class Tag>
    class base_tensor<T, 2, diagonal_tag<Tag> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<T, 2, diagonal_tag<Tag> >
            iterator;
        typedef base_tensor_const_iterator<T, 2, diagonal_tag<Tag> >
            const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<2> shape_type;
        typedef index_t<2> index_type;

    private:
        // Elements on the diagonal.
        const base_tensor<T, 1, Tag> &m_arg;

        // Number of rows and columns.
        shape_type m_shape;

        // Number of elements.
        size_type m_size;

        // Offset from the main diagonal.
        difference_type m_offset;

    public:
        /// Constructors.

        /**
         * @brief Constructs a diagonal matrix with given values on the
         * diagonal and zeros elsewhere.
         *
         * @param a Array with the elements on the diagonal.
         * @param k Offset of the diagonal from the main diagonal
         */
        base_tensor(const base_tensor<T, 1, Tag> &a, difference_type k = 0)
         : m_arg(a), m_offset(k) {
            m_shape[0] = (k >= 0) ? a.size() + k : a.size() - k;
            m_shape[1] = m_shape[0];
            m_size = m_shape.prod();
        }

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
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i, j Row and column position of an element in the matrix.
         *
         * @return The element at the specified position.
         */
        value_type operator()(size_type i, size_type j) const {
            if (m_offset >= 0) {
                return (i + m_offset == j) ? m_arg[i] : T();
            }
            else {
                return (i == j - m_offset) ? m_arg[j] : T();
            }
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param index An index_t object with the position of an element in
         *     the matrix.
         *
         * @return The element at the specified position.
         */
        value_type operator[](const index_type &index) const {
            return this->operator()(index[0], index[1]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
            return 2;
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
            return row_major;
        }

        /// Public methods.

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, 2> copy() const {
            return tensor<value_type, 2>(*this);
        }
    };

    /**
     * @brief A light-weight object with given values below the diagonal and
     * zeros elsewhere (a lower triangular matrix), or with given values above
     * the diagonal and zeros elsewhere (an upper diagonal matrix). Convertible
     * to a 2-dimensional tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Tag Type of the base_tensor container.
     */
    template <class T, class Tag>
    class base_tensor<T, 2, triangular_tag<Tag> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<T, 2, triangular_tag<Tag> >
            iterator;
        typedef base_tensor_const_iterator<T, 2, triangular_tag<Tag> >
            const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<2> shape_type;
        typedef index_t<2> index_type;

    private:
        // Elements below/above the diagonal.
        const base_tensor<T, 2, Tag> &m_arg;

        // Whether to extract the lower or upper triangle.
        bool m_lower;

        // Offset from the main diagonal.
        difference_type m_offset;

    public:
        /// Constructors.

        /**
         * @brief Constructs a triangular matrix with given values below/above
         * the diagonal and zeros elsewhere.
         *
         * @param a Matrix from which to extract the lower/upper triangle.
         * @param lower Whether to extract the lower triangle or the upper
         *     triangle.
         * @param k Offset of the diagonal from the main diagonal
         */
        base_tensor(
            const base_tensor<T, 2, Tag> &a, bool lower = true,
            difference_type k = 0
        ) : m_arg(a), m_lower(lower), m_offset(k) {}

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
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i, j Row and column position of an element in the matrix.
         *
         * @return The element at the specified position.
         */
        value_type operator()(size_type i, size_type j) const {
            if (m_offset >= 0) {
                if (m_lower) {
                    return (j <= i + m_offset) ? m_arg(i, j) : T();
                }
                else {
                    return (j >= i + m_offset) ? m_arg(i, j) : T();
                }
            }
            else {
                if (m_lower) {
                    return (j - m_offset <= i) ? m_arg(i, j) : T();
                }
                else {
                    return (j - m_offset >= i) ? m_arg(i, j) : T();
                }
            }
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param index An index_t object with the position of an element in
         *     the matrix.
         *
         * @return The element at the specified position.
         */
        value_type operator[](const index_type &index) const {
            return this->operator()(index[0], index[1]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
            return 2;
        }

        /**
         * @brief Return the shape of the tensor.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        auto shape() -> decltype(m_arg.shape()) {
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
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, 2> copy() const {
            return tensor<value_type, 2>(*this);
        }
    };
}

#endif // NUMCPP_DIAGONAL_VIEW_H_INCLUDED
