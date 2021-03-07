////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yCommon.hpp"
#include <db/Workspace.hpp>
#include <db/ShareDir.hpp>
#include <db/bit/Copyright.hpp>
#include <srv/ArgDecode.hpp>
#include <srv/HtmlPage.hpp>
#include <srv/JsonPage.hpp>
#include <srv/Session.hpp>
#include <srv/TLSGlobals.hpp>
#include <srv/Utilities.hpp>
#include <util/DelayInit.hpp>
#include <util/Logging.hpp>

#include <QCoreApplication>
#include <QJsonObject>

namespace {
    using namespace html;

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  HELPERS
    
    
        
    
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  Standard content
        
    ContentType     css_page(QByteArray& h)
    {
        h   = cur_css();
        return ContentType::css;
    }
    
    ContentType    index_page(QByteArray& h)
    {
        TypedBytes         cur  = cur_index();
        return do_direct_content(h, do_expand(cur.content), cur.type);
    }
    
    ContentType     background_page(QByteArray&h)
    {
        TypedBytes      cur = cur_background();
        h   = cur.content;
        return cur.type;
    }
    
    QByteArray      get_scripts()
    {
        QByteArray  ret;
        for(const QByteArray& x : x_scripts)
            ret += "\t\t<script src=\"/js/" + x + "\">\n";
        return ret;
    }
    
    //  API control pages

    ContentType      api_alive(QByteArray&dst)
    {
        dst     = "Santa Claus";
        return ContentType::text;
    }

    HttpStatus      api_quit()
    {
        QCoreApplication::instance()->quit();
        return HttpStatus::Success;
    }
    
    QJsonObject     api_workspace()
    {
        QJsonObject     ret;
        ret["author"]       = wksp::author();
        ret["abbreviation"] = wksp::abbreviation();
        ret["name"]         = wksp::name();
        ret["copyright"]    = wksp::copyright().text.qString();
        ret["c_stance"]     = wksp::copyright().stance.key().qString();
        ret["c_from"]       = QString::number(wksp::copyright().from);
        ret["c_to"]         = QString::number(wksp::copyright().to);
        ret["color"]        = cur_text_color();
        ret["bkcolor"]      = cur_back_color();
        if(x_is_local){
            ret["quill"]    = wksp::quill_path();
            ret["cache"]    = wksp::cache_db();
        }
        return ret;
    }
    
    QJsonObject     api_wksp_quill()
    {
        QJsonObject     ret;
        ret["quill"]        = wksp::quill_path();
        return ret;
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  File importers
    
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  Class Configuration 


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  Regular Pages
    
    
    void         wksp_class_create()
    {
        if(!x_can_edit)
            throw HttpStatus::Unauthorized;

        test(decode_root(), true);
        test(decode_key(),  false);
        test(decode_edit(), true);
        
        if(!x_root)
            x_root      = def_root(DataRole::Classes);

        Class   c   = cdb::make_class(x_key, x_root);
        if(!c)
            throw HttpStatus::InternalError;
        yNotice() << "Class " << x_key << " created by " << cur_user();

        if(x_edit_req){
            throw Redirect(HttpStatus::SeeOther, "/wksp/class?id=" + QByteArray::number((quint64) c.id));
        } else 
            return_to_sender();
    }
    
    void            wksp_classes(HtmlWriter& h)
    {
        test(decode_columns(), true);
        h.title("'" + wksp::name() + "' Classes");

        if(x_can_edit){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            {
                auto f = h.form("/wksp/class/create");
                h << "Add Class:<br>";
                h << IKey();
                h << "<br><hr width=\"10%\">\n";
                h << IRoot{ DataRole::Classes };
                h << "<hr width=\"10%\">\n";
                h << IEditReq();
                h << "<hr width=\"10%\">\n";
                h << SubmitLabel::Create;
            }
            h << "</table>\n";
        }
        
        h.table(col_array_cols(cdb::all_classes(Sorted::YES), x_columns), [&](Class c){
            if(!c)   
                return ;
                
            auto ni     = cdb::nki(c, true);

            h << link(QString("/wksp/class?cls=%1").arg(ni.key), ni.name) << " [";
            Comma m(",");
            Document    d   = cdb::document(c);
            for(const Root* rt : cdb::roots(d)){
                if(!rt)
                    continue;
                h << m.text() << rt -> key();
                ++m;
            }
            h << "]";
            
            QString b = cdb::brief(c);
            if(!b.isEmpty())
                h << "<br><i>" << b << "</i>";
        });
    }
    
    void            wksp_classes_use_graph(HtmlWriter&h)
    {
        Graph   g   = cur_class_dep_graph();
        if(g)
            h << g;
    }
    
    void            wksp_home(HtmlWriter& h)
    {
        h.title("'" + wksp::name() + "'");
        
        if(x_can_edit)
            h << "<font color=\"red\">Warning: Changes here (may) require a restart of the server.</font>\n";
        
        auto t = h.table();
        h.key("Name") << wksp::name();
        h.key("Author") << wksp::author();
        h.key("Abbreviation") << wksp::abbreviation();
        h.key("Copyright") << wksp::copyright().text.qString();
    }
    
    void            wksp_tags(HtmlWriter& h)
    {
        Vector<Tag>     tags    = cdb::all_tags(Sorted::YES);
        h.title("'" + wksp::name() + "' Tags");
        auto _t = h.table();
        h << "<TR><TH>Key</TH><TH>Name</TH><TH>Brief</TH>\n";
        for(Tag t : tags){
            auto i = cdb::info(t);
            h << "<tr><td>"<< i.key << "</td><td>" << i.name << "</td><td>" << i.brief << "</td></tr>\n";
        }
        
        if(x_can_edit){
            h << "<tr><td colspan=\"3\"><hr /></td></tr>\n";
            
            
        }
        
    }
    
    
    
        //  And the registration
    INVOKE(
    
    
            //  Register directories
        reg_dispatcher("img", shared_dir("www/img"));
        reg_dispatcher("dev", shared_dir("www/dev"));
        reg_dispatcher("help", shared_dir("www/help"));
        reg_dispatcher("icon", shared_dir("www/ico"));
        reg_dispatcher("js", shared_dir("www/js"));
        reg_dispatcher("markdown", shared_dir("www/markdown"));
        
            //  Register Getters
        reg_getter("abbr", []() -> QByteArray {
            static QByteArray ret = wksp::abbreviation().toUtf8();
            return ret;
        });
        
        reg_getter("author", []() -> QByteArray {
            static QByteArray ret = wksp::author().toUtf8();
            return ret;
        });
        
        reg_getter("can_edit",[]() -> QByteArray {
            return x_can_edit ? "true" : "false";
        });
        
        reg_getter("date", []() -> QByteArray {
            return x_time.toUtf8();
        });
        
        reg_getter("footer", []() -> QByteArray{
            return do_expand(cur_footer());
        });
        
        reg_getter("home", []() -> QByteArray{
            static QByteArray   ret = wksp::home().toUtf8();
            return ret;
        });
        
        reg_getter("host", []() -> QByteArray{
            static QByteArray   ret = wksp::hostname().toUtf8();
            return ret;
        });
        
        reg_getter("local", []() -> QByteArray{
            return x_is_local ? "true" : "false";
        });
        
        reg_getter("logged_in", []() -> QByteArray {
            return (x_session && x_session -> loggedIn) ? "true" : "false";
        });
        
        reg_getter("luser", []() -> QByteArray {
            static QByteArray ret   = wksp::local_user().toUtf8();
            if(x_is_local){
                return ret;
            } else
                return QByteArray();
        });
        
        reg_getter("name", []() -> QByteArray {
            static QByteArray   ret = wksp::name().toUtf8();
            return ret;
        });
        
        reg_getter("port", []() -> QByteArray{
            static QByteArray   ret = QByteArray::number(wksp::port());
            return ret;
        });
        
        reg_getter("scripts", get_scripts);
        
        reg_getter("summary", []() -> QByteArray{
            return cur_summary();
        });
        
        reg_getter("year", []() -> QByteArray{
            return QByteArray::number(x_at.date().year());
        });
        
        
            //  Register pages
    
        reg_page(hGet,  "/", index_page).description("Home page");
        reg_page(hGet, "/api/alive", api_alive).description("Simple health check");
        reg_page(hPost, "/api/quit", api_quit).local().description("Causes the server to quit.");
        reg_page(hGet, "/api/wksp/quill", api_wksp_quill).local().description("Quill Workspace Path");
        reg_page(hGet, "/api/wksp", api_workspace).description("Workspace Information");
        reg_page(hGet,  "/background", background_page).description("Background");
        reg_page(hGet,  "/css", css_page).description("Our style sheet!");
        

        reg_page(hPost, "/wksp/class/create", wksp_class_create).description("Creates a class!");
        reg_tabbar({
            reg_page(hGet, "/wksp", wksp_home).description("Home to the workspace").label("Overview"),
            reg_page(hGet, "/wksp/classes", wksp_classes).description("Classes for the workspace").label("Classes"),
            reg_page(hGet, "/wksp/classes/use", wksp_classes_use_graph).description("Use Class diagram for workspace").label("Use"),
            reg_page(hGet, "/wksp/tags", wksp_tags).description("Tags for the workspace").label("Tags")
        });
        
    )
}

