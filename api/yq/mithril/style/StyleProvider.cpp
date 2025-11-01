////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/style/StyleProvider.hpp>
#include <yq/mithril/style/StyleCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_styles()
    {
        return [](){
            return ids(cdb::all_styles(Sorted::YES));
        };
    }
}

