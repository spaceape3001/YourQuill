////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/collection/vector_utils.hpp>
#include <yq/file/dir_utils.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/type/ByteArray.hpp>

#include "cdb_sq.hpp"
#include "nki.hpp"
#include "root.hpp"
#include "workspace.hpp"


#include "directory_cdb.hpp"
#include "document_cdb.hpp"
#include "folder_cdb.hpp"
#include "fragment_cdb.hpp"
#include "image_cdb.hpp"
#include "root_cdb.hpp"
#include "tag_cdb.hpp"
#include "user_cdb.hpp"

namespace yq {
    namespace cdb {
        namespace {
            static constexpr const uint64_t     kMaxFixedFolder = Folder::USERS;

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

        std::string_view         base_key(std::string_view key)
        {
            size_t i   = key.find_last_of('/');
            if(i == std::string_view::npos){
                size_t j   = key.find_first_of('.');
                if(j == std::string_view::npos)
                    return key;
                return key.substr(0,j);
            } else {
                size_t j   = key.find_first_of('.', i);
                if(j == std::string_view::npos)
                    return key.substr(i+1);
                return key.substr(i+1,j-i-1);
            }
        }
    }
}

#include "cdb_sq.ipp"
#include "directory_cdb.ipp"
#include "document_cdb.ipp"
#include "folder_cdb.ipp"
#include "fragment_cdb.ipp"
#include "image_cdb.ipp"
#include "root_cdb.ipp"
#include "tag_cdb.ipp"
#include "user_cdb.ipp"

//#include "image_cdb.ipp"
//#include "tag_cdb.ipp"
//#include "user_cdb.ipp"

