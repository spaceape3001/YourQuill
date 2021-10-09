
bool        Char32::is_alnum() const
{
    return ::iswalnum(m_code);
}

bool        Char32::is_alpha() const
{
    return ::iswalpha(m_code);
}

bool        Char32::is_blank() const
{
    return ::iswblank(m_code);
}

bool        Char32::is_control() const
{
    return ::iswcntrl(m_code);
}

bool        Char32::is_digit() const
{
    return ::iswdigit(m_code);
}

bool        Char32::is_graph() const
{
    return ::iswgraph(m_code);
}

bool        Char32::is_lower() const
{
    return ::iswlower(m_code);
}

bool        Char32::is_print() const
{
    return ::iswprint(m_code);
}

bool        Char32::is_punct() const
{
    return ::iswpunct(m_code);
}

bool        Char32::is_space() const
{
    return ::iswspace(m_code);
}

bool        Char32::is_upper() const
{
    return ::iswupper(m_code);
}

bool        Char32::is_xdigit() const
{
    return ::iswxdigit(m_code);
}

char32_t    Char32::to_lower() const
{
    return ::towlower(m_code);
}

char32_t    Char32::to_upper() const
{
    return ::towupper(m_code);
}

