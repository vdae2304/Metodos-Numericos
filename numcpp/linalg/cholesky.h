/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

/** @file include/numcpp/linalg/cholesky.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/linalg.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_CHOLESKY_H_INCLUDED
#define NUMCPP_CHOLESKY_H_INCLUDED

namespace numcpp {
namespace linalg {
/**
 * @brief Class returned by @c linalg::ldl.
 *
 * @tparam T Type of the elements contained in the matrix. This shall be a
 *           floating-point type or a complex type.
 */
template <class T> class ldl_result {
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
   * @brief Constructor used by @c linalg::ldl.
   */
  ldl_result(tensor<T, 2> &&l, tensor<T, 1> &&d)
      : m_l(std::move(l)), m_d(std::move(d)) {}

  /// Destructor.

  ~ldl_result() = default;

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   */
  ldl_result &operator=(const ldl_result &other) {
    m_l = other.m_l;
    m_d = other.m_d;
    return *this;
  }

  /**
   * @brief Move assignment.
   */
  ldl_result &operator=(ldl_result &&other) {
    m_l = std::move(other.m_l);
    m_d = std::move(other.m_d);
    return *this;
  }

  /// Public methods.

  /**
   * @brief Unpack L and D into the destination matrices.
   *
   * @param L A location into which the lower triangular matrix is stored.
   * @param D A location into which the diagonal matrix is stored.
   */
  template <class TagL, class TagD>
  void tie(base_tensor<T, 2, TagL> &L, base_tensor<T, 2, TagD> &D) const {
    L = this->L();
    D = this->D();
  }

  /**
   * @brief Return the lower triangular matrix of the decomposition.
   */
  const tensor<T, 2> &L() const;

  /**
   * @brief Return the diagonal matrix of the decomposition.
   */
  tensor<T, 2> D() const;

  /**
   * @brief Return the diagonal of the diagonal matrix of the decomposition.
   */
  const tensor<T, 1> &d() const;

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
template <class T> class ldl_result<std::complex<T>> {
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

  ldl_result &operator=(const ldl_result &other) {
    m_l = other.m_l;
    m_d = other.m_d;
    return *this;
  }

  ldl_result &operator=(ldl_result &&other) {
    m_l = std::move(other.m_l);
    m_d = std::move(other.m_d);
    return *this;
  }

  /// Public methods.

  template <class TagL, class TagD>
  void tie(base_tensor<std::complex<T>, 2, TagL> &L,
           base_tensor<std::complex<T>, 2, TagD> &D) const {
    L = this->L();
    D = this->D();
  }

  const tensor<std::complex<T>, 2> &L() const;

  tensor<std::complex<T>, 2> D() const;

  const tensor<std::complex<T>, 1> &d() const;

  tensor<std::complex<T>, 2> A() const;

private:
  // Lower triangular factor.
  tensor<std::complex<T>, 2> m_l;

  // Entries of the diagonal factor.
  tensor<std::complex<T>, 1> m_d;
};

template <class T> const tensor<T, 2> &ldl_result<T>::L() const { return m_l; }

template <class T>
const tensor<std::complex<T>, 2> &ldl_result<std::complex<T>>::L() const {
  return m_l;
}

template <class T> tensor<T, 2> ldl_result<T>::D() const {
  size_t n = m_d.size();
  tensor<T, 2> out({n, n}, T());
  for (size_t i = 0; i < n; ++i) {
    out(i, i) = m_d[i];
  }
  return out;
}

template <class T>
tensor<std::complex<T>, 2> ldl_result<std::complex<T>>::D() const {
  size_t n = m_d.size();
  tensor<std::complex<T>, 2> out({n, n}, T());
  for (size_t i = 0; i < n; ++i) {
    out(i, i) = m_d[i];
  }
  return out;
}

template <class T> const tensor<T, 1> &ldl_result<T>::d() const { return m_d; }

template <class T>
const tensor<std::complex<T>, 1> &ldl_result<std::complex<T>>::d() const {
  return m_d;
}

template <class T> tensor<T, 2> ldl_result<T>::A() const {
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
tensor<std::complex<T>, 2> ldl_result<std::complex<T>>::A() const {
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
 * @brief Class returned by @c linalg::cholesky.
 *
 * @tparam T Type of the elements contained in the matrix. This shall be a
 *           floating-point type or a complex type.
 */
template <class T> class cho_result {
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
  cho_result &operator=(const cho_result &other) {
    m_l = other.m_l;
    return *this;
  }

  /**
   * @brief Move assignment.
   */
  cho_result &operator=(cho_result &&other) {
    m_l = std::move(other.m_l);
    return *this;
  }

  /// Public methods.

  /**
   * @brief Unpack L into the destination matrix.
   *
   * @param L A location into which the lower triangular matrix is stored.
   */
  template <class TagL> void tie(base_tensor<T, 2, TagL> &L) const {
    L = this->L();
  }

  /**
   * @brief Return the lower triangular matrix of the decomposition.
   */
  const tensor<T, 2> &L() const;

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
template <class T> class cho_result<std::complex<T>> {
public:
  /// Constructors.

  cho_result() : m_l() {}

  cho_result(const cho_result &other) : m_l(other.m_l) {}

  cho_result(cho_result &&other) : m_l(std::move(other.m_l)) {}

  cho_result(tensor<std::complex<T>, 2> &&l) : m_l(std::move(l)) {}

  /// Destructor.

  ~cho_result() = default;

  /// Assignment operator.

  cho_result &operator=(const cho_result &other) {
    m_l = other.m_l;
    return *this;
  }

  cho_result &operator=(cho_result &&other) {
    m_l = std::move(other.m_l);
    return *this;
  }

  /// Public methods.

  template <class TagL>
  void tie(base_tensor<std::complex<T>, 2, TagL> &L) const {
    L = this->L();
  }

  const tensor<std::complex<T>, 2> &L() const;

  tensor<std::complex<T>, 2> A() const;

private:
  // Lower triangular factor.
  tensor<std::complex<T>, 2> m_l;
};

template <class T> inline const tensor<T, 2> &cho_result<T>::L() const {
  return m_l;
}

template <class T>
inline const tensor<std::complex<T>, 2> &
cho_result<std::complex<T>>::L() const {
  return m_l;
}

template <class T> tensor<T, 2> cho_result<T>::A() const {
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
tensor<std::complex<T>, 2> cho_result<std::complex<T>>::A() const {
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
} // namespace linalg
} // namespace numcpp

#endif // NUMCPP_CHOLESKY_H_INCLUDED
