////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Property.hpp"
#include "CacheUtil.hpp"

#include "Atom.hpp"
#include "Wksp.hpp"

//  This will fold into the unity once it's complete..... (not there, yet)

namespace yq {
    namespace cdb {
        Vector<Property>        all_properties()
        {
            static thread_local SQ s("SELECT id FROM Properties");
            return s.vec<Property>();
        }
        
        size_t                 all_properties_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Properties");
            return s.size();
        }
        
        Atom                    atom(Property p)
        {
            static thread_local SQ s("SELECT atom FROM Properties WHERE id=?");
            return s.as<Atom>(p.id);
        }
        
        bool                    exists(Property p)
        {
            return exists_property(p.id);
        }
        
        bool                    exists_property(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Properties WHERE id=?");
            return s.present(i);
        }
        
        
        String                  key(Property p)
        {
            static thread_local SQ s("SELECT k FROM Properties WHERE id=?");
            return s.str(p.id);
        }
        
        AttrKind                kind(Property p)
        {
            static thread_local SQ s("SELECT kind FROM Properties WHERE id=?");
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
        
        String                  type(Property p)
        {
            static thread_local SQ s("SELECT type FROM Properties WHERE id=?");
            return s.str(p.id);
        }
        
        String                  uid(Property p)
        {
            static thread_local SQ s("SELECT uid FROM Properties WHERE id=?");
            return s.str(p.id);
        }
        
        String                  value(Property p)
        {
            static thread_local SQ s("SELECT value FROM Properties WHERE id=?");
            return s.str(p.id);
        }
    }
}
