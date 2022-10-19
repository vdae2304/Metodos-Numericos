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

 /** @file include/numcpp/linalg/lu_factor.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/linalg.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LU_FACTOR_H_INCLUDED
#define NUMCPP_LU_FACTOR_H_INCLUDED

namespace numcpp {
namespace linalg {
    /**
     * @brief Class returned by linalg::lu.
     *
     * @tparam T Type of the elements contained in the matrix. This shall be a
     *     floating-point type or a complex type.
     */
    template <class T>
    class lu_result {
    public:
        /// Constructors.

        /**
         * @brief Default constructor.
         */
        lu_result() : m_lu(), m_piv() {}

        /**
         * @brief Copy constructor.
         */
        lu_result(const lu_result &other)
         : m_lu(other.m_lu), m_piv(other.m_piv) {}

        /**
         * @brief Move constructor.
         */
        lu_result(lu_result &&other)
         : m_lu(std::move(other.m_lu)), m_piv(std::move(other.m_piv)) {}

        /**
         * @brief Constructor reseved for linalg::lu.
         */
        lu_result(tensor<T, 2> &&lu, tensor<size_t, 1> &&piv)
         : m_lu(std::move(lu)), m_piv(std::move(piv)) {}

        /// Destructor.

        ~lu_result() = default;

        /// Assignment operator.

        /**
         * @brief Copy assignment.
         */
        lu_result& operator=(const lu_result &other) {
            m_lu = other.m_lu;
            m_piv = other.m_piv;
            return *this;
        }

        /**
         * @brief Move assignment.
         */
        lu_result& operator=(lu_result &&other) {
            m_lu = std::move(other.m_lu);
            m_piv = std::move(other.m_piv);
            return *this;
        }

        /// Public methods.

        /**
         * @brief Unpack P, L and U into the destination matrices.
         *
         * @param P A location into which the permutation matrix is stored.
         * @param L A location into which the lower triangular matrix with unit
         *     diagonal is stored.
         * @param U A location into which the upper triangular matrix is
         *     stored.
         */
        template <class TagP, class TagL, class TagU>
        void tie(
            base_tensor<T, 2, TagP> &P,
            base_tensor<T, 2, TagL> &L,
            base_tensor<T, 2, TagU> &U
        ) const {
            P = std::move(this->P());
            L = std::move(this->L());
            U = std::move(this->U());
        }

        /**
         * @brief Return the permutation matrix.
         */
        tensor<T, 2> P() const;

        /**
         * @brief Return the lower triangular matrix with unit diagonal of the
         * decomposition.
         */
        tensor<T, 2> L() const;

        /**
         * @brief Return the upper triangular matrix of the decomposition.
         */
        tensor<T, 2> U() const;

        /**
         * @brief Return the decomposed matrix.
         */
        tensor<T, 2> A() const;

        /**
         * @brief Return a matrix containing U in its upper triangle and L in
         * its lower triangle. The unit elements of L are not stored.
         */
        const tensor<T, 2>& LU() const;

        /**
         * @brief Return the pivot indices representing the permutation matrix.
         * row i was interchanged with row piv[i].
         */
        const tensor<size_t, 1>& piv() const;

    private:
        // Matrix containing U in its upper triangle and L in its lower
        // triangle.
        tensor<T, 2> m_lu;

        // Pivot indices representing the permutation matrix.
        tensor<size_t, 1> m_piv;
    };

    template <class T>
    tensor<T, 2> lu_result<T>::P() const {
        size_t m = m_piv.size();
        tensor<T, 2> out({m, m}, T());
        for (size_t i = 0; i < m; ++i) {
            out(i, i) = T(1);
        }
        for (size_t k = 0; k < m; ++k) {
            size_t i = m - 1 - k;
            for (size_t j = 0; j < m; ++j) {
                std::swap(out(m_piv[i], j), out(i, j));
            }
        }
        return out;
    }

    template <class T>
    tensor<T, 2> lu_result<T>::L() const {
        size_t m = m_lu.shape(0);
        size_t n = std::min(m_lu.shape(0), m_lu.shape(1));
        tensor<T, 2> out({m, n}, T());
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < i && j < n; ++j) {
                out(i, j) = m_lu(i, j);
            }
            if (i < n) {
                out(i, i) = T(1);
            }
        }
        return out;
    }

    template <class T>
    tensor<T, 2> lu_result<T>::U() const {
        size_t m = std::min(m_lu.shape(0), m_lu.shape(1));
        size_t n = m_lu.shape(1);
        tensor<T, 2> out({m, n}, T());
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = i; j < n; ++j) {
                out(i, j) = m_lu(i, j);
            }
        }
        return out;
    }

    template <class T>
    tensor<T, 2> lu_result<T>::A() const {
        size_t m = m_lu.shape(0), n = m_lu.shape(1);
        tensor<T, 2> out(m, n);
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T val = T();
                for (size_t k = 0; k < i && k <= j; ++k) {
                    val += m_lu(i, k) * m_lu(k, j);
                }
                if (i <= j) {
                    val += m_lu(i, j);
                }
                out(i, j) = val;
            }
        }
        for (size_t k = 0; k < m; ++k) {
            size_t i = m - 1 - k;
            for (size_t j = 0; j < n; ++j) {
                std::swap(out(m_piv[i], j), out(i, j));
            }
        }
        return out;
    }

    template <class T>
    inline const tensor<T, 2>& lu_result<T>::LU() const {
        return m_lu;
    }

    template <class T>
    inline const tensor<size_t, 1>& lu_result<T>::piv() const {
        return m_piv;
    }

    /**
     * @brief Class returned by linalg::ldl.
     *
     * @tparam T Type of the elements contained in the matrix. This shall be a
     *     floating-point type or a complex type.
     */
    template <class T>
    class ldl_result {
    public:
        /// Constructors.

        /**
         * @brief Default constructor.
         */
        ldl_result() : m_l(), m_d() {}

        /**
         * @brief Copy constructor.
         */
        ldl_result(const ldl_result &other) : m_l(other.m_l), m_d(other.m_d) {}

        /**
         * @brief Move constructor.
         */
        ldl_result(ldl_result &&other)
         : m_l(std::move(other.m_l)), m_d(std::move(other.m_d)) {}

        /**
         * @brief Constructor reseved for linalg::ldl.
         */
        ldl_result(tensor<T, 2> &&l, tensor<T, 1> &&d)
         : m_l(std::move(l)), m_d(std::move(d)) {}

        /// Destructor.

        ~ldl_result() = default;

        /// Assignment operator.

        /**
         * @brief Copy assignment.
         */
        ldl_result& operator=(const ldl_result &other) {
            m_l = other.m_l;
            m_d = other.m_d;
            return *this;
        }

        /**
         * @brief Move assignment.
         */
        ldl_result& operator=(ldl_result &&other) {
            m_l = std::move(other.m_l);
            m_d = std::move(other.m_d);
            return *this;
        }

        /// Public methods.

        /**
         * @brief Unpack L and D into the destination matrices.
         *
         * @param L A location into which the lower triangular matrix is
         *     stored.
         * @param D A location into which the diagonal matrix is stored.
         */
        template <class TagL, class TagD>
        void tie(
            base_tensor<T, 2, TagL> &L,
            base_tensor<T, 2, TagD> &D
        ) const {
            L = std::move(this->L());
            D = std::move(this->D());
        }

        /**
         * @brief Return the lower triangular matrix of the decomposition.
         */
        const tensor<T, 2>& L() const;

        /**
         * @brief Return the diagonal matrix of the decomposition.
         */
        tensor<T, 2> D() const;

        /**
         * @brief Return the diagonal of the diagonal matrix of the
         * decomposition.
         */
        const tensor<T, 1>& d() const;

        /**
         * @brief Return the decomposed matrix.
         */
        tensor<T, 2> A() const;

    private:
        // Lower triangular factor.
        tensor<T, 2> m_l;

        // Entries of the diagonal factor.
        tensor<T, 1> m_d;
    };

    /**
     * @brief Partial specialization of ldl_result for complex types.
     */
    template <class T>
    class ldl_result< std::complex<T> > {
    public:
        /// Constructors.

        ldl_result() : m_l(), m_d() {}

        ldl_result(const ldl_result &other) : m_l(other.m_l), m_d(other.m_d) {}

        ldl_result(ldl_result &&other)
         : m_l(std::move(other.m_l)), m_d(std::move(other.m_d)) {}

        ldl_result(tensor<T, 2> &&l, tensor<T, 1> &&d)
         : m_l(std::move(l)), m_d(std::move(d)) {}

        /// Destructor.

        ~ldl_result() = default;

        /// Assignment operator.

        ldl_result& operator=(const ldl_result &other) {
            m_l = other.m_l;
            m_d = other.m_d;
            return *this;
        }

        ldl_result& operator=(ldl_result &&other) {
            m_l = std::move(other.m_l);
            m_d = std::move(other.m_d);
            return *this;
        }

        /// Public methods.

        template <class TagL, class TagD>
        void tie(
            base_tensor<std::complex<T>, 2, TagL> &L,
            base_tensor<std::complex<T>, 2, TagD> &D
        ) const {
            L = std::move(this->L());
            D = std::move(this->D());
        }

        const tensor<std::complex<T>, 2>& L() const;

        tensor<std::complex<T>, 2> D() const;

        const tensor<std::complex<T>, 1>& d() const;

        tensor<std::complex<T>, 2> A() const;

    private:
        // Lower triangular factor.
        tensor<std::complex<T>, 2> m_l;

        // Entries of the diagonal factor.
        tensor<std::complex<T>, 1> m_d;
    };

    template <class T>
    const tensor<T, 2>& ldl_result<T>::L() const {
        return m_l;
    }

    template <class T>
    const tensor<std::complex<T>, 2>& ldl_result< std::complex<T> >::L() const {
        return m_l;
    }

    template <class T>
    tensor<T, 2> ldl_result<T>::D() const {
        size_t n = m_d.size();
        tensor<T, 2> out({n, n}, T());
        for (size_t i = 0; i < n; ++i) {
            out(i, i) = m_d[i];
        }
        return out;
    }

    template <class T>
    tensor<std::complex<T>, 2> ldl_result< std::complex<T> >::D() const {
        size_t n = m_d.size();
        tensor<std::complex<T>, 2> out({n, n}, T());
        for (size_t i = 0; i < n; ++i) {
            out(i, i) = m_d[i];
        }
        return out;
    }

    template <class T>
    const tensor<T, 1>& ldl_result<T>::d() const {
        return m_d;
    }

    template <class T>
    const tensor<std::complex<T>, 1>& ldl_result< std::complex<T> >::d() const {
        return m_d;
    }

    template <class T>
    tensor<T, 2> ldl_result<T>::A() const {
        size_t n = m_l.shape(0);
        tensor<T, 2> out(n, n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T val = T();
                for (size_t k = 0; k <= i && k <= j; ++k) {
                    val += m_l(i, k) * m_d[k] * m_l(j, k);
                }
                out(i, j) = val;
            }
        }
        return out;
    }

    template <class T>
    tensor<std::complex<T>, 2> ldl_result< std::complex<T> >::A() const {
        size_t n = m_l.shape(0);
        tensor<std::complex<T>, 2> out(n, n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T val = T();
                for (size_t k = 0; k <= i && k <= j; ++k) {
                    val += m_l(i, k) * m_d[k] * std::conj(m_l(j, k));
                }
                out(i, j) = val;
            }
        }
        return out;
    }

    /**
     * @brief Class returned by linalg::cholesky.
     *
     * @tparam T Type of the elements contained in the matrix. This shall be a
     *     floating-point type or a complex type.
     */
    template <class T>
    class cho_result {
    public:
        /// Constructors.

        /**
         * @brief Default constructor.
         */
        cho_result() : m_l() {}

        /**
         * @brief Copy constructor.
         */
        cho_result(const cho_result &other) : m_l(other.m_l) {}

        /**
         * @brief Move constructor.
         */
        cho_result(cho_result &&other) : m_l(std::move(other.m_l)) {}

        /**
         * @brief Constructor reseved for linalg::cholesky.
         */
        cho_result(tensor<T, 2> &&l) : m_l(std::move(l)) {}

        /// Destructor.

        ~cho_result() = default;

        /// Assignment operator.

        /**
         * @brief Copy assignment.
         */
        cho_result& operator=(const cho_result &other) {
            m_l = other.m_l;
            return *this;
        }

        /**
         * @brief Move assignment.
         */
        cho_result& operator=(cho_result &&other) {
            m_l = std::move(other.m_l);
            return *this;
        }

        /// Public methods.

        /**
         * @brief Unpack L into the destination matrix.
         *
         * @param L A location into which the lower triangular matrix is
         *     stored.
         */
        template <class TagL>
        void tie(base_tensor<T, 2, TagL> &L) const {
            L = std::move(this->L());
        }

        /**
         * @brief Return the lower triangular matrix of the decomposition.
         */
        const tensor<T, 2>& L() const;

        /**
         * @brief Return the decomposed matrix.
         */
        tensor<T, 2> A() const;

    private:
        // Lower triangular factor.
        tensor<T, 2> m_l;
    };

    /**
     * @brief Partial specialization of cho_result for complex types.
     */
    template <class T>
    class cho_result< std::complex<T> > {
    public:
        /// Constructors.

        cho_result() : m_l() {}

        cho_result(const cho_result &other) : m_l(other.m_l) {}

        cho_result(cho_result &&other) : m_l(std::move(other.m_l)) {}

        cho_result(tensor<std::complex<T>, 2> &&l) : m_l(std::move(l)) {}

        /// Destructor.

        ~cho_result() = default;

        /// Assignment operator.

        cho_result& operator=(const cho_result &other) {
            m_l = other.m_l;
            return *this;
        }

        cho_result& operator=(cho_result &&other) {
            m_l = std::move(other.m_l);
            return *this;
        }

        /// Public methods.

        template <class TagL>
        void tie(base_tensor<std::complex<T>, 2, TagL> &L) const {
            L = std::move(this->L());
        }

        const tensor<std::complex<T>, 2>& L() const;

        tensor<std::complex<T>, 2> A() const;

    private:
        // Lower triangular factor.
        tensor<std::complex<T>, 2> m_l;
    };

    template <class T>
    inline const tensor<T, 2>& cho_result<T>::L() const {
        return m_l;
    }

    template <class T>
    inline const tensor<std::complex<T>, 2>&
    cho_result< std::complex<T> >::L() const {
        return m_l;
    }

    template <class T>
    tensor<T, 2> cho_result<T>::A() const {
        size_t n = m_l.shape(0);
        tensor<T, 2> out(n, n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T val = T();
                for (size_t k = 0; k <= i && k <= j; ++k) {
                    val += m_l(i, k) * m_l(j, k);
                }
                out(i, j) = val;
            }
        }
        return out;
    }

    template <class T>
    tensor<std::complex<T>, 2> cho_result< std::complex<T> >::A() const {
        size_t n = m_l.shape(0);
        tensor<std::complex<T>, 2> out(n, n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                std::complex<T> val = T();
                for (size_t k = 0; k <= i && k <= j; ++k) {
                    val += m_l(i, k) * std::conj(m_l(j, k));
                }
                out(i, j) = val;
            }
        }
        return out;
    }
}
}

#endif // NUMCPP_LU_FACTOR_H_INCLUDED
