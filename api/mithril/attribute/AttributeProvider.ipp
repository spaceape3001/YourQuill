////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/attribute/AttributeProvider.hpp>
#include <mithril/document/Document.hpp>

namespace yq::mithril::provider {
    IdProvider  all_attributes()
    {
        return [](){
            return ids(cdb::all_attributes());
        };
    }
    
    IdProvider  all_attributes(Document x)
    {
        return [x](){
            return ids(cdb::all_attributes(x));
        };
    }
    
    IdProvider  attributes(Document x)
    {
        return [x](){
            return ids(cdb::attributes(x));
        };
    }

    IdProvider  child_attributes(Attribute x)
    {
        return [x](){
            return ids(cdb::child_attributes(x));
        };
    }
}
