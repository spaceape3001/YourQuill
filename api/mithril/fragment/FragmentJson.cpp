////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/directory/Directory.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/folder/Folder.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/fragment/FragmentJson.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {
    json json_(Fragment x)
    {
        Fragment::Info  i   = cdb::info(x);
        json j{
            { "directory", i.directory.id },
            { "document", i.document.id },
            { "folder", i.folder.id },
            { "hidden", i.hidden },
            { "id", x.id },
            { "modified", i.modified },
            { "path", i.path.string() },
            { "removed", i.removed },
            { "size", i.size }
        };
        return j;
    }
    
    json json_(const FragmentVector& xs)
    {
        json j = json::array();
        for(Fragment x : xs)
            j.push_back(json_(x));
        return j;
    }
}
