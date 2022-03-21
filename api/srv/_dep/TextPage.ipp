////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

TextWriter::TextWriter() = default;
TextWriter::~TextWriter() = default;

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct TextPage : public Page {
        typedef void (*FN)(TextWriter&);
        FN m_fn;
        TextPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType    handle(String& dst, const String&) const override
        {
            TextWriter    txt;
            m_fn(txt);
            txt.flush();
            x_title.clear();
            x_content   = String("<PRE>" + txt.bytes() + "</PRE>").qBytes();
            dst         = do_expand(default_page().qBytes(), m_getters);
            return ContentType::html;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(TextWriter&))
{
    return Page::Writer(new TextPage(hOp, path, fn));
}
