////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/document/DocumentUpdate.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    void UDocument::u_icon(Document x,Image img)
    {
        static thread_local CacheQuery sql("UPDATE Documents SET icon=? WHERE id=?");
        sql.exec(img.id, x.id);
    }

    UDocument::UDocument(Document doc) : U<Document>(doc, cdb::key(doc))
    {
    }
}


