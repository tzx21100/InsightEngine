//pch has to go to the top of every cpp
#include "Pch.h"

namespace IS {
    template <typename T, size_t N>
    IS_Array<T, N>::IS_Array(std::initializer_list<T> rhs) {
        size_t count = 0;
        for (auto& ele : rhs) {
            if (count < N) {
                elements[count] = ele;
                count++;
            }
            else {
                break;
            }
        }
        for (size_t i = count; i < N; ++i) {
            elements[i] = T{};
        }
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::iterator IS_Array<T, N>::begin() {
        return elements;
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_iterator IS_Array<T, N>::begin() const {
        return elements;
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::iterator IS_Array<T, N>::end() {
        return (elements + N);
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_iterator IS_Array<T, N>::end() const {
        return (elements + N);
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_iterator IS_Array<T, N>::cbegin() const {
        return elements;
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_iterator IS_Array<T, N>::cend() const {
        return (elements + N);
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::reference IS_Array<T, N>::front() {
        return elements[0];
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_reference IS_Array<T, N>::front() const {
        return elements[0];
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::reference IS_Array<T, N>::back() {
        return elements[N - 1];
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_reference IS_Array<T, N>::back() const {
        return elements[N - 1];
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::reference IS_Array<T, N>::operator[](size_type index) {
        return elements[index];
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_reference IS_Array<T, N>::operator[](size_type index) const {
        return elements[index];
    }

    template <typename T, size_t N>
    bool IS_Array<T, N>::empty() const {
        return (N == 0) ? true : false;
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::pointer IS_Array<T, N>::data() {
        return elements;
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::const_pointer IS_Array<T, N>::data() const {
        return elements;
    }

    template <typename T, size_t N>
    typename IS_Array<T, N>::size_type IS_Array<T, N>::size() const {
        return N;
    }

    template <typename T, size_t N>
    void IS_Array<T, N>::fill(const T& value) {
        for (size_t i = 0; i < N; ++i) {
            elements[i] = value;
        }
    }

    template <typename T, size_t N>
    void IS_Array<T, N>::swap(IS_Array<T, N>& arr) {
        std::swap_ranges(begin(), end(), arr.begin());
    }

    template <typename T, size_t N>
    void swap(IS_Array<T, N>& lhs, IS_Array<T, N>& rhs) {
        lhs.swap(rhs);
    }

    template <typename T, size_t N>
    bool operator==(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs) {
        return (std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())) ? true : false;
    }

    template <typename T, size_t N>
    bool operator!=(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs) {
        return !(lhs == rhs);
    }

    template <typename T, size_t N>
    bool operator>(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs) {
        return (std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end())) ? true : false;
    }

    template <typename T, size_t N>
    bool operator<(const IS_Array<T, N>& lhs, const IS_Array<T, N>& rhs) {
        return (std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())) ? true : false;
    }
}