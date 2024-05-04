////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/DObject.hpp>

namespace yq::doodle {
    class LightInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        LightInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    
        Light* createC(DObject* parent=nullptr) const;
    };
    

    /*! \brief A constraint

        A constraint is something that spaces... could be distance, organization, etc.
    */
    class Light : public DObject {
        YQ_OBJECT_INFO(LightInfo)
        YQ_OBJECT_DECLARE(Light, DObject)
    public:
    
    protected:
        Light(DObject* parent=nullptr);
        Light(const Light&);
        ~Light();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Light(Light&&) = delete;
        Light& operator=(const Light&) = delete;
        Light& operator=(Light&&) = delete;
    };

    template <typename T>
    class LightInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(LightInfo* lInfo) : DObjectInfo::Writer<T>(lInfo)
        {
        }
        
        Writer(LightInfo& lInfo) : Writer(&lInfo)
        {
        }
    };

}
