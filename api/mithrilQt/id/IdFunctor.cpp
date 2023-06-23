////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdFunctor.hpp"

#include <gluon/core/Utilities.hpp>

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/book/BookCDB.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/character/CharacterCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/event/EventCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/game/GameCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/place/PlaceCDB.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/tag/TagCDB.hpp>

#include <mithrilQt/atom/AtomColumn.hpp>
#include <mithrilQt/book/BookColumn.hpp>
#include <mithrilQt/category/CategoryColumn.hpp>
#include <mithrilQt/character/CharacterColumn.hpp>
#include <mithrilQt/class/ClassColumn.hpp>
#include <mithrilQt/directory/DirectoryColumn.hpp>
#include <mithrilQt/document/DocumentColumn.hpp>
#include <mithrilQt/event/EventColumn.hpp>
#include <mithrilQt/field/FieldColumn.hpp>
#include <mithrilQt/folder/FolderColumn.hpp>
#include <mithrilQt/fragment/FragmentColumn.hpp>
#include <mithrilQt/game/GameColumn.hpp>
#include <mithrilQt/leaf/LeafColumn.hpp>
#include <mithrilQt/place/PlaceColumn.hpp>
#include <mithrilQt/root/RootColumn.hpp>
#include <mithrilQt/tag/TagColumn.hpp>

#include <QIcon>

using namespace yq::gluon;

namespace yq::mithril::column {
    IdColumn                id_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::id_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::id_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn                id_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::id_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::id_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn                id_label(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::id_label();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::id_icon();
        ret.label               = "Label";
        return ret;
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN     id_icon()
    {
        return [](Id i) -> QVariant {
            static QIcon    s_ico(":/generic/unknown.svg");
            switch(i.type()){
            case IdType::Atom:
                return qIcon(Atom(i.id()));
            case IdType::Book:
                return qIcon(Book(i.id()));
            case IdType::Category:
                return qIcon(Category(i.id()));
            case IdType::Class:
                return qIcon(Class(i.id()));
            case IdType::Directory:
                return qIcon(Directory(i.id()));
            case IdType::Document:
                return qIcon(Document(i.id()));
            case IdType::Event:
                return qIcon(Event(i.id()));
            case IdType::Field:
                return qIcon(Field(i.id()));
            case IdType::Folder:
                return qIcon(Folder(i.id()));
            case IdType::Fragment:
                return qIcon(Fragment(i.id()));
            case IdType::Game:
                return qIcon(Game(i.id()));
            case IdType::Leaf:
                return qIcon(Leaf(i.id()));
            case IdType::Place:
                return qIcon(Place(i.id()));
            case IdType::Root:
                return qIcon(Root(i.id()));
            case IdType::Tag:
                return qIcon(Tag(i.id()));
            default:
                return s_ico;
            }
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     id_id()
    {
        return [](Id i) -> QVariant {
            return (quint64) i.id();
        };
    }
    
    IdColumn::VariantFN     id_key()
    {
        return [](Id i) -> QVariant {
            switch(i.type()){
            case IdType::Atom:
                return qString(cdb::key(Atom(i.id())));
            case IdType::Book:
                return qString(cdb::key(Book(i.id())));
            case IdType::Category:
                return qString(cdb::key(Category(i.id())));
            case IdType::Class:
                return qString(cdb::key(Class(i.id())));
            case IdType::Directory:
                return qString(cdb::key(Directory(i.id())));
            case IdType::Document:
                return qString(cdb::key(Document(i.id())));
            case IdType::Event:
                return qString(cdb::key(Event(i.id())));
            case IdType::Field:
                return qString(cdb::key(Field(i.id())));
            case IdType::Folder:
                return qString(cdb::key(Folder(i.id())));
            case IdType::Fragment:
                return qString(cdb::key(Fragment(i.id())));
            case IdType::Game:
                return qString(cdb::key(Game(i.id())));
            case IdType::Leaf:
                return qString(cdb::key(Leaf(i.id())));
            case IdType::Place:
                return qString(cdb::key(Place(i.id())));
            case IdType::Root:
                return qString(cdb::key(Root(i.id())));
            case IdType::Tag:
                return qString(cdb::key(Tag(i.id())));
            default:
                return QString("??");
            }
        };
    }
    
    IdColumn::VariantFN     id_label()
    {
        return [](Id i) -> QVariant {
            switch(i.type()){
            case IdType::Atom:
                return qString(cdb::label(Atom(i.id())));
            case IdType::Book:
                return qString(cdb::key(Book(i.id())));         // TODO
            case IdType::Category:
                return qString(cdb::label(Category(i.id())));
            case IdType::Class:
                return qString(cdb::label(Class(i.id())));
            case IdType::Directory:
                return qString(cdb::key(Directory(i.id())));    // TODO
            case IdType::Document:
                return qString(cdb::label(Document(i.id())));
            case IdType::Event:
                return qString(cdb::key(Event(i.id())));        // TODO
            case IdType::Field:
                return qString(cdb::label(Field(i.id())));
            case IdType::Folder:
                return qString(cdb::label(Folder(i.id())));
            case IdType::Fragment:
                return qString(cdb::label(Fragment(i.id())));
            case IdType::Game:
                return qString(cdb::key(Game(i.id())));         // TODO
            case IdType::Leaf:
                return qString(cdb::label(Leaf(i.id())));
            case IdType::Place:
                return qString(cdb::key(Place(i.id())));        // TODO
            case IdType::Root:
                return qString(cdb::key(Root(i.id())));         // TODO
            case IdType::Tag:
                return qString(cdb::key(Tag(i.id())));          // TODO
            default:
                return QString("??");
            }
        };
    }
}

