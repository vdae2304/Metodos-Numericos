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

 /** @file include/numcpp/tensor/shape.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/shape.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_SHAPE_TCC_INCLUDED
#define NUMCPP_SHAPE_TCC_INCLUDED

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    template <size_t Rank>
    shape_t<Rank>::shape_t() {
        std::fill_n(m_shape, Rank, 0);
    }

    template <size_t Rank>
    template <class... Args, typename, typename>
    shape_t<Rank>::shape_t(Args... args) {
        __initialize(args...);
    }

    template <size_t Rank>
    void shape_t<Rank>::__initialize(size_t arg0) {
        m_shape[Rank - 1] = arg0;
    }

    template <size_t Rank>
    template <class... Args>
    void shape_t<Rank>::__initialize(size_t arg0, Args... args) {
        m_shape[Rank - 1 - sizeof...(Args)] = arg0;
        __initialize(args...);
    }

    template <size_t Rank>
    shape_t<Rank>::shape_t(const shape_t &other) {
        std::copy_n(other.m_shape, Rank, m_shape);
    }

    template <size_t Rank>
    inline shape_t<Rank>& shape_t<Rank>::operator=(const shape_t &other) {
        std::copy_n(other.m_shape, Rank, m_shape);
        return *this;
    }

    template <size_t Rank>
    inline shape_t<Rank>& shape_t<Rank>::operator=(size_t val) {
        static_assert(Rank == 1, "Unknown conversion from integral type");
        m_shape[0] = val;
        return *this;
    }

    template <size_t Rank>
    inline shape_t<Rank>::operator size_t() {
        static_assert(Rank == 1, "Unknown conversion to integral type");
        return m_shape[0];
    }

    template <size_t Rank>
    constexpr size_t shape_t<Rank>::ndim() const {
        return Rank;
    }

    template <size_t Rank>
    inline size_t shape_t<Rank>::size() const {
        size_t m_size = 1;
        for (size_t i = 0; i < Rank; ++i) {
            m_size *= m_shape[i];
        }
        return m_size;
    }

    template <size_t Rank>
    inline size_t& shape_t<Rank>::operator[](size_t i) {
        assert_within_bounds(Rank, i);
        return m_shape[i];
    }

    template <size_t Rank>
    inline size_t shape_t<Rank>::operator[](size_t i) const {
        assert_within_bounds(Rank, i);
        return m_shape[i];
    }

    template <size_t Rank>
    inline shape_t<Rank> shape_t<Rank>::transpose() const {
        shape_t<Rank> out;
        std::reverse_copy(m_shape, m_shape + Rank, out.m_shape);
        return out;
    }

    template <class... Args, detail::RequiresIntegral<Args...> >
    inline shape_t<sizeof...(Args)> make_shape(Args... args) {
        return shape_t<sizeof...(Args)>(args...);
    }

    template <class... Args, detail::RequiresIntegral<Args...> >
    inline index_t<sizeof...(Args)> make_index(Args... args) {
        return index_t<sizeof...(Args)>(args...);
    }

    template <size_t Rank>
    size_t ravel_index(
        const index_t<Rank> &index, const shape_t<Rank> &shape, bool order
    ) {
        size_t flat_index = 0;
        size_t size = 1;
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (order) {
                size_t j = shape.ndim() - 1 - i;
                flat_index += size * index[j];
                size *= shape[j];
            }
            else {
                flat_index += size * index[i];
                size *= shape[i];
            }
        }
        return flat_index;
    }

    template <size_t Rank>
    index_t<Rank> unravel_index(
        size_t index, const shape_t<Rank> &shape, bool order
    ) {
        index_t<Rank> multi_index;
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (order) {
                size_t j = shape.ndim() - 1 - i;
                multi_index[j] = index % shape[j];
                index /= shape[j];
            }
            else {
                multi_index[i] = index % shape[i];
                index /= shape[i];
            }
        }
        return multi_index;
    }

    template <size_t Rank>
    shape_t<Rank> broadcast_shapes(
        const shape_t<Rank> &shape1, const shape_t<Rank> &shape2
    ) {
        shape_t<Rank> common_shape;
        for (size_t i = 0; i < shape1.ndim(); ++i) {
            if (shape1[i] == shape2[i]) {
                common_shape[i] = shape1[i];
            }
            else if (shape1[i] == 1) {
                common_shape[i] = shape2[i];
            }
            else if (shape2[i] == 1) {
                common_shape[i] = shape1[i];
            }
            else {
                std::ostringstream error;
                error << "operands could not be broadcast together with shapes "
                      << shape1 << " " << shape2;
                throw std::invalid_argument(error.str());
            }
        }
        return common_shape;
    }

    inline void assert_within_bounds(size_t size, size_t i) {
        if (i >= size) {
            std::ostringstream error;
            error << "index " << i <<  " is out of bounds with size " << size;
            throw std::out_of_range(error.str());
        }
    }

    template <size_t Rank>
    inline void assert_within_bounds(
        const shape_t<Rank> &shape, const index_t<Rank> &index
    ) {
        for (size_t i = 0; i < index.ndim(); ++i) {
            if (index[i] >= shape[i]) {
                std::ostringstream error;
                error << "index " << index <<  " is out of bounds with size "
                      << shape;
                throw std::out_of_range(error.str());
            }
        }
    }

    template <size_t Rank>
    inline void assert_within_bounds(
        const shape_t<Rank> &shape, size_t index, size_t axis
    ) {
        if (index >= shape[axis]) {
            std::ostringstream error;
            error << "index " << index << " is out of bounds for axis "
                  << axis << " with size " << shape[axis];
            throw std::out_of_range(error.str());
        }
    }

    template <size_t Rank1, size_t Rank2>
    inline bool operator==(
        const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2
    ) {
        if (shape1.ndim() != shape2.ndim()) {
            return false;
        }
        for (size_t i = 0; i < shape1.ndim(); ++i) {
            if (shape1[i] != shape2[i]) {
                return false;
            }
        }
        return true;
    }

    template <size_t Rank1, size_t Rank2>
    inline bool operator!=(
        const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2
    ) {
        return !(shape1 == shape2);
    }
}

#endif // NUMCPP_SHAPE_TCC_INCLUDED
