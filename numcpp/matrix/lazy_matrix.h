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

 /** @file include/numcpp/matrix/lazy_matrix.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/matrix.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_MATRIX_H_INCLUDED
#define NUMCPP_LAZY_MATRIX_H_INCLUDED

#include <cstdio>
#include <stdexcept>

namespace numcpp {
    /**
     * @brief A lazy_matrix is a light-weight object which stores the result of 
     * applying an unary function on each element in a matrix object. The 
     * function is evaluated only when required. A lazy_matrix is convertible 
     * to a matrix object.
     *
     * @tparam R Result type of the function.
     * @tparam Function Type of the applied function.
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container.
     */
    template <class R, class Function, class T, class Tag>
    class base_matrix< R, lazy_unary_tag<Function, T, Tag> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator<
            R, lazy_unary_tag<Function, T, Tag> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Lazy matrix constructor. Constructs a lazy_matrix which 
         * stores the result of applying an unary function on a matrix object.
         * 
         * @param f The function to apply.
         * @param mat Matrix-like object.
         */
        base_matrix(Function f, const base_matrix<T, Tag> &mat)
         : m_fun(f), m_mat(mat) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * lazy_matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     lazy_matrix. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the lazy_matrix. It does not point to any element, and thus shall 
         * not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     lazy_matrix.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the lazy_matrix (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the lazy_matrix.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the lazy_matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     lazy_matrix.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        /**
         * @brief Call operator. Returns the result of applying the underlying 
         * function to the element at row i and column j.
         *
         * @param i Row position of an element in the lazy_matrix. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the lazy_matrix. Must be 
         *     between 0 and cols() - 1.
         *
         * @return The result of the function evaluation at the specified 
         *     row and column in the lazy_matrix. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        R operator()(size_t i, size_t j) const {
            return this->m_fun(this->m_mat(i, j));
        }

        /** 
         * @brief Returns the number of rows in the lazy_matrix. 
         * 
         * @return The number of rows in the lazy_matrix.
         */
        size_t rows() const {
            return this->m_mat.rows();
        }

        /** 
         * @brief Returns the number of columns in the lazy_matrix.
         * 
         * @return The number of columns in the lazy_matrix.
         */
        size_t cols() const {
            return this->m_mat.cols();
        }

        /**
         * @brief Returns the number of elements in the lazy_matrix, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the lazy_matrix.
         */
        size_t size() const {
            return this->m_mat.size();
        }

        /**
         * @brief Returns whether the lazy_matrix is empty (i.e., whether its 
         * size is 0).
         *
         * @return true if the lazy_matrix size is 0, false otherwise.
         */
        bool empty() const {
            return this->m_mat.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Underlying matrix object.
        const base_matrix<T, Tag> &m_mat;
    };

    /**
     * @brief A lazy_matrix is a light-weight object which stores the result of 
     * applying a binary function over the elements in two matrix objects. The 
     * function is evaluated only when required. A lazy_matrix is convertible 
     * to a matrix object.
     *
     * @tparam R Result type of the function.
     * @tparam Function Type of the applied function.
     * @tparam T1 Type of the elements contained in the first matrix argument.
     * @tparam Tag1 Type of the first matrix container.
     * @tparam T2 Type of the elements contained in the second matrix argument.
     * @tparam Tag2 Type of the second matrix container.
     */
    template <
        class R, class Function, class T1, class Tag1, class T2, class Tag2
    >
    class base_matrix< R, lazy_binary_tag<Function, T1, Tag1, T2, Tag2> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator<
            R, lazy_binary_tag<Function, T1, Tag1, T2, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Lazy matrix constructor. Constructs a lazy_matrix which 
         * stores the result of applying a binary function on two matrix 
         * objects.
         * 
         * @param f The function to apply.
         * @param lhs First matrix-like argument.
         * @param rhs Second matrix-like argument.
         * 
         * @throw std::invalid_argument Thrown if both matrix arguments have 
         *     different shapes.
         */
        base_matrix(
            Function f, 
            const base_matrix<T1, Tag1> &lhs, 
            const base_matrix<T2, Tag2> &rhs
        ) : m_fun(f), m_lhs(lhs), m_rhs(rhs) {
            if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
                char error[160];
                sprintf(
                    error, "operands could not be broadcast together with "
                    "shapes (%zu, %zu) (%zu, %zu)", lhs.rows(), lhs.cols(), 
                    rhs.rows(), rhs.cols()
                );
                throw std::invalid_argument(error);
            }
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * lazy_matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     lazy_matrix. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the lazy_matrix. It does not point to any element, and thus shall 
         * not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     lazy_matrix.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the lazy_matrix (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the lazy_matrix.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the lazy_matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     lazy_matrix.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        /**
         * @brief Call operator. Returns the result of applying the underlying 
         * function to the elements at row i and column j.
         *
         * @param i Row position of an element in the lazy_matrix. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the lazy_matrix. Must be 
         *     between 0 and cols() - 1.
         *
         * @return The result of the function evaluation at the specified 
         *     row and column in the lazy_matrix. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        R operator()(size_t i, size_t j) const {
            return this->m_fun(this->m_lhs(i, j), this->m_rhs(i, j));
        }

        /** 
         * @brief Returns the number of rows in the lazy_matrix. 
         * 
         * @return The number of rows in the lazy_matrix.
         */
        size_t rows() const {
            return this->m_lhs.rows();
        }

        /** 
         * @brief Returns the number of columns in the lazy_matrix.
         * 
         * @return The number of columns in the lazy_matrix.
         */
        size_t cols() const {
            return this->m_lhs.cols();
        }

        /**
         * @brief Returns the number of elements in the lazy_matrix, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the lazy_matrix.
         */
        size_t size() const {
            return this->m_lhs.size();
        }

        /**
         * @brief Returns whether the lazy_matrix is empty (i.e., whether its 
         * size is 0).
         *
         * @return true if the lazy_matrix size is 0, false otherwise.
         */
        bool empty() const {
            return this->m_lhs.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Reference to the first argument.
        const base_matrix<T1, Tag1> &m_lhs;

        /// Reference to the second argument.
        const base_matrix<T2, Tag2> &m_rhs;
    };

    /**
     * @brief Partial specialization of lazy_matrix class which stores the 
     * result of applying a binary function over the elements in the left-hand 
     * side matrix against a value. The function is evaluated only when 
     * required. A lazy_matrix is convertible to a matrix object.
     */
    template <class R, class Function, class T1, class Tag1, class T2>
    class base_matrix< 
        R, lazy_binary_tag<Function, T1, Tag1, T2, scalar_tag> 
    > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator<
            R, lazy_binary_tag<Function, T1, Tag1, T2, scalar_tag> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(Function f, const base_matrix<T1, Tag1> &lhs, const T2 &val)
         : m_fun(f), m_lhs(lhs), m_val(val) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        R operator()(size_t i, size_t j) const {
            return this->m_fun(this->m_lhs(i, j), this->m_val);
        }

        size_t rows() const {
            return this->m_lhs.rows();
        }

        size_t cols() const {
            return this->m_lhs.cols();
        }

        size_t size() const {
            return this->m_lhs.size();
        }

        bool empty() const {
            return this->m_lhs.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Reference to the first argument.
        const base_matrix<T1, Tag1> &m_lhs;

        /// Second argument.
        T2 m_val;
    };

    /**
     * @brief Partial specialization of lazy_matrix class which stores the 
     * result of applying a binary function over the elements in the right-hand 
     * side matrix against a value. The function is evaluated only when 
     * required. A lazy_matrix is convertible to a matrix object.
     */
    template <class R, class Function, class T1, class T2, class Tag2>
    class base_matrix< 
        R, lazy_binary_tag<Function, T1, scalar_tag, T2, Tag2> 
    > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator<
            R, lazy_binary_tag<Function, T1, scalar_tag, T2, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(Function f, const T1 &val, const base_matrix<T2, Tag2> &rhs)
         : m_fun(f), m_val(val), m_rhs(rhs) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        iterator end() const {
            return iterator(this, this->size());
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        R operator()(size_t i, size_t j) const {
            return this->m_fun(this->m_val, this->m_rhs(i, j));
        }

        size_t rows() const {
            return this->m_rhs.rows();
        }

        size_t cols() const {
            return this->m_rhs.cols();
        }

        size_t size() const {
            return this->m_rhs.size();
        }

        bool empty() const {
            return this->m_rhs.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// First argument.
        T1 m_val;

        /// Reference to the second argument.
        const base_matrix<T2, Tag2> &m_rhs;
    };
}

#endif // NUMCPP_LAZY_MATRIX_H_INCLUDED
