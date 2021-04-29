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
       //  Make the basics, which, for now, is hardcoded
       
        const char*   tmpdir  = getenv("TMPDIR");
        if(!tmpdir)
            tmpdir          = "/tmp";


        dirs.tmp        = String(tmpdir) + "/yquill";
        dirs.log        = dirs.tmp  + "/logs";
        dirs.ipc        = dirs.tmp + "/ipc";
        dirs.ini        = dirs.tmp + "/ini";
        dirs.pid        = dirs.tmp + "/pid";
        dirs.cache      = dirs.tmp + "/cache";
        String  lHomeDir;

        Vector<String>  ndirs({ dirs.tmp, dirs.log, dirs.ipc, dirs.cache, dirs.pid, dirs.ini });

        //  And user home dir...
        const char*     hdir    = homeDir();
        if(hdir && ::access(hdir, W_OK)){
            lHomeDir    = String(hdir) + "/.yquill";
            ndirs << hdir;
        }
        
        
        bool    okay    = true;
        for(const String& s : ndirs){
            if(mkdir(s.c_str(), 0755) && (errno != EEXIST)){
                yError() << "Unable to create directory: " << s;
                okay    = false;
            }
        }
        
        //  share directories....
        if(hdir && access(lHomeDir.c_str(), R_OK))
            dirs.share << lHomeDir;
            
        for(const char* z : { kShareDir, "/usr/local/share/yquill", "/usr/share/yquill" })
            if(access(z, R_OK))
                dirs.share << String(z);
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

