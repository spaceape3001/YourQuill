////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/image/Image.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/image/ImageSearch.hpp>

namespace yq::mithril {
    ImageVector  search(WebContext&ctx, image_t)
    {
        WebSearcher<Image>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_images(Sorted::YES);
        return ws.data;
    }
}
