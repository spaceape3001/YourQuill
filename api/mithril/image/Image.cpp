////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ImageCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Image)
YQ_TYPE_IMPLEMENT(yq::mithril::ImageSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ImageVector)

namespace yq::mithril {
    void reg_image_meta()
    {
        {
            auto w  = writer<Image>();
            w.property("id", READ_ONLY, &Image::id);
            w.property("key", (std::string(*)(Image)) cdb::key);
        }
    }

    YQ_INVOKE(reg_image_meta();)
}
