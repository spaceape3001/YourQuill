////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    namespace {

        #ifdef WIN32
        using path_set      = Set<String,IgCase>;
        #else
        using path_set      = Set<String>;
        #endif
        
        static constexpr const mode_t   kDirMode        = 0755;
        
        
        std::filesystem::path   absolute_proximate(const String& spec, const std::filesystem::path& base)
        {
            std::error_code ec1, ec2;
            return std::filesystem::absolute(std::filesystem::proximate(spec.c_str(), base, ec1), ec2);
        }
        
        template <typename T>
        struct Spot {
            T           data;
            unsigned    count   = 0;
        };
        
        
        struct QuillSpots {
            Spot<String>        abbr;
            Spot<String>        author;
            Spot<unsigned>      copyfrom;
            Spot<unsigned>      copyto;
            Spot<AssertDeny>    copystance;
            Spot<String>        copytext;
            Spot<String>        home;
            Spot<String>        name;
        };
        
        //template <typename T>
        //struct RT {
            //T               r;
            //T               t;
        //};
        
        enum DirType {
            BadDir,
            ReadDir,
            WriteDir
        };
        
        DirType  dir_type(const std::filesystem::path& fs)
        {
            if(fs.filename().c_str()[0] == '.')  // filter out hidden
                return BadDir;
        
            std::error_code ec;
            if(!std::filesystem::is_directory(fs, ec))
                return BadDir;
            if(access(fs.c_str(), R_OK | W_OK | X_OK))
                return WriteDir;
            if(access(fs.c_str(), R_OK | X_OK))
                return ReadDir;
            return BadDir;
        }

        struct MWorkspace : public Workspace {
            bool            load(const fspath&, unsigned opts = SEARCH|INIT_LOG);
            QuillSpots      mkRoot(const QuillFile&, path_set& rSeen, path_set& tSeen, PolicyMap rPolicy, PolicyMap tPolicy, bool fTemplate, unsigned depth);
            Vector<Root*>   roots_()
            {
                Vector<Root*>   ret;
                for(const Root* rt : roots)
                    ret << const_cast<Root*>(rt);
                return ret;
            }
        };
        
        
        MWorkspace    sWksp;
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

    bool                Workspace::init(const std::filesystem::path& spec, unsigned opts)
    {
        static bool ret = sWksp.load(spec, opts);
        return ret;
    }



    bool                        MWorkspace::load(const std::filesystem::path& spec, unsigned opts)
    {
        std::time_t         now;
        std::time(&now);
        
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
            
        if(dir_type(qdir) == BadDir){
            wkspError << "Bad directory for workspace --> " << qdir;
            return false;
        }
        
        aux             = std::move(doc.aux_ports);
        port            = doc.port;
        rtimeout        = doc.read_timeout;
        if(!doc.log_dir.empty())
            logs        = absolute_proximate(doc.log_dir.c_str(), tmp);
        else
            logs        = tmp / "log";
        mkdir(logs.c_str(), kDirMode);
        
        start          = now;
        
        {
            std::tm     gt;
            gmtime_r(&now, &gt);
            char    buffer[256];
            strftime(buffer, sizeof(buffer), "yyyyMMdd-HHmmss", &gt);
            starttext   = buffer;
        }
        
        
        if(opts & INIT_LOG){
            String  lf  = String(CmdArgs::appName()) + "-" + starttext + ".log";
            log         = logs / lf.c_str();
            log_to_file(log.c_str(), LogPriority::All);
        }

        if(!doc.cache.empty())
            cache       = absolute_proximate(doc.cache.c_str(), tmp);
        else
            cache       = tmp / "cache";
        
        path_set        rSeen, tSeen;
        Root*           rt  = new Root{qdir, PolicyMap(Access::ReadWrite)};
        roots << rt;
        rpath["."]              = rt;
        rpath[qdir.c_str()]     = rt;
        
        QuillSpots  qs  = mkRoot(doc, rSeen, tSeen, PolicyMap(Access::ReadWrite), 
            PolicyMap((opts&TEMPLATES_RO) ? Access::ReadOnly : Access::ReadWrite), false, 0);
        
        abbr                = qs.abbr.data;
        author              = qs.author.data;
        copyright.from      = qs.copyfrom.data;
        copyright.to        = qs.copyto.data;
        copyright.stance    = qs.copystance.data;
        copyright.text      = qs.copytext.data;
        home                = qs.home.data;
        name                = qs.name.data;
        
        for(auto& i : tSeen)
            templates << i;
        
        roots.sort([](const Root* a, const Root* b){
            if(a->is_template != b->is_template)
                return a->is_template < b->is_template;
            if(a->depth != b->depth)
                return a->depth < b->depth;
            return a->id < b->id;
        });
        
        Vector<Root*>   eroots  = roots_();
        
        for(Root* r2 : eroots){
            fspath  cd      = r2 -> path / szConfigDir;
            mkdir(cd.c_str(), kDirMode);
        }
        
        StringSet   keys;
        uint64_t    i   = 0;
        for(Root* r2 : eroots){  // give everything IDs and record the keys
            r2 -> id = i++;
            if(dir_type(r2 -> path) != WriteDir){
                for(DataRole dr : DataRole::all_values())
                    r2 -> access[dr]    = moderate(r2 -> access[dr], Access::ReadOnly);
            }
            keys << r2 -> key;
        }
        
        //  make sure everything has a key, default is the number
        for(Root* r2 : eroots){
            if(!r2->key.empty())
                continue;
            String s   = String::number(r2->id);
            if(!keys.has(s)){
                r2 -> key = s;
                keys << s;
            }
            s += '_';
            for(i=0;; ++i){
                String  k   = s + String::number(i);
                if(!keys.has(k)){
                    r2 -> key = k;
                    keys << k;
                    break;
                }
            }
        }
        
        //  Sort out the writers
        for(DataRole dr : DataRole::all_values()){
            bool    hasFirst    = false;
            for(Root* r3 : eroots){
                if(r3->access[dr] == Access::WriteFirst){
                    if(hasFirst)    
                        r3->access[dr]   = Access::ReadWrite;
                    else
                        hasFirst    = true;
                }
            }

            if(!hasFirst){
                for(Root* r3 : eroots){
                    if(r3->access[dr]     == Access::WriteFirst) {
                        r3 -> access[dr]    = Access::WriteFirst;
                        hasFirst    = true;
                        break;
                    }
                }
            }

            if(!hasFirst)
                rfirst[dr]     = nullptr;

            for(Root* r3 : eroots){
                switch(r3->access[dr]){
                case Access::WriteFirst:
                    rfirst[dr]     = r3;
                    /* fall through */
                case Access::ReadWrite:
                    rwrite[dr] << r3;
                case Access::ReadOnly:
                    rread[dr] << r3;
                case Access::NoAccess:
                default:
                    break;
                }
            }
        }

        //  TODO (LATER)
        return true;

    }



    QuillSpots      MWorkspace::mkRoot(const QuillFile&doc, path_set& rSeen, path_set& tSeen, PolicyMap rPolicy, PolicyMap tPolicy, bool fTemplate, unsigned depth)
    {
        QuillSpots  ret;
        if(!fTemplate){
            ret.abbr        = { doc.abbr, depth };
            ret.author      = { doc.author, depth };
            ret.copyfrom    = { doc.copyright.from, depth };
            ret.copyto      = { doc.copyright.to, depth };
            ret.copystance  = { doc.copyright.stance, depth };
            ret.copytext    = { doc.copyright.text, depth };
            ret.home        = { doc.home, depth };
            ret.name        = { doc.name, depth };
        }
        ++depth;
        
        fspath  q   = doc.file().parent_path();
        for(auto& rs : doc.templates){
            if(!tSeen.add(rs.path))
                continue;
            PolicyMap   pm2 = moderate(tPolicy, rs.policy);

    #if 0        
            for(QString t : template_dir_paths(rsPath)){
                Root*   rt          = nullptr;
                bool    rcre        = false;
                std::tie(rt,rcre)   = _root(t);
                if(rcre)

                    rt -> m_depth   = depth;
                for(DataRole dr : DataRole::all_values()){
                    if(pm2[dr] == Access::Default)
                        pm2[dr] = tPolicy[dr];
                    rt -> m_roles[dr].access    = pm2[dr];
                }
                rt->m_name          =  ":" + rsPath;
                rt->m_key           = rs.key.qString();
                if(rt->m_key.isEmpty())
                    rt->m_key       = rt->m_name;
                rt->m_isTemplate    = true;
                rt->m_color         = rs.color.qString();
                rt->m_icon          = rs.icon.qString();
            
                QString     qf  = quill_fragment_for_dir(t);
                if(!QFile::exists(qf)){
                    yDebug() << "Quill fragment " << qf << " does not exist (not required to).";
                    continue;
                }
                
                QuillFile        sub;
                if(!sub.load(qf.toStdString())){
                    yWarning() << "Unable to read Quill fragment: " << qf;
                    continue;
                }
                
                _load(sub, rSeen, tSeen, rPolicy, pm2, spots, true, depth+1);
            }
    #endif
        }
        
        if(!fTemplate){
            for(auto& rs : doc.roots){
                String      rsPath  = rs.path.trimmed();
                
    #if 0            
                
                QString     rpath   = q.absoluteFilePath(rsPath);
                if(rsPath == ".")
                    rpath          = q.absolutePath();

                if(!rSeen.add(rpath))   // already seen
                    continue;
                    
                PolicyMap   pm2 = moderate(rPolicy, rs.policy);    
                
                Root*   rt      = nullptr;
                bool    rcre    = false;
                std::tie(rt,rcre)   = _root(rpath);
                if(rcre)
                    rt -> m_depth   = depth;
                    
                for(DataRole dr : DataRole::all_values()){
                    if(pm2[dr] == Access::Default)
                        pm2[dr] = rPolicy[dr];
                    rt -> m_roles[dr].access    = pm2[dr];
                }
                rt -> m_name        = rs.name.qString();
                rt -> m_key         = rs.key.qString();
                if(rt -> m_key.isEmpty())
                    rt -> m_key     = rt -> m_name;
                if(rs.vcs != Vcs())
                    rt -> m_vcs    |= rs.vcs;
                rt -> m_isTemplate  = false;
                rt->m_color         = rs.color.qString();
                rt->m_icon          = rs.icon.qString();
                
                if(!rcre)       // should only trigger for the root, avoid a nasty recursion
                    continue;
                
                QString     qf  = quill_fragment_for_dir(rpath);
                if(!QFile::exists(qf)){
                    yDebug() << "Quill fragment " << qf << " does not exist (not required to).";
                    continue;
                }
                
                QuillFile        sub;
                if(!sub.load(qf.toStdString())){
                    yWarning() << "Unable to read Quill fragment: " << qf;
                    continue;
                }
                
                
                if((!sub.abbr.empty()) && (depth < spots.abbr)){
                    m.abbreviation      = sub.abbr.qString();
                    spots.abbr          = depth;
                }
                if((!sub.author.empty()) && (depth < spots.author)){
                    m.author            = sub.author.qString();
                    spots.author        = depth;
                }
                
                if(sub.copyright.from && (depth < spots.copyfrom)){
                    m.copyright.from    = sub.copyright.from;
                    spots.copyfrom      = depth;
                }
                if((sub.copyright.stance != AssertDeny()) && (depth < spots.copystance)){
                    m.copyright.stance  = sub.copyright.stance;
                    spots.copystance    = depth;
                }
                if((!sub.copyright.text.empty()) && (depth < spots.copytext)){
                    m.copyright.text    = sub.copyright.text;
                    spots.copytext      = depth;
                }
                if(sub.copyright.to && (depth < spots.copyto)){
                    m.copyright.to      = sub.copyright.to;
                    spots.copyto        = depth;
                }
                if((!sub.home.empty()) && (depth < spots.home)){
                    m.home              = sub.home.qString();
                    spots.home          = depth;
                }
                if((!sub.name.empty()) && (depth < spots.name)){
                    m.name              = sub.name.qString();
                    spots.name          = depth;
                }
                
                _load(sub, rSeen, tSeen, pm2, tPolicy, spots, false, depth+1);
    #endif

            }
        }
                    
        //  TODO
        
        
        return ret;
    }

}
