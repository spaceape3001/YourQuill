#pragma once


class Char32 {
public:
    Char32() : m_code(0) {}
    Char32(char32_t c) : m_code(c) {}

    operator char32_t () const { return m_code; }
    
    char32_t    code() const { return m_code; }
    bool        good() const { return m_code != 0; }
    
    //bool        operator==(Char32 b) { return m_code == b.m_code; }
    //bool        operator!=(Char32 b) { return m_code != b.m_code; }
    //bool        operator<(Char32 b) { return m_code < b.m_code; }
    //bool        operator<=(Char32 b) { return m_code <= b.m_code; }
    //bool        operator>(Char32 b) { return m_code > b.m_code; }
    //bool        operator>=(Char32 b) { return m_code >= b.m_code; }

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
    
    char32_t    to_lower() const;
    char32_t    to_upper() const;
    
private:
    char32_t    m_code;
};
