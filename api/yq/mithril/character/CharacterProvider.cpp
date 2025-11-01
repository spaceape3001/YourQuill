////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/character/CharacterProvider.hpp>
#include <yq/mithril/character/CharacterCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_characters()
    {
        return [](){
            return ids(cdb::all_characters(Sorted::YES));
        };
    }
}

