#ifndef GAM200_INSIGHT_ENGINE_SOURCE_STL_VECTOR_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_STL_VECTOR_H_
#include "Engine/Core/Core.h"

namespace IS {
    template <typename T>
    class IS_API IS_Vector
    {
    public:
        using size_type = size_t;
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        IS_Vector(); // default ctor
        explicit IS_Vector(size_type n);
        IS_Vector(std::initializer_list<value_type> rhs); // non-default, conversion ctor
        IS_Vector(IS_Vector const& rhs);                     // copy ctor
        ~IS_Vector();                                     // destructor

        IS_Vector& operator=(IS_Vector const&);                        // copy assignment operator for assignment with IS_Vector object
        IS_Vector& operator=(std::initializer_list<value_type> rhs); // copy assignment operator for std::initializer_list<value_type>

        reference operator[](size_type index);             // overload subscript operator
        const_reference operator[](size_type index) const; // same as abv for const

        /**************************************************************************/
        /*!
          \brief
            appends a new element at the back of the IS_Vector

          \param value
            the num to pushback
        */
        /**************************************************************************/
        void push_back(value_type value);
        /**************************************************************************/
        /*!
          \brief
            reserve space for new elements

          \param newsize
            how many space to be reserved
        */
        /**************************************************************************/
        void reserve(size_type newsize);
        /**************************************************************************/
        /*!
          \brief
            resizes the obj according to newsize

          \param newsize
            resizes the IS_Vector with this var
        */
        /**************************************************************************/
        void resize(size_type newsize);

        bool empty() const;         // is container empty?
        size_type size() const;     // what is size?
        size_type capacity() const; // what is space?

        size_type allocations() const; // how many allocations or "growths"?

        // iterators
        pointer begin(); // pointer to first element
        pointer end();   // pointer to one past last element
        const_pointer begin() const;
        const_pointer end() const;
        const_pointer cbegin() const;
        const_pointer cend() const;

        void swap(IS_Vector& rhs);
        void pop_back();

    private:
        size_type sz;     // the number of elements in the array
        size_type space;  // the allocated size (in terms of elements) of the array
        size_type allocs; // number of times space has been updated
        pointer data;     // the dynamically allocated array
    };
}

#endif //GAM200_INSIGHT_ENGINE_SOURCE_STL_VECTOR_H_