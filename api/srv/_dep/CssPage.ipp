////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    CssWriter::CssWriter(Stream& str) : NetWriter(str)
    {
    }

    CssWriter::~CssWriter() = default;

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    namespace {
        struct CssPage : public Page {
            typedef void (*FN)(CssWriter&);
            FN          m_fn;
            ContentType    handle(Stream& str) const override
            {
                CssWriter css(str);
                m_fn(css);
                return ContentType::css;
            }
            
            CssPage(HttpOp op, const String& path, FN fn) : Page(op, path, false), m_fn(fn) {}
        };
    }

    Page::Writer        reg_page(HttpOp op, const String&path, void (*fn)(CssWriter&))
    {
        return Page::Writer{new CssPage(op, path, fn)};
    }

}
