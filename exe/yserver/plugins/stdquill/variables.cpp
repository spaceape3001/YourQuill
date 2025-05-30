////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/core/DelayInit.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/net/Http.hpp>
#include <yq/core/Logging.hpp>
#include <yq/core/StreamOps.hpp>

#include <yq/text/join.hpp>
#include <yq/typedef/string.hpp>

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

#include <mithril/web/VarTemplate.hpp>
#include <mithril/web/WebAdapters.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebVariable.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    add_script(yq::mithril::WebHtml& str, std::string_view name)
    {
        switch(mimeTypeForExt(file_extension(name))){
        case ContentType::javascript:
            str << "\t\t<script srce=\"/js/" << name << "\">\n";
            break;
        default:
            //  do nothing if unclear...
            break;
        }
    }


    void    var_abbr(WebHtml&h)
    {
        html_escape_write(h, wksp::abbreviation());
    }
    
    void    var_atom_count(WebHtml&h)
    {
        h << cdb::count_atoms();
    }
    
    void    var_attribute_count(WebHtml&h)
    {
        h << cdb::all_attributes_count();
    }

    void    var_author(WebHtml&h)
    {
        html_escape_write(h, wksp::author());
    }

    void    var_body(WebHtml& h)
    {
        h << h.context().var_body;
    }

    void    var_can_edit(WebHtml&h)
    {
        //  TODO
        h << true;
    }
    
    void    var_category_count(WebHtml&h)
    {
        h << cdb::all_categories_count();
    }

    void     var_class_count(WebHtml&h)
    {
        h << cdb::all_classes_count();
    }
    
    void    var_directory_count(WebHtml&h)
    {
        h << cdb::all_directories_count();
    }
    
    void    var_document_count(WebHtml&h)
    {
        h << cdb::all_documents_count();
    }
    
    void    var_field_count(WebHtml&h)
    {
        h << cdb::all_fields_count();
    }
    
    void    var_folder_count(WebHtml&h)
    {
        h << cdb::all_folders_count();
    }
    
    void    var_fragment_count(WebHtml&h)
    {
        h << cdb::all_fragments_count();
    }
    
    void    var_has_tabbar(WebHtml&h)
    {
        WebContext& ctx = h.context();
        h << (ctx.page->group() != nullptr);
    }

    void    var_home(WebHtml&h)
    {
        html_escape_write(h, wksp::home());
    }

    void    var_host(WebHtml&h)
    {
        html_escape_write(h, wksp::host());
    }
    
    void    var_html_css(WebHtml&h)
    {
        for(auto s : h.h_css()){
            h << "\n\t\t		<LINK rel=\"stylesheet\" type=\"text/css\" href=\"" << s << "\" />";
        }
    }

    void    var_html_script(WebHtml&h)
    {
        
        for(auto name : h.h_scripts()){
            switch(mimeTypeForExt(file_extension(name))){
            case ContentType::javascript:
                h << "\n\t\t<SCRIPT src=\"" << name << "\"></SCRIPT>";
                break;
            default:
                break;
            }
        }
    }

    void    var_image_count(WebHtml&h)
    {
        h << cdb::all_images_count();
    }
    
    void    var_leaf_count(WebHtml&h)
    {
        h << cdb::all_leafs_count();
    }

    void    var_local(WebHtml& h)
    {
        h << h.context().is_local();
    }

    void    var_logged_in(WebHtml& h)
    {
        h << h.context().session.logged_in;
    }

    void    var_name(WebHtml& h)
    {
        html_escape_write(h, wksp::name());
    }

    void    var_port(WebHtml&h)
    {
        h << wksp::port();
    }
    
    void    var_root_count(WebHtml&h)
    {
        h << wksp::root_dirs().size();
    }
    
    void    var_script(WebHtml&h)
    {
        h << h.context().var_script;
    }

    
    void    var_scripts(WebHtml& h)
    {
        WebContext& ctx = h.context();
        for(std::string_view s : ctx.page->scripts())
            add_script(h, s);
        for(const std::string& s : ctx.var_scripts)
            add_script(h, s);
    }

    void    var_ssid(WebHtml& h)
    {
        h << h.context().session.ssid;
    }


    void    var_tabbar(WebHtml& h)
    {
        WebContext& ctx = h.context();
        const WebGroup* grp = ctx.page -> group();
        if(!grp)
            return ;
    
        h << "<table class=\"tabbar\"><tr>\n";
        for(const WebPage* p : grp -> pages){
            bool    us = p == ctx.page;
            if(us) {
                h << "<td class=\"tb-select\">";
                html_escape_write(h, p -> label());
                h << "</td>";
            } else {
                UrlView url = ctx.url;
                url.path    = p->path();
                h << "<td class=\"tabbar\"><a href=\"" << url << "\">";
                html_escape_write(h, p -> label());
                h << "</a></td>";
            }
        }
        h << "</tr></table>\n";
    }
    
    void    var_tag_count(WebHtml& h)
    {
        h << cdb::count_tags();
    }

    void    var_templates(WebHtml&h)
    {
        h << join(wksp::templates(), ", ");
    }

    void    var_time(WebHtml& h)
    {
        h  << h.context().timestamp;
    }
    
    void    var_title(WebHtml&h)
    {
        html_escape_write(h, h.context().var_title);
    }


    void    var_user(WebHtml& h)
    {
        auto& ctx = h.context().session;
        if(!ctx.logged_in)
            h << "(anonymous)";
        else
            h << ctx.username;
    }
    
    void    var_user_count(WebHtml&h)
    {
        h << cdb::all_users_count();
    }

    void    var_year(WebHtml& h)
    {
        h << (h.context().timeparts.tm_year+1900);
    }

    YQ_INVOKE( 
        reg_webvar<var_abbr>("abbreviation")
            .description("The really SHORT form of your project's name");
            
        reg_webvar<var_atom_count>("atom_count")
            .description("Total number of atoms in the cache database");
            
        reg_webvar<var_attribute_count>("attribute_count")
            .description("Total number of attributes in the cache database");
            
        reg_webvar<var_author>("author")
            .description("It's your project, right?"); 
            
        reg_webvar<var_body>("body")
            .description("Body of the HTML page, used by the standard page template");
            
        reg_webvar<var_can_edit>("can_edit")
            .todo();        
        
        reg_webvar<var_category_count>("category_count")
            .description("Total number of categories in the cache database.");
            
        reg_webvar<var_class_count>("class_count")
            .description("Total number of classes in the cache database.");
            
        reg_webvar<var_directory_count>("directory_count")
            .description("Total number of directories in the cache database.");
            
        reg_webvar<var_document_count>("document_count")
            .description("Total number of documents in the cache database.");
            
        reg_webvar<var_field_count>("field_count")
            .description("Total number of fields in the cache database.");
            
        reg_webvar<var_folder_count>("folder_count")
            .description("Total number of folders in the cache database.");
            
        reg_webvar("footer", wksp::shared("www/footer.ht"sv))
            .source(".footer")
            .description("Footer used by the standarad page template.  Read from the /.footer file, treated as a template.");
            
        reg_webvar<var_fragment_count>("fragment_count")
            .description("Total number of fragments in the cache database.");
            
        reg_webvar<var_has_tabbar>("has_tabbar");
        
        reg_webvar<var_home>("home")
            .description("Home URL...TODO");        
        
        reg_webvar<var_host>("host")
            .description("Host name of the computer this program is running on.");
            
        reg_webvar<var_html_css>("html_css")
            .description("Used for page-template css");

        reg_webvar<var_html_script>("html_script")
            .description("Used for page-template scripts");
        
        reg_webvar<var_image_count>("image_count")
            .description("Total number of images in the cache database.");
            
        reg_webvar<var_leaf_count>("leaf_count")
            .description("Total number of leafs in the cache database.");
            
        reg_webvar<var_local>("local")
            .description("Boolean to indicate it's a localhost connection.");;
            
        reg_webvar<var_logged_in>("logged_in")
            .todo()
            .description("Boolean to indicate that a user is currently logged in on the session."); 
            
        reg_webvar<var_port>("port")
            .description("Port number to use on the server.");
            
        reg_webvar<var_name>("project")
            .description("Name of the project/universe.");
            
        reg_webvar<var_root_count>("root_count")
            .description("Total number of root_dirs (template & non-template) in the project/workspace.");
            
        reg_webvar<var_script>("script")
            .description("Javascript used with the standard page template.");
            
        reg_webvar<var_scripts>("scripts")
            .description("Javascript file(s), used by the standard page template");
            
        reg_webvar<var_ssid>("ssid")
            .description("Current session ID");
            
        reg_webvar("summary", wksp::shared("www/summary.ht"sv))
            .source(".summary")
            .description("Summary of the project used by the default home page.  Read from the /.summary file, treated as a template.");;

        reg_webvar<var_tabbar>("tabbar")
            .description("Injects a navigation tab-bar in for registered web-groups.");
        
        reg_webvar<var_tag_count>("tag_count")
            .description("Total number of tags in the cache database.");
            
        reg_webvar<var_templates>("templates")
            .description("Comma separated list of templates being used by this project/universe.");
            
        reg_webvar<var_time>("time")
            .description("Current time of access by your request to the server.");
        
        reg_webvar<var_title>("title")
            .description("Title of the page, used by the standard page template.");
            
        reg_webvar<var_user>("user")
            .description("User (if known) of the current session.");
            
        reg_webvar<var_user_count>("user_count")
            .description("Total number of users in the cache database.");
            
        reg_webvar<var_year>("year")
            .description("Current calendar year, might be a little wonky around New Year's.");
    )
}

