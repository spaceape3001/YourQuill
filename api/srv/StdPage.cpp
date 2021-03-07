////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Page.hpp"
#include "MarkdownWriter.hpp"
#include "TLSGlobals.hpp"

#include <util/FileUtils.hpp>
#include <util/Utilities.hpp>

#include <QDir>
#include <QFileInfo>


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


QByteArray                   do_expand(const QByteArray&content, const GetMap&vars)
{
    QByteArray      ret;
    const char*     z   = content.constData();
    int             n = 0;
    int             i = 0;
    while( (i = content.indexOf("{{", n)) >= 0){
        ret.append(z+n, i-n);
        n = i + 2;
        i   = content.indexOf("}}", n);
        if(i < 0){
            n = i = -1;
            break;
        }
        
        QByteArray k    = QByteArray(z+n, i-n);
        FNGet   fn  = vars.get(k,nullptr);
        if(!fn)
            fn  = Page::static_getter(k);
        if(fn)
            ret.append(fn());
        n   = i + 2;
    }
    
    if(n >= 0)
        ret.append(z+n);
    return ret;
}


ContentType                  do_direct_content(QByteArray&dst, const QByteArray&data, ContentType ret, const QByteArray&title)
{
    switch(ret){
    case ContentType::html:
        x_title     = title;
        x_content   = data;
        dst         = do_expand(Page::default_page());
        return ContentType::html;
    case ContentType::markdown:
        {
            auto ct     = MarkdownWriter::exec(data);
            x_title     = ct.title;
            if(x_title.isEmpty())
                x_title = title;
            x_content   = ct.content;
            dst         = do_expand(Page::default_page());
            return ContentType::html;
        }
        return ContentType::html;
    case ContentType::text:
        x_title     = title;
        x_content   = "<PRE>" + data + "</PRE>";
        dst         = do_expand(Page::default_page());
        return ContentType::html;
    default:
        break;
    };
    dst     = data;
    return ret;
}


ContentType                 do_direct_file(QByteArray& dst, const QString&file, bool fExpand)
{
    QByteArray  data    = file_bytes(file);
    QFileInfo   fi(file);
    ContentType ret = mimeTypeForExt(fi.suffix());
    switch(ret){
    case ContentType::html:
        if(fExpand){
            x_title     = fi.fileName().toUtf8();
            x_content   = std::move(data);
            dst         = do_expand(Page::default_page());
            return ContentType::html;
        }
        break;
    case ContentType::markdown:
        if(fExpand){
            auto ct     = MarkdownWriter::exec(data);
            x_title     = ct.title;
            if(x_title.isEmpty())
                x_title = fi.fileName().toUtf8();
            x_content   = ct.content;
            dst         = do_expand(Page::default_page());
            return ContentType::html;
        }
        break;
    case ContentType::text:
        if(fExpand){
            x_content   = "<PRE>" + data + "</PRE>";
            x_title     = fi.fileName().toUtf8();
            dst         = do_expand(Page::default_page());
            return ContentType::html;
        }
        break;
    default:
        break;
    }
    dst     = std::move(data);
    return ret;
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

Page::Writer     reg_page(HttpOp op, const String&path, void (*fn)())
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

Page::Writer     reg_page(HttpOp op, const String&path, HttpStatus (*fn)())
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

Page::Writer     reg_page(HttpOp op, const String& path, ContentType (*fn)(QByteArray&))
{
    return Page::Writer(new GenericPage(op, path, fn));
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

Page::Writer    reg_dispatcher(const String&path, const QDir&dir)
{
    return Page::Writer(new DirDispatcher(hGet, path, dir));
}

Page::Writer    reg_dispatcher(HttpOp hOp, const String&path, const QDir&dir)
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

Page::Writer    reg_dispatcher(HttpOp op, const String&path, void(*fn)(const String&))
{
    return Page::Writer(new NoneDispatcher(op, path, fn));
}




