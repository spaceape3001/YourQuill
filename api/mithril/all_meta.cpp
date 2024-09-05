////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq-toolbox/meta/ObjectInfoWriter.hpp>


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



#include "atom/AtomMeta.ipp"
#include "attribute/AttributeMeta.ipp"
#include "auth/AuthMeta.ipp"
#include "book/BookMeta.ipp"
#include "category/CategoryMeta.ipp"
#include "character/CharacterMeta.ipp"
#include "class/ClassMeta.ipp"
#include "directory/DirectoryMeta.ipp"
#include "document/DocumentMeta.ipp"
#include "doodle/DoodleMeta.ipp"
#include "entity/EntityMeta.ipp"
#include "event/EventMeta.ipp"
#include "field/FieldMeta.ipp"
#include "folder/FolderMeta.ipp"
#include "fragment/FragmentMeta.ipp"
#include "game/GameMeta.ipp"
#include "graphviz/GraphvizMeta.ipp"
#include "group/GroupMeta.ipp"
#include "image/ImageMeta.ipp"
#include "leaf/LeafMeta.ipp"
#include "meta/MetaMeta.ipp"
#include "novel/NovelMeta.ipp"
#include "organization/OrganizationMeta.ipp"
#include "place/PlaceMeta.ipp"
#include "root/RootMeta.ipp"
#include "rule/RuleMeta.ipp"
#include "style/StyleMeta.ipp"
#include "tag/TagMeta.ipp"
#include "thing/ThingMeta.ipp"
#include "user/UserMeta.ipp"
#include "value/ValueMeta.ipp"


#include <yq-toolbox/meta/Init.hpp>


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
YQ_TYPE_IMPLEMENT(yq::mithril::Doodle)
YQ_TYPE_IMPLEMENT(yq::mithril::DoodleSet)
YQ_TYPE_IMPLEMENT(yq::mithril::DoodleVector)
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
YQ_TYPE_IMPLEMENT(yq::mithril::Novel)
YQ_TYPE_IMPLEMENT(yq::mithril::NovelSet)
YQ_TYPE_IMPLEMENT(yq::mithril::NovelVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Organization)
YQ_TYPE_IMPLEMENT(yq::mithril::OrganizationSet)
YQ_TYPE_IMPLEMENT(yq::mithril::OrganizationVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Place)
YQ_TYPE_IMPLEMENT(yq::mithril::PlaceSet)
YQ_TYPE_IMPLEMENT(yq::mithril::PlaceVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Root)
YQ_TYPE_IMPLEMENT(yq::mithril::Style)
YQ_TYPE_IMPLEMENT(yq::mithril::StyleSet)
YQ_TYPE_IMPLEMENT(yq::mithril::StyleVector)
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


YQ_OBJECT_IMPLEMENT(yq::mithril::Authentication)
YQ_OBJECT_IMPLEMENT(yq::mithril::auth::AutoAccept)
YQ_OBJECT_IMPLEMENT(yq::mithril::auth::AutoReject)
YQ_OBJECT_IMPLEMENT(yq::mithril::auth::Simple)

YQ_OBJECT_IMPLEMENT(yq::mithril::Action)
YQ_OBJECT_IMPLEMENT(yq::mithril::Constraint)
YQ_OBJECT_IMPLEMENT(yq::mithril::Rule)
YQ_OBJECT_IMPLEMENT(yq::mithril::Trigger)


namespace {
    void        reg_mithril()
    {
        reg_atom_meta();
        reg_attribute_meta();
        reg_authentication_meta();
        reg_book_meta();
        reg_category_meta();
        reg_character_meta();
        reg_class_meta();
        reg_directory_meta();
        reg_document_meta();
        reg_doodle_meta();
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
        reg_novel_meta();
        reg_organization_meta();
        reg_place_meta();
        reg_root_meta();
        reg_rule_meta();
        reg_style_meta();
        reg_tag_meta();
        reg_thing_meta();
        reg_user_meta();
        reg_value_meta();
    }
    YQ_INVOKE(reg_mithril();)
}


