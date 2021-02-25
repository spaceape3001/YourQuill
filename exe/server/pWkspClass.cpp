////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yImporter.hpp"
#include "yNetWriter.hpp"
#include "yPage.hpp"
#include "yScanner.hpp"
#include "yCommon.hpp"

#include "db/Cache.hpp"
#include "db/Copyright.hpp"
#include "util/Http.hpp"
#include "util/Logging.hpp"
#include "db/ShareDir.hpp"
#include "util/Array2.hpp"
#include "util/Compare.hpp"
#include "util/DelayInit.hpp"
#include "util/Guarded.hpp"
#include "util/Reverse.hpp"
#include "util/Utilities.hpp"


#include <httpserver/httprequest.h>

#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QJsonObject>
#include <QString>

namespace {
    using namespace html;
    
    QString cls_title()
    {
        if(x_root)
            return QString("Class '%1' {%2}").arg(cdb::label(x_class)).arg(x_root->name());
        return QString("Class '%1'").arg(cdb::label(x_class));
    }

    
    void    class_overview(Html&h)
    {
        test(decode_class_prime(), false);
        h.title(cls_title());
        if(x_can_edit)
            test(decode_root(), true);
            
        ClassFile::Shared   whole;
        if(x_root)
            whole       = cdb::read(x_class, x_root);
        else
            whole       = cdb::merged(x_class);

        if(x_can_edit)
            h << ControlRoot();

        h << link("/wksp/classes", icon_for("/icon/parent.svg", "Classes")) << link("/wksp/classes", QString("Back to Class Listing"));

        if(x_root && x_can_edit){
            auto f = h.form(Edit<Class>());
            {
                auto t = h.table();
                h.key("Key")        << cdb::key(x_class);
                h.key("ID")         << x_class.id;
                h.key("Name")       << ILine("name", whole->name().qString());
                h.key("Plural")     << ILine("plural", whole->plural().qString());
                h.key("Brief")      << ILine("brief", whole->brief().qString());
                h.key("Notes")      << IText("notes", whole->notes().qString());
            }
            
            h << SubmitLabel::Save << "\n";
        } else {
            Vector<Field>       fields   = cdb::fields(x_class, Sorted::YES);
        
            auto _t = h.table();
            h.key("Key")        << cdb::key(x_class);
            h.key("ID")         << x_class.id;
            h.key("Name")       << whole -> name();
            h.key("Plural")     << whole -> plural();
            h.key("Brief")      << whole -> brief();
            h.key("Use")        << config(get_class_vector(whole->use()));
            if(!x_root)
                h.key("Derives") << config(cdb::dependents(x_class, Linkage::Direct));
            h.key("Sources")    << config(get_class_vector(whole->source()));
            h.key("Targets")    << config(get_class_vector(whole->target()));
            h.key("Reverse")    << config(get_class_vector(whole->reverse()));
            h.key("Notes")      << whole -> notes();
            h.key("Fields")     << fields;
        }
    }
    
    void    edit_class()
    {
        if(!x_can_edit)
            throw HttpStatus::Unauthorized;
            
        test(decode_class_prime(), false);
        test(decode_root(), false);
        
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        x -> name(utf8(x_request -> getParameter("name")));
        x -> plural(utf8(x_request -> getParameter("plural")));
        x -> brief(utf8(x_request -> getParameter("brief")));
        x -> notes(utf8(x_request -> getParameter("notes")));
        x -> save();
        return_to_sender();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    struct FData {
        String              key, name, brief;
        ::Image             icon;
        Field               field;
        Vector<const Root*> roots;
        bool                direct;
        
        FData() : direct(false) {}
    };

    void        class_fields(Html&h)
    {
        test(decode_class_prime(), false);
        test(decode_columns(), true);
        Map<String,FData>      fields;
        for(const Root* rt : wksp::roots()){
            ClassFile::Shared    p   = cdb::read(x_class, rt);
            if(!p)
                continue;
            for(auto& j : p->fields()){
                FData&  f   = fields[j.key];
                f.key       = j.key;
                f.name.set_if(j.name);
                f.brief.set_if(j.desc);
                f.roots << rt;
                f.direct    = true;
                if(!f.field)
                    f.field = cdb::field(x_class, j.key.qString());
            }
        }
        
        for(Field field : cdb::fields(x_class)){
            cdb::NKI  nki   = cdb::nki(field);
            FData& f        = fields[nki.key];
            f.key.set_if(nki.key);
            f.name.set_if(nki.name);
            if(!f.field)
                f.field     = field;
            f.icon          = nki.icon;
            if(f.brief.empty())
                f.brief     = cdb::brief(field);
        }
        
        
        h.title(cls_title() + "\" Fields");
        if(x_can_edit){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            {
                auto f = h.form("/wksp/field/create");
                h << IHidden("cls", cdb::key(x_class));
                h << "Add Field:<br>";
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

        auto t = h.table();
        t << "<tr><th></th><th>Key</th><th>Brief</th></tr>\n";
        for(auto& j : fields){
            h << "<tr><td>";
            if(j.second.direct){
                h << "[";
                Comma m(",");
                for(const Root* rt : j.second.roots){
                    h << m.text() << rt->key();
                    ++m;
                }
                h << "]";
            } else {
                h << config(cdb::class_(j.second.field));
            }
            h << "</td><td>" << j.first << "</td><td>" << j.second.brief << "</td></tr>\n";
        }
    }
    
    void        field_create()
    {
        if(!x_can_edit)
            throw HttpStatus::Unauthorized;
            
        test(decode_root(),  true);
        test(decode_class(), false);
        test(decode_key(),   false);
        test(decode_edit(),  true);

        if(!x_root)
            x_root      = def_root(DataRole::Classes);
        
        auto    p   = cdb::write(x_class, x_root);
        if(!p)
            throw HttpStatus::BadRequest;
        auto  fd    = p -> field( x_key, true);
        fd.name(x_key);
        fd.plural(x_key + "s");
        p -> save();

        //if(x_edit_req){
            //throw Redirect(HttpStatus::SeeOther, "/wksp/field?id=" + QByteArray::number((quint64) c.id()));
        //} else 
            return_to_sender();
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    
    void    class_reverses(Html& h)
    {
        test(decode_columns(), true);
        test(decode_class_prime(), false);
        if(x_can_edit)
            test(decode_root(), true);
        h.title(cls_title() + "\" Reverses");
        if(x_can_edit)
            h << ControlRoot();
            
        if(x_can_edit && x_root){
            ClassFile::Shared    us, whole;
            whole       = cdb::merged(x_class);
            us          = cdb::read(x_class, x_root);
            
            Vector<Class>   all = cdb::all_classes(Sorted::YES);
            
            auto urev   = us->reverse();
            auto wrev  = whole->reverse();
            
            auto f = h.form("/wksp/class/edit/reverses", QString("id=%1&root=%2").arg((quint64) x_class.id).arg(x_root->id()));

            h.table(col_array_cols(all, x_columns), [&](Class c){
                if(!c) 
                    return ;
                    
                QString     k   = cdb::key(c);
                    
                QString     id  = "rev-" + k;
                h << IHidden(id, "false") << "\n";
                h << "<input type=\"checkbox\" id=\"" << id << "\" name=\"" << id << "\" value=\"true\"";
                if(urev.has(k))
                    h << " checked=\"true\"";
                h << "><label for=\"" << id << "\">" << cdb::label(c) << "</label>";
                if(wrev.has(k) && !urev.has(k))
                    h << "<br><i>(implicitly checked)</i>";
            });
            h << SubmitLabel::Save;
        } else {
            h << "(Class reverse graph TBD)";
        }
    }

    void    edit_reverses()
    {
        if(!x_can_edit)
            throw HttpStatus::Unauthorized;
            
        test(decode_class_prime(), false);
        test(decode_root(), false);
        
        StringSet   rev;
        Vector<Class>   all = cdb::all_classes(Sorted::YES);
        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("rev-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                rev << k;
        }
        
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        if(!x)
            throw HttpStatus::BadRequest;
        x -> reverse(rev);
        x -> save();
        return_to_sender();
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    void    class_sources(Html& h)
    {
        test(decode_columns(), true);
        test(decode_class_prime(), false);
        if(x_can_edit)
            test(decode_root(), true);
        h.title(cls_title() + "\" Sources");
        if(x_can_edit)
            h << ControlRoot();
            
        if(x_can_edit && x_root){
            ClassFile::Shared    us, whole;
            whole       = cdb::merged(x_class);
            us          = cdb::read(x_class, x_root);
            
            Vector<Class>   all = cdb::all_classes(Sorted::YES);
            
            auto usrc   = us->source();
            auto wsrc   = whole->source();
            
            auto f = h.form("/wksp/class/edit/sources", QString("id=%1&root=%2").arg((quint64) x_class.id).arg(x_root->id()));

            h.table(col_array_cols(all, x_columns), [&](Class c){
                if(!c)   
                    return ;
                    
                QString     k   = cdb::key(c);
                QString     id  = "src-" + k;
                h << IHidden(id, "false") << "\n";
                h << "<input type=\"checkbox\" id=\"" << id << "\" name=\"" << id << "\" value=\"true\"";
                if(usrc.has(k))
                    h << " checked=\"true\"";
                h << "><label for=\"" << id << "\">" << cdb::label(c) << "</label>";
                if(wsrc.has(k) && !usrc.has(k))
                    h << "<br><i>(implicitly checked)</i>";
            });
            h << SubmitLabel::Save;
        } else {
            h << "(Class source graph TBD)";
        }
    }

    void    edit_sources()
    {
        if(!x_can_edit)
            throw HttpStatus::Unauthorized;
            
        test(decode_class_prime(), false);
        test(decode_root(), false);
        
        StringSet src;
            
        Vector<Class>   all = cdb::all_classes();
        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("src-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                src << k;
        }
        
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        if(!x)
            throw HttpStatus::BadRequest;
        x -> source(src);
        x -> save();
        return_to_sender();
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    
    void    class_targets(Html& h)
    {
        test(decode_columns(), true);
        test(decode_class_prime(), false);
        if(x_can_edit)
            test(decode_root(), true);
        h.title(cls_title() + "\" Targets");
        if(x_can_edit)
            h << ControlRoot();
        if(x_can_edit && x_root){
            ClassFile::Shared    us, whole;
            whole       = cdb::merged(x_class);
            us          = cdb::read(x_class, x_root);
            auto utgt   = us->target();
            auto wtgt   = whole->target();
            
            Vector<Class>   all = cdb::all_classes(Sorted::YES);
            
            auto f = h.form("/wksp/class/edit/targets", QString("id=%1&root=%2").arg((quint64) x_class.id).arg(x_root->id()));

            h.table(col_array_cols(all, x_columns), [&](Class c){
                if(!c)   
                    return ;
                    
                QString     k   = cdb::key(c);
                QString     id  = "tgt-" + k;
                h << IHidden(id, "false") << "\n";
                h << "<input type=\"checkbox\" id=\"" << id << "\" name=\"" << id << "\" value=\"true\"";
                if(utgt.has(k))
                    h << " checked=\"true\"";
                h << "><label for=\"" << id << "\">" << cdb::label(c) << "</label>";
                if(wtgt.has(k) && !utgt.has(k))
                    h << "<br><i>(implicitly checked)</i>";
            });
            h << SubmitLabel::Save;
        } else {
            h << "(Class Target Graph TBD)";
        }
    }
    
    void    edit_targets()
    {
        if(!x_can_edit)
            throw HttpStatus::Unauthorized;
            
        test(decode_class_prime(), false);
        test(decode_root(), false);
        
        StringSet   tgt;
        Vector<Class>   all = cdb::all_classes();
        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("tgt-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                tgt << k;
        }
        
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        if(!x)
            throw HttpStatus::BadRequest;
        x -> target(tgt);
        x -> save();
        return_to_sender();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    void    class_use(Html& h)
    {
        test(decode_columns(), true);
        test(decode_class_prime(), false);
        if(x_can_edit)
            test(decode_root(), true);
        h.title(cls_title() + "\" Dependencies");
        if(x_can_edit)
            h << ControlRoot();
        if(x_can_edit && x_root){
            ClassFile::Shared    us, whole;
            whole       = cdb::merged(x_class);
            us          = cdb::read(x_class, x_root);
            
            auto uuse   = us->use();
            auto wuse   = whole->use();
            
            Vector<Class>   all = cdb::all_classes(Sorted::YES);
            
            auto f = h.form("/wksp/class/edit/use");
            h << IHidden("cls", cdb::key(x_class));
            h << IHidden("root", x_root->id());

            h.table(col_array_cols(all, x_columns), [&](Class c){
                if(!c)   
                    return ;
                    
                cdb::NKI nki     = cdb::nki(c,true);
                    
                if(c == x_class){
                    h << "[--] " << nki.name;
                } else {
                    QString     id  = "use-" + nki.key;
                    h << IHidden(id, "false");
                    h << ICheck(id, nki.name, uuse.has(nki.key));
                    if(wuse.has(nki.key) && !uuse.has(nki.key))
                        h << "<br><i>(implicitly checked)</i>";
                    h << "\n";
                }
            });
            h << SubmitLabel::Save;
        } else {
            Graph g = cdb::dep_graph(x_class);
            if(g){
                h << g;
            } else {
                h << "No class graph available";
            }
        }
    }
    
    void    edit_use()
    {
        if(!x_can_edit)
            throw HttpStatus::Unauthorized;
            
        test(decode_class_prime(), false);
        test(decode_root(), false);
        
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        if(!x)
            throw HttpStatus::BadRequest;
        StringSet use;
        Vector<Class>   all = cdb::all_classes();
        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("use-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                use << k;
        }
        
        x -> use(use);
        x -> save();
        return_to_sender();
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    INVOKE(
        tabbar({
            page(hGet, "/wksp/class", class_overview).description("Class").id().key().label("Overview"),
            page(hGet, "/wksp/class/fields", class_fields).description("Class Fields").id().key().label("Fields"),
            page(hGet, "/wksp/class/dependency", class_use).description("Class Dependencies").id().key().label("Dependencies"),
            page(hGet, "/wksp/class/sources", class_sources).description("Class Sources").id().key().label("Sources"),
            page(hGet, "/wksp/class/targets", class_targets).description("Class Targets").id().key().label("Targets"),
            page(hGet, "/wksp/class/reverses", class_reverses).description("Class Reverses").id().key().label("Reverses")
        });
        page(hPost, "/wksp/class/edit", edit_class).description("Class Editing").id().key();
        page(hPost, "/wksp/class/edit/use", edit_use).description("Class Use Editing").id().key();
        page(hPost, "/wksp/class/edit/reverses", edit_reverses).description("Class Reverse  Editing").id().key();
        page(hPost, "/wksp/class/edit/sources", edit_sources).description("Class Source Editing").id().key();
        page(hPost, "/wksp/class/edit/targets", edit_targets).description("Class Target Editing").id().key();
        page(hPost, "/wksp/field/create", field_create).description("Create Field").argument("cls", "Class").key();
    );
}