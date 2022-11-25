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
        std::vector<Attribute>   all_attributes()
        {
            static thread_local SQ s("SELECT id FROM Attributes");
            return s.vec<Attribute>();
        }
        
        std::vector<Attribute>   all_attributes(Document d) 
        {
            static thread_local SQ s("SELECT id FROM Attributes WHERE doc=?");
            return s.vec<Attribute>(d.id);
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
        std::vector<Attribute>   attributes(Document d)
        {
            static thread_local SQ s("SELECT id FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
            return s.vec<Attribute>(d.id);
        }
        
        
        size_t              attributes_count(Document d)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Attributes WHERE doc=? AND parent=0");
            return s.size(d.id);
        }

        std::vector<Attribute>   child_attributes(Attribute a)
        {
            static thread_local SQ s("SELECT id FROM Attributes WHERE parent=? ORDER BY idx");
            return s.vec<Attribute>(a.id);
        }

        size_t              child_attributes_count(Attribute a)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Attributes WHERE parent=?");
            return s.size(a.id);
        }

        Attribute db_attribute(Document doc, Attribute par, uint64_t idx, std::string_view k, std::string_view uid, std::string_view val)
        {
            static thread_local SQ i("INSERT INTO Attributes (doc,parent,idx,k,uid,value) VALUES (?,?,?,?,?,?)");
            i.bind(1, doc.id);
            i.bind(2, par.id);
            i.bind(3, idx);
            i.bind(4, k);
            i.bind(5, uid);
            i.bind(6, val);
            if(i.exec())
                return Attribute{(uint64_t) i.last_id()};
            return Attribute{};
        }

        Document            document(Attribute a)
        {
            static thread_local SQ s("SELECT doc FROM Attributes WHERE id=?");
            return s.as<Document>(a.id);
        }
        
        void                erase_all_attributes(Document doc)
        {
            static thread_local SQ x("DELETE FROM Attributes WHERE doc=?");
            x.exec(doc.id);
        }

        bool                exists(Attribute a)
        {
            return exists_attribute(a.id);
        }
        
        bool                exists_attribute(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Attributes WHERE id=?");
            return s.present(i);
        }

        //! Index in the file's list
        uint64_t            index(Attribute a)
        {
            static thread_local SQ s("SELECT idx FROM Attributes WHERE id=?");
            return s.u64(a.id);
        }
        

        Attribute::Info         info(Attribute a)
        {
            Attribute::Info     ret;
            static thread_local SQ s("SELECT k,value,uid,idx,doc,parent FROM Attributes WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SqlQuery::Row){
                ret.key    = s.v_string(1);
                ret.value  = s.v_string(2);
                ret.uid    = s.v_string(3);
                ret.idx    = s.v_uint64(4);
                ret.doc    = Document{ s.v_uint64(5) };
                ret.parent = Attribute{ s.v_uint64(6) };
            }
            return ret;
        }

        std::string          key(Attribute a)
        {
            static thread_local SQ s("SELECT k FROM Attributes WHERE id=?");
            return s.str(a.id);
        }

        std::vector<Attribute::KVUA>    kvua(Attribute a)
        {
            std::vector<Attribute::KVUA> ret;
            static thread_local SQ s("SELECT k,value,uid,id,idx FROM Attributes WHERE parent=? ORDER BY idx");
            auto s_af = s.af();
            s.bind(1, a.id);
            while(s.step() == SqlQuery::Row){
                ret.push_back(Attribute::KVUA{
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
        std::vector<Attribute::KVUA>    kvua(Document d)
        {
            std::vector<Attribute::KVUA> ret;
            static thread_local SQ s("SELECT k,value,uid,id,idx FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
            auto s_af = s.af();
            s.bind(1, d.id);
            while(s.step() == SqlQuery::Row){
                ret.push_back(Attribute::KVUA{
                    s.v_string(1),
                    s.v_string(2),
                    s.v_string(3),
                    s.v_uint64(4),
                    s.v_uint64(5)
                });
            }
            return ret;
        }
        
        Attribute           parent(Attribute a)
        {
            static thread_local SQ s("SELECT parent FROM Attributes WHERE id=?");
            return s.as<Attribute>(a.id);
        }

        std::string             uid(Attribute a)
        {
            static thread_local SQ s("SELECT uid FROM Attributes WHERE id=?");
            return s.str(a.id);
        }
        
        std::string             value(Attribute a)
        {
            static thread_local SQ s("SELECT value FROM Attributes WHERE id=?");
            return s.str(a.id);
        }
    }
}
