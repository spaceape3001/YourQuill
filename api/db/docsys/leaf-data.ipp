////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    Leaf::Data&   Leaf::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(title, b.title, fOverride);
        set_if_empty(abbr, b.abbr, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        notes += b.notes;
        classes += b.classes;
        tags += b.tags;
        attrs += b.attrs;
        context += b.context;
        return *this;
    }

    Leaf::KV&  Leaf::KV::operator=(const KeyValue& kv)
    {
        key     = kv.key;
        cmd     = kv.cmd;
        id      = kv.id;
        data    = kv.data;
        
        if(!kv.subs.empty()){
            subs.reserve(kv.subs.size());
            for(const KeyValue& k : kv.subs){
                if(is_similar(k.key, "start")){
                    at      = kv.data;
                } else if(is_similar(k.key, "end")){
                    until   = kv.data;
                } else {
                    KV kv2;
                    kv2 = k;
                    subs.push_back(kv2);
                }
            }
        }
        return *this;
    }
}
