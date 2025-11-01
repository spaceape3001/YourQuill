////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/id/Id.hpp>
#include <string>
#include <functional>

namespace yq::mithril::update {

    /*! \brief Update base
    
        This is the base update object.  
    */
    template <IdType T>
    class U {
    public:

        //! Capture this class type
        using B                 = U;
        
        //! Capture the template parameter
        using ID                = T;
    
        const T                 x;
        const id_t              id;
        const std::string       key;
        
    protected:
        U(T _x, std::string&&k) : x(_x), id(_x), key(std::move(k)) {}
        
        /*!\brief Caching Lookup
        
            This will lookup, and CREATE a cache object to persist in memory.
            
            \note THIS IS SINGLE THREADED USE ONLY!   Only call from the update thread.
        */
        template <typename UU>
        static UU&   lookup(T x);
    };
}
