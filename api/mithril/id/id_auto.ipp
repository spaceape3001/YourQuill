////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*
    WARNING... this file is auto generated!
*/


#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomCDB.hpp>   

#include <mithril/attribute/Attribute.hpp>
#include <mithril/attribute/AttributeCDB.hpp>   

#include <mithril/book/Book.hpp>
#include <mithril/book/BookCDB.hpp>   

#include <mithril/category/Category.hpp>
#include <mithril/category/CategoryCDB.hpp>   

#include <mithril/character/Character.hpp>
#include <mithril/character/CharacterCDB.hpp>   

#include <mithril/class/Class.hpp>
#include <mithril/class/ClassCDB.hpp>   

#include <mithril/directory/Directory.hpp>
#include <mithril/directory/DirectoryCDB.hpp>   

#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentCDB.hpp>   

#include <mithril/drafter/Drafter.hpp>
#include <mithril/drafter/DrafterCDB.hpp>   

#include <mithril/entity/Entity.hpp>
#include <mithril/entity/EntityCDB.hpp>   

#include <mithril/event/Event.hpp>
#include <mithril/event/EventCDB.hpp>   

#include <mithril/field/Field.hpp>
#include <mithril/field/FieldCDB.hpp>   

#include <mithril/folder/Folder.hpp>
#include <mithril/folder/FolderCDB.hpp>   

#include <mithril/fragment/Fragment.hpp>
#include <mithril/fragment/FragmentCDB.hpp>   

#include <mithril/game/Game.hpp>
#include <mithril/game/GameCDB.hpp>   

#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>   

#include <mithril/group/Group.hpp>
#include <mithril/group/GroupCDB.hpp>   

#include <mithril/image/Image.hpp>
#include <mithril/image/ImageCDB.hpp>   

#include <mithril/leaf/Leaf.hpp>
#include <mithril/leaf/LeafCDB.hpp>   

#include <mithril/meta/Meta.hpp>
#include <mithril/meta/MetaCDB.hpp>   

#include <mithril/organization/Organization.hpp>
#include <mithril/organization/OrganizationCDB.hpp>   

#include <mithril/place/Place.hpp>
#include <mithril/place/PlaceCDB.hpp>   

#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>   

#include <mithril/style/Style.hpp>
#include <mithril/style/StyleCDB.hpp>   

#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagCDB.hpp>   

#include <mithril/thing/Thing.hpp>
#include <mithril/thing/ThingCDB.hpp>   

#include <mithril/user/User.hpp>
#include <mithril/user/UserCDB.hpp>   

#include <mithril/value/Value.hpp>
#include <mithril/value/ValueCDB.hpp>   

namespace yq::mithril {
    static constexpr const IdTypeId HIGH_ID = 30;
    
    std::string_view  Id::type_name(IdTypeId ct)
    {
        switch(ct){
        case Atom::ID:
            return "Atom"sv;
        case Atom::Property::ID:
            return "Atom::Property"sv;
        case Attribute::ID:
            return "Attribute"sv;
        case Book::ID:
            return "Book"sv;
        case Category::ID:
            return "Category"sv;
        case Character::ID:
            return "Character"sv;
        case Class::ID:
            return "Class"sv;
        case Directory::ID:
            return "Directory"sv;
        case Document::ID:
            return "Document"sv;
        case Drafter::ID:
            return "Drafter"sv;
        case Entity::ID:
            return "Entity"sv;
        case Event::ID:
            return "Event"sv;
        case Field::ID:
            return "Field"sv;
        case Folder::ID:
            return "Folder"sv;
        case Fragment::ID:
            return "Fragment"sv;
        case Game::ID:
            return "Game"sv;
        case Graphviz::ID:
            return "Graphviz"sv;
        case Group::ID:
            return "Group"sv;
        case Image::ID:
            return "Image"sv;
        case Leaf::ID:
            return "Leaf"sv;
        case Meta::ID:
            return "Meta"sv;
        case Organization::ID:
            return "Organization"sv;
        case Place::ID:
            return "Place"sv;
        case Root::ID:
            return "Root"sv;
        case Style::ID:
            return "Style"sv;
        case Tag::ID:
            return "Tag"sv;
        case Thing::ID:
            return "Thing"sv;
        case User::ID:
            return "User"sv;
        case Value::ID:
            return "Value"sv;
        default:
            return "Unknown"sv;
        }
    }
    
    const TypeInfo*  Id::type_info(IdTypeId ct)
    {
        switch(ct){
        case Atom::ID:
            return &meta<Atom>();
        case Atom::Property::ID:
            return &meta<Atom::Property>();
        case Attribute::ID:
            return &meta<Attribute>();
        case Book::ID:
            return &meta<Book>();
        case Category::ID:
            return &meta<Category>();
        case Character::ID:
            return &meta<Character>();
        case Class::ID:
            return &meta<Class>();
        case Directory::ID:
            return &meta<Directory>();
        case Document::ID:
            return &meta<Document>();
        case Drafter::ID:
            return &meta<Drafter>();
        case Entity::ID:
            return &meta<Entity>();
        case Event::ID:
            return &meta<Event>();
        case Field::ID:
            return &meta<Field>();
        case Folder::ID:
            return &meta<Folder>();
        case Fragment::ID:
            return &meta<Fragment>();
        case Game::ID:
            return &meta<Game>();
        case Graphviz::ID:
            return &meta<Graphviz>();
        case Group::ID:
            return &meta<Group>();
        case Image::ID:
            return &meta<Image>();
        case Leaf::ID:
            return &meta<Leaf>();
        case Meta::ID:
            return &meta<Meta>();
        case Organization::ID:
            return &meta<Organization>();
        case Place::ID:
            return &meta<Place>();
        case Root::ID:
            return &meta<Root>();
        case Style::ID:
            return &meta<Style>();
        case Tag::ID:
            return &meta<Tag>();
        case Thing::ID:
            return &meta<Thing>();
        case User::ID:
            return &meta<User>();
        case Value::ID:
            return &meta<Value>();
        default:
            return nullptr;
        }
    }
    
    IdTypes  Id::base_types(IdTypeId ct)
    {
        switch(ct){
        case Atom::ID:
            return Atom::PARENTS;
        case Atom::Property::ID:
            return Atom::Property::PARENTS;
        case Attribute::ID:
            return Attribute::PARENTS;
        case Book::ID:
            return Book::PARENTS;
        case Category::ID:
            return Category::PARENTS;
        case Character::ID:
            return Character::PARENTS;
        case Class::ID:
            return Class::PARENTS;
        case Directory::ID:
            return Directory::PARENTS;
        case Document::ID:
            return Document::PARENTS;
        case Drafter::ID:
            return Drafter::PARENTS;
        case Entity::ID:
            return Entity::PARENTS;
        case Event::ID:
            return Event::PARENTS;
        case Field::ID:
            return Field::PARENTS;
        case Folder::ID:
            return Folder::PARENTS;
        case Fragment::ID:
            return Fragment::PARENTS;
        case Game::ID:
            return Game::PARENTS;
        case Graphviz::ID:
            return Graphviz::PARENTS;
        case Group::ID:
            return Group::PARENTS;
        case Image::ID:
            return Image::PARENTS;
        case Leaf::ID:
            return Leaf::PARENTS;
        case Meta::ID:
            return Meta::PARENTS;
        case Organization::ID:
            return Organization::PARENTS;
        case Place::ID:
            return Place::PARENTS;
        case Root::ID:
            return Root::PARENTS;
        case Style::ID:
            return Style::PARENTS;
        case Tag::ID:
            return Tag::PARENTS;
        case Thing::ID:
            return Thing::PARENTS;
        case User::ID:
            return User::PARENTS;
        case Value::ID:
            return Value::PARENTS;
        default:
            return {};
        }
    }
    
    IdTypeId  Id::max_type()
    {
        return HIGH_ID - 1;
    }
    
    std::string  Id::key() const
    {
        switch(type()){
    
        case Atom::ID:
            return cdb::key(Atom(id()));
        case Atom::Property::ID:
            return cdb::key(Atom(id()));
        case Attribute::ID:
            return cdb::key(Attribute(id()));
        case Book::ID:
            return cdb::key(Book(id()));
        case Category::ID:
            return cdb::key(Category(id()));
        case Character::ID:
            return cdb::key(Character(id()));
        case Class::ID:
            return cdb::key(Class(id()));
        case Directory::ID:
            return cdb::key(Directory(id()));
        case Document::ID:
            return cdb::key(Document(id()));
        case Drafter::ID:
            return cdb::key(Drafter(id()));
        case Entity::ID:
            return cdb::key(Entity(id()));
        case Event::ID:
            return cdb::key(Event(id()));
        case Field::ID:
            return cdb::key(Field(id()));
        case Folder::ID:
            return cdb::key(Folder(id()));
        case Fragment::ID:
            return cdb::key(Fragment(id()));
        case Game::ID:
            return cdb::key(Game(id()));
        case Graphviz::ID:
            return cdb::key(Graphviz(id()));
        case Group::ID:
            return cdb::key(Group(id()));
        case Image::ID:
            return cdb::key(Image(id()));
        case Leaf::ID:
            return cdb::key(Leaf(id()));
        case Meta::ID:
            return cdb::key(Meta(id()));
        case Organization::ID:
            return cdb::key(Organization(id()));
        case Place::ID:
            return cdb::key(Place(id()));
        case Root::ID:
            return cdb::key(Root(id()));
        case Style::ID:
            return cdb::key(Style(id()));
        case Tag::ID:
            return cdb::key(Tag(id()));
        case Thing::ID:
            return cdb::key(Thing(id()));
        case User::ID:
            return cdb::key(User(id()));
        case Value::ID:
            return cdb::key(Value(id()));
        default:
            return std::string();
        }
    }
    
    std::string Id::name() const
    {
        switch(type()){
    
            case Atom::ID:
                return cdb::name(Atom(id()));
            case Atom::Property::ID:
                return cdb::name(Atom::Property(id()));
            case Attribute::ID:
                return cdb::name(Attribute(id()));
            case Book::ID:
                return cdb::name(Book(id()));
            case Category::ID:
                return cdb::name(Category(id()));
            case Character::ID:
                return cdb::name(Character(id()));
            case Class::ID:
                return cdb::name(Class(id()));
            case Directory::ID:
                return cdb::name(Directory(id()));
            case Document::ID:
                return cdb::name(Document(id()));
            case Drafter::ID:
                return cdb::name(Drafter(id()));
            case Entity::ID:
                return cdb::name(Entity(id()));
            case Event::ID:
                return cdb::name(Event(id()));
            case Field::ID:
                return cdb::name(Field(id()));
            case Folder::ID:
                return cdb::name(Folder(id()));
            case Fragment::ID:
                return cdb::name(Fragment(id()));
            case Game::ID:
                return cdb::name(Game(id()));
            case Graphviz::ID:
                return cdb::name(Graphviz(id()));
            case Group::ID:
                return cdb::name(Group(id()));
            case Image::ID:
                return cdb::name(Image(id()));
            case Leaf::ID:
                return cdb::name(Leaf(id()));
            case Meta::ID:
                return cdb::name(Meta(id()));
            case Organization::ID:
                return cdb::name(Organization(id()));
            case Place::ID:
                return cdb::name(Place(id()));
            case Root::ID:
                return cdb::name(Root(id()));
            case Style::ID:
                return cdb::name(Style(id()));
            case Tag::ID:
                return cdb::name(Tag(id()));
            case Thing::ID:
                return cdb::name(Thing(id()));
            case User::ID:
                return cdb::name(User(id()));
            case Value::ID:
                return cdb::name(Value(id()));
        default:
            return std::string();
        }
    }
}

namespace yq::mithril::cdb {
    std::vector<Id>     all(IdTypeId ct, Sorted sorted)
    {
        switch(ct){
        case Atom::ID:
            return ids<Atom>(all_atoms(sorted));
        case Attribute::ID:
            return ids<Attribute>(all_attributes(sorted));
        case Book::ID:
            return ids<Book>(all_books(sorted));
        case Category::ID:
            return ids<Category>(all_categories(sorted));
        case Character::ID:
            return ids<Character>(all_characters(sorted));
        case Class::ID:
            return ids<Class>(all_classes(sorted));
        case Directory::ID:
            return ids<Directory>(all_directories(sorted));
        case Document::ID:
            return ids<Document>(all_documents(sorted));
        case Drafter::ID:
            return ids<Drafter>(all_drafters(sorted));
        case Entity::ID:
            return ids<Entity>(all_entities(sorted));
        case Event::ID:
            return ids<Event>(all_events(sorted));
        case Field::ID:
            return ids<Field>(all_fields(sorted));
        case Folder::ID:
            return ids<Folder>(all_folders(sorted));
        case Fragment::ID:
            return ids<Fragment>(all_fragments(sorted));
        case Game::ID:
            return ids<Game>(all_games(sorted));
        case Graphviz::ID:
            return ids<Graphviz>(all_graphvizs(sorted));
        case Group::ID:
            return ids<Group>(all_groups(sorted));
        case Image::ID:
            return ids<Image>(all_images(sorted));
        case Leaf::ID:
            return ids<Leaf>(all_leafs(sorted));
        case Meta::ID:
            return ids<Meta>(all_metas(sorted));
        case Organization::ID:
            return ids<Organization>(all_organizations(sorted));
        case Place::ID:
            return ids<Place>(all_places(sorted));
        case Root::ID:
            return ids<Root>(all_roots(sorted));
        case Style::ID:
            return ids<Style>(all_styles(sorted));
        case Tag::ID:
            return ids<Tag>(all_tags(sorted));
        case Thing::ID:
            return ids<Thing>(all_things(sorted));
        case User::ID:
            return ids<User>(all_users(sorted));
        case Value::ID:
            return ids<Value>(all_values(sorted));
        default:
            return std::vector<Id>();
        }
    }
}    
