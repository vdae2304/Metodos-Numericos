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

 /** @file include/numcpp/slice.h
  *  This header defines slice class.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_SLICE_H_INCLUDED
#define NUMCPP_SLICE_H_INCLUDED

#include <type_traits>

namespace numcpp {
    /**
     * @brief A slice is a class that identifies a subset of elements in an
     * array. It holds three values: the starting index, the stride and the
     * number of elements in the subset.
     */
    class slice {
    public:
        /// Constructors.

        /**
         * @brief Default constructor. Construct an empty slice. Equivalent to
         * slice(0, 0, 0).
         */
        slice() : m_offset(0), m_size(0), m_stride(0) {}

        /**
         * @brief Slice constructor. Constructs a slice object.
         *
         * @param start The position of the first element in the slice.
         *     Defaults to 0 if not provided.
         * @param size The number of elements in the slice.
         * @param stride The span that separates the elements selected into the
         *     slice. Defaults to 1 if not provided.
         */
        slice(size_t size) : m_offset(0), m_size(size), m_stride(1) {}

        slice(size_t start, size_t size)
         : m_offset(start), m_size(size), m_stride(1) {}

        slice(size_t start, size_t size, size_t stride)
         : m_offset(start), m_size(size), m_stride(stride) {}

        /**
         * @brief Copy constructor. Constructs a slice as a copy of other.
         *
         * @param other A slice object.
         */
        slice(const slice& other)
         : m_offset(other.m_offset), m_size(other.m_size),
           m_stride(other.m_stride) {}

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns a copy of other.
         *
         * @param other A slice object.
         *
         * @return *this
         */
        slice& operator=(const slice& other) {
            m_offset = other.m_offset;
            m_size = other.m_size;
            m_stride = other.m_stride;
            return *this;
        }

        /// Public methods.

        /**
         * @brief Returns the index of the first element in the slice.
         */
        size_t start() const {
            return m_offset;
        }

        /**
         * @brief Returns the number of elements in the slice.
         */
        size_t size() const {
            return m_size;
        }

        /**
         * @brief Returns the separation of the elements in the slice.
         */
        size_t stride() const {
            return m_stride;
        }

        /// Indexing.

        /**
         * @brief Returns the element at position i in the slice.
         *
         * @param i Position of an element in the slice. This function does not
         *     check for out of bounds.
         *
         * @return start() + i * stride()
         */
        size_t operator[](size_t i) const {
            return m_offset + i * m_stride;
        }

    private:
        /// Offset, size and stride of the slice.
        size_t m_offset, m_size, m_stride;
    };

    /// Operator overloading (non member functions).

    /**
     * @brief Compares if two slices are equal.
     */
    inline bool operator==(const slice &lhs, const slice &rhs) {
        return lhs.start() == rhs.start() &&
               lhs.size() == rhs.size() &&
               lhs.stride() == rhs.stride();
    }

    /**
     * @brief Compares if two slices are different.
     */
    inline bool operator!=(const slice &lhs, const slice &rhs) {
        return !(lhs == rhs);
    }
}

#endif // NUMCPP_SLICE_H_INCLUDED
