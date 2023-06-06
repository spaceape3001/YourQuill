////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "value/ValueArg.ipp"
#include "value/ValueCDB.ipp"
#include "value/ValueData.ipp"
#include "value/ValueFile.ipp"
#include "value/ValueHtml.ipp"

#include "db/IDLock.ipp"

namespace yq::mithril {
    template class IDLock<Value>;
}
