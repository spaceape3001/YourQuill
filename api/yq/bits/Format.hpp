#pragma once

#include "yq/util/Enum.hpp"
#include "yq/meta/MetaValue.hpp"


YQ_ENUM(Format, , 
    Markdown, 
    ASCII, 
    Plain, 
    Html, 
    Novel 
)

MV_DECLARE(Format)
