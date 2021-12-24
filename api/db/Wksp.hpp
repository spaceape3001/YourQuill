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
#include <util/QDirVectorFwd.hpp>
#include <filesystem>

class QSqlDatabase;
class QDateTime;
struct Copyright;

#ifdef WIN32
using RootPathMap   = Map<std::filesystem::path, const Root*, IgCase>;
#else
using RootPathMap   = Map<std::filesystem::path, const Root*>;
#endif


/*! Global workspace  
*/
namespace wksp {
    using fspath = std::filesystem::path;
    using PathVector    = Vector<fspath>;

    static constexpr const char*    szQuillFile = ".yquill";
    static constexpr const char*    szConfigDir = ".config";

    enum {
        SEARCH          = 0x1,  //  Search upward for quill fragment
        TEMPLATES_RO    = 0x2,  //  Templates are read only
        INIT_LOG        = 0x4,
        BAIL_PID        = 0x8,
        AUX_INIT        = 0x10  // Signals that we initialize from AUX, if active/possible (TODO)
    };
    
    enum App : uint8_t {
        ANY = 0,
        BROKER,
        UPDATER,
        SERVER
    };
    

    //YQ_ENUM(Option, ,
        //SEARCH,         // Search upward for the quill fragment 
        //BAIL_PID,       // Checks for pid fragment
        //RO_TEMPLATE     // Templates are read_only
    //)
    //using Options = Flag<Option>;
    
    /*! \brief Abbreviation for this workspace 
    */
    const String&                   abbreviation();
    
    
    App                             app();
    
    /*! \brief Author for this workspace 
    */
    const String&                   author();
    
    /*! \brief Set of "other" ports
    
        Given the chaining capability of the quill roots, this lists "other" ports to check to see if the
        server is active.
    */
    const Set<uint16_t>&            aux_ports();
    
    /*! \brief Searches for the "best" .quill fragment 
    */
    std::filesystem::path           best_guess(const std::filesystem::path& dirOrFragment);
    
    //const std::filesystme::path&    broker_ipc();
    
    const std::filesystem::path&    build_path();

    QSqlDatabase                    cache();
    
    const std::filesystem::path&    cache_db();
    
    
    
    //Folder                  config_folder();
    
    //QString                 config_key(Document);
    //QString                 config_key(const QString&);
    
    const Copyright&                copyright();
    
    const std::filesystem::path&    db_pid();

    const std::filesystem::path&    dot();

    const std::filesystem::path&    git();

    bool                            has_init();

    const String&                   home();
    
    const String&                   host();

    const std::filesystem::path&    ini();

        // an EMPTY path initializes us to NO workspace, all workspace-specific items will be blank
    bool                            initialize(const std::filesystem::path&, unsigned int opts=TEMPLATES_RO, App app={});
    
    const String&                   local_user();
    
    const std::filesystem::path&    log_dir();
    
    const std::filesystem::path&    markdown();
    
    const String&                   name();
    
    const std::filesystem::path&    perl();
    
    unsigned short                  port();
    
        // argument that initialized the worspace
    const std::filesystem::path&    quill_arg();
    
        //  Directory with the quill file
    const std::filesystem::path&    quill_dir();

        //  Quill file itself
    const std::filesystem::path&    quill_file();
    
    std::filesystem::path           quill_fragment_for_dir(const std::filesystem::path&);

    const String&                   quill_key();
    
    std::filesystem::path           quill_resolve(const char*);
    std::filesystem::path           quill_resolve(const QByteArray&);
    std::filesystem::path           quill_resolve(const QString&);
    std::filesystem::path           quill_resolve(const std::string&);
    std::filesystem::path           quill_resolve(const std::string_view&);
    std::filesystem::path           quill_resolve(const std::filesystem::path&);
    
    unsigned int                    read_timeout();
    
    const Root*                     root(uint64_t rootId);
    const Root*                     root(const std::string_view& key);
    
    //const Root*                     root_of(const std::filesystem::path&);
    
    uint64_t                        root_count();
    const Root*                     root_first(DataRole);
    const RoleMap&                  root_firsts();
    const RootPathMap&              root_path_map();
    const RoleVecMap&               root_reads();
    const RootVector&               root_reads(DataRole);
    const RoleVecMap&               root_writes();
    const RootVector&               root_writes(DataRole);
    const RootVector&               roots();
    
    //const std::filesystem::path&    server_ipc();
    const std::filesystem::path&    server_pid();
    
    std::filesystem::path           shared(const char*);
    std::filesystem::path           shared(const QByteArray&);
    std::filesystem::path           shared(const QString&);
    std::filesystem::path           shared(const std::string&);
    std::filesystem::path           shared(const std::string_view&);
    std::filesystem::path           shared(const std::filesystem::path&);
    
    PathVector                      shared_all(const char*);
    PathVector                      shared_all(const QByteArray&);
    PathVector                      shared_all(const QString&);
    PathVector                      shared_all(const std::string&);
    PathVector                      shared_all(const std::string_view&);
    PathVector                      shared_all(const std::filesystem::path&);

    const PathVector&               shared_dirs();
    
    const std::filesystem::path&    smartypants();
    
    const String&                   start();
    std::time_t                     start_time();
    
    
    const std::filesystem::path&    subversion();
    
    //TagPtr                  tag(const QString&);
    //uint64_t                tag_count();
    //Vector<QString>         tag_keys();
    //void                    tag_remove(const QString&);
    //void                    tag_scan();
    //void                    tag_scan(Document);
    //Vector<TagPtr>          tags();
    
    
    //Vector<QString>         template_dir_paths(const QString& templateName);
    const PathVector&               template_dirs();
    PathVector                      template_dirs(const std::string_view& templateName);
    //! Finds the quill associated with the template name
    std::filesystem::path           template_quill(const std::string_view& templateName);
    const StringSet&                templates();
    const StringSet&                templates_available();
    
    const std::filesystem::path&    temp_dir();
    
    //const QString&          temp_dir_default();

    //const QString&          temp_dir_path();
    
    std::filesystem::path           temp_resolve(const char*);
    std::filesystem::path           temp_resolve(const QByteArray&);
    std::filesystem::path           temp_resolve(const QString&);
    std::filesystem::path           temp_resolve(const std::string&);
    std::filesystem::path           temp_resolve(const std::string_view&);
    std::filesystem::path           temp_resolve(const std::filesystem::path&);
    
    const std::filesystem::path&    updater_ipc();
    const std::filesystem::path&    updater_pid();
}
