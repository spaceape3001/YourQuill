////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Attribute.hpp"
#include "Document.hpp"
#include "Workspace.hpp"

#include <util/Logging.hpp>
#include <util/SqlQuery.hpp>
#include <util/Utilities.hpp>

namespace cdb {

    Vector<Attribute>   all_attributes()
    {
        Vector<Attribute>   ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Attributes");
        auto s_af = s.af();
        if(s.exec()){
            while(s.next())
                ret << Attribute{s.valueU64(0)};
        }
        return ret;
    }
    
    Vector<Attribute>   all_attributes(Document d) 
    {
        Vector<Attribute>   ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Attributes WHERE doc=?");
        auto s_af = s.af();
        s.bind(0, d.id);
        if(s.exec()){
            while(s.next())
                ret << Attribute{s.valueU64(0)};
        }
        return ret;
    }
    
    size_t              all_attributes_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Attributes");
        auto s_af = s.af();
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }
    
    size_t              all_attributes_count(Document d)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Attributes WHERE doc=?");
        auto s_af = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }

    //! Gets attributes (sub)
    Vector<Attribute>   attributes(Attribute a)
    {
        Vector<Attribute>   ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Attributes WHERE parent=? ORDER BY idx");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec()){
            while(s.next())
                ret << Attribute{s.valueU64(0)};
        }
        return ret;
    }

    //! Gets top-level attirbutes
    Vector<Attribute>   attributes(Document d)
    {
        Vector<Attribute>   ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
        auto s_af   = s.af();
        s.bind(0, d.id);
        if(s.exec()){
            while(s.next())
                ret << Attribute{s.valueU64(0)};
        }
        return ret;
    }
    
    size_t              attributes_count(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Attributes WHERE parent=?");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }
    
    size_t              attributes_count(Document d)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Attributes WHERE doc=? AND parent=0");
        auto s_af   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }

    Document            document(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT doc FROM Attributes WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return Document{s.valueU64(0)};
        return Document{};
    }
    
    bool                exists(Attribute a)
    {
        return exists_attribute(a.id);
    }
    
    bool                exists_attribute(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Attributes WHERE id=?");
        auto s_af = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    //! Index in the file's list
    uint64_t            index(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT idx FROM Attributes WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }
    

    QString             key(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT k FROM Attributes WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return 0;
    }
    
    DocOrAttr           parent(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT parent,doc FROM Attributes WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next()){
            uint64_t    p   = s.valueU64(0);
            if(p)
                return Attribute{p};
            return Document{s.valueU64(1)};
        }
        return false;
    }
    
    QString             uid(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT uid FROM Attributes WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             value(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT value FROM Attributes WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
}
