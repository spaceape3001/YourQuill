////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/type/ByteArray.hpp>
#include <yq/wksp/CacheFwd.hpp>
#include <yq/wksp/CacheSQ.hpp>
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>

namespace yq {

    namespace {
        inline std::string make_filename(std::string_view k, const char* ext)
        {
            std::string     ret;
            ret.reserve(2+k.size()+strlen(ext));
            ret = k;
            ret += '.';
            ret += ext;
            return ret;
        }
    }
}

#include "tag_cdb.ipp"
#include "user_cdb.ipp"
