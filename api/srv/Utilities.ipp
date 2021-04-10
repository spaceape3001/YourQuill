////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

bool        auto_edit()
{
    if(x_session)
        return x_session -> autoEdit;
    return false;
}



QString     cur_user()
{
    if(!x_session)
        return "(no-session)";
    if(x_is_local)
        return wksp::local_user();
    return utf8(x_session -> user);
}


const Root* def_root(DataRole dr)
{
    if(x_session){
        const Root *rt   = x_session -> defRoot;
        if(rt)  
            return rt;
    }
    
    return wksp::root_reads(dr).value(0,nullptr);
}


Vector<Class>   get_class_vector(const StringSet& sset)
{
    Vector<Class>   ret;
    for(const String& s : sset){
        for(String t : s.split(',')){
            Class   c   = cdb::class_(t.trimmed().qString());
            if(c)
                ret << c;
        }
    }
    return ret;
}


SizeDesc    icon_size()
{
    if(!x_session)
        return SizeDesc::Small;
    return x_session -> iconSize;
}

bool        inspect_submit()
{
    if(x_session)   
        return x_session -> inspectSubmit;
    return false;
}

bool        is_mobile()
{
    if(!x_request)
        return false;
    QByteArray  h   = x_request->getHeader("user-agent").toLower();
    if(h.contains("android"))
        return true;
    if(h.contains("iphone"))
        return true;
    return false;
}


QByteArray  reencode_parameters()
{
    QByteArray  ret;
    bool        f = true;
    if(x_request){
        for(auto& i : x_request -> getParameterMap().toStdMap()){
            if(f)
                f   = false;
            else
                ret += '&';
            ret += QUrl::toPercentEncoding(utf8(i.first)) + '=' + QUrl::toPercentEncoding(utf8(i.second));
        }
    }
    return ret;
}


bool        valid_key(const QByteArray& b)
{
    for(int i=0;i<b.count();++i){
        char ch     = b.at(i);
        if(isalnum(ch))
            continue;
        if(ch == '_')
            continue;
        return false;
    }
    return true;
}

void        return_to_sender()
{
    if(!x_request)
        throw HttpStatus::InternalError;
    throw Redirect(HttpStatus::SeeOther, x_request -> getHeader("referer"));
}
