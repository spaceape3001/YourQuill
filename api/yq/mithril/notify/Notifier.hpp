////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mithril/enum/Change.hpp>
#include <mithril/id/Id.hpp>
#include <functional>

namespace yq::mithril {
    template <typename ... Args>
    class Notifier {
    public:
        
        using FNExecute = std::function<void(Args...)>;
        
        static size_t   add(Id, FNExecute&&);
        static size_t   add(FNExecute&&);
        static void     erase(size_t);
        static void     erase_all(Id);
        static void     notify(Args...);
        
    private:
    
        struct Handler;
        struct Repo;
        static Repo& repo();
    };
}
