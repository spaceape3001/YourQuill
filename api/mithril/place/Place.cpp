////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "PlaceCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Place)
YQ_TYPE_IMPLEMENT(yq::mithril::PlaceSet)
YQ_TYPE_IMPLEMENT(yq::mithril::PlaceVector)

namespace yq::mithril {
    void reg_place_meta()
    {
        {
            auto w  = writer<Place>();
            w.property("id", READ_ONLY, &Place::id);
            w.property("key", (std::string(*)(Place)) cdb::key);
        }
    }

    YQ_INVOKE(reg_place_meta();)
}
