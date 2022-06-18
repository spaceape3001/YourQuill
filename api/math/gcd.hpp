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

    /*! \brief Greatest common divisor in template parameters
    */
    template <int, int> struct GCD;
    template <int A> struct GCD<A,0> { 
        static constexpr const int V = A;
    };
    
    ;
    template <int A, int B> 
    struct GCD {
       static constexpr const int P   = (A>B)?(A-B):(B-A);
       static constexpr const int V   = GCD<B,P>::V;
    };
}
