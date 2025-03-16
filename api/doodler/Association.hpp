////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObject.hpp>

namespace yq::doodler {
    class AssociationInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        AssociationInfo(std::string_view zName, DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! An association
    
        An association is basically notes/link to something else.
    */
    class Association : public DObject {
        YQ_OBJECT_INFO(AssociationInfo)
        YQ_OBJECT_DECLARE(Association, DObject)
    public:

        static void init_info();
    
    protected:
        Association(Project&);
        Association(Project&, const Association&);
        ~Association();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&);
        
        
    private:
        Association(const Association&) = delete;
        Association(Association&&) = delete;
        Association& operator=(const Association&) = delete;
        Association& operator=(Association&&) = delete;
    };

}
