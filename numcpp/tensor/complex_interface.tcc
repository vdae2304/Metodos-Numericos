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

 /** @file include/numcpp/tensor/complex_interface.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_COMPLEX_INTERFACE_TCC_INCLUDED
#define NUMCPP_COMPLEX_INTERFACE_TCC_INCLUDED

#include "numcpp/iterators/index_sequence.h"

namespace numcpp {
    /// Public methods.

    template <class T, size_t Rank, class Tag>
    inline base_tensor<std::complex<T>, Rank, Tag>*
    complex_interface<std::complex<T>, Rank, Tag>::base() {
        typedef base_tensor<std::complex<T>, Rank, Tag> Derived;
        return static_cast<Derived*>(this);
    }

    template <class T, size_t Rank, class Tag>
    inline const base_tensor<std::complex<T>, Rank, Tag>*
    complex_interface<std::complex<T>, Rank, Tag>::base() const {
        typedef base_tensor<std::complex<T>, Rank, Tag> Derived;
        return static_cast<const Derived*>(this);
    }

    template <class T, size_t Rank, class Tag>
    inline
    base_tensor<T, Rank, lazy_unary_tag<math::real, std::complex<T>, Tag> >
    complex_interface<std::complex<T>, Rank, Tag>::real() const {
        typedef lazy_unary_tag<math::real, std::complex<T>, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::real(), *this->base());
    }

    template <class T, size_t Rank, class Tag>
    template <class TagReal>
    void complex_interface<std::complex<T>, Rank, Tag>::real(
        const base_tensor<T, Rank, TagReal> &arg
    ) {
        if (this->base()->shape() != arg.shape()) {
            std::ostringstream error;
            error << "input shape " << arg.shape() << " does not match the "
                  << "output shape " << this->base()->shape();
            throw std::invalid_argument(error.str());
        }
        for (index_t<Rank> i : make_indices(this->base()->shape())) {
            (*this->base())[i].real(arg[i]);
        }
    }

    template <class T, size_t Rank, class Tag>
    void complex_interface<std::complex<T>, Rank, Tag>::real(const T &val) {
        for (index_t<Rank> i : make_indices(this->base()->shape())) {
            (*this->base())[i].real(val);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline
    base_tensor<T, Rank, lazy_unary_tag<math::imag, std::complex<T>, Tag> >
    complex_interface<std::complex<T>, Rank, Tag>::imag() const {
        typedef lazy_unary_tag<math::imag, std::complex<T>, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::imag(), *this->base());
    }

    template <class T, size_t Rank, class Tag>
    template <class TagImag>
    void complex_interface<std::complex<T>, Rank, Tag>::imag(
        const base_tensor<T, Rank, TagImag> &arg
    ) {
        if (this->base()->shape() != arg.shape()) {
            std::ostringstream error;
            error << "input shape " << arg.shape() << " does not match the "
                  << "output shape " << this->base()->shape();
            throw std::invalid_argument(error.str());
        }
        for (index_t<Rank> i : make_indices(this->base()->shape())) {
            (*this->base())[i].imag(arg[i]);
        }
    }

    template <class T, size_t Rank, class Tag>
    void complex_interface<std::complex<T>, Rank, Tag>::imag(const T &val) {
        for (index_t<Rank> i : make_indices(this->base()->shape())) {
            (*this->base())[i].imag(val);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<std::complex<T>, Rank,
                       lazy_unary_tag<math::conj, std::complex<T>, Tag> >
    complex_interface<std::complex<T>, Rank, Tag>::conj() const {
        typedef std::complex<T> Rt;
        typedef lazy_unary_tag<math::conj, std::complex<T>, Tag> Closure;
        return base_tensor<Rt, Rank, Closure>(math::conj(), *this->base());
    }
}

#endif // NUMCPP_COMPLEX_INTERFACE_TCC_INCLUDED
