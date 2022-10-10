////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/EnumMap.hpp>
#include <basic/Vector.hpp>
#include <kernel/atom/Atom.hpp>
#include <kernel/atom/Class.hpp>
#include <kernel/enum/Change.hpp>
#include <vector>
#include <source_location>

namespace yq {
    class AtomNotifier {
    public:
        static const std::vector<const AtomNotifier*>& all();
        
        virtual void        change(Atom, Class, Change) const = 0;
        struct Writer;
        
        std::string_view                description() const { return m_description; }
        Flag<Change>                    change() const { return m_change; }
        const std::source_location&     source() const { return m_source; }
        Class                           class_() const { return m_class; }
    
        static const EnumMap<Change,Vector<const AtomNotifier*>>&     change_map();
        
    protected:
        AtomNotifier(Flag<Change>, Class, const std::source_location&);
        ~AtomNotifier();
        
    private:
        Class                   m_class;
        Flag<Change>            m_change;
        std::string             m_description;
        std::source_location    m_source;
        
        struct Repo;
        static Repo&            repo();
    };
    
    struct AtomNotifier::Writer {
        Writer&     description(std::string_view);
        AtomNotifier*   importer  = nullptr;
    };
}
