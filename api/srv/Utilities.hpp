////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/DataRole.hpp>
#include <db/enum/SizeDesc.hpp>
#include <util/TypeFwd.hpp>

class Class;
class Root;
class QByteArray;

bool            auto_edit();
QString         cur_user();
const Root*     def_root(DataRole dr = DataRole());
Vector<Class>   get_class_vector(const StringSet& sset);
SizeDesc        icon_size();
bool            inspect_submit();
bool            is_mobile();
QByteArray      reencode_parameters();
void            return_to_sender();
bool            valid_key(const QByteArray&);




