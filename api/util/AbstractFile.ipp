////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once



AbstractFile::AbstractFile()
{
}

AbstractFile::~AbstractFile()
{
}

void            AbstractFile::clear()
{
    reset();
    m_file.clear();
}

bool            AbstractFile::load(const std::filesystem::path& ipath)
{
    if(!read_enabled())
        return false;
    
    //if(load_it(ipath)){
        //m_file  = ipath;
        //return true;
    //} else
        //return false;

    std::ios_base::openmode om  = std::ios_base::in;
    if(is_binary())
        om |= std::ios_base::binary;
    std::ifstream   fin(ipath, om);
    //u8_ifstream   fin(ipath, om);
    if(!fin)
        return false;
    return load(fin, ipath);
}

bool            AbstractFile::load(const QString& s)
{
    return load(std::filesystem::path(s.toStdString()));
}


//bool            AbstractFile::load(u8_istream&fin, const std::filesystem::path& fp)
bool            AbstractFile::load(std::istream&fin, const std::filesystem::path& fp)
{
    if(!read_enabled())
        return false;

    Vector<char>     data;
    
    fin.seekg( 0, std::ios::end);
    bool    ok  = !fin.fail();
    size_t  sz  = fin.tellg();
    fin.seekg( 0, std::ios::beg);
    
    if(ok){
        data.resize(sz+1);
        fin.read(data.data(), sz);
        data[sz]    = u8'\0';
    } else {
        char     buffer[64<<10];
        while(!fin.eof() && !fin.fail()){
            fin.read(buffer, sizeof(buffer));
            size_t sz  = fin.gcount();
            data.append(buffer, sz);
        }
        data << u8'\0';
    }
    
    return load(data, fp);
}

bool            AbstractFile::load(Vector<char>&data, const std::filesystem::path& fp)
{
    if(!read_enabled())
        return false;
    reset();
    bool ok = read(data, fp.string());
    if(ok)
        m_file  = fp;
    return ok;
}


bool            AbstractFile::reload()
{
    if(m_file.empty())
        return false;
    return load(m_file);
}


bool            AbstractFile::save()
{
    if(m_file.empty())
        return false;
    return save_to(m_file);
}

bool            AbstractFile::save_as(const std::filesystem::path& oPath)
{
    if(save_to(oPath)){
        m_file  = oPath;
        return true;
    } else
        return false;
}

bool            AbstractFile::save_as(const QString& p)
{
    return save_as(std::filesystem::path(p.toStdString()));
}


bool            AbstractFile::save_to(const std::filesystem::path& oPath)
{
    if(!write_enabled())
        return false;
        
    Vector<char>    data;
    if(!write(data))
        return false;
        
    std::filesystem::path   tmp  = oPath.string() + ".tmp";
    int         n   = 0;
    while(file_exists(tmp.c_str()))
        tmp = oPath.string() + ".tmp" + std::to_string(++n);
    std::ios_base::openmode om  = std::ios_base::out | std::ios_base::trunc;
    if(is_binary())
        om |= std::ios_base::binary;
    bool    f   = false;
    {
        std::ofstream fout(tmp, om);
        if(!fout)
            return false;
        fout.write(data.data(), data.size());
        f   = !fout.fail();
        fout.close();
    }
    if(f){
        file_backup(oPath.c_str(), "bck");
        rename(tmp.c_str(), oPath.c_str());
    } else {
        remove(tmp.c_str());
    }
    return f;
}

bool            AbstractFile::save_to(const QString&p)
{
    return save_to(std::filesystem::path(p.toStdString()));
}

bool            AbstractFile::save_to(std::ostream&out)
{   
    if(!write_enabled())
        return false;
    Vector<char>    data;
    if(!write(data))
        return false;
    out.write(data.data(), data.size());
    return true;
}

bool            AbstractFile::save_to(Vector<char>&data)
{
    if(!write_enabled())
        return false;
    return write(data);
}

void            AbstractFile::set_file(const std::filesystem::path&p)
{
    if(can_change_to(p))
        m_file = p;
}


void            AbstractFile::set_file(const QString& s)
{
    set_file(std::filesystem::path(s.toStdString()));
}


