//pch has to go to the top of every cpp
#include "Pch.h"

namespace IS {
    template <typename T>
    IS_Vector<T>::IS_Vector() : sz{ 0 }, space{ 0 }, allocs{ 0 }, data{ nullptr } {}                         // default ctor

    template <typename T>
    IS_Vector<T>::IS_Vector(size_type n) : sz{ n }, space{ n }, allocs{ 1 }, data{ new value_type[n]{0} } {} // Single-argument conversion constructor that allocates memory for n elements with each element initialized to 0

    template <typename T>
    IS_Vector<T>::IS_Vector(std::initializer_list<T> rhs) : sz{ rhs.size() }, space{ rhs.size() }, allocs{ 1 }, data{ new value_type[rhs.size()] }
    { // Another single-argument conversion constructor that allocates memory
      // to store values in an initializer_list<int>
        std::copy(rhs.begin(), rhs.end(), data); // deep copy
    }

    template <typename T>
    IS_Vector<T>::IS_Vector(IS_Vector<T> const& rhs) : sz{ rhs.sz }, space{ rhs.sz }, allocs{ 1 }, data{ new value_type[rhs.sz] }
    {                                               // copy ctor
        std::copy(rhs.data, rhs.data + rhs.sz, data); // deep copy
    }

    template <typename T>
    IS_Vector<T>::~IS_Vector()
    { // destructor
        delete[] data;
    }

    template <typename T>
    IS_Vector<T>& IS_Vector<T>::operator=(IS_Vector<T> const& rhs)
    { // copy assignment operator for assignment with IS_Vector object
        space = rhs.space;
        resize(rhs.sz);
        pointer temp = new value_type[rhs.sz];
        for (size_type i{ 0 }; i < rhs.sz; ++i)
        {
            temp[i] = rhs.data[i];
        }
        delete[] data;
        data = temp;
        space = rhs.sz;
        ++allocs;
        return *this;
    }

    template <typename T>
    IS_Vector<T>& IS_Vector<T>::operator=(std::initializer_list<T> rhs)
    { // copy assignment operator for std::initializer_list<T>
        sz = rhs.size();
        space = rhs.size();
        allocs = 1;
        delete[] data;
        data = new value_type[rhs.size()];
        std::copy(rhs.begin(), rhs.end(), data); // deep copy
        return *this;
    }

    template <typename T>
    typename IS_Vector<T>::reference IS_Vector<T>::operator[](size_type index)
    { // overload subscript operator
        return data[index];
    }

    template <typename T>
    typename IS_Vector<T>::const_reference IS_Vector<T>::operator[](size_type index) const
    { // same as abv for const
        return data[index];
    }

    /**************************************************************************/
    /*!
      \brief
        appends a new element at the back of the IS_Vector

      \param value
        the num to pushback
    */
    /**************************************************************************/
    template <typename T>
    void IS_Vector<T>::push_back(value_type value)
    {
        if (space == 0)
        {
            reserve(1);
        }
        else if (sz == space)
        {
            reserve(2 * space);
        }
        data[sz] = value;
        ++sz;
    }

    /**************************************************************************/
    /*!
      \brief
        reserve space for new elements

      \param newsize
        how many space to be reserved
    */
    /**************************************************************************/
    template <typename T>
    void IS_Vector<T>::reserve(size_type newsize)
    {
        if (newsize <= space)
        {
            return;
        }

        pointer temp = new value_type[newsize];
        for (size_t i = 0; i < sz; i++)
        {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
        space = newsize;
        ++allocs;
    }

    /**************************************************************************/
    /*!
      \brief
        resizes the obj according to newsize

      \param newsize
        resizes the IS_Vector with this var
    */
    /**************************************************************************/
    template <typename T>
    void IS_Vector<T>::resize(size_type newsize)
    {
        if (newsize > space)
        {
            reserve(newsize);
        }
        if (newsize > sz && newsize <= space)
        {
            sz = newsize;
        }
        if (newsize < sz)
        {
            sz = newsize;
        }
    }

    template <typename T>
    bool IS_Vector<T>::empty() const
    {
        bool flag = (sz == 0) ? true : false;
        return flag;
    }

    template <typename T>
    typename IS_Vector<T>::size_type IS_Vector<T>::size() const
    {
        return this->sz;
    }

    template <typename T>
    typename IS_Vector<T>::size_type IS_Vector<T>::capacity() const
    {
        return space;
    }

    template <typename T>
    typename IS_Vector<T>::size_type IS_Vector<T>::allocations() const
    {
        return allocs;
    }

    template <typename T>
    typename IS_Vector<T>::pointer IS_Vector<T>::begin()
    {
        return data;
    }

    template <typename T>
    typename IS_Vector<T>::pointer IS_Vector<T>::end()
    {
        return (data + sz);
    }

    template <typename T>
    typename IS_Vector<T>::const_pointer IS_Vector<T>::begin() const
    {
        return data;
    }

    template <typename T>
    typename IS_Vector<T>::const_pointer IS_Vector<T>::end() const
    {
        return (data + sz);
    }

    template <typename T>
    typename IS_Vector<T>::const_pointer IS_Vector<T>::cbegin() const
    {
        return data;
    }

    template <typename T>
    typename IS_Vector<T>::const_pointer IS_Vector<T>::cend() const
    {
        return (data + sz);
    }

    template <typename T>
    void IS_Vector<T>::swap(IS_Vector<T>& rhs)
    {
        std::swap(sz, rhs.sz);
        std::swap(space, rhs.space);
        std::swap(data, rhs.data);
        std::swap(allocs, rhs.allocs);
    }

    template <typename T>
    void IS_Vector<T>::pop_back()
    {
        if (sz > 0)
        {
            --sz;
        }
    }
}