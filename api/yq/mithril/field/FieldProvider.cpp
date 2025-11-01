////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/field/FieldProvider.hpp>
#include <yq/mithril/field/FieldCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_fields()
    {
        return [](){
            return ids(cdb::all_fields(Sorted::YES));
        };
    }
}

