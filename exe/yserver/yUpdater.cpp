////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yCommon.hpp"
#include "yUpdater.hpp"
#include "update/uAtom.hpp"
#include "update/uClass.hpp"
#include "update/uField.hpp"
#include "update/uImage.hpp"
#include "update/uLeaf.hpp"
#include "update/uTag.hpp"

#include <db/Image.hpp>
#include <db/Tag.hpp>
#include <db/ShareDir.hpp>
#include <db/Root.hpp>
#include <db/Workspace.hpp>
#include <srv/Importer.hpp>
#include <srv/Page.hpp>
#include <srv/Scanner.hpp>
#include <util/FileUtils.hpp>
#include <util/Guarded.hpp>
#include <util/Hash.hpp>
#include <util/Utilities.hpp>


using namespace cdb;


/*
    This section is about maintaining the data graph, in memory, and it needs to remain lightweight.
*/

Image            calc_icon_for(Folder f, const QString&k)
{
    for(const char* z : Image::kSupportedExtensions){
        Image i = cdb::image(cdb::document(f, k + "." + z));
        if(i)
            return i;
    }
    return Image{};
}

Image           calc_icon_for(const Root* rt)
{
    if(!rt)
        return Image{};
    
    if(!rt->def_icon_file().isEmpty()){
        Fragment    f   = cdb::fragment(rt->resolve(rt->def_icon_file()));
        if(f)
            return cdb::image(f);
    }
    
    for(const char* z : { ".icon", ".root" }){
        for(const char* y : Image::kSupportedExtensions){
            QString     d   = QString(z) + "." + y;
            Document    doc = cdb::document(d);
            if(!doc)
                continue;
            Image       img = cdb::image(doc);
            if(img)
                return img;
        }
    }
    return Image{};
}

    

namespace {

    /*
        We've got some common data here.... cache it!
    */

    std::atomic<bool>       gHasBackground{false};
    Guarded<TypedBytes>     gBackground;
    Guarded<TypedBytes>     gIndex;
    Guarded<QByteArray>     gFooter;
    Guarded<QByteArray>     gCss;
    Guarded<QByteArray>     gSummary;
    Guarded<QString>        gTextColor, gBkColor;

    void    update_css();

    void    update_background()
    {
        TypedBytes  n   = TypedBytes(DataRole::Image, {".background.jpg", ".backgorund.png", ".background.svg", ".background.gif"});
        gBackground = n;
        if(gHasBackground != !n.content.isEmpty()){
            gHasBackground  = !n.content.isEmpty();
            update_css();
        }
    }

    void    update_css()
    {
        QByteArray      css;
        if(wksp::has_init()){
            for(Fragment f : cdb::fragments(".css", DataRole::Style))
                css += cdb::bytes(f);
        }
        
        QByteArray  newTxtColor     = "black";
        QByteArray  newBkColor      = "white";
        
        Map<QByteArray, QByteArray> vars;
        bool    inBody  = false;
        for(QByteArray l : css.split('\n')){
            if(inBody){
                if(l.startsWith('}')){
                    inBody  = false;
                    continue;
                }
                int i  = l.indexOf(':');
                if(i<=0)
                    continue;
                QByteArray  k   = l.mid(0,i).simplified().toLower();
                QByteArray  v   = l.mid(i+1).trimmed();
                if(k == "color")
                    newTxtColor      = v;
                if(k == "background-color")
                    newBkColor      = v;
                vars[k]     = v;
            } else if(l.toLower().startsWith("body")) {
                inBody  = true;
            }
        }
        gTextColor  = utf8(newTxtColor);
        gBkColor    = utf8(newBkColor);
        if(!vars.empty()){
            css += "\n/*\n    Variables auto-picked from existing CSS.\n*/\n";
            css += ":root {\n";
            for(auto& j : vars)
                css += "    --" + j.first + ": " + j.second + '\n';
            css += "}\n\n";
        }
        if(gHasBackground){
            css += "body {\n"
                    "    background-image: url(\"/background\");"
                    "}\n";
        }
            
        css        += shared_bytes("std/css");
        gCss        = css;
    }
    
    void    update_footer()
    {
        qbytearray_r    r;
        if(wksp::has_init())
            r               = cdb::bytes(".footer");
        if(!r.good || r.value.isEmpty())
            r.value         = shared_bytes("std/footer");
        gFooter             = r.value;
    }
    
    void    update_page()
    {
        Page::default_page(shared_bytes("std/page"));
    }
    
    void    update_index()
    {
        TypedBytes      ctr({".index.html", ".index.htm", ".index.md", ".index"}, ContentType::markdown);
        if(ctr.empty()){
            ctr.content             = shared_bytes("std/index");
            ctr.type                = ContentType::markdown;
        }
        gIndex      = ctr;
    }
    
    void    update_summary()
    {
        if(wksp::has_init())
            gSummary    = file_bytes(".summary");
    }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  OTHER STUFF

    //void    update_image(Fragment frag)
    //{
        //update(db_image(frag));
    //}

    //void    on_image_change(Fragment frag)
    //{
        //update_image(frag);
        //if(cdb::folder(frag) == cdb::top_folder()){
            //const Root* rt      = cdb::root(frag);
            //if(rt)
                //cdb::update_root(rt, calc_icon_for(rt));
        //} else if(cdb::folder(frag) == cdb::classes_folder()){
            //update_class_icon(frag);
        //} else if(cdb::within(cdb::classes_folder(), frag)){
            //update_field_icon(frag);
        //}
    //}
}

bool        has_background()
{
    return gHasBackground;
}

TypedBytes  cur_background()
{
    return gBackground;
}

TypedBytes  cur_index()
{
    return gIndex;
}

QByteArray  cur_footer()
{
    return gFooter;
}

QByteArray  cur_css()
{
    return gCss;
}

QByteArray  cur_summary()
{
    return gSummary;
}

QString     cur_text_color()
{
    return gTextColor;
}

QString     cur_back_color()
{
    return gBkColor;
}


void        updater_init()
{
    update_css();
    update_footer();
    update_page();
    update_index();

    Scanner::watch(shared_file("std/css"), update_css);
    Scanner::watch(shared_file("std/index"), update_index);
    Scanner::watch(shared_file("std/page"), update_page);
    Scanner::watch(shared_file("std/footer"), update_footer);

    on_change(all_set<Change>(), ".css", update_css);
    //on_change(all_set<Change>(), ".page", update_page);
    on_change(all_set<Change>(), ".footer", update_footer);
    on_change(all_set<Change>(), ".index", update_index);
    on_change(all_set<Change>(), ".index.md", update_index);
    on_change(all_set<Change>(), ".index.htm", update_index);
    on_change(all_set<Change>(), ".index.html", update_index);
    //on_change(all_set<Change>(), ".theme", update_css);
    on_change(all_set<Change>(), ".summary", update_summary);

    on_change(all_set<Change>(), ".background.jpg", update_background);
    on_change(all_set<Change>(), ".background.png", update_background);
    on_change(all_set<Change>(), ".background.svg", update_background);

    UImage::init_scan();

    for(Document d : documents_by_suffix(cdb::tags_folder(), "tag"))
        cdb::update(cdb::db_tag(d), Update::Interior);

    init_class();

    ULeaf::init_read();
    ULeaf::init_link();

    for(Tag  t : all_tags())
        cdb::update(t, Update::Exterior);
    

    init_class_graphs();

    on_change("*.tag", [](Fragment frag){
        if(cdb::folder(frag) == cdb::tags_folder()){
            cdb::update(cdb::db_tag(frag), { Update::Exterior, Update::Interior });
        }
    });
}

