#ifndef SUBARRAY_H_INCLUDED
#define SUBARRAY_H_INCLUDED

#include <cstddef>

// Subarrays are intermediate types returned by array's subscript operator
// (operator[]). It references the elements in the array object that are
// selected by the subscript, and overloads the assignment and compound
// assignment operators, allowing direct access to the elements in the
// selection. The type is convertible to an array, producing a new object with
// copies of the referred elements.

namespace numcpp {
    template <class T>
    class subarray {
    protected:
        T* values;
        friend class array<T>;

    public:
        // Assignment operators.
        void operator= (const array<T> &v);
        void operator= (const T &val);

        // Compound assignment.
        void operator+= (const array<T> &v);
        void operator-= (const array<T> &v);
        void operator*= (const array<T> &v);
        void operator/= (const array<T> &v);
        void operator%= (const array<T> &v);
        void operator&= (const array<T> &v);
        void operator|= (const array<T> &v);
        void operator^= (const array<T> &v);
        void operator<<= (const array<T> &v);
        void operator>>= (const array<T> &v);

        void operator+= (const T &val);
        void operator-= (const T &val);
        void operator*= (const T &val);
        void operator/= (const T &val);
        void operator%= (const T &val);
        void operator&= (const T &val);
        void operator|= (const T &val);
        void operator^= (const T &val);
        void operator<<= (const T &val);
        void operator>>= (const T &val);

        // Subarray subscript. Returns a reference to the element at position 
        // i in the subarray.
        virtual T& operator[] (size_t i);

        virtual const T& operator[] (size_t i) const;

        // Methods.
        virtual size_t size() const;
    };

    template <class T>
    class slice_subarray : public subarray<T> {
    protected:
        slice slc;
        friend class array<T>;

    public:
        // Assignment operators.
        void operator= (const array<T> &v);
        void operator= (const T &val);
        
        // Subarray subscript. Returns a reference to the element at position 
        // i in the subarray.
        T& operator[] (size_t i);

        const T& operator[] (size_t i) const;

        // Methods.
        size_t size() const;
    };

    template <class T>
    class index_subarray : public subarray<T> {
    protected:
        array<size_t> index;
        friend class array<T>;
        friend class matrix<T>;

    public:
        // Assignment operators.
        void operator= (const array<T> &v);
        void operator= (const T &val);
        
        // Subarray subscript. Returns a reference to the element at position 
        // i in the subarray.
        T& operator[] (size_t i);

        const T& operator[] (size_t i) const;

        // Methods.
        size_t size() const;
    };
}

#endif // ARRAY_H_INCLUDED
