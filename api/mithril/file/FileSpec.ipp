////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileSpec.hpp"

#include <0/io/FileUtils.hpp>

#include <mithril/class/ClassCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/Folder.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/id/Id.hpp>

namespace yq::mithril {
    FileSpec::FileSpec(const std::filesystem::path& fp)
    {
        type    = ByFile;
        str     = fp.string();
    }

    FileSpec::FileSpec(cache_t, std::string_view sv)
    {
        if(sv.empty()){
            type    = Never;
        } else if(sv == "*"){
            type    = Always;
        } else if(starts(sv, "*.")){
            type    = ByCacheExt;
            str     = sv.substr(2);
        } else {
            type    = ByCacheDoc;
            str     = sv;
        }
    }
    
    FileSpec::FileSpec(cache_t, Folder f)
    {
        type    = ByFolder;
        u64     = f.id;
    }
    
    FileSpec::FileSpec(cache_t, Folder f, std::string_view sv) : FileSpec(CACHE, sv)
    {
        u64     = f.id;
        switch(type){
        case Always:
            type    = ByFolder;
            break;
        case ByCacheExt:
            type    = ByFolderExt;
            break;
        case ByCacheDoc:
            type    = ByFolderDoc;
            break;
        default:
            break;
        }
    }
    

    FileSpec        by_file(const std::filesystem::path&fp)
    {
        return FileSpec(fp);
    }
    
    FileSpec        by_cache(std::string_view sv)
    {
        return FileSpec(CACHE, sv);
    }

    FileSpec        by_cache(Folder f)
    {
        return FileSpec(CACHE, f);
    }
    
        
    FileSpec        by_cache(Folder f, std::string_view sv)
    {
        return FileSpec(CACHE, f, sv);
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

    bool    FileSpec::match(Id id) const
    {
        Fragment    frag    = id.as<Fragment>();
        if(frag)
            return match(cdb::folder(frag), cdb::name(frag));
        
        Document    doc     = id.as<Document>();
        if(doc)
            return match(cdb::folder(doc), cdb::name(doc));
            
        return false;
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

