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

 /** @file include/numcpp/routines/sequence_array.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_SEQUENCE_ARRAY_H_INCLUDED
#define NUMCPP_SEQUENCE_ARRAY_H_INCLUDED

#include <cmath>

namespace numcpp {
    /// Forward declarations.
    struct sequence_tag;

    /**
     * @brief A light-weight object which stores a sequence of evenly spaced
     * values. Convertible to a 1-dimensional tensor object.
     *
     * @tparam T Type of the elements contained in the sequence.
     */
    template <class T>
    class base_tensor<T, 1, sequence_tag> {
    public:
        /// Member types.
        typedef T value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<T, 1, sequence_tag> iterator;
        typedef base_tensor_const_iterator<T, 1, sequence_tag> const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<1> shape_type;
        typedef index_t<1> index_type;

    private:
        // Starting value of the sequence.
        T m_start;

        // Number of elements in the sequence.
        size_type m_size;

        // Step of the sequence.
        T m_step;

        // Whether the sequence is in a log scale.
        bool m_log;

        // Base of the log scale.
        T m_base;

    public:
        /// Constructors.

        /**
         * @brief Constructs a sequence of evenly spaced values.
         *
         * @param start The starting value of the sequence.
         * @param size Number of values in the sequence.
         * @param step Spacing between values.
         * @param base The base of the log-scale. If not provided, the sequence
         *     is in linear scale.
         */
        base_tensor(const T &start, size_type size, const T &step)
         : m_start(start), m_size(size), m_step(step), m_log(false) {}

        base_tensor(
            const T &start, size_type size, const T &step, const T &base
        ) : m_start(start), m_size(size), m_step(step), m_log(true),
            m_base(base) {}

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * sequence.
         *
         * @return A random access iterator to the beginning of the sequence.
         */
        const_iterator begin(layout_t = row_major) const {
            return const_iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the sequence. It does not point to any element, and thus shall not
         * be dereferenced.
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
         * @param i Position of an element in the sequence.
         *
         * @return The element at the specified position.
         */
        value_type operator()(size_type i) const {
            T val = m_start + T(i) * m_step;
            return m_log ? std::pow(m_base, val) : val;
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param i An index_t object with the position of an element in the
         *     sequence.
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
}

#endif // NUMCPP_SEQUENCE_ARRAY_H_INCLUDED