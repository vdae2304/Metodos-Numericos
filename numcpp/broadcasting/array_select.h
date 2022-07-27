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

 /** @file include/numcpp/broadcasting/array_select.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_SELECT_H_INCLUDED
#define NUMCPP_ARRAY_SELECT_H_INCLUDED

#include "numcpp/array/array_iterator.h"

namespace numcpp {
    template <class TagCond, class TagTrue, class TagFalse>
    struct select_tag;

    /**
     * @brief An array_select is a light-weight object which selects elements
     * from two arrays depending on condition. An array_select is a readonly
     * array which is convertible to an array object.
     *
     * @tparam T Type of the elements contained in the array.
     * @tparam Tag Type of the array container.
     */
    template <class T, class TagCond, class TagTrue, class TagFalse>
    class base_array< T, select_tag<TagCond, TagTrue, TagFalse> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            T, select_tag<TagCond, TagTrue, TagFalse>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Array select constructor. Constructs an array_select.
         *
         * @param condition Where true, yield x, otherwise yield y.
         * @param x Values from which to choose where condition is true.
         * @param y Values from which to choose where condition is false.
         */
        base_array(
            const base_array<bool, TagCond> &condition,
            const base_array<T, TagTrue> &x,
            const base_array<T, TagFalse> &y
        ) : m_cond(condition), m_true(x), m_false(y) {
            __assert_equal_length(condition.size(), x.size());
            __assert_equal_length(condition.size(), y.size());
        }

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * array_select.
         *
         * @return A random access iterator to the beginning of the
         *     array_select.
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the array_select. It does not point to any element, and thus shall
         * not be dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     array_select.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the array_select (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the array_select.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the array_select (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the
         *     array_select.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the element at position i in the
         * reversed array.
         *
         * @param i Position of an element in the array_select. Must be between
         *     0 and size() - 1.
         *
         * @return The element in the array at the specified position.
         */
        T operator[](size_t i) const {
            if (m_cond[i]) {
                return m_true[i];
            }
            else {
                return m_false[i];
            }
        }

        /**
         * @brief Returns the number of elements in the array_reverse.
         *
         * @return The number of elements in the array_reverse.
         */
        size_t size() const {
            return m_cond.size();
        }

        /**
         * @brief Returns whether the array_reverse is empty (i.e., whether
         * its size is 0).
         *
         * @return true if the array_reverse size is 0, false otherwise.
         */
        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_array<bool, TagCond> &m_cond;

        /// Reference to the true argument.
        const base_array<T, TagTrue> &m_true;

        /// Reference to the false argument.
        const base_array<T, TagFalse> &m_false;
    };

    /**
     * @brief Partial specialization of array_select when the second argument
     * is a single value.
     */
    template <class T, class TagCond, class TagTrue>
    class base_array< T, select_tag<TagCond, TagTrue, scalar_tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            T, select_tag<TagCond, TagTrue, scalar_tag>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_array(
            const base_array<bool, TagCond> &condition,
            const base_array<T, TagTrue> &x,
            const T &y
        ) : m_cond(condition), m_true(x), m_false(y) {
            __assert_equal_length(condition.size(), x.size());
        }

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

        T operator[](size_t i) const {
            if (m_cond[i]) {
                return m_true[i];
            }
            else {
                return m_false;
            }
        }

        size_t size() const {
            return m_cond.size();
        }

        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_array<bool, TagCond> &m_cond;

        /// Reference to the true argument.
        const base_array<T, TagTrue> &m_true;

        /// False argument.
        T m_false;
    };

    /**
     * @brief Partial specialization of array_select when the first argument is
     * a single value.
     */
    template <class T, class TagCond, class TagFalse>
    class base_array< T, select_tag<TagCond, scalar_tag, TagFalse> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            T, select_tag<TagCond, scalar_tag, TagFalse>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_array(
            const base_array<bool, TagCond> &condition,
            const T &x,
            const base_array<T, TagFalse> &y
        ) : m_cond(condition), m_true(x), m_false(y) {
            __assert_equal_length(condition.size(), y.size());
        }

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

        T operator[](size_t i) const {
            if (m_cond[i]) {
                return m_true;
            }
            else {
                return m_false[i];
            }
        }

        size_t size() const {
            return m_cond.size();
        }

        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_array<bool, TagCond> &m_cond;

        /// True argument.
        T m_true;

        /// Reference to the false argument.
        const base_array<T, TagFalse> &m_false;
    };

    /**
     * @brief Partial specialization of array_select when both argument are
     * single values.
     */
    template <class T, class TagCond>
    class base_array< T, select_tag<TagCond, scalar_tag, scalar_tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator<
            T, select_tag<TagCond, scalar_tag, scalar_tag>
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_array(
            const base_array<bool, TagCond> &condition,
            const T &x,
            const T &y
        ) : m_cond(condition), m_true(x), m_false(y) {}

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

        T operator[](size_t i) const {
            if (m_cond[i]) {
                return m_true;
            }
            else {
                return m_false;
            }
        }

        size_t size() const {
            return m_cond.size();
        }

        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_array<bool, TagCond> &m_cond;

        /// True argument.
        T m_true;

        /// False argument.
        T m_false;
    };
}

#endif // NUMCPP_ARRAY_SELECT_H_INCLUDED
