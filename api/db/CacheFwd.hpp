////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>
#include <utility>
#include <variant>

template <typename> class Vector;

class QByteArray;
class QString;

struct Atom;
struct Attribute;
struct Class;
struct Directory;
struct Document;
struct Field;
struct Folder;
struct Fragment;
struct Graph;
struct Image;
struct Leaf;
struct Tag;

class Root;

namespace cdb {

    enum {
        //! Consider this an "update" so reset the fragment to "updated" 
        //! \note Only the scanner should be using this, may cause issues for otherrs to set this
        IsUpdate                = 0x1,
        //! Ignore attributes
        IgnoreAttributes        = 0x2,
        //! Ignore context
        IgnoreContext           = 0x4,
        
        Override                = 0x8,
        
        AllowEmpty              = 0x10
    };


    struct NKI;

    using DirOrFrag     = std::variant<bool,Directory,Fragment>;
    using DocOrAttr     = std::variant<bool,Document,Attribute>;
    using DocOrFold     = std::variant<bool,Document,Folder>;
    using DirOrFragStr  = std::pair<DirOrFrag,QString>;
    using DocOrFoldStr  = std::pair<DocOrFold,QString>;

    using DirString     = std::pair<Directory,QString>;
    using DocString     = std::pair<Document,QString>;
    using FoldString    = std::pair<Folder,QString>;
    using FragString    = std::pair<Fragment,QString>;

    using ClassPair     = std::pair<Class,Class>;
}


