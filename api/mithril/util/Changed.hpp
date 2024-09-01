////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <set>
#include <yq/container/set_utils.hpp>
#include <yq/container/Map.hpp>
#include <yq/container/Set.hpp>

namespace yq::mithril {
    template <typename T>
    struct Changed {
        T       from;
        T       to;
        
        constexpr operator bool () const noexcept { return from != to; }
    };
    
    template <typename T, typename C, typename A>
    struct Changed<std::set<T,C,A>> {
        std::set<T,C,A>     added;
        std::set<T,C,A>     from;
        std::set<T,C,A>     removed;
        std::set<T,C,A>     same;
        std::set<T,C,A>     to;

        constexpr operator bool () const noexcept { return from != to; }
        
        void    analyze(C comp=C())
        {
            set_changes(from.begin(), from.end(), to.begin(), to.end(),
                std::inserter(removed, removed.end()), 
                std::inserter(same, same.end()), 
                std::inserter(added, added.end()), 
                comp
            );
        }
    };

    template <typename T, typename C>
    struct Changed<Set<T,C>> {
        Set<T,C>     added;
        Set<T,C>     from;
        Set<T,C>     removed;
        Set<T,C>     same;
        Set<T,C>     to;

        constexpr operator bool () const noexcept { return from != to; }
        
        void    analyze(C comp=C())
        {
            set_changes(from.begin(), from.end(), to.begin(), to.end(),
                std::inserter(removed, removed.end()), 
                std::inserter(same, same.end()), 
                std::inserter(added, added.end()), 
                comp
            );
        }
    };
    
    template <typename K, typename V, typename C, typename A>
    struct Changed<std::map<K,V,C,A>> {
        using map_t = std::map<K,V,C,A>;
    
        map_t   added;
        map_t   from;
        map_t   modified;
        map_t   removed;
        map_t   same;
        map_t   to;
        
        constexpr operator bool () const noexcept { return from != to; }
        
        void    analyze(C comp=C())
        {
            map_difference_exec2(from, to, 
                [&](const typename map_t::value_type& j){
                    removed.insert(j);
                }, [&](const typename map_t::value_type& j){
                    modified.insert(j);
                }, [&](const typename map_t::value_type& j){
                    same.insert(j);
                }, [&](const typename map_t::value_type& j){
                    added.insert(j);
                }
            );
        }
    };

    template <typename K, typename V, typename C>
    struct Changed<Map<K,V,C>> {
        using map_t = Map<K,V,C>;
    
        map_t   added;
        map_t   from;
        map_t   modified;
        map_t   removed;
        map_t   same;
        map_t   to;
        
        constexpr operator bool () const noexcept { return from != to; }
        
        void    analyze(C comp=C())
        {
            map_difference_exec2(from, to, 
                [&](const typename map_t::value_type& j){
                    removed.insert(j);
                }, [&](const typename map_t::value_type& j){
                    modified.insert(j);
                }, [&](const typename map_t::value_type& j){
                    same.insert(j);
                }, [&](const typename map_t::value_type& j){
                    added.insert(j);
                }
            );
        }
    };
}
