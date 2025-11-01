////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/folder/Folder.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>
#include <yq/mithril/folder/FolderInfo.hpp>
#include <yq/mithril/folder/FolderJson.hpp>
#include <yq/net/json.hpp>

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
