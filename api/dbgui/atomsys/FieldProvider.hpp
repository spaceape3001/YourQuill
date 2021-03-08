////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/FileSys.hpp>
#include <gui/Provider.hpp>

using FieldProvider = std::shared_ptr<Provider<Field>>;

namespace provider {
    FieldProvider       all_fields();
}

