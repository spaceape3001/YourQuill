////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>

class QString;

namespace yq {

    class String;
    using StringPair            = std::pair<String,String>;
    using StringIntPair         = std::pair<String,int>;
    using QStringIntPair        = std::pair<QString,int>;
}
