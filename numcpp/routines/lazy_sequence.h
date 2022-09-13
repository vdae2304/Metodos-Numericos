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

 /** @file include/numcpp/routines/lazy_sequence.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_SEQUENCE_H_INCLUDED
#define NUMCPP_LAZY_SEQUENCE_H_INCLUDED

#include <cmath>

namespace numcpp {
    struct sequence_tag;

    /**
     * @brief A lazy_sequence is a light-weight object which stores a sequence
     * of evenly spaced values. A lazy_sequence is convertible to a
     * one-dimensional tensor object.
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
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_sequence.
         *
         * @param start The starting value of the sequence.
         * @param size Number of values in the sequence.
         * @param step Spacing between values.
         * @param base The base of the log-scale (if any).
         */
        base_tensor(const T &start, size_t size, const T &step)
         : m_start(start), m_shape(size), m_step(step),
           m_log(false) {}

        base_tensor(const T &start, size_t size, const T &step, const T &base)
         : m_start(start), m_shape(size), m_step(step),
           m_log(true), m_base(base) {}

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
            return make_tensor_const_iterator(this, 0);
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
            return make_tensor_const_iterator(this, this->size());
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param i Position of an element in the sequence.
         *
         * @return The element at the specified position.
         */
        T operator()(size_t i) const {
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
        tensor<T, 1> copy() const {
            return tensor<T, 1>(*this);
        }

    private:
        // Starting value of the sequence.
        T m_start;

        // Number of elements in the sequence.
        shape_t<1> m_shape;

        // Step of the sequence.
        T m_step;

        // Whether the sequence is in a log scale.
        bool m_log;

        // Base of the log scale.
        T m_base;
    };
}

#endif // NUMCPP_LAZY_SEQUENCE_H_INCLUDED
