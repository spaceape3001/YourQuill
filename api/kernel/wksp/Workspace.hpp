////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/file/RootFwd.hpp>
#include <basic/Enum.hpp>
#include <basic/Flag.hpp>
#include <functional>

namespace yq {
    class SqlLite;
    struct Copyright;

    #ifdef WIN32
        using root_path_map_t   = Map<std::filesystem::path, const Root*, IgCase>;
    #else
        using root_path_map_t   = Map<std::filesystem::path, const Root*>;
    #endif

    struct Oracle {
        enum Type : uint8_t {
            NONE    = 0,    //!< No data source configured
            DB,             //!< Use local cache database 
            CURL            //!< Use curl (remote)
        };
        
        Type    type = NONE;
    };

    namespace wksp {
        static constexpr const char*    szQuillFile = ".yquill";
        static constexpr const char*    szConfigDir = ".config";
    
        typedef int (*FNDbFlags)();
    
        //! Options
        enum {
            SEARCH          = 0x1,  //!<  Search upward for quill fragment
            TEMPLATES_RO    = 0x2,  //!<  Templates are read only
            INIT_LOG        = 0x4,  //!<  Initialize log
            AUX_INIT        = 0x8   //!<  Signals that we initialize from AUX/PORTS, if active/possible (TODO)
        };
        
        static constexpr const unsigned kStdOpts    = SEARCH | TEMPLATES_RO | INIT_LOG | AUX_INIT;

        //! General type of the application, may be added to (later)
        enum App : uint8_t {
            ANY = 0,
            BROKER,
            UPDATER,
            SERVER
        };
        
        struct Config {
            std::filesystem::path               spec;
            FNDbFlags                           db_flags    = nullptr;
            unsigned int                        options     = kStdOpts;
            App                                 app         = App::ANY;
            std::vector<std::filesystem::path>  share_dirs;
        };
        
            // an EMPTY path initializes us to NO workspace, all workspace-specific items will be blank
        bool                    initialize(const Config&);

        /*! \brief Searches for the "best" .quill fragment 
        
            Best is either this file (if it's .quill) or the first directory upward containing ".quill"
        */
        std::filesystem::path   best_guess(const std::filesystem::path& dirOrFragment);

            //! TRUE if initialize was called successfully
        bool                            has_init();

        void                            set_db_init();

        //  ================================================================
        //     INFORMATION & QUERY -- valid AFTER successful initialize()
        //  ================================================================
        
        //! Abbreviation for this workspace
        std::string_view                abbreviation();
        
        //! Author(s) for this workspace
        std::string_view                author();
        
        //! Application declaration (above)
        App                             app_type();
        
        /*! \brief Set of "other" ports
        
            Given the chaining capability of the quill roots, this lists "other" ports to check to see if the
            server is active.
        */
        const Set<uint16_t>&            aux_ports();


        //! Location for the workspace's cache file
        const std::filesystem::path&    cache();
        
        bool                            can_cdb();

        //! Copyright information
        const Copyright&                copyright();
        
        const Oracle&                   oracle();
        
        //! This connection is READ_ONLY!
        SqlLite&                        db();
        
        
        //! Location for the DOT executable
        const std::filesystem::path&    dot();

        //! Location for the GIT executable
        const std::filesystem::path&    git();


        //! Location for the "home" of the project
        std::string_view                home();
        
        //! Host we're on
        std::string_view                host();

        //! Local user
        std::string_view                local_user();
        
        //! Log directory
        const std::filesystem::path&    log_dir();
        
        //! Markdown loaction
        const std::filesystem::path&    markdown();
        
        //! Project name
        std::string_view                name();
        
        //! Perl executable location
        const std::filesystem::path&    perl();
        
        //! Port number
        uint16_t                        port();
        
        //! Initializaiton argument
        const std::filesystem::path&    quill_arg();

        //! Quill directory (used)
        const std::filesystem::path&    quill_dir();

        //! Quill file location (used)
        const std::filesystem::path&    quill_file();

        //! Quill "key" (ie, short)
        std::string_view                quill_key();


        std::filesystem::path           quill_resolve(const std::string_view&);
        std::filesystem::path           quill_resolve(const std::filesystem::path&);


        //! Any read timeout metric
        unsigned int                    read_timeout();
        
        //! Resolves across all the roots
        path_vector_t                   resolve_all(const std::string_view&, bool templatesOnly=false);
        
        //! Root by ID....
        const Root*                     root(uint64_t);
        const Root*                     root(std::string_view);
        const Root*                     root(const std::filesystem::path&);
        uint64_t                        root_count();
        const Root*                     root_first(DataRole);
        const root_role_map_t&          root_firsts();
        const root_path_map_t&          root_path_map();
        const root_role_vec_map_t&      root_reads();
        const root_vector_t&            root_reads(DataRole);
        const root_role_vec_map_t&      root_writes();
        const root_vector_t&            root_writes(DataRole);
        const root_vector_t&            roots();

        std::filesystem::path           shared(const std::string_view&);
        std::filesystem::path           shared(const std::filesystem::path&);

        path_vector_t                   shared_all(const std::string_view&);
        path_vector_t                   shared_all(const std::filesystem::path&);

        //! Shared directory path
        const path_vector_t&            shared_dirs();

        //! Smartypants location
        const std::filesystem::path&    smartypants();
        
        //! Formatted text for start time (local convention)
        std::string_view                start();
        //! "KIND" version for files
        std::string_view                start_file();
        
        //! Start timestamp
        std::time_t                     start_time();
        
        //! Subversion executable path
        const std::filesystem::path&    subversion();
        
        const std::filesystem::path&    temp_dir();

        std::filesystem::path           temp_resolve(const std::string_view&);
        std::filesystem::path           temp_resolve(const std::filesystem::path&);

        
        const path_vector_t&            template_dirs();
        path_vector_t                   template_dirs(const std::string_view& templateName);
        
        //! Finds the quill associated with the template name
        std::filesystem::path           template_quill(const std::string_view& templateName);

        //! \brief Get templates
        //!
        //! \param[in] all  Flag to get all available templates, otherwise, only the USED templates
        const string_set_t&             templates(bool all=false);
        
        unsigned int                    threads();
    }
}
