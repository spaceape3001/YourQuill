////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/preamble.hpp>
//#include <functional>

namespace yq::doodler {
    

    /*! \brief A camera
    */
    class Project {
    public:
        Project();
        ~Project();
    
        const std::string&  title() const { return m_title; }
        void                set_title(const std::string&);
        
        uint64_t            revision() const;
        void                bump();
        
        DObject*            object(D);
        const DObject*      object(D) const;
        
        class Xml;
        
    private:
        friend class DObject;
        
        Project(Project&&) = delete;
        Project(const Project&) = delete;
        Project& operator=(const Project&) = delete;
        Project& operator=(Project&&) = delete;

        std::string                 m_title;
        std::vector<DObject*>       m_objects;
        std::atomic<uint64_t>       m_revision;
        
        D       _insert(DObject*);
    };
}

