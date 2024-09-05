////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/InfoBinder.hpp>
#include <yq-toolbox/basic/Enum.hpp>

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
