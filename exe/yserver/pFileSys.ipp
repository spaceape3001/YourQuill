////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    Map<QString,QString,IgCase>     extension_icons()
    {
        static const char*  szDir   = "ico/ext";
        
        Map<QString,QString,IgCase> ret;
        auto sh     = shared_dir(szDir);
        if(sh.good){
            QDir    d   = sh.value;
            for(const QString& s : d.entryList(QStringList() << "*.svg")){
                if(s.count('.') != 1)
                    continue;
                ret[s.mid(0, s.indexOf('.'))] = QString(szDir) + "/" + s;
            }
        }
        return ret;
    }

    
    void    folder(HtmlWriter& h)
    {
        static const Map<QString,QString,IgCase>    xicons  = extension_icons();

        using namespace html;
        R      r   = decode_folder_prime();
        test(r, true);
        if(r == R::EMPTY)
            x_folder    = cdb::top_folder();
 
        Folder  p = cdb::parent(x_folder);
        h.title("Folder '" + cdb::name(x_folder) + "'");
        
        
        if(x_can_edit){
            
        }
        
        h << "<table>\n"
          << "<tr><th></th><th></th></tr>\n";
    
    
    
        if(p){
            QString url    = QString("/folder?id=%1").arg((quint64) p.id);
            h << "<tr><td>" << link(url, icon_for("icon/parent.svg", "go up")) << "</td><td>" << link(url, full(p)) << "</td></tr>\n";
        }
        
        for(Folder fo : cdb::folders(x_folder, Sorted::YES)){
            if(cdb::hidden(fo))  
                continue;
                
            QString url    = QString("/folder?id=%1").arg((quint64) fo.id);
            h << "<TR><TD>" << link(url, icon_for(fo)) << "</td><td>" << link(url, full(fo)) << "</td><TD>" 
                << cdb::folders_count(fo) << "</TD><TD>" 
                << cdb::documents_count(fo) << "</TD><TD>" << full(fo) << "</TD></TR>\n";
        }
                
        //h << "</TABLE>\n";
        
        //h << H3("Documents") << "\n";
        //h << "<TABLE><TR><TH>Document</TH><TH>Fragments</TH></TR>\n";
        for(Document a : cdb::documents(x_folder, Sorted::YES)){
            h << "<TR><TD>" << icon_for("icon/generic.svg") << "</td><td>" << cdb::label(a) << "</TD><TD>" 
                << cdb::fragments_count(a) << "</TD></TR>\n";
        }
        h << "</TABLE>\n";
       
    }
    
    
    ContentType    get_icon(QByteArray& b)
    {
        test(decode_icon_prime());
        if(cdb::raster(x_icon)){
            b       = cdb::bytes(x_icon, icon_size());
        } else
            b       = cdb::bytes(x_icon);
        return cdb::type(x_icon);
    }


    INVOKE(
        reg_page(hGet, "/folder", folder).id().key().description("Folder!");
        reg_page(hGet, "/icon", get_icon).id().description("Icon!");
    )
}

