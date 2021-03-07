////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TextPage.hpp"
#include "TLSGlobals.hpp"

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
        
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            TextWriter    txt;
            m_fn(txt);
            txt.flush();
            x_title.clear();
            x_content   = "<PRE>" + txt.steal() + "</PRE>";
            dst         = do_expand(default_page(), m_getters);
            return ContentType::html;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(TextWriter&))
{
    return Page::Writer(new TextPage(hOp, path, fn));
}
