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
#include "atom/AtomDiff.hpp"    // syntax check
#include "atom/AtomHtml.ipp"
#include "atom/AtomMeta.ipp"
#include "atom/AtomPost.ipp"
#include "atom/AtomProvider.ipp"
#include "atom/AtomSpec.ipp"

//#include "atom/Property.hpp"


#include "attribute/AttributeArg.ipp"
#include "attribute/AttributeCDB.ipp"
#include "attribute/AttributeDiff.ipp"
#include "attribute/AttributeHtml.ipp"
#include "attribute/AttributeMeta.ipp"

#include "auth/Authentication.ipp"

#include "bit/Context.ipp"
#include "bit/Copyright.ipp"
#include "bit/KeyValue.ipp"
#include "bit/Policy.ipp"

//#include "book/BookArg.ipp"
#include "book/BookCDB.ipp"
#include "book/BookHtml.ipp"
#include "book/BookMeta.ipp"
//#include "book/BookPost.ipp"
#include "book/BookProvider.ipp"


#include "category/CategoryArg.ipp"
#include "category/CategoryCDB.ipp"
#include "category/CategoryData.ipp"
#include "category/CategoryDiff.hpp"        // syntax check
#include "category/CategoryFile.ipp"
#include "category/CategoryHtml.ipp"
#include "category/CategoryMeta.ipp"
#include "category/CategoryPost.ipp"
#include "category/CategoryProvider.ipp"

//#include "character/CharacterArg.ipp"
#include "character/CharacterCDB.ipp"
#include "character/CharacterHtml.ipp"
#include "character/CharacterMeta.ipp"
//#include "character/CharacterPost.ipp"
#include "character/CharacterProvider.ipp"

#include "class/Class.ipp"
#include "class/ClassArg.ipp"
#include "class/ClassCDB.ipp"
#include "class/ClassData.ipp"
#include "class/ClassDiff.hpp"          // syntax check
#include "class/ClassFile.ipp"
#include "class/ClassHtml.ipp"
#include "class/ClassMeta.ipp"
#include "class/ClassPost.ipp"
#include "class/ClassProvider.ipp"
//#include "class/ClassUpdate.ipp"

#include "db/DBUtils.ipp"

#include "directory/DirectoryArg.ipp"
#include "directory/DirectoryCDB.ipp"
#include "directory/DirectoryHtml.ipp"
#include "directory/DirectoryMeta.ipp"
#include "directory/DirectoryPost.ipp"
#include "directory/DirectoryProvider.ipp"

#include "document/DocumentArg.ipp"
#include "document/DocumentCDB.ipp"
#include "document/DocumentHtml.ipp"
#include "document/DocumentMeta.ipp"
#include "document/DocumentPost.ipp"
#include "document/DocumentProvider.ipp"
#include "document/DocumentUpdate.ipp"

#include "entity/EntityCDB.ipp"
#include "entity/EntityMeta.ipp"

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
#include "event/EventMeta.ipp"
#include "event/EventProvider.ipp"

#include "field/FieldArg.ipp"
#include "field/FieldCDB.ipp"
#include "field/FieldData.ipp"
#include "field/FieldDiff.hpp"      // syntax check
#include "field/FieldFile.ipp"
#include "field/FieldHtml.ipp"
#include "field/FieldMeta.ipp"
#include "field/FieldPost.ipp"
#include "field/FieldProvider.ipp"

#include "file/FileSpec.ipp"

#include "folder/FolderArg.ipp"
#include "folder/FolderCDB.ipp"
#include "folder/FolderHtml.ipp"
#include "folder/FolderMeta.ipp"
#include "folder/FolderPost.ipp"
#include "folder/FolderProvider.ipp"

#include "fragment/FragmentArg.ipp"
#include "fragment/FragmentCDB.ipp"
#include "fragment/FragmentHtml.ipp"
#include "fragment/FragmentMeta.ipp"
#include "fragment/FragmentPost.ipp"
#include "fragment/FragmentProvider.ipp"

//#include "game/GameArg.ipp"
#include "game/GameCDB.ipp"
#include "game/GameHtml.ipp"
#include "game/GameMeta.ipp"
//#include "game/GamePost.ipp"
#include "game/GameProvider.ipp"

#include "graphviz/Graphviz.ipp"
#include "graphviz/GraphvizArg.ipp"
#include "graphviz/GraphvizBuilder.ipp"
#include "graphviz/GraphvizCDB.ipp"
#include "graphviz/GraphvizHtml.ipp"
#include "graphviz/GraphvizMeta.ipp"

//#include "group/GroupArg.ipp"
#include "group/GroupCDB.ipp"
#include "group/GroupHtml.ipp"
#include "group/GroupMeta.ipp"
//#include "group/GroupPost.ipp"

#include "html/Html.ipp"
#include "html/Post.ipp"

#include "id/id.ipp"        // order sensitive with id_auto.ipp

#include "image/ImageArg.ipp"
#include "image/ImageCDB.ipp"
#include "image/ImageDiff.hpp"  // syntax check
#include "image/ImageHtml.ipp"
#include "image/ImageMeta.ipp"
#include "image/ImagePost.ipp"

#include "io/StdFile.ipp"
#include "io/StdObject.ipp"

#include "ipc/DirWatcher.ipp"
#include "ipc/ipcPackets.hpp"
#include "ipc/ipcSocket.ipp"

#include "leaf/LeafArg.ipp"
#include "leaf/LeafCDB.ipp"
#include "leaf/LeafData.ipp"
#include "leaf/LeafDiff.hpp"    // syntax check
#include "leaf/LeafFile.ipp"
#include "leaf/LeafHtml.ipp"
#include "leaf/LeafMeta.ipp"
#include "leaf/LeafPost.ipp"
#include "leaf/LeafProvider.ipp"

#include "meta/MetaCDB.ipp"
#include "meta/MetaHtml.ipp"
#include "meta/MetaMeta.ipp"
#include "meta/MetaObjectArg.ipp"
#include "meta/MetaTypeArg.ipp"

#include "notify/AtomNotifier.ipp"
#include "notify/FileWatch.ipp"
#include "notify/FunctionalNotifier.ipp"
#include "notify/FunctionalStage4.ipp"
#include "notify/FileNotifier.ipp"
#include "notify/Notifier.hxx"
#include "notify/Stage2.ipp"
#include "notify/Stage3.ipp"
#include "notify/Stage4.ipp"
#include "notify/Stage5.ipp"

#include "organization/OrganizationCDB.ipp"
#include "organization/OrganizationMeta.ipp"

//#include "place/PlaceArg.ipp"
#include "place/PlaceCDB.ipp"
#include "place/PlaceHtml.ipp"
#include "place/PlaceMeta.ipp"
//#include "place/PlacePost.ipp"
#include "place/PlaceProvider.ipp"

#include "quill/QuillData.ipp"
#include "quill/QuillFile.ipp"

#include "root/RootArg.ipp"
#include "root/RootCDB.ipp"
#include "root/RootHtml.ipp"
#include "root/RootMeta.ipp"
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
#include "tag/TagDiff.hpp"  // syntax check
#include "tag/TagFile.ipp"
#include "tag/TagHtml.ipp"
#include "tag/TagMeta.ipp"
#include "tag/TagPost.ipp"
#include "tag/TagProvider.ipp"

//#include "thing/ThingArg.ipp"
#include "thing/ThingCDB.ipp"
#include "thing/ThingHtml.ipp"
#include "thing/ThingMeta.ipp"
//#include "thing/ThingPost.ipp"

#include "update/U.hxx"

#include "user/UserArg.ipp"
#include "user/UserCDB.ipp"
#include "user/UserData.ipp"
#include "user/UserDiff.hpp"
#include "user/UserFile.ipp"
#include "user/UserHtml.ipp"
#include "user/UserMeta.ipp"
#include "user/UserPost.ipp"

#include "util/Markdown.ipp"


#include "value/ValueArg.ipp"
#include "value/ValueCDB.ipp"
#include "value/ValueData.ipp"
#include "value/ValueFile.ipp"
#include "value/ValueHtml.ipp"
#include "value/ValueMeta.ipp"

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


#include <0/meta/Init.hpp>


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
YQ_TYPE_IMPLEMENT(yq::mithril::AtomSet)
YQ_TYPE_IMPLEMENT(yq::mithril::AtomVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Atom::Property)
YQ_TYPE_IMPLEMENT(yq::mithril::Atom::PropertySet)
YQ_TYPE_IMPLEMENT(yq::mithril::Atom::PropertyVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Attribute)
YQ_TYPE_IMPLEMENT(yq::mithril::AttributeSet)
YQ_TYPE_IMPLEMENT(yq::mithril::AttributeVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Book)
YQ_TYPE_IMPLEMENT(yq::mithril::BookSet)
YQ_TYPE_IMPLEMENT(yq::mithril::BookVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Category)
YQ_TYPE_IMPLEMENT(yq::mithril::CategorySet)
YQ_TYPE_IMPLEMENT(yq::mithril::CategoryVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Character)
YQ_TYPE_IMPLEMENT(yq::mithril::CharacterSet)
YQ_TYPE_IMPLEMENT(yq::mithril::CharacterVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Class)
YQ_TYPE_IMPLEMENT(yq::mithril::ClassSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ClassVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Directory)
YQ_TYPE_IMPLEMENT(yq::mithril::DirectorySet)
YQ_TYPE_IMPLEMENT(yq::mithril::DirectoryVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Document)
YQ_TYPE_IMPLEMENT(yq::mithril::DocumentSet)
YQ_TYPE_IMPLEMENT(yq::mithril::DocumentVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Entity)
YQ_TYPE_IMPLEMENT(yq::mithril::EntitySet)
YQ_TYPE_IMPLEMENT(yq::mithril::EntityVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Event)
YQ_TYPE_IMPLEMENT(yq::mithril::EventSet)
YQ_TYPE_IMPLEMENT(yq::mithril::EventVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Field)
YQ_TYPE_IMPLEMENT(yq::mithril::FieldSet)
YQ_TYPE_IMPLEMENT(yq::mithril::FieldVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Folder)
YQ_TYPE_IMPLEMENT(yq::mithril::FolderSet)
YQ_TYPE_IMPLEMENT(yq::mithril::FolderVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Fragment)
YQ_TYPE_IMPLEMENT(yq::mithril::FragmentSet)
YQ_TYPE_IMPLEMENT(yq::mithril::FragmentVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Game)
YQ_TYPE_IMPLEMENT(yq::mithril::GameSet)
YQ_TYPE_IMPLEMENT(yq::mithril::GameVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Graphviz)
YQ_TYPE_IMPLEMENT(yq::mithril::GraphvizSet)
YQ_TYPE_IMPLEMENT(yq::mithril::GraphvizVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Group)
YQ_TYPE_IMPLEMENT(yq::mithril::GroupSet)
YQ_TYPE_IMPLEMENT(yq::mithril::GroupVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Image)
YQ_TYPE_IMPLEMENT(yq::mithril::ImageSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ImageVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Leaf)
YQ_TYPE_IMPLEMENT(yq::mithril::LeafSet)
YQ_TYPE_IMPLEMENT(yq::mithril::LeafVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Meta)
YQ_TYPE_IMPLEMENT(yq::mithril::Organization)
YQ_TYPE_IMPLEMENT(yq::mithril::OrganizationSet)
YQ_TYPE_IMPLEMENT(yq::mithril::OrganizationVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Place)
YQ_TYPE_IMPLEMENT(yq::mithril::PlaceSet)
YQ_TYPE_IMPLEMENT(yq::mithril::PlaceVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Root)
YQ_TYPE_IMPLEMENT(yq::mithril::Tag)
YQ_TYPE_IMPLEMENT(yq::mithril::TagSet)
YQ_TYPE_IMPLEMENT(yq::mithril::TagVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Thing)
YQ_TYPE_IMPLEMENT(yq::mithril::ThingSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ThingVector)
YQ_TYPE_IMPLEMENT(yq::mithril::User)
YQ_TYPE_IMPLEMENT(yq::mithril::UserSet)
YQ_TYPE_IMPLEMENT(yq::mithril::UserVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Value)
YQ_TYPE_IMPLEMENT(yq::mithril::ValueSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ValueVector)


namespace yq::mithril {
    template class Notifier<const Atom::Diff&>;
    template class Notifier<const Category::Diff&>;
    template class Notifier<const Class::Diff&>;
    template class Notifier<const Field::Diff&>;
    template class Notifier<const Image::Diff&>;
    template class Notifier<const Leaf::Diff&>;
    template class Notifier<const Tag::Diff&>;
    template class Notifier<const User::Diff&>;
}



using namespace yq;
using namespace yq::mithril;

namespace {
    void        reg_mithril()
    {
        reg_atom_meta();
        reg_attribute_meta();
        reg_book_meta();
        reg_category_meta();
        reg_character_meta();
        reg_class_meta();
        reg_directory_meta();
        reg_document_meta();
        reg_entity_meta();
        reg_event_meta();
        reg_field_meta();
        reg_folder_meta();
        reg_fragment_meta();
        reg_game_meta();
        reg_graphviz_meta();
        reg_group_meta();
        reg_image_meta();
        reg_leaf_meta();
        reg_meta_meta();
        reg_organization_meta();
        reg_place_meta();
        reg_root_meta();
        reg_tag_meta();
        reg_thing_meta();
        reg_user_meta();
        reg_value_meta();
    }
    YQ_INVOKE(reg_mithril();)
}


