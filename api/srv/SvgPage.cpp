////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SvgPage.hpp"
#include <QBuffer>
#include <QSvgGenerator>

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

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(QSvgGenerator&))
{
    return Page::Writer(new SvgPage(hOp, path, fn));
}
