////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/enum/DataRole.hpp>
#include <yq/Http.hpp>
#include <yq/String.hpp>
#include <initializer_list>

struct TypedBytes {
    String          content;
    ContentType     type;
    
    TypedBytes(){}
    TypedBytes(std::initializer_list<String>,ContentType ct=ContentType());
    TypedBytes(DataRole, std::initializer_list<String>,ContentType ct=ContentType());
    
    bool    empty() const { return content.empty(); }
};
