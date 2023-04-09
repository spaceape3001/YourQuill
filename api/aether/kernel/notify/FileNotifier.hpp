////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/EnumMap.hpp>
#include <aether/kernel/enum/Change.hpp>
#include <aether/kernel/file/FileSpec.hpp>
#include <aether/kernel/file/Folder.hpp>
#include <basic/Flag.hpp>
#include <filesystem>
#include <source_location>

namespace yq {

    struct Fragment;


    /*! \brief Notifier for files that have changed

        Technically "stage 5", this is a notification that a particular file has changed
    */
    class FileNotifier {
    public:

        static const std::vector<const FileNotifier*>&  all();

        // generic "change" handler
        virtual bool                    change(Fragment,Change) const = 0;

        struct Writer;
        
        std::string_view                description() const { return m_description; }
       
        Flag<Change>                    change() const { return m_change; }
        const std::source_location&     source() const { return m_source; }
        int                             order() const { return m_order; }
        
        const FileSpec&                 spec() const { return m_spec; }
        
        static const EnumMap<Change,Vector<const FileNotifier*>>&     change_map();

    protected:
        FileNotifier(Flag<Change>, const FileSpec&, int order, const std::source_location&);
        ~FileNotifier();

    private:
        FileSpec                m_spec;
        std::string             m_description;
        std::source_location    m_source;
        Flag<Change>            m_change;
        int                     m_order;
        
        struct Repo;
        static Repo&            repo();
    };

    struct FileNotifier::Writer {
        Writer&     description(std::string_view);
        FileNotifier*   importer  = nullptr;
    };

}
