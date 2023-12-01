//pch has to go to the top of every cpp
#include "Pch.h"

namespace IS {
    IS_String::size_type IS_String::counter{ 0 };

    IS_String::IS_String() : len{ 0 }, data{ new char[len + 1] } {
    #ifdef DEBUG
            std::cout << __PRETTY_FUNCTION__ << "\n";
    #endif
        data[0] = '\0';
        ++counter;
    }

    IS_String::IS_String(IS_String::const_pointer rhs)
        : len{ std::strlen(rhs) }, data{ new char[len + 1] } {
    #ifdef DEBUG
            std::cout << __PRETTY_FUNCTION__ << rhs << "\n";
    #endif
        IS_String::strcpy(data, rhs);
        ++counter;
    }

    IS_String::IS_String(IS_String const& rhs)
        : len{ rhs.len }, data{ new char[len + 1] } {
    #ifdef DEBUG
            std::cout << __PRETTY_FUNCTION__ << rhs.data << "\n";
    #endif
        IS_String::strcpy(data, rhs.data);
        ++counter;
    }

    IS_String::IS_String(std::initializer_list<char> rhs)
        : len{ rhs.size() }, data{ new char[len + 1] } {
        pointer tmp{ data };
        for (char ch : rhs) {
            *tmp++ = ch;
        }
        *tmp = '\0';
    #ifdef DEBUG
            std::cout << __PRETTY_FUNCTION__ << data << "\n";
    #endif
        ++counter;
    }


    IS_String::~IS_String() {
    #ifdef DEBUG
            std::cout << __PRETTY_FUNCTION__ << data << "\n";
    #endif
        delete[] data;
        --counter;
    }

    IS_String& IS_String::operator=(IS_String const& rhs) {
        IS_String tmp{ rhs };
        swap(tmp);
        return *this;
    }

    IS_String& IS_String::operator=(IS_String::const_pointer rhs) {
        IS_String tmp{ rhs };
        swap(tmp);
        return *this;
    }

    IS_String& IS_String::operator+=(IS_String const& rhs) {
        size_type tmp_len{ len + rhs.len };
        pointer   tmp_data{ new value_type[tmp_len + 1] };
        IS_String::strcpy(tmp_data, data);
        IS_String::strcat(tmp_data, rhs.data);

        len = tmp_len;
        delete[] data; data = tmp_data;

        return *this;
    }

    IS_String& IS_String::operator+=(IS_String::const_pointer rhs) {
        size_type tmp_len{ len + std::strlen(rhs) };
        pointer   tmp_data{ new value_type[tmp_len + 1] };
        IS_String::strcpy(tmp_data, data);
        IS_String::strcat(tmp_data, rhs);

        len = tmp_len;
        delete[] data; data = tmp_data;

        return *this;
    }

    IS_String::reference IS_String::operator[](IS_String::size_type index) {
        return const_cast<reference>((static_cast<IS_String const&>(*this))[index]);
    }

    IS_String::const_reference IS_String::operator[](IS_String::size_type index) const {
        return data[index];
    }

    IS_String::size_type IS_String::size() const {
        return len;
    }

    bool IS_String::empty() const {
        return len == 0;
    }

    IS_String::const_pointer IS_String::c_str() const {
        return data;
    }

    int IS_String::compare(IS_String const& rhs) const {
        return strcmp(data, rhs.data);
    }

    void IS_String::swap(IS_String& rhs) {
        std::swap(len, rhs.len);
        std::swap(data, rhs.data);
    }

    void IS_String::strcpy(IS_String::pointer dest, IS_String::const_pointer src) {
        size_type i = 0;
        while (src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }

    void IS_String::strcat(IS_String::pointer dest, IS_String::const_pointer src) {
        size_type dest_len = 0;
        while (dest[dest_len] != '\0') {
            dest_len++;
        }

        size_type i = 0;
        while (src[i] != '\0') {
            dest[dest_len + i] = src[i];
            i++;
        }
        dest[dest_len + i] = '\0';
    }

    IS_String::iterator IS_String::begin() {
        return data;
    }
    IS_String::const_iterator IS_String::begin() const {
        return data;
    }
    IS_String::iterator IS_String::end() {
        return data + len;
    }
    IS_String::const_iterator IS_String::end() const {
        return data + len;
    }
    IS_String::const_iterator IS_String::cbegin() const {
        return data;
    }
    IS_String::const_iterator IS_String::cend() const {
        return data + len;
    }

    IS_String operator+(IS_String const& lhs, IS_String const& rhs) {
        IS_String tmp{ lhs };
        tmp += rhs;
        return tmp;
    }

    IS_String operator+(IS_String const& lhs, IS_String::const_pointer rhs) {
        IS_String tmp{ lhs };
        tmp += rhs;
        return tmp;
    }

    IS_String operator+(IS_String::const_pointer lhs, IS_String const& rhs) {
        IS_String tmp{ lhs };
        tmp += rhs;
        return tmp;
    }

    bool operator==(IS_String const& lhs, IS_String const& rhs) {
        return lhs.compare(rhs) == 0 ? true : false;
    }
    bool operator!=(IS_String const& lhs, IS_String const& rhs) {
        return !(lhs == rhs);
    }
    bool operator< (IS_String const& lhs, IS_String const& rhs) {
        return lhs.compare(rhs) < 0 ? true : false;
    }
    bool operator<=(IS_String const& lhs, IS_String const& rhs) {
        return lhs.compare(rhs) <= 0 ? true : false;
    }
    bool operator> (IS_String const& lhs, IS_String const& rhs) {
        return lhs.compare(rhs) > 0 ? true : false;
    }
    bool operator>=(IS_String const& lhs, IS_String const& rhs) {
        return lhs.compare(rhs) >= 0 ? true : false;
    }

    std::ostream& operator<<(std::ostream& os, IS_String const& rhs) {
        os << rhs.c_str();
        return os;
    }

    std::istream& operator>>(std::istream& is, IS_String& rhs) {
        int const max_buff_size{ 1024 };
        IS_String::value_type buffer[max_buff_size];
        is.width(max_buff_size - 1);
        is >> buffer;
        rhs = buffer;
        return is;
    }

    IS_String::size_type IS_String::get_count() {
        return IS_String::counter;
    }
}