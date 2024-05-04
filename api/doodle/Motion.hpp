////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/DObject.hpp>

namespace yq::doodle {
    class MotionInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        MotionInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    
        Motion* createM(DObject* parent=nullptr) const;
    };
    

    /*! \brief A motion

        Motion is dynamic changes to properties.
    */
    class Motion : public DObject {
        YQ_OBJECT_INFO(MotionInfo)
        YQ_OBJECT_DECLARE(Motion, DObject)
    public:
    
    protected:
        Motion(DObject* parent=nullptr);
        Motion(const Motion&);
        ~Motion();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Motion(Motion&&) = delete;
        Motion& operator=(const Motion&) = delete;
        Motion& operator=(Motion&&) = delete;
    };

    template <typename T>
    class MotionInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(MotionInfo* mInfo) : DObjectInfo::Writer<T>(mInfo)
        {
        }
        
        Writer(MotionInfo& mInfo) : Writer(&mInfo)
        {
        }
    };

}
