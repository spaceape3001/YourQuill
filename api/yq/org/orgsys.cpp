////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Category.hpp"
#include "CategoryArg.hpp"
#include "CategoryCDB.hpp"
#include "CategoryData.hpp"
#include "CategoryFile.hpp"
#include "CategoryHtml.hpp"
#include "CategoryPost.hpp"

#include "Tag.hpp"
#include "TagArg.hpp"
#include "TagCDB.hpp"
#include "TagData.hpp"
#include "TagFile.hpp"
#include "TagHtml.hpp"
#include "TagPost.hpp"

#include <yq/atom/Class.hpp>
#include <yq/atom/Field.hpp>
#include <kernel/bit/KeyValue.hpp>
#include <kernel/io/Strings.hpp>
#include <yq/db/HtmlLayout.hpp>
#include <yq/db/NKI.hpp>
#include <yq/db/SQ.hpp>
#include <yq/file/DocumentCDB.hpp>
#include <yq/file/DocumentHtml.hpp>
#include <yq/file/FolderCDB.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <kernel/file/Root.hpp>
#include <yq/file/RootHtml.hpp>
#include <yq/image/ImageCDB.hpp>
#include <yq/image/ImageHtml.hpp>
#include <basic/FileUtils.hpp>
#include <yq/leaf/LeafCDB.hpp>
#include <basic/TextUtils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <kernel/wksp/Workspace.hpp>

#include <yq/db/IDLock.ipp>

namespace yq {


////////////////////////////////////////////////////////////////////////////////
//  CATEGORY
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Category category(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Category{};
                
            Category t   = cdb::category( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_category(i))
                return Category{i};
            return Category{};
        }
        
        Category category(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return category_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return category_key(k);
            
            k       = ctx.find_query("cat");
            if(!k.empty())
                return category(k);
            return Category{};
        }
        
        
        Category category(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return category(arg_string);
        }
        
        Category category(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return category(arg_string);
        }

        Category category_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_category(i))
                return Category{i};
            return Category{};
        }

        Category category_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return category_id(arg_string);
        }
        
        Category category_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return category_id(arg_string);
        }
        
        Category category_key(std::string_view arg_string)
        {
            return cdb::category(trimmed(arg_string));
        }
        
        Category category_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return category_key(arg_string);
        }
        
        Category category_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return category_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml& h, Category v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
    
        WebHtml&    operator<<(WebHtml&h, Dev<Category> v)
        {
            std::string n       = cdb::label(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Category> v)
        {
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void        admin_table(WebHtml&h, const std::vector<Category>& categories)
        {
            auto tac = h.table();
            auto iz = h.context().session.icon_size;
            html::columns(h, categories, 
                [&](Category c)
                {
                    if(c){
                        Image   i   = cdb::icon(c);
                        if(i){
                            h << cdb::thumbnail(i, iz);
                        } else
                            h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                    }
                },
                [&](Category c)
                {
                    if(c){
                        h << "<a href=\"/admin/category?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                    }
                }
            );
        }

        void    dev_table(WebHtml& h, const std::vector<Category>&categorys)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Category t : categorys){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                    << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

        void        new_category_control(WebHtml&h, std::string_view npath)
        {
            Url url;
            url.path=copy(npath);
            h << html::form_start(url, false);
            h << "Add Category:<br>";
            h << ikey();
            h << "<br><hr width=\"10%\">\n";
            h << iroot( DataRole::Config );
            h << "<hr width=\"10%\">\n";
            h << iedit();
            h << "<hr width=\"10%\">\n";
            h << Submit(Submit::Create);
            h << "</form>\n";
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Category category(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("category");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::category_id(k);
            }
            return Category();
        }
        
        Category category(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::category_id(arg_string);
        }
        
        Category category(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::category_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  TAG
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////


    namespace arg {
        Tag tag(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Tag{};
                
            Tag t   = cdb::tag( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
        }
        
        Tag tag(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return tag_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return tag_key(k);
            
            k       = ctx.find_query("tag");
            if(!k.empty())
                return tag(k);
            return Tag{};
        }
        
        
        Tag tag(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return tag(arg_string);
        }
        
        Tag tag(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return tag(arg_string);
        }

        Tag tag_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
        }

        Tag tag_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_id(arg_string);
        }
        
        Tag tag_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_id(arg_string);
        }
        
        Tag tag_key(std::string_view arg_string)
        {
            return cdb::tag(trimmed(arg_string));
        }
        
        Tag tag_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_key(arg_string);
        }
        
        Tag tag_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Tag t)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(t), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(t);
            //  end the url (later)
            return h;
        }
    
        WebHtml&    operator<<(WebHtml&h, Dev<Tag> v)
        {
            h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << cdb::label(v.data);
            h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Tag> v)
        {
            if(v.data)
                h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    admin_table(WebHtml&h, const std::vector<Tag>&tags)
        {
            auto tac = h.table();
            auto iz = h.context().session.icon_size;
            html::columns(h, tags, 
                [&](Tag c)
                {
                    if(c){
                        Image   i   = cdb::icon(c);
                        if(i){
                            h << cdb::thumbnail(i, iz);
                        } else
                            h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                    }
                },
                [&](Tag c)
                {
                    if(c){
                        h << "<a href=\"/admin/tag?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                    }
                }
            );
        }

        void    dev_table(WebHtml& h, const std::vector<Tag>&tags)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Leaf</th><th>Brief</th></tr>\n";
            for(Tag t : tags){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                    << "</td><td>" << dev(i.doc) << "</td><td>" << /* dev(cdb::leaf(t)) << */ "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

        void        new_tag_control(WebHtml&h, std::string_view npath)
        {
            Url url;
            url.path=copy(npath);
            h << html::form_start(url, false);
            h << "Add Tag:<br>";
            h << ikey();
            h << "<br><hr width=\"10%\">\n";
            h << iroot( DataRole::Config );
            h << "<hr width=\"10%\">\n";
            h << iedit();
            h << "<hr width=\"10%\">\n";
            h << Submit(Submit::Create);
            h << "</form>\n";
        }
        

    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Tag tag(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("tag");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::tag_id(k);
            }
            return Tag();
        }
        
        Tag tag(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::tag_id(arg_string);
        }
        
        Tag tag(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::tag_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
