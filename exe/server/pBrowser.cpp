////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yNetWriter.hpp"
#include "yPage.hpp"
#include "yCommon.hpp"

#include <httpserver/httprequest.h>

#include "db/Cache.hpp"
#include "util/Compare.hpp"
#include "util/DelayInit.hpp"
#include "util/Utilities.hpp"

namespace {
    using namespace html;
    void    browse(Html&h)
    {
        test(decode_class(), true);
        test(decode_tag(), true);
        test(decode_folder(), true);
        test(decode_columns(), true);
        
        Vector<Atom>       atoms = cdb::all_atoms(Sorted::YES);
        if(x_folder){
            atoms.erase_if([](Atom a) -> bool{
                return x_folder != cdb::folder(a);
            });
        }
        if(x_class){
            atoms.erase_if([](Atom a) -> bool{
                return !cdb::is(a, x_class);
            });
        }
        if(x_tag){
            atoms.erase_if([](Atom a) -> bool {
                return !cdb::tagged(a, x_tag);
            });
        }
        
        h.title("Browser");
        h.table(col_array_cols(atoms, x_columns), [&](Atom a){
            if(!a)
                return ;
            QString     url;
            if(Leaf l = cdb::leaf(a)){
                url = QString("/leaf?id=%1").arg((quint64) l.id);
            } else {
                url = QString("/atom?id=%1").arg((quint64) a.id);
            }
            h << "[" << cdb::key(a) << "]: " << link(url, cdb::title(a));
        });
    }
    
    void    categories(Html&h)
    {
        test(decode_columns(), true);
        h.title("Categories");
        h.table(col_array_cols(cdb::all_classes(Sorted::YES), x_columns), [&](Class c){
            if(!c)   
                return ;
            auto ni = cdb::nki(c, true);
            h << link(QString("/browse?cls=%1").arg(ni.key), ni.name);
        });
    }


    INVOKE(
        page(hGet, "/browse", browse);
        page(hGet, "/categories", categories);
    )

}
