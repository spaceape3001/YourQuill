////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/document/DocumentProvider.hpp>
#include <mithril/document/DocumentCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_documents()
    {
        return [](){
            return ids(cdb::all_documents(Sorted::YES));
        };
    }
}

