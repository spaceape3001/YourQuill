////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    WARNING... this file is auto generated!
*/


#include <yq/mithrilQt/atom.hpp>
#include <yq/mithrilQt/attribute.hpp>
#include <yq/mithrilQt/book.hpp>
#include <yq/mithrilQt/category.hpp>
#include <yq/mithrilQt/character.hpp>
#include <yq/mithrilQt/class.hpp>
#include <yq/mithrilQt/directory.hpp>
#include <yq/mithrilQt/document.hpp>
#include <yq/mithrilQt/doodle.hpp>
#include <yq/mithrilQt/entity.hpp>
#include <yq/mithrilQt/event.hpp>
#include <yq/mithrilQt/field.hpp>
#include <yq/mithrilQt/folder.hpp>
#include <yq/mithrilQt/fragment.hpp>
#include <yq/mithrilQt/game.hpp>
#include <yq/mithrilQt/graphviz.hpp>
#include <yq/mithrilQt/group.hpp>
#include <yq/mithrilQt/image.hpp>
#include <yq/mithrilQt/leaf.hpp>
#include <yq/mithrilQt/meta.hpp>
#include <yq/mithrilQt/novel.hpp>
#include <yq/mithrilQt/organization.hpp>
#include <yq/mithrilQt/place.hpp>
#include <yq/mithrilQt/root.hpp>
#include <yq/mithrilQt/style.hpp>
#include <yq/mithrilQt/tag.hpp>
#include <yq/mithrilQt/thing.hpp>
#include <yq/mithrilQt/user.hpp>
#include <yq/mithrilQt/value.hpp>

#include <yq/mithril/atom/Atom.hpp>
#include <yq/mithril/attribute/Attribute.hpp>
#include <yq/mithril/book/Book.hpp>
#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/character/Character.hpp>
#include <yq/mithril/class/Class.hpp>
#include <yq/mithril/directory/Directory.hpp>
#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/doodle/Doodle.hpp>
#include <yq/mithril/entity/Entity.hpp>
#include <yq/mithril/event/Event.hpp>
#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/folder/Folder.hpp>
#include <yq/mithril/fragment/Fragment.hpp>
#include <yq/mithril/game/Game.hpp>
#include <yq/mithril/graphviz/Graphviz.hpp>
#include <yq/mithril/group/Group.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/leaf/Leaf.hpp>
#include <yq/mithril/meta/Meta.hpp>
#include <yq/mithril/novel/Novel.hpp>
#include <yq/mithril/organization/Organization.hpp>
#include <yq/mithril/place/Place.hpp>
#include <yq/mithril/root/Root.hpp>
#include <yq/mithril/style/Style.hpp>
#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/thing/Thing.hpp>
#include <yq/mithril/user/User.hpp>
#include <yq/mithril/value/Value.hpp>
#include <yq/mithrilQt/id.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon       id_qIcon(Id i)
    {
        static QIcon s_ico(":/generic/unknown.svg");
        switch(i.type()){

        case Atom::ID:
            return qIcon(Atom(i.id()));
        case Atom::Property::ID:
            return qIcon(Atom::Property(i.id()));
        case Attribute::ID:
            return qIcon(Attribute(i.id()));
        case Book::ID:
            return qIcon(Book(i.id()));
        case Category::ID:
            return qIcon(Category(i.id()));
        case Character::ID:
            return qIcon(Character(i.id()));
        case Class::ID:
            return qIcon(Class(i.id()));
        case Directory::ID:
            return qIcon(Directory(i.id()));
        case Document::ID:
            return qIcon(Document(i.id()));
        case Doodle::ID:
            return qIcon(Doodle(i.id()));
        case Entity::ID:
            return qIcon(Entity(i.id()));
        case Event::ID:
            return qIcon(Event(i.id()));
        case Field::ID:
            return qIcon(Field(i.id()));
        case Folder::ID:
            return qIcon(Folder(i.id()));
        case Fragment::ID:
            return qIcon(Fragment(i.id()));
        case Game::ID:
            return qIcon(Game(i.id()));
        case Graphviz::ID:
            return qIcon(Graphviz(i.id()));
        case Group::ID:
            return qIcon(Group(i.id()));
        case Image::ID:
            return qIcon(Image(i.id()));
        case Leaf::ID:
            return qIcon(Leaf(i.id()));
        case Meta::ID:
            return qIcon(Meta(i.id()));
        case Novel::ID:
            return qIcon(Novel(i.id()));
        case Organization::ID:
            return qIcon(Organization(i.id()));
        case Place::ID:
            return qIcon(Place(i.id()));
        case Root::ID:
            return qIcon(Root(i.id()));
        case Style::ID:
            return qIcon(Style(i.id()));
        case Tag::ID:
            return qIcon(Tag(i.id()));
        case Thing::ID:
            return qIcon(Thing(i.id()));
        case User::ID:
            return qIcon(User(i.id()));
        case Value::ID:
            return qIcon(Value(i.id()));
        default:
            return s_ico;
        }
    }
}
