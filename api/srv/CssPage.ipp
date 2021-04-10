////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

CssWriter::CssWriter() = default;
CssWriter::~CssWriter() = default;

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct CssPage : public Page {
        typedef void (*FN)(CssWriter&);
        FN          m_fn;
        ContentType    handle(QByteArray& dst, const QByteArray&) const override
        {
            CssWriter css;
            m_fn(css);
            css.flush();
            dst = css.steal();
            return ContentType::css;;
        }
        
        CssPage(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
    };
}

Page::Writer        reg_page(HttpOp op, const String&path, void (*fn)(CssWriter&))
{
    return Page::Writer{new CssPage(op, path, fn)};
}

