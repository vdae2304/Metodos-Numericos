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

 /** @file include/numcpp/matrix/lazy_axis.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/matrix.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_AXIS_H_INCLUDED
#define NUMCPP_LAZY_AXIS_H_INCLUDED

#include "numcpp/array/array_iterator.h"

namespace numcpp {
    /**
     * @brief A lazy_axis is a light-weight object which stores the result of 
     * applying a function along each row or column in a matrix object. The 
     * function is evaluated only when required. A lazy_axis is convertible 
     * to an array object.
     *
     * @tparam R Result type of the function.
     * @tparam Function Type of the applied function.
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container.
     */
    template <class R, class Function, class T, class Tag>
    class base_array< R, lazy_axis_tag<Function, T, Tag> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_array_const_iterator<
            R, lazy_axis_tag<Function, T, Tag> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Lazy axis constructor. Constructs a lazy_axis which 
         * stores the result of applying a function along each row or column in 
         * a matrix object.
         * 
         * @param f The function to apply.
         * @param mat Matrix-like object.
         * @param rowwise If true, apply the function along each row. 
         *     Otherwise, apply the function along each column.
         */
        base_array(Function f, const base_matrix<T, Tag> &mat, bool rowwise)
         : m_fun(f), m_mat(mat), m_order(rowwise) {}

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * lazy_axis.
         *
         * @return A random access iterator to the beginning of the 
         *     lazy_axis. 
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the lazy_axis. It does not point to any element, and thus shall 
         * not be dereferenced.
         *
         * @return A random access iterator to the element past the end of the 
         *     lazy_axis.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the lazy_axis (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the lazy_axis.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the lazy_axis (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     lazy_axis.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the result of applying the 
         * underlying function along the row/column i.
         *
         * @param i Position of an element in the lazy_axis. Must be between 
         *     0 and size() - 1.
         *
         * @return The result of the function evaluation at the specified 
         *     position in the lazy_axis. 
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        R operator[](size_t i) const {
            __assert_within_bounds(this->size(), i);
            size_t tda = this->m_order ? this->m_mat.cols() 
                                       : this->m_mat.rows();
            return this->m_fun(
                this->m_mat.begin(this->m_order) + i*tda,
                this->m_mat.begin(this->m_order) + (i + 1)*tda
            );
        }

        /**
         * @brief Returns the number of elements in the lazy_axis.
         *
         * @return The number of elements in the lazy_axis.
         */
        size_t size() const {
            return this->m_order ? this->m_mat.rows() : this->m_mat.cols();
        }

        /**
         * @brief Returns whether the lazy_axis is empty (i.e., whether its 
         * size is 0).
         *
         * @return true if the lazy_axis size is 0, false otherwise.
         */
        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Underlying matrix object.
        const base_matrix<T, Tag> &m_mat;

        /// Whether to apply the function row-wise or column-wise.
        bool m_order;
    };
}

#endif // NUMCPP_LAZY_AXIS_H_INCLUDED
