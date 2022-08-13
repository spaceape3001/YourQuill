////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValueCDB.hpp"
#include <kernel/atom/Field.hpp>
#include <kernel/db/SQ.hpp>

namespace yq {
    namespace cdb {
        std::string     brief(Value v)
        {
            static thread_local SQ s("SELECT brief FROM Vals WHERE id=?");
            return s.str(v.id);
        }
        
        std::string     data(Value v)
        {
            static thread_local SQ s("SELECT data FROM Vals WHERE id=?");
            return s.str(v.id);
        }
    
        Value           db_value(Field field, std::string_view data, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            
            if(data.empty() || !field)
                return Value();
            
            static thread_local SQ i("INSERT INTO Vals OR FAIL (field, data) VALUES (?,?)");
            static thread_local SQ s("SELECT id FROM Vals WHERE field=? AND data=?");
            
            auto iaf = i.af();
            auto saf = s.af();

            i.bind(1, field.id);
            i.bind(2, data);
            if(i.step(false) == SqlQuery::Done){
                if(wasCreated)
                    *wasCreated = true;
                return Value{(uint64_t) i.last_id()};
            } else {
                s.bind(1, field.id);
                s.bind(2, data);
                if(s.step() == SqlQuery::Row)
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
            static thread_local SQ s("SELECT 1 FROM Vals WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Field           field(Value v)
        {
            static thread_local SQ s("SELECT field FROM Vals WHERE id=?");
            return s.as<Field>(v.id);
        }
        
        Value::Info     info(Value v)
        {
            Value::Info ret;

            static thread_local SQ s("SELECT field, brief, data FROM Vals WHERE id=?");
            auto af = s.af();
            s.bind(1, v.id);
            if(s.step() == SqlQuery::Row){
                ret.field   = Field{ s.v_uint64(1) };
                ret.brief   = s.v_string(2);
                ret.data    = s.v_string(3);
            }
            return ret;
        }
        
        Value           value(uint64_t i)
        {
            return exists_value(i) ? Value{i} : Value{};
        }
        
        namespace {
            std::vector<Value>   values_sorted(Field f)
            {
                static thread_local SQ s("SELECT values FROM Vals WHERE field=? ORDER BY data");
                return s.vec<Value>(f.id);
            }

            std::vector<Value>   values_unsorted(Field f)
            {
                static thread_local SQ s("SELECT values FROM Vals WHERE field=?");
                return s.vec<Value>(f.id);
            }
        }

        std::vector<Value>   values(Field f, Sorted sorted)
        {
            return sorted ? values_sorted(f) : values_unsorted(f);
        }
        
        std::set<Value>      values_set(Field f)
        {
            static thread_local SQ s("SELECT values FROM Vals WHERE field=?");
            return s.set<Value>(f.id);
        }
    }
}
