////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>
#include "DocumentCDB.hpp"
#include <mithril/attribute/Attribute.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/folder/Folder.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Document)
YQ_TYPE_IMPLEMENT(yq::mithril::DocumentSet)
YQ_TYPE_IMPLEMENT(yq::mithril::DocumentVector)

namespace yq::mithril {
    void reg_document_meta()
    {
        {
            auto w  = writer<Document>();
            w.property("attributes", (AttributeVector(*)(Document)) cdb::attributes);
            w.property("base", (std::string(*)(Document)) cdb::base_key);
            w.property("folder", (Folder(*)(Document)) cdb::folder);
            w.property("id", READ_ONLY, &Document::id);
            w.property("key", (std::string(*)(Document)) cdb::key);
            w.property("skey", (std::string(*)(Document)) cdb::skey);
            w.property("skeyb", (std::string(*)(Document)) cdb::skeyb);
            w.property("skeyc", (std::string(*)(Document)) cdb::skeyc);
        }
    }
    
    YQ_INVOKE(reg_document_meta();)
}
