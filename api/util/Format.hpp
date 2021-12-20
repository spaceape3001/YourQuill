#pragma once

#include "String.hpp"
#include "Vector.hpp"
#include <string>
#include <string_view>


/*! Lightly formatting string

    Qt has it's .arg().  Anyways, we won't worry about recursive expansion here, simple %1 %2 etc.  
*/    



class Format {
public:

    class Args;

    //  Need a syntax...
    //  %(number)   Will join
    //  %+          Concatentate (in case of immediate number)
    //  %%          Percent
    
    
    //!  Assuming this is a compiled string or similarly stable source
    Format(const char*);                // Will NOT copy
    Format(const std::string_view&);    // will NOT copy
    Format(const std::string&);         // will NOT copy

    Format(std::string&&);              // WILL copy

        // COPYING is forbidden
    Format(const Format&) = delete;
    Format& operator=(const Format&) = delete;

        // MOVING is the way
    Format(Format&&) = default;
    Format& operator=(Format&&) = default;
    
    ~Format() = default;
    
    
    
    template <typename ... T>
    Args  arg(T... args) const;
    
    
private:
    struct Token {
        std::string_view        bit;
        unsigned int            id  = ~0;
        
        constexpr Token() = default;
        constexpr Token(const std::string_view&b) : bit(b) {}
        constexpr Token(unsigned int i) : id(i) {}
    };

    std::string                 m_input;    // not always used... a string view/char* will NOT set this
    Vector<Token>               m_tokens;   //  ZERO means no-tokens
    unsigned int                m_max   = 0;
    
    void parse(const char*, size_t);
};

Format operator ""_fmt(const char* z) { return Format(z); }


class Format::Args {
public:

    Args&     arg(const std::string_view&);
    Args&     arg(bool, const char* szTrue="true", const char* szFalse="false");

    String          string() const;
    operator String() const { return string(); }


private:
    friend class Format;
    Args(const Format*);
    
    const Format*   m_format    = nullptr;
    Vector<String>  m_args;
    
    void    push(const std::string_view&);
};


template <typename ... T>
Format::Args  Format::arg(T... args) const
{
    Args ret(this);
    ret.arg(args...);
    return ret;
}

