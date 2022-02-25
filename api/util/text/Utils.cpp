////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Comma.hpp"
#include "Iter32.hpp"
#include "IterW.hpp"
#include "Utils.hpp"

#include <util/algo/Compare.hpp>
#include <util/collection/List.hpp>
#include <util/collection/Map.hpp>
#include <util/collection/Set.hpp>



    /*
        GCC v10 did not support to/from chars on floating point types.  It's been enabled in v11.
        Therefore, we'll macro this portion. :(
    */
#if defined(MSVC)
    #define FP_CHARCONV 1
#elif defined(__GNUC__)
    #if __GNUC__ >= 11
        #define FP_CHARCONV 1
    #endif
#endif
#ifndef FP_CHARCONV
    #define FP_CHARCONV 0
#endif

namespace yq {

    namespace {
        //! Standard  buffer size for formatting numbers and the like
        static constexpr const size_t   kStdBuf = 63;
        

        //! Trims a string's leading/trailing whitespace by altering parameters
        void    trim_ws(const char*& s, size_t& n)
        {
            while(s && *s && isspace(*s) && n)
                ++s, --n;
            while(s && n && isspace(s[n-1]))
                --n;
        }
        
        //const char* nonspace(const char* s, size_t n)
        //{
            //while(s && *s && isspace(*s) && n)
                //++s, --n;
            //return s;
        //}

        //! Checks for equality by assuming left may vary in case, the right will be lower case.
        bool    is_same(const char*a, size_t n, const char *b)
        {
            if(a && b){
                for(;*a && *b && n; ++a, ++b, --n){
                    if(::tolower(*a) != *b)
                        return false;
                }
                return !(*b || n);
            }
            return false;
        }
        
        
        static constexpr const auto bTRUE     = boolean_r( true, true );
        static constexpr const auto bFALSE    = boolean_r( true, false );
        static constexpr const auto dFAIL     = double_r(false, NaN);
        static constexpr const auto fFAIL     = float_r(false, NaNf);
        

        template <typename T>
        Result<T>   int_from_chars(const char*s, size_t n, int base=10)
        {
            T    res = 0;
            auto [p,ec] = std::from_chars(s, s+n, res, base);
            return Result<T>(ec == std::errc(), res);
        }
        
        std::string     cvt_u32string(const char32_t *z, size_t n)
        {
            std::string ret;
            ret.resize((std::max<size_t>(64, n*4)));    // approx guess
            std::mbstate_t  state{};
            size_t          m   = 0;
            for(size_t i=0;i<n;++i){
                if(ret.size() - m < 6)
                    ret.resize(ret.size() * 2);
                m += c32rtomb(ret.data()+m, z[i], &state);
            }
            ret.resize(m);
            return ret;
        }
        
        std::string     cvt_wstring(const wchar_t *z, size_t n)
        {
            std::string ret;
            ret.resize((std::max<size_t>(64, n*4)));    // approx guess
            std::mbstate_t  state{};
            size_t          m   = 0;
            for(size_t i=0;i<n;++i){
                if(ret.size() - m < 6)
                    ret.resize(ret.size() * 2);
                m += wcrtomb(ret.data()+m, z[i], &state);
            }
            ret.resize(m);
            return ret;
        }
        //  IF ever needed for wchar_t, rename "c32rtomb" to "wcrtomb".
        
    }

    
    bool    any_starts(const std::vector<std::string>&haystack, const std::string_view&pat)
    {
        for(auto& s : haystack)
            if(starts(s, pat))
                return true;
        return false;
    }
    
    bool    any_starts(const std::vector<std::string_view>&haystack, const std::string_view&pat)
    {
        for(auto& s : haystack)
            if(starts(s, pat))
                return true;
        return false;
    }
    
    bool    any_starts(const std::initializer_list<std::string_view>&haystack, const std::string_view&pat)
    {
        for(auto& s : haystack)
            if(starts(s, pat))
                return true;
        return false;
    }
    
    bool    any_starts_igCase(const std::vector<std::string>&haystack, const std::string_view&pat)
    {
        for(auto& s : haystack)
            if(starts_igCase(s, pat))
                return true;
        return false;
    }
    
    bool    any_starts_igCase(const std::vector<std::string_view>&haystack, const std::string_view&pat)
    {
        for(auto& s : haystack)
            if(starts_igCase(s, pat))
                return true;
        return false;
    }
    
    bool    any_starts_igCase(const std::initializer_list<std::string_view>&haystack, const std::string_view&pat)
    {
        for(auto& s : haystack)
            if(starts_igCase(s, pat))
                return true;
        return false;
    }

    std::string&  operator+=(std::string&str, char32_t ch)
    {
        std::mbstate_t  state{};
        char            buffer[MB_CUR_MAX+1];
        int len = (int) c32rtomb(buffer, ch, &state);
        if(len > 0)
            str.append(buffer, len);
        return str;
    }

    unsigned_r  as_hex(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<unsigned int>(s, n, 16);
    }


    unsigned_r  as_hex(const std::string_view&s)
    {
        return as_hex(s.data(), s.size());
    }

    uint8_r  as_hex8(const char*s, size_t n)
    {
        if(!s)  
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<uint8_t>(s, n, 16);
    }

    uint8_r  as_hex8(const std::string_view&s)
    {
        return as_hex8(s.data(), s.size());
    }

    uint16_r  as_hex16(const char*s, size_t n)
    {
        if(!s)  
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<uint16_t>(s, n, 16);
    }

    uint16_r  as_hex16(const std::string_view&s)
    {
        return as_hex16(s.data(), s.size());
    }

    uint32_r  as_hex32(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<uint32_t>(s, n, 16);
    }

    uint32_r  as_hex32(const std::string_view&s)
    {
        return as_hex32(s.data(), s.size());
    }

    uint64_r  as_hex64(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0ULL;
        return int_from_chars<uint64_t>(s, n, 16);
    }

    uint64_r  as_hex64(const std::string_view&s)
    {
        return as_hex64(s.data(), s.size());
    }

    void    blank_cpp_comments(std::string& s)
    {
        const size_t kSize = s.size();
        for(size_t i=0;i<kSize;++i){
            if((s.at(i) == '/') && (s.at(i+1) == '/')){
                for(; (i<kSize) && (s.at(i) != '\n'); ++i)
                    s.at(i)   = ' ';
            }
        }
    }

    std::string  capitalize(const char*s, size_t n)
    {
        std::string ret;
        if(s && n){
            ret.reserve(n);
            bool        space = true;
            iter32(s, n, [&](char32_t c){
                if(is_space(c)){
                    space   = true;
                    ret += c;
                } else {
                    if(is_alpha(c)){
                        if(space)
                            ret += to_upper(c);
                        else
                            ret += to_lower(c);
                    } else 
                        ret += c;
                    space   = false;
                }
            });
        }
        return ret;
    }
    
    std::string  capitalize(const std::string_view&s)
    {
        return capitalize(s.data(), s.size());
    }

    Compare compare(const std::string_view& a, const std::string_view& b)
    {
        size_t  c   = std::min(a.size(), b.size());
        for(size_t i=0;i<c;++i){
            if(a[i] < b[i])
                return Compare::LESSER;
            if(a[i] > b[i])
                return Compare::GREATER;
        }
        
        return compare(a.size(), b.size());
    }

    Compare compare_igCase(const std::string_view& sa, const std::string_view& sb)
    {
        Iter32  a(sa);
        Iter32  b(sb);
        
        for(;;){
            char32_t  ca  = a.next();
            char32_t  cb  = b.next();
            
            int64_t n   = (int64_t) to_lower(ca) - (int64_t) to_lower(cb);
            if(n<0)
                return Compare::LESSER;
            else if(n>0)
                return Compare::GREATER;
            if(!ca)
                return Compare::EQUAL;
        }
    }

    bool    contains(const char* haystack, size_t nHaystack, const char* needle, size_t nNeedle)
    {
        return strnstr(haystack, nHaystack, needle, nNeedle) ? true : false;
    }

    bool    contains(const std::string_view& haystack, const std::string_view& needle)
    {
        return contains(haystack.data(), haystack.size(), needle.data(), needle.size());
    }

    bool    contains_igCase(const char* haystack, size_t nHaystack, const char* needle, size_t nNeedle)
    {
        return strnstr_igCase(haystack, nHaystack, needle, nNeedle) ? true : false;
    }

    bool    contains_igCase(const std::string_view& haystack, const std::string_view& needle)
    {
        return contains_igCase(haystack.data(), haystack.size(), needle.data(), needle.size());
    }

    std::vector<std::string>    copy(const std::vector<std::string>&input)
    {
        std::vector<std::string>    ret;
        ret.reserve(input.size());
        for(const std::string& s : input)
            ret.push_back(s);
        return ret;
    }
    
    std::vector<std::string>    copy(const std::vector<std::string_view>&input)
    {
        std::vector<std::string>    ret;
        ret.reserve(input.size());
        for(const std::string_view& s : input)
            ret.push_back(std::string(s));
        return ret;
    }

    size_t  count_characters(const char* s, size_t n)
    {
        size_t  ret = 0;
        if(s && n){
            iter32(s, n, [&](char32_t){ ++ret; });
        }
        return ret;
    }
    
    size_t  count_characters(const std::string_view&s)
    {
        return count_characters(s.data(), s.size());
    }
    
    size_t  count_characters(const char* s, size_t n, char ch)
    {
        size_t ret =0;
        if(s && n){
            for(size_t i=0;i<n;++i)
                if(s[i] == ch)
                    ++ret;
        }
        return ret;
    }
    
    size_t  count_characters(const std::string_view&s, char ch)
    {
        return count_characters(s.data(), s.size(), ch);
    }
    
    size_t  count_characters(const char* s, size_t n, char32_t ch)
    {
        size_t ret=0;
        if(s && n){
            iter32(s, n, [&](char32_t c){ 
                if(c == ch) 
                    ++ret; 
            });
        }
        return ret;
    }
    
    size_t  count_characters(const std::string_view&s, char32_t ch)
    {
        return count_characters(s.data(), s.size(), ch);
    }


    size_t  count_characters_igCase(const char* s, size_t n, char ch)
    {
        size_t  ret = 0;
        if(s && n){
            ch  = to_lower(ch);
            for(size_t i=0;i<n;++i){
                if(to_lower(s[i]) == ch)
                    ++ ret;
            }
        }
        return ret;
    }
    

    size_t  count_characters_igCase(const std::string_view&s, char ch)
    {
        return count_characters_igCase(s.data(), s.size(), ch);
    }
    
    size_t  count_characters_igCase(const char* s, size_t n, char32_t ch)
    {
        size_t  ret = 0;
        if(s && n){
            ch  = to_lower(ch);
            iter32(s, n, [&](char32_t c){
                if(to_lower(c) == ch)
                    ++ret;
            });
        }
        return ret;
    }
    
    size_t  count_characters_igCase(const std::string_view&s, char32_t ch)
    {
        return count_characters_igCase(s.data(), s.size(), ch);
    }

    size_t  count_start_spaces(const std::string_view&a)
    {
        size_t  ret   = 0;
        iter32_abort(a, [&](char32_t ch) -> bool {
            if(is_space(ch)){
                ++ret;
                return true;
            } else
                return false;
        });
        return ret;
    }

    char32_t  first_non_blank_char(const std::string_view&a)
    {
        char32_t  ret   = 0;
        iter32_abort(a, [&](char32_t ch) -> bool {
            if(!is_space(ch)){
                ret = ch;
                return false;
            }
            return true;
        });
        return ret;
    }

    std::string_view  first_non_empty(const std::string_view&a, const std::string_view&b)
    {
        return a.empty() ? b : a;
    }

    std::string_view  first_non_empty(const std::string_view&a, const std::string_view&b, const std::string_view&c)
    {
        if(!a.empty())
            return a;
        if(!b.empty())
            return b;
        return c;
    }

    std::string_view  first_non_empty(const std::string_view&a, const std::string_view&b, const std::string_view&c, const std::string_view&d)
    {
        if(!a.empty())
            return a;
        if(!b.empty())
            return b;
        if(!c.empty())
            return c;
        return d;
    }

    std::string_view  first_non_empty(std::initializer_list<std::string_view> args)
    {
        for(const std::string_view& s : args)
            if(!s.empty())
                return s;
        return std::string_view();
    }

    std::string_view fmt_hex(uint8_t n, char f)
    {
        static thread_local char    buf[kStdBuf+1];
        buf[0] = buf[1] = f;
        auto [p,ec] = std::to_chars(buf+2, buf+kStdBuf, n, 16);
        return std::string_view(p-2,2);
    }

    std::string_view fmt_hex(uint16_t n, char f)
    {
        static thread_local char    buf[kStdBuf+1];
        buf[0] = buf[1] = buf[2] = buf[3] = f;
        auto [p,ec] = std::to_chars(buf+4, buf+kStdBuf, n, 16);
        return std::string_view(p-4,4);
    }

    std::string_view fmt_hex(uint32_t n, char f)
    {
        static thread_local char    buf[kStdBuf+1];
        buf[0] = buf[1] = buf[2] = buf[3] = buf[4] = buf[5] = buf[6] = buf[7] = f;
        auto [p,ec] = std::to_chars(buf+8, buf+kStdBuf, n, 16);
        return std::string_view(p-8,8);
    }

    std::string_view fmt_hex(uint64_t n, char f)
    {
        static thread_local char    buf[kStdBuf+1];
        buf[0] = buf[1] = buf[2] = buf[3] = buf[4] = buf[5] = buf[6] = buf[7] = buf[8] = buf[9] = buf[10] = 
            buf[11] = buf[12] = buf[13] = buf[14] = buf[15] = f;
        auto [p,ec] = std::to_chars(buf+16, buf+kStdBuf, n, 16);
        return std::string_view(p-16,16);
    }

    std::vector<std::string_view>  hard_wrap(const char*s, size_t n, size_t width)
    {
        std::vector<std::string_view>   ret;
        const char*                     z = s;
        size_t                          i = 0;
        iter32(s, n, [&](const char*p, char32_t){
            if(i >= width){
                ret.push_back(std::string_view(z, p));
                z   = p;
                i = 0;
            } else
                ++i;
        });
        if(i)
            ret.push_back(std::string_view(z, i));
        return ret;
    }
    
    std::vector<std::string_view>  hard_wrap(const std::string_view&s, size_t width)
    {
        return hard_wrap(s.data(), s.size(), width);
    }

    bool  is_alpha(const char*s, size_t n)
    {
        bool    ret = true;
        return iter32_abort(s, n, [&](char32_t ch) -> bool {
            ret = ret &&  is_alpha(ch);
            return ret;
        });
        return ret;
    }
    
    bool  is_alpha(const std::string_view&s)
    {
        return is_alpha(s.data(), s.size());
    }
    
    bool  is_blank(const char*s, size_t n)
    {
        bool    ret = true;
        return iter32_abort(s, n, [&](char32_t ch) -> bool {
            ret = ret &&  is_blank(ch);
            return ret;
        });
        return ret;
    }

    bool  is_blank(const std::string_view&s)
    {
        return is_blank(s.data(), s.size());
    }

    bool  is_greater_igCase(const std::string_view&a, const std::string_view&b)
    {
        return is_greater(compare_igCase(a,b));
    }

    bool  is_less_igCase(const std::string_view&a, const std::string_view&b)
    {
        return is_less( compare_igCase(a,b));
    }

    bool  is_similar(const std::string_view&a, const std::string_view&b)
    {
        return is_equal( compare_igCase(a, b));
    }

    bool  is_space(const std::string_view&a)
    {
        bool    ret = true;
        return iter32_abort(a, [&](char32_t ch) -> bool {
            ret = ret &&  is_space(ch);
            return ret;
        });
        return ret;
    }
    
    namespace {
        template <template <typename...> class Tmpl, typename... T>
        std::string  _joinString(const Tmpl<T...>& collection, const std::string_view& separator)
        {
            size_t  n   = collection.size() * separator.size();
            for(const auto& s : collection)
                n += s.size();
            std::string ret;
            ret.reserve(n);
            bool    f   = true;
            for(const auto&s : collection){
                if(f){
                    f   = false;
                } else {
                    ret += separator;
                }
                ret += s;
            }
            return ret;
        }

        template <template <typename...> class Tmpl, typename... T>
        std::string  _joinOther(const Tmpl<T...>& collection, const std::string_view& separator)
        {
            std::string ret;
            bool    f   = true;
            for(const auto&s : collection){
                if(f){
                    f   = false;
                } else {
                    ret += separator;
                }
                ret += to_string(s);
            }
            return ret;
        }
    }
    
    std::string  join(const std::list<std::string>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }
    
    std::string  join(const std::list<std::string_view>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }
    
    std::string  join(const std::set<std::string>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }
    
    std::string  join(const std::set<std::string,IgCase>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }
    
    std::string  join(const std::set<std::string_view>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }
    
    std::string  join(const std::set<std::string_view,IgCase>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }
    
    std::string  join(const std::set<uint16_t>& collection, const std::string_view& separator)
    {
        return _joinOther(collection, separator);
    }
    
    std::string  join(const std::set<uint32_t>& collection, const std::string_view& separator)
    {
        return _joinOther(collection, separator);
    }
    
    std::string  join(const std::vector<std::string>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }
    
    std::string  join(const std::vector<std::string_view>& collection, const std::string_view& separator)
    {
        return _joinString(collection, separator);
    }

    bool        matches_at(const std::string_view& haystack, size_t pos, const std::string_view& pattern)
    {
        return starts(haystack.substr(pos), pattern);
    }
    
    bool        matches_at_igCase(const std::string_view& haystack, size_t pos, const std::string_view& pattern)
    {
        return starts_igCase(haystack.substr(pos), pattern);
    }

    std::string     pad_left(const std::string_view& data, size_t minSize, char ch)
    {
        size_t  ccount  = count_characters(data);
        if(ccount >= minSize)
            return std::string(data);
        std::string ret(minSize-ccount, ch);
        ret += data;
        return ret;
    }
    
    std::string     pad_left(const std::string_view& data, size_t minSize, char32_t ch)
    {
        size_t  ccount  = count_characters(data);
        if(ccount >= minSize)
            return std::string(data);
            
        size_t  deficit = minSize - ccount;
        std::string bit = copy(to_string(ch));
        std::string ret;
        ret.reserve(deficit * bit.size() + data.size());
        for(size_t i = 0; i<deficit; ++i)
            ret += bit;
        ret += data;
        return ret;
    }
    
    std::string  pad_right(const std::string_view& data, size_t minSize, char ch)
    {
        size_t  ccount  = count_characters(data);
        std::string ret = copy(data);
        if(ccount < minSize)
            ret.append(minSize-ccount, ch);
        return ret;
    }

    std::string  pad_right(const std::string_view& data, size_t minSize, char32_t ch)
    {
        size_t  ccount  = count_characters(data);
        if(ccount >= minSize)
            return std::string(data);
            
        size_t  deficit = minSize - ccount;
        std::string_view bit = to_string(ch);
        std::string ret;
        ret.reserve(deficit * bit.size() + data.size());
        ret += data;
        for(size_t i = 0; i<deficit; ++i)
            ret += bit;
        return ret;
    }

    size_t  replace_all(std::string& data, char what, char with)
    {
        size_t  cnt = 0;
        for(char& ch : data){
            if(ch == what){
                ch  = with;
                ++cnt;
            }
        }
        return cnt;
    }

    size_t  replace_all_igCase(std::string&data, char what, char with)
    {
        size_t  cnt = 0;
        what    = to_lower(what);
        for(char& ch : data){
            if(to_lower(ch) == what){
                ch  = with;
                ++cnt;
            }
        }
        return cnt;
    }


    //  Need to keep this for future reference, though we'll need a less heap-destroying algorithm
    //QString         sanitize_path(const QString& path)
    //{
        //return sanitize_path(path.split('/')).join('/');
    //}

    //String          sanitize_path(const String& path)
    //{
        //return sanitize_path(path.qString());
    //}

    //QStringList     sanitize_path(const QStringList& path)
    //{
        //QStringList ret;
        //for(const QString& s : path){
            //if(s.isEmpty())
                //continue;
            //if(s == ".")
                //continue;
            //if(s == "..")
                //continue;
            //ret << s;
        //}
        //if(path.last().isEmpty())   // preserve the final empty for a "/"
            //ret << QString();
        //return ret;
    //}

    std::string&    set_if_empty(std::string&a, const std::string_view&b, bool fOverride)
    {
        if((a.empty() || fOverride) && !b.empty())
            a  = b;
        return a;
    }
    
    std::string     simplify(const char*s, size_t n, char sp)
    { 
        bool            first   = true;
        bool            space   = true;
        std::string     ret;
        
        if(s && n){
            ret.reserve(n);
            iter32(s, n, [&](const char* p, int len, char32_t ch){
                if(is_space(ch)){
                    space   = true;
                } else {
                    if(first){
                        first   = false;
                    } else if(space){
                        ret += sp;
                        space   = false;
                    }
                    ret.append(p, len);
                }
            });
        };
    
        return ret;
    }
    

    std::string     simplify(const std::string_view&sv, char sp)
    {
        return simplify(sv.data(), sv.size(), sp);
    }

    Vector<std::string_view>  split(const char* s, size_t n, size_t num)
    {
        Vector<std::string_view>    ret;
        if(s && n){
            if(!num)
                ++num;
                
            ret.reserve(num);
            size_t  j0  =  0;
            size_t  j1;
            
            for(size_t i = 0; i<num; j0=j1, ++i){
                j1  = std::min(((i+1)*n)/num, n);
                ret << std::string_view(s+j0, j1-j0);
            }
        }
        return ret;
    }

    Vector<std::string_view>  split(const std::string_view& s, size_t number)
    {
        return split(s.data(), s.size(), number);
    }

    Vector<std::string_view>  split(const char*s, size_t n, char ch)
    {
        Vector<std::string_view>  ret;
        if(s && n){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnchr(i, end-i, ch)); i = j+1)
                ret << std::string_view(i, j-i);
            ret << std::string_view(i, end-i);
        }
        return ret;
    }

    Vector<std::string_view>  split(const std::string_view&s, char ch)
    {
        return split(s.data(), s.size(), ch);
    }

    Vector<std::string_view>  split(const char* s, size_t n, char ch, size_t number)
    {
        Vector<std::string_view>  ret;
        if(s && n){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            if(number){
                for(i = s; (j = strnchr(i, end-i, ch)) && --number; i = j)
                    ret << std::string_view(i, j-i);
            }
            ret << std::string_view(i, end-i);
        }
        return ret;
    }

    Vector<std::string_view>  split(const std::string_view&s, char ch, size_t number)
    {
        return split(s.data(), s.size(), ch, number);
    }

    Vector<std::string_view>  split(const char* s, size_t n, char32_t ch)
    {
        Vector<std::string_view>    ret;
        if(s && n){
            const char*     z0  = s;
            iter32(s, n, [&](const char*z, char32_t wc){
                if(wc == ch){
                    ret.push_back(std::string_view(z0, z));
                    z0      = z;
                }
            });
            ret.push_back(std::string_view(z0, s+n));
        }
        return ret;
    }

    Vector<std::string_view>  split(const std::string_view&s, char32_t ch)
    {
        return split(s.data(), s.size(), ch);
    }
    
    Vector<std::string_view>  split(const char* s, size_t n, char32_t ch, size_t number)
    {
        Vector<std::string_view>    ret;
        if(s && n){
            const char*     z0  = s;
            if(number){
                iter32(s, n, [&](const char*z, char32_t wc){
                    if(wc == ch){
                        if(--number){
                            ret.push_back(std::string_view(z0, z));
                            z0      = z;
                        }
                    }
                });
            }
            ret.push_back(std::string_view(z0, s+n));
        }
        return ret;
    }
    
    Vector<std::string_view>  split(const std::string_view&s, char32_t ch, size_t number)
    {
        return split(s.data(), s.size(), ch, number);
    }

    Vector<std::string_view>  split(const char* s, size_t n, const char* p, size_t pn)
    {
        Vector<std::string_view>  ret;
        if(s && n){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnstr(i, end-i, p, pn)); i = j)
                ret << std::string_view(i, j-i);
            ret << std::string_view(i, end-i);
        }
        return ret;
    }
    
    Vector<std::string_view>  split(const std::string_view&s, const std::string_view&p)
    {
        return split(s.data(), s.size(), p.data(), p.size());
    }

    Vector<std::string_view>  split_igCase(const char* s , size_t n, char ch)
    {
        Vector<std::string_view>  ret;
        if(s && n){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnchr_igCase(i, end-i, ch)); i = j)
                ret << std::string_view(i, j-i);
            ret << std::string_view(i, end-i);
        }
        return ret;
    }
    
    Vector<std::string_view>  split_igCase(const std::string_view&s, char ch)
    {
        return split_igCase(s.data(), s.size(), ch);
    }

    Vector<std::string_view>  split_igCase(const char* s, size_t n, char32_t ch)
    {
        Vector<std::string_view>    ret;
        if(s && n){
            ch = to_lower(ch);
            const char*     z0  = s;
            iter32(s, n, [&](const char*z, char32_t wc){
                if(to_lower(wc) == ch){
                    ret.push_back(std::string_view(z0, z));
                    z0      = z;
                }
            });
            ret.push_back(std::string_view(z0, s+n));
        }
        return ret;
    }
    
    Vector<std::string_view>  split_igCase(const std::string_view&s, char32_t ch)
    {
        return split_igCase(s.data(), s.size(), ch);
    }
    
    Vector<std::string_view>  split_igCase(const char* s, size_t n, const char*p, size_t pn)
    {
        Vector<std::string_view>  ret;
        if(s && n){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnstr_igCase(i, end-i, p, pn)); i = j)
                ret << std::string_view(i, j-i);
            ret << std::string_view(i, end-i);
        }
        return ret;
    }
    
    Vector<std::string_view>  split_igCase(const std::string_view&s, const std::string_view&p)
    {
        return split_igCase(s.data(), s.size(), p.data(), p.size());
    }
    
    bool  starts(const char* s, size_t n, const char* pat, size_t nPath)
    {
        if(s && n && pat && (nPath>=n)){
            for(size_t i=0;i<nPath;++i)
                if(s[i] != pat[i])
                    return false;
            return true;
        }
        
        return false;
    }
    
    bool  starts(const std::string_view&s, const std::string_view& pat)
    {
        return starts(s.data(), s.size(), pat.data(), pat.size());
    }
    
    bool  starts_igCase(const char* s, size_t n, const char* pat, size_t nPath)
    {
        if(s && n && pat && (nPath>=n)){
            for(size_t i=0;i<nPath;++i)
                if(to_lower(s[i]) != to_lower(pat[i]))
                    return false;
            return true;
        }
        
        return false;
    }
    
    bool  starts_igCase(const std::string_view&s, const std::string_view& pat)
    {
        return starts_igCase(s.data(), s.size(), pat.data(), pat.size());
    }

    std::string_view    strip_extension(const std::string_view&name)
    {
        ssize_t  x   = (ssize_t) name.find_last_of('.');
        ssize_t  s   = (ssize_t) name.find_last_of('/');
        if(x > s)
            return name.substr(0,x);
        return name;
    }

    std::string  strip_spaces(const char* s, size_t n)
    {
        std::string  ret;
        ret.reserve(n);
        if(s && n){
            iter32(s, n, [&](const char*p, int len, char32_t ch){
                if(len && !is_space(ch)){
                    ret.append(p, len);
                }
            });
        }
        return ret;
    }
    
    std::string  strip_spaces(const std::string_view&s)
    {
        return strip_spaces(s.data(), s.size());
    }

    const char*  strnchr(const char*s, size_t n, char ch)
    {
        if(!s || !n)
            return nullptr;
        for(;*s && n; ++s, --n)
            if(*s == ch)
                return s;
        return nullptr;
    }

    const char*  strnchr(const char*s, size_t n, char32_t ch)
    {
        return iter32_find(s, n, [&](char32_t c) -> bool {
            return ch == c;
        });
    }

    const char*  strnchr_igCase(const char*s, size_t n, char ch)
    {
        ch      = to_lower(ch);
        if(!s || !n)
            return nullptr;
        for(;*s && n; ++s, --n)
            if(to_lower(*s) == ch)
                return s;
        return nullptr;
    }
    
    const char*  strnchr_igCase(const char*s, size_t n, char32_t ch)
    {   
        ch  = to_lower(ch);
        return iter32_find(s, n, [&](char32_t c) -> bool {
            return ch == to_lower(c);
        });
    }

    size_t       strnlen(const char32_t* z, size_t cb)
    { 
        size_t  n   = 0;
        for(; z && *z && cb; --cb, ++z, ++n)
            ;
        return n;
    }

    const char*  strnstr(const char* haystack, size_t nHay, const char* needle, size_t nNeedle)
    {
        //  weed outs
        if(!haystack || !needle || !nHay || (nHay < nNeedle)) 
            return nullptr;

        if(nNeedle == 1)    
            return strnchr(haystack, nHay, *needle);

        if(nNeedle <= 8){
            uint64_t    mask    = 0ULL;
            uint64_t    pattern = 0ULL;
            uint64_t    mem     = 0ULL;
            size_t      i;
            
                //  build up the mask & pattern
            for(i=0;i<nNeedle;++i){
                mask        = 0xFF | (mask << 8);
                pattern     = needle[i] | (pattern << 8);
            }
            
                //  prime
            for(i=0;i<(nNeedle-1);++i)
                mem         = haystack[i] | (mem << 8);
            
                //  search
            for(++i;i<nHay;++i){
                mem         = haystack[i] | (mem << 8);
                if((mem & mask) == pattern)
                    return haystack + i - nNeedle;
            }
        } else {
            size_t      i   = 0;
            size_t      j   = 0;
            for(i=0;i<nHay - nNeedle; ++i){
                if(haystack[i] == needle[0]){
                    bool    good    = true;
                    for(j=1;good && (j<nNeedle);++j){
                        if(haystack[i+j] != needle[j])
                            good    = false;
                    }
                    if(good)
                        return haystack+i;
                }
            }
        }
        return nullptr;
    }

    const char*  strnstr_igCase(const char* haystack, size_t nHay, const char* needle, size_t nNeedle)
    {
        //  weed outs
        if(!haystack || !needle || !nHay || (nHay < nNeedle)) 
            return nullptr;

        if(nNeedle == 1)    
            return strnchr_igCase(haystack, nHay, *needle);

        if(nNeedle <= 8){
            uint64_t    mask    = 0ULL;
            uint64_t    pattern = 0ULL;
            uint64_t    mem     = 0ULL;
            size_t      i;
            
                //  build up the mask & pattern
            for(i=0;i<nNeedle;++i){
                mask        = 0xFF | (mask << 8);
                pattern     = to_lower(needle[i]) | (pattern << 8);
            }
            
                //  prime
            for(i=0;i<(nNeedle-1);++i)
                mem         = to_lower(haystack[i]) | (mem << 8);
            
                //  search
            for(++i;i<nHay;++i){
                mem         = to_lower(haystack[i]) | (mem << 8);
                if((mem & mask) == pattern)
                    return haystack + i - nNeedle;
            }
        } else {
            size_t      i   = 0;
            size_t      j   = 0;
            for(i=0;i<nHay - nNeedle; ++i){
                if(to_lower(haystack[i]) == to_lower(needle[0])){
                    bool    good    = true;
                    for(j=1;good && (j<nNeedle);++j){
                        if(to_lower(haystack[i+j]) != to_lower(needle[j]))
                            good    = false;
                    }
                    if(good)
                        return haystack+i;
                }
            }
        }
        return nullptr;
    }
    
    boolean_r  to_boolean(const char*s, size_t n)
    {
        if(!s)
            return {};
        
        trim_ws(s, n);
        switch(*s){
        case '0':
            if(is_same(s,n,"0"))
                return bFALSE;
            break;
        case '1':
            if(is_same(s, n, "1"))
                return bTRUE;
            break;
        case 'y':
        case 'Y':
            if(n == 1)
                return bTRUE;
            if(is_same(s, n, "yes"))
                return bTRUE;
            break;
        case 'n':
        case 'N':
            if(n == 1)
                return bFALSE;
            if(is_same(s, n, "no"))
                return bFALSE;
            break;
        case 't':
        case 'T':
            if(n == 1)
                return bTRUE;
            if(is_same(s, n, "true"))
                return bTRUE;
            break;
        case 'f':
        case 'F':
            if(n==1)
                return bFALSE;
            if(is_same(s, n, "false"))
                return bFALSE;
            break;
        default:
            break;
        }
        
        return {};
    }

    boolean_r to_boolean(const std::string_view&s)
    {
        return to_boolean(s.data(), s.size());
    }

    double_r  to_double(const char*s, size_t n)
    {
        if(!s)
            return dFAIL;
        trim_ws(s, n);
        if(!n)
            return 0.;

        #if FP_CHARCONV
            double  result = NaN;
            auto [p,ec] = std::from_chars(s, s+n, result, std::chars_format::general);
            return double_r(ec == std::errc(), result);
        #else
            char*   z       = nullptr;
            double  res     = std::strtod(s, &z);
            if((const char*) s != z)
                return res;
            return dFAIL;
        #endif
    }

    double_r  to_double(const std::string_view&s)
    {
        return to_double(s.data(), s.size());
    }

    float_r  to_float(const char*s, size_t n)
    {
        if(!s)
            return fFAIL;
        trim_ws(s, n);
        if(!n)
            return 0.f;
        
        #if FP_CHARCONV
            float  result = NaNf;
            auto [p,ec] = std::from_chars(s, s+n, result, std::chars_format::general);
            return float_r(ec == std::errc(), result);
        #else
            char*   z       = nullptr;
            float  res     = std::strtof(s, &z);
            if((const char*) s != z)
                return res;
            return fFAIL;
        #endif
    }

    float_r  to_float(const std::string_view&s)
    {
        return to_float(s.data(), s.size());
    }

    integer_r  to_int(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<int>(s,n);
    }

    integer_r   to_int(const std::string_view&s)
    {
        return to_int(s.data(), s.size());
    }

    int8_r  to_int8(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<int8_t>(s,n);
    }

    int8_r  to_int8(const std::string_view&s)
    {
        return to_int8(s.data(), s.size());
    }


    int16_r  to_int16(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<int16_t>(s,n);
    }

    int16_r  to_int16(const std::string_view&s)
    {
        return to_int16(s.data(), s.size());
    }

    int32_r  to_int32(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<int32_t>(s,n);
    }

    int32_r  to_int32(const std::string_view&s)
    {
        return to_int32(s.data(), s.size());
    }


    int64_r  to_int64(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0LL;
        return int_from_chars<int64_t>(s,n);
    }

    int64_r  to_int64(const std::string_view&s)
    {
        return to_int64(s.data(), s.size());
    }

    integer_r  to_integer(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0L;
        return int_from_chars<int>(s,n);
    }

    integer_r  to_integer(const std::string_view&s)
    {
        return to_integer(s.data(), s.size());
    }

    std::string  to_lower(const std::string_view&s)
    {
        std::string ret;
        ret.reserve(s.size());
        iter32(s, [&](char32_t ch){
            ret += to_lower(ch);
        });
        return ret;
    }
    
    std::u32string  to_lower(const std::u32string_view&s)
    {
        std::u32string ret;
        ret.reserve(s.size());
        for(char32_t ch : s)
            ret += to_lower(ch);
        return ret;
    }

    short_r  to_short(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<short>(s,n);
    }

    short_r  to_short(const std::string_view&s)
    {
        return to_short(s.data(), s.size());
    }

    std::string_view   to_string(char32_t ch)
    {
        static thread_local char    buf[kStdBuf+1];
        std::mbstate_t  state{};
        int len = (int) c32rtomb(buf, ch, &state);
        return std::string_view(buf, len);
    }
    
    std::string   to_string(const char32_t*z)
    {
        if(!z)
            return std::string();
        return cvt_u32string(z, strnlen(z, MAX_NULL_TERM));
    }

    std::string_view   to_string(double v)
    {
        //  std::to_chars would be GREAT, if GCC implemented it.....
        static thread_local char    buf[kStdBuf+1];
        #if FP_CHARCONV
        int n  = snprintf(buf, kStdBuf, "%lg", v);
        return std::string_view(buf, n);
        #else
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
        #endif
    }
    
    std::string_view   to_string(float v)
    {
        //  std::to_chars would be GREAT, if GCC implemented it.....
        static thread_local char    buf[kStdBuf+1];
        #if FP_CHARCONV
        int n  = snprintf(buf, kStdBuf, "%lg", v);
        return std::string_view(buf, n);
        #else
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
        #endif
    }

    std::string_view   to_string(int8_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, (int) v);
        return std::string_view(buf, (p-buf));
    }
    
    std::string_view   to_string(int16_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
    }
    
    std::string_view   to_string(int32_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
    }

    std::string_view   to_string(int64_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
    }

    std::string_view  to_string(const std::u8string_view&s)
    {
        return std::string_view((const char*) s.data(), s.size());
    }

    std::string  to_string(const std::u32string_view&s)
    {
        return cvt_u32string(s.data(), s.size());
    }

    std::string to_string(const std::wstring_view&s)
    {
        return cvt_wstring(s.data(), s.size());
    }

    std::string_view   to_string(uint8_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
    }
    
    std::string_view   to_string(uint16_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
    }
    
    std::string_view   to_string(uint32_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
    }

    std::string_view   to_string(uint64_t v)
    {
        static thread_local char    buf[kStdBuf+1];
        auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
        return std::string_view(buf, (p-buf));
    }

    unsigned_r  to_uint(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<unsigned>(s,n);
    }

    unsigned_r  to_uint(const std::string_view&s)
    {
        return to_uint(s.data(), s.size());
    }

    uint8_r     to_uint8(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<uint8_t>(s,n);
    }

    uint8_r     to_uint8(const std::string_view&s)
    {
        return to_uint8(s.data(), s.size());
    }

    uint16_r    to_uint16(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<uint16_t>(s,n);
    }

    uint16_r    to_uint16(const std::string_view&s)
    {
        return to_uint16(s.data(), s.size());
    }

    uint32_r    to_uint32(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<uint32_t>(s,n);
    }

    uint32_r    to_uint32(const std::string_view&s)
    {
        return to_uint32(s.data(), s.size());
    }

    uint64_r    to_uint64(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0ULL;
        return int_from_chars<uint64_t>(s,n);
    }

    uint64_r    to_uint64(const std::string_view&s)
    {
        return to_uint64(s.data(), s.size());
    }

    unsigned_r  to_uinteger(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<unsigned int>(s,n);
    }

    unsigned_r  to_uinteger(const std::string_view&s)
    {
        return to_uinteger(s.data(), s.size());
    }

    unsigned_r  to_unsigned(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<unsigned int>(s,n);
    }

    unsigned_r  to_unsigned(const std::string_view&s)
    {
        return to_unsigned(s.data(), s.size());
    }

    ushort_r    to_ushort(const char*s, size_t n)
    {
        if(!s)
            return {};
        trim_ws(s,n);
        if(!n)
            return 0;
        return int_from_chars<unsigned short>(s,n);
    }


    ushort_r    to_ushort(const std::string_view&s)
    {
        return to_ushort(s.data(), s.size());
    }

    std::string_view    trimmed(const std::string_view&sv)
    {
        const char* s   = nullptr;
        const char* e   = nullptr;
        iter32(sv, [&](const char* z, char32_t ch){
            if(!is_space(ch)){
                e   = z;
                if(!s)
                    s   = z;
            }
        });
        
        if(s && e){
            return std::string_view(s, e+1);
        } else
            return std::string_view();
    }
    
    std::string_view    trimmed_end(const std::string_view&sv)
    {
        const char* e   = nullptr;
        iter32(sv, [&](const char* z, char32_t ch){
            if(!is_space(ch)){
                e   = z;
            }
        });
        
        if(e){
            return std::string_view(sv.data(), e+1);
        } else
            return std::string_view();
    }
    
    std::string_view    trimmed_start(const std::string_view&sv)
    {
        const char* s   = nullptr;
        iter32_abort(sv, [&](const char* z, char32_t ch) -> bool {
            if(!is_space(ch)){
                s   = z;
                return false;
            }
            return true;
        });
        if(s)
            return std::string_view(s, sv.data()+sv.size());
        return sv;
    }
    
}


 
