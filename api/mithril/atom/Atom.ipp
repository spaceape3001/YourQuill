////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomCDB.hpp>

#include <io/FileUtils.hpp>
#include <basic/TextUtils.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentCDB.hpp>

namespace yq::mithril {
    std::string         Atom::full_key(Document doc, std::string_view ck)
    {
        std::string     kk  = cdb::key(doc);
        std::string     dk = std::string(strip_extension(kk));
        if(dk.empty())    
            return std::string();

        if(!ck.empty()){
            dk += '#';
            dk += ck;
        }
        return dk;
    }
}

