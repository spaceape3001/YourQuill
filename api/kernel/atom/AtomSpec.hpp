////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace yq {
    struct Folder;
    struct Class;
    struct Fragment;
    
    struct AtomSpec {
        enum Type {
            Never    = 0,       //<! Never detect
            Always,             //<! Always detect
            ByClassKey,         //!< By atom class (key)
            ByClassId,          //!< By atom class (ID)
            ByFieldKey,
            ByFieldId
        };
    
        std::string             str;
        uint64_t                u64     = 0;
        Type                    type    = Never;
        
        bool    is_class() const;
        bool    is_field() const;
        
        Class   class_() const;
        Field   field() const;
        
        
        bool    match(Class) const;
        bool    match(Field) const;
        bool    match_field(std::string_view) const;
        bool    match_class(std::string_view) const;
    };
    
    AtomSpec        by_class(std::string_view);
    AtomSpec        by_class(Class);
    AtomSpec        by_field(std::string_view);
    AtomSpec        by_field(Field);
}

