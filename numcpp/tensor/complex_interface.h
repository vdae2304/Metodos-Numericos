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

 /** @file include/numcpp/tensor/complex_interface.h
  *  This header defines an interface with additional methods for
  *  complex-valued tensor subclasses.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_COMPLEX_INTERFACE_H_INCLUDED
#define NUMCPP_COMPLEX_INTERFACE_H_INCLUDED

#include "numcpp/tensor/tensor_interface.h"
#include <complex>

namespace numcpp {
    /// Forward declarations.
    template <class T, size_t Rank, class Tag>
    class complex_interface {};

/// Namespace for mathematical functions.
namespace math {
    /**
     * @brief Function object implementing std::real.
     */
    struct real {
        template <class T>
        auto operator()(T&& z) const
         -> decltype(std::real(std::forward<T>(z))) {
            return std::real(std::forward<T>(z));
        }
    };

    /**
     * @brief Function object implementing std::imag.
     */
    struct imag {
        template <class T>
        auto operator()(T&& z) const
         -> decltype(std::imag(std::forward<T>(z))) {
            return std::imag(std::forward<T>(z));
        }
    };

    /**
     * @brief Function object implementing std::conj.
     */
    struct conj {
        template <class T>
        auto operator()(T&& z) const
         -> decltype(std::conj(std::forward<T>(z))) {
            return std::conj(std::forward<T>(z));
        }
    };
}

    /**
     * @brief A complex_interface is an interface with additional methods for
     * complex-valued tensor subclasses. It can be used in conjunction with
     * tensor_interface. A complex_interface works by using the Curiously
     * Recurring Template Pattern
     * @link https://en.cppreference.com/w/cpp/language/crtp @endlink
     *
     * @tparam T Type of the real and imaginary components in the base_tensor.
     * @tparam Rank Dimension of the base_tensor. It must be a positive
     *     integer.
     * @tparam Tag Type indicating which specialization of base_tensor refers
     *     to.
     */
    template <class T, size_t Rank, class Tag>
    class complex_interface<std::complex<T>, Rank, Tag> {
    public:
        /// Public methods.

        /**
         * @brief Return the real part, element-wise.
         *
         * @return A light-weight object with the real part of each element in
         *     the tensor. This function does not create a new tensor, instead,
         *     it returns a readonly view with the real part of each element.
         */
        base_tensor<T, Rank, lazy_unary_tag<math::real, std::complex<T>, Tag> >
        real() const;

        /**
         * @brief Set the real part, element-wise.
         *
         * @param arg A tensor-like object with the values to set the real part
         *     to.
         * @param val Value to set the real part to.
         *
         * @throw std::invalid_argument Thrown if the shapes are different.
         */
        template <class TagReal>
        void real(const base_tensor<T, Rank, TagReal> &arg);
        void real(const T &val);

        /**
         * @brief Return the imaginary part, element-wise.
         *
         * @return A light-weight object with the imaginary part of each
         *     element in the tensor. This function does not create a new
         *     tensor, instead, it returns a readonly view with the imaginary
         *     part of each element.
         */
        base_tensor<T, Rank, lazy_unary_tag<math::imag, std::complex<T>, Tag> >
        imag() const;

        /**
         * @brief Set the imaginary part, element-wise.
         *
         * @param arg A tensor-like object with the values to set the imaginary
         *     part to.
         * @param val Value to set the imaginary part to.
         *
         * @throw std::invalid_argument Thrown if the shapes are different.
         */
        template <class TagImag>
        void imag(const base_tensor<T, Rank, TagImag> &arg);
        void imag(const T &val);

        /**
         * @brief Return the complex conjugate, element-wise.
         *
         * @return A light-weight object with the complex conjugate of each
         *     element in the tensor. This function does not create a new
         *     tensor, instead, it returns a readonly view with the complex
         *     conjugate of each element.
         */
        base_tensor<std::complex<T>, Rank,
                    lazy_unary_tag<math::conj, std::complex<T>, Tag> >
        conj() const;

    private:
        /**
         * @brief Returns a pointer to the (derived) tensor class.
         */
        base_tensor<std::complex<T>, Rank, Tag>* base();
        const base_tensor<std::complex<T>, Rank, Tag>* base() const;
    };
}

#include "numcpp/tensor/complex_interface.tcc"

#endif // NUMCPP_COMPLEX_INTERFACE_H_INCLUDED
