////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/core/Enum.hpp>

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
