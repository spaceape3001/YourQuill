////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <mithril/kernel/enum/Access.hpp>
#include <mithril/kernel/enum/AssertDeny.hpp>
#include <mithril/kernel/enum/AttrKind.hpp>
#include <mithril/kernel/enum/CalSystem.hpp>
#include <mithril/kernel/enum/Change.hpp>
#include <mithril/kernel/enum/DataRole.hpp>
#include <mithril/kernel/enum/FileType.hpp>
#include <mithril/kernel/enum/Format.hpp>
#include <mithril/kernel/enum/Linkage.hpp>
#include <mithril/kernel/enum/Multiplicity.hpp>
#include <mithril/kernel/enum/Permission.hpp>
#include <mithril/kernel/enum/Reset.hpp>
#include <mithril/kernel/enum/Restriction.hpp>
#include <mithril/kernel/enum/SizeDesc.hpp>
#include <mithril/kernel/enum/Submit.hpp>
#include <mithril/kernel/enum/Vcs.hpp>

#include <meta/Init.hpp>

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

#include "atom/Atom.ipp"
#include "atom/AtomCDB.ipp"
#include "atom/AtomSpec.ipp"
#include "atom/Class.ipp"
#include "atom/ClassCDB.ipp"
#include "atom/ClassData.ipp"
#include "atom/ClassFile.ipp"
#include "atom/Field.ipp"
#include "atom/FieldCDB.ipp"
#include "atom/FieldData.ipp"
#include "atom/FieldFile.ipp"
#include "atom/Property.hpp"
#include "atom/ValueCDB.ipp"
#include "atom/ValueData.ipp"
#include "atom/ValueFile.ipp"
#include "attr/AttributeCDB.ipp"
#include "attr/AttributeDiff.ipp"
#include "bit/Context.ipp"
#include "bit/Copyright.ipp"
#include "bit/KeyValue.ipp"
#include "bit/Policy.ipp"
#include "db/DBUtils.ipp"
#include "db/IDLockImpl.ipp"
#include "file/DirectoryCDB.ipp"
#include "file/DocumentCDB.ipp"
#include "file/FileSpec.ipp"
#include "file/FolderCDB.ipp"
#include "file/FragmentCDB.ipp"
#include "file/RootCDB.ipp"
#include "image/ImageCDB.ipp"
#include "io/StdFile.ipp"
#include "io/StdObject.ipp"
#include "leaf/LeafData.ipp"
#include "leaf/LeafFile.ipp"
#include "leaf/LeafCDB.ipp"
#include "notify/AtomNotifier.ipp"
#include "notify/FileWatch.ipp"
#include "notify/FunctionalNotifier.ipp"
#include "notify/FunctionalStage4.ipp"
#include "notify/FileNotifier.ipp"
#include "notify/Stage2.ipp"
#include "notify/Stage3.ipp"
#include "notify/Stage4.ipp"
#include "org/CategoryCDB.ipp"
#include "org/CategoryData.ipp"
#include "org/CategoryFile.ipp"
#include "org/Tag.ipp"
#include "org/TagCDB.ipp"
#include "org/TagData.ipp"
#include "org/TagFile.ipp"
#include "plot/CharacterCDB.ipp"
#include "rule/Action.ipp"
#include "rule/Constraint.ipp"
#include "rule/Trigger.ipp"
#include "user/Authentication.ipp"
#include "wksp/CacheQuery.ipp"
#include "wksp/CacheStatement.ipp"
#include "wksp/QuillData.ipp"
#include "wksp/QuillFile.ipp"
#include "user/UserCDB.ipp"
#include "user/UserData.ipp"
#include "user/UserFile.ipp"

#include "directories.hpp"

YQ_TYPE_IMPLEMENT(yq::mithril::Atom)
YQ_TYPE_IMPLEMENT(yq::mithril::Attribute)
YQ_TYPE_IMPLEMENT(yq::mithril::Category)
YQ_TYPE_IMPLEMENT(yq::mithril::Class)
YQ_TYPE_IMPLEMENT(yq::mithril::Directory)
YQ_TYPE_IMPLEMENT(yq::mithril::Document)
//YQ_TYPE_IMPLEMENT(yq::mithril::Edge)
YQ_TYPE_IMPLEMENT(yq::mithril::Field)
YQ_TYPE_IMPLEMENT(yq::mithril::Folder)
YQ_TYPE_IMPLEMENT(yq::mithril::Fragment)
YQ_TYPE_IMPLEMENT(yq::mithril::Image)
YQ_TYPE_IMPLEMENT(yq::mithril::Leaf)
YQ_TYPE_IMPLEMENT(yq::mithril::Property)
YQ_TYPE_IMPLEMENT(yq::mithril::Tag)
YQ_TYPE_IMPLEMENT(yq::mithril::User)
YQ_TYPE_IMPLEMENT(yq::mithril::Value)

namespace yq::mithril {
    namespace kernel {
        const char*     build_root()
        {
            return YQ_BUILD_ROOT;
        }
    
        const char*     share_directories()
        {
            return YQ_SHARE_DIRS;
        }
    }
    
    template class IDLock<Atom>;
    template class IDLock<Category>;
    template class IDLock<Class>;
    template class IDLock<Directory>;
    template class IDLock<Document>;
    template class IDLock<Field>;
    template class IDLock<Folder>;
    template class IDLock<Fragment>;
    template class IDLock<Leaf>;
    template class IDLock<Tag>;
    template class IDLock<User>;
    
    namespace {
    }
}
