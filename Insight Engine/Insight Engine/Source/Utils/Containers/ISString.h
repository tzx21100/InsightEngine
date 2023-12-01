#ifndef GAM200_INSIGHT_ENGINE_SOURCE_STL_STRING_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_STL_STRING_H_

namespace IS {
    class IS_String
    {
    public:
        using value_type = char;
        using size_type = size_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = pointer;
        using const_reverse_iterator = const_pointer;

    public:
        IS_String();
        IS_String(const_pointer);
        IS_String(IS_String const&);
        IS_String(std::initializer_list<char>);
        ~IS_String();

        IS_String& operator=(IS_String const&);
        IS_String& operator=(const_pointer);
        IS_String& operator+=(IS_String const&);
        IS_String& operator+=(const_pointer);
        reference operator[](size_type);
        const_reference operator[](size_type) const;

        size_type size() const;
        bool empty() const;
        const_pointer c_str() const;
        int compare(IS_String const& rhs) const;
        void swap(IS_String&);
        void strcpy(IS_String::pointer dest, IS_String::const_pointer src);
        void strcat(IS_String::pointer dest, IS_String::const_pointer src);

        iterator               begin();
        const_iterator         begin() const;
        iterator               end();
        const_iterator         end() const;
        const_iterator         cbegin() const;
        const_iterator         cend() const;

        static size_type       get_count();
    private:
        size_type len;
        pointer   data;
        static size_type counter;
    };

    IS_String operator+(IS_String const&, IS_String const&);
    IS_String operator+(IS_String const&, IS_String::const_pointer);
    IS_String operator+(IS_String::const_pointer, IS_String const&);

    bool operator==(IS_String const&, IS_String const&);
    bool operator!=(IS_String const&, IS_String const&);
    bool operator< (IS_String const&, IS_String const&);
    bool operator<=(IS_String const&, IS_String const&);
    bool operator> (IS_String const&, IS_String const&);
    bool operator>=(IS_String const&, IS_String const&);

    std::ostream& operator<<(std::ostream&, IS_String const&);
    std::istream& operator>>(std::istream&, IS_String&);
}

#endif //GAM200_INSIGHT_ENGINE_SOURCE_STL_STRING_H_