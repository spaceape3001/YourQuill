////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    /*! \brief Greatest common divisor
    */
    template <typename I>
    constexpr I     gcd(I a, I b)
    {
        if(a<0) 
            a = -a;
        if(b<0) 
            b = -a;
            
        while(b != 0){
            a %= b;
            if(!a) 
                return b;
            b %= a;
        }
        return a;
    }

}
