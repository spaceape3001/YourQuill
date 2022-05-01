////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <yq/file/XmlUtils.hpp>

#include <db/enum.hpp>
#include <db/core/cdb_common.hpp>
#include <db/core/id_lock.ipp>
#include <db/core/strings.hpp>

#include <yq/meta/TypeInfo.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>

namespace yq {
    
    bool    IDLockRepo::inc_read(uint64_t i)
    {
        tbb::spin_mutex::scoped_lock    _lock(mutex);
        auto& d = locks[i];
        if(d.count == 0xFF)
            return false;
        ++d.count;
        return true;
    }
    
    bool    IDLockRepo::get_write(uint64_t i)
    {
        tbb::spin_mutex::scoped_lock    _lock(mutex);
        return locks.insert({ i, { 0xFF } }).second;
    }
    
    void    IDLockRepo::free_read(uint64_t i)
    {
        tbb::spin_mutex::scoped_lock    _lock(mutex);
        auto d = locks.find(i);
        if(d != locks.end()){
            if(!--(d->second.count))
                locks.erase(i);
        }
    }
    
    void    IDLockRepo::free_write(uint64_t i)
    {
        tbb::spin_mutex::scoped_lock    _lock(mutex);
        locks.erase(i);
    }

    namespace cdb {
        std::string make_filename(std::string_view k, const char* ext)
        {
            std::string     ret;
            ret.reserve(2+k.size()+strlen(ext));
            ret = k;
            ret += '.';
            ret += ext;
            return ret;
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
        
        std::set<uint64_t>      ids_for(const std::vector<const TypeInfo*>&types)
        {
            std::set<uint64_t>  ret;
            for(const TypeInfo* ti : types){
                if(!ti)
                    continue;
                ret.insert(ti->id());
            }
            return ret;
        }
        
    }
}


#include <db/bit/authentication.ipp>
#include <db/bit/context.ipp>
#include <db/bit/copyright.ipp>
#include <db/bit/key_value.ipp>
#include <db/bit/policy.ipp>
#include <db/bit/rule.ipp>
#include <db/core/cdb_sq.ipp>
#include <db/core/std_file.ipp>
#include <db/core/std_object.ipp>

#include <db/enum/size_desc_arg.ipp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::Access)
YQ_TYPE_IMPLEMENT(yq::AssertDeny)
YQ_TYPE_IMPLEMENT(yq::AttrKind)
YQ_TYPE_IMPLEMENT(yq::CalSystem)
YQ_TYPE_IMPLEMENT(yq::Change)
YQ_TYPE_IMPLEMENT(yq::DataRole)
YQ_TYPE_IMPLEMENT(yq::FileType)
YQ_TYPE_IMPLEMENT(yq::Format)
YQ_TYPE_IMPLEMENT(yq::Linkage)
YQ_TYPE_IMPLEMENT(yq::Multiplicity)
YQ_TYPE_IMPLEMENT(yq::Permission)
YQ_TYPE_IMPLEMENT(yq::Permissions)
YQ_TYPE_IMPLEMENT(yq::Reset)
YQ_TYPE_IMPLEMENT(yq::Restriction)
YQ_TYPE_IMPLEMENT(yq::SizeDesc)
YQ_TYPE_IMPLEMENT(yq::Submit)
YQ_TYPE_IMPLEMENT(yq::Vcs)
