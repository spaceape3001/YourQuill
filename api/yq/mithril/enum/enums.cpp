////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Access.hpp"
#include "AssertDeny.hpp"
#include "AttrKind.hpp"
#include "CalSystem.hpp"
#include "Change.hpp"
#include "DataRole.hpp"
#include "FileType.hpp"
#include "Format.hpp"
#include "Linkage.hpp"
#include "Multiplicity.hpp"
#include "Permission.hpp"
#include "Reset.hpp"
#include "Restriction.hpp"
#include "SizeDesc.hpp"
#include "Submit.hpp"
#include "Vcs.hpp"

#include <yq/meta/Init.hpp>

#include <yq/core/Enumeration.hxx>

//  ENUM TYPES

YQ_ENUM_IMPLEMENT(yq::mithril::Access)
YQ_ENUM_IMPLEMENT(yq::mithril::AssertDeny)
YQ_ENUM_IMPLEMENT(yq::mithril::AttrKind)
YQ_ENUM_IMPLEMENT(yq::mithril::CalSystem)
YQ_ENUM_IMPLEMENT(yq::mithril::Change)
YQ_ENUM_IMPLEMENT(yq::mithril::DataRole)
YQ_ENUM_IMPLEMENT(yq::mithril::FileType)
YQ_ENUM_IMPLEMENT(yq::mithril::Format)
YQ_ENUM_IMPLEMENT(yq::mithril::Linkage)
YQ_ENUM_IMPLEMENT(yq::mithril::Multiplicity)
YQ_ENUM_IMPLEMENT(yq::mithril::Permission)
YQ_ENUM_IMPLEMENT(yq::mithril::Reset)
YQ_ENUM_IMPLEMENT(yq::mithril::Restriction)
YQ_ENUM_IMPLEMENT(yq::mithril::SizeDesc)
YQ_ENUM_IMPLEMENT(yq::mithril::Submit)
YQ_ENUM_IMPLEMENT(yq::mithril::Vcs)

YQ_TYPE_IMPLEMENT(yq::mithril::Access)
YQ_TYPE_IMPLEMENT(yq::mithril::AssertDeny)
YQ_TYPE_IMPLEMENT(yq::mithril::AttrKind)
YQ_TYPE_IMPLEMENT(yq::mithril::CalSystem)
YQ_TYPE_IMPLEMENT(yq::mithril::Change)
YQ_TYPE_IMPLEMENT(yq::mithril::DataRole)
YQ_TYPE_IMPLEMENT(yq::mithril::FileType)
YQ_TYPE_IMPLEMENT(yq::mithril::Format)
YQ_TYPE_IMPLEMENT(yq::mithril::Linkage)
YQ_TYPE_IMPLEMENT(yq::mithril::Multiplicity)
YQ_TYPE_IMPLEMENT(yq::mithril::Permission)
YQ_TYPE_IMPLEMENT(yq::mithril::Permissions)
YQ_TYPE_IMPLEMENT(yq::mithril::Reset)
YQ_TYPE_IMPLEMENT(yq::mithril::Restriction)
YQ_TYPE_IMPLEMENT(yq::mithril::SizeDesc)
YQ_TYPE_IMPLEMENT(yq::mithril::Submit)
YQ_TYPE_IMPLEMENT(yq::mithril::Vcs)

namespace yq::mithril {
    namespace {
        void reg_enumerations()
        {
            {
                auto& ew = enumeration<Access>(WRITE);
                ew.pretty(Access::NoAccess, "No Access");
                ew.pretty(Access::ReadOnly, "Read Only");
                ew.pretty(Access::ReadWrite, "Read/Write");
                ew.pretty(Access::WriteFirst, "Write First");
            }
        }
        
        YQ_INVOKE(reg_enumerations();)
    }
}


