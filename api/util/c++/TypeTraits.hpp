////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <type_traits>

namespace yq {


    // bool    has_less<T>()
    template <typename T, class = void>
    struct has_less : public std::false_type {};
    
    template <typename T>
    using less_than = decltype(std::declval<T>() < std::declval<T>());
    
    template <typename T>
    struct has_less<T,std::void_t<less_than<T>>> : public std::true_type{};
    
    // bool    has_less<T>()

    template <typename T>
    inline constexpr bool has_less_v = has_less<T>::value;
    
    template <typename> struct is_template : std::false_type {};
    template <template <typename...> class Tmpl, typename ...Args>
    struct is_template<Tmpl<Args...>> : std::true_type {};
    
    template <typename T>
    inline constexpr bool is_template_v = is_template<T>::value;
}

