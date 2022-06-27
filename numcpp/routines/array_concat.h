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
#include <tuple>

namespace numcpp {
    template <class... Arrays>
    struct concat_tag;

    /**
     * @brief An array_concat is a light-weight object which stores the 
     * concatenation of one or more arrays. An array_concat is a readonly array 
     * which is convertible to an array object.
     *
     * @tparam T Type of the elements contained in the array.
     * @tparam Arrays... Array containers to concatenate.
     */
    template <class T, class... Arrays>
    class base_array< T, concat_tag<Arrays...> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_array_const_iterator< T, concat_tag<Arrays...> > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Array concatenation constructor. Constructs an array_concat.
         * 
         * @param arrays Arrays to concatenate. 
         */
        base_array(const Arrays&... arrays) : m_arrays(arrays...) {
            this->m_size = __sum_sizes();
        }

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
            return __at(i);
        }

        /**
         * @brief Returns the number of elements in the array_concat.
         *
         * @return The number of elements in the array_concat.
         */
        size_t size() const {
            return this->m_size;
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

    private:
        /// Helper function. Return the sum of the sizes of the arrays in the 
        /// concatenation.
        template <size_t I = 0>
        typename std::enable_if<I < sizeof...(Arrays), size_t>::type 
        __sum_sizes() const {
            return __array_size(std::get<I>(this->m_arrays)) + 
                   __sum_sizes<I + 1>();
        }

        template <size_t I = 0>
        typename std::enable_if<I == sizeof...(Arrays), size_t>::type 
        __sum_sizes() const {
            return 0;
        }

        /// Helper function. Return the i-th element in the concatenation.
        template <size_t I = 0>
        typename std::enable_if<I < sizeof...(Arrays), T>::type 
        __at(size_t i) const {
            if (i < __array_size(std::get<I>(this->m_arrays))) {
                return __array_element(std::get<I>(this->m_arrays), i);
            }
            else {
                i -= __array_size(std::get<I>(this->m_arrays));
                return __at<I + 1>(i);
            }
        }

        template <size_t I = 0>
        typename std::enable_if<I == sizeof...(Arrays), T>::type 
        __at(size_t) const {
            return T();
        }

        /// Helper function. Return the size of an array.
        template <class Tag>
        size_t __array_size(const base_array<T, Tag> &arr) const {
            return arr.size();
        }

        /// Helper function. Return an element from an array.
        template <class Tag>
        T __array_element(const base_array<T, Tag> &arr, size_t i) const {
            return arr[i];
        }
    
    protected:
        /// Number of elements in the concatenation.
        size_t m_size;

        /// Array arguments.
        std::tuple<const Arrays&...> m_arrays;
    };
}

#endif // NUMCPP_ARRAY_CONCAT_H_INCLUDED
