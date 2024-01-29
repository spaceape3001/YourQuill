////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <set>
#include <0/basic/CollectionUtils.hpp>
#include <0/basic/Set.hpp>

namespace yq::mithril {
    template <typename T>
    struct Changed {
        T       from;
        T       to;
        
        constexpr operator bool () const noexcept { return from != to; }
    };
    
    template <typename T, typename C, typename A>
    struct Changed<std::set<T,C,A>> {
        std::set<T,C,A>     from;
        std::set<T,C,A>     to;
        std::set<T,C,A>     added;
        std::set<T,C,A>     same;
        std::set<T,C,A>     removed;

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
        Set<T,C>     from;
        Set<T,C>     to;
        Set<T,C>     added;
        Set<T,C>     same;
        Set<T,C>     removed;

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
}
