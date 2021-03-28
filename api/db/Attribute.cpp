////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Attribute.hpp"
#include "Document.hpp"
#include "Workspace.hpp"

#include <util/Array1.hpp>
#include <util/IntRange.hpp>
#include <util/Logging.hpp>
#include <util/SqlQuery.hpp>
#include <util/Utilities.hpp>
#include <util/Vector.hpp>

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

    namespace {
        using Range = IntRange<ssize_t>;
    
        template <typename T>
        Range   fullRange(const Vector<T>& vals)
        {
            return Range{0, (ssize_t) vals.size()};
        }
        
        struct Match {
            Range       x, y;
            ssize_t     D;
            bool        good;
        };
        
            // Based on https://github.com/RobertElderSoftware/roberteldersoftwarediff/blob/master/myers_diff_and_variations.py
        struct DiffEngine {
            const Vector<KVUA>&     A;
            const Vector<KeyValue>& B;
            Vector<KVDiff>&         C;
            Array1<ssize_t>         Vf, Vb;
            
            
            DiffEngine(const Vector<KVUA>& a, const Vector<KeyValue>& b, Vector<KVDiff>& c) : A(a), B(b), C(c)
            {
                ssize_t mx  = a.size() + b.size() + 1;
                Vf.resize(-mx-1, mx+2, 0ULL);
                Vb.resize(-mx-1, mx+2, 0ULL);
            }
            
            void    add(Range Y)
            {
                for(ssize_t y : Y){
                    const KeyValue& kv  = B[y];
                    KVDiff& c = C.emplace_back();
                    
                    c.key   = kv.key.qString();
                    c.value = kv.data.qString();
                    c.uid   = kv.id.qString();
                    c.nidx  = (uint64_t) y;
                    c.chg   =  KVDiff::INSERT;
                }
            }
            
            void    remove(Range X)
            {
                for(ssize_t x : X){
                    const KVUA& kv = A[x];
                    KVDiff& c = C.emplace_back();
                    
                    c.key   = kv.key;
                    c.value = kv.value;
                    c.uid   = kv.uid;
                    c.attr  = kv.attr;
                    c.oidx  = (uint64_t) x;
                    c.chg   = KVDiff::DELETE;
                }
            }
            
            void    untouch(Range X, Range Y)
            {
                assert(X.count() == Y.count());
                for(ssize_t  r : Range{0,std::min(X.count(),Y.count())}){
                    auto& c = C.emplace_back();
                    c.nidx  = Y.low+r;
                    c.oidx  = X.low+r;

                    auto& a = A[c.oidx];
                    auto& b = B[c.nidx];
                    c.key   = b.key.qString();
                    c.value = b.data.qString();
                    c.uid   = b.id.qString();
                    c.attr  = a.attr;
                    c.chg   = 0;
                }
            }
            
            bool    same(ssize_t x, ssize_t y) const
            {
                const auto& a   = A[x];
                const auto& b   = B[y];
                return (a.key == b.key.qString()) && (a.value == b.data.qString()) && (a.uid == b.id.qString());
            }
            
            bool    similar(ssize_t x, ssize_t y) const
            {
                const auto& a   = A[x];
                const auto& b   = B[y];
                return is_similar(a.key, b.key.qString()) && is_similar(a.value, b.data.qString()) && is_similar(a.uid, b.id.qString());
            }
            
            
            Match   middleSnake(const IntRange<ssize_t>& X, const IntRange<ssize_t>& Y)
            {
                ssize_t N       = X.count();
                ssize_t M       = Y.count();
                ssize_t mx      = N + M;
                ssize_t delta   = N - M;
                ssize_t         x, y, xi, yi, D, k;

                for(D = 0; D <= mx/2+(mx&1); ++D){
                    for(k=-D;k<=D;k+=2){
                        if((k==-D) || (k!=D && (Vf[k-1] < Vf[k+1]))){
                            x   = Vf[k+1];
                        } else
                            x   = Vf[k-1]+1;
                        y   = x - k;
                        xi  = x;
                        yi  = y;
                        while((x<N) && (y<M) && similar(X.low+x, Y.low+y))
                            ++x, ++y;
                        Vf[k]   = x;
                        
                        int64_t knd = k - delta;
                        if((delta&1) && (-knd >= -(D-1)) && (-knd <= (D-1))){
                            if(Vf[k] + Vb[-knd] >= N){
                                return Match{ 
                                    range(xi+X.low, x+X.low),
                                    range(yi+Y.low, y+Y.low),
                                    2*D-1, true
                                };
                            }
                        }
                    }
                    for(k=-D;k<=D;k+=2){
                        if((k==-D) || (k!=D && (Vf[k-1] < Vf[k+1]))){
                            x   = Vb[k+1];
                        } else
                            x   = Vb[k-1]+1;
                        y   = x - k;
                        xi  = x;
                        yi  = y;
                        while((x<N) && (y<M) && similar(X.low+N-x-1, Y.low+M-y-1))
                            ++x, ++y;
                        Vb[k]   = x;
                        int64_t knd = k - delta;
                        if((delta&1) && (-knd >= --D) && (-knd <= D)){
                            if(Vb[k]+Vf[-knd] >= N){
                                return Match{
                                    range(X.low+N-x, X.low+N-xi),
                                    range(Y.low+M-y, Y.low+M-yi),
                                    2*D, true
                                };
                            }
                        }
                    }
                }
                return Match{};
            }
            
            bool            compare(Range X, Range Y)
            {
                if(X.count() <= 0){
                    if(Y.count() <= 0){
                        return false;
                    } else {
                        add(Y);
                        return true;
                    }
                } else {
                    if(Y.count() <= 0){
                        remove(X);
                        return true;
                    } else {
                    
                        Match   m   = middleSnake(X,Y);
                        assert(m.good);
                        if((m.D > 1) || (m.x.not_empty()  && m.y.not_empty())){ // can be further subdivided
                            bool    rtn = false;
                            rtn = compare(range(X.low, m.x.low), range(Y.low, m.y.low)) || rtn; // before....
                            untouch(m.x, m.y);
                            rtn = compare(range(m.x.high, X.high), range(m.y.high, Y.high)) || rtn;
                            return rtn;
                        } else if(Y.count() > X.count()){
                            //  There's a single insertion (at the end)
                            untouch(m.x, m.y);
                            add(range(m.y.high, Y.high));
                            return true;
                        } else if(Y.count() < X.count()){
                            //  There's a single deletion
                            untouch(m.x, m.y);
                            remove(range(m.x.high, X.high));
                            return true;
                        } else {
                            //  All unmodified 
                            untouch(m.x, m.y);
                            return false;
                        }
                    }
                }
                return false;
            }

            bool            compare(ssize_t depth)
            {
                return compare(fullRange(A), fullRange(B));
            }
            
        };
        
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
            DiffEngine      de(old, kvs.subs, items);
            bool chg      = de.compare(depth);
            
            for(auto& r : items){
                if(r.oidx != r.nidx)
                    r.chg         |= KVDiff::INDEX;
                if(r.unchanged() && !de.same(r.oidx, r.nidx)){
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
    
    KVU                 kvu(Attribute a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT k,value,uid FROM Attributes WHERE id=?");
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
            static thread_local SqlQuery s(wksp::cache(), "SELECT k,value,uid,id,idx FROM Attributes WHERE parent=? ORDER BY idx");
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
        static thread_local SqlQuery s(wksp::cache(), "SELECT k,value,uid,id,idx FROM Attributes WHERE doc=? AND parent=0 ORDER BY idx");
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
