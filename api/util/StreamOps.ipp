#pragma once

Stream&     operator<<(Stream&str, const char* v)
{
    if(v)
        str.write(v, strlen(v));
    return str;
}

Stream&     operator<<(Stream&str, const std::string&v)
{
    str.write(v.data(), v.size());
    return str;
}

Stream&     operator<<(Stream&str, const std::string_view&v)
{
    str.write(v.data(), v.size());
    return str;
}

Stream&     operator<<(Stream&str, const QString&v)
{
    return str << v.toStdString();
}

Stream&     operator<<(Stream&str,  const QByteArray&v)
{
    str.write(v.constData(), v.size());
    return str;
}

Stream&     operator<<(Stream& str, char v)
{
    str.write(&v, 1);
    return str;
}

Stream&     operator<<(Stream&str, float v)
{
    //  std::to_chars would be GREAT, if GCC implemented it.....
    char    buf[kStdBuf+1];
    int n  = snprintf(buf, kStdBuf, "%g", v);
    str.write(buf, n);
    return str;
}

Stream&     operator<<(Stream&str, double v)
{
    //  std::to_chars would be GREAT, if GCC implemented it.....
    char    buf[kStdBuf+1];
    int n  = snprintf(buf, kStdBuf, "%lg", v);
    str.write(buf, n);
    return str;
}

Stream&     operator<<(Stream&str, bool v)
{
    static constexpr const std::string_view     kFalse  = "false";
    static constexpr const std::string_view     kTrue   = "true";
    return str << (v?kTrue:kFalse);
}


Stream&     operator<<(Stream&str, int8_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}

Stream&     operator<<(Stream&str, int16_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}

Stream&     operator<<(Stream&str, int32_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}

Stream&     operator<<(Stream&str, int64_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}


Stream&     operator<<(Stream&str, uint8_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}

Stream&     operator<<(Stream&str, uint16_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}

Stream&     operator<<(Stream&str, uint32_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}

Stream&     operator<<(Stream&str, uint64_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    str.write(buf, (p-buf));
    return str;
}


Stream&     operator<<(Stream&str, const std::filesystem::path&v)
{
    return str << v.native();
}


