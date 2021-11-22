////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    namespace html {
        TabBar::TabBar(std::initializer_list<Page::Writer> wlist)
        {
            for(Page::Writer w : wlist)
                if(w.m_page)
                    m_pages << w.m_page;
            for(Page::Writer w : wlist)
                if(w.m_page)
                    w.m_page -> m_tabbar    = this;
        }
    }

    HtmlWriter::HtmlWriter() = default;
    HtmlWriter::~HtmlWriter() = default;

    HtmlWriter::FormAF      HtmlWriter::form(const String& url, const String& args, bool fInspectOverride)
    {
        write(html::Form{url, args, fInspectOverride});
        return FormAF(this);
    }
    

    HtmlWriter::KVRow       HtmlWriter::key(const String& k, const String& url)
    {
        m_stream << "<tr><th align=\"left\">";
        if(!url.empty())
            m_stream << "<a href=\"" << url << "\">";
        m_stream << k.qString().toHtmlEscaped();
        if(!url.empty())
            m_stream << "</a>";
        m_stream << "</th><td>";
        return KVRow(this);
    }

    HtmlWriter::LinkAF      HtmlWriter::link(const String&url)
    {
        m_stream << "<a href=\"" << url << "\">";
        return LinkAF(this);
    }

    HtmlWriter::Table       HtmlWriter::table()
    {
        m_stream << "<TABLE>";
        return Table(this);
    }
    

    void    HtmlWriter::write(const Comma&c)
    {
        m_stream << c.text();
    }

    void    HtmlWriter::write(const Class& cl)
    {
        auto ni = cdb::nki(cl,true);
        m_stream << "<a href=\"/class?key=" << ni.key << "\">" << ni.name << "</a>";
    }

    void    HtmlWriter::write(const Field& f)
    {
        auto    ni  = cdb::nki(f, true);
                //  No URL (yet)
        m_stream << ni.name;
    }

    void    HtmlWriter::write(const Folder& fo)
    {
        auto ni = cdb::nki(fo, true);
        m_stream << "<a href=\"/folder?key=" << ni.key << "\">" << ni.name << "</a>";
    }

    void    HtmlWriter::write(const Graph& g)
    {
        String n = cdb::name(g).qString().toHtmlEscaped();
        m_stream << "<img src=\"/graph?id=" << (quint64) g.id << "\" alt=\"" << n << "\" usemap=\"#" << n << "\">\n" 
            << cdb::html(g) << "\n";
    }

    void    HtmlWriter::write(const html::Bold&b)
    {
        m_stream << "<B>" << b.txt.qString().toHtmlEscaped() << "</B>\n";
    }

    void    HtmlWriter::write(const html::Config<Class>& c)
    {
        auto ni = cdb::nki(c.v, true);
        m_stream << "<a href=\"/wksp/class?key=" << ni.key << "\">" << ni.name << "</a>";
    }

    void    HtmlWriter::write(const html::ControlRoot&)
    {
        if(!x_request)
            return;
            
        QByteArray  nonroot = x_request -> getRawPath();
        QByteArray  args;

        auto pmap   =  x_request -> getParameterMap();
        for(auto i=pmap.cbegin(); i!=pmap.cend(); ++i){
            if(i.key() != "root"){
                if(!args.isEmpty())
                    args += '&';
                args += i.key() + '=' + i.value();
            }
        }
        
        if(!args.isEmpty())
            nonroot += '?' + args;
        *this << "<table align=\"right\" class=\"roots\">\n";
        *this << "<tr><td class=\"roots\">";
        if(x_root)
            *this << "<a href=\"" << nonroot << "\">";
        *this << "(Fused)";
        if(x_root)
            *this << "</a>";
        *this << "</td></tr><tr><td class=\"roots\"><hr width=\"10%\"></td></tr>\n";
        if(args.isEmpty())
            nonroot += "?root=";
        else
            nonroot += "&root=";
        
        for(const Root* r : wksp::roots()){
            *this << "<tr><td class=\"roots\">";
            if(r != x_root)
                *this << "<a href=\"" << nonroot << r->key << "\">";
            *this << r -> name;
            if(r != x_root)
                *this << "</a>";
            *this  << "</td></tr>\n";
        }
        *this << "</table>\n";
    }
    
    
    void    HtmlWriter::write(const html::Dev<Atom>&c)
    {
        m_stream << "<a href=\"/dev/atom?id=" << c.v.id << "\">" << cdb::key(c.v) << "</a>";
    }

    void    HtmlWriter::write(const html::Dev<Class>&c)
    {
        m_stream << "<a href=\"/dev/class?id=" << c.v.id << "\">" << cdb::key(c.v) << "</a>";
    }

    void    HtmlWriter::write(const html::Dev<Directory>&d)
    {
        m_stream << "<a href=\"/dev/directory?id=" << d.v.id << "\">" << cdb::path(d.v) << "</a>";
    }

    void    HtmlWriter::write(const html::Dev<Document>&f)
    {
        m_stream << "<a href=\"/dev/document?id=" << f.v.id << "\">" << cdb::key(f.v) << "</a>";
    }

    void    HtmlWriter::write(const html::Dev<Folder>& fo)
    {
        String k   = cdb::key(fo.v);
        if(k.empty())
            k   = "/";
        m_stream << "<a href=\"/dev/folder?id=" << k << "\">" << k << "</a>";
    }

    void    HtmlWriter::write(const html::Dev<Fragment>&a)
    {
        m_stream << "<a href=\"/dev/fragment?id=" << a.v.id << "\">" << cdb::path(a.v) << "</a>";
    }

    void    HtmlWriter::write(const html::Dev<Leaf>&c)
    {
        String k = cdb::key(c.v);
        m_stream << "<a href=\"/dev/leaf?id=" << c.v.id << "\">" << k << "</a>";
    }

    void    HtmlWriter::write(const html::Dev<const Root*>&r)
    {
        if(!r.v)
            return ;
        m_stream << "<a href=\"/dev/root?id=" << r.v->id << "\">" << r.v->path << "</a>";
    }
    
    void    HtmlWriter::write(const html::Dev<Tag>&c)
    {
        auto k = cdb::key(c.v);
        m_stream << "<a href=\"/dev/tag?k=" << k << "\">" << k << "</a>";
    }

    void    HtmlWriter::write(const html::Edit<Class>& arg)
    {
        html::Edit<Class>   r = arg;
        if(!r.root)
            r.root   = x_root;
        if(!r.value)
            r.value = x_class;
            
        html::Form    f;
        f.url   = "/wksp/class/edit";
        if(r.root){
            if(r.value)
                f.args   = QString("id=%1&root=%2").arg(x_class.id).arg(r.root->id);
            else
                f.args   = QString("root=%1").arg(r.root->id);
        } else {
            if(r.value)
                f.args   = QString("id=%1").arg(x_class.id);
        }
        f.inspect   = r.inspect;
        write(f);
        
    }

    void    HtmlWriter::write(const html::Escape&e)
    {
        m_stream << e.txt.qString().toHtmlEscaped();
    }

    void    HtmlWriter::write(const html::Form&v)
    {
        bool    inspect = inspect_submit() || v.inspect;
        m_stream << "<form action=\"";
        if(inspect){
            m_stream << "/dev/echo";
        } else 
            m_stream << v.url;
        m_stream << "?" << v.args << "\" method=\"post\">\n";
        if(inspect){
            m_stream << "<input type=\"hidden\" name=\"url\" id=\"url\" value=\"" << v.url << "\">\n";
            m_stream << "<input type=\"hidden\" name=\"args\" id=\"args\" value=\"" << v.args << "\">\n";
        }
    }

    void    HtmlWriter::write(const html::Full<Folder>& fo)
    {
        m_stream << "<a href=\"/folder?key=" << cdb::key(fo.v) << "\">";
        if(fo.v == cdb::top_folder())
            m_stream << "Root (/)";
        else
            m_stream << cdb::key(fo.v);
        m_stream << "</a>\n";
    }


    void    HtmlWriter::write(const html::H1&h)
    {
        m_stream << "<H1>" << h.txt.qString().toHtmlEscaped() << "</H1>\n";
    }
    
    void    HtmlWriter::write(const html::H2&h)
    {
        m_stream << "<H2>" << h.txt.qString().toHtmlEscaped() << "</H2>\n";
    }
    
    void    HtmlWriter::write(const html::H3&h)
    {
        m_stream << "<H3>" << h.txt.qString().toHtmlEscaped() << "</H3>\n";
    }
    
    void    HtmlWriter::write(const html::H4&h)
    {
        m_stream << "<H4>" << h.txt.qString().toHtmlEscaped() << "</H4>\n";
    }
    
    void    HtmlWriter::write(const html::H5&h)
    {
        m_stream << "<H5>" << h.txt.qString().toHtmlEscaped() << "</H5>\n";
    }
    
    void    HtmlWriter::write(const html::H6&h)

    {
        m_stream << "<H6>" << h.txt.qString().toHtmlEscaped() << "</H6>\n";
    }
    
    void    HtmlWriter::write(const html::Image&img)
    {
        if(!img.src.empty()){
            m_stream << "<IMG SRC=\"" << img.src.qString().toHtmlEscaped() << '"';
            if(!img.alt.empty())
                m_stream << " ALT=\"" << img.alt.qString().toHtmlEscaped() << '"';
            if(!img.style.empty())
                m_stream << " STYLE=\"" << img.style.qString().toHtmlEscaped() << '"';
            m_stream << "/>";
        }
    }

    void        HtmlWriter::write(const html::ICheck& v)
    {
        m_stream << "<input type=\"checkbox\" id=\"" << v.key << "\" name=\"" << v.key << "\" value=\"true\"";
        if(v.checked)
            m_stream << " checked=\"true\"";
        m_stream << "><label for=\"" << v.key << "\">" << v.label << "</label>";
    }

    void        HtmlWriter::write(const html::IEditReq&)
    {
        bool    de  = auto_edit();
        *this << "Edit Behavior:<br>";
        *this << "<input type=\"radio\" name=\"edit\" id=\"later\" value=\"later\"><label for=\"later\">";
        if(!de)
            *this << "<b><u>";
        *this << "Later";
        if(!de)
            *this << "</u></b>";
        *this << "</label></br>\n";
        *this << "<input type=\"radio\" name=\"edit\" id=\"now\" value=\"now\"><label for=\"now\">";
        if(de)
            *this << "<b><u>";
        *this << "Now";
        if(de)
            *this << "</u></b>";
        *this << "</label></br>\n";
    }
    
    void        HtmlWriter::write(const html::IHidden&v)
    {
        m_stream << "<input type=\"hidden\" id=\"" << v.key << "\" name=\"" << v.key << "\" value=\"" << v.value << "\">";
    }

    void        HtmlWriter::write(const html::IKey&)
    {
        m_stream << "<input type=\"text\" name=\"key\" id=\"key\">";
    }
    
    void        HtmlWriter::write(const html::ILabel&v)
    {
        m_stream << "<label for=\"" << v.key << "\">" << v.label.qString().toHtmlEscaped() << "</label>";
    }

    void    HtmlWriter::write(const html::ILine& i)
    {
        QString     ke  = i.key.qString().toHtmlEscaped();
        m_stream << "<input type=\"text\" name=\"" << ke << "\" id=\"" << ke << "\" value=\"" << i.value.qString().toHtmlEscaped() << "\"";
        if(i.size > 0)
            m_stream << " size=\"" << i.size << "\"";
        m_stream << ">";
    }
    
    void        HtmlWriter::write(const html::IRoot&v)
    {
        *this << "Change Root:<br>";
        const Root* re = def_root(v.dr);
        for(const Root* rt : wksp::roots()){
            *this << "<input type=\"radio\" name=\"root\" id=\"" << rt->key << "\" value=\"" << rt->id 
                << "\"><label for=\"" << rt->key << "\">";
            if(rt == re)
                *this << "<b><u>";
            *this << "[" << rt->key << "] " << rt->name;
            if(rt == re)
                *this << "</u></b>";
            *this  << "</label><br>\n";
        }
    }
    
    void        HtmlWriter::write(const html::ISubmit&v)
    {
        m_stream << "<input type=\"submit\"";
        switch(v.submit){
        case SubmitLabel::Save:
            m_stream << " value=\"Save\"";
            break;
        case SubmitLabel::Create:
            m_stream << " value=\"Create\"";
            break;
        default:
            break;
        }
        
        m_stream << "><input type=\"reset\"";
        // TODO the reset
        m_stream << ">";
    }

    void    HtmlWriter::write(const html::IText& v)
    {
        m_stream << "<textarea id=\"" << v.key << "\" name=\"" << v.key << "\" rows=\"" << v.rows << "\" cols=\"" 
            << v.cols << "\">" << v.value.qString().toHtmlEscaped() << "</textarea>";
    }

    void    HtmlWriter::write(const html::Icon<Document>&d)
    {
        html::Icon<String>   v;
        Image   i   = cdb::icon(d.src);
        if(i)
            v.src   = QString("/icon?id=%1").arg((quint64) i.id);
         else
            v.src   = "/icon/document.svg";
        if(d.alt.empty()){
            v.alt   = cdb::key(d.src);
        } else
            v.alt   = d.alt;
        write(v);
    }

    void    HtmlWriter::write(const html::Icon<Folder>&f)
    {
        html::Icon<String>   v;
        auto    ni  = cdb::nki(f.src);
        if(ni.icon)
            v.src   = QString("/icon?id=%1").arg((quint64) ni.icon.id);
         else
            v.src   = "/icon/folder.svg";
        if(f.alt.empty()){
            v.alt   = ni.key;
        } else
            v.alt   = f.alt;
        write(v);
    }
    
    void    HtmlWriter::write(const html::Icon<String>&v)
    {
        m_stream << "<img src=\"" << v.src << "\" class=\"";
        switch(icon_size()){
        case SizeDesc::Original:
            m_stream << "original";
            break;
        case SizeDesc::Large:
            m_stream << "large";
            break;
        case SizeDesc::Medium:
            m_stream << "medium";
            break;
        case SizeDesc::Small:
        default:
            m_stream << "small";
            break;
        }
        m_stream << "\"";
        if(!v.alt.empty())
            m_stream << " alt=\"" << v.alt.qString().toHtmlEscaped() << "\"";
        m_stream << ">";
    }
    

    void    HtmlWriter::write(const html::Italic&i)
    {
        m_stream << "<I>" << i.txt.qString().toHtmlEscaped() << "</I>\n";
    }

    void    HtmlWriter::write(const html::Mark& m)
    {
        m_stream << MarkdownWriter::exec(m.bytes).content;
    }
    //void    HtmlWriter::write(const html::Mark& md)
    //{
        //m_stream << runMarkdown(md.bytes);
    //}

    void    HtmlWriter::write(const html::Pre&p)
    {
        m_stream << "<PRE>" << p.bytes << "</PRE>";
    }
    
    void    HtmlWriter::write(const html::TabBar&bar)
    {
        m_stream << "<table class=\"tabbar\"><tr>\n";
        QByteArray  args    = reencode_parameters();
        for(const Page* p : bar.pages()){
            bool    us  = String(x_request->getPath()) == p->path();
            if(us)
                m_stream << "<td class=\"tb-select\">";
            else
                m_stream << "<td class=\"tabbar\">";
            if(!us)
                m_stream << "<a href=\"" << p->path().c_str() << "?" << args << "\">";
            m_stream << p->label();
            if(!us)
                m_stream << "</a>";
            m_stream << "</td>\n";
        }
        m_stream << "</tr></table>\n";
    }
    

    void    HtmlWriter::write(const html::Underline&u)
    {
        m_stream << "<U>" << u.txt.qString().toHtmlEscaped() << "</U>\n";
    }

    void    HtmlWriter::write(const Leaf& l)
    {
        m_stream << "<a href=\"/w/" << cdb::key(l) << "\">" << cdb::title(l) << "</a>";
    }


    void    HtmlWriter::write(SubmitLabel sl)
    {
        write(html::ISubmit(sl));
    }
    
    void    HtmlWriter::write(SubmitLabel::enum_t sl)
    {
        write(SubmitLabel(sl));
    }
    

    //  ------------------------------------------------------------------------

    HtmlWriter::KVRow::KVRow(KVRow&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    HtmlWriter::KVRow& HtmlWriter::KVRow::operator=(KVRow&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</TD></TR>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    HtmlWriter::KVRow::~KVRow()
    {
        if(m_html){
            m_html -> m_stream << "</TD></TR>\n";
            m_html  = nullptr;
        }
    }


    //  ------------------------------------------------------------------------
    HtmlWriter::Table::Table(Table&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    HtmlWriter::Table& HtmlWriter::Table::operator=(Table&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</TABLE>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    HtmlWriter::Table::~Table()
    {
        if(m_html){
            m_html -> m_stream << "</TABLE>\n";
            m_html  = nullptr;
        }
    }

    HtmlWriter::KVRow       HtmlWriter::Table::key(const String&k, const String& url)
    {
        if(m_html){
            return m_html -> key(k, url);
        } else
            return KVRow();
    }


    //  ------------------------------------------------------------------------
    HtmlWriter::FormAF::FormAF(FormAF&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    HtmlWriter::FormAF& HtmlWriter::FormAF::operator=(FormAF&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</FORM>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    HtmlWriter::FormAF::~FormAF()
    {
        if(m_html){
            m_html -> m_stream << "</FORM>\n";
            m_html  = nullptr;
        }
    }

    //  ------------------------------------------------------------------------
    HtmlWriter::LinkAF::LinkAF(LinkAF&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    HtmlWriter::LinkAF& HtmlWriter::LinkAF::operator=(LinkAF&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</A>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    HtmlWriter::LinkAF::~LinkAF()
    {
        if(m_html){
            m_html -> m_stream << "</A>\n";
            m_html  = nullptr;
        }
    }

    

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct HtmlPage : public Page {
        typedef void (*FN)(HtmlWriter&);
        FN m_fn;
        HtmlPage(HttpOp hOp,const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            HtmlWriter    h;

            if(!no_expand() && tabbar())
                h << *tabbar();

            m_fn(h);
            h.flush();
         
            if(no_expand()){
                dst         = h.steal();
            } else {
                x_content   = h.steal();
                x_title     = h.title();
                dst         = do_expand(default_page(), m_getters);
            } 
            return ContentType::html;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(HtmlWriter&))
{
    return Page::Writer(new HtmlPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


namespace {
    struct HtmlDispatcher : public Page {
        typedef void(*FN)(HtmlWriter&, const String&);
        FN      m_fn;
        HtmlDispatcher(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, true), m_fn(fn)
        {
        }
        
        ContentType     handle(QByteArray& dst, const QByteArray& path) const override
        {
            HtmlWriter    h;

            if(!no_expand() && tabbar())
                h << *tabbar();

            m_fn(h, path);
            h.flush();
         
            if(no_expand()){
                dst         = h.steal();
            } else {
                x_content   = h.steal();
                x_title     = h.title();
                dst         = do_expand(default_page(), m_getters);
            } 
            return ContentType::html;
        }
    };
}

Page::Writer    reg_dispatcher(HttpOp op, const String&path, void(*fn)(HtmlWriter&, const String&))
{
    return Page::Writer(new HtmlDispatcher(op, path, fn));
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


void        reg_tabbar(std::initializer_list<Page::Writer> wlist)
{
    new html::TabBar(wlist);
}
