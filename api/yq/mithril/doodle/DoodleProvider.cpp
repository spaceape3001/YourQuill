////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/doodle/DoodleProvider.hpp>
#include <yq/mithril/doodle/DoodleCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_doodles()
    {
        return [](){
            return ids(cdb::all_doodles(Sorted::YES));
        };
    }
}

