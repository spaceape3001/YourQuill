////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/docsys/leaf.hpp>
#include <db/bit/context.hpp>
#include <db/enum/format.hpp>

//  temporary....
#include <yq/bit/KeyValue.hpp>

namespace yq {

    struct Leaf::KV {
        std::string             key;
        std::string             cmd;
        std::string             id;
        std::string             data;
        std::string             type;
        std::string             at, until;
        std::vector<KV>         subs;
        
        bool                    operator==(const KV&) const = default;
        KV&                     operator=(const KeyValue&);
    };

    struct Leaf::Data {
        std::string             title;
        string_set_t            classes, tags;
        std::string             brief, notes, abbr;
        std::vector<KV>         attrs;
        std::vector<Context>    context;
        
        Data&   merge(const Data&, bool fOverride=false);
        bool operator==(const Data&) const = default;
    };
}
