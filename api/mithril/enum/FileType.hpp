////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>

namespace yq {

    namespace mithril {

        /*! \brief File Type 
        */
        YQ_ENUM(FileType, ,
            Default,
            KeyValue,
            Xml
        )
    }
}

YQ_TYPE_DECLARE(yq::mithril::FileType)
