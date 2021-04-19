////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

bool    makeDirectories()
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

void  makeLogFile()
{
    char        buffer[256];
    time_t      n;  
    time(&n);
    strftime(buffer, sizeof(buffer), "%Y%m%d-%H%M%S", localtime(&n));
    buffer[255] = '\0';
    std::string fname   = gLogDir + "/broker-" + buffer + ".log";
    log_to_file(fname, LogPriority::Debug);
}
