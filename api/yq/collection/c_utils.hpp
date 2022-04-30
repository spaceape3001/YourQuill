////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <list>
#include <set>
#include <vector>

namespace yq {

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  SET RELATED
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    template <typename T, typename C, typename C2, typename A, typename A2>
    std::set<T, C, A>&    operator+=(std::set<T, C, A>& a, const std::set<T, C2, A2>& b)
    {
        a.insert(b.begin(), b.end());
        return a;
    }

    template <typename T, typename C, typename A, typename A2>
    std::set<T, C, A>&    operator+=(std::set<T, C, A>& a, const std::vector<T, A2>& b)
    {
        a.insert(b.begin(), b.end());
        return a;
    }

    template <typename T, typename C, typename A>
    std::set<T,C,A>&    operator<<(std::set<T, C, A>& a, const T& b)
    {
        a.insert(b);
        return a;
    }

    template <typename T, typename C, typename A>
    std::set<T,C,A>&    operator<<(std::set<T, C, A>& a, T&& b)
    {
        a.insert(std::move(b));
        return a;
    }

    template <typename T, typename A>
    std::set<T, std::less<T>, A>         make_set(const std::vector<T,A>& data)
    {
        return std::set<T, std::less<T>, A>(data.begin(), data.end(), std::less<T>(), data.get_allocator());
    }
    
    template <typename T, typename C, typename A>
    std::set<T,C,A>      make_set(const std::vector<T, A>& data, const C& lesser)
    {
        return std::set<T,C>(data.begin(), data.end(), lesser, data.get_allocator());
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  VECTOR RELATED
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    template <typename T, typename A>
    std::vector<T, A>&    operator+=(std::vector<T, A>& a, const std::vector<T, A>& b)
    {
        a.insert(a.end(), b.begin(), b.end());
        return a;
    }
    
    template <typename T, typename A>
    std::vector<T,A>&    operator<<(std::vector<T, A>& a, const T& b)
    {
        a.push_back(b);
        return a;
    }

    template <typename T, typename A>
    std::vector<T,A>&    operator<<(std::vector<T, A>& a, T&& b)
    {
        a.push_back(std::move(b));
        return a;
    }
    
}
