#ifndef NUMCPP_H_INCLUDED
#define NUMCPP_H_INCLUDED

#include <initializer_list>

namespace numcpp {

    template <class T>
    class array {
        public:
            typedef T value_type;
            typedef unsigned int size_type;

        private:
            size_type n;
            T* data;

        public:
            array(size_type n) {
                this->n = n;
                this->data = new T[n];
            }

            array(size_type n, const T &val) {
                this->n = n;
                this->data = new T[n];
                for (size_type i = 0; i < n; ++i) {
                    this->data[i] = val;
                }
            }

            array(initializer_list<T> il) {
                this->n = il.size();
                this->data = new T[this->n];
                size_type i = 0;
                for (val : il) {
                    this->data[i++] = val;
                }
            }
    };

}

#endif // NUMCPP_H_INCLUDED
