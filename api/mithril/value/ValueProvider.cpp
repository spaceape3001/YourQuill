////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/value/ValueProvider.hpp>
#include <mithril/value/ValueCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_values()
    {
        return [](){
            return ids(cdb::all_values());
        };
    }
}

