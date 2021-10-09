#pragma once
#include <string_view>
#include "ResultFwd.hpp"

class StringView : public std::string_view {
public:

    class Iter32;
    class IterW;


    constexpr StringView(){}
    
    constexpr StringView(const StringView&) = default;
    constexpr StringView(const std::string_view& cp) : std::string_view(cp) {}
    constexpr StringView(StringView&&) = default;
    constexpr StringView(std::string_view&&mv) : std::string_view(std::move(mv)) {}
    
    constexpr StringView(const char*s) : std::string_view(s) {}
    constexpr StringView(const char*s, size_t n) : std::string_view(s,n) {}
    template <class It, class End>
    constexpr StringView( It first, End last ) : std::string_view(first, last) {}
    
    constexpr StringView& operator=(const StringView&) = default;
    constexpr StringView& operator=(const std::string_view&cp) 
    {
        std::string_view::operator=(cp);
        return *this;
    }
    
    ~StringView(){}

    template <typename Pred>
    bool                for_each(Pred p) const;

    template <typename Pred>
    bool                for_each_abort(Pred p) const;

    template <typename Pred>
    bool                for_each_w(Pred p) const;

    Iter32              iterate() const;
    IterW               iterate_w() const;

    bool                matches_at(const String&, size_t pos=0) const;
    
    Vector<StringView>  split(char ch) const;
    Vector<StringView>  split(char ch, unsigned int) const;
    Vector<StringView>  split(const char*z) const;
    Vector<StringView>  split(const char*z,unsigned int) const;
    Vector<StringView>  split(const std::string& s) const;
    Vector<StringView>  split(const std::string& s,unsigned int) const;
    Vector<StringView>  split(const std::string_view& s) const;
    Vector<StringView>  split(const std::string_view& s,unsigned int) const;

    bool                starts_with(const std::string&) const;
    bool                starts_with(const std::string_view&) const;
    bool                starts_with(const char*) const;

    
    boolean_r           to_boolean() const;
    
    double_r            to_double() const;
    float_r             to_float() const;
    unsigned_r          to_hex() const;
    uint8_r             to_hex8() const;
    uint16_r            to_hex16() const;
    uint32_r            to_hex32() const;
    uint64_r            to_hex64() const;
    int8_r              to_int8() const;
    int16_r             to_int16() const;
    int32_r             to_int32() const;
    int64_r             to_int64() const;
    integer_r           to_integer() const;
    short_r             to_short() const;
    uint8_r             to_uint8() const;
    uint16_r            to_uint16() const;
    uint32_r            to_uint32() const;
    uint64_r            to_uint64() const;
    unsigned_r          to_uinteger() const;
    ushort_r            to_ushort() const;
};
