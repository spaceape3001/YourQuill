////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Constraint.hpp"
#include "ConstraintInfoWriter.hpp"

YQ_OBJECT_IMPLEMENT(yq::doodler::Constraint)

namespace yq::doodler {
    ConstraintInfo::ConstraintInfo(std::string_view zName, DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::CONSTRAINT);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Constraint::init_info()
    {
        auto w = writer<Constraint>();
        w.description("Constraint");
    }

    Constraint::Constraint(Project&prj) : DObject(prj)
    {
    }
    
    Constraint::Constraint(Project& prj, const Constraint& cp) : DObject(prj, cp)
    {
    }
    
    Constraint::~Constraint()
    {
    }


    void        Constraint::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

