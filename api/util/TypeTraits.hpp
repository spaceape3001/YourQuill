////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <type_traits>

namespace yq {
    template <class ...> using void_t = void;


    // bool    has_less<T>()
    template <typename T, class = void>
    struct has_less : public std::false_type {};
    
    template <typename T>
    using less_than = decltype(std::declval<T>() < std::declval<T>());
    
    template <typename T>
    struct has_less<T,void_t<less_than<T>>> : public std::true_type{};
    
    template <typename T>
    inline constexpr bool has_less_v = has_less<T>::value;
}

