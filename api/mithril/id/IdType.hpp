////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <basic/Flags.hpp>

namespace yq::mithril {


    /*
        An ID tracking system for those cases where id-types need to be squeezed
        into 64-bits along with their type.  So, for this, we're figuring that
        56-bits for the actual item IDs is more than sufficient, so we strip
        away the upper 8-bits for tracking the type information.
    */

    enum class IdType : uint8_t {
            // list the code types... up to 255
        Unknown = 0,
        Atom,
        Attribute,
        Book,
        Category,
        Character,
        Class,
        Directory,
        Document,
        Edge,
        Entity,
        Event,
        Field,
        Folder,
        Fragment,
        Graph,
        Group,
        Image,
        Leaf,
        Place,
        Property,
        Root,
        Tag,
        Thing,
        User
    };

    template <typename> struct id_type_deducer : std::integral_constant<IdType,IdType::Unknown> {};

    #define idtype(name)    \
        template <> struct id_type_deducer<name> : std::integral_constant<IdType,IdType::name> {};
    
        //  unfortunately, gotta repeat the above list
    idtype(Atom)
    idtype(Attribute)
    idtype(Book)
    idtype(Category)
    idtype(Character)
    idtype(Class)
    idtype(Directory)
    idtype(Document)
    idtype(Edge)
    idtype(Entity)
    idtype(Event)
    idtype(Field)
    idtype(Folder)
    idtype(Fragment)
    idtype(Graph)
    idtype(Group)
    idtype(Image)
    idtype(Leaf)
    idtype(Place)
    idtype(Property)
    idtype(Root)
    idtype(Tag)
    idtype(Thing)
    idtype(User)
    
    #undef idtype

    template <typename T>
    static constexpr const IdType    id_type_v      = id_type_deducer<T>::value;
    
    using IdTypes   = Flags<IdType,uint64_t>;
    
    static constexpr bool   id_compatible(IdType from, IdType to) noexcept
    {
        if(from == to)
            return true;

        switch(from){
        case IdType::Book:
            return to == IdType::Atom;
        case IdType::Category:
            return to == IdType::Document;
        case IdType::Character:
            return (to == IdType::Atom) || (to == IdType::Entity);
        case IdType::Class:
            return to == IdType::Document;
        case IdType::Edge:
            return to == IdType::Atom;
        case IdType::Event:
            return to == IdType::Atom;
        case IdType::Field:
            return to == IdType::Document;
        case IdType::Group:
            return to == IdType::Group;
        case IdType::Image:
            return to == IdType::Fragment;
        case IdType::Leaf:
            return to == IdType::Document;
        case IdType::Place:
            return to == IdType::Atom;
        case IdType::Tag:
            return to == IdType::Document;
        case IdType::User:
            return to == IdType::Document;
        default:
            return false;
        }
    }
}
