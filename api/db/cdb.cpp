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
#include "id_lock.hpp"
#include "bit/nki.hpp"
#include "root/struct.hpp"
#include "workspace.hpp"

#include "class/struct.hpp"
#include "leaf/struct.hpp"

#include "category/cdb.hpp"
#include "class/cdb.hpp"
#include "directory/cdb.hpp"
#include "document/cdb.hpp"
#include "field/cdb.hpp"
#include "folder/cdb.hpp"
#include "fragment/cdb.hpp"
#include "image/cdb.hpp"
#include "root/cdb.hpp"
#include "tag/cdb.hpp"
#include "user/cdb.hpp"
#include "value/cdb.hpp"

#include "id_lock.ipp"

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
    
    // explicit instantiation
    template class IDLock<Category>;
    template class IDLock<Class>;
    template class IDLock<Field>;
    template class IDLock<Fragment>;
    template class IDLock<Leaf>;
    template class IDLock<Tag>;
    template class IDLock<User>;
}


#include "cdb_sq.ipp"
#include "category/cdb.ipp"
#include "class/cdb.ipp"
#include "directory/cdb.ipp"
#include "document/cdb.ipp"
#include "field/cdb.ipp"
#include "folder/cdb.ipp"
#include "fragment/cdb.ipp"
#include "image/cdb.ipp"
#include "root/cdb.ipp"
#include "tag/cdb.ipp"
#include "user/cdb.ipp"
#include "value/cdb.ipp"

//#include "image_cdb.ipp"
//#include "tag_cdb.ipp"
//#include "user_cdb.ipp"

