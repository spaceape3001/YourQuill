////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/image/ImageSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    ImageVector  search(WebContext&ctx, image_k)
    {
        WebSearcher<Image>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_images(Sorted::YES);
        return ws.data;
    }
}
