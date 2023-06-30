////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/preamble.hpp>

#include "directories.hpp"

#include "atom/Atom.ipp"
#include "atom/AtomArg.ipp"
#include "atom/AtomCDB.ipp"
#include "atom/AtomHtml.ipp"
#include "atom/AtomPost.ipp"
#include "atom/AtomProvider.ipp"
#include "atom/AtomSpec.ipp"

//#include "atom/Property.hpp"


#include "attribute/AttributeArg.ipp"
#include "attribute/AttributeCDB.ipp"
#include "attribute/AttributeHtml.ipp"
#include "attribute/AttributeDiff.ipp"

#include "auth/Authentication.ipp"

#include "bit/Context.ipp"
#include "bit/Copyright.ipp"
#include "bit/KeyValue.ipp"
#include "bit/Policy.ipp"

//#include "book/BookArg.ipp"
#include "book/BookCDB.ipp"
#include "book/BookHtml.ipp"
//#include "book/BookPost.ipp"
#include "book/BookProvider.ipp"


#include "category/CategoryArg.ipp"
#include "category/CategoryCDB.ipp"
#include "category/CategoryData.ipp"
#include "category/CategoryFile.ipp"
#include "category/CategoryHtml.ipp"
#include "category/CategoryPost.ipp"
#include "category/CategoryProvider.ipp"

//#include "character/CharacterArg.ipp"
#include "character/CharacterCDB.ipp"
#include "character/CharacterHtml.ipp"
//#include "character/CharacterPost.ipp"
#include "character/CharacterProvider.ipp"

#include "class/Class.ipp"
#include "class/ClassArg.ipp"
#include "class/ClassCDB.ipp"
#include "class/ClassData.ipp"
#include "class/ClassFile.ipp"
#include "class/ClassHtml.ipp"
#include "class/ClassPost.ipp"
#include "class/ClassProvider.ipp"

#include "db/DBUtils.ipp"

#include "directory/DirectoryArg.ipp"
#include "directory/DirectoryCDB.ipp"
#include "directory/DirectoryHtml.ipp"
#include "directory/DirectoryPost.ipp"
#include "directory/DirectoryProvider.ipp"

#include "document/DocumentArg.ipp"
#include "document/DocumentCDB.ipp"
#include "document/DocumentHtml.ipp"
#include "document/DocumentPost.ipp"
#include "document/DocumentProvider.ipp"

#include "entity/EntityCDB.ipp"

#include "enum/Access.hpp"
#include "enum/AssertDeny.hpp"
#include "enum/AttrKind.hpp"
#include "enum/CalSystem.hpp"
#include "enum/Change.hpp"
#include "enum/DataRole.hpp"
#include "enum/FileType.hpp"
#include "enum/Format.hpp"
#include "enum/Linkage.hpp"
#include "enum/Multiplicity.hpp"
#include "enum/Permission.hpp"
#include "enum/Reset.hpp"
#include "enum/Restriction.hpp"
#include "enum/SizeDesc.hpp"
#include "enum/Submit.hpp"
#include "enum/Vcs.hpp"


#include "event/EventCDB.ipp"
#include "event/EventHtml.ipp"
#include "event/EventProvider.ipp"


#include "field/FieldArg.ipp"
#include "field/FieldCDB.ipp"
#include "field/FieldData.ipp"
#include "field/FieldFile.ipp"
#include "field/FieldHtml.ipp"
#include "field/FieldPost.ipp"
#include "field/FieldProvider.ipp"

#include "file/FileSpec.ipp"

#include "folder/FolderArg.ipp"
#include "folder/FolderCDB.ipp"
#include "folder/FolderHtml.ipp"
#include "folder/FolderPost.ipp"
#include "folder/FolderProvider.ipp"

#include "fragment/FragmentArg.ipp"
#include "fragment/FragmentCDB.ipp"
#include "fragment/FragmentHtml.ipp"
#include "fragment/FragmentPost.ipp"
#include "fragment/FragmentProvider.ipp"

//#include "game/GameArg.ipp"
#include "game/GameCDB.ipp"
#include "game/GameHtml.ipp"
//#include "game/GamePost.ipp"
#include "game/GameProvider.ipp"

#include "graphviz/GraphvizCDB.ipp"

//#include "group/GroupArg.ipp"
#include "group/GroupCDB.ipp"
#include "group/GroupHtml.ipp"
//#include "group/GroupPost.ipp"

#include "html/Html.ipp"
#include "html/Post.ipp"

#include "id/id.ipp"        // order sensitive with id_auto.ipp

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
#include "leaf/LeafCDB.ipp"
#include "leaf/LeafData.ipp"
#include "leaf/LeafFile.ipp"
#include "leaf/LeafHtml.ipp"
#include "leaf/LeafPost.ipp"
#include "leaf/LeafProvider.ipp"

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

#include "organization/OrganizationCDB.ipp"

//#include "place/PlaceArg.ipp"
#include "place/PlaceCDB.ipp"
#include "place/PlaceHtml.ipp"
//#include "place/PlacePost.ipp"
#include "place/PlaceProvider.ipp"

#include "quill/QuillData.ipp"
#include "quill/QuillFile.ipp"

#include "root/RootArg.ipp"
#include "root/RootCDB.ipp"
#include "root/RootHtml.ipp"
#include "root/RootPost.ipp"
#include "root/RootProvider.ipp"

#include "rule/Action.ipp"
#include "rule/Constraint.ipp"
#include "rule/Trigger.ipp"

//#include "rule/Action.ipp"
//#include "rule/Constraint.ipp"
//#include "rule/Trigger.ipp"


//#include "tag/Tag.ipp"
#include "tag/TagArg.ipp"
#include "tag/TagCDB.ipp"
#include "tag/TagData.ipp"
#include "tag/TagFile.ipp"
#include "tag/TagHtml.ipp"
#include "tag/TagPost.ipp"
#include "tag/TagProvider.ipp"

//#include "thing/ThingArg.ipp"
#include "thing/ThingCDB.ipp"
#include "thing/ThingHtml.ipp"
//#include "thing/ThingPost.ipp"

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

#include "wksp/CacheQuery.ipp"
#include "wksp/CacheStatement.ipp"

namespace yq::mithril::kernel {
    const char*     build_root()
    {
        return YQ_BUILD_ROOT;
    }

    const char*     share_directories()
    {
        return YQ_SHARE_DIRS;
    }
}


#include <meta/Init.hpp>


//  ENUM TYPES

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

//  ID TYPES
YQ_TYPE_IMPLEMENT(yq::mithril::Atom)
YQ_TYPE_IMPLEMENT(yq::mithril::Atom::Property)
YQ_TYPE_IMPLEMENT(yq::mithril::Attribute)
YQ_TYPE_IMPLEMENT(yq::mithril::Book)
YQ_TYPE_IMPLEMENT(yq::mithril::Category)
YQ_TYPE_IMPLEMENT(yq::mithril::Character)
YQ_TYPE_IMPLEMENT(yq::mithril::Class)
YQ_TYPE_IMPLEMENT(yq::mithril::Directory)
YQ_TYPE_IMPLEMENT(yq::mithril::Document)
YQ_TYPE_IMPLEMENT(yq::mithril::Entity)
YQ_TYPE_IMPLEMENT(yq::mithril::Event)
YQ_TYPE_IMPLEMENT(yq::mithril::Field)
YQ_TYPE_IMPLEMENT(yq::mithril::Folder)
YQ_TYPE_IMPLEMENT(yq::mithril::Fragment)
YQ_TYPE_IMPLEMENT(yq::mithril::Game)
YQ_TYPE_IMPLEMENT(yq::mithril::Graphviz)
YQ_TYPE_IMPLEMENT(yq::mithril::Group)
YQ_TYPE_IMPLEMENT(yq::mithril::Image)
YQ_TYPE_IMPLEMENT(yq::mithril::Leaf)
YQ_TYPE_IMPLEMENT(yq::mithril::Organization)
YQ_TYPE_IMPLEMENT(yq::mithril::Place)
YQ_TYPE_IMPLEMENT(yq::mithril::Root)
YQ_TYPE_IMPLEMENT(yq::mithril::Tag)
YQ_TYPE_IMPLEMENT(yq::mithril::Thing)
YQ_TYPE_IMPLEMENT(yq::mithril::User)
YQ_TYPE_IMPLEMENT(yq::mithril::Value)



using namespace yq;
using namespace yq::mithril;

namespace {
    void        reg_mithril()
    {
        {
            auto w  = writer<Atom>();
            w.property("id", &Atom::id);
        }
    }
    YQ_INVOKE(reg_mithril();)
}


