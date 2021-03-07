////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "util/Http.hpp"
#include <QByteArray>

struct Session;
struct Request;


class Markdown;
class Html;
class Css;
class Text;

class QDateTime;
class QDir;
class QHostAddress;
class QImage;
class QJsonArray;
class QJsonDocument;
class QJsonObject;
class QSvgGenerator;
class QUrl;


struct Redirect {
    HttpStatus              why;
    QByteArray              where;
    
    Redirect(const QByteArray& b ) : why(HttpStatus::TemporaryRedirect), where(b) {}
    Redirect(const QUrl& b );
    Redirect(HttpStatus a, const QByteArray& b ) : why(a), where(b) {}
    Redirect(HttpStatus a, const QUrl&);
};


namespace html { struct TabBar; }

typedef QByteArray (*FNGet)();
using GetMap    = Map<String,FNGet,IgCase>;


class Page {
public:
    struct Reply;
    struct Writer;
    struct Arg;
    struct Request;
    struct Session;
    
    
    static const Vector<const Page*>&   all();
    //static void                         def_directory(const char*, const QDir&);
    //static void                         def_getter(const char*, FNGet);
    //static void                         def_page(const QByteArray&);
    //static ContentType                  direct_file(QByteArray& dst, const QString&file, bool fExpand=true);
    //static ContentType                  direct_content(QByteArray&, const QByteArray&, ContentType, const QByteArray&title=QByteArray());
    //static Reply                        dispatch();
    //static QByteArray                   expand(const QByteArray&, const GetMap& args=GetMap());
    //static const Page*                  find(HttpOp, const String&);
    //static const GetMap&                static_getters();
    
    
    const String&                       path() const { return m_path; }
    HttpOp                              httpOp() const { return m_op; }
    
    const Vector<Arg>&                  arguments() const { return m_args; }
    const QByteArray&                   description() const { return m_description; }
    const QByteArray&                   label() const { return m_label; }
    bool                                local_only() const { return m_localOnly; }
    bool                                login_required() const { return m_loginReq; }
    bool                                post_anon() const { return m_postAnon; }
    const QByteArray&                   icon() const { return m_icon; }
    const html::TabBar*                 tabbar() const { return m_tabbar; }
    bool                                no_expand() const { return m_noExpand; }
    bool                                is_dispatcher() const { return m_dispatch; }
    
    virtual ContentType                 handle(QByteArray&, const QByteArray&) const = 0;

    //FNGet                               getter(const QByteArray&,bool incDef=true) const;
    
    //  Warning duplicates can be had here!
    //Vector<String>                      getters(bool incDef=true) const;
   
   
    /*
        PATH syntax:
        
        *       any-file (excluding periods)
        **      any-depth wildcard, to any directory
        *.*     all files
        *.ext   specific etension
        
        NOTE!  This isn't terribly sophisticated.... just trying to get the basics in place.
        
        FULL specification will take precedence over wildcards.
    */
    
    
    
protected:    
    friend class html::TabBar;
    Page(HttpOp, const String&, bool);
    virtual ~Page();
    
    Vector<Arg>                 m_args;
    HttpOp                      m_op;
    String                      m_path;
    QByteArray                  m_description;
    GetMap                      m_getters;
    QByteArray                  m_icon;
    html::TabBar*               m_tabbar;
    QByteArray                  m_label;
    bool                        m_loginReq;
    bool                        m_localOnly;
    bool                        m_postAnon;
    bool                        m_noExpand;
    bool                        m_dispatch;
};



struct Page::Arg {
    QByteArray  name;
    QByteArray  description;
};

struct Page::Reply {
    ContentType type;
    QByteArray  content;
    HttpStatus  status;
};


struct Page::Writer {
    explicit Writer(Page*);
    
    Writer& login();
    Writer& description(const QByteArray&);
    Writer& argument(const QByteArray&, const QByteArray& d=QByteArray());
    Writer& getter(const QByteArray&, FNGet);
    Writer& local();
    Writer& anonymous();
    Writer& icon(const QByteArray&);
    Writer& no_expand();
    Writer& label(const QByteArray&);
    
    Writer& id();
    Writer& key();
    
private:
    friend class html::TabBar;
    Page*                m_page;
};

namespace html {
    class TabBar {
    public:
        TabBar(std::initializer_list<Page::Writer>);
        const Vector<const Page*>&      pages() const { return m_pages; }
    private:
        Vector<const Page*>             m_pages;
    };
}


//Page::Writer     page(HttpOp, const char*, Redirect(*)());
Page::Writer     page(HttpOp, const String&, void (*)());
Page::Writer     page(HttpOp, const String&, HttpStatus (*)());
Page::Writer     page(HttpOp, const String&, ContentType (*)(QByteArray&));
Page::Writer     page(HttpOp, const String&, void (*)(Css&));
Page::Writer     page(HttpOp, const String&, void (*)(Html&));
Page::Writer     page(HttpOp, const String&, void (*)(Markdown&));
Page::Writer     page(HttpOp, const String&, void (*)(Text&));
Page::Writer     page(HttpOp, const String&, void (*)(QImage&));
Page::Writer     page(HttpOp, const String&, void (*)(QJsonArray&));
Page::Writer     page(HttpOp, const String&, QJsonArray (*)());
Page::Writer     page(HttpOp, const String&, void (*)(QJsonDocument&));
Page::Writer     page(HttpOp, const String&, QJsonDocument (*)());
Page::Writer     page(HttpOp, const String&, void (*)(QJsonObject&));
Page::Writer     page(HttpOp, const String&, QJsonObject (*)());
Page::Writer     page(HttpOp, const String&, void (*)(QSvgGenerator&));

Page::Writer    dispatcher(const String&, const QDir&);
Page::Writer    dispatcher(HttpOp, const String&, const QDir&);
Page::Writer    dispatcher(HttpOp, const String&, void(*)(const String&));
Page::Writer    dispatcher(HttpOp, const String&, void(*)(Html&, const String&));
Page::Writer    dispatcher(HttpOp, const String&, void(*)(Markdown&, const String&));

void             tabbar(std::initializer_list<Page::Writer>);

//void            reg_directory(const char*z, const QDir& d);
void            reg_getter(const char*z, FNGet fn);
void            reg_def_page(const QByteArray& bytes);
QByteArray      def_page();
ContentType     do_direct_content(QByteArray&dst, const QByteArray&data, ContentType ret, const QByteArray&title=QByteArray());
ContentType     do_direct_file(QByteArray& dst, const QString&file, bool fExpand=true);
QByteArray      do_expand(const QByteArray&content, const GetMap&vars = GetMap());
const GetMap&   static_getters();
