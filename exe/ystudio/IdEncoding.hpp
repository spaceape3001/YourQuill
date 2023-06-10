////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>

/*
    Given the 64-bit integers are way more than we'll likely use, we'll encode the 
    above into 64-bit integers, stealing the high byte.
*/

enum class MType : uint8_t {
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


namespace impl {
    template <typename> struct mtype_deducer : std::integral_constant<MType,MType::Unknown> {};

    #define mtype(name)    \
        template <> struct mtype_deducer<yq::mithril::name> : std::integral_constant<MType,MType::name> {};
    
    mtype(Atom)
    mtype(Attribute)
    mtype(Book)
    mtype(Category)
    mtype(Character)
    mtype(Class)
    mtype(Directory)
    mtype(Document)
    mtype(Edge)
    mtype(Entity)
    mtype(Event)
    mtype(Field)
    mtype(Folder)
    mtype(Fragment)
    mtype(Graph)
    mtype(Group)
    mtype(Image)
    mtype(Leaf)
    mtype(Place)
    mtype(Property)
    mtype(Tag)
    mtype(Thing)
    mtype(User)
    
    template <> struct mtype_deducer<const yq::mithril::RootDir*> : std::integral_constant<MType,MType::Root> {};
    
    #undef mtype
}

template <typename T>
static constexpr const MType    mtype_v = impl::mtype_deducer<T>::value;

inline constexpr uint64_t u64_encode(MType type, uint64_t id) noexcept
{
    return (((uint64_t) type) << 56ULL) | (id & 0x00FF'FFFF'FFFF'FFFFULL);
}

inline constexpr uint64_t u64_id(uint64_t u64) noexcept
{
    return u64 & 0x00FF'FFFF'FFFF'FFFFULL;
}

inline constexpr MType u64_type(uint64_t u64) noexcept
{
    return (MType) (u64 >> 56ULL);
}

template <yq::mithril::cdb_object C>
constexpr uint64_t  u64(C c) noexcept
{
    return u64_encode(mtype_v<C>, c.id);
}

uint64_t    u64(const yq::mithril::RootDir* rt);
const yq::mithril::RootDir* u64_rootdir(uint64_t);

template <yq::mithril::cdb_object C>
constexpr C u64(uint64_t u) noexcept
{
    if(u64_type(u) == mtype_v<C>)
        return C(u64_id(u));
    return C(0ULL);
}
