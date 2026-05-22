////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/net/Curl.hpp>

//  spot to disable (to test) the vulkan
// #undef YIDE_VULKAN


#ifdef YIDE_VULKAN
namespace yq::tachyon {
    class TEManager;
    class TypedID;
}
#endif

using namespace yq;
using namespace yq::tachyon;

extern const Url&           gHomeUrl;
extern const uint16_t&      gPort;

#ifdef YIDE_VULKAN
extern TEManager* const&    gTEManager;       
extern const TypedID&       gFileIO;
#endif
