////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "StdFile.hpp"
#include "util/Stream.hpp"

#include "util/Logging.hpp"
#include "util/Set.hpp"
#include "util/Reverse.hpp"
#include <cctype>

#include <iostream>

void        StdFile::add_attr(Vector<Attribute>& datas, size_t& i, Attribute& v)
{
    size_t      z   = datas[i].indent;
    v.attrs << std::move(datas[i]);
    while( (++i) < datas.size()){
        if(datas[i].indent == z){
            v.attrs << std::move(datas[i]);
            continue;
        } 
        
        while((i < datas.size()) && (datas[i].indent > z))
            add_attr(datas, i, v.attrs.back());

        if(i >= datas.size())
            break;
        if(datas[i].indent < z)
            break;
            
        v.attrs << std::move(datas[i]);
    }
}

bool        StdFile::empty() const
{
    return body.empty() && attrs.empty();
}



bool        StdFile::read(Vector<char>&buffer, const std::string& fname) 
{
    if(buffer.empty())
        return true;
        
    enum class Mode {
        SPACE,          //!< Space at start of line
        CMD,            //!< Cmd at the start of the line
        CMDS,           //!< Space between command and ID
        ID,             //!< ID
        IDS,            //!< Space between ID and key
        KEY,            //!< Key
        KEYS,           //!< Space between key and data
        VALUE,          //!< Everything afterwards to new-line
        BODY            //!< Body of the document
    };

    Mode            mode    = Mode::SPACE;
    body.reserve(buffer.size());
    Attribute           val;
    unsigned int    line    = 1;
    unsigned int    lident  = 0;
    Vector<Attribute>   datas;

    auto push   = [&](){
        if(!val.empty()){
            lident  = val.indent;
            if(!val.data.empty() && val.data.back() == '"')
                val.data.pop_back();
            
            datas << std::move(val);
            val     = Attribute();
        }
        mode    = Mode::SPACE;
    };
    
    for(char ch : buffer){
        if(!ch)
            continue;
        switch(mode){
        case Mode::BODY:
            if(ch)
                body += ch;
            break;
        case Mode::SPACE:
            switch(ch){
            case '#':
                val.cmd = "#";
                mode    = Mode::VALUE;
                break;
            case '\n':
                if(has_body()){
                    mode    = Mode::BODY;
                } else {
                    //  No body means a new attriubte, clone the last indent
                    Attribute   a;
                    a.indent    = lident;
                    datas << a;
                }
                break;
            case '%':
                mode    = Mode::KEY;
                val.key += '%';
                break;
            case '[':
                mode    = Mode::ID;
                break;
            default:
                if(ispunct(ch)){
                    mode    = Mode::CMD;
                    val.cmd += ch;
                    break;
                }
                if(isalnum(ch)){
                    mode    = Mode::KEY;
                    val.key += ch;
                    break;
                }
                if(isspace(ch)){
                    if(datas.empty() && !val.indent){
                        yWarning() << "First attribute should always start left-adjusted! " << line << " : " << fname;
                    }
                    ++ val.indent;
                    break;
                }
                //  This state is an error!
                yError() << "Non processible character found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::CMD:
            if(isspace(ch)){
                mode    = Mode::CMDS;
                break;
            }
            if(isalnum(ch) || ispunct(ch)){
                val.cmd += ch;
                break;
            }
            if(ch == '\n'){
                push();
                break;
            }
            break;
        case Mode::CMDS:
            switch(ch){
            case '[':
                mode    = Mode::ID;
                break;
            case '\n':
                push();
                break;
            default:
                if(isalnum(ch)){
                    val.key += ch;
                    mode    = Mode::KEY;
                    break;
                }
                if(isspace(ch))
                    break;
                yError() << "Non processible character found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::ID:
            switch(ch){
            case ']':
                mode    = Mode::IDS;
                break;
            case '\n':
                yError() << "Line break during ID, on line " << line << " : " << fname;
                push();
                break;
            case '[':
                yError() << "Stray '[' detected on line " << line << " : " << fname;
                break;
      
            default:
                if(isalnum(ch) || ispunct(ch)){
                    val.id += ch;
                    break;
                }
                if(isspace(ch)){
                    yError() << "Erroneous space detected in ID on line " << line << " : " << fname;
                    break;
                }
                break;
            }
            break;
        case Mode::IDS:
            switch(ch){
            case '\n':
                push();
                break;
            default:
                if(isalnum(ch)){
                    val.key += ch;
                    mode    = Mode::KEY;
                    break;
                }
                if(isspace(ch))
                    break;
                    
                yError() << "Non processible character found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::KEY:
            switch(ch){
            case '\n':
                push();
                break;
            default:
                if(isalnum(ch) || ispunct(ch)){
                    val.key += ch;
                    break;
                }
                if(isspace(ch)){
                    mode        = Mode::KEYS;
                    break;
                }
                yError() << "Non processible character found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::KEYS:
            switch(ch){
            case '\n':
                push();
                break;
            case '"':
                mode        = Mode::VALUE;
                break;
            default:
                if(isspace(ch)){
                    break;
                } else {
                    val.data += ch;
                    mode    =    Mode::VALUE;
                    break;
                }
                break;
            }
            break;
        case Mode::VALUE:
            if(ch == '\n'){
                push();
            } else {
                val.data += ch;
            }
            break;
        }
    }
    
    if(datas.empty())
        return true;
        
    if(recursive_attributes()){
        unsigned int        z   = datas[0].indent;
        for(size_t i=0; i<datas.size(); ++i){
            if(datas[i].indent <= z){
                attrs << std::move(datas[i]);
            } else {
                add_attr(datas, i, attrs.back());
                if(i<datas.size())
                    attrs << std::move(datas[i]);
            }
        }
    } else {
        attrs       = std::move(datas);
    }
    
    return true;
}

void        StdFile::reset() 
{
    attrs.clear();
    body.clear();
}

namespace {
    size_t      max_pad(const AttrTree& at)
    {
        size_t  a   = 0;
        for(const Attribute& v : at.attrs){
            if(v.empty())
                continue;
            if(v.cmd == "#")
                continue;
            size_t  b   = v.cmd.size() + v.id.size() + v.key.size();
            if(!v.cmd.empty())
                ++b;
            if(!v.id.empty())
                b += 3;
            a   = std::max({a,b,max_pad(v)});
        }
        ++a;
        return a;
    }
}


bool        StdFile::write(Vector<char>& buffer) 
{
    Stream  out(buffer);
    write_out(out, attrs, max_pad(*this));
    out << '\n' << body;
    return true;
}


void       StdFile::write_out(Stream& out, const Vector<Attribute>&datas, unsigned int mxval, unsigned int depth) const
{
    for(const Attribute& v : datas){
        if(v.empty()){
            out << '\n';
            continue;
        }

        for(unsigned int n = 0;n<depth;++n)
            out << "    ";
            
        if(v.cmd == "#"){
            out << "#" << v.data << '\n';
        } else {
            size_t  tgt = mxval + out.column();
            if(!v.cmd.empty())
                out << v.cmd << ' ';
            if(!v.id.empty())
                out << '[' << v.id << ']' << ' ';
            if(!v.key.empty())
                out << v.key;
            while(out.column() < tgt)
                out << ' ';
            out << v.data << '\n';
        }
        write_out(out, v.attrs, mxval, depth+1);
    }
}

