////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/folder/Folder.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/folder/FolderInfo.hpp>
#include <mithril/folder/FolderJson.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {
    json json_(Folder x)
    {
        Folder::Info  i   = cdb::info(x);
        json j{
            { "hidden", i.hidden },
            { "id", x.id },
            { "key", i.key },
            { "name", i.name },
            { "removed", i.removed },
            { "skey", i.skey }
        };
        if(i.parent)
            j["parent"] = i.parent.id;
        
        return j;
    }
    
    json json_(const FolderVector& xs)
    {
        json j = json::array();
        for(Folder x : xs)
            j.push_back(json_(x));
        return j;
    }
}
