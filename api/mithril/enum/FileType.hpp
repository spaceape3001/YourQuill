////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

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