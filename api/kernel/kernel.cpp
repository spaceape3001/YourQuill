////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <kernel/Access.hpp>
#include <kernel/AssertDeny.hpp>
#include <kernel/AttrKind.hpp>
#include <kernel/CalSystem.hpp>
#include <kernel/Change.hpp>
#include <kernel/DataRole.hpp>
#include <kernel/FileType.hpp>
#include <kernel/Format.hpp>
#include <kernel/Linkage.hpp>
#include <kernel/Multiplicity.hpp>
#include <kernel/Permission.hpp>
#include <kernel/Reset.hpp>
#include <kernel/Restriction.hpp>
#include <kernel/SizeDesc.hpp>
#include <kernel/Submit.hpp>
#include <kernel/Vcs.hpp>

#include <basic/meta/Init.hpp>

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

#include "Atom.ipp"
#include "AtomCDB.ipp"
#include "AttributeCDB.ipp"
#include "AttributeDiff.ipp"
#include "CategoryCDB.ipp"
#include "CategoryData.ipp"
#include "CategoryFile.ipp"
#include "Class.ipp"
#include "ClassCDB.ipp"
#include "ClassData.ipp"
#include "ClassFile.ipp"
#include "Field.ipp"
#include "FieldCDB.ipp"
#include "FieldData.ipp"
#include "FieldFile.ipp"
#include "Tag.ipp"
#include "TagCDB.ipp"
#include "TagData.ipp"
#include "TagFile.ipp"
#include "ValueCDB.ipp"
#include "ValueData.ipp"
#include "ValueFile.ipp"

#include "Context.ipp"
#include "Copyright.ipp"
#include "KeyValue.ipp"
#include "Policy.ipp"

#include "DBUtils.ipp"
#include "IDLockImpl.ipp"
#include "SQ.ipp"

#include "DirectoryCDB.ipp"
#include "DocumentCDB.ipp"
#include "FileSpec.ipp"
#include "FolderCDB.ipp"
#include "FragmentCDB.ipp"
#include "QuillData.ipp"
#include "QuillFile.ipp"
#include "RootCDB.ipp"

#include "ImageCDB.ipp"

#include "StdFile.ipp"
#include "StdObject.ipp"

#include "LeafData.ipp"
#include "LeafFile.ipp"
#include "LeafCDB.ipp"

#include "AtomNotifier.ipp"
#include "FileWatch.ipp"
#include "FunctionalNotifier.ipp"
#include "FunctionalStage4.ipp"
#include "FileNotifier.ipp"
#include "Stage2.ipp"
#include "Stage3.ipp"
#include "Stage4.ipp"

#include "rule/Action.ipp"
#include "rule/Constraint.ipp"
#include "rule/Trigger.ipp"
#include "Authentication.ipp"

#include "UserCDB.ipp"
#include "UserData.ipp"
#include "UserFile.ipp"

#include "directories.hpp"

namespace yq {
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
}

