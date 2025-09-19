////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/directory/Directory.hpp>
#include <mithril/directory/DirectoryJson.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
//#include <mithril/directory/DirectoryInfo.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Directory x)
    {
        Directory::Info  i   = cdb::info(x);
        json j{
            { "folder", i.folder.id },
            { "hidden", i.hidden },
            { "id", x.id },
            { "key", cdb::key(x) },
            { "path", i.path.string() }
        };
        return j;
    }
    
    json json_(const DirectoryVector& xs)
    {
        json j = json::array();
        for(Directory x : xs)
            j.push_back(json_(x));
        return j;
    }
}
