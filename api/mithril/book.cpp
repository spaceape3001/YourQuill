////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include "book/BookArg.ipp"
#include "book/BookCDB.ipp"
#include "book/BookHtml.ipp"
//#include "book/BookPost.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Book)

namespace yq::mithril {
    template class IDLock<Book>;
}
