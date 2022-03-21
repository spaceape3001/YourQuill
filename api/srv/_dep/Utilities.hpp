////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/DataRole.hpp>
#include <db/enum/SizeDesc.hpp>

class QByteArray;

namespace yq {

    class Class;
    class Root;

    bool            auto_edit();
    String          cur_user();
    const Root*     def_root(DataRole dr = DataRole());
    Vector<Class>   get_class_vector(const StringSet& sset);
    SizeDesc        icon_size();
    bool            inspect_submit();
    bool            is_mobile();
    QByteArray      reencode_parameters();
    void            return_to_sender();
    bool            valid_key(const QByteArray&);

}



