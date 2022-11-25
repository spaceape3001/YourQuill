////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AttributeCDB.hpp"
#include <kernel/db/SQ.hpp>

namespace yq {
    namespace cdb {
        agw::AttributeVector   all_attributes()
        {
            static thread_local SQ s("SELECT id FROM Attributes");
            return s.vec<agw::Attribute>();
        }
        
        agw::AttributeVector   all_attributes(Document d) 
        {
            static thread_local SQ s("SELECT id FROM Attributes WHERE doc=?");
            return s.vec<agw::Attribute>(d.id);
        }
        
        size_t              all_attributes_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Attributes");
            return s.size();
        }
        
        size_t              all_attributes_count(Document d)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Attributes WHERE doc=?");
            return s.size(d.id);
        }


        //! Gets top-level attirbutes
        agw::AttributeVector   attributes(Document d)
        {
            static thread_local SQ s("SELECT id FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
            return s.vec<agw::Attribute>(d.id);
        }
        
        
        size_t              attributes_count(Document d)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Attributes WHERE doc=? AND parent=0");
            return s.size(d.id);
        }

        agw::AttributeVector   child_attributes(agw::Attribute a)
        {
            static thread_local SQ s("SELECT id FROM Attributes WHERE parent=? ORDER BY idx");
            return s.vec<agw::Attribute>(a.id);
        }

        size_t              child_attributes_count(agw::Attribute a)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Attributes WHERE parent=?");
            return s.size(a.id);
        }

        agw::Attribute db_attribute(Document doc, agw::Attribute par, uint64_t idx, std::string_view k, std::string_view uid, std::string_view val)
        {
            static thread_local SQ i("INSERT INTO Attributes (doc,parent,idx,k,uid,value) VALUES (?,?,?,?,?,?)");
            i.bind(1, doc.id);
            i.bind(2, par.id);
            i.bind(3, idx);
            i.bind(4, k);
            i.bind(5, uid);
            i.bind(6, val);
            if(i.exec())
                return agw::Attribute{(uint64_t) i.last_id()};
            return agw::Attribute{};
        }

        Document            document(agw::Attribute a)
        {
            static thread_local SQ s("SELECT doc FROM Attributes WHERE id=?");
            return s.as<Document>(a.id);
        }
        
        void                erase_all_attributes(Document doc)
        {
            static thread_local SQ x("DELETE FROM Attributes WHERE doc=?");
            x.exec(doc.id);
        }

        bool                exists(agw::Attribute a)
        {
            return exists_attribute(a.id);
        }
        
        bool                exists_attribute(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Attributes WHERE id=?");
            return s.present(i);
        }

        //! Index in the file's list
        uint64_t            index(agw::Attribute a)
        {
            static thread_local SQ s("SELECT idx FROM Attributes WHERE id=?");
            return s.u64(a.id);
        }
        

        agw::Attribute::Info         info(agw::Attribute a)
        {
            agw::Attribute::Info     ret;
            static thread_local SQ s("SELECT k,value,uid,idx,doc,parent FROM Attributes WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SqlQuery::Row){
                ret.key    = s.v_string(1);
                ret.value  = s.v_string(2);
                ret.uid    = s.v_string(3);
                ret.idx    = s.v_uint64(4);
                ret.doc    = Document{ s.v_uint64(5) };
                ret.parent = agw::Attribute{ s.v_uint64(6) };
            }
            return ret;
        }

        std::string          key(agw::Attribute a)
        {
            static thread_local SQ s("SELECT k FROM Attributes WHERE id=?");
            return s.str(a.id);
        }

        std::vector<agw::Attribute::KVUA>    kvua(agw::Attribute a)
        {
            std::vector<agw::Attribute::KVUA> ret;
            static thread_local SQ s("SELECT k,value,uid,id,idx FROM Attributes WHERE parent=? ORDER BY idx");
            auto s_af = s.af();
            s.bind(1, a.id);
            while(s.step() == SqlQuery::Row){
                ret.push_back(agw::Attribute::KVUA{
                    s.v_string(1),
                    s.v_string(2),
                    s.v_string(3),
                    s.v_uint64(4),
                    s.v_uint64(5)
                });
            }
            return ret;
        }
        
        //! Used in the change detection
        std::vector<agw::Attribute::KVUA>    kvua(Document d)
        {
            std::vector<agw::Attribute::KVUA> ret;
            static thread_local SQ s("SELECT k,value,uid,id,idx FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
            auto s_af = s.af();
            s.bind(1, d.id);
            while(s.step() == SqlQuery::Row){
                ret.push_back(agw::Attribute::KVUA{
                    s.v_string(1),
                    s.v_string(2),
                    s.v_string(3),
                    s.v_uint64(4),
                    s.v_uint64(5)
                });
            }
            return ret;
        }
        
        agw::Attribute           parent(agw::Attribute a)
        {
            static thread_local SQ s("SELECT parent FROM Attributes WHERE id=?");
            return s.as<agw::Attribute>(a.id);
        }

        std::string             uid(agw::Attribute a)
        {
            static thread_local SQ s("SELECT uid FROM Attributes WHERE id=?");
            return s.str(a.id);
        }
        
        std::string             value(agw::Attribute a)
        {
            static thread_local SQ s("SELECT value FROM Attributes WHERE id=?");
            return s.str(a.id);
        }
    }
}
