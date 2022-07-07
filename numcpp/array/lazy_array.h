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

 /** @file include/numcpp/array/lazy_array.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/array.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_ARRAY_H_INCLUDED
#define NUMCPP_LAZY_ARRAY_H_INCLUDED

namespace numcpp {
    /**
     * @brief A lazy_array is a light-weight object which stores the result of
     * applying an unary function on each element in an array object. The
     * function is evaluated only when required. A lazy_array is convertible
     * to an array object.
     *
     * @tparam R Result type of the function.
     * @tparam Function Type of the applied function.
     * @tparam T Type of the elements contained in the array.
     * @tparam Tag Type of the array container.
     */
    template <class R, class Function, class T, class Tag>
    class base_array< R, lazy_unary_tag<Function, T, Tag> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            R, lazy_unary_tag<Function, T, Tag>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Lazy array constructor. Constructs a lazy_array which stores
         * the result of applying an unary function on an array object.
         *
         * @param f The function to apply.
         * @param arr Array-like object.
         */
        base_array(Function f, const base_array<T, Tag> &arr)
         : m_fun(f), m_arr(arr) {}

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * lazy_array.
         *
         * @return A random access iterator to the beginning of the lazy_array.
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the lazy_array. It does not point to any element, and thus shall not
         * be dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     lazy_array.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the lazy_array (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the lazy_array.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the lazy_array (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the
         *     lazy_array.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the result of applying the
         * underlying function to the element at position i.
         *
         * @param i Position of an element in the lazy_array. Must be between
         *     0 and size() - 1.
         *
         * @return The result of the function evaluation at the specified
         *     position in the lazy_array.
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        R operator[](size_t i) const {
            return m_fun(m_arr[i]);
        }

        /**
         * @brief Returns the number of elements in the lazy_array.
         *
         * @return The number of elements in the lazy_array.
         */
        size_t size() const {
            return m_arr.size();
        }

        /**
         * @brief Returns whether the lazy_array is empty (i.e., whether its
         * size is 0).
         *
         * @return true if the lazy_array size is 0, false otherwise.
         */
        bool empty() const {
            return m_arr.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Underlying array object.
        const base_array<T, Tag> &m_arr;
    };

    /**
     * @brief A lazy_array is a light-weight object which stores the result of
     * applying a binary function over the elements in two array objects. The
     * function is evaluated only when required. A lazy_array is convertible
     * to an array object.
     *
     * @tparam R Result type of the function.
     * @tparam Function Type of the applied function.
     * @tparam T1 Type of the elements contained in the first array argument.
     * @tparam Tag1 Type of the first array container.
     * @tparam T2 Type of the elements contained in the second array argument.
     * @tparam Tag2 Type of the second array container.
     */
    template <
        class R, class Function, class T1, class Tag1, class T2, class Tag2
    >
    class base_array< R, lazy_binary_tag<Function, T1, Tag1, T2, Tag2> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            R, lazy_binary_tag<Function, T1, Tag1, T2, Tag2>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Lazy array constructor. Constructs a lazy_array which stores
         * the result of applying a binary function on two array objects.
         *
         * @param f The function to apply.
         * @param lhs First array-like argument.
         * @param rhs Second array-like argument.
         *
         * @throw std::invalid_argument Thrown if both array arguments have
         *     different sizes.
         */
        base_array(
            Function f,
            const base_array<T1, Tag1> &lhs,
            const base_array<T2, Tag2> &rhs
        ) : m_fun(f), m_lhs(lhs), m_rhs(rhs) {
            __assert_equal_length(lhs.size(), rhs.size());
        }

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * lazy_array.
         *
         * @return A random access iterator to the beginning of the lazy_array.
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the lazy_array. It does not point to any element, and thus shall not
         * be dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     lazy_array.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the lazy_array (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the lazy_array.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the lazy_array (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the
         *     lazy_array.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the result of applying the
         * underlying function to the elements at position i.
         *
         * @param i Position of an element in the lazy_array. Must be between
         *     0 and size() - 1.
         *
         * @return The result of the function evaluation at the specified
         *     position in the lazy_array.
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        R operator[](size_t i) const {
            return m_fun(m_lhs[i], m_rhs[i]);
        }

        /**
         * @brief Returns the number of elements in the lazy_array.
         *
         * @return The number of elements in the lazy_array.
         */
        size_t size() const {
            return m_lhs.size();
        }

        /**
         * @brief Returns whether the lazy_array is empty (i.e., whether its
         * size is 0).
         *
         * @return true if the lazy_array size is 0, false otherwise.
         */
        bool empty() const {
            return m_lhs.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Reference to the first argument.
        const base_array<T1, Tag1> &m_lhs;

        /// Reference to the second argument.
        const base_array<T2, Tag2> &m_rhs;
    };

    /**
     * @brief Partial specialization of lazy_array class which stores the
     * result of applying a binary function over the elements in the left-hand
     * side array against a value. The function is evaluated only when
     * required. A lazy_array is convertible to an array object.
     */
    template <class R, class Function, class T1, class Tag1, class T2>
    class base_array< R, lazy_binary_tag<Function, T1, Tag1, T2, scalar_tag> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            R, lazy_binary_tag<Function, T1, Tag1, T2, scalar_tag>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_array(Function f, const base_array<T1, Tag1> &lhs, const T2 &val)
         : m_fun(f), m_lhs(lhs), m_val(val) {}

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        iterator begin() const {
            return iterator(this, 0);
        }

        iterator end() const {
            return iterator(this, this->size());
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        R operator[](size_t i) const {
            return m_fun(m_lhs[i], m_val);
        }

        size_t size() const {
            return m_lhs.size();
        }

        bool empty() const {
            return m_lhs.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Reference to the first argument.
        const base_array<T1, Tag1> &m_lhs;

        /// Second argument.
        T2 m_val;
    };

    /**
     * @brief Partial specialization of lazy_array class which stores the
     * result of applying a binary function over the elements in the right-hand
     * side array against a value. The function is evaluated only when
     * required. A lazy_array is convertible to an array object.
     */
    template <class R, class Function, class T1, class T2, class Tag2>
    class base_array< R, lazy_binary_tag<Function, T1, scalar_tag, T2, Tag2> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            R, lazy_binary_tag<Function, T1, scalar_tag, T2, Tag2>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_array(Function f, const T1 &val, const base_array<T2, Tag2> &rhs)
         : m_fun(f), m_val(val), m_rhs(rhs) {}

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        iterator begin() const {
            return iterator(this, 0);
        }

        iterator end() const {
            return iterator(this, this->size());
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        R operator[](size_t i) const {
            return m_fun(m_val, m_rhs[i]);
        }

        size_t size() const {
            return m_rhs.size();
        }

        bool empty() const {
            return m_rhs.empty();
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// First argument.
        T1 m_val;

        /// Reference to the second argument.
        const base_array<T2, Tag2> &m_rhs;
    };
}

#endif // NUMCPP_LAZY_ARRAY_H_INCLUDED
