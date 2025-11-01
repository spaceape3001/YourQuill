////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/update/preamble.hpp>
#include <yq/mithril/update/U.hpp>
#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>

namespace yq::mithril::update {
    class UDocument : public U<Document> {
    public:
        static void u_icon(Document,Image);
        
    private:
        UDocument(Document);
    };
}


