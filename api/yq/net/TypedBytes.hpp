////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "yq/enum/DataRole.hpp"
#include "yq/io/Http.hpp"

#include <QByteArray>


struct TypedBytes {
    QByteArray      content;
    ContentType     type;
    
    TypedBytes(){}
    TypedBytes(std::initializer_list<QString>,ContentType ct=ContentType());
    TypedBytes(DataRole, std::initializer_list<QString>,ContentType ct=ContentType());
    
    bool    empty() const { return content.isEmpty(); }
};
