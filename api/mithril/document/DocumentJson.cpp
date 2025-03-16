////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/document/DocumentJson.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {
    json json_(Document x)
    {
        Document::Info  i   = cdb::info(x);
        json j{
            { "folder", i.folder.id },
            { "hidden", i.hidden },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "name", i.name },
            { "skey", i.skey },
            { "skeyb", i.skeyb },
            { "skeyc", i.skeyc },
            { "suffix", i.suffix }
        };
        return j;
    }
    
    json json_(const DocumentVector& xs)
    {
        json j = json::array();
        for(Document x : xs)
            j.push_back(json_(x));
        return j;
    }
}
