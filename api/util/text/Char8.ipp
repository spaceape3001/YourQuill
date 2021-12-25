#pragma once

namespace yq {
    bool        Char8::is_alnum() const
    {
        return ::isalnum(m_code);
    }

    bool        Char8::is_alpha() const
    {
        return ::isalpha(m_code);
    }

    bool        Char8::is_blank() const
    {
        return ::isblank(m_code);
    }

    bool        Char8::is_control() const
    {
        return ::iscntrl(m_code);
    }

    bool        Char8::is_digit() const
    {
        return ::isdigit(m_code);
    }

    bool        Char8::is_graph() const
    {
        return ::isgraph(m_code);
    }

    bool        Char8::is_lower() const
    {
        return ::islower(m_code);
    }

    bool        Char8::is_print() const
    {
        return ::isprint(m_code);
    }

    bool        Char8::is_punct() const
    {
        return ::ispunct(m_code);
    }

    bool        Char8::is_space() const
    {
        return ::isspace(m_code);
    }

    bool        Char8::is_upper() const
    {
        return ::isupper(m_code);
    }

    bool        Char8::is_xdigit() const
    {
        return ::isxdigit(m_code);
    }

    char        Char8::to_lower() const
    {
        return ::tolower(m_code);
    }

    char        Char8::to_upper() const
    {
        return ::toupper(m_code);
    }
}
