////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "QByteArrayStream.hpp"

MarkdownWriter::CT        MarkdownWriter::exec(const QByteArray&buf)
{
    static const QStringList    s_markdown      = QStringList() << QString::fromStdString(wksp::markdown().string());
    static const QStringList    s_smartypants   = QStringList() << QString::fromStdString(wksp::smartypants().string());
    static const QString        s_perl          = QString::fromStdString(wksp::perl().string());
    
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


MarkdownWriter::MarkdownWriter(Stream& str) : m_stream(str)
{
}

MarkdownWriter::~MarkdownWriter() = default;

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct MarkdownPage : public Page {
        typedef void (*FN)(MarkdownWriter&);
        FN m_fn;
        MarkdownPage(HttpOp hOp, const String& path, FN fn) : Page(hOp,  path, false), m_fn(fn)
        {
        }
        
        ContentType  handle(String& dst, const String&) const override
        {
            QByteArray          buff;
            QByteArrayStream    bas(buff);
            MarkdownWriter      md(bas);
            m_fn(md);
            auto ct = MarkdownWriter::exec(buff);
            x_title = md.title();
            if(x_title.empty() && !ct.title.isEmpty())
                x_title     = ct.title;
            x_content   = std::move(ct.content);
            dst         = do_expand(default_page().qBytes(), m_getters);
            return ContentType::html;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(MarkdownWriter&))
{
    return Page::Writer(new MarkdownPage(hOp, path, fn));
}


namespace {
    struct MarkdownDispatcher : public Page {
        typedef void (*FN)(MarkdownWriter&, const String&);
        FN m_fn;
        MarkdownDispatcher(HttpOp hOp, const String& path, FN fn) : Page(hOp,  path, true), m_fn(fn)
        {
        }
        
        ContentType  handle(String& dst, const String&path) const override
        {
            MarkdownWriter    md;
            m_fn(md, path);
            md.flush();
            auto ct = MarkdownWriter::exec(String(md.bytes()).qBytes());
            x_title = md.title().qBytes();
            if(x_title.isEmpty() && !ct.title.isEmpty())
                x_title     = ct.title;
            x_content   = std::move(ct.content);
            dst         = do_expand(default_page().qBytes(), m_getters);
            return ContentType::html;
        }
    };
}

Page::Writer    reg_dispatcher(HttpOp op, const String&path, void(*fn)(MarkdownWriter&, const String&))
{
    return Page::Writer(new MarkdownDispatcher(op, path, fn));
}



