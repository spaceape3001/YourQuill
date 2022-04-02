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


    /*! \brief Notifier for files that have changed

    */
    class Notifier {
    public:
    
        enum Trigger {
            NoTrigger,      //!< No trigger, it's a no-op
            ByFile,         //!< Change is rigged to specific file, fragment/change invalid
            ByDocument,     //!< Match name
            ByExtension,    //!< Change to file extension
            ByFolderExt,    //!< Change to folder & file extension
            ByFolderDoc,    //!< Change to folder & doc
            
            SpecName,       //!< Signal to the constructor
            SpecFolderName  //!< Signal to the constructor
        };
    

        static const std::vector<const Notifier*>&  all();

        // generic "change" handler
        virtual bool                    change(Fragment,Change) const = 0;

        struct Writer;
        
        const std::filesystem::path&    path() const { return m_path; }
        std::string_view                specific() const { return m_specific; }
        Folder                          folder() const { return m_folder; }
        std::string_view                description() const { return m_description; }
        
        Flag<Change>                    change() const { return m_change; }
        Trigger                         trigger() const { return m_trigger; }
        const std::source_location&     source() const { return m_source; }
        
        static const EnumMap<Change,Vector<const Notifier*>>&     change_map();

    protected:
        Notifier(Trigger, Flag<Change>, Folder, std::string_view, const std::filesystem::path&, const std::source_location&);
        ~Notifier();

    private:
        std::string             m_specific;
        std::filesystem::path   m_path;
        std::string             m_description;
        std::source_location    m_source;
        Folder                  m_folder;
        Trigger                 m_trigger;
        Flag<Change>            m_change;
        
        struct Repo;
        static Repo&            repo();
    };

    struct Notifier::Writer {
        Writer&     description(std::string_view);
        Notifier*   importer  = nullptr;
    };

}
