////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Attribute.hpp"
#include "CacheUtil.hpp"
#include "Document.hpp"

#include <util/DiffEngine.hpp>
#include <util/Logging.hpp>
#include <util/Utilities.hpp>
#include <util/Vector.hpp>

namespace cdb {

    Vector<Attribute>   all_attributes()
    {
        static thread_local SQ s("SELECT id FROM Attributes");
        return s.vec<Attribute>();
    }
    
    Vector<Attribute>   all_attributes(Document d) 
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

    //! Gets attributes (sub)
    Vector<Attribute>   attributes(Attribute a)
    {
        static thread_local SQ s("SELECT id FROM Attributes WHERE parent=? ORDER BY idx");
        return s.vec<Attribute>(a.id);
    }

    //! Gets top-level attirbutes
    Vector<Attribute>   attributes(Document d)
    {
        static thread_local SQ s("SELECT id FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
        return s.vec<Attribute>(d.id);
    }
    
    size_t              attributes_count(Attribute a)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM Attributes WHERE parent=?");
        return s.size(a.id);
    }
    
    size_t              attributes_count(Document d)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM Attributes WHERE doc=? AND parent=0");
        return s.size(d.id);
    }

    namespace {
        bool    is_same(const KVUA& a, const KeyValue& b) 
        {
            return (a.key == b.key.qString()) && (a.value == b.data.qString()) && (a.uid == b.id.qString());
        }
        
        bool    is_similar_kv(const KVUA& a, const KeyValue& b) 
        {
            return ::is_similar(a.key, b.key.qString()) && ::is_similar(a.value, b.data.qString()) && is_similar(a.uid, b.id.qString());
        }
        
        void                add_all(Vector<KVDiff>& items, const KVTree& kvs, ssize_t depth)
        {
            for(ssize_t i=0;i<(ssize_t) kvs.subs.size(); ++i){
                auto& kv    = items.emplace_back();
                kv.chg      = KVDiff::INSERT;
                kv.key      = kvs.subs[i].key.qString();
                kv.value    = kvs.subs[i].data.qString();
                kv.uid      = kvs.subs[i].id.qString();
                kv.nidx     = i;
                
                if(depth && !kvs.subs[i].empty()){
                    kv.chg |= KVDiff::SUB;
                    add_all(kv.sub, kvs.subs[i], depth-1);
                }
            }
        }

        void                remove_all(Vector<KVDiff>& items, const Vector<KVUA>& kvs, ssize_t depth)
        {
            for(const KVUA& k : kvs){
                auto& kv    = items.emplace_back();
                kv.chg      = KVDiff::DELETE;
                kv.key      = k.key;
                kv.value    = k.value;
                kv.attr     = k.attr;
                kv.uid      = k.uid;
                kv.oidx     = k.idx;
                
                if(depth){
                    auto old    = kvua(k.attr);
                    if(!old.empty()){
                        remove_all(kv.sub, old, depth-1);
                        kv.chg |= KVDiff::SUB;
                    }
                }
            }
        }
   
        bool                difference(Vector<KVDiff>& items, const Vector<KVUA>& old, const KVTree& kvs, ssize_t depth)
        {
            bool    chg     = diff_it(old, kvs.subs, is_similar_kv, 
                [&](SSizeRange Y){
                    for(ssize_t y  : Y){
                        auto& kv    = items.emplace_back();
                        kv.chg      = KVDiff::INSERT;
                        kv.key      = kvs.subs[y].key.qString();
                        kv.value    = kvs.subs[y].data.qString();
                        kv.uid      = kvs.subs[y].id.qString();
                        kv.nidx     = y;
                    }
                }, 
                [&](SSizeRange X){
                    for(ssize_t x  : X){
                        auto& kv    = items.emplace_back();
                        kv.chg      = KVDiff::DELETE;
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
                        c.key   = b.key.qString();
                        c.value = b.data.qString();
                        c.uid   = b.id.qString();
                        c.attr  = a.attr;
                        c.chg   = 0;
                    }
                }
            );
            
            for(auto& r : items){
                if(r.oidx != r.nidx)
                    r.chg         |= KVDiff::INDEX;
                if(r.unchanged() && !is_same(old[r.oidx], kvs.subs[r.nidx])){
                    r.chg |= KVDiff::MODIFY;
                    chg  = true;
                }
            }
            
            if(depth--){
                for(auto& r : items){
                    if(difference(r.sub, kvua(r.attr), kvs.subs.valid(r.nidx) ?  kvs.subs[r.nidx] : KeyValue{}, depth)){
                        r.chg |= KVDiff::SUB;
                        chg = true;
                    }
                }
            }
            return chg;
        }
    }

    KVReport changes(Document doc, const KVTree& kv, ssize_t depth)
    {
        KVReport    ret;
        ret.changed = difference(ret.items, kvua(doc), kv, depth);
        ret.parent  = doc;
        return ret;
    }
    
    KVReport changes(Attribute attr, const KVTree& kv, ssize_t depth)
    {
        KVReport    ret;
        ret.changed =  difference(ret.items, kvua(attr), kv, depth);
        ret.parent  = attr;
        return ret;
    }
    
    KVReport changes(const Vector<KVUA>& old, const KVTree&kv, ssize_t depth)
    {
        KVReport    ret;
        ret.changed =  difference(ret.items, old, kv, depth);
        ret.parent  = false;
        return ret;
    }

    Document            document(Attribute a)
    {
        static thread_local SQ s("SELECT doc FROM Attributes WHERE id=?");
        return s.as<Document>(a.id);
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
    

    QString             key(Attribute a)
    {
        static thread_local SQ s("SELECT k FROM Attributes WHERE id=?");
        return s.str(a.id);
    }
    
    KVU                 kvu(Attribute a)
    {
        static thread_local SQ s("SELECT k,value,uid FROM Attributes WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return KVU{ s.valueString(0), s.valueString(1), s.valueString(2)};
        return KVU{};
    }

    Vector<KVUA>        kvua(Attribute a)
    {
        Vector<KVUA>    ret;
        if(a){
            static thread_local SQ s("SELECT k,value,uid,id,idx FROM Attributes WHERE parent=? ORDER BY idx");
            auto s_af = s.af();
            s.bind(0, a.id);
            if(s.exec()){
                while(s.next()){
                    ret << KVUA{
                        s.valueString(0),
                        s.valueString(1),
                        s.valueString(2),
                        s.valueU64(3),
                        s.valueU64(4)
                    };
                }
            }
        }
        return ret;
    }
    
    Vector<KVUA>        kvua(Document d)
    {
        Vector<KVUA>    ret;
        static thread_local SQ s("SELECT k,value,uid,id,idx FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
        auto s_af = s.af();
        s.bind(0, d.id);
        if(s.exec()){
            while(s.next()){
                ret << KVUA{
                    s.valueString(0),
                    s.valueString(1),
                    s.valueString(2),
                    s.valueU64(3),
                    s.valueU64(4)
                };
            }
        }
        return ret;
    }

    DocOrAttr           parent(Attribute a)
    {
        static thread_local SQ s("SELECT parent,doc FROM Attributes WHERE id=?");
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
        static thread_local SQ s("SELECT uid FROM Attributes WHERE id=?");
        return s.str(a.id);
    }
    
    QString             value(Attribute a)
    {
        static thread_local SQ s("SELECT value FROM Attributes WHERE id=?");
        return s.str(a.id);
    }
}
