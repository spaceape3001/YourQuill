////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yCommon.hpp"
#include "yUpdater.hpp"

#include "uFwd.hpp"

#include "uAtom.hpp"
#include "uAttribute.hpp"
#include "uClass.hpp"
#include "uField.hpp"
#include "uImage.hpp"
#include "uLeaf.hpp"
#include "uTag.hpp"

#include <db/Atom.hpp>
#include <db/Attribute.hpp>
#include <db/CacheUtil.hpp>
#include <db/FileSys.hpp>
#include <db/Graph.hpp>
#include <db/GraphBuilder.hpp>
#include <db/Image.hpp>
#include <db/Leaf.hpp>
#include <db/Property.hpp>
#include <db/Root.hpp>
#include <db/ShareDir.hpp>
#include <db/Tag.hpp>
#include <db/Wksp.hpp>

#include <db/enum/AttrKind.hpp>

#include <srv/Importer.hpp>
#include <srv/Page.hpp>
#include <srv/Scanner.hpp>

#ifdef emit
    #undef emit
#endif

#include <util/FileUtils.hpp>
#include <util/Guarded.hpp>
#include <util/Hash.hpp>
#include <util/Logging.hpp>
#include <util/Set.hpp>
#include <util/SqlQuery.hpp>
#include <util/SqlUtils.hpp>
#include <util/String.hpp>
#include <util/Utilities.hpp>


#include <QBuffer>
#include <QIcon>
#include <QImage>
#include <QImageReader>
#include <QTextStream>
#include <QThreadPool>

#include "uAtom.ipp"
#include "uAttribute.ipp"
#include "uClass.ipp"
#include "uField.ipp"
#include "uImage.ipp"
#include "uLeaf.ipp"
#include "uTag.ipp"


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
    
    if(!rt->icon.empty()){
        Fragment    f   = cdb::fragment(rt->resolve(rt->icon));
        if(f)
            return cdb::image(f);
    }
    
    for(const char* z : { ".icon", ".root" }){
        for(const char* y : Image::kSupportedExtensions){
            String      d   = String(z) + "." + y;
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
    UTag::init_read();
    
    init_class();

    ULeaf::init_read();
    ULeaf::init_link();

    UTag::init_link();

    init_class_graphs();
}

