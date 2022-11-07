////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/CollectionUtils.hpp>
#include <basic/EnumMap.hpp>
#include <basic/Vector.hpp>
#include <kernel/atom/Atom.hpp>
#include <kernel/atom/AtomSpec.hpp>
#include <kernel/atom/Class.hpp>
#include <kernel/atom/Field.hpp>
#include <kernel/attr/AttributeDiff.hpp>
#include <kernel/enum/Change.hpp>
#include <vector>
#include <source_location>

namespace yq {

    struct AtomChangeData {
        Document                    doc;
        Atom                        atom;
        SetChanges<Class>           classes;
        SetChanges<Field>           fields;
        SetChanges<Tag>             tags;
        bool                        title   = false;
        std::span<Attribute::Diff>  diffs;  //< Will be empty for STARTUP
    };

    /*! \brief Notification to a changed class
    
        This notification will be triggered whenever an atom is touched. 
    */
    class AtomNotifier {
    public:
        static const std::vector<const AtomNotifier*>& all();
        
        virtual void        change(const AtomChangeData&) const = 0;
        struct Writer;
        
        std::string_view                description() const { return m_description; }
        Flag<Change>                    change() const { return m_change; }
        const std::source_location&     source() const { return m_source; }
        const AtomSpec&                 spec() const { return m_spec; }
    
        static const EnumMap<Change,Vector<const AtomNotifier*>>&     change_map();
        
    protected:
        AtomNotifier(Flag<Change>, const AtomSpec&, const std::source_location&);
        ~AtomNotifier();
        
    private:
        AtomSpec                m_spec;
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
