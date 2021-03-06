////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/Document.hpp>
#include <gui/Provider.hpp>

using DocumentProvider = std::shared_ptr<Provider<Document>>;

namespace provider {
    DocumentProvider    all_documents();
}

