////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/DObject.hpp>

namespace yq::doodle {
    class AssociationInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        AssociationInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    
        Association* createA(DObject* parent=nullptr) const;
    };
    

    /*! An association
    
        An association is basically notes/link to something else.
    */
    class Association : public DObject {
        YQ_OBJECT_INFO(AssociationInfo)
        YQ_OBJECT_DECLARE(Association, DObject)
    public:
    
    protected:
        Association(DObject* parent=nullptr);
        Association(const Association&);
        ~Association();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Association(Association&&) = delete;
        Association& operator=(const Association&) = delete;
        Association& operator=(Association&&) = delete;
    };

    template <typename T>
    class AssociationInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(AssociationInfo* aInfo) : DObjectInfo::Writer<T>(aInfo)
        {
        }
        
        Writer(AssociationInfo& aInfo) : Writer(&aInfo)
        {
        }
    };

}
