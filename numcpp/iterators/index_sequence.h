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

 /** @file include/numcpp/iterators/index_sequence.h
  *  This header defines an iterator to retrieve indices.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_INDEX_SEQUENCE_H_INCLUDED
#define NUMCPP_INDEX_SEQUENCE_H_INCLUDED

#include "numcpp/shape.h"
#include <iterator>

namespace numcpp {
    /**
     * @brief An n-dimensional iterator to index tensors.
     *
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     */
    template <size_t Rank>
    class index_sequence {
    public:
        /// Member types.

        typedef index_t<Rank> value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        class iterator {
        public:
            /// Member types.

            typedef ptrdiff_t difference_type;
            typedef index_t<Rank> value_type;
            typedef nullptr_t pointer;
            typedef index_t<Rank> reference;
            typedef std::forward_iterator_tag iterator_category;

            /// Constructors.

            iterator() : m_shape(), m_index(0) {}

            iterator(const shape_t<Rank> &shape, size_t index = 0)
             : m_shape(shape), m_index(index) {}

            iterator(const iterator &other)
             : m_shape(other.m_shape), m_index(other.m_index) {}

            /// Copy assignment.

            iterator& operator=(const iterator &other) {
                m_shape = other.m_shape;
                m_index = other.m_index;
                return *this;
            }

            /// Operator overloading.

            index_t<Rank> operator*() const {
                return unravel_index(m_index, m_shape);
            }

            iterator& operator++() {
                ++m_index;
                return *this;
            }

            iterator operator++(int) {
                iterator it = *this;
                ++m_index;
                return it;
            }

            friend bool operator==(const iterator &lhs, const iterator &rhs) {
                return (lhs.m_index == rhs.m_index);
            }

            friend bool operator!=(const iterator &lhs, const iterator &rhs) {
                return (lhs.m_index != rhs.m_index);
            }

        private:
            /// Number of elements along each axis.
            shape_t<Rank> m_shape;

            /// Flat index.
            size_t m_index;
        };

        /**
         * @brief Constructor.
         *
         * @param shape The size of each dimension.
         */
        index_sequence(const shape_t<Rank> &shape)
         : m_size(shape.size()), m_shape(shape) {}

        /**
         * @brief Return an iterator to the first index.
         */
        iterator begin() const {
            return iterator(m_shape, 0);
        }

        /**
         * @brief Return an iterator to the past the end index.
         */
        iterator end() const {
            return iterator(m_shape, m_size);
        }

        /**
         * @brief Return the number of elements along each axis.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape along all
         *     axes.
         */
        const shape_t<Rank>& shape() const {
            return m_shape;
        }

        size_t shape(size_t axis) const {
            return m_shape[axis];
        }

        /**
         * @brief Return the number of elements.
         */
        size_t size() const {
            return m_size;
        }

    private:
        /// Number of elements.
        size_t m_size;

        /// Number of elements along each axis.
        shape_t<Rank> m_shape;
    };

    /**
     * @brief An n-dimensional iterator to index tensors.
     *
     * @param shape The size of each dimension. It can be a shape_t object or
     *     the elements of the shape passed as separate arguments.
     *
     * @return An index_sequence object which iterates over the indices of a
     *     tensor. At each iteration, a new index is returned. The elements are
     *     iterated in row-major order, i.e., from first axis to last axis.
     */
    template <size_t Rank>
    index_sequence<Rank> make_indices(const shape_t<Rank> &shape) {
        return index_sequence<Rank>(shape);
    }

    template <class... Args, detail::RequiresIntegral<Args...> = true>
    index_sequence<sizeof...(Args)> make_indices(Args... args) {
        return index_sequence<sizeof...(Args)>(make_shape(args...));
    }
}

#endif // NUMCPP_INDEX_SEQUENCE_H_INCLUDED
