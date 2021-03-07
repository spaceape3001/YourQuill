////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/DataRole.hpp>
#include <util/Http.hpp>
#include <QByteArray>
#include <initializer_list>

struct TypedBytes {
    QByteArray      content;
    ContentType     type;
    
    TypedBytes(){}
    TypedBytes(std::initializer_list<QString>,ContentType ct=ContentType());
    TypedBytes(DataRole, std::initializer_list<QString>,ContentType ct=ContentType());
    
    bool    empty() const { return content.isEmpty(); }
};
