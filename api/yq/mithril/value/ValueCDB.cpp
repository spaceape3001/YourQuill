////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ValueCDB.hpp"
#include <mithril/field/Field.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/logging.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::cdb {
    std::vector<Value>      all_values(Sorted sorted)
    {
        static thread_local CacheQuery s("SELECT id FROM Vals");
        return s.vec<Value>();
    }

    std::string     brief(Value v)
    {
        static thread_local CacheQuery s("SELECT brief FROM Vals WHERE id=?");
        return s.str(v.id);
    }
    
    std::string     data(Value v)
    {
        static thread_local CacheQuery s("SELECT data FROM Vals WHERE id=?");
        return s.str(v.id);
    }

    Value           db_value(Field field, std::string_view data, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        
        if(data.empty() || !field)
            return Value();
        
        static thread_local CacheQuery i("INSERT INTO Vals OR FAIL (field, data) VALUES (?,?)");
        static thread_local CacheQuery s("SELECT id FROM Vals WHERE field=? AND data=?");
        
        auto iaf = i.af();
        auto saf = s.af();

        i.bind(1, field.id);
        i.bind(2, data);
        if(i.step(false) == SQResult::Done){
            if(wasCreated)
                *wasCreated = true;
            return Value{(uint64_t) i.last_id()};
        } else {
            s.bind(1, field.id);
            s.bind(2, data);
            if(s.step() == SQResult::Row)
                return Value{s.v_uint64(1)};
            cdbError << "Unable to get value ID";
            return Value();
        }
    }
    
    
    bool            exists(Value v)
    {
        return exists_value(v.id);
    }
    
    bool            exists_value(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM Vals WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Field           field(Value v)
    {
        static thread_local CacheQuery s("SELECT field FROM Vals WHERE id=?");
        return s.as<Field>(v.id);
    }

    std::string     key(Value v)
    {
        return key(field(v));
    }
    
    Image                   icon(Value v)
    {
        return icon(field(v));
    }
    
    Value::Info     info(Value v)
    {
        Value::Info ret;

        static thread_local CacheQuery s("SELECT field, brief, data FROM Vals WHERE id=?");
        auto af = s.af();
        s.bind(1, v.id);
        if(s.step() == SQResult::Row){
            ret.field   = Field{ s.v_uint64(1) };
            ret.brief   = s.v_string(2);
            ret.data    = s.v_string(3);
        }
        return ret;
    }
    
    std::string     name(Value v)
    {
        return name(field(v));
    }
    
    Value           value(uint64_t i)
    {
        return exists_value(i) ? Value{i} : Value{};
    }

    std::vector<Value>   values(Field f, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT values FROM Vals WHERE field=? ORDER BY data");
        static thread_local CacheQuery qu("SELECT values FROM Vals WHERE field=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Value>(f.id);
    }
    
    std::set<Value>      values_set(Field f)
    {
        static thread_local CacheQuery s("SELECT values FROM Vals WHERE field=?");
        return s.set<Value>(f.id);
    }
}
