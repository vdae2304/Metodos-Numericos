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

 /** @file include/numcpp/tensor/indirect_tensor.h
  *  This header defines the indirect_tensor class.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_INDIRECT_TENSOR_H_INCLUDED
#define NUMCPP_INDIRECT_TENSOR_H_INCLUDED

namespace numcpp {
    /**
     * @brief An indirect_tensor is a view of a subset of elements from a
     * multidimensional array. It references the elements in the original
     * array through an array of indices. The view itself does not own the data
     * and any changes made to the view will affect the original array, and any
     * changes made to the original array will affect the view.
     *
     * @tparam T Type of the elements contained in the indirect_tensor. This
     *     shall be an arithmetic type or a class that behaves like one (such
     *     as std::complex).
     * @tparam Rank Dimension of the indirect_tensor. It must be a positive
     *     integer.
     */
    template <class T, size_t Rank>
    class base_tensor<T, Rank, indirect_tag>
     : public tensor_interface<T, Rank, indirect_tag>,
       public complex_interface<T, Rank, indirect_tag> {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

        /// Constructors.

        /**
         * @brief Default constructor. Constructs an indirect_tensor that does
         * not reference any object.
         */
        base_tensor();

        /**
         * @brief Index array constructor. Constructs an indirect_tensor that
         * references a subset of elements from a multidimensional array.
         *
         * @param data Pointer to the memory array used by the indirect_tensor.
         * @param shape Number of elements along each axis.
         * @param indices Pointer to the array of indices with its elements
         *     identifying which elements of data are selected.
         * @param order Order in which elements shall be iterated. In row-major
         *     order, the last index is varying the fastest. In column-major
         *     order, the first index is varying the fastest. Defaults to
         *     row-major order.
         * @param mode If positive, creates a copy of indices. If zero, stores
         *     the pointer directly without making a copy of it. If negative,
         *     acquires the ownership of indices, which will be deleted
         *     along with the indirect_tensor. Defaults to make a copy.
         */
        base_tensor(
            T *data, const shape_t<Rank> &shape, const size_t *indices,
            layout_t order = row_major, int mode = 1
        );

        /**
         * @brief Copy constructor. Constructs an indirect_tensor as a copy of
         * other. The ownership is also copied, i.e., if other is owner of an
         * array of indices, then *this creates a copy of it. Otherwise, *this
         * and other shares the same array of indices.
         *
         * @param other An indirect_tensor of the same type and rank.
         */
        base_tensor(const base_tensor &other);

        /**
         * @brief Move constructor. Constructs an indirect_tensor that acquires
         * the elements of other.
         *
         * @param other An indirect_tensor of the same type and rank. The
         *     ownership is directly transferred from other. other is left in
         *     an empty state.
         */
        base_tensor(base_tensor &&other);

        /// Destructor.
        ~base_tensor();

        /// Indexing.

        /**
         * @brief Call operator. Returns a reference to the element at the
         * given position. The elements in an indirect_tensor are given by
         *     data[indices[ravel_index(index, shape, order)]]
         * where data is the memory array and indices is the array of indices.
         *
         * @param index... Position of an element along each axis.
         *
         * @return The element at the specified position. If the
         *     indirect_tensor is const-qualified, the function returns a
         *     reference to const T. Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if index is out of bounds.
         */
        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        T& operator()(Index... index);

        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        const T& operator()(Index... index) const;

        /**
         * @brief Subscript operator. Returns a reference to the element at the
         * given position.
         *
         * @param index An index_t object with the position of an element in
         *     the indirect_tensor. If the tensor is one dimensional, an
         *     integer can be used instead.
         *
         * @return The element at the specified position. If the
         *     indirect_tensor is const-qualified, the function returns a
         *     reference to const T. Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if index is out of bounds.
         */
        T& operator[](const index_t<Rank> &index);
        const T& operator[](const index_t<Rank> &index) const;

        T& operator[](size_t i);
        const T& operator[](size_t i) const;

        /**
         * @brief Return the dimension of the indirect_tensor.
         */
        static constexpr size_t ndim();

        /**
         * @brief Return the shape of the indirect_tensor.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        const shape_t<Rank>& shape() const;
        size_t shape(size_t axis) const;

        /**
         * @brief Returns the number of elements in the indirect_tensor (i.e.,
         * the product of the sizes along all the axes).
         *
         * @note Time complexity: O(1)
         */
        size_t size() const;

        /**
         * @brief Returns whether the indirect_tensor is empty (i.e., whether
         * its size is 0).
         */
        bool empty() const;

        /**
         * @brief Returns a pointer to the memory array used internally by the
         * indirect_tensor.
         *
         * @return A pointer to the memory array used internally by the
         *     indirect_tensor. If the indirect_tensor is const-qualified, the
         *     function returns a pointer to const T. Otherwise, it returns a
         *     pointer to T.
         */
        T* data();
        const T* data() const;

        /**
         * @brief Returns a pointer to the array of indices used internally by
         * the indirect_tensor.
         *
         * @return A const pointer to the array of indices used internally by
         *     the indirect_tensor.
         */
        const size_t* indices() const;

        /**
         * @brief Returns the order in which elements are iterated. It is not
         * necessarily the memory layout in which elements are stored as the
         * elements might not be continuous in memory.
         */
        layout_t layout() const;

        /**
         * @brief Returns whether the indirect_tensor is owner of an array of
         * indices.
         */
        bool is_owner() const;

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns to each element the value of the
         * corresponding element in other.
         *
         * @param other A tensor-like object of the same shape.
         *
         * @return *this
         *
         * @throw std::invalid_argument Thrown if the shapes are different.
         */
        template <class U, class Tag>
        base_tensor& operator=(const base_tensor<U, Rank, Tag> &other);

        /**
         * @brief Fill assignment. Assigns val to every element.
         *
         * @param val Value assigned to all the elements in the indirect_tensor.
         *
         * @return *this
         */
        base_tensor& operator=(const T &val);

        /**
         * @brief Move assignment. Acquires the contents of other, leaving
         * other in an empty state.
         *
         * @param other An indirect_tensor of the same type and rank. The
         *     ownership is directly transferred from other. other is left in
         *     an empty state.
         *
         * @return *this
         */
        base_tensor& operator=(base_tensor &&other);

    private:
        // Pointer to data.
        T *m_data;

        // Number of elements along each axis.
        shape_t<Rank> m_shape;

        // Number of elements.
        size_t m_size;

        // Pointer to the array of indices.
        const size_t *m_indices;

        // Memory layout.
        layout_t m_order;

        // Ownership of the array of indices.
        bool m_owner;
    };
}

#include "numcpp/tensor/indirect_tensor.tcc"

#endif // NUMCPP_INDIRECT_TENSOR_H_INCLUDED
