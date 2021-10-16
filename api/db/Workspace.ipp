////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


namespace {
    Workspace           sWksp;
    
    std::filesystem::path   absolute_proximate(const String& spec, const std::filesystem::path& base)
    {
        std::error_code ec1, ec2;
        return std::filesystem::absolute(std::filesystem::proximate(spec.c_str(), base, ec1), ec2);
    }
}

const Workspace&    gWksp   = sWksp;

std::filesystem::path       Workspace::best_guess(const std::filesystem::path& sv)
{
    using namespace std::filesystem;
    if(is_similar(sv.filename().c_str(), szQuillFile))
        return sv;
    path    rt  = sv.root_path();
    for(path q = is_directory(sv) ? sv : q.parent_path(); q != rt; q = q.parent_path()){
        path    f   = q / szQuillFile;
        if(exists(f))
            return f;
    }
    return path();  // failed to find.... abort
}


bool                Workspace::load(const std::filesystem::path& spec, unsigned opts)
{
    using namespace std::filesystem;
    if(!CommonDir::init())
        return false;
        
    qspec               = spec;
    path   q(spec);
    if(q.is_relative()){
        char        cwd[PATH_MAX+1];
        if(getcwd(cwd, PATH_MAX) != cwd)
            return false;
        cwd[PATH_MAX]   = '\0';
        q   = path(cwd) / spec.c_str();
    }
    q = q.lexically_normal();
    
    char        hname[HOST_NAME_MAX+1];
    gethostname(hname, sizeof(hname));
    hname[HOST_NAME_MAX]    = '\0';
    host            = String(hname);
    
    //  Right now, hardcode these
    dot             = "/usr/bin/dot";
    git             = "/usr/bin/git";
    perl            = "/usr/bin/perl";
    subversion      = "/usr/bin/svn";

    markdown        = shared("perl/Markdown.pl");
    smartypants     = shared("perl/SmartyPants.pl");

        //  While we have Qt, this will remain.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    
    if(opts & SEARCH){
        qfile   = best_guess(q);
    } else {
        if(is_similar(q.filename().c_str(), szQuillFile)){
            qfile   = q;
        } else if(is_directory(q)){
            qfile   = q / szQuillFile;
        }
    }
    
    if(qfile.empty()){
        wkspError << "No " << szQuillFile << " fragment at specified location, REQUIRED!";
        return false;
    }
    

    QuillFile       doc;
    if(!doc.load(qfile)){
        wkspError << "Unable to load the file: " << qfile;
        return false;
    }
    
    qdir            = qfile.parent_path();
    qkey            = qdir.filename().c_str();
    if(doc.temp_dir.empty()){
        tmp = gDir.tmp / qkey.c_str();
    } else 
        tmp = absolute_proximate(doc.temp_dir, qdir);
    
    
    //  TODO (LATER)


    return true;

}

bool                Workspace::init(const std::filesystem::path& spec, unsigned opts)
{
    static bool ret = sWksp.load(spec, opts);
    return ret;
}


