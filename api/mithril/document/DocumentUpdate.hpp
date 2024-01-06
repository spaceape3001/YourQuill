////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/update/preamble.hpp>
#include <mithril/update/U.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>

namespace yq::mithril::update {
    class UDocument : public U<Document> {
    public:
        static void u_icon(Document,Image);
        
    private:
        UDocument(Document);
    };
}


