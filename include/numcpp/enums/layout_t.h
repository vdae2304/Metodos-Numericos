/*
 * File: include/numcpp/enum/layout_t.h
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

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAYOUT_T_H_INCLUDED
#define NUMCPP_LAYOUT_T_H_INCLUDED

namespace numcpp {
/**
 * @brief Layout in which elements are stored or iterated.
 */
enum layout_t {
  /**
   * @brief Right layout. The last dimension is contiguous, and the memory
   * offset of each axis is a constant multiple of the following axis. Default
   * style in C/C++.
   */
  layout_right = 1,

  /**
   * @brief Left layout. The first dimension is contiguous, and the memory
   * offset of each axis is a constant multiple of the previous axis. Default
   * style in Fortran/Matlab.
   */
  layout_left = 2,

  /**
   * @brief Default layout.
   */
  default_layout = layout_right
};

inline layout_t operator&(layout_t layout1, layout_t layout2) {
  return static_cast<layout_t>(static_cast<int>(layout1) &
                               static_cast<int>(layout2));
}

inline layout_t operator|(layout_t layout1, layout_t layout2) {
  return static_cast<layout_t>(static_cast<int>(layout1) |
                               static_cast<int>(layout2));
}
}
#endif // NUMCPP_LAYOUT_T_H_INCLUDED
