////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

Url      Url::fromQUrl(const QUrl&q)
{
    Url ret;
    ret.scheme      = q.scheme();
    ret.port        = q.port();
    ret.host        = q.host();
    ret.user        = q.userName();
    ret.path        = q.path();
    ret.query       = q.query();
    ret.fragment    = q.fragment();
    ret.pwd         = q.password();
    return ret;
}


url_r    Url::parse(const std::string_view&sv, unsigned int opts)
{
    // THACK!  (Leverage Qt beneath the hood)
    return fromQUrl(QString::fromLatin1(sv.data(), sv.size()));
}

Url      Url::file(const std::filesystem::path& pth)
{
    Url ret;
    ret.scheme  = "file";
    ret.path    = pth;
    return ret;
}

String   Url::string(unsigned int opts) const
{
    //  HACK!  (Leverage Qt beneath the hood)
    return qUrl(opts).toString();
    
    //BufferStream    buf;
    //write_to(buf, opts);
    //return buf.view();
}

void    Url::write_to(yq::Stream&str, unsigned int opts) const
{
    str << string(opts);
}

QUrl    Url::qUrl(unsigned int opts) const
{
    QUrl    ret;
    if((opts & SCHEME) && !scheme.empty())
        ret.setScheme(scheme.qString());
    if((opts & HOST) && !host.empty())
        ret.setHost(host.qString());
    if((opts & USER) && !user.empty())
        ret.setUserName(user.qString());
    if((opts & PWD) && !pwd.empty())
        ret.setPassword(pwd.qString());
    if((opts & QUERY) && !query.empty())
        ret.setQuery(query.qString());
    if((opts & FRAGMENT) && !fragment.empty())
        ret.setFragment(fragment.qString());
    if((opts & PORT) && port != -1)
        ret.setPort(port);
    return ret;
}

yq::Stream& operator<<(yq::Stream&str, const Url&v)
{
    v.write_to(str);
    return str;
}

