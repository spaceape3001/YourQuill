////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/document/DocumentProvider.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_documents()
    {
        return [](){
            return ids(cdb::all_documents(Sorted::YES));
        };
    }
}

