////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    struct JsonArrPage : public Page {
        typedef void (*FN)(QJsonArray&);
        FN m_fn;
        JsonArrPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }

        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            QJsonArray      arr;
            m_fn(arr);
            doc.setArray(arr);
            dst   = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(QJsonArray&))
{
    return Page::Writer(new JsonArrPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonArrRetPage : public Page {
        typedef QJsonArray (*FN)();
        FN m_fn;
        JsonArrRetPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }

        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            doc.setArray(m_fn());
            dst   = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, QJsonArray(*fn)())
{
    return Page::Writer(new JsonArrRetPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonDocPage : public Page {
        typedef void (*FN)(QJsonDocument&);
        FN m_fn;
        JsonDocPage(HttpOp hOp,  const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            m_fn(doc);
            dst   = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(QJsonDocument&))
{
    return Page::Writer(new JsonDocPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonDocRetPage : public Page {
        typedef QJsonDocument (*FN)();
        FN m_fn;
        JsonDocRetPage(HttpOp hOp,  const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray& dst, const QByteArray&) const override
        {
            dst   = (m_fn()).toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, QJsonDocument(*fn)())
{
    return Page::Writer(new JsonDocRetPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonObjPage : public Page {
        typedef void (*FN)(QJsonObject&);
        FN m_fn;
        JsonObjPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray&dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            QJsonObject     obj;
            m_fn(obj);
            doc.setObject(obj);
            dst = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, void(*fn)(QJsonObject&))
{
    return Page::Writer(new JsonObjPage(hOp, path, fn));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct JsonObjRetPage : public Page {
        typedef QJsonObject (*FN)();
        FN m_fn;
        JsonObjRetPage(HttpOp hOp, const String& path, FN fn) : Page(hOp, path, false), m_fn(fn)
        {
        }
        
        ContentType handle(QByteArray&dst, const QByteArray&) const override
        {
            QJsonDocument   doc;
            doc.setObject(m_fn());
            dst = doc.toJson(QJsonDocument::Compact);
            return ContentType::json;
        }
    };
}

Page::Writer     reg_page(HttpOp hOp, const String& path, QJsonObject(*fn)())
{
    return Page::Writer(new JsonObjRetPage(hOp, path, fn));
}
