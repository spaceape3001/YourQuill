////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <list>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace yq {

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  SET RELATED
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    template<class InputIt1, class InputIt2, class OutputIt>
    void    set_exclusive(InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2,
                              OutputIt d_first)
    {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                *d_first++  = *first1++;
            } else  {
                if (!(*first2 < *first1)) { 
                    // *first1 and *first2 are equivalent.
                } else {
                    *d_first++ = *first2; 
                }
                ++first2;
            }
        }
    }
    
    template<class InputIt1, class InputIt2, class OutputIt, class Compare>
    void    set_changes(InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2,
                              OutputIt d_left, OutputIt d_middle, 
                              OutputIt d_right, Compare comp
    ){
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1 < *first2)) {
                *d_left++  = *first1++;
            } else  {
                if (!comp(*first2 < *first1)) { 
                    *d_middle++ = *first1++;
                } else {
                    *d_right++ = *first2; 
                }
                ++first2;
            }
        }
    }
                              

    template <typename T, typename C, typename A>
    std::set<T, C, A>    operator+(const std::set<T, C, A>& a, const std::set<T, C, A>& b)
    {
        std::set<T, C, A>   ret = a;
        ret.insert(b.begin(), b.end());
        return ret;
    }
    
    template <typename T, typename C, typename C2, typename A, typename A2>
    std::set<T, C, A>&    operator+=(std::set<T, C, A>& a, const std::set<T, C2, A2>& b)
    {
        a.insert(b.begin(), b.end());
        return a;
    }

    template <typename C, typename C2, typename A, typename A2>
    std::set<std::string, C, A>&    operator+=(std::set<std::string, C, A>& a, const std::set<std::string_view, C2, A2>& b)
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
    std::set<T, C, A>    operator-(const std::set<T, C, A>& a, const std::set<T, C, A>& b)
    {
        std::set<T, C, A>   ret;
        std::set_difference(a.begin(), a.end(), b.begin(), b.end(), ret.begin());
        return ret;
    }

    template <typename T, typename C, typename A>
    std::set<T, C, A>    operator|(const std::set<T, C, A>& a, const std::set<T, C, A>& b)
    {
        std::set<T, C, A>   ret;
        std::set_union(a.begin(), a.end(), b.begin(), b.end(), ret.begin());
        return ret;
    }

    template <typename T, typename C, typename A>
    std::set<T, C, A>    operator&(const std::set<T, C, A>& a, const std::set<T, C, A>& b)
    {
        std::set<T, C, A>   ret;
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), ret.begin());
        return ret;
    }

    template <typename T, typename C, typename A>
    std::set<T, C, A>    operator^(const std::set<T, C, A>& a, const std::set<T, C, A>& b)
    {
        std::set<T, C, A>   ret;
        set_exclusive(a.begin(), a.end(), b.begin(), b.end(), ret.begin());
        return ret;
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

    template <typename T, typename C, typename A>
    struct changes_t {
        std::set<T,C,A>     added;
        std::set<T,C,A>     same;
        std::set<T,C,A>     removed;
    };

    template <typename T, typename C, typename A>
    changes_t<T,C,A>    changes(const std::set<T,C,A>& from, const std::set<T,C,A>& to)
    {
        changes_t<T,C,A>    ret;
        set_changes(from.begin(), from.end(), to.begin(), to.end(), ret.removed.begin(), ret.same.begin(), ret.added.begin(), from.key_compare());
        return ret;
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
    
    template <typename A>
    std::vector<std::string, A>&    operator+=(std::vector<std::string, A>& a, const std::vector<std::string_view, A>& b)
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
