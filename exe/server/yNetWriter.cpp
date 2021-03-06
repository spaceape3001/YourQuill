////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yCommon.hpp"
#include "yNetWriter.hpp"
#include "yPage.hpp"

#include "db/Cache.hpp"
#include "srv/SrvLogging.hpp"
//#include "core/IOFormatters.hpp"

#include "util/Execute.hpp"
#include "util/Utilities.hpp"
#include <httpserver/httprequest.h>


#include <QColor>
#include <QDir>
#include <QFile>
#include <QRect>
#include <QUrl>
#include <stdio.h>


    NetWriter::NetWriter() : m_stream(&m_bytes)
    {
        m_stream.setCodec("UTF-8");
    }

    NetWriter::~NetWriter()
    {
    }

    bool    NetWriter::append_document(const QString& documentname, bool fSuppressMsg)
    {
        QFile   document(documentname);
        if(!document.open(QIODevice::ReadOnly)){
            if(!fSuppressMsg)
                srvError << "Unable to read document " << documentname;
            return false;
        }
        
        m_stream << document.readAll();
        document.close();
        return true;
    }


    void    NetWriter::flush()
    {
        m_stream.flush();
    }

    QByteArray          NetWriter::steal()
    {
        return QByteArray(std::move(m_bytes));
    }

    void    NetWriter::title(const char*z)
    {
        m_title = z;
    }
    
    void    NetWriter::title(const QByteArray&z)
    {
        m_title = z;
    }
    
    void    NetWriter::title(const QString&z)
    {
        m_title = utf8(z);
    }
    
    void    NetWriter::title(const String&z)
    {
        m_title = z.c_str();
    }

    void    NetWriter::write(bool f)
    {
        m_stream << (f?"Yes":"No");
    }

    void    NetWriter::write(const String&s)
    {
        m_stream << s.qString();
    }

    void    NetWriter::write(const QColor&v)
    {
        if(v.isValid())
            m_stream << v.name();
        else
            m_stream << "(invalid)";
    }

    //void    NetWriter::write(const QDate& dt)
    //{
        //m_stream << toGUIString(dt);
    //}

    //void    NetWriter::write(const QDateTime&dt)
    //{
        //m_stream << toGUIString(dt);
    //}

    void    NetWriter::write(const QDir& dir)
    {
        m_stream << dir.absolutePath();
    }

    void    NetWriter::write(const QRect&r)
    {
        char    buffer[256];
        sprintf(buffer, "%dx%d%+d%+d", r.width(), r.height(), r.left(), r.top());
        m_stream << buffer;
    }

    void    NetWriter::write(const QUrl&url)
    {
        m_stream << url.toString();
    }

    void    NetWriter::write(const QStringList&data, const QString& sep)
    {
        m_stream << data.join(sep);
    }

    //void    NetWriter::write(const QVariant&var)
    //{
        //m_stream << variantToIOString(var);
    //}

    void    NetWriter::write_in(NetWriter&& mv)
    {
        mv.flush();
        //mv.finish();
        m_stream << mv.m_bytes;
        mv.m_bytes.clear();
        //mv.m_stream.close();
    }


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    Css::Css() = default;
    Css::~Css() = default;

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

    Html::Html() = default;
    Html::~Html() = default;

    Html::FormAF      Html::form(const QString& url, const QString& args, bool fInspectOverride)
    {
        write(html::Form{url, args, fInspectOverride});
        return FormAF(this);
    }
    

    Html::KVRow       Html::key(const QString& k, const QString& url)
    {
        m_stream << "<tr><th align=\"left\">";
        if(!url.isEmpty())
            m_stream << "<a href=\"" << url << "\">";
        m_stream << k.toHtmlEscaped();
        if(!url.isEmpty())
            m_stream << "</a>";
        m_stream << "</th><td>";
        return KVRow(this);
    }

    Html::LinkAF      Html::link(const QString&url)
    {
        m_stream << "<a href=\"" << url << "\">";
        return LinkAF(this);
    }

    Html::Table       Html::table()
    {
        m_stream << "<TABLE>";
        return Table(this);
    }
    

    void    Html::write(const Comma&c)
    {
        m_stream << c.text();
    }

    void    Html::write(const Class& cl)
    {
        auto ni = cdb::nki(cl,true);
        m_stream << "<a href=\"/class?key=" << ni.key << "\">" << ni.name << "</a>";
    }

    void    Html::write(const Field& f)
    {
        auto    ni  = cdb::nki(f, true);
                //  No URL (yet)
        m_stream << ni.name;
    }

    void    Html::write(const Folder& fo)
    {
        auto ni = cdb::nki(fo, true);
        m_stream << "<a href=\"/folder?key=" << ni.key << "\">" << ni.name << "</a>";
    }

    void    Html::write(const Graph& g)
    {
        QString n = cdb::name(g).toHtmlEscaped();
        m_stream << "<img src=\"/graph?id=" << (quint64) g.id << "\" alt=\"" << n << "\" usemap=\"#" << n << "\">\n" 
            << cdb::html(g) << "\n";
    }

    void    Html::write(const html::Bold&b)
    {
        m_stream << "<B>" << b.txt.toHtmlEscaped() << "</B>\n";
    }

    void    Html::write(const html::Config<Class>& c)
    {
        auto ni = cdb::nki(c.v, true);
        m_stream << "<a href=\"/wksp/class?key=" << ni.key << "\">" << ni.name << "</a>";
    }

    void    Html::write(const html::ControlRoot&)
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
                *this << "<a href=\"" << nonroot << r->key() << "\">";
            *this << r -> name();
            if(r != x_root)
                *this << "</a>";
            *this  << "</td></tr>\n";
        }
        *this << "</table>\n";
    }
    
    
    void    Html::write(const html::Dev<Atom>&c)
    {
        m_stream << "<a href=\"/dev/atom?id=" << c.v.id << "\">" << cdb::key(c.v) << "</a>";
    }

    void    Html::write(const html::Dev<Class>&c)
    {
        m_stream << "<a href=\"/dev/class?id=" << c.v.id << "\">" << cdb::key(c.v) << "</a>";
    }

    void    Html::write(const html::Dev<Directory>&d)
    {
        m_stream << "<a href=\"/dev/directory?id=" << d.v.id << "\">" << cdb::path(d.v) << "</a>";
    }

    void    Html::write(const html::Dev<Document>&f)
    {
        m_stream << "<a href=\"/dev/document?id=" << f.v.id << "\">" << cdb::key(f.v) << "</a>";
    }

    void    Html::write(const html::Dev<Folder>& fo)
    {
        QString k   = cdb::key(fo.v);
        if(k.isEmpty())
            k   = "/";
        m_stream << "<a href=\"/dev/folder?id=" << k << "\">" << k << "</a>";
    }

    void    Html::write(const html::Dev<Fragment>&a)
    {
        m_stream << "<a href=\"/dev/fragment?id=" << a.v.id << "\">" << cdb::path(a.v) << "</a>";
    }

    void    Html::write(const html::Dev<Leaf>&c)
    {
        QString k = cdb::key(c.v);
        m_stream << "<a href=\"/dev/leaf?id=" << c.v.id << "\">" << k << "</a>";
    }

    void    Html::write(const html::Dev<const Root*>&r)
    {
        if(!r.v)
            return ;
        m_stream << "<a href=\"/dev/root?id=" << r.v->id() << "\">" << r.v->path() << "</a>";
    }
    
    void    Html::write(const html::Dev<Tag>&c)
    {
        auto k = cdb::key(c.v);
        m_stream << "<a href=\"/dev/tag?k=" << k << "\">" << k << "</a>";
    }

    void    Html::write(const html::Edit<Class>& arg)
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
                f.args   = QString("id=%1&root=%2").arg(x_class.id).arg(r.root->id());
            else
                f.args   = QString("root=%1").arg(r.root->id());
        } else {
            if(r.value)
                f.args   = QString("id=%1").arg(x_class.id);
        }
        f.inspect   = r.inspect;
        write(f);
        
    }

    void    Html::write(const html::Escape&e)
    {
        m_stream << e.txt.toHtmlEscaped();
    }

    void    Html::write(const html::Form&v)
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

    void    Html::write(const html::Full<Folder>& fo)
    {
        m_stream << "<a href=\"/folder?key=" << cdb::key(fo.v) << "\">";
        if(fo.v == cdb::top_folder())
            m_stream << "Root (/)";
        else
            m_stream << cdb::key(fo.v);
        m_stream << "</a>\n";
    }


    void    Html::write(const html::H1&h)
    {
        m_stream << "<H1>" << h.txt.toHtmlEscaped() << "</H1>\n";
    }
    
    void    Html::write(const html::H2&h)
    {
        m_stream << "<H2>" << h.txt.toHtmlEscaped() << "</H2>\n";
    }
    
    void    Html::write(const html::H3&h)
    {
        m_stream << "<H3>" << h.txt.toHtmlEscaped() << "</H3>\n";
    }
    
    void    Html::write(const html::H4&h)
    {
        m_stream << "<H4>" << h.txt.toHtmlEscaped() << "</H4>\n";
    }
    
    void    Html::write(const html::H5&h)
    {
        m_stream << "<H5>" << h.txt.toHtmlEscaped() << "</H5>\n";
    }
    
    void    Html::write(const html::H6&h)

    {
        m_stream << "<H6>" << h.txt.toHtmlEscaped() << "</H6>\n";
    }
    
    void    Html::write(const html::Image&img)
    {
        if(!img.src.isEmpty()){
            m_stream << "<IMG SRC=\"" << img.src.toHtmlEscaped() << '"';
            if(!img.alt.isEmpty())
                m_stream << " ALT=\"" << img.alt.toHtmlEscaped() << '"';
            if(!img.style.isEmpty())
                m_stream << " STYLE=\"" << img.style.toHtmlEscaped() << '"';
            m_stream << "/>";
        }
    }

    void        Html::write(const html::ICheck& v)
    {
        m_stream << "<input type=\"checkbox\" id=\"" << v.key << "\" name=\"" << v.key << "\" value=\"true\"";
        if(v.checked)
            m_stream << " checked=\"true\"";
        m_stream << "><label for=\"" << v.key << "\">" << v.label << "</label>";
    }

    void        Html::write(const html::IEditReq&)
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
    
    void        Html::write(const html::IHidden&v)
    {
        m_stream << "<input type=\"hidden\" id=\"" << v.key << "\" name=\"" << v.key << "\" value=\"" << v.value << "\">";
    }

    void        Html::write(const html::IKey&)
    {
        m_stream << "<input type=\"text\" name=\"key\" id=\"key\">";
    }
    
    void        Html::write(const html::ILabel&v)
    {
        m_stream << "<label for=\"" << v.key << "\">" << v.label.toHtmlEscaped() << "</label>";
    }

    void    Html::write(const html::ILine& i)
    {
        m_stream << "<input type=\"text\" name=\"" << i.key.toHtmlEscaped() << "\" id=\"" << i.key.toHtmlEscaped() << "\" value=\"" << i.value.toHtmlEscaped() << "\"";
        if(i.size > 0)
            m_stream << " size=\"" << i.size << "\"";
        m_stream << ">";
    }
    
    void        Html::write(const html::IRoot&v)
    {
        *this << "Change Root:<br>";
        const Root* re = def_root(v.dr);
        for(const Root* rt : wksp::roots()){
            *this << "<input type=\"radio\" name=\"root\" id=\"" << rt->key() << "\" value=\"" << rt->id() 
                << "\"><label for=\"" << rt->key() << "\">";
            if(rt == re)
                *this << "<b><u>";
            *this << "[" << rt->key() << "] " << rt->name();
            if(rt == re)
                *this << "</u></b>";
            *this  << "</label><br>\n";
        }
    }
    
    void        Html::write(const html::ISubmit&v)
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

    void    Html::write(const html::IText& v)
    {
        m_stream << "<textarea id=\"" << v.key << "\" name=\"" << v.key << "\" rows=\"" << v.rows << "\" cols=\"" 
            << v.cols << "\">" << v.value.toHtmlEscaped() << "</textarea>";
    }

    void    Html::write(const html::Icon<Document>&d)
    {
        html::Icon<QString>   v;
        Image   i   = cdb::icon(d.src);
        if(i)
            v.src   = QString("/icon?id=%1").arg((quint64) i.id);
         else
            v.src   = "/icon/document.svg";
        if(d.alt.isEmpty()){
            v.alt   = cdb::key(d.src);
        } else
            v.alt   = d.alt;
        write(v);
    }

    void    Html::write(const html::Icon<Folder>&f)
    {
        html::Icon<QString>   v;
        auto    ni  = cdb::nki(f.src);
        if(ni.icon)
            v.src   = QString("/icon?id=%1").arg((quint64) ni.icon.id);
         else
            v.src   = "/icon/folder.svg";
        if(f.alt.isEmpty()){
            v.alt   = ni.key;
        } else
            v.alt   = f.alt;
        write(v);
    }
    
    void    Html::write(const html::Icon<QString>&v)
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
        if(!v.alt.isEmpty())
            m_stream << " alt=\"" << v.alt.toHtmlEscaped() << "\"";
        m_stream << ">";
    }
    

    void    Html::write(const html::Italic&i)
    {
        m_stream << "<I>" << i.txt.toHtmlEscaped() << "</I>\n";
    }

    void    Html::write(const html::Mark& m)
    {
        m_stream << Markdown::exec(m.bytes).content;
    }
    //void    Html::write(const html::Mark& md)
    //{
        //m_stream << runMarkdown(md.bytes);
    //}

    void    Html::write(const html::Pre&p)
    {
        m_stream << "<PRE>" << p.bytes << "</PRE>";
    }
    
    void    Html::write(const html::TabBar&bar)
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
    

    void    Html::write(const html::Underline&u)
    {
        m_stream << "<U>" << u.txt.toHtmlEscaped() << "</U>\n";
    }


    void    Html::write(SubmitLabel sl)
    {
        write(html::ISubmit(sl));
    }
    
    void    Html::write(SubmitLabel::enum_t sl)
    {
        write(SubmitLabel(sl));
    }
    

    //  ------------------------------------------------------------------------

    Html::KVRow::KVRow(KVRow&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    Html::KVRow& Html::KVRow::operator=(KVRow&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</TD></TR>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    Html::KVRow::~KVRow()
    {
        if(m_html){
            m_html -> m_stream << "</TD></TR>\n";
            m_html  = nullptr;
        }
    }


    //  ------------------------------------------------------------------------
    Html::Table::Table(Table&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    Html::Table& Html::Table::operator=(Table&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</TABLE>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    Html::Table::~Table()
    {
        if(m_html){
            m_html -> m_stream << "</TABLE>\n";
            m_html  = nullptr;
        }
    }

    Html::KVRow       Html::Table::key(const QString&k, const QString& url)
    {
        if(m_html){
            return m_html -> key(k, url);
        } else
            return KVRow();
    }


    //  ------------------------------------------------------------------------
    Html::FormAF::FormAF(FormAF&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    Html::FormAF& Html::FormAF::operator=(FormAF&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</FORM>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    Html::FormAF::~FormAF()
    {
        if(m_html){
            m_html -> m_stream << "</FORM>\n";
            m_html  = nullptr;
        }
    }

    //  ------------------------------------------------------------------------
    Html::LinkAF::LinkAF(LinkAF&& mv) : m_html(mv.m_html)
    {
        mv.m_html = nullptr;
    }
    
    Html::LinkAF& Html::LinkAF::operator=(LinkAF&&mv)
    {
        if(this != &mv){
            if(m_html)
                m_html -> m_stream << "</A>\n";
            m_html      = mv.m_html;
            mv.m_html   = nullptr;
        }
        return *this;
    }
    
    Html::LinkAF::~LinkAF()
    {
        if(m_html){
            m_html -> m_stream << "</A>\n";
            m_html  = nullptr;
        }
    }

    
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    Markdown::CT        Markdown::exec(const QByteArray&buf)
    {
        static const QStringList    s_markdown      = QStringList() << wksp::markdown();
        static const QStringList    s_smartypants   = QStringList() << wksp::smartypants();
        static const QString        s_perl          = wksp::perl();
        
        CT              ret;
        QByteArray      clean;
        QByteArrayList  list;
        //  First, find the title.....
        for(const QByteArray& b : buf.split('\n')){
            if(b.startsWith("#! ")){
                ret.title = b.mid(3).trimmed();
            } else
                list << b;
        }
        clean = list.join('\n');
        
        Vector<ProcessDescriptor>  procs;
        procs << ProcessDescriptor( s_perl, s_markdown,     500);
        procs << ProcessDescriptor( s_perl, s_smartypants,  500);
        ret.content = executeProcessSeries(procs, clean);        
        return ret;
    }


    Markdown::Markdown() = default;
    Markdown::~Markdown() = default;


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    Text::Text() = default;
    Text::~Text() = default;
    
