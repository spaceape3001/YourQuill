#pragma once

namespace {
    CommonDir               dirs;
}
const CommonDir&     gDir   = dirs;


namespace {
    static constexpr const char*    kBuildDir   = YQ_BUILD_ROOT;
    static constexpr const char*    kShareDir   = YQ_SHARE_DIR;
    

    const char* homeDir()
    {
        const char* s   = getenv("HOME");
        if(s)
            return s;
        struct passwd*  pw = getpwuid(getuid());;
        if(pw)
            return pw -> pw_dir;
        return nullptr;
    }

    bool    initDirs_impl()
    {
        using namespace std::filesystem;
        
       //  Make the basics, which, for now, is hardcoded
       
        const char*   tmpdir  = getenv("TMPDIR");
        if(!tmpdir)
            tmpdir          = "/tmp";

        dirs.build      = kBuildDir;
        dirs.tmp        = path(tmpdir) / "yquill";
        dirs.log        = dirs.tmp / "logs";
        dirs.ipc        = dirs.tmp / "ipc";
        dirs.ini        = dirs.tmp / "ini";
        dirs.pid        = dirs.tmp / "pid";
        dirs.cache      = dirs.tmp / "cache";
        path  lHomeDir;

        Vector<path>  ndirs({ dirs.tmp, dirs.log, dirs.ipc, dirs.cache, dirs.pid, dirs.ini });

        //  And user home dir...
        const char*     hdir    = homeDir();
        if(hdir && ::access(hdir, W_OK)){
            lHomeDir    = path(hdir) / ".yquill";
            ndirs << hdir;
        }
        
        
        bool    okay    = true;
        for(const path& s : ndirs){
            if(mkdir(s.c_str(), 0755) && (errno != EEXIST)){
                yError() << "Unable to create directory: " << s;
                okay    = false;
            }
        }
        
        //  share directories....
        if((!lHomeDir.empty()) && !access(lHomeDir.c_str(), R_OK)){
            yInfo() << "Adding home directory " << lHomeDir;
            dirs.share << lHomeDir;
        }
            
        for(const char* z : { "/usr/local/share/yquill", "/usr/share/yquill", kShareDir }){
            if(!access(z, R_OK))
                dirs.share << z;
        }
        
        return okay;
    }
}

const char* buildDir()
{
    return YQ_BUILD_ROOT;
}


bool     CommonDir::init()
{
    static bool ret = initDirs_impl();
    return ret;
}

std::filesystem::path              shared(const std::filesystem::path&sv)
{
    for(const std::filesystem::path& s : dirs.share){
        std::filesystem::path  s2  = s / sv;
        if(file_exists(s2.c_str()))
            return s2;
    }
    return std::filesystem::path();
}

Vector<std::filesystem::path>      shared_all(const std::filesystem::path&sv)
{
    Vector<std::filesystem::path>  ret;
    for(const std::filesystem::path& s : dirs.share){
        std::filesystem::path  s2  = s / sv;
        if(file_exists(s2.c_str()))
            ret << s2;
    }
    return ret;
}



