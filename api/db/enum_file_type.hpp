////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/type/Enum.hpp>

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
