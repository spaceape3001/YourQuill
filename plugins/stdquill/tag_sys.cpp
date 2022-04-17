////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <db/leaf.hpp>
#include <db/document_html.hpp>
#include <db/tag_arg.hpp>
#include <db/tag_cdb.hpp>
#include <db/tag_html.hpp>
#include <db/cdb_sq.hpp>

#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage3.hpp>

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;

namespace {

    void    page_dev_tag(WebHtml& out)
    {
        Tag x   = tag(out);
        if(!x)
            throw hNotFound;
        auto i = info(x);
        
        out.title() << "Tag \"" << i.name << "\" (" << i.key << ")";
        auto ta = out.table();
        out.kvrow("ID") << x.id;
        out.kvrow("Name") << i.name;
        out.kvrow("Key") << i.key;
        out.kvrow("Brief") << i.brief;
        out.kvrow("Document") << dev(i.doc);
        out.kvrow("Leaf"); //  << dev(leaf(x_tag));
        out.kvrow("Image");
    }

    void    page_dev_tags(WebHtml& out)
    {
        out.title("All Tags");
        dev_table(out, all_tags());
    }
    
    void    update_info(Tag x, const Tag::SharedData& data)
    {
        static thread_local SQ u("UPDATE Tags SET name=?,brief=? WHERE id=?");
        auto u_af = u.af();
        u.bind(1, data->name);
        u.bind(2, data->brief);
        u.bind(3, x.id);
        u.exec();
    }
    
    void    update_image(Tag tag, const Tag::SharedData& data)
    {
        // TODO
    }
    
    void    update_leaf(Tag tag, const Tag::SharedData& data)
    {
        // TODO
    }

    void    stage3_tag1(Document doc)
    {
        //  Initial import
        Tag t = db_tag(doc);
        Tag::SharedData data    = merged(t, IsUpdate);
        update_info(t, data);
    }

    void    stage3_tag2(Document doc)
    {
        //  leaf & image cross
        Tag t = db_tag(doc);
        Tag::SharedData data    = merged(t, IsUpdate);
        update_leaf(t, data);
        update_image(t, data);
    }

    void    update_tag_data(Fragment frag, Change change)
    {
        Document    doc = document(frag);
        Tag         t   = db_tag(doc);

        if(change == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(t);
                return ;
            }
        }
        
        Tag::SharedData data    = merged(t, IsUpdate);
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
    
    on_change<update_tag_data>(tags_folder(), "*.tag");
    for(const char* z : Image::kSupportedExtensionWildcards)
        on_change<update_tag_image>(tags_folder(), z);

    on_stage3<stage3_tag1>(s3_tag1, tags_folder(), "*.tag");
    on_stage3<stage3_tag2>(s3_tag2, tags_folder(), "*.tag");
)

