////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/bit/KeyValue.hpp>
#include <yq/file/Strings.hpp>
#include <yq/file/XmlUtils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/type/ByteArray.hpp>

#include "enum_access.hpp"
#include "enum_assert_deny.hpp"
#include "enum_attr_kind.hpp"
#include "enum_cal_system.hpp"
#include "enum_change.hpp"
#include "enum_data_role.hpp"
#include "enum_file_type.hpp"
#include "enum_format.hpp"
#include "enum_linkage.hpp"
#include "enum_multiplicity.hpp"
#include "enum_permission.hpp"
#include "enum_reset.hpp"
#include "enum_restriction.hpp"
#include "enum_size_desc.hpp"
#include "enum_submit.hpp"
#include "enum_vcs.hpp"


#include "action.ipp"
#include "authentication.ipp"

#include "constraint.ipp"
#include "context.ipp"
#include "copyright.ipp"
#include "policy.ipp"
#include "rule.ipp"

#include "trigger.ipp"

//#include "class_data.ipp"
#include "quill_file.ipp"
#include "tag_data.ipp"
#include "tag_file.ipp"
#include "user_data.ipp"
#include "user_file.ipp"

#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::Access)
YQ_TYPE_IMPLEMENT(yq::AssertDeny)
YQ_TYPE_IMPLEMENT(yq::AttrKind)
YQ_TYPE_IMPLEMENT(yq::CalSystem)
YQ_TYPE_IMPLEMENT(yq::Change)
YQ_TYPE_IMPLEMENT(yq::DataRole)
YQ_TYPE_IMPLEMENT(yq::FileType)
YQ_TYPE_IMPLEMENT(yq::Format)
YQ_TYPE_IMPLEMENT(yq::Linkage)
YQ_TYPE_IMPLEMENT(yq::Multiplicity)
YQ_TYPE_IMPLEMENT(yq::Permission)
YQ_TYPE_IMPLEMENT(yq::Permissions)
YQ_TYPE_IMPLEMENT(yq::Reset)
YQ_TYPE_IMPLEMENT(yq::Restriction)
YQ_TYPE_IMPLEMENT(yq::SizeDesc)
YQ_TYPE_IMPLEMENT(yq::Submit)
YQ_TYPE_IMPLEMENT(yq::Vcs)
