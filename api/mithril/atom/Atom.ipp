////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomCDB.hpp>

#include <yq-toolbox/io/FileUtils.hpp>
#include <yq-toolbox/text/transform.hpp>

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

    std::string         Atom::full_key(Atom at, std::string_view ck)
    {
        std::string     kk  = cdb::key(at);
        if(!ck.empty()){
            if(cdb::is_document(at)){
                kk += '#';
            } else {
                kk += '.';
            }
            kk += ck;
        }
        return kk;
    }

    std::string  Atom::sub_key(Atom at, std::string_view ck)
    {
        std::string     kk  = cdb::skey(at);
        if(!kk.empty())
            kk += '.';
        kk += ck;
        return kk;
    }
}

