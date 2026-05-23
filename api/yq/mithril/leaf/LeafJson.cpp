////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LeafJson.hpp"

#include <yq/core/Enumeration.hpp>
#include <yq/mithril/leaf/Leaf.hpp>
#include <yq/mithril/leaf/LeafCDB.hpp>
#include <yq/mithril/leaf/LeafData.hpp>
#include <yq/net/json.hpp>
#include <yq/text/match.hpp>

namespace yq::mithril {
    json json_(Leaf x)
    {
        Leaf::Info  i   = cdb::info(x);
        json j{
            { "brief", i.brief },
            { "document", i.doc.id },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "title", i.title }
        };
        return j;
    }
    
    json json_(const LeafVector& xs)
    {
        json j = json::array();
        for(Leaf x : xs)
            j.push_back(json_(x));
        return j;
    }
    
    namespace {
        std::string_view lx_key_for(const json& j)
        {
            auto x = j.find("key");
            if(x == j.end())
                return {};
            if(!x->is_string())
                return {};
            
            static thread_local std::string s   = x->get<std::string>();
            return s;
        }
    
        void apply_attrs(json& j, const KVTree& tree)
        {
            j = json::array();
            for(auto& kv : tree.subs){
                json j2{
                    { "key", kv.key },
                    { "id", kv.id },
                    { "cmd", kv.cmd },
                    { "data", kv.data },
                };
                
                if(!kv.subs.empty())
                    apply_attrs(j2["sub"], kv);
                j.push_back(std::move(j2));
            }
            
            std::stable_sort(j.begin(), j.end(), [](const json& a, const json& b) -> bool {
                return is_less_igCase(lx_key_for(a), lx_key_for(b));
            });
        }
        
        void apply_context(json&j, const std::vector<Context>& context)
        {
            j   = json::array();
            for(auto& ctx : context){
                json j2{
                    { "data", ctx.data },
                    { "format", key_of(ctx.format) },
                    { "icon", ctx.icon },
                    { "title", ctx.title }
                };
                j.push_back(j2);
            }
        }
    }

    json json_(Leaf x, all_k)
    {
        Leaf::Info  i   = cdb::info(x);
        json j{
            { "brief", i.brief },
            { "document", i.doc.id },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "title", i.title }
        };
        
        if(auto m = cdb::merged(x)){
            if(!m->attrs.empty())
                apply_attrs(j["attrs"], m->attrs);
            if(!m->context.empty())
                apply_context(j["context"], m->context);
        }
        
        return j;
    }
}
