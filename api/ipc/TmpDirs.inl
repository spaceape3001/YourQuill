#pragma once

String      gTmpRoot;
String      gLogDir;
String      gIpcDir;
String      gPidDir;
String      gCacheDir;


bool    makeTempDirectories()
{
   //  Make the basics, which, for now, is hardcoded
   
    const char*   tmpdir  = getenv("TMPDIR");
    if(!tmpdir)
        tmpdir          = "/tmp";


    gTmpRoot        = std::string(tmpdir) + "/yquill";
    gLogDir         = gTmpRoot + "/logs";
    gIpcDir         = gTmpRoot + "/ipc";
    gPidDir         = gTmpRoot + "/pid";
    gCacheDir       = gTmpRoot + "/cache";

    Vector<String>  dirs({ gTmpRoot, gLogDir, gIpcDir, gCacheDir, gPidDir });
    
    bool    okay    = true;
    for(const String& s : dirs){
        if(mkdir(s.c_str(), 0755) && (errno != EEXIST))
            yError() << "Unable to create directory: " << s;
    }
    return okay;
}
