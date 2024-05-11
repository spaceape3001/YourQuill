////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObject.hpp>

namespace yq::doodler {
    class ConstraintInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        ConstraintInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! \brief A constraint

        A constraint is something that spaces... could be distance, organization, etc.
    */
    class Constraint : public DObject {
        YQ_OBJECT_INFO(ConstraintInfo)
        YQ_OBJECT_DECLARE(Constraint, DObject)
    public:
    
    protected:
        Constraint(Project&);
        Constraint(Project&, const Constraint&);
        ~Constraint();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Constraint(Constraint&&) = delete;
        Constraint(const Constraint&) = delete;
        Constraint& operator=(const Constraint&) = delete;
        Constraint& operator=(Constraint&&) = delete;
    };

    template <typename T>
    class ConstraintInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(ConstraintInfo* cInfo) : DObjectInfo::Writer<T>(cInfo)
        {
        }
        
        Writer(ConstraintInfo& cInfo) : Writer(&cInfo)
        {
        }
    };

}
