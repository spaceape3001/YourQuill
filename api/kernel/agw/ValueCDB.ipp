////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValueCDB.hpp"
#include <kernel/agw/Field.hpp>
#include <kernel/db/SQ.hpp>

namespace yq {
    namespace cdb {
        std::string     brief(agw::Value v)
        {
            static thread_local SQ s("SELECT brief FROM Vals WHERE id=?");
            return s.str(v.id);
        }
        
        std::string     data(agw::Value v)
        {
            static thread_local SQ s("SELECT data FROM Vals WHERE id=?");
            return s.str(v.id);
        }
    
        agw::Value           db_value(agw::Field field, std::string_view data, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            
            if(data.empty() || !field)
                return agw::Value();
            
            static thread_local SQ i("INSERT INTO Vals OR FAIL (field, data) VALUES (?,?)");
            static thread_local SQ s("SELECT id FROM Vals WHERE field=? AND data=?");
            
            auto iaf = i.af();
            auto saf = s.af();

            i.bind(1, field.id);
            i.bind(2, data);
            if(i.step(false) == SqlQuery::Done){
                if(wasCreated)
                    *wasCreated = true;
                return agw::Value{(uint64_t) i.last_id()};
            } else {
                s.bind(1, field.id);
                s.bind(2, data);
                if(s.step() == SqlQuery::Row)
                    return agw::Value{s.v_uint64(1)};
                cdbError << "Unable to get value ID";
                return agw::Value();
            }
        }
        
        
        bool            exists(agw::Value v)
        {
            return exists_value(v.id);
        }
        
        bool            exists_value(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Vals WHERE id=? LIMIT 1");
            return s.present(i);
        }

        agw::Field           field(agw::Value v)
        {
            static thread_local SQ s("SELECT field FROM Vals WHERE id=?");
            return s.as<agw::Field>(v.id);
        }
        
        agw::Value::Info     info(agw::Value v)
        {
            agw::Value::Info ret;

            static thread_local SQ s("SELECT field, brief, data FROM Vals WHERE id=?");
            auto af = s.af();
            s.bind(1, v.id);
            if(s.step() == SqlQuery::Row){
                ret.field   = agw::Field{ s.v_uint64(1) };
                ret.brief   = s.v_string(2);
                ret.data    = s.v_string(3);
            }
            return ret;
        }
        
        agw::Value           value(uint64_t i)
        {
            return exists_value(i) ? agw::Value{i} : agw::Value{};
        }

        agw::ValueVector   values(agw::Field f, Sorted sorted)
        {
            static thread_local SQ qs("SELECT values FROM Vals WHERE field=? ORDER BY data");
            static thread_local SQ qu("SELECT values FROM Vals WHERE field=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Value>(f.id);
        }
        
        agw::ValueSet      values_set(agw::Field f)
        {
            static thread_local SQ s("SELECT values FROM Vals WHERE field=?");
            return s.set<agw::Value>(f.id);
        }
    }
}
