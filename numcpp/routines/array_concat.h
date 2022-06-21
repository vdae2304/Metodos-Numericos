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

 /** @file include/numcpp/routines/array_concat.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_CONCAT_H_INCLUDED
#define NUMCPP_ARRAY_CONCAT_H_INCLUDED

#include "numcpp/array/array_iterator.h"

namespace numcpp {
    template <class Tag1, class Tag2>
    struct concat_tag;

    /**
     * @brief An array_concat is a light-weight object which stores the 
     * concatenation of two arrays. An array_concat is a readonly array which 
     * is convertible to an array object.
     *
     * @tparam T Type of the elements contained in the array.
     * @tparam Tag1 Type of the first array container.
     * @tparam Tag2 Type of the second array container.
     */
    template <class T, class Tag1, class Tag2>
    class base_array< T, concat_tag<Tag1, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_array_const_iterator< T, concat_tag<Tag1, Tag2> > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Array concatenation constructor. Constructs an array_concat.
         * 
         * @param arr1 First array argument.
         * @param arr2 Second array argument. 
         */
        base_array(
            const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
        ) : m_arr1(arr1), m_arr2(arr2) {}

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * array_concat.
         *
         * @return A random access iterator to the beginning of the 
         *     array_concat. 
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the array_concat. It does not point to any element, and thus shall 
         * not be dereferenced.
         *
         * @return A random access iterator to the element past the end of the 
         *     array_concat.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the array_concat (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the array_concat.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the array_concat (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     array_concat.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the element at position i in the 
         * concatenation.
         *
         * @param i Position of an element in the array_concat. Must be between 
         *     0 and size() - 1.
         *
         * @return The element in the array at the specified position. 
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        T operator[](size_t i) const {
            __assert_within_bounds(this->size(), i);
            if (i < this->m_arr1.size()) {
                return m_arr1[i];
            }
            else {
                return m_arr2[i - m_arr1.size()];
            }
        }

        /**
         * @brief Returns the number of elements in the array_concat.
         *
         * @return The number of elements in the array_concat.
         */
        size_t size() const {
            return this->m_arr1.size() + this->m_arr2.size();
        }

        /**
         * @brief Returns whether the array_concat is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the array_concat size is 0, false otherwise.
         */
        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First array argument.
        const base_array<T, Tag1> &m_arr1;

        /// Second array argument.
        const base_array<T, Tag2> &m_arr2;
    };
}

#endif // NUMCPP_ARRAY_CONCAT_H_INCLUDED
