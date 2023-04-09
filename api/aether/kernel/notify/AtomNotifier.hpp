////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <source_location>
#include <span>
#include <vector>

#include <basic/CollectionUtils.hpp>
#include <basic/EnumMap.hpp>
#include <basic/Vector.hpp>
#include <aether/kernel/atom/Atom.hpp>
#include <aether/kernel/atom/AtomSpec.hpp>
#include <aether/kernel/atom/Class.hpp>
#include <aether/kernel/atom/Field.hpp>
#include <aether/kernel/attr/AttributeDiff.hpp>
#include <aether/kernel/enum/Change.hpp>

namespace yq {

    struct AtomChangeData {
        Document                    doc;
        Atom                        atom;
        SetChanges<Class>           classes;
        SetChanges<Field>           fields;
        SetChanges<Tag>             tags;
        bool                        title           = false;
        bool                        abbreviation    = false;
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
    
        /* Not enough worry for separate file of adapters */
    
    template <void (*FN)(const AtomChangeData&)>
    class ACDAtomNofiierAdapter : public AtomNotifier {
    public:
        ACDAtomNofiierAdapter(Flag<Change> ch, const AtomSpec& spec, const std::source_location& sl) :
            AtomNotifier(ch, spec, sl)
        {
        }
        
        void        change(const AtomChangeData&acd) const override
        {
            FN(acd);
        }
    };
    
    template <void (*FN)(const AtomChangeData&)>
    AtomNotifier::Writer on_change(const AtomSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return AtomNotifier::Writer(new ACDAtomNofiierAdapter<FN>(all_set<Change>(), spec, sl));
    }

    template <void (*FN)(const AtomChangeData&)>
    AtomNotifier::Writer on_change(ChangeFlags chg, const AtomSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return AtomNotifier::Writer(new ACDAtomNofiierAdapter<FN>(chg, spec, sl));
    }
    
    
    AtomNotifier::Writer on_change(const AtomSpec&, std::function<void(const AtomChangeData&)>, const std::source_location& sl = std::source_location::current());
    AtomNotifier::Writer on_change(ChangeFlags, const AtomSpec&, std::function<void(const AtomChangeData&)>, const std::source_location& sl = std::source_location::current());
    
}
