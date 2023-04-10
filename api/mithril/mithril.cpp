////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/preamble.hpp>

#include <mithril/enum/Access.hpp>
#include <mithril/enum/AssertDeny.hpp>
#include <mithril/enum/AttrKind.hpp>
#include <mithril/enum/CalSystem.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/enum/DataRole.hpp>
#include <mithril/enum/FileType.hpp>
#include <mithril/enum/Format.hpp>
#include <mithril/enum/Linkage.hpp>
#include <mithril/enum/Multiplicity.hpp>
#include <mithril/enum/Permission.hpp>
#include <mithril/enum/Reset.hpp>
#include <mithril/enum/Restriction.hpp>
#include <mithril/enum/SizeDesc.hpp>
#include <mithril/enum/Submit.hpp>
#include <mithril/enum/Vcs.hpp>

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
#include "atom/AtomArg.ipp"
#include "atom/AtomCDB.ipp"
#include "atom/AtomHtml.ipp"
#include "atom/AtomPost.ipp"
#include "atom/AtomSpec.ipp"

#include "attribute/AttributeArg.ipp"
#include "attribute/AttributeCDB.ipp"
#include "attribute/AttributeHtml.ipp"
#include "attribute/AttributeDiff.ipp"

#include "auth/Authentication.ipp"

#include "bit/Context.ipp"
#include "bit/Copyright.ipp"
#include "bit/KeyValue.ipp"
#include "bit/Policy.ipp"

#include "category/CategoryArg.ipp"
#include "category/CategoryCDB.ipp"
#include "category/CategoryData.ipp"
#include "category/CategoryFile.ipp"
#include "category/CategoryHtml.ipp"
#include "category/CategoryPost.ipp"

#include "character/CharacterCDB.ipp"
#include "character/CharacterHtml.ipp"

#include "class/Class.ipp"
#include "class/ClassArg.ipp"
#include "class/ClassCDB.ipp"
#include "class/ClassData.ipp"
#include "class/ClassFile.ipp"
#include "class/ClassHtml.ipp"
#include "class/ClassPost.ipp"

#include "db/DBUtils.ipp"
#include "db/IDLockImpl.ipp"

#include "directory/DirectoryArg.ipp"
#include "directory/DirectoryCDB.ipp"
#include "directory/DirectoryHtml.ipp"
#include "directory/DirectoryPost.ipp"

#include "document/DocumentArg.ipp"
#include "document/DocumentCDB.ipp"
#include "document/DocumentHtml.ipp"
#include "document/DocumentPost.ipp"

#include "field/Field.ipp"
#include "field/FieldArg.ipp"
#include "field/FieldCDB.ipp"
#include "field/FieldData.ipp"
#include "field/FieldFile.ipp"
#include "field/FieldHtml.ipp"
#include "field/FieldPost.ipp"

//#include "atom/Property.hpp"

#include "file/FileSpec.ipp"

#include "folder/FolderArg.ipp"
#include "folder/FolderCDB.ipp"
#include "folder/FolderHtml.ipp"
#include "folder/FolderPost.ipp"

#include "fragment/FragmentArg.ipp"
#include "fragment/FragmentCDB.ipp"
#include "fragment/FragmentHtml.ipp"
#include "fragment/FragmentPost.ipp"

#include "html/Html.ipp"
#include "html/Post.ipp"

#include "image/ImageArg.ipp"
#include "image/ImageCDB.ipp"
#include "image/ImageHtml.ipp"
#include "image/ImagePost.ipp"

#include "io/StdFile.ipp"
#include "io/StdObject.ipp"

#include "ipc/DirWatcher.ipp"
#include "ipc/ipcPackets.hpp"
#include "ipc/ipcSocket.ipp"

#include "leaf/LeafArg.ipp"
#include "leaf/LeafData.ipp"
#include "leaf/LeafFile.ipp"
#include "leaf/LeafCDB.ipp"
#include "leaf/LeafHtml.ipp"
#include "leaf/LeafPost.ipp"

#include "meta/MetaHtml.ipp"
#include "meta/MetaObjectArg.ipp"
#include "meta/MetaTypeArg.ipp"


#include "notify/AtomNotifier.ipp"
#include "notify/FileWatch.ipp"
#include "notify/FunctionalNotifier.ipp"
#include "notify/FunctionalStage4.ipp"
#include "notify/FileNotifier.ipp"
#include "notify/Stage2.ipp"
#include "notify/Stage3.ipp"
#include "notify/Stage4.ipp"

#include "quill/QuillData.ipp"
#include "quill/QuillFile.ipp"

#include "root/RootArg.ipp"
#include "root/RootCDB.ipp"
#include "root/RootHtml.ipp"
#include "root/RootPost.ipp"

#include "rule/Action.ipp"
#include "rule/Constraint.ipp"
#include "rule/Trigger.ipp"

#include "tag/Tag.ipp"
#include "tag/TagArg.ipp"
#include "tag/TagCDB.ipp"
#include "tag/TagData.ipp"
#include "tag/TagFile.ipp"
#include "tag/TagHtml.ipp"
#include "tag/TagPost.ipp"

#include "user/UserArg.ipp"
#include "user/UserCDB.ipp"
#include "user/UserData.ipp"
#include "user/UserFile.ipp"
#include "user/UserHtml.ipp"
#include "user/UserPost.ipp"

#include "util/Markdown.ipp"

#include "value/ValueArg.ipp"
#include "value/ValueCDB.ipp"
#include "value/ValueData.ipp"
#include "value/ValueFile.ipp"
#include "value/ValueHtml.ipp"


//#include "rule/Action.ipp"
//#include "rule/Constraint.ipp"
//#include "rule/Trigger.ipp"
//#include "user/Authentication.ipp"
#include "wksp/CacheQuery.ipp"
#include "wksp/CacheStatement.ipp"

#include "directories.hpp"

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
}
