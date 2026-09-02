/*
 * File: include/numcpp/tensor/shape.tcc
 * Repository: https://github.com/vdae2304/Metodos-Numericos
 * 
 * Copyright (C) 2026 vdae2304
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NUMCPP_SHAPE_TCC_INCLUDED
#define NUMCPP_SHAPE_TCC_INCLUDED

#include <algorithm>
#include "numcpp/utilities/assert.h"

namespace numcpp {
template <size_t Rank>
void __broadcast_shapes_impl(shape_t<Rank>&) {}

/**
 * @brief Broadcast input shapes into a common shape.
 */
template <size_t Rank, size_t... Ranks>
void __broadcast_shapes_impl(shape_t<Rank>& out, const shape_t<Rank>& shape1,
                             const shape_t<Ranks>&... shapes) {
  for (size_t i = 0; i < Rank; ++i) {
    if (out[i] == 1) {
      out[i] = shape1[i];
    } else if (shape1[i] != 1 && shape1[i] != out[i]) {
      std::ostringstream error;
      error << "operands could not be broadcast together with shapes " << out
            << " " << shape1;
      throw std::invalid_argument(error.str());
    }
  }
  __broadcast_shapes_impl(out, shapes...);
}

template <size_t Rank, size_t... Ranks>
shape_t<Rank> broadcast_shapes(const shape_t<Rank>& shape1,
                               const shape_t<Ranks>&... shapes) {
  shape_t<Rank> out = shape1;
  __broadcast_shapes_impl(out, shapes...);
  return out;
}

template <class T>
void __shape_cat_impl(T*) {}

/**
 * @brief Concatenate one or more shapes.
 */
template <class T, size_t Rank, size_t... Ranks>
void __shape_cat_impl(T* out, const basic_shape<T, Rank> &shape1,
                      const basic_shape<T, Ranks> &...shapes) {
  out = std::copy(shape1.begin(), shape1.end(), out);
  __shape_cat_impl(out, shapes...);
}

template <class T, size_t Rank, size_t... Ranks>
basic_shape<T, (Rank + ... + Ranks)> shape_cat(
    const basic_shape<T, Rank>& shape1,
    const basic_shape<T, Ranks>&... shapes) {
  basic_shape<T, (Rank + ... + Ranks)> out;
  __shape_cat_impl(out.data(), shape1, shapes...);
  return out;
}

template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits>& operator>>(
    std::basic_istream<charT, traits>& istr, basic_shape<T, Rank>& shape) {
  charT ch;
  char delimiter = '(';
  for (size_t i = 0; i < Rank; ++i) {
    if (!(istr >> ch)) {
      return istr;
    }
    if (!traits::eq(ch, istr.widen(delimiter))) {
      istr.putback(ch);
      istr.setstate(std::ios_base::failbit);
      return istr;
    }
    if (!(istr >> shape[i])) {
      return istr;
    }
    delimiter = ',';
  }
  if (!(istr >> ch)) {
    return istr;
  }
  if (!traits::eq(ch, istr.widen(')'))) {
    istr.putback(ch);
    istr.setstate(std::ios_base::failbit);
    return istr;
  }
  return istr;
}

template <class charT, class traits, class T>
std::basic_istream<charT, traits>& operator>>(
    std::basic_istream<charT, traits>& istr, basic_shape<T, 1>& shape) {
  charT ch;
  if (!(istr >> ch)) {
    return istr;
  }
  if (traits::eq(ch, istr.widen('('))) {
    if (!(istr >> shape[0])) {
      return istr;
    }
    for (char next : std::string_view(",)")) {
      if (!(istr >> ch)) {
        return istr;
      }
      if (!traits::eq(ch, istr.widen(next))) {
        istr.putback(ch);
        istr.setstate(std::ios_base::failbit);
        return istr;
      }
    }
  } else {
    istr.putback(ch);
    istr >> shape[0];
  }
  return istr;
}

template <class charT, class traits, class T, size_t Rank>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits>& ostr,
    const basic_shape<T, Rank>& shape) {
  std::basic_stringstream<charT, traits> sstr;
  sstr.flags(ostr.flags());
  sstr.imbue(ostr.getloc());
  sstr << "(" << shape[0];
  for (size_t i = 1; i < Rank; ++i) {
    sstr << ", " << shape[i];
  }
  sstr << ")";
  return ostr << sstr.str();
}

template <class charT, class traits, class T>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits>& ostr, const basic_shape<T, 1>& shape) {
  std::basic_stringstream<charT, traits> sstr;
  sstr.flags(ostr.flags());
  sstr.imbue(ostr.getloc());
  sstr << "(" << shape[0] << ",)";
  return ostr << sstr.str();
}
} // namespace numcpp

#endif // NUMCPP_SHAPE_TCC_INCLUDED
