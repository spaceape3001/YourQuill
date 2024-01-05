////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mithril/enum/Change.hpp>
#include <mithril/id/Id.hpp>
#include <string>
#include <functional>

namespace yq::mithril::update {

    /*! \brief Update base
    
        This is the base update object.  
    */
    template <IdType T>
    class U {
    public:
    
        using FNExecute     = std::function<void(T,Change)>;
        using FNTrigger     = std::function<bool(T,Change)>;

        //! Capture this class type
        using B                 = U;
        
        //! Capture the template parameter
        using ID                = T;
    
        const T                 x;
        const id_t              id;
        const std::string       key;
        
        void                notify(Change);
        
        struct Handler {
            Id              origin  = {};
            ChangeFlags     change  = ChangeFlags({ Change::Added, Change::Modified, Change::Removed });
            FNTrigger       trigger;
            FNExecute       execute;
        };
    
        static size_t       add_handler(Handler&&);
        static void         erase_handler(size_t);
        static void         erase_all_handlers(Id);
    
    protected:
        U(T _x, std::string&&k) : x(_x), id(_x), key(std::move(k)) {}
        
        /*!\brief Caching Lookup
        
            This will lookup, and CREATE a cache object to persist in memory.
            
            \note THIS IS SINGLE THREADED USE ONLY!   Only call from the update thread.
        */
        template <typename UU>
        static UU&   lookup(T x);
        
        struct Repo;
        static Repo&    repo();
    };
}
