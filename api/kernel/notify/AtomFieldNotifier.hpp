////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/EnumMap.hpp>
#include <basic/Vector.hpp>
#include <kernel/atom/Atom.hpp>
#include <kernel/atom/Field.hpp>
#include <kernel/enum/Change.hpp>
#include <vector>
#include <source_location>

namespace yq {
    /*! \brief Notification to a changed class
    
        This notification will be triggered whenever an atom is touched. 
    */
    class AtomFieldNotifier {
    public:
        static const std::vector<const AtomFieldNotifier*>& all();
        
        virtual void        change(Atom, Field, Change) const = 0;
        struct Writer;
        
        std::string_view                description() const { return m_description; }
        Flag<Change>                    change() const { return m_change; }
        const std::source_location&     source() const { return m_source; }
        Field                           field() const { return m_field; }
    
        static const EnumMap<Change,Vector<const AtomFieldNotifier*>>&     change_map();
        
    protected:
        AtomFieldNotifier(Flag<Change>, Field, const std::source_location&);
        ~AtomFieldNotifier();
        
    private:
        Field                   m_field;
        Flag<Change>            m_change;
        std::string             m_description;
        std::source_location    m_source;
        
        struct Repo;
        static Repo&            repo();
    };
    
    struct AtomFieldNotifier::Writer {
        Writer&     description(std::string_view);
        AtomFieldNotifier*   importer  = nullptr;
    };
}
