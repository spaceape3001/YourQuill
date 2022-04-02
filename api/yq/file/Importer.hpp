////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Folder.hpp"
#include <yq/enum/Change.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/type/Flag.hpp>
#include <filesystem>
#include <source_location>

namespace yq {

    struct Fragment;


    /*! \brief Importer for files that have changed

    */
    class Importer {
    public:
    
        enum Trigger {
            NoTrigger,      //!< No trigger, it's a no-op
            ByFile,         //!< Change is rigged to specific file, fragment/change invalid
            ByFolderFile,   //!< Change to folder & file extension
            ByExtension     //!< Change to file extension
        };
    

        static const std::vector<const Importer*>&  all();

        // generic "change" handler
        virtual bool                    change(Fragment,Change) const = 0;

        struct Writer;
        
        const std::filesystem::path&    path() const { return m_path; }
        std::string_view                extension() const { return m_extension; }
        Folder                          folder() const { return m_folder; }
        std::string_view                description() const { return m_description; }
        
        Flag<Change>                    change() const { return m_change; }
        Trigger                         trigger() const { return m_trigger; }
        const std::source_location&     source() const { return m_source; }
        

    protected:
        Importer(Trigger, Flag<Change>, Folder, std::string_view, const std::filesystem::path&, const std::source_location&);
        ~Importer();

    private:
        std::string             m_extension;
        std::filesystem::path   m_path;
        std::string             m_description;
        std::source_location    m_source;
        Folder                  m_folder;
        Trigger                 m_trigger;
        Flag<Change>            m_change;
        
        struct Repo;
        static Repo&            repo();
    };

    struct Importer::Writer {
        Writer&     description(std::string_view);
        Importer*   importer  = nullptr;
    };

#if 0


    Importer::Writer    on_change( Flag<Change>, Folder, std::string_view ext, void(*)());
    Importer::Writer    on_change( Flag<Change>, Folder, std::string_view ext, void(*)(Fragment));
    Importer::Writer    on_change( Flag<Change>, Folder, std::string_view ext, bool(*)(Fragment,Change));
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
#endif
}
