////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObject.hpp>

namespace yq::doodler {
    class LightInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        LightInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! \brief A constraint

        A constraint is something that spaces... could be distance, organization, etc.
    */
    class Light : public DObject {
        YQ_OBJECT_INFO(LightInfo)
        YQ_OBJECT_DECLARE(Light, DObject)
    public:
    
    protected:
        Light(Project&);
        Light(Project&, const Light&);
        ~Light();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Light(const Light&) = delete;
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
