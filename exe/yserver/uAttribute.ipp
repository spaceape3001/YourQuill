////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    Attribute db_attribute(Document doc, Attribute par, uint64_t idx, const String& k, const String& uid, const String& val)
    {
        static thread_local SqlQuery i(wksp::cache(), "INSERT INTO Attributes (doc,parent,idx,k,uid,value) VALUES (?,?,?,?,?,?)");
        auto s_af = i.af();
        i.bind(0, doc.id);
        i.bind(1, par.id);
        i.bind(2, idx);
        i.bind(3, k);
        i.bind(4, uid);
        i.bind(5, val);
        if(i.exec())
            return Attribute{i.lastInsertIdU64()};
        return Attribute{};
    }
    
    void        u_insert(Document doc, Attribute par, Vector<cdb::KVDiff>& items)
    {
        for(auto& d : items){
            if((d.chg & cdb::KVDiff::XMASK) == cdb::KVDiff::INSERT)
                d.attr  = db_attribute(doc, par, d.nidx, d.key, d.uid, d.value);
            u_insert(doc, d.attr, d.sub);
        }
    }
    
    void        f_index(QVariantList&attr, QVariantList& idx, const Vector<cdb::KVDiff>& items)
    {
        for(auto& d: items){
            if(d.attr && d.indexed() && !d.deleted()){
                attr << (quint64) d.attr.id;
                idx << (quint64) d.nidx;
            }
            
            if(d.subchanged())
                f_index(attr, idx, d.sub);
        }
    }
    
    void        u_index(Vector<cdb::KVDiff>& items)
    {
        QVariantList    attrs, idx;
        f_index(attrs, idx, items);
        if(!attrs.isEmpty() && (attrs.count() == idx.count())){
            static thread_local SqlQuery u(wksp::cache(), "UPDATE Attributes SET idx=? WHERE id=?");
            auto u_af   = u.af();
            u.addBindValue(idx);
            u.addBindValue(attrs);
            u.batch();
        }
    }
    
    void        f_delete(QVariantList& attr, const Vector<cdb::KVDiff>& items)
    {
        for(auto& d : items){
            if(d.deleted())
                attr << (quint64) d.attr.id;
            if(d.subchanged())
                f_delete(attr, d.sub);
        }
    }
    
    void        u_delete(Vector<cdb::KVDiff>&items)
    {
        QVariantList    attrs;
        f_delete(attrs, items);
        if(!attrs.isEmpty()){
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM Attributes WHERE id=?");
            auto d_af = d.af();
            d.addBindValue(attrs);
            d.batch();
        }
    }
}


cdb::KVReport   attr_changes(Document doc, const KVTree& kv, bool fRecurse)
{
    cdb::KVReport   ret = cdb::changes(doc, kv, fRecurse ? -1 : 0);
    u_insert(doc, Attribute{}, ret.items);
    u_index(ret.items);
    return ret;
}

cdb::KVReport   attr_changes(Attribute attr, const KVTree& kv, bool fRecurse)
{
    cdb::KVReport   ret = cdb::changes(attr, kv, fRecurse ? -1 : 0);
    u_insert(cdb::document(attr), attr, ret.items);
    u_index(ret.items);
    return ret;
}


void            execute_removals(cdb::KVReport&rep)
{
    u_delete(rep.items);
}
