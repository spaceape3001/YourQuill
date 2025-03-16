////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    WARNING... this file is auto generated!
*/


#include <mithrilQt/atom.hpp>
#include <mithrilQt/attribute.hpp>
#include <mithrilQt/book.hpp>
#include <mithrilQt/category.hpp>
#include <mithrilQt/character.hpp>
#include <mithrilQt/class.hpp>
#include <mithrilQt/directory.hpp>
#include <mithrilQt/document.hpp>
#include <mithrilQt/doodle.hpp>
#include <mithrilQt/entity.hpp>
#include <mithrilQt/event.hpp>
#include <mithrilQt/field.hpp>
#include <mithrilQt/folder.hpp>
#include <mithrilQt/fragment.hpp>
#include <mithrilQt/game.hpp>
#include <mithrilQt/graphviz.hpp>
#include <mithrilQt/group.hpp>
#include <mithrilQt/image.hpp>
#include <mithrilQt/leaf.hpp>
#include <mithrilQt/meta.hpp>
#include <mithrilQt/novel.hpp>
#include <mithrilQt/organization.hpp>
#include <mithrilQt/place.hpp>
#include <mithrilQt/root.hpp>
#include <mithrilQt/style.hpp>
#include <mithrilQt/tag.hpp>
#include <mithrilQt/thing.hpp>
#include <mithrilQt/user.hpp>
#include <mithrilQt/value.hpp>

#include <mithril/atom/Atom.hpp>
#include <mithril/attribute/Attribute.hpp>
#include <mithril/book/Book.hpp>
#include <mithril/category/Category.hpp>
#include <mithril/character/Character.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/directory/Directory.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/doodle/Doodle.hpp>
#include <mithril/entity/Entity.hpp>
#include <mithril/event/Event.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/folder/Folder.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithril/game/Game.hpp>
#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/group/Group.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/meta/Meta.hpp>
#include <mithril/novel/Novel.hpp>
#include <mithril/organization/Organization.hpp>
#include <mithril/place/Place.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/style/Style.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/thing/Thing.hpp>
#include <mithril/user/User.hpp>
#include <mithril/value/Value.hpp>
#include <mithrilQt/id.hpp>
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
