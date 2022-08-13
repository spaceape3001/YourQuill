////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <kernel/enum/Access.hpp>
#include <kernel/enum/AssertDeny.hpp>
#include <kernel/enum/AttrKind.hpp>
#include <kernel/enum/CalSystem.hpp>
#include <kernel/enum/Change.hpp>
#include <kernel/enum/DataRole.hpp>
#include <kernel/enum/FileType.hpp>
#include <kernel/enum/Format.hpp>
#include <kernel/enum/Linkage.hpp>
#include <kernel/enum/Multiplicity.hpp>
#include <kernel/enum/Permission.hpp>
#include <kernel/enum/Reset.hpp>
#include <kernel/enum/Restriction.hpp>
#include <kernel/enum/SizeDesc.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/enum/Submit.hpp>
#include <kernel/enum/Vcs.hpp>

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

#include "atom/AtomCDB.ipp"
#include "atom/Class.ipp"
#include "atom/ClassCDB.ipp"
#include "atom/ClassData.ipp"
#include "atom/ClassFile.ipp"
#include "atom/Field.ipp"
#include "atom/FieldCDB.ipp"
#include "atom/FieldData.ipp"
#include "atom/FieldFile.ipp"
#include "atom/ValueCDB.ipp"
#include "atom/ValueData.ipp"
#include "atom/ValueFile.ipp"
#include "bit/Context.ipp"
#include "bit/Copyright.ipp"
#include "bit/KeyValue.ipp"
#include "bit/Policy.ipp"
#include "db/DBUtils.ipp"
#include "db/IDLockImpl.ipp"
#include "db/SQ.ipp"
#include "file/DirectoryCDB.ipp"
#include "file/DocumentCDB.ipp"
#include "file/FolderCDB.ipp"
#include "file/FragmentCDB.ipp"
#include "file/RootCDB.ipp"
#include "image/ImageCDB.ipp"
#include "io/StdFile.ipp"
#include "io/StdObject.ipp"
#include "leaf/LeafData.ipp"
#include "leaf/LeafFile.ipp"
#include "leaf/LeafCDB.ipp"
#include "org/CategoryCDB.ipp"
#include "org/CategoryData.ipp"
#include "org/CategoryFile.ipp"
#include "org/TagCDB.ipp"
#include "org/TagData.ipp"
#include "org/TagFile.ipp"
#include "wksp/QuillData.ipp"
#include "wksp/QuillFile.ipp"

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
}

