////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/AtomSys.hpp>
#include <db/ClassFile.hpp>
#include <srv/ArgDecode.hpp>
#include <srv/HtmlPage.hpp>
#include <srv/TLSGlobals.hpp>
#include <srv/Utilities.hpp>
#include <util/DelayInit.hpp>
#include <util/Utilities.hpp>

#include <httpserver/httprequest.h>

namespace {
    using namespace html;
    
    QString cls_title()
    {
        if(x_root)
            return QString("Class '%1' {%2}").arg(cdb::label(x_class)).arg(x_root->name());
        return QString("Class '%1'").arg(cdb::label(x_class));
    }

    
    void    class_overview(HtmlWriter&h)
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
                h.key("Name")       << ILine("name", whole->name.qString());
                h.key("Plural")     << ILine("plural", whole->plural.qString());
                h.key("Brief")      << ILine("brief", whole->brief.qString());
                h.key("Notes")      << IText("notes", whole->notes.qString());
            }
            
            h << SubmitLabel::Save << "\n";
        } else {
            Vector<Field>       fields   = cdb::fields(x_class, Sorted::YES);
        
            auto _t = h.table();
            h.key("Key")        << cdb::key(x_class);
            h.key("ID")         << x_class.id;
            h.key("Name")       << whole -> name;
            h.key("Plural")     << whole -> plural;
            h.key("Brief")      << whole -> brief;
            h.key("Use")        << config(get_class_vector(whole->use));
            if(!x_root)
                h.key("Derives") << config(cdb::dependents(x_class, Linkage::Direct));
            h.key("Sources")    << config(get_class_vector(whole->sources));
            h.key("Targets")    << config(get_class_vector(whole->targets));
            h.key("Reverse")    << config(get_class_vector(whole->reverse));
            h.key("Notes")      << whole -> notes;
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
        x -> name   = utf8(x_request -> getParameter("name"));
        x -> plural = utf8(x_request -> getParameter("plural"));
        x -> brief  = utf8(x_request -> getParameter("brief"));
        x -> notes  = utf8(x_request -> getParameter("notes"));
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

    void        class_fields(HtmlWriter&h)
    {
        test(decode_class_prime(), false);
        test(decode_columns(), true);
        Map<String,FData>      fields;
        for(const Root* rt : wksp::roots()){
            ClassFile::Shared    p   = cdb::read(x_class, rt);
            if(!p)
                continue;
            for(auto& j : p->fields){
                FData&  f   = fields[j.first];
                f.key       = j.first;
                f.name.set_if(j.second.name);
                f.brief.set_if(j.second.brief);
                f.roots << rt;
                f.direct    = true;
                if(!f.field)
                    f.field = cdb::field(x_class, j.first.qString());
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
        auto  &fd    = p -> fields[ x_key];
        fd.name = x_key;
        fd.plural = x_key + "s";
        p -> save();

        //if(x_edit_req){
            //throw Redirect(HttpStatus::SeeOther, "/wksp/field?id=" + QByteArray::number((quint64) c.id()));
        //} else 
            return_to_sender();
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    
    void    class_reverses(HtmlWriter& h)
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
            
            auto f = h.form("/wksp/class/edit/reverses", QString("id=%1&root=%2").arg((quint64) x_class.id).arg(x_root->id()));

            h.table(col_array_cols(all, x_columns), [&](Class c){
                if(!c) 
                    return ;
                    
                QString     k   = cdb::key(c);
                    
                QString     id  = "rev-" + k;
                h << IHidden(id, "false") << "\n";
                h << "<input type=\"checkbox\" id=\"" << id << "\" name=\"" << id << "\" value=\"true\"";
                if(us->reverse.has(k))
                    h << " checked=\"true\"";
                h << "><label for=\"" << id << "\">" << cdb::label(c) << "</label>";
                if(whole->reverse.has(k) && !us->reverse.has(k))
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

    
        Vector<Class>   all = cdb::all_classes(Sorted::YES);
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        if(!x)
            throw HttpStatus::BadRequest;

        x -> reverse.clear();
        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("rev-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                x->reverse << k;
        }
        
        x -> save();
        return_to_sender();
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    void    class_sources(HtmlWriter& h)
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
            
            
            auto f = h.form("/wksp/class/edit/sources", QString("id=%1&root=%2").arg((quint64) x_class.id).arg(x_root->id()));

            h.table(col_array_cols(all, x_columns), [&](Class c){
                if(!c)   
                    return ;
                    
                QString     k   = cdb::key(c);
                QString     id  = "src-" + k;
                h << IHidden(id, "false") << "\n";
                h << "<input type=\"checkbox\" id=\"" << id << "\" name=\"" << id << "\" value=\"true\"";
                if(us->sources.has(k))
                    h << " checked=\"true\"";
                h << "><label for=\"" << id << "\">" << cdb::label(c) << "</label>";
                if(whole->sources.has(k) && !us->sources.has(k))
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
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        if(!x)
            throw HttpStatus::BadRequest;

        x->sources.clear();
        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("src-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                x->sources << k;
        }
        
        x -> save();
        return_to_sender();
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    
    void    class_targets(HtmlWriter& h)
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
            
            Vector<Class>   all = cdb::all_classes(Sorted::YES);
            
            auto f = h.form("/wksp/class/edit/targets", QString("id=%1&root=%2").arg((quint64) x_class.id).arg(x_root->id()));

            h.table(col_array_cols(all, x_columns), [&](Class c){
                if(!c)   
                    return ;
                    
                QString     k   = cdb::key(c);
                QString     id  = "tgt-" + k;
                h << IHidden(id, "false") << "\n";
                h << "<input type=\"checkbox\" id=\"" << id << "\" name=\"" << id << "\" value=\"true\"";
                if(us->targets.has(k))
                    h << " checked=\"true\"";
                h << "><label for=\"" << id << "\">" << cdb::label(c) << "</label>";
                if(whole->targets.has(k) && !us->targets.has(k))
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
        
        Vector<Class>   all = cdb::all_classes();
        ClassFile::Shared   x = cdb::write(x_class, x_root);
        if(!x)
            throw HttpStatus::BadRequest;
        x->targets.clear();

        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("tgt-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                x->targets << k;
        }
        
        x -> save();
        return_to_sender();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    void    class_use(HtmlWriter& h)
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
                    h << ICheck(id, nki.name, us->use.has(nki.key));
                    if(whole->use.has(nki.key) && !us->use.has(nki.key))
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
        x->use.clear();
        Vector<Class>   all = cdb::all_classes();
        for(auto c : all){
            if(c == x_class)
                continue;
            QString     k   = cdb::key(c);
            QByteArray  id   = utf8("use-" + k);
            auto r = to_boolean(x_request -> getParameter(id));
            if(r.good && r.value)
                x->use << k;
        }
        
        x -> save();
        return_to_sender();
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    INVOKE(
        reg_tabbar({
            reg_page(hGet, "/wksp/class", class_overview).description("Class").id().key().label("Overview"),
            reg_page(hGet, "/wksp/class/fields", class_fields).description("Class Fields").id().key().label("Fields"),
            reg_page(hGet, "/wksp/class/dependency", class_use).description("Class Dependencies").id().key().label("Dependencies"),
            reg_page(hGet, "/wksp/class/sources", class_sources).description("Class Sources").id().key().label("Sources"),
            reg_page(hGet, "/wksp/class/targets", class_targets).description("Class Targets").id().key().label("Targets"),
            reg_page(hGet, "/wksp/class/reverses", class_reverses).description("Class Reverses").id().key().label("Reverses")
        });
        reg_page(hPost, "/wksp/class/edit", edit_class).description("Class Editing").id().key();
        reg_page(hPost, "/wksp/class/edit/use", edit_use).description("Class Use Editing").id().key();
        reg_page(hPost, "/wksp/class/edit/reverses", edit_reverses).description("Class Reverse  Editing").id().key();
        reg_page(hPost, "/wksp/class/edit/sources", edit_sources).description("Class Source Editing").id().key();
        reg_page(hPost, "/wksp/class/edit/targets", edit_targets).description("Class Target Editing").id().key();
        reg_page(hPost, "/wksp/field/create", field_create).description("Create Field").argument("cls", "Class").key();
    );
}
