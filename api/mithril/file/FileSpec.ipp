////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileSpec.hpp"

#include <io/FileUtils.hpp>
#include <basic/TextUtils.hpp>
#include <mithril/kernel/atom/ClassCDB.hpp>
#include <mithril/kernel/file/Folder.hpp>
#include <mithril/kernel/file/FragmentCDB.hpp>

namespace yq::mithril {
    FileSpec        by_file(const std::filesystem::path&fp)
    {
        FileSpec    ret;
        ret.type    = FileSpec::ByFile;
        ret.str     = fp.string();
        return ret;
    }
    
    FileSpec        by_cache(std::string_view sv)
    {
        FileSpec    ret;
        if(sv.empty()){
            ret.type    = FileSpec::Never;
        } else if(sv == "*"){
            ret.type    = FileSpec::Always;
        } else if(starts(sv, "*.")){
            ret.type    = FileSpec::ByCacheExt;
            ret.str     = sv.substr(2);
        } else {
            ret.type    = FileSpec::ByCacheDoc;
            ret.str     = sv;
        }
        return ret;
    }

    FileSpec        by_cache(Folder f)
    {
        FileSpec    ret;
        ret.type    = FileSpec::ByFolder;
        ret.u64     = f.id;
        return ret;
    }
    
        
    FileSpec        by_cache(Folder f, std::string_view sv)
    {
        FileSpec    ret = by_cache(sv);
        ret.u64     = f.id;
        switch(ret.type){
        case FileSpec::Always:
            ret.type    = FileSpec::ByFolder;
            break;
        case FileSpec::ByCacheExt:
            ret.type    = FileSpec::ByFolderExt;
            break;
        case FileSpec::ByCacheDoc:
            ret.type    = FileSpec::ByFolderDoc;
            break;
        default:
            break;
        }
        return ret;
    }
    
    //FileSpec        by_class(std::string_view sv)
    //{
        //FileSpec    ret;
        //ret.type        = FileSpec::ByClassKey;
        //ret.str         = sv;
        //return ret;
    //}

    //FileSpec        by_class(Class c)
    //{
        //FileSpec    ret;
        //ret.type    = FileSpec::ByClassId;
        //ret.u64     = c.id;
        //return ret;
    //}

    bool    FileSpec::match(Fragment frag) const
    {
        return match(cdb::folder(frag), cdb::name(frag));
    }

    bool    FileSpec::match(Folder fo, std::string_view k) const
    {
        switch(type){
        case Never:
            return false;
        case Always:
            return true;
        case ByFile:
            return false;
        case ByCacheDoc:
            return is_similar(k, str);
        case ByExtension:
        case ByCacheExt:
            return is_similar(k, file_extension(k));
        case ByFolder:
            return fo.id == u64;
        case ByFolderExt:
            return (fo.id == u64) && is_similar(k, file_extension(str));
        case ByFolderDoc:
            return (fo.id == u64) && is_similar(k, str);
        default:
            return false;
        }
    }
    
    bool    FileSpec::match(const std::filesystem::path& path) const
    {
        std::string     file    = path.string();
        switch(type){
        case Never:
            return false;
        case Always:
            return true;
        case ByFile:
            #ifdef WIN32
            return is_similar(file, str);
            #else
            return file == str;
            #endif
        case ByExtension:
            return is_similar(file_extension(file), str);
        case ByCacheDoc:
        case ByCacheExt:
        case ByFolder:
        case ByFolderDoc:
        case ByFolderExt:
            return match(cdb::fragment(path));
        default:
            return false;
        }
    }
    
}

