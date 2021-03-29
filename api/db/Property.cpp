////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Property.hpp"

#include "Atom.hpp"
#include "Workspace.hpp"

#include <util/SqlQuery.hpp>


namespace cdb {
    Vector<Property>        all_properties()
    {
        Vector<Property>    ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Properties");
        auto s_af = s.af();
        if(s.exec()){
            while(s.next())
                ret << Property{s.valueU64(0)};
        }
        return ret;
    }
    
    uint64_t                all_properties_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Properties");
        auto s_af = s.af();
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }
    
    Atom                    atom(Property p)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM Properties WHERE id=?");
        auto s_af = s.af();
        s.bind(0, p.id);
        if(s.exec() && s.next())
            return Atom{s.valueU64(0)};
        return Atom{};
    }
    
    bool                    exists(Property p)
    {
        return exists_property(p.id);
    }
    
    bool                    exists_property(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Properties WHERE id=?");
        auto s_af = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    
    QString                 key(Property p)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT k FROM Properties WHERE id=?");
        auto s_af = s.af();
        s.bind(0, p.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    AttrKind                kind(Property p)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT kind FROM Properties WHERE id=?");
        auto s_af = s.af();
        s.bind(0, p.id);
        if(s.exec() && s.next())
            return AttrKind(s.valueAs<int>(0));
        return AttrKind();
    }
    
    Property                property(uint64_t i)
    {
        return exists_property(i) ? Property{i} : Property{};
    }
    
    QString                 type(Property p)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT type FROM Properties WHERE id=?");
        auto s_af = s.af();
        s.bind(0, p.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString                 uid(Property p)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT uid FROM Properties WHERE id=?");
        auto s_af = s.af();
        s.bind(0, p.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString                 value(Property p)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT value FROM Properties WHERE id=?");
        auto s_af = s.af();
        s.bind(0, p.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
}
