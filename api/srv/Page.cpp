////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ArgStrings.hpp"
#include "Page.hpp"
#include "TLSGlobals.hpp"

#include <db/ShareDir.hpp>
#include <util/EnumMap.hpp>
#include <util/Guarded.hpp>
#include <util/Vector.hpp>

#include <QByteArray>
#include <QUrl>
//#include "yCommon.hpp"


//#include "yPage.hpp"
//#include "yNetWriter.hpp"

//#include "db/ShareDir.hpp"

//#include "util/DelayInit.hpp"
//#include "util/EnumMap.hpp"
//#include "util/Guarded.hpp"
//#include "util/Map.hpp"
//#include "util/Utilities.hpp"
//#include "util/Vector.hpp"

//#include "db/Workspace.hpp"

//#include <QBuffer>
//#include <QDir>
//#include <QImage>
//#include <QJsonArray>
//#include <QJsonObject>
//#include <QJsonDocument>
//#include <QString>
//#include <QSvgGenerator>

#include <tbb/spin_rw_mutex.h>


Redirect::Redirect(const QUrl& b ) : why(HttpStatus::TemporaryRedirect), where(b.toString().toUtf8())
{
}

Redirect::Redirect(HttpStatus a, const QUrl& b ) : why(a), where(b.toString().toUtf8())
{
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
namespace {
    using PageMap =  Map<QByteArray,const Page*,IgCase>;
    
        /*
            We're keeping "dispatchers' separate from pages.....  seems safer
        */

    struct PageRepo {
        Vector<const Page*>                                 all;

        EnumMap<HttpOp,PageMap>         pages;
        GetMap                          getters;
        Guarded<QByteArray>             t_page;             // not locked with the rest
        //Map<String,QDir,IgCase>         dirs;
        EnumMap<HttpOp,PageMap>         dispatchers;
        mutable tbb::spin_rw_mutex      mutex;
        volatile bool                   openReg;
      
        PageRepo() : t_page(shared_bytes("std/page")), openReg(true)
        {
            //  Only hook the dynamic ones here.....
            getters[sz_body] = []()->QByteArray {
                return x_content;
            };
            getters[sz_title] = []()->QByteArray {
                return x_title;
            };
        }
    };
    
    PageRepo&   repo()
    {
        static PageRepo s_repo;
        return s_repo;
    }
};

#define LOCK                                                    \
    const PageRepo& _r = repo();                                \
    tbb::spin_rw_mutex::scoped_lock     _lock;                  \
    if(_r.openReg)                                              \
        _lock.acquire(_r.mutex, false);

#define WLOCK                                                   \
    PageRepo& _r = repo();                                      \
    tbb::spin_rw_mutex::scoped_lock     _lock(_r.mutex, true);

const Vector<const Page*>&   Page::all()
{
    return repo().all;
}

QByteArray      Page::default_page()
{
    return repo().t_page;
}

void            Page::default_page(const QByteArray&v)
{
    repo().t_page   = v;
}

const Page*     Page::find(HttpOp op, const QByteArray& p, bool fDispatcher)
{
    LOCK
    return fDispatcher ? _r.dispatchers[op].get(p, nullptr) : _r.pages[op].get(p, nullptr);
}

void            Page::freeze()
{
    repo().openReg      = false;
}

bool            Page::frozen()
{
    return repo().openReg;
}

FNGet           Page::static_getter(const QByteArray&z)
{
    LOCK
    return _r.getters.get(z, nullptr);
}


const GetMap&   Page::static_getters()
{
    return repo().getters;
}


void    reg_getter(const char*z, FNGet fn)
{
    WLOCK
    if(z && fn && _r.openReg)
        _r.getters[z]   = fn;
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Page::Page(HttpOp h, const String& path, bool fDisp): m_op(h), m_path(path), m_tabbar(nullptr), 
    m_loginReq(false), m_localOnly(false), 
    m_postAnon(false), m_noExpand(false), m_dispatch(fDisp)
{
    WLOCK
    if(_r.openReg){
        _r.all << this;
        if(fDisp)
            _r.dispatchers[h][path.qBytes()]    = this;
        else
            _r.pages[h][path.qBytes()]          = this;
    }
}

Page::~Page()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



Page::Writer& Page::Writer::anonymous()
{
    if(m_page)
        m_page -> m_postAnon = true;
    return *this;
}

Page::Writer& Page::Writer::argument(const QByteArray&k, const QByteArray& d)
{
    if(m_page){
        Arg a;
        a.name  = k;
        a.description   = d;
        m_page -> m_args << a;
    }
    return *this;
}

Page::Writer& Page::Writer::description(const QByteArray&d)
{
    if(m_page)
        m_page -> m_description = d;
    return *this;
}

Page::Writer& Page::Writer::getter(const QByteArray&k, FNGet fn)
{
    if(m_page && fn && !k.isEmpty())
        m_page -> m_getters[k]   = fn;
    return *this;
}

Page::Writer& Page::Writer::icon(const QByteArray&i)
{
    if(m_page)
        m_page -> m_icon = i;
    return *this;
}

Page::Writer& Page::Writer::label(const QByteArray&v)
{
    if(m_page)
        m_page -> m_label   = v;
    return *this;
}


Page::Writer& Page::Writer::login()
{
    if(m_page)
        m_page -> m_loginReq = true;
    return *this;
}

Page::Writer& Page::Writer::local()
{
    if(m_page)
        m_page -> m_localOnly = true;
    return *this;
}


Page::Writer& Page::Writer::no_expand()
{
    if(m_page)
        m_page  -> m_noExpand   = true;
    return *this;
}

Page::Writer& Page::Writer::id()
{
    argument(sz_id, "Numeric identiifer to resource");
    return *this;
}

Page::Writer& Page::Writer::key()
{
    argument(sz_key, "Textual identifier to resource");
    return *this;
}

