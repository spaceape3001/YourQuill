////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CacheFwd.hpp"


/*! \brief Attribute of documents
*/
struct Attribute {
    uint64_t    id  = 0ULL;
    constexpr auto    operator<=>(const Attribute&rhs) const = default; 
    constexpr operator bool() const { return id != 0ULL; }
};

namespace cdb {


    //! Gets attributes (sub)
    Vector<Attribute>   attributes(Attribute);

    //! Gets top-level attirbutes
    Vector<Attribute>   attributes(Document);
    
    size_t              attributes_count(Attribute);
    size_t              attributes_count(Document);

    Document            document(Attribute);
    
    bool                exists(Attribute);
    bool                exists_attribute(uint64_t);

    //! Index in the file's list
    uint64_t            index(Attribute);

    QString             key(Attribute);
    
    DocOrAttr           parent(Attribute);
    
    //! User defined ID (from the file)
    QString             uid(Attribute);
    QString             value(Attribute);
}
