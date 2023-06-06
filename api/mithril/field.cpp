////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "field/Field.ipp"
#include "field/FieldArg.ipp"
#include "field/FieldCDB.ipp"
#include "field/FieldData.ipp"
#include "field/FieldFile.ipp"
#include "field/FieldHtml.ipp"
#include "field/FieldPost.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Field)

namespace yq::mithril {
    template class IDLock<Field>;
}
