////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

namespace yq {

    /*! \brief File Type 
    */
    YQ_ENUM(FileType, ,
        Default,
        KeyValue,
        Xml
    )
}

YQ_TYPE_DECLARE(yq::FileType)
