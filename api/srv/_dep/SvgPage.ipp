////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


namespace {
    struct SvgPage : public Page {
        typedef void(*FN)(QSvgGenerator&);
        SvgPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn){}

        ContentType    handle(String&dst, const String&) const override
        {
            QByteArray  bytes;
            QBuffer buf(&bytes);
            buf.open(QIODevice::ReadWrite);
            {
                QSvgGenerator   svg;
                svg.setOutputDevice(&buf);
                m_fn(svg);
            }
            buf.close();
            dst = bytes;
            return ContentType::svg;
        }
    private:
        FN  m_fn;
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(QSvgGenerator&))
{
    return Page::Writer(new SvgPage(hOp, path, fn));
}
