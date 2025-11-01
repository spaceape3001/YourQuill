////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/character/CharacterProvider.hpp>
#include <mithril/character/CharacterCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_characters()
    {
        return [](){
            return ids(cdb::all_characters(Sorted::YES));
        };
    }
}

