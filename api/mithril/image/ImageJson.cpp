////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/image/Image.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/image/ImageJson.hpp>
#include <nlohmann/json.hpp>

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
