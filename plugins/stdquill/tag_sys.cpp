////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <yq/leaf/Leaf.hpp>
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/tag/cdb.hpp>
#include <yq/wksp/CacheFwd.hpp>
#include <yq/wksp/CacheSQ.hpp>

using namespace yq;

WebHtml&    operator<<(WebHtml&h, const Dev<Tag>&v)
{
    std::string n       = cdb::name(v.data);
    if(n.empty())
        n   = "(no-name)";
    if(v.data)
        h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
    h << "[" << v.data.id << "] " << n;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const DevID<Tag>&v)
{
    if(v.data)
        h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

Tag tag(std::string_view k)
{
    k   = trimmed(k);
    if(k.empty())
        return Tag();
    Tag t   = cdb::tag(k);
    if(t)
        return t;
    uint64_t    i = to_uint64(k).value;
    if(cdb::exists_tag(i))
        return Tag{i};
    return Tag{};
}

Tag     tag(const WebContext&ctx)
{
    std::string    k    = ctx.find_query("id");
    if(!k.empty())
        return Tag{ to_uint64(k).value };
    
    k       = ctx.find_query("key");
    if(!k.empty())
        return cdb::tag(k);
    
    k       = ctx.find_query("tag");
    if(!k.empty())
        return tag(k);
    return Tag{};
}

Tag     tag(const WebHtml&h)
{
    return tag(h.context());
}

Tag     tag(const WebContext& ctx, std::string_view k)
{
    std::string     v  = ctx.find_query(k);
    return tag(v);
}

Tag     tag(const WebHtml& h, std::string_view k)
{
    return tag(h.context(), k);
}

Tag     tag(const WebContext&ctx, std::initializer_list<std::string_view> keys)
{
    std::string v   = ctx.find_query(keys);
    return tag(v);
}

Tag     tag(const WebHtml&h, std::initializer_list<std::string_view> keys)
{
    return tag(h.context(), keys);
}


void    dev_table(WebHtml& h, const std::vector<Tag>&tags)
{
    auto ta = html::table(h);
    h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Leaf</th><th>Brief</th></tr>\n";
    for(Tag t : tags){
        auto i  = cdb::info(t);
        h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
            << "</td><td>" << dev(i.doc) << "</td><td>" << /* dev(cdb::leaf(t)) << */ "</td><td>" << i.brief << "</td></tr>\n";
    }
}



namespace {

    void    page_dev_tag(WebHtml& out)
    {
        Tag x   = tag(out);
        if(!x)
            throw hNotFound;
        auto i = cdb::info(x);
        
        out.title("Tag \"" + i.name + "\" (" + i.key + ")");
        auto ta = html::table(out);
        html::kvrow(out, "ID") << x.id;
        html::kvrow(out, "Name") << i.name;
        html::kvrow(out, "Key") << i.key;
        html::kvrow(out, "Brief") << i.brief;
        html::kvrow(out, "Document") << dev(i.doc);
        html::kvrow(out, "Leaf"); //  << dev(cdb::leaf(x_tag));
        html::kvrow(out, "Image");
    }

    void    page_dev_tags(WebHtml& out)
    {
        out.title("All Tags");
        dev_table(out, cdb::all_tags());
    }
    
    void    update_info(Tag x, const TagData::Shared& data)
    {
        static thread_local cdb::SQ u("UPDATE Tags SET name=?,brief=? WHERE id=?");
        auto u_af = u.af();
        u.bind(1, data->name);
        u.bind(2, data->brief);
        u.bind(3, x.id);
        u.exec();
    }
    
    void    update_image(Tag tag, const TagData::Shared& data)
    {
        // TODO
    }
    
    void    update_leaf(Tag tag, const TagData::Shared& data)
    {
        // TODO
    }

    void    stage3_tag1(Document doc)
    {
        //  Initial import
        Tag t = cdb::db_tag(doc);
        TagData::Shared data    = cdb::merged(t, cdb::IsUpdate);
        update_info(t, data);
    }

    void    stage3_tag2(Document doc)
    {
        //  leaf & image cross
        Tag t = cdb::db_tag(doc);
        TagData::Shared data    = cdb::merged(t, cdb::IsUpdate);
        update_leaf(t, data);
        update_image(t, data);
    }

    void    update_tag_data(Fragment frag, Change change)
    {
        Document    doc = cdb::document(frag);
        Tag         t   = cdb::db_tag(doc);

        if(change == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                cdb::erase(t);
                return ;
            }
        }
        // TODO
        
        TagData::Shared data    = cdb::merged(t, cdb::IsUpdate);
        update_info(t, data);
        update_leaf(t, data);
    }
    
    void    update_tag_image(Fragment frag, Change change)
    {
        // TODO
    }
}

YQ_INVOKE(
    reg_webpage<page_dev_tag>("/dev/tag")
        .argument("id", "Tag id (number)")
        .argument("key", "Tag key (string)")
        .argument("tag", "Tag key/identifier");
        
    reg_webpage<page_dev_tags>("/dev/alltags");
    
    on_change<update_tag_data>(cdb::tags_folder(), "*.tag");
    for(const char* z : Image::kSupportedExtensionWildcards)
        on_change<update_tag_image>(cdb::tags_folder(), z);

    on_stage3<stage3_tag1>(s3_tag1, cdb::tags_folder(), "*.tag");
    on_stage3<stage3_tag2>(s3_tag2, cdb::tags_folder(), "*.tag");
)

