////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NotifyAdapters.hpp"

namespace yq {
    class FunctionalNotifier : public Notifier {
    public:
        std::function<void()>   m_function;
        FunctionalNotifier(Flag<Change> ch, const FileSpec& spec, int _order, std::function<void()> handler, const std::source_location&sl) :
            Notifier(ch, spec, _order, sl), m_function(handler)
        {
            
        }
    
        bool change(Fragment, Change) const override
        {
            m_function();
            return true;
        }
        
    };

    Notifier::Writer    on_change(const FileSpec& spec, std::function<void()>fn, const std::source_location& sl)
    {
        return Notifier::Writer{new FunctionalNotifier(all_set<Change>(), spec, 0, fn, sl)};
    }
}
