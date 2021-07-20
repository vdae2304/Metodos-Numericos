#ifndef SLICE_H_INCLUDED
#define SLICE_H_INCLUDED

#include <cstddef>

// This class represents an array slice selector. It does not contain nor
// refers to any element - it only describes a selection of elements to be
// used as an index in array::operator[].

namespace numcpp {
    class slice {
    public:
        size_t start, stop;
        long step;

        slice();

        slice(size_t stop);

        slice(size_t start, size_t stop, long step = 1);
    };
}

#endif // SLICE_H_INCLUDED
