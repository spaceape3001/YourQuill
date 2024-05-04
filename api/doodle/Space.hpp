////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/DObject.hpp>

namespace yq::doodle {
    class SpaceInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        SpaceInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! A space 
    
        Defines a space
    */
    class Space : public DObject {
        YQ_OBJECT_INFO(SpaceInfo)
        YQ_OBJECT_DECLARE(Space, DObject)
    public:
    
    protected:
        Space(Project&);
        Space(Project&, const Space&);
        ~Space();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Space(const Space&) = delete;
        Space(Space&&) = delete;
        Space& operator=(const Space&) = delete;
        Space& operator=(Space&&) = delete;
    };

    template <typename T>
    class SpaceInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(SpaceInfo* sInfo) : DObjectInfo::Writer<T>(sInfo)
        {
        }
        
        Writer(SpaceInfo& sInfo) : Writer(&sInfo)
        {
        }
    };

}
