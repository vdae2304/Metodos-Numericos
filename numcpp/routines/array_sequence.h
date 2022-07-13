/*
 * This file is part of the NumCpp project.
 *
 * NumCpp is a package for scientific computing in C++. It is a C++ library
 * that provides an array and a matrix object, and an assortment of routines
 * for fast operations on arrays and matrices, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * The NumCpp package is inspired by the NumPy package for Python, although it
 * is not related to it or any of its parts.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/routines/array_sequence.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_SEQUENCE_H_INCLUDED
#define NUMCPP_ARRAY_SEQUENCE_H_INCLUDED

#include "numcpp/array/array_iterator.h"

namespace numcpp {
    struct sequence_tag;
    struct log_sequence_tag;

    /**
     * @brief An array_sequence is a light-weight object which stores a
     * sequence of evenly spaced values. An array_sequence is a readonly array
     * which is convertible to an array object.
     *
     * @tparam T Type of the elements contained in the array.
     */
    template <class T>
    class base_array<T, sequence_tag> {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<T, sequence_tag> iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Array sequence constructor. Constructs an array_sequence.
         *
         * @param start The starting value of the sequence.
         * @param size Number of samples to generate.
         * @param step Spacing between values. This is the distance between two
         *     adjacent values.
         */
        base_array(const T &start, size_t size, const T &step) {
            m_start = start;
            m_size = size;
            m_step = step;
        }

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * array_sequence.
         *
         * @return A random access iterator to the beginning of the
         *     array_sequence.
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the array_sequence. It does not point to any element, and thus shall
         * not be dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     array_sequence.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the array_sequence (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the array_sequence.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the array_sequence (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the
         *     array_sequence.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the element at position i in the
         * sequence.
         *
         * @param i Position of an element in the array_sequence. Must be
         *     between 0 and size() - 1.
         *
         * @return The element in the sequence at the specified position.
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        T operator[](size_t i) const {
            __assert_within_bounds(m_size, i);
            return m_start + T(i) * m_step;
        }

        /**
         * @brief Returns the number of elements in the array_sequence.
         *
         * @return The number of elements in the array_sequence.
         */
        size_t size() const {
            return m_size;
        }

        /**
         * @brief Returns whether the array_sequence is empty (i.e., whether
         * its size is 0).
         *
         * @return true if the array_sequence size is 0, false otherwise.
         */
        bool empty() const {
            return (m_size == 0);
        }

    protected:
        /// Starting value of the sequence.
        T m_start;

        /// Number of elements in the sequence.
        size_t m_size;

        /// Step of the sequence.
        T m_step;
    };

    /**
     * @brief An array_log_sequence is a light-weight object which stores a
     * sequence of evenly spaced values on a log scale. An array_log_sequence
     * is a readonly array which is convertible to an array object.
     *
     * @tparam T Type of the elements contained in the array.
     */
    template <class T>
    class base_array<T, log_sequence_tag> {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<T, log_sequence_tag> iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Array sequence constructor. Constructs an array_log_sequence.
         *
         * @param start The starting value of the sequence in the log scale.
         * @param size Number of samples to generate.
         * @param step Spacing between values in the log scale.
         * @param base The base of the log space.
         */
        base_array(const T &start, size_t size, const T &step, const T &base) {
            m_start = start;
            m_size = size;
            m_step = step;
            m_base = base;
        }

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * array_log_sequence.
         *
         * @return A random access iterator to the beginning of the
         *     array_log_sequence.
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the array_log_sequence. It does not point to any element, and thus
         * shall not be dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     array_log_sequence.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the array_log_sequence (i.e., its reverse beginning). Reverse
         * iterators iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the array_log_sequence.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the array_log_sequence (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the
         *     array_log_sequence.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the element at position i in the
         * sequence.
         *
         * @param i Position of an element in the array_log_sequence. Must be
         *     between 0 and size() - 1.
         *
         * @return The element in the sequence at the specified position.
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        T operator[](size_t i) const {
            __assert_within_bounds(m_size, i);
            return std::pow(m_base, m_start + T(i) * m_step);
        }

        /**
         * @brief Returns the number of elements in the array_log_sequence.
         *
         * @return The number of elements in the array_log_sequence.
         */
        size_t size() const {
            return m_size;
        }

        /**
         * @brief Returns whether the array_log_sequence is empty (i.e.,
         * whether its size is 0).
         *
         * @return true if the array_log_sequence size is 0, false otherwise.
         */
        bool empty() const {
            return (m_size == 0);
        }

    protected:
        /// Starting value of the sequence in the log scale.
        T m_start;

        /// Number of elements in the sequence.
        size_t m_size;

        /// Step of the sequence in the log scale.
        T m_step;

        /// Base of the log scale.
        T m_base;
    };
}

#endif // NUMCPP_ARRAY_SEQUENCE_H_INCLUDED
