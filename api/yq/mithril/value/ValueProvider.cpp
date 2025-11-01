////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/value/ValueProvider.hpp>
#include <yq/mithril/value/ValueCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_values()
    {
        return [](){
            return ids(cdb::all_values());
        };
    }
}

