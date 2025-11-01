////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    WARNING... this file is auto generated!
*/


#include <yq/mithril/atom/Atom.hpp>
#include <yq/mithril/atom/AtomCDB.hpp>   

#include <yq/mithril/attribute/Attribute.hpp>
#include <yq/mithril/attribute/AttributeCDB.hpp>   

#include <yq/mithril/book/Book.hpp>
#include <yq/mithril/book/BookCDB.hpp>   

#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/category/CategoryCDB.hpp>   

#include <yq/mithril/character/Character.hpp>
#include <yq/mithril/character/CharacterCDB.hpp>   

#include <yq/mithril/class/Class.hpp>
#include <yq/mithril/class/ClassCDB.hpp>   

#include <yq/mithril/directory/Directory.hpp>
#include <yq/mithril/directory/DirectoryCDB.hpp>   

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>   

#include <yq/mithril/doodle/Doodle.hpp>
#include <yq/mithril/doodle/DoodleCDB.hpp>   

#include <yq/mithril/entity/Entity.hpp>
#include <yq/mithril/entity/EntityCDB.hpp>   

#include <yq/mithril/event/Event.hpp>
#include <yq/mithril/event/EventCDB.hpp>   

#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/field/FieldCDB.hpp>   

#include <yq/mithril/folder/Folder.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>   

#include <yq/mithril/fragment/Fragment.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>   

#include <yq/mithril/game/Game.hpp>
#include <yq/mithril/game/GameCDB.hpp>   

#include <yq/mithril/graphviz/Graphviz.hpp>
#include <yq/mithril/graphviz/GraphvizCDB.hpp>   

#include <yq/mithril/group/Group.hpp>
#include <yq/mithril/group/GroupCDB.hpp>   

#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/image/ImageCDB.hpp>   

#include <yq/mithril/leaf/Leaf.hpp>
#include <yq/mithril/leaf/LeafCDB.hpp>   

#include <yq/mithril/meta/Meta.hpp>
#include <yq/mithril/meta/MetaCDB.hpp>   

#include <yq/mithril/novel/Novel.hpp>
#include <yq/mithril/novel/NovelCDB.hpp>   

#include <yq/mithril/organization/Organization.hpp>
#include <yq/mithril/organization/OrganizationCDB.hpp>   

#include <yq/mithril/place/Place.hpp>
#include <yq/mithril/place/PlaceCDB.hpp>   

#include <yq/mithril/root/Root.hpp>
#include <yq/mithril/root/RootCDB.hpp>   

#include <yq/mithril/style/Style.hpp>
#include <yq/mithril/style/StyleCDB.hpp>   

#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/tag/TagCDB.hpp>   

#include <yq/mithril/thing/Thing.hpp>
#include <yq/mithril/thing/ThingCDB.hpp>   

#include <yq/mithril/user/User.hpp>
#include <yq/mithril/user/UserCDB.hpp>   

#include <yq/mithril/value/Value.hpp>
#include <yq/mithril/value/ValueCDB.hpp>   


#include "id_high.hpp"

namespace yq::mithril {
    std::string_view  Id::type_name(IdTypeId ct)
    {
        switch(ct){
        case Atom::ID:
            return "Atom";
        case Atom::Property::ID:
            return "Atom::Property";
        case Attribute::ID:
            return "Attribute";
        case Book::ID:
            return "Book";
        case Category::ID:
            return "Category";
        case Character::ID:
            return "Character";
        case Class::ID:
            return "Class";
        case Directory::ID:
            return "Directory";
        case Document::ID:
            return "Document";
        case Doodle::ID:
            return "Doodle";
        case Entity::ID:
            return "Entity";
        case Event::ID:
            return "Event";
        case Field::ID:
            return "Field";
        case Folder::ID:
            return "Folder";
        case Fragment::ID:
            return "Fragment";
        case Game::ID:
            return "Game";
        case Graphviz::ID:
            return "Graphviz";
        case Group::ID:
            return "Group";
        case Image::ID:
            return "Image";
        case Leaf::ID:
            return "Leaf";
        case Meta::ID:
            return "Meta";
        case Novel::ID:
            return "Novel";
        case Organization::ID:
            return "Organization";
        case Place::ID:
            return "Place";
        case Root::ID:
            return "Root";
        case Style::ID:
            return "Style";
        case Tag::ID:
            return "Tag";
        case Thing::ID:
            return "Thing";
        case User::ID:
            return "User";
        case Value::ID:
            return "Value";
        default:
            return "Unknown";
        }
    }
    
    const TypeMeta*  Id::type_info(IdTypeId ct)
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
        case Doodle::ID:
            return &meta<Doodle>();
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
        case Novel::ID:
            return &meta<Novel>();
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
        case Doodle::ID:
            return Doodle::PARENTS;
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
        case Novel::ID:
            return Novel::PARENTS;
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
        case Doodle::ID:
            return cdb::key(Doodle(id()));
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
        case Novel::ID:
            return cdb::key(Novel(id()));
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
            case Doodle::ID:
                return cdb::name(Doodle(id()));
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
            case Novel::ID:
                return cdb::name(Novel(id()));
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
        case Doodle::ID:
            return ids<Doodle>(all_doodles(sorted));
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
        case Novel::ID:
            return ids<Novel>(all_novels(sorted));
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
