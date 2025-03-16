////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/image/Image.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/image/ImageSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

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
