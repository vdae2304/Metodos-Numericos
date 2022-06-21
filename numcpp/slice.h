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

 /** @file include/numcpp/slice.h
  *  This header defines slice class.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_SLICE_H_INCLUDED
#define NUMCPP_SLICE_H_INCLUDED

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
        slice();

        /**
         * @brief Slice constructor. Constructs a slice object.
         * 
         * @param start The position of the first element in the slice. 
         *     Defaults to 0 if not provided.
         * @param size The number of elements in the slice.
         * @param stride The span that separates the elements selected into the 
         *     slice. Defaults to 1 if not provided.
         */
        slice(size_t size);
        slice(size_t start, size_t size);
        slice(size_t start, size_t size, size_t stride);

        /**
         * @brief Copy constructor. Constructs a slice as a copy of other.
         * 
         * @param other A slice object.
         */
        slice(const slice& other); 

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns a copy of other.
         * 
         * @param other A slice object.
         * 
         * @return *this
         */
        slice& operator=(const slice& other);

        /// Public methods.

        /**
         * @brief Returns the index of the first element in the slice.
         * 
         * @return Index of the first element selected by the slice.
         */
        size_t start() const;

        /**
         * @brief Returns the number of elements in the slice.
         * 
         * @return Number of elements selected by the slice.
         */
        size_t size() const;

        /**
         * @brief Returns the separation of the elements in the slice.
         * 
         * @return Separation of the elements selected by the slice.
         */
        size_t stride() const;

        /// Indexing.

        /**
         * @brief Returns the element at position i in the slice.
         * 
         * @param i Position of an element in the slice. This function does not 
         *     check for out of bounds.
         * 
         * @return start() + i*stride()
         */
        size_t operator[](size_t i) const;

    protected:
        /// Offset, size and stride of the slice.
        size_t m_offset, m_size, m_stride;
    };

    inline slice::slice() {
        this->m_offset = 0;
        this->m_size = 0;
        this->m_stride = 0;
    }

    inline slice::slice(size_t size) {
        this->m_offset = 0;
        this->m_size = size;
        this->m_stride = 1;
    }
    
    inline slice::slice(size_t start, size_t size) {
        this->m_offset = start;
        this->m_size = size;
        this->m_stride = 1;
    }
    
    inline slice::slice(size_t start, size_t size, size_t stride) {
        this->m_offset = start;
        this->m_size = size;
        this->m_stride = stride;
    }

    inline slice::slice(const slice& other) {
        this->m_offset = other.m_offset;
        this->m_size = other.m_size;
        this->m_stride = other.m_stride;
    }

    inline slice& slice::operator=(const slice& other) {
        this->m_offset = other.m_offset;
        this->m_size = other.m_size;
        this->m_stride = other.m_stride;
        return *this;
    }

    inline size_t slice::start() const {
        return this->m_offset;
    }
    
    inline size_t slice::size() const {
        return this->m_size;
    }

    inline size_t slice::stride() const {
        return this->m_stride;
    }

    inline size_t slice::operator[](size_t i) const {
        return this->m_offset + i*this->m_stride;
    }

    /// Operator overloading (non member functions).

    inline bool operator==(const slice &lhs, const slice &rhs) {
        return lhs.start() == rhs.start() && 
               lhs.size() == rhs.size() && 
               lhs.stride() == rhs.stride();
    }

    inline bool operator!=(const slice &lhs, const slice &rhs) {
        return lhs.start() != rhs.start() || 
               lhs.size() != rhs.size() || 
               lhs.stride() != rhs.stride();
    }
}

#endif // NUMCPP_SLICE_H_INCLUDED
