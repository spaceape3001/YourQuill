////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "DirectoryCDB.hpp"

namespace {
    void reg_directory_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Directory>();
            w.property("folder", (Folder(*)(Directory)) cdb::folder);
            w.property("hidden", (bool(*)(Directory)) cdb::hidden);
            w.property("id", READ_ONLY, &Directory::id);
            w.property("key", (std::string(*)(Directory)) cdb::key);
            w.property("name", (std::string(*)(Directory)) cdb::name);
            w.property("parent", (Directory(*)(Directory)) cdb::parent);
            //w.property("path", (std::filesystem::path(*)(Directory)) cdb::path);
            w.property("skey", (std::string(*)(Directory)) cdb::skey);
            w.property("skeyb", (std::string(*)(Directory)) cdb::skeyb);
        }
    }
}
