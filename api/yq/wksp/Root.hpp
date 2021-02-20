////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootFwd.hpp"
#include "yq/bit/Policy.hpp"
#include "yq/enum/Access.hpp"
#include "yq/enum/Vcs.hpp"
#include "yq/util/Flag.hpp"
#include "yq/util/EnumMap.hpp"
#include <QDir>

namespace wksp { struct Init; }

/*! \brief Represents a "root" directory for the workspace, all keys are relative to one of these.
*/
class Root {
    friend struct wksp::Init;
public:



    YQ_ENUM(G, , 
        Complain,
        Writable
    )
    
    //  whether or not the provided string is a directory, or not
    static bool         is_good_dir(const QString& s, Flag<G> f=Flag<G>());
    
    static const Root*  by_key(const QString&);

    //Vector<Directory>   all_directories() const;
    //uint64_t            all_directory_count() const;
    //uint64_t            all_fragment_count() const;
    //Vector<Fragment>    all_fragments() const;

    const QString&      color() const { return m_color; }

    const QDir&         dir() const { return m_dir; }

    QString             key() const { return m_key; }
    
    const QString&      path() const { return m_path; }         

    bool                has_git() const;
    bool                has_subversion() const;

    //  version control
    void                move(const QString&, const QString&);
    void                remove(const QString&);
    void                add(const QString&);
    bool                commit();
    bool                update();


    bool                exists(const char*) const;
    bool                exists(const std::string&) const;
    bool                exists(const QByteArray&) const;
    bool                exists(const QString&) const;
    
    bool                make_path(const char*) const;
    bool                make_path(const std::string&) const;
    bool                make_path(const QByteArray&) const;
    bool                make_path(const QString&) const;

    
    bool                is_readable(DataRole) const;
    bool                is_writable(DataRole) const;
    bool                is_write_first(DataRole) const;
    bool                is_template() const { return m_isTemplate; }

    unsigned short      read_order(DataRole) const;

    QString             resolve(const char*) const;
    QString             resolve(const QByteArray&) const;
    QString             resolve(const QString&   ) const;
    QString             resolve(const std::string&) const;
    
    const QString&      name() const { return m_name; }
    

    Access              policy(DataRole) const;

    unsigned short      write_order(DataRole) const;
    
    uint64_t            id() const { return m_id; }
    uint64_t            depth() const { return m_depth; }
    
private:

    Root(const QString&);
    Root(const QString&, PolicyMap);

    struct Role {
        Access          access;
        unsigned short  readOrder;  // cannot be overriden...
        unsigned short  writeOrder;
        Role() : readOrder(0), writeOrder(0) {}
    };

    Root(const Root&) = delete;
    Root(Root&&) = delete;
    
    ~Root();
    
    Root&    operator=(const Root&) = delete;
    Root&    operator=(Root&&) = delete;
    
    
    //const Root*                dirFor(const QStringList&) const;
    //Root*                      dirFor(const QStringList&, bool fCreate);
    
    //mutable tbb::spin_rw_mutex          m_mutex;
    QString                             m_path;
    QDir                                m_dir;
    //Map<QString,Root*,IgCase>      m_dirMap;
    EnumMap<DataRole, Role>             m_roles;
    bool                                m_isTemplate;
    QString                             m_key;
    QString                             m_name;
    QString                             m_color;
    Flag<Vcs>                           m_vcs;
    uint64_t                            m_id;
    uint64_t                            m_depth;   // used by the workspace during load to shuffle into order
};
