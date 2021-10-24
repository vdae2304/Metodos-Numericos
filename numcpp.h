#ifndef NUMCPP_H_INCLUDED
#define NUMCPP_H_INCLUDED

namespace numcpp {
    template <class T> class array;
    template <class T> class matrix;

    class slice;
    template <class T> class subarray;
    template <class T> class slice_subarray;
    template <class T> class index_subarray;
    
    template <class T> class submatrix;
    template <class T> class slice_slice_submatrix;
    template <class T> class slice_index_submatrix;
    template <class T> class index_slice_submatrix;
    template <class T> class index_index_submatrix;
}

#include "numcpp/array.h"
#include "numcpp/matrix.h"

#include "numcpp/slice.h"
#include "numcpp/subarray.h"
#include "numcpp/submatrix.h"

#include "numcpp/io.h"
#include "numcpp/routines.h"
#include "numcpp/math.h"

#include "numcpp/array.cpp"
#include "numcpp/matrix.cpp"

#include "numcpp/slice.cpp"
#include "numcpp/subarray.cpp"
#include "numcpp/submatrix.cpp"

#include "numcpp/io.cpp"
#include "numcpp/routines.cpp"

#endif // NUMCPP_H_INCLUDED
