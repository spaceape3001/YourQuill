////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Attribute.hpp"
#include "AttributeArg.hpp"
#include "AttributeCDB.hpp"
#include "AttributeDiff.hpp"
#include "AttributeHtml.hpp"

#include <basic/IntRange.hpp>

#include <basic/preamble.hpp>
#include <basic/DiffEngine.hpp>
#include <yq/bit/KeyValue.hpp>
#include <yq/db/SQ.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>


namespace yq {
////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Attribute attribute(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Attribute{};
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_attribute(i))
                return Attribute{i};
            return Attribute{};
        }
        
        Attribute attribute(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return attribute(k);
            k       = ctx.find_query("attr");
            if(!k.empty())
                return attribute(k);
            return Attribute{};
        }
        
        
        Attribute attribute(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return attribute(arg_string);
        }
        
        Attribute attribute(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return attribute(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

    namespace diff {
        namespace {
            bool    is_same(const Attribute::KVUA& a, const KeyValue& b) 
            {
                return (a.key == b.key) && (a.value == b.data) && (a.uid == b.id);
            }
            
            bool    is_similar_kv(const Attribute::KVUA& a, const KeyValue& b) 
            {
                return is_similar(a.key, b.key) && is_similar(a.value, b.data) && is_similar(a.uid, b.id);
            }
            
            void                add_all(std::vector<Attribute::Diff>& items, const KVTree& kvs, ssize_t depth)
            {
                for(ssize_t i=0;i<(ssize_t) kvs.subs.size(); ++i){
                    auto& kv    = items.emplace_back();
                    kv.chg      = Attribute::Diff::INSERT;
                    kv.key      = kvs.subs[i].key;
                    kv.value    = kvs.subs[i].data;
                    kv.uid      = kvs.subs[i].id;
                    kv.nidx     = i;
                    
                    if(depth && !kvs.subs[i].empty()){
                        kv.chg |= Attribute::Diff::SUB;
                        add_all(kv.sub, kvs.subs[i], depth-1);
                    }
                }
            }

            void                remove_all(std::vector<Attribute::Diff>& items, const std::vector<Attribute::KVUA>& kvs, ssize_t depth)
            {
                for(const Attribute::KVUA& k : kvs){
                    auto& kv    = items.emplace_back();
                    kv.chg      = Attribute::Diff::DELETE;
                    kv.key      = k.key;
                    kv.value    = k.value;
                    kv.attr     = k.attr;
                    kv.uid      = k.uid;
                    kv.oidx     = k.idx;
                    
                    if(depth){
                        auto old    = cdb::kvua(k.attr);
                        if(!old.empty()){
                            remove_all(kv.sub, old, depth-1);
                            kv.chg |= Attribute::Diff::SUB;
                        }
                    }
                }
            }
       
            bool                difference(std::vector<Attribute::Diff>& items, const std::vector<Attribute::KVUA>& old, const KVTree& kvs, ssize_t depth)
            {
                bool    chg     = diff_it(old, kvs.subs, is_similar_kv, 
                    [&](SSizeRange Y){
                        for(ssize_t y  : Y){
                            auto& kv    = items.emplace_back();
                            kv.chg      = Attribute::Diff::INSERT;
                            kv.key      = kvs.subs[y].key;
                            kv.value    = kvs.subs[y].data;
                            kv.uid      = kvs.subs[y].id;
                            kv.nidx     = y;
                        }
                    }, 
                    [&](SSizeRange X){
                        for(ssize_t x  : X){
                            auto& kv    = items.emplace_back();
                            kv.chg      = Attribute::Diff::DELETE;
                            kv.key      = old[x].key;
                            kv.value    = old[x].value;
                            kv.attr     = old[x].attr;
                            kv.uid      = old[x].uid;
                            kv.oidx     = x;
                        }
                    }, 
                    [&](SSizeRange X, SSizeRange Y){
                        assert(X.count() == Y.count());
                        for(ssize_t  r : SSizeRange{0,std::min(X.count(),Y.count())}){
                            auto& c = items.emplace_back();
                            c.nidx  = Y.low+r;
                            c.oidx  = X.low+r;

                            auto& a = old[c.oidx];
                            auto& b = kvs.subs[c.nidx];
                            c.key   = b.key;
                            c.value = b.data;
                            c.uid   = b.id;
                            c.attr  = a.attr;
                            c.chg   = 0;
                        }
                    }
                );
                
                for(auto& r : items){
                    if(r.oidx != r.nidx)
                        r.chg         |= Attribute::Diff::INDEX;
                    if(r.unchanged() && !is_same(old[r.oidx], kvs.subs[r.nidx])){
                        r.chg |= Attribute::Diff::MODIFY;
                        chg  = true;
                    }
                }
                
                if(depth--){
                    for(auto& r : items){
                        if(difference(r.sub, cdb::kvua(r.attr), kvs.subs.valid(r.nidx) ?  kvs.subs[r.nidx] : KeyValue{}, depth)){
                            r.chg |= Attribute::Diff::SUB;
                            chg = true;
                        }
                    }
                }
                return chg;
            }
        }

        Attribute::Report  additions(Document doc, const KVTree& kv, ssize_t depth)
        {
            Attribute::Report    ret;
            ret.changed     = !kv.empty();
            add_all(ret.items, kv, depth);
            ret.document    = doc;
            return ret;
        }

        Attribute::Report changes(Document doc, const KVTree& kv, ssize_t depth)
        {
            Attribute::Report    ret;
            ret.changed     = difference(ret.items, cdb::kvua(doc), kv, depth);
            ret.document    = doc;
            return ret;
        }
        
        Attribute::Report changes(Attribute attr, const KVTree& kv, ssize_t depth)
        {
            Attribute::Report    ret;
            ret.changed     = difference(ret.items, cdb::kvua(attr), kv, depth);
            ret.parent      = attr;
            ret.document    = cdb::document(attr);
            return ret;
        }
        
        Attribute::Report changes(const std::vector<Attribute::KVUA>& old, const KVTree&kv, ssize_t depth)
        {
            Attribute::Report    ret;
            ret.changed =  difference(ret.items, old, kv, depth);
            return ret;
        }

    }
    
    namespace cdb {
        void        u_insert(Document doc, Attribute par, std::vector<Attribute::Diff>& items)
        {
            for(auto& d : items){
                if((d.chg & Attribute::Diff::XMASK) == Attribute::Diff::INSERT)
                    d.attr  = db_attribute(doc, par, d.nidx, d.key, d.uid, d.value);
                u_insert(doc, d.attr, d.sub);
            }
        }
        
        void        u_index(const std::vector<Attribute::Diff>& items)
        {
            static thread_local SQ u("UPDATE Attributes SET idx=? WHERE id=?");
            for(auto& d : items){
                if(d.attr && d.indexed() && !d.deleted())
                    u.exec(d.nidx, d.attr.id);
                if(d.subchanged())
                    u_index(d.sub);
            }
        }
        
        void        u_delete(const std::vector<Attribute::Diff>&items)
        {
            static thread_local SQ x("DELETE FROM Attributes WHERE id=?");
            for(auto& d : items){
                if(d.deleted())
                    x.exec(d.attr.id);
                if(d.subchanged())
                    u_delete(d.sub);
            }
        }        
    }

    size_t  Attribute::Diff::total_items() const
    {
        size_t  c   = sub.size();
        for(const Diff &d : sub)
            c += d.total_items();
        return c;
    }

    size_t  Attribute::Report::total_items() const
    {
        size_t   c = items.size();
        for(const Diff& d : items)
            c += d.total_items();
        return c;
    }

    void    Attribute::Report::exec_inserts()
    {
        cdb::u_insert(document, parent, items);
    }
    
    void    Attribute::Report::exec_reindex()
    {
        cdb::u_index(items);
    }
    
    void    Attribute::Report::exec_removals()
    {
        cdb::u_delete(items);
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        void    dev_table(WebHtml&h, const std::vector<Attribute>&table)
        {
            auto ta = h.table();
            ta << "<TR><TH>ID</TH><TH>Key</TH><TH>Value</TH></TR>\n";
            for(Attribute a : table){
                auto i = cdb::info(a);
                ta << "<tr><td>" << a.id << "</td><td>" << i.key << "</td><td>";
                html_escape_write(h, i.value);
                ta << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
}


