////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include "character/CharacterArg.ipp"
#include "character/CharacterCDB.ipp"
#include "character/CharacterHtml.ipp"
//#include "character/CharacterPost.ipp"
#include "character/CharacterProvider.ipp"

#include "db/IDLock.ipp"

namespace yq::mithril {
    template class IDLock<Character>;
}
