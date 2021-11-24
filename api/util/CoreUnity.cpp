////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Array2.hpp"
#include "Char8.hpp"
#include "Char32.hpp"
#include "Compare.hpp"
#include "Deque.hpp"
#include "DelayInit.hpp"
#include "Hash.hpp"
#include "List.hpp"
#include "Map.hpp"
#include "MultiMap.hpp"
#include "NaN.hpp"
#include "Set.hpp"
#include "Stack.hpp"
#include "StreamOps.hpp"
#include "String.hpp"
#include "StringUtil.hpp"
#include "StringView.hpp"
#include "Vector.hpp"


#include <cwctype>
#include <cctype>
#include <charconv>
#include <cstring>
#include <clocale>
#include <optional>

#include <QByteArray>
#include <QString>


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
    static constexpr const auto bFAIL     = boolean_r(false, false);
    
    static constexpr const auto dFAIL     = double_r(false, NaN);
    static constexpr const auto dZERO     = double_r(false, 0.);
    
    static constexpr const auto fFAIL     = float_r(false, NaNf);
    static constexpr const auto fZERO     = float_r(false, 0.f);
    
    static constexpr const auto iFAIL     = int_r(false, 0);
    static constexpr const auto i8FAIL    = int8_r(false, 0);
    static constexpr const auto i16FAIL   = int16_r(false, 0);
    static constexpr const auto i32FAIL   = int32_r(false, 0);
    static constexpr const auto i64FAIL   = int64_r(false, 0);
    
    static constexpr const auto sFAIL     = short_r(false, 0);

    static constexpr const auto uFAIL     = unsigned_r(false, 0.);
    static constexpr const auto u8FAIL    = uint8_r(false, 0);
    static constexpr const auto u16FAIL   = uint16_r(false, 0);
    static constexpr const auto u32FAIL   = uint32_r(false, 0);
    static constexpr const auto u64FAIL   = uint64_r(false, 0);
    
    static constexpr const auto usFAIL    = ushort_r(false, 0);


    template <typename T>
    Result<T>   int_from_chars(const char*s, size_t n, int base=10)
    {
        T    res = 0;
        auto [p,ec] = std::from_chars(s, s+n, res, base);
        return Result<T>(ec == std::errc(), res);
    }
}


#include "Char8.ipp"
#include "Char32.ipp"
#include "DelayInit.ipp"
#include "Enum.ipp"
#include "IgCase.ipp"
#include "Random.ipp"
#include "Ref.ipp"
#include "StreamOps.ipp"
#include "String.ipp"
#include "StringUtil.ipp"
#include "StringView.ipp"
#include "Utilities.ipp"
