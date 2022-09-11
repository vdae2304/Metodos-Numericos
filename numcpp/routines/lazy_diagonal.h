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

 /** @file include/numcpp/routines/lazy_diagonal.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_DIAGONAL_H_INCLUDED
#define NUMCPP_LAZY_DIAGONAL_H_INCLUDED

namespace numcpp {
    struct lazy_eye_tag;

    template <class Tag>
    struct lazy_diagonal_tag;

    /**
     * @brief A lazy_eye is a light-weight object with ones on the diagonal and
     * zeros elsewhere. A lazy_eye is convertible to a two-dimensional tensor
     * object.
     *
     * @tparam T Type of the elements contained in the tensor.
     */
    template <class T>
    class base_tensor<T, 2, lazy_eye_tag> {
    public:
        /// Member types.
        typedef T value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<T, 2, lazy_eye_tag> iterator;
        typedef base_tensor_const_iterator<T, 2, lazy_eye_tag> const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_eye with ones on the diagonal and zeros
         * elsewhere.
         *
         * @param m Number of rows.
         * @param n Number of columns.
         * @param k Offset of the diagonal from the main diagonal
         */
        base_tensor(size_t m, size_t n, ptrdiff_t k = 0)
         : m_shape(m, n), m_size(m_shape.size()), m_offset(k) {}

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
            return this->begin(this->layout());
        }

        const_iterator begin(layout_t order) const {
            return make_tensor_const_iterator(this, 0, order);
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
            return this->end(this->layout());
        }

        const_iterator end(layout_t order) const {
            return make_tensor_const_iterator(this, this->size(), order);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i, j Row and column position of an element in the matrix.
         *
         * @return The element at the specified position.
         */

        T operator()(size_t i, size_t j) const {
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
        T operator[](const index_t<2> &index) const {
            return this->operator()(index[0], index[1]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_t ndim() {
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
        const shape_t<2>& shape() const {
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

    private:
        // Number of rows and columns.
        shape_t<2> m_shape;

        // Number of elements.
        size_t m_size;

        // Offset from the main diagonal.
        ptrdiff_t m_offset;
    };

    /**
     * @brief A lazy_diagonal is a light-weight object with given values on the
     * diagonal and zeros elsewhere. A lazy_diagonal is convertible to a
     * two-dimensional tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Tag Type of the base_tensor container.
     */
    template <class T, class Tag>
    class base_tensor<T, 2, lazy_diagonal_tag<Tag> > {
    public:
        /// Member types.
        typedef typename base_tensor<T, 1, Tag>::value_type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, 2, lazy_diagonal_tag<Tag>
        > iterator;
        typedef base_tensor_const_iterator<
            T, 2, lazy_diagonal_tag<Tag>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_diagonal with given values on the diagonal
         * and zeros elsewhere.
         *
         * @param a Array with the elements on the diagonal.
         * @param k Offset of the diagonal from the main diagonal
         */
        base_tensor(const base_tensor<T, 1, Tag> &a, ptrdiff_t k = 0)
         : m_arg(a), m_offset(k) {
            m_shape[0] = (k >= 0) ? a.size() + k : a.size() - k;
            m_shape[1] = m_shape[0];
            m_size = m_shape.size();
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
            return this->begin(this->layout());
        }

        const_iterator begin(layout_t order) const {
            return make_tensor_const_iterator(this, 0, order);
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
            return this->end(this->layout());
        }

        const_iterator end(layout_t order) const {
            return make_tensor_const_iterator(this, this->size(), order);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i, j Row and column position of an element in the matrix.
         *
         * @return The element at the specified position.
         */

        T operator()(size_t i, size_t j) const {
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
        T operator[](const index_t<2> &index) const {
            return this->operator()(index[0], index[1]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_t ndim() {
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
        const shape_t<2>& shape() const {
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

    private:
        // Elements on the diagonal.
        const base_tensor<T, 1, Tag> &m_arg;

        // Offset from the main diagonal.
        ptrdiff_t m_offset;

        // Number of rows and columns.
        shape_t<2> m_shape;

        // Number of elements.
        size_t m_size;
    };

    /**
     * @brief A lazy_diagonal is a light-weight object with the elements on the
     * diagonal of a given matrix. A lazy_diagonal is convertible to a
     * one-dimensional tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Tag Type of the base_tensor container.
     */
    template <class T, class Tag>
    class base_tensor<T, 1, lazy_diagonal_tag<Tag> > {
    public:
        /// Member types.
        typedef typename base_tensor<T, 2, Tag>::value_type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, 1, lazy_diagonal_tag<Tag>
        > iterator;
        typedef base_tensor_const_iterator<
            T, 1, lazy_diagonal_tag<Tag>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_diagonal with the elements on the diagonal
         * of a matrix.
         *
         * @param a Matrix from which the diagonal is taken.
         * @param k Offset of the diagonal from the main diagonal
         */
        base_tensor(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0)
         : m_arg(a), m_offset(k), m_shape() {
            if (k >= 0 && a.shape(1) > size_t(k)) {
                m_shape[0] = std::min(a.shape(0), a.shape(1) - k);
            }
            else if (k < 0 && a.shape(0) > size_t(-k)) {
                m_shape[0] = std::min(a.shape(0) + k, a.shape(1));
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
            return make_tensor_const_iterator(this, 0);
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
            return make_tensor_const_iterator(this, this->size());
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i Position of an element in the array.
         *
         * @return The element at the specified position.
         */

        T operator()(size_t i) const {
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
        T operator[](const index_t<1> &index) const {
            return this->operator()(index[0]);
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_t ndim() {
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
        const shape_t<1>& shape() const {
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
            return static_cast<size_t>(m_shape);
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

    private:
        // Matrix from which the diagonal is taken.
        const base_tensor<T, 2, Tag> &m_arg;

        // Offset from the main diagonal.
        ptrdiff_t m_offset;

        // Number of elements.
        shape_t<1> m_shape;
    };
}

#endif // NUMCPP_LAZY_DIAGONAL_H_INCLUDED
