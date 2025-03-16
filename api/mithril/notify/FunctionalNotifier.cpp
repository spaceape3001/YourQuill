////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileNotifyAdapters.hpp"

namespace yq::mithril {
    class FunctionalFileNotifier : public FileNotifier {
    public:
        std::function<void()>   m_function;
        FunctionalFileNotifier(Flag<Change> ch, const FileSpec& spec, int _order, std::function<void()> handler, const std::source_location&sl) :
            FileNotifier(ch, spec, _order, sl), m_function(handler)
        {
            
        }
    
        bool change(Fragment, Change) const override
        {
            m_function();
            return true;
        }
        
    };

    FileNotifier::Writer    on_change(const FileSpec& spec, std::function<void()>fn, const std::source_location& sl)
    {
        return FileNotifier::Writer{new FunctionalFileNotifier(all_set<Change>(), spec, 0, fn, sl)};
    }
}
