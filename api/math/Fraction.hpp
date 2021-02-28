////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once 
#include "MathFwd.hpp"
#include <meta/MetaValue.hpp>

struct Fraction {
    int         num = 0;
    int         den = 1;
    
    static constexpr int gcd(int a,int b);
    static int ipow(int base, unsigned int exp);

    constexpr operator double() const { return (double) num / (double) den; }

    Fraction   simplified() const;
    
    constexpr Fraction operator+() const { return *this; }
    constexpr Fraction operator-() const { return Fraction{-num,den}; }
    
    Fraction operator+(const Fraction&) const;
    Fraction operator-(const Fraction&) const;
    Fraction operator*(const Fraction&) const;
    Fraction operator/(const Fraction&) const;
    /*! \brief power (integer only) */
    Fraction operator^(int) const;

    //Fraction operator+(const Fraction&) const;
    //Fraction operator-(const Fraction&) const;
    //Fraction operator*(const Fraction&) const;
    //Fraction operator/(const Fraction&) const;

    Fraction& operator+=(const Fraction&);
    Fraction& operator-=(const Fraction&);
    Fraction& operator*=(const Fraction&);
    Fraction& operator/=(const Fraction&);
    Fraction& operator^=(int);
    
    bool    operator==(const Fraction& rhs) const;
    bool    operator!=(const Fraction& rhs) const;
    bool    operator< (const Fraction& rhs) const;
    bool    operator> (const Fraction& rhs) const;
    bool    operator<=(const Fraction& rhs) const;
    bool    operator>=(const Fraction& rhs) const;
    
    String      to_string() const;
};

constexpr int Fraction::gcd(int a,int b)
{
    if(a<0) a = -a;
    if(b<0) b = -a;
    while(b != 0){
        a %= b;
        if(!a) return b;
        b %= a;
    }
    return a;
}

MV_DECLARE(Fraction)
