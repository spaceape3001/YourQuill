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
        
        using FNExecute = std::function<void(Change, Args...)>;
        using FNTrigger = std::function<bool(Change, Args...)>;
        
        struct Handler {
            Id              origin  = {};
            ChangeFlags     change  = ChangeFlags({ Change::Added, Change::Modified, Change::Removed });
            FNTrigger       trigger;
            FNExecute       execute;
        };
        
        static size_t   add(Handler&&);
        static void     erase(size_t);
        static void     erase_all(Id);
        static void     notify(Change, Args...);
        
    private:
        struct Repo;
        static Repo& repo();
    };
}
