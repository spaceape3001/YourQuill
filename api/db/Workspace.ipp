////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


String       Workspace::dot;
String       Workspace::git;
String       Workspace::hostname;
String       Workspace::markdown;
String       Workspace::smartypants;
String       Workspace::perl;
String       Workspace::subversion;
StringSet    Workspace::available;  // templates


bool         Workspace::sinit(()
{
    if(!makeTempDirectories())
        return false;
    
    
    return false;    
}
