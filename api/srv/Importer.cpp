////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Importer.hpp"

#include <db/FileSys.hpp>
#include <util/EnumMap.hpp>
#include <util/Vector.hpp>

struct Importer::Repo {
    Vector<const Importer*>                 all;
    EnumMap<Change,Vector<const Importer*>> byChange;
};

Importer::Repo& Importer::repo()
{
    static Repo s_repo;
    return s_repo;
}

EnumMap<Change,Vector<const Importer*>>&     Importer::change_map()
{
    return repo().byChange;
}


Vector<const Importer*>  Importer::all()
{
    return repo().all;
}


Importer::Importer(Flag<Change> chg, const QString& expr) : m_pattern(expr), m_change(chg)
{
    int i   = expr.lastIndexOf('/');
    QString f, d;
    
    if(i < 0){
        f   = expr;
    } else {
        d   = expr.mid(0,i);
        f   = expr.mid(i+1);
    }
    
    if(d == "**")
        d   = QString();
    if(!d.isEmpty()){
        m_dir.setPatternOptions( QRegularExpression::CaseInsensitiveOption );
        m_dir.setPattern( QRegularExpression::wildcardToRegularExpression(d));
    }
    if(!f.isEmpty()){
        m_fragment.setPatternOptions( QRegularExpression::CaseInsensitiveOption );
        m_fragment.setPattern( QRegularExpression::wildcardToRegularExpression(f));
    }
}


Importer:: ~Importer()
{
}


void        Importer::initialize() 
{
    repo().all << this;
}

bool        Importer::is_added() const
{
    return m_change.is_set(Change::Added);
}

bool        Importer::is_modified() const
{
    return m_change.is_set(Change::Modified);
}

bool        Importer::is_removed() const
{
    return m_change.is_set(Change::Removed);
}


bool        Importer::matches(const QString&d, const QString& f) const
{
    if(m_dir.isValid() && !m_dir.match(d).hasMatch())
        return false;
    if(m_fragment.isValid() && !m_fragment.match(f).hasMatch())
        return false;
    return true;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Importer::Writer::Writer(Importer* h) : m_handler(h)
{
}

Importer::Writer::Writer(Writer&& w)
{
    m_handler   = w.m_handler;
    w.m_handler = nullptr;
}

Importer::Writer::~Writer()
{
    if(m_handler){
        Repo&   _r  = repo();
        _r.all << m_handler;
        for(Change c : Change::all_values()){
            if(m_handler -> m_change.is_set(c))
                _r.byChange[c] << m_handler;
        }
    }
    
    m_handler   = nullptr;
}

Importer::Writer& Importer::Writer::operator=(Writer&&mv)
{
    if(this != &mv){
        m_handler       = mv.m_handler;
        mv.m_handler    = nullptr;
    }
    return *this;
}

Importer::Writer& Importer::Writer::description(const QByteArray&d)
{
    if(m_handler)
        m_handler -> m_description = d;
    return *this;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {
    struct OnChange_VV : public Importer {
        typedef void (*FN)();
        FN m_fn;
        OnChange_VV(Flag<Change> ch, const QString& expr, FN fn) : Importer(ch, expr), m_fn(fn) {}
        bool        change(Fragment,Change) const override 
        {
            m_fn();
            return true;
        }
    };

    struct OnChange_VF : public Importer {
        typedef void (*FN)(Fragment);
        FN m_fn;
        OnChange_VF(Flag<Change> ch, const QString& expr, FN fn) : Importer(ch, expr), m_fn(fn) {}
        bool        change(Fragment f, Change) const override 
        {
            m_fn(f);
            return true;
        }
    };

    struct OnChange_BF : public Importer {
        typedef bool (*FN)(Fragment);
        FN m_fn;
        OnChange_BF(Flag<Change> ch, const QString& expr, FN fn) : Importer(ch, expr), m_fn(fn) {}
        bool        change(Fragment f, Change) const override 
        {
            return m_fn(f);
        }
    };

    struct OnChange_BFC : public Importer {
        typedef bool (*FN)(Fragment, Change);
        FN m_fn;
        OnChange_BFC(Flag<Change> ch, const QString& expr, FN fn) : Importer(ch, expr), m_fn(fn) {}
        bool        change(Fragment f,Change c) const override 
        {
            return m_fn(f, c);
        }
    };
}



Importer::Writer    on_change( Flag<Change>ch, const QString&expr, void(*fn)())
{
    return Importer::Writer(new OnChange_VV(ch, expr, fn));
}

Importer::Writer    on_change( Flag<Change> ch, const QString&expr, void(*fn)(Fragment))
{
    return Importer::Writer(new OnChange_VF(ch, expr, fn));
}



Importer::Writer    on_change( Flag<Change> ch, const QString& expr, bool(*fn)(Fragment,Change))
{
    return Importer::Writer(new OnChange_BFC(ch, expr, fn));
}

Importer::Writer    on_change( const QString&expr, void(*fn)())
{
    return on_change({Change::Added, Change::Modified, Change::Removed}, expr, fn);
}

Importer::Writer    on_change( const QString&expr, void(*fn)(Fragment))
{
    return on_change({Change::Added, Change::Modified, Change::Removed}, expr, fn);
}

Importer::Writer    on_change( const QString&expr, bool(*fn)(Fragment,Change))
{
    return on_change({Change::Added, Change::Modified, Change::Removed}, expr, fn);
}

Importer::Writer    on_add( const QString& expr, void(*fn)(Fragment))
{
    return Importer::Writer(new OnChange_VF(Change::Added, expr, fn));
}

Importer::Writer    on_add( const QString& expr, bool(*fn)(Fragment))
{
    return Importer::Writer(new OnChange_BF(Change::Added, expr, fn));
}

Importer::Writer    on_add( const QString& expr, bool(*fn)(Fragment,Change))
{
    return Importer::Writer(new OnChange_BFC(Change::Added, expr, fn));
}


Importer::Writer    on_modify( const QString&expr, void(*fn)(Fragment))
{
    return Importer::Writer(new OnChange_VF(Change::Modified, expr, fn));
}

Importer::Writer    on_modify( const QString&expr, bool(*fn)(Fragment))
{
    return Importer::Writer(new OnChange_BF(Change::Modified, expr, fn));
}


Importer::Writer    on_modify( const QString&expr, bool(*fn)(Fragment,Change))
{
    return Importer::Writer(new OnChange_BFC(Change::Modified, expr, fn));
}


Importer::Writer    on_remove( const QString&expr, void (*fn)(Fragment))
{
    return Importer::Writer(new OnChange_VF(Change::Removed, expr, fn));
}

Importer::Writer    on_remove( const QString&expr, bool(*fn)(Fragment))
{
    return Importer::Writer(new OnChange_BF(Change::Removed, expr, fn));
}

Importer::Writer    on_remove( const QString&expr, bool(*fn)(Fragment, Change))
{
    return Importer::Writer(new OnChange_BFC(Change::Removed, expr, fn));
}


Importer::Writer    on_startup( const QString&expr, void (*fn)(Fragment))
{
    return Importer::Writer(new OnChange_VF(Change::Startup, expr, fn));
}

Importer::Writer    on_startup( const QString&expr, bool(*fn)(Fragment))
{
    return Importer::Writer(new OnChange_BF(Change::Startup, expr, fn));
}

Importer::Writer    on_startup( const QString&expr, bool(*fn)(Fragment, Change))
{
    return Importer::Writer(new OnChange_BFC(Change::Startup, expr, fn));
}

