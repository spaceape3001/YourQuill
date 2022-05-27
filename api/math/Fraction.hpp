////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once 

#include <math/preamble.hpp>
#include <math/gcd.hpp>
#include <basic/meta/InfoBinder.hpp>
#include <basic/StreamOps.hpp>
#include <log4cpp/CategoryStream.hh>

namespace yq {


    template <typename I>
    struct Fraction {
        I   num = I{0};
        I   den = I{1};

        constexpr operator double() const { return (double) num / (double) den; }
    };

    template <typename I>
    constexpr auto i_power(std::make_signed_t<I> base, std::make_unsigned_t<I> exp)
    {
        std::make_signed_t<I>   result{1};
        for(; exp; exp >>= 1){
            if(exp & 1)
                result *= base;
            base *= base;
        }
        return result;
    }
    
    template <typename I>
    bool        is_valid(Fraction<I> a)
    {
        return a.den != I{0};
    }

    template <typename I>
    Fraction<I>  simplified(Fraction<I> a)
    {
        I   n   = (a.den > 0) ? a.num : -a.num;
        I   d   = (a.den > 0) ? a.den : -a.den;
        I   cf  = gcd(n, d);
        return Fraction<I>{ n, d };
    }
   
    template <typename I>
    constexpr Fraction<I> operator+(Fraction<I> a)
    {
        return a;
    }
   
    template <typename I>
    constexpr Fraction<I> operator-(Fraction<I> a)
    {
        return { -a.num, a.den };
    }
    
    template <typename I>
    constexpr Fraction<I> operator+(Fraction<I> a, Fraction<I> b)
    {
        return Fraction<I>{ a.num*b.den+a.den*b.num, a.den*b.den };
    }
    
    template <typename I>
    Fraction<I>&  operator+=(Fraction<I>&a, Fraction<I>b)
    {
        a = a + b;
        return a;
    }
    
    template <typename I>
    constexpr Fraction<I> operator-(Fraction<I> a, Fraction<I> b)
    {
        return Fraction<I>{ a.num*b.den-a.den*b.num, a.den*b.den };
    }
    
    template <typename I>
    Fraction<I>&  operator-=(Fraction<I>&a, Fraction<I>b)
    {
        a = a - b;
        return a;
    }
    
    template <typename I>
    constexpr Fraction<I> operator*(Fraction<I> a, Fraction<I>b)
    {
        return Fraction<I>{ a.num*b.num, a.den*b.den };
    }

    template <typename I>
    Fraction<I>&  operator*=(Fraction<I>&a, Fraction<I>b)
    {
        a = a * b;
        return a;
    }

    template <typename I>
    constexpr Fraction<I> operator/(Fraction<I> a, Fraction<I>b)
    {
        return Fraction<I>{ a.num*b.den, a.den*b.num };
    }

    template <typename I>
    Fraction<I>&  operator/=(Fraction<I>&a, Fraction<I>b)
    {
        a = a / b;
        return a;
    }

    template <typename I>
    Fraction<I> operator^(Fraction<I>a, I b)
    {
        I     n   = 1;
        I     d   = 1;
        
        if(b < 0){
            n   = i_power<I>(a.den, -b);
            d   = i_power<I>(a.num, -b);
        } else {
            n   = i_power<I>(a.num, b);
            d   = i_power<I>(a.den, b);
        }
        return Fraction<I>{n,d};
    }

    template <typename I>
    Fraction<I>&  operator^=(Fraction<I>&a, I b)
    {
        a = a ^ b;
        return a;
    }

    template <typename I>
    std::strong_ordering   operator<=>(Fraction<I>&a, Fraction<I>b)
    {
        auto    lt  = a.num*b.den;
        auto    rt  = a.den*b.den;
        
        if(lt == rt)
            return std::strong_ordering::equal;
        return (lt < rt) ? std::strong_ordering::less : std::strong_ordering::greater;
    }
    
   
    template <typename S, typename I>
    S&  as_stream(S& s, Fraction<I> f)
    {
        switch(f.den){
        case I{0}:
            s << f.num << "/ZERO";
            break;
        case I{1}:
            s << f.num;
            break;
        default:
            s << f.num << '/' << f.den;
            break;
        }
        return s;
    }
    
    template <typename I>
    Stream&     operator<<(Stream& s, Fraction<I> f)
    {
        return as_stream(s, f);
    }
   
    template <typename I>
    log4cpp::CategoryStream&     operator<<(log4cpp::CategoryStream& s, Fraction<I> f)
    {
        return as_stream(s, f);
    }
}

YQ_TYPE_DECLARE(yq::Frac8)
YQ_TYPE_DECLARE(yq::Frac16)
YQ_TYPE_DECLARE(yq::Frac32)
YQ_TYPE_DECLARE(yq::Frac64)

