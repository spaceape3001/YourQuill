////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/Change.hpp>
#include <util/DelayInit.hpp>
#include <util/Flag.hpp>
#include <util/TypeFwd.hpp>

#include <QRegularExpression>

class Fragment;
class Scanner;


/*! \brief Importer for files that have changed

*/
class Importer : public DelayInit {
public:

    static Vector<const Importer*>  all();

    virtual bool        change(Fragment,Change) const = 0;

    struct Writer;
    
    const QString&      pattern() const { return m_pattern; }
    const QString&      description() const { return m_description; }
    
    bool                matches(const QString&dir, const QString&fragment) const;
    Flag<Change>        change() const { return m_change; }
    bool                is_added() const;
    bool                is_modified() const;
    bool                is_removed() const;

protected:
    Importer(Flag<Change>, const QString&);
    virtual ~Importer();
    virtual void        initialize() override;

private:
    QString             m_pattern;
    QRegularExpression  m_dir, m_fragment;
    QString             m_description;
    Flag<Change>        m_change;
    
    friend class Scanner;
    
    struct Repo;
    static Repo& repo();
};

struct Importer::Writer {
    Writer(const Writer&) = delete;
    Writer& operator=(const Writer&) = delete;
    
    Writer(Writer&&);
    Writer& operator=(Writer&&);
    ~Writer();
    explicit Writer(Importer*);
    
    Writer& description(const QByteArray&);
    Writer& depends(const QString&);            // <-- TODO
    
private:
    Importer*                m_handler;
};


Importer::Writer    on_change( Flag<Change>, const QString&, void(*)());
Importer::Writer    on_change( Flag<Change>, const QString&, void(*)(Fragment));
Importer::Writer    on_change( Flag<Change>, const QString&, bool(*)(Fragment,Change));
Importer::Writer    on_change( const QString&, void(*)());
Importer::Writer    on_change( const QString&, void(*)(Fragment));
Importer::Writer    on_change( const QString&, bool(*)(Fragment,Change));


Importer::Writer    on_add( const QString&, void(*)(Fragment));
Importer::Writer    on_add( const QString&, bool(*)(Fragment));
Importer::Writer    on_add( const QString&, bool(*)(Fragment,Change));

Importer::Writer    on_modify( const QString&, void(*)(Fragment));
Importer::Writer    on_modify( const QString&, bool(*)(Fragment));
Importer::Writer    on_modify( const QString&, bool(*)(Fragment,Change));

Importer::Writer    on_remove( const QString&, void(*)(Fragment));
Importer::Writer    on_remove( const QString&, bool(*)(Fragment));
Importer::Writer    on_remove( const QString&, bool(*)(Fragment,Change));

Importer::Writer    on_startup( const QString&, void(*)(Fragment));
Importer::Writer    on_startup( const QString&, bool(*)(Fragment));
Importer::Writer    on_startup( const QString&, bool(*)(Fragment,Change));
