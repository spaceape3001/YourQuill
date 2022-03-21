////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    struct ImageArgPage : public Page {
        typedef void (*FN)(QImage&);
        FN m_fn;
        
        ImageArgPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }

        ContentType    handle(String& dst, const String&) const override
        {
            QImage    img;
            m_fn(img);
            //dst         = make_png(img);
            return ContentType::png; 
        }
    };
}


Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(QImage&))
{
    return Page::Writer(new ImageArgPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct ImageRetPage : public Page {
        typedef QImage (*FN)();
        FN m_fn;
        
        ImageRetPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }

        ContentType    handle(String& dst, const String&) const override
        {
            QImage    img;
            //dst         = make_png(m_fn());
            return ContentType::png; 
        }
    };
}


Page::Writer     reg_page(HttpOp hOp, const String& path, QImage (*fn)())
{
    return Page::Writer(new ImageRetPage(hOp, path, fn));
}

