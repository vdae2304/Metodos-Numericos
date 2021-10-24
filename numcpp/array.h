#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include <cstddef>
#include <initializer_list>

// Arrays are fixed-size sequence containers: they hold a specific number of
// elements ordered in a strict linear sequence.

namespace numcpp {
    template <class T>
    class array {
    protected:
        size_t length;
        T *values;

    public:
        typedef size_t size_type;
        typedef T value_type;

        // Default constructor. Constructs an empty array with no elements.
        array();

        // Size constructor. Constructs an array with n elements.
        array(size_t n);

        // Fill constructor. Constructs an array with n elements, each
        // initialized to val.
        array(size_t n, const T &val);

        // Range constructor. Constructs an array with as many elements as the
        // range [first, last), with each element constructed from its
        // corresponding element in that range, in the same order.
        template <class InputIterator>
        array(InputIterator first, InputIterator last);

        // Copy constructor. Constructs an array with a copy of each of the
        // elements in v, in the same order.
        array(const array &v);

        template <class U>
        array(const array<U> &v);

        // Move constructor. Constructs an array that acquires the elements of
        // v.
        array(array &&v);

        // Subarray constructor. Constructs an array with a copy of each of the 
        // elements in v, in the same order.
        array(const subarray<T> &v);

        // Initializer list. Constructs an array with a copy of each of the
        // elements in il, in the same order.
        array(std::initializer_list<T> il);

        // Destructor. Destroys the array.
        ~array();

        // Copy assignment. Assigns the contents of v to *this after resizing
        // the object (if necessary).
        array& operator= (const array &v);

        template <class U>
        array& operator= (const array<U> &v);

        // Move assignment. Acquires the contents of v.
        array& operator= (array &&v);

        // Subarray assignment. Assigns the contents of v to *this after 
        // resizing the object (if necessary).
        array& operator= (const subarray<T> &v);

        // Fill assignment. Assigns val to every element.
        array& operator= (const T &val);

        // Compound assignments.
        array& operator+= (const array &v);

        array& operator-= (const array &v);

        array& operator*= (const array &v);

        array& operator/= (const array &v);

        array& operator%= (const array &v);

        array& operator&= (const array &v);

        array& operator|= (const array &v);

        array& operator^= (const array &v);

        array& operator<<= (const array &v);

        array& operator>>= (const array &v);

        array& operator+= (const T &val);

        array& operator-= (const T &val);

        array& operator*= (const T &val);

        array& operator/= (const T &val);

        array& operator%= (const T &val);

        array& operator&= (const T &val);

        array& operator|= (const T &val);

        array& operator^= (const T &val);

        array& operator<<= (const T &val);

        array& operator>>= (const T &val);

        // Array subscript. Returns a reference to the element at position i in
        // the array.
        T& operator[] (size_t i);

        const T& operator[] (size_t i) const;

        // Slice indexing: Return a sub-array object that selects the elements
        // specified by its argument.
        slice_subarray<T> operator[] (slice slc);

        const slice_subarray<T> operator[] (slice slc) const;

        // Integer array indexing: Return a sub-array object that selects the
        // elements specified by its argument.
        index_subarray<T> operator[] (const array<size_t> &index);

        const index_subarray<T> operator[] (const array<size_t> &index) const;

        // Apply a function to each of the elements in *this.
        template <class Function = T(T)>
        void apply(Function f);

        // Return the index of the maximum value.
        size_t argmax() const;

        // Return the index of the minimum value.
        size_t argmin() const;

        // Returns the indices that would sort this array.
        array<size_t> argsort() const;

        // Clip (limit) the values in the array. Given an interval, values
        // outside the interval are clipped to the interval edges.
        void clip(const T &a_min, const T &a_max);

        // Return the cumulative product of the elements.
        array cumprod() const;

        // Return the cumulative sum of the elements.
        array cumsum() const;

        // Returns a pointer to the memory array used internally by the array.
        T* data();

        const T* data() const;

        // Return the dot product of two arrays.
        T dot(const array &v) const;

        // Return the matrix multiplication of a row vector and a matrix.
        array dot(const matrix<T> &A) const;

        // Returns the maximum value contained in the array.
        T max() const;

        // Returns the average of the array elements.
        T mean() const;

        // Returns the minimum value contained in the array.
        T min() const;

        // Return the product of the array elements.
        T prod() const;

        // Resizes the array, changing its size to n elements.
        // If n is smaller than the current size, the content is reduced to its
        // first n elements, removing those beyond.
        // If n is greater than the current size, the content is expanded by
        // inserting at the end as many elements as needed to reach a size of n.
        void resize(size_t n, const T &val = T());

        // Returns the number of elements in the array.
        size_t size() const;

        // Sort an array in-place.
        void sort();

        // Returns the standard deviation of the array elements.
        T stddev(size_t ddof = 0) const;

        // Return the sum of the array elements.
        T sum() const;

        // Swap contents with v.
        void swap(array &v);

        // Returns the variance of the array elements.
        T var(size_t ddof = 0) const;
    };

    // Unary operators.
    template <class T>
    array<T> operator+ (const array<T> &v);

    template <class T>
    array<T> operator- (const array<T> &v);

    template <class T>
    array<T> operator~ (const array<T> &v);

    array<bool> operator! (const array<bool> &v);

    // Arithmetic operators.
    template <class T>
    array<T> operator+ (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator+ (const array<T> &v, const T &val);

    template <class T>
    array<T> operator+ (const T &val, const array<T> &v);

    template <class T>
    array<T> operator- (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator- (const array<T> &v, const T &val);

    template <class T>
    array<T> operator- (const T &val, const array<T> &v);

    template <class T>
    array<T> operator* (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator* (const array<T> &v, const T &val);

    template <class T>
    array<T> operator* (const T &val, const array<T> &v);

    template <class T>
    array<T> operator/ (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator/ (const array<T> &v, const T &val);

    template <class T>
    array<T> operator/ (const T &val, const array<T> &v);

    template <class T>
    array<T> operator% (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator% (const array<T> &v, const T &val);

    template <class T>
    array<T> operator% (const T &val, const array<T> &v);

    // Bitwise operators.
    template <class T>
    array<T> operator& (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator& (const array<T> &v, const T &val);

    template <class T>
    array<T> operator& (const T &val, const array<T> &v);

    template <class T>
    array<T> operator| (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator| (const array<T> &v, const T &val);

    template <class T>
    array<T> operator| (const T &val, const array<T> &v);

    template <class T>
    array<T> operator^ (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator^ (const array<T> &v, const T &val);

    template <class T>
    array<T> operator^ (const T &val, const array<T> &v);

    template <class T>
    array<T> operator<< (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator<< (const array<T> &v, const T &val);

    template <class T>
    array<T> operator<< (const T &val, const array<T> &v);

    template <class T>
    array<T> operator>> (const array<T> &v, const array<T> &w);

    template <class T>
    array<T> operator>> (const array<T> &v, const T &val);

    template <class T>
    array<T> operator>> (const T &val, const array<T> &v);

    // Logical operators.
    array<bool> operator&& (const array<bool> &v, const array<bool> &w);

    array<bool> operator&& (const array<bool> &v, bool val);

    array<bool> operator&& (bool val, const array<bool> &v);

    array<bool> operator|| (const array<bool> &v, const array<bool> &w);

    array<bool> operator|| (const array<bool> &v, bool val);

    array<bool> operator|| (bool val, const array<bool> &v);

    // Relational operators.
    template <class T>
    array<bool> operator== (const array<T> &v, const array<T> &w);

    template <class T>
    array<bool> operator== (const array<T> &v, const T &val);

    template <class T>
    array<bool> operator== (const T &val, const array<T> &v);

    template <class T>
    array<bool> operator!= (const array<T> &v, const array<T> &w);

    template <class T>
    array<bool> operator!= (const array<T> &v, const T &val);

    template <class T>
    array<bool> operator!= (const T &val, const array<T> &v);

    template <class T>
    array<bool> operator< (const array<T> &v, const array<T> &w);

    template <class T>
    array<bool> operator< (const array<T> &v, const T &val);

    template <class T>
    array<bool> operator< (const T &val, const array<T> &v);

    template <class T>
    array<bool> operator> (const array<T> &v, const array<T> &w);

    template <class T>
    array<bool> operator> (const array<T> &v, const T &val);

    template <class T>
    array<bool> operator> (const T &val, const array<T> &v);

    template <class T>
    array<bool> operator<= (const array<T> &v, const array<T> &w);

    template <class T>
    array<bool> operator<= (const array<T> &v, const T &val);

    template <class T>
    array<bool> operator<= (const T &val, const array<T> &v);

    template <class T>
    array<bool> operator>= (const array<T> &v, const array<T> &w);

    template <class T>
    array<bool> operator>= (const array<T> &v, const T &val);

    template <class T>
    array<bool> operator>= (const T &val, const array<T> &v);
}

#endif // ARRAY_H_INCLUDED
