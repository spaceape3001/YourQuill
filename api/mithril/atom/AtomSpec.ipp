////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <0/basic/TextUtils.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomSpec.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/id/Id.hpp>

namespace yq::mithril {
    AtomSpec        by_class(std::string_view k)
    {
        return AtomSpec{ copy(k), 0, AtomSpec::ByClassKey };
    }
    
    AtomSpec        by_class(Class c)
    {
        return AtomSpec{ {}, c.id, AtomSpec::ByClassId };
    }
    
    AtomSpec        by_field(std::string_view k)
    {
        return AtomSpec{ copy(k), 0, AtomSpec::ByFieldKey };
    }
    
    AtomSpec        by_field(Field f)
    {
        return AtomSpec{ {}, f.id, AtomSpec::ByFieldId };
    }

    Class   AtomSpec::class_() const
    {
        switch(type){
        case ByClassKey:
            return cdb::class_(str);
        case ByClassId:
            return Class{ u64 };
        default:
            return Class();
        }
    }
    
    Field   AtomSpec::field() const
    {
        switch(type){
        case ByFieldKey:
            return cdb::field(str);
        case ByFieldId:
            return Field{ u64 };
        default:
            return Field{};
        }
    }

    bool    AtomSpec::is_class() const
    {
        switch(type){
        case ByClassKey:
        case ByClassId:
            return true;
        default:
            return false;
        }
    }
    
    bool    AtomSpec::is_field() const
    {
        switch(type){
        case ByFieldKey:
        case ByFieldId:
            return true;
        default:
            return false;
        }
    }
    
    bool    AtomSpec::match(Id id) const
    {
        Class   c   = id.as<Class>();
        if(c)
            return match(c);
        Field   f   = id.as<Field>();
        if(f)
            return match(f);
        
        Atom    a   = id.as<Atom>();
        if(a){
            for(Class c : cdb::classes(a)){
                if(match(c))
                    return true;
            }
            return false;
        }
        
        return false;
        
    }


    bool    AtomSpec::match(Class c) const
    {
        switch(type){
        case Never:
            return false;
        case Always:
            return true;
        case ByClassId:
            return c.id == u64;
        case ByClassKey:
            return c == cdb::class_(str);
        default:
            return false;
        }
    }
    

    bool    AtomSpec::match(Field f) const
    {
        switch(type){
        case Never:
            return false;
        case Always:
            return true;
        case ByFieldId:
            return f.id == u64;
        case ByFieldKey:
            return f == cdb::field(str);
        default:
            return false;
        }
    }
    

    bool    AtomSpec::match_class(std::string_view k) const
    {
        switch(type){
        case Never:
            return false;
        case Always:
            return true;
        case ByClassKey:
            return is_similar(k, str);
        case ByClassId:
            return is_similar(cdb::key(Class{u64}), k);
        default:
            return false;
        }
    }
    
    bool    AtomSpec::match_field(std::string_view k) const
    {
        switch(type){
        case Never:
            return false;
        case Always:
            return true;
        case ByFieldKey:
            return is_similar(k, str);
        case ByFieldId:
            return is_similar(cdb::key(Field{u64}), k);
        default:
            return false;
        }
    }

}

