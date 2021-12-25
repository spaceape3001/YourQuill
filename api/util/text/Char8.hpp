#pragma once

namespace yq {

    //  DEPRECATED ... this is merely a pass thru to standard library....
    class Char8 {
    public:
        constexpr Char8() : m_code(0) {}
        constexpr Char8(char c) : m_code(c) {}
        constexpr Char8(char8_t c) : m_code((char) c) {}
        
        operator char () const { return m_code; }
        char        code() const { return m_code; }
        bool        good() const { return m_code != 0; }
        
        bool        operator==(Char8 b) { return m_code == b.m_code; }
        bool        operator!=(Char8 b) { return m_code != b.m_code; }
        bool        operator<(Char8 b) { return m_code < b.m_code; }
        bool        operator<=(Char8 b) { return m_code <= b.m_code; }
        bool        operator>(Char8 b) { return m_code > b.m_code; }
        bool        operator>=(Char8 b) { return m_code >= b.m_code; }
        
        bool        is_alnum() const;
        bool        is_alpha() const;
        bool        is_blank() const;
        bool        is_control() const;
        bool        is_digit() const;
        bool        is_graph() const;
        bool        is_lower() const;
        bool        is_print() const;
        bool        is_punct() const;
        bool        is_space() const;
        bool        is_upper() const;
        bool        is_xdigit() const;
        
        char        to_lower() const;
        char        to_upper() const;
        
    private:
        char        m_code;
    };
}
