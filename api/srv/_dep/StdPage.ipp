////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
        
        String k    = String(z+n, i-n);
        FNGet   fn  = vars.get(k,nullptr);
        if(!fn)
            fn  = Page::static_getter(k);
        if(fn)
            ret.append(fn().qBytes());
        n   = i + 2;
    }
    
    if(n >= 0)
        ret.append(z+n);
    return ret;
}


ContentType                  do_direct_content(String&dst, const String&data, ContentType ret, const String&title)
{
    switch(ret){
    case ContentType::html:
        x_title     = title.qBytes();
        x_content   = data.qBytes();
        dst         = do_expand(Page::default_page().qBytes());
        return ContentType::html;
    case ContentType::markdown:
        {
            auto ct     = MarkdownWriter::exec(data.qBytes());
            x_title     = ct.title;
            if(x_title.isEmpty())
                x_title = title.qBytes();
            x_content   = ct.content;
            dst         = do_expand(Page::default_page().qBytes());
            return ContentType::html;
        }
        return ContentType::html;
    case ContentType::text:
        x_title     = title.qBytes();
            x_content   = String("<PRE>" + data + "</PRE>").qBytes();
        dst         = do_expand(Page::default_page().qBytes());
        return ContentType::html;
    default:
        break;
    };
    dst     = data;
    return ret;
}


ContentType                 do_direct_file(String& dst, const String&file, bool fExpand)
{
    String  data    = file_bytes(file);
    QFileInfo   fi(file.qString());
    ContentType ret = mimeTypeForExt(fi.suffix());
    switch(ret){
    case ContentType::html:
        if(fExpand){
            x_title     = fi.fileName().toUtf8();
            x_content   = data.qBytes();
            dst         = do_expand(Page::default_page().qBytes());
            return ContentType::html;
        }
        break;
    case ContentType::markdown:
        if(fExpand){
            auto ct     = MarkdownWriter::exec(data.qBytes());
            x_title     = ct.title;
            if(x_title.isEmpty())
                x_title = fi.fileName().toUtf8();
            x_content   = ct.content;
            dst         = do_expand(Page::default_page().qBytes());
            return ContentType::html;
        }
        break;
    case ContentType::text:
        if(fExpand){
            x_content   = String("<PRE>" + data + "</PRE>").qBytes();
            x_title     = fi.fileName().toUtf8();
            dst         = do_expand(Page::default_page().qBytes());
            return ContentType::html;
        }
        break;
    default:
        break;
    }
    dst     = data.qBytes();
    return ret;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct NonePage : public Page {
        typedef void (*FN)();
        FN          m_fn;
        ContentType    handle(String& dst) const override
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
        
        ContentType    handle(Stream& dst) const override
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
        typedef ContentType (*FN)(String&);
        FN                  m_fn;
        
        ContentType    handle(Stream& dst) const override
        {
            String  buf;
            auto ct =  m_fn(dst);
            dst << buf;
            return ct;
        }
        
        GenericPage(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
    };
}

Page::Writer     reg_page(HttpOp op, const String& path, ContentType (*fn)(String&))
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
        
        ContentType     handle(Stream& dst) const override
        {
            return do_direct_file(dst, m_dir.absoluteFilePath(QString::fromUtf8(x_pathinfo)));
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
        ContentType    handle(String& dst) const override
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




