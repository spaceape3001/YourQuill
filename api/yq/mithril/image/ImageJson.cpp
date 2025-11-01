////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/image/ImageJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Image x)
    {
        Image::Info  i   = cdb::info(x);
        json j{
            { "fragment", x.id },
            { "id", x.id },
            { "key", cdb::key(x) },
            { "height", i.dim.y },
            { "mime", mimeType(i.type) },
            { "raster", cdb::is_raster(i.type) },
            { "type", i.type.key() },
            { "width", i.dim.x }
        };
        return j;
    }
    
    json json_(const ImageVector& xs)
    {
        json j = json::array();
        for(Image x : xs)
            j.push_back(json_(x));
        return j;
    }
}
