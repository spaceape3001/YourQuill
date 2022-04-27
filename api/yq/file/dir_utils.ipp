////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "dir_utils.hpp"

#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/text/text_utils.hpp>

#include <sys/stat.h>
#include <dirent.h>

namespace yq {

    Vector<std::string>  dir_files(const char* iDir)
    {
        Vector<std::string>  ret;
        DIR*            d   = opendir(iDir);
        if(d){
            while(struct dirent* r = readdir(d)){
                if(r -> d_type != DT_REG)
                    continue;
                ret << std::string(r->d_name);
            }
            closedir(d);
        }
        return ret;
    }

    Vector<std::string>  dir_directories(const char* iDir)
    {
        Vector<std::string>  ret;
        DIR*            d   = opendir(iDir);
        if(d){
            while(struct dirent* r = readdir(d)){
                if(r -> d_type != DT_DIR)
                    continue;
                if(r->d_name[0] == '.'){
                    if(r->d_name[1] == '.' || r->d_name[1] == '\0')
                        continue;
                }
                ret << std::string(r->d_name);
            }
            closedir(d);
        }
        return ret;
    }



    //  Make the path (including the specified filename)
    bool            make_path(const std::filesystem::path& p, mode_t dirMode)
    {
        if(::access(p.c_str(), X_OK) == 0)  // it exists
            return true;
        std::filesystem::path   pp  = p.parent_path();
        if((pp != p) && !make_path(pp, dirMode))
            return false;
        return !mkdir(p.c_str(), dirMode);
    }

    //  Make the path (not including the specified filename)
    bool            make_parent_path(const std::filesystem::path&p, mode_t dirMode)
    {
        return make_path(p.parent_path());
    }


    namespace dir {

        // FIXME .... none of these deal with sub-directories!

        path_vec        all_children(const path& d, std::string_view name)
        {
            path_vec    ret;
            for_all_children(d, 0, [&](const path& p) { 
                if(is_similar(p.filename().c_str(), name))
                    ret << p; 
            });
            return ret;
        }

        path_vec        all_children(const std::vector<path>& dirs, std::string_view name)
        {
            path_vec    ret;
            for_all_children(dirs, 0, [&](const path& p) { 
                if(is_similar(p.filename().c_str(), name))
                    ret << p; 
            });
            return ret;
        }
        
        path            first_child(const path& d, std::string_view name)
        {
            // FIXME WARNING... paths....
            return for_all_children( d, 0, [&](const path&p) -> auto {
                //yInfo() << "   Checking " << d << " '" << p << "' for " << name;
                return is_similar(p.filename().c_str(), name) ? p : path();
            });
        }
        
        path            first_child(const std::vector<path>& d, std::string_view name)
        {
            // FIXME WARNING... paths.... 
            return for_all_children( d, 0, [&](const path&p) -> auto {
                //yInfo() << "   Checking " << p << " for " << name;
                return is_similar(p.filename().c_str(), name) ? p : path();
            });
        }

        
        Vector<std::string>  subdirectory_names(const path&d, unsigned options) 
        {
            Vector<std::string>  ret;
            for_all_children(d, options | NO_FILES, [&](const path&p){
                ret << p.filename().string();
            });
            return ret;
        }
        
        Vector<std::string>  subdirectory_names(const std::vector<path>&dirs, unsigned options) 
        {
            Vector<std::string>  ret;
            for_all_children(dirs, options | NO_FILES, [&](const path&p){
                ret << p.filename().string();
            });
            return ret;
        }


        //  Finds all chlidren (not hidden)
        StringSet       subdirectory_names_set(const path&d, unsigned options) 
        {
            StringSet   ret;
            for_all_children(d, options | NO_FILES, [&](const path&p){
                ret << p.filename().string();
            });
            return ret;
        }
        
        StringSet       subdirectory_names_set(const std::vector<path>&dirs, unsigned options) 
        {
            StringSet   ret;
            for_all_children(dirs, options | NO_FILES, [&](const path&p){
                ret << p.filename().string();
            });
            return ret;
        }


        //  Finds all chlidren (not hidden)
        path_vec        subdirectories(const path&d, unsigned options)
        {
            path_vec    ret;
            for_all_children(d, options | NO_FILES, [&](const path&p){
                ret << p;
            });
            return ret;
        }
        
        path_vec        subdirectories(const std::vector<path>&dirs, unsigned options)
        {
            path_vec    ret;
            for_all_children(dirs, options | NO_FILES, [&](const path&p){
                ret << p;
            });
            return ret;
        }

        bool            child_exists(const path&d, std::string_view name)
        {
            return for_all_children(d, 0, [&](const path&p) -> bool {
                return is_similar(p.filename().c_str(), name);
            });
        }

        bool            child_exists(const std::vector<path>&dirs, std::string_view name)
        {
            return for_all_children(dirs, 0, [&](const path&p) -> bool {
                return is_similar(p.filename().c_str(), name);
            });
        }
    }
}