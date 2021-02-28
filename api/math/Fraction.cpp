////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Fraction.hpp"
#include <meta/MetaValueImpl.hxx>

int Fraction::ipow(int base, unsigned int exp)
{
    int result = 1;
    while(exp) {
        if(exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}


Fraction   Fraction::simplified() const
{
    int     n   = num;
    int     d   = den;
    if(den < 0){
        n   = - n;
        d   = - d;
    }
    int     cf  = (int) gcd((unsigned int) n, (unsigned int) d);
    return Fraction(n/cf,d/cf);
}

String   Fraction::to_string() const
{
    switch(den){
    case '0':
        return String::number(num) + "/ZERO";
    case '1':
        return String::number(num);
    default:
        return String::number(num) + "/" + String::number(den);
    }
}


Fraction Fraction::operator+(const Fraction&rhs) const
{
    return Fraction(num*rhs.den+den*rhs.num, den*rhs.den);
}

Fraction Fraction::operator-(const Fraction&rhs) const
{
    return Fraction(num*rhs.den-den*rhs.num, den*rhs.den);
}

Fraction Fraction::operator*(const Fraction&rhs) const
{
    return Fraction(num*rhs.num, den*rhs.den);
}

Fraction Fraction::operator/(const Fraction&rhs) const
{
    return Fraction(num*rhs.den, den*rhs.num);
}

Fraction Fraction::operator^(int rhs) const
{
    int     n   = 1;
    int     d   = 1;
    
    if(rhs < 0){
        n   = ipow(den, (unsigned int) -rhs);
        d   = ipow(num, (unsigned int) -rhs);
    } else {
        n   = ipow(num, rhs);
        d   = ipow(den, rhs);
    }
    return Fraction(n,d);
}

Fraction& Fraction::operator+=(const Fraction&rhs)
{
    // FIXME Non-optimal
    *this = *this + rhs;
    return *this;
}

Fraction& Fraction::operator-=(const Fraction&rhs)
{
    // FIXME Non-optimal
    *this = *this - rhs;
    return *this;
}

Fraction& Fraction::operator*=(const Fraction&rhs)
{
    // FIXME Non-optimal
    *this = *this * rhs;
    return *this;
}

Fraction& Fraction::operator/=(const Fraction&rhs)
{
    // FIXME Non-optimal
    *this = *this / rhs;
    return *this;
}

Fraction& Fraction::operator^=(int rhs)
{
    // FIXME Non-optimal
    *this = *this ^ rhs;
    return *this;
}

bool    Fraction::operator==(const Fraction& rhs) const
{
    return num*rhs.den == den*rhs.num;
}

bool    Fraction::operator!=(const Fraction& rhs) const
{
    return num*rhs.den != den*rhs.num;
}

bool    Fraction::operator< (const Fraction& rhs) const
{
    return num*rhs.den < den*rhs.num;
}

bool    Fraction::operator> (const Fraction& rhs) const
{
    return num*rhs.den > den*rhs.num;
}

bool    Fraction::operator<=(const Fraction& rhs) const
{
    return num*rhs.den <= den*rhs.num;
}

bool    Fraction::operator>=(const Fraction& rhs) const
{
    return num*rhs.den >= den*rhs.num;
}

MV_IMPLEMENT(Fraction)

