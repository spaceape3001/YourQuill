////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootFwd.hpp"
#include <db/enum/SizeDesc.hpp>
#include <util/Enum.hpp>
#include <util/Flag.hpp>

class QSqlDatabase;
class QDateTime;
struct Copyright;

/*! Global workspace  
*/
namespace wksp {
    static constexpr const char*    szQuillFile = ".yquill";
    static constexpr const char*    szConfigDir = ".config";

    YQ_ENUM(Option, ,
        SEARCH,         // Search upward for the quill fragment 
        BAIL_PID,       // Checks for pid fragment
        RO_TEMPLATE     // Templates are read_only
    )
    using Options = Flag<Option>;
    
    /*! \brief Abbreviation for this workspace 
    */
    const QString&          abbreviation();
    
    /*! \brief Author for this workspace 
    */
    const QString&          author();
    
    /*! \brief Set of "other" ports
    
        Given the chaining capability of the quill roots, this lists "other" ports to check to see if the
        server is active.
    */
    const Set<uint16_t>&    aux_ports();
    
    /*! \brief Searches for the "best" .quill fragment 
    */
    QString                 best_guess(const QString& dirOrFragment);
    
    QSqlDatabase            cache();
    
    const QString&          cache_db();
    
    
    
    //Folder                  config_folder();
    
    //QString                 config_key(Document);
    //QString                 config_key(const QString&);
    
    const Copyright&        copyright();
    
    const QString&          db_pid();

    const QString&          dot();

    const QString&          git();

    bool                    has_init();

    const QString&          home();
    
    const QString&          hostname();

    const QString&          ini();
    
    bool                    initialize(const QString& dirOrFragment, Options opts={ Option::RO_TEMPLATE });
    
    const QString&          local_user();
    
    const QString&          log_dir_path();
    
    const QString&          markdown();
    
    const QString&          name();
    
    const QString&          perl();
    
    unsigned short          port();
    
    const QDir&             quill_dir();
    
    QString                 quill_fragment_for_dir(const QString& dirPath);

    const QString&          quill_key();
    
    const QString&          quill_path();
    
    QString                 quill_resolve(const char*);
    QString                 quill_resolve(const QByteArray&);
    QString                 quill_resolve(const QString&);
    QString                 quill_resolve(const std::string&);
    
    unsigned int            read_timeout();
    
    const Root*             root(uint64_t rootId);
    const Root*             root(const QString& rootPath);
    uint64_t                root_count();
    const Root*             root_first(DataRole);
    const RoleMap&          root_firsts();
    const RootMap&          root_map();
    const RoleVecMap&       root_reads();
    const RootVector&       root_reads(DataRole);
    const RoleVecMap&       root_writes();
    const RootVector&       root_writes(DataRole);
    const RootVector&       roots();
    
    const QString&          smartypants();
    const QString&          start();
    
    QDateTime               start_time();
    
    const QString&          subversion();
    
    //TagPtr                  tag(const QString&);
    //uint64_t                tag_count();
    //Vector<QString>         tag_keys();
    //void                    tag_remove(const QString&);
    //void                    tag_scan();
    //void                    tag_scan(Document);
    //Vector<TagPtr>          tags();
    
    
    Vector<QString>         template_dir_paths(const QString& templateName);
    const QDirVector&       template_dirs();
    QDirVector              template_dirs(const QString& templateName);
    //! Finds the quill associated with the template name
    QString                 template_quill(const QString& templateName);
    const QStringSet&       templates();
    const QStringSet&       templates_available();
    
    const QDir&             temp_dir();
    
    const QString&          temp_dir_default();

    const QString&          temp_dir_path();
    
    QString                 temp_resolve(const char*);
    QString                 temp_resolve(const QByteArray&);
    QString                 temp_resolve(const QString&);
    QString                 temp_resolve(const std::string&);
}
