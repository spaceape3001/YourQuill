////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/class/ClassProvider.hpp>
#include <yq/mithril/class/ClassCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_classes()
    {
        return [](){
            return ids(cdb::all_classes(Sorted::YES));
        };
    }
}

