#ifndef GAM200_INSIGHT_ENGINE_SOURCE_STL_ARRAY_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_STL_ARRAY_H_
#include "Engine/Core/Core.h"

namespace IS {
    template <typename T, size_t N>
    class IS_API IS_Array
    {

    public:
        using size_type = size_t;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;
        using pointer = T*;
        using const_pointer = const T*;

        IS_Array<T, N>(std::initializer_list<T> rhs);
        IS_Array() : elements{} {}

    private:
        T elements[N];

    public:
        /*!***********************************************************************
         \brief
            returns a pointer to the first element
         \return
            T*
        *************************************************************************/
        iterator begin();

        /*!***********************************************************************
         \brief
            returns a read only pointer to the first element
         \return
            const T*
        *************************************************************************/
        const_iterator begin() const;

        /*!***********************************************************************
         \brief
            returns a pointer to the last element in a half-open range
         \return
            T*
        *************************************************************************/
        iterator end();

        /*!***********************************************************************
         \brief
            returns a read only pointer to the last element in a half-open range
         \return
            const T*
        *************************************************************************/
        const_iterator end() const;

        /*!***********************************************************************
         \brief
            returns a read only pointer to the first element
         \return
            const T*
        *************************************************************************/
        const_iterator cbegin() const;

        /*!***********************************************************************
         \brief
            returns a read only pointer to the last element in a half-open range
         \return
            const T*
        *************************************************************************/
        const_iterator cend() const;

        /*!***********************************************************************
         \brief
            returns a refrernce to the first element
         \return
            T&
        *************************************************************************/
        reference front();

        /*!***********************************************************************
         \brief
            returns a read only refrernce to the first element
         \return
            const T&
        *************************************************************************/
        const_reference front() const;

        /*!***********************************************************************
         \brief
            returns a refrernce to the last element
         \return
            T&
        *************************************************************************/
        reference back();

        /*!***********************************************************************
         \brief
            returns a read only refrernce to the last element
         \return
            const T&
        *************************************************************************/
        const_reference back() const;

        /*!***********************************************************************
         \brief
            overloads the subscript operator
         \param[size_type index]
            index of element accessed
         \return
            a refernce to the element specified by index
        *************************************************************************/
        reference operator[](size_type index);

        /*!***********************************************************************
         \brief
            overloads the subscript operator
         \param[size_type index]
            index of element accessed
         \return
            a const refernce to the element specified by index
        *************************************************************************/
        const_reference operator[](size_type index) const;

        /*!***********************************************************************
         \brief
            is array empty?
         \return
            true if empty
            else false
        *************************************************************************/
        bool empty() const;

        /*!***********************************************************************
         \brief
            returns a pointer to first element in array
         \return
            T*
        *************************************************************************/
        pointer data();

        /*!***********************************************************************
         \brief
            returns a const pointer to first element in array
         \return
            const T*
        *************************************************************************/
        const_pointer data() const;

        /*!***********************************************************************
         \brief
            capacity of array
         \return
            size_t
        *************************************************************************/
        size_type size() const;

        /*!***********************************************************************
         \brief
            fills the array with param
         \param [const T &value]
            value to be filled
        *************************************************************************/
        void fill(const T& value);

        /*!***********************************************************************
         \brief
            swap contents with param
         \param [IS_Array &arr]
            arr contents to be swaped with
        *************************************************************************/
        void swap(IS_Array& arr);
    };

    /*!***********************************************************************
    \brief
        swap contents with other array
    \param [&lhs]
        first array
    \param [&rhs]
        second array
    *************************************************************************/
    template <typename T, size_t N>
    void swap(IS_Array<T, N>& lhs, IS_Array<T, N>& rhs);

    /*!***********************************************************************
    \brief
        is array equal?
    \param [const &lhs]
        first array
    \param [const &rhs]
        second array
    \return
        true if equal
        else false
    *************************************************************************/
    template <typename T, size_t N>
    bool operator==(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs);

    /*!***********************************************************************
    \brief
        is array not equal?
    \param [const &lhs]
        first array
    \param [const &rhs]
        second array
    \return
        true if not equal
        else false
    *************************************************************************/
    template <typename T, size_t N>
    bool operator!=(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs);

    /*!***********************************************************************
    \brief
        is rhs less than lhs?
    \param [const &lhs]
        first array
    \param [const &rhs]
        second array
    \return
        true if is
        else false
    *************************************************************************/
    template <typename T, size_t N>
    bool operator>(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs);

    /*!***********************************************************************
    \brief
        is lhs less than rhs?
    \param [const &lhs]
        first array
    \param [const &rhs]
        second array
    \return
        true if is
        else false
    *************************************************************************/
    template <typename T, size_t N>
    bool operator<(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs);
}

#endif //GAM200_INSIGHT_ENGINE_SOURCE_STL_ARRAY_H_