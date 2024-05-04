////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/DObject.hpp>

namespace yq::doodle {
    class ProjectInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        ProjectInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    
        Project* createP(DObject* parent=nullptr) const;
    };
    

    /*! \brief A camera
    */
    class Project : public DObject {
        YQ_OBJECT_INFO(ProjectInfo)
        YQ_OBJECT_DECLARE(Project, DObject)
    public:
    
        const std::string&  title() const { return m_title; }
        void                set_title(const std::string&);
    
    protected:
        Project(DObject* parent=nullptr);
        Project(const Project&);
        ~Project();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Project(Project&&) = delete;
        Project& operator=(const Project&) = delete;
        Project& operator=(Project&&) = delete;

        std::string                             m_title;
        std::unordered_map<uint64_t, DObject*>  m_objects;
    };

    template <typename T>
    class ProjectInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(ProjectInfo* pInfo) : DObjectInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(ProjectInfo& pInfo) : Writer(&pInfo)
        {
        }
    };

}
