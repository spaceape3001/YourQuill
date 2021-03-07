////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yCommon.hpp"


#include "yPage.hpp"
#include "yNetWriter.hpp"

#include "db/ShareDir.hpp"

#include "util/DelayInit.hpp"
#include "util/EnumMap.hpp"
#include "util/Guarded.hpp"
#include "util/Map.hpp"
#include "util/Utilities.hpp"
#include "util/Vector.hpp"

#include "db/Workspace.hpp"

#include <QBuffer>
#include <QDir>
#include <QImage>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QSvgGenerator>



Redirect::Redirect(const QUrl& b ) : why(HttpStatus::TemporaryRedirect), where(b.toString().toUtf8())
{
}

Redirect::Redirect(HttpStatus a, const QUrl& b ) : why(a), where(b.toString().toUtf8())
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void        tabbar(std::initializer_list<Page::Writer> wlist)
{
    new html::TabBar(wlist);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
void            reg_page(Page*);

namespace {
    struct PageRepo {
        Vector<const Page*>                                 all;
    };
    
    PageRepo&   repo()
    {
        static PageRepo s_repo;
        return s_repo;
    }
};


const Vector<const Page*>&   Page::all()
{
    return repo().all;
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Page::Page(HttpOp h, const String& path, bool fDisp): m_op(h), m_path(path), m_tabbar(nullptr), 
    m_loginReq(false), m_localOnly(false), 
    m_postAnon(false), m_noExpand(false), m_dispatch(fDisp)
{
    repo().all << this;
    reg_page(this);
}

Page::~Page()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Page::Writer::Writer(Page* h) : m_page(h)
{
}


Page::Writer& Page::Writer::anonymous()
{
    if(m_page)
        m_page -> m_postAnon = true;
    return *this;
}

Page::Writer& Page::Writer::argument(const QByteArray&k, const QByteArray& d)
{
    if(m_page){
        Arg a;
        a.name  = k;
        a.description   = d;
        m_page -> m_args << a;
    }
    return *this;
}

Page::Writer& Page::Writer::description(const QByteArray&d)
{
    if(m_page)
        m_page -> m_description = d;
    return *this;
}

Page::Writer& Page::Writer::getter(const QByteArray&k, FNGet fn)
{
    if(m_page && fn && !k.isEmpty())
        m_page -> m_getters[k]   = fn;
    return *this;
}

Page::Writer& Page::Writer::icon(const QByteArray&i)
{
    if(m_page)
        m_page -> m_icon = i;
    return *this;
}

Page::Writer& Page::Writer::label(const QByteArray&v)
{
    if(m_page)
        m_page -> m_label   = v;
    return *this;
}


Page::Writer& Page::Writer::login()
{
    if(m_page)
        m_page -> m_loginReq = true;
    return *this;
}

Page::Writer& Page::Writer::local()
{
    if(m_page)
        m_page -> m_localOnly = true;
    return *this;
}


Page::Writer& Page::Writer::no_expand()
{
    if(m_page)
        m_page  -> m_noExpand   = true;
    return *this;
}

Page::Writer& Page::Writer::id()
{
    argument(sz_id, "Numeric identiifer to resource");
    return *this;
}

Page::Writer& Page::Writer::key()
{
    argument(sz_key, "Textual identifier to resource");
    return *this;
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct NonePage : public Page {
        typedef void (*FN)();
        FN          m_fn;
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            m_fn();
            throw HttpStatus::Teapot;
        }
        
        NonePage(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
    };
}

Page::Writer     page(HttpOp op, const String&path, void (*fn)())
{
    return Page::Writer(new NonePage(op, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct ControlPage : public Page {
        typedef HttpStatus (*FN)();
        FN                  m_fn;
        
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            throw m_fn();
        }
        
        ControlPage(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
    };
}

Page::Writer     page(HttpOp op, const String&path, HttpStatus (*fn)())
{
    return Page::Writer(new ControlPage(op, path, fn));
}

namespace {
    struct GenericPage : public Page {
        typedef ContentType (*FN)(QByteArray&);
        FN                  m_fn;
        
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            return m_fn(dst);
        }
        
        GenericPage(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
    };
}

Page::Writer     page(HttpOp op, const String& path, ContentType (*fn)(QByteArray&))
{
    return Page::Writer(new GenericPage(op, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct CssPage : public Page {
        typedef void (*FN)(Css&);
        FN          m_fn;
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            Css css;
            m_fn(css);
            css.flush();
            dst = css.steal();
            return ContentType::css;;
        }
        
        CssPage(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
    };
}


Page::Writer     page(HttpOp op, const String& path, void (*fn)(Css&))
{
    return Page::Writer(new CssPage(op, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct HtmlPage : public Page {
        typedef void (*FN)(Html&);
        FN m_fn;
        HtmlPage(HttpOp hOp,const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            Html    h;

            if(!no_expand() && tabbar())
                h << *tabbar();

            m_fn(h);
            h.flush();
         
            if(no_expand()){
                dst         = h.steal();
            } else {
                x_content   = h.steal();
                x_title     = h.title();
                dst         = do_expand(def_page(), m_getters);
            } 
            return ContentType::html;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, void(*fn)(Html&))
{
    return Page::Writer(new HtmlPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct ImagePage : public Page {
        typedef void (*FN)(QImage&);
        FN m_fn;
        
        ImagePage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }

        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            QImage    img;
            m_fn(img);
            dst         = make_png(img);
            return ContentType::png; 
        }
    };
}


Page::Writer     page(HttpOp hOp, const String& path, void(*fn)(QImage&))
{
    return Page::Writer(new ImagePage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonArrPage : public Page {
        typedef void (*FN)(QJsonArray&);
        FN m_fn;
        JsonArrPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }

        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            QJsonArray      arr;
            m_fn(arr);
            doc.setArray(arr);
            dst   = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, void(*fn)(QJsonArray&))
{
    return Page::Writer(new JsonArrPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonArrRetPage : public Page {
        typedef QJsonArray (*FN)();
        FN m_fn;
        JsonArrRetPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }

        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            doc.setArray(m_fn());
            dst   = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, QJsonArray(*fn)())
{
    return Page::Writer(new JsonArrRetPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonDocPage : public Page {
        typedef void (*FN)(QJsonDocument&);
        FN m_fn;
        JsonDocPage(HttpOp hOp,  const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            m_fn(doc);
            dst   = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, void(*fn)(QJsonDocument&))
{
    return Page::Writer(new JsonDocPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonDocRetPage : public Page {
        typedef QJsonDocument (*FN)();
        FN m_fn;
        JsonDocRetPage(HttpOp hOp,  const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            dst   = (m_fn()).toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, QJsonDocument(*fn)())
{
    return Page::Writer(new JsonDocRetPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonObjPage : public Page {
        typedef void (*FN)(QJsonObject&);
        FN m_fn;
        JsonObjPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray&dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            QJsonObject     obj;
            m_fn(obj);
            doc.setObject(obj);
            dst = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, void(*fn)(QJsonObject&))
{
    return Page::Writer(new JsonObjPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonObjRetPage : public Page {
        typedef QJsonObject (*FN)();
        FN m_fn;
        JsonObjRetPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray&dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            doc.setObject(m_fn());
            dst = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, QJsonObject(*fn)())
{
    return Page::Writer(new JsonObjRetPage(hOp, path, fn));
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//  MARKDOWN (TODO)


namespace {
    struct MarkdownPage : public Page {
        typedef void (*FN)(Markdown&);
        FN m_fn;
        MarkdownPage(HttpOp hOp, const String& path, FN fn) : Page(hOp,  path, false), m_fn(fn)
        {
        }
        
        ContentType  handle(QByteArray& dst, const QByteArray&) const override
        {
            Markdown    md;
            m_fn(md);
            md.flush();
            auto ct = Markdown::exec(md.steal());
            x_title = md.title();
            if(x_title.isEmpty() && !ct.title.isEmpty())
                x_title     = ct.title;
            x_content   = std::move(ct.content);
            dst         = do_expand(def_page(), m_getters);
            return ContentType::html;
        }
    };
}

Page::Writer     page(HttpOp hOp, Match m, const QByteArray& path, void(*fn)(Markdown&))
{
    return Page::Writer(new MarkdownPage(hOp, path, fn));
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


namespace {
    struct SvgPage : public Page {
        typedef void(*FN)(QSvgGenerator&);
        SvgPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn){}

        ContentType    handle(QByteArray&dst, const QByteArray&) const override
        {
            QBuffer buf(&dst);
            buf.open(QIODevice::ReadWrite);
            {
                QSvgGenerator   svg;
                svg.setOutputDevice(&buf);
                m_fn(svg);
            }
            buf.close();
            return ContentType::svg;
        }
    private:
        FN  m_fn;
    };
}

Page::Writer     page(HttpOp hOp, const String& path, void(*fn)(QSvgGenerator&))
{
    return Page::Writer(new SvgPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct TextPage : public Page {
        typedef void (*FN)(Text&);
        FN m_fn;
        TextPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            Text    txt;
            m_fn(txt);
            txt.flush();
            x_title.clear();
            x_content   = "<PRE>" + txt.steal() + "</PRE>";
            dst         = do_expand(def_page(), m_getters);
            return ContentType::html;
        }
    };
}

Page::Writer     page(HttpOp hOp, const String& path, void(*fn)(Text&))
{
    return Page::Writer(new TextPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct DirDispatcher : public Page {
        QDir        m_dir;
        DirDispatcher(HttpOp hOp, const String& path, const QDir& dir) : Page(hOp, path, true), m_dir(dir)
        {
        }
        
        ContentType     handle(QByteArray& dst, const QByteArray& path) const override
        {
            return do_direct_file(dst, m_dir.absoluteFilePath(utf8(path)));
        }
    };
}

Page::Writer    dispatcher(const String&path, const QDir&dir)
{
    return Page::Writer(new DirDispatcher(hGet, path, dir));
}

Page::Writer    dispatcher(HttpOp hOp, const String&path, const QDir&dir)
{
    return Page::Writer(new DirDispatcher(hOp, path, dir));
}

namespace {
    struct NoneDispatcher : public Page {
        typedef void (*FN)(const String&);
        FN          m_fn;
        ContentType    handle(QByteArray& dst, const QByteArray&path) const override
        {
            m_fn(path);
            throw HttpStatus::Teapot;
        }
        
        NoneDispatcher(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
    };
}

Page::Writer    dispatcher(HttpOp op, const String&path, void(*fn)(const String&))
{
    return Page::Writer(new NoneDispatcher(op, path, fn));
}


namespace {
    struct HtmlDispatcher : public Page {
        typedef void(*FN)(Html&, const String&);
        FN      m_fn;
        HtmlDispatcher(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, true), m_fn(fn)
        {
        }
        
        ContentType     handle(QByteArray& dst, const QByteArray& path) const override
        {
            Html    h;

            if(!no_expand() && tabbar())
                h << *tabbar();

            m_fn(h, path);
            h.flush();
         
            if(no_expand()){
                dst         = h.steal();
            } else {
                x_content   = h.steal();
                x_title     = h.title();
                dst         = do_expand(def_page(), m_getters);
            } 
            return ContentType::html;
        }
    };
}

Page::Writer    dispatcher(HttpOp op, const String&path, void(*fn)(Html&, const String&))
{
    return Page::Writer(new HtmlDispatcher(op, path, fn));
}

namespace {
    struct MarkdownDispatcher : public Page {
        typedef void (*FN)(Markdown&, const String&);
        FN m_fn;
        MarkdownDispatcher(HttpOp hOp, const String& path, FN fn) : Page(hOp,  path, true), m_fn(fn)
        {
        }
        
        ContentType  handle(QByteArray& dst, const QByteArray&path) const override
        {
            Markdown    md;
            m_fn(md, path);
            md.flush();
            auto ct = Markdown::exec(md.steal());
            x_title = md.title();
            if(x_title.isEmpty() && !ct.title.isEmpty())
                x_title     = ct.title;
            x_content   = std::move(ct.content);
            dst         = do_expand(def_page(), m_getters);
            return ContentType::html;
        }
    };
}

Page::Writer    dispatcher(HttpOp op, const String&path, void(*fn)(Markdown&, const String&))
{
    return Page::Writer(new MarkdownDispatcher(op, path, fn));
}


