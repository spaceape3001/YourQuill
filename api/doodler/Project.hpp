////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/preamble.hpp>
//#include <functional>

namespace yq::doodler {
    
    enum class DFormat : uint8_t {
        AUTO,
        XML
    };
    
    struct Census {
        std::string     title;
    };


    /*! \brief A camera
    */
    class Project {
    public:
        Project();
        ~Project();
    
        const std::string&              title() const { return m_title; }
        void                            set_title(const std::string&);
        
        uint64_t                        revision() const;
        void                            bump();
        
        const std::filesystem::path&   file() const { return m_file; }
        
        DObject*            object(D);
        const DObject*      object(D) const;
        
        static std::unique_ptr<Project>    load(const std::filesystem::path&, DFormat fmt=DFormat::AUTO);
        
        static Expect<Census>               census(const std::filesystem::path&, DFormat fmt=DFormat::AUTO);
        
        std::error_code     save() const;
        
        std::error_code     save_as(const std::filesystem::path&, DFormat fmt=DFormat::AUTO);
        std::error_code     save_to(const std::filesystem::path&, DFormat fmt=DFormat::AUTO) const;
        
    protected:
    
        static std::error_code  census_xml(Census&, const std::filesystem::path&);

        std::error_code     load_impl(const std::filesystem::path&, DFormat fmt=DFormat::AUTO);
        std::error_code     load_xml(const std::filesystem::path&);
        std::error_code     save_xml(const std::filesystem::path&) const;
        
    private:
        friend class DObject;
        
        Project(Project&&) = delete;
        Project(const Project&) = delete;
        Project& operator=(const Project&) = delete;
        Project& operator=(Project&&) = delete;

        std::string                 m_title;
        std::vector<DObject*>       m_objects;
        std::atomic<uint64_t>       m_revision;
        std::filesystem::path       m_file;
        DFormat                     m_format    = DFormat::AUTO;
        
        D       _insert(DObject*);
    };
}

