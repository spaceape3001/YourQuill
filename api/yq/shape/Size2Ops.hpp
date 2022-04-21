////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Size2.hpp"

namespace yq {
    template <typename T>
    bool    within(const Size2<T>& big, const Size2<T>& small);
    
    template <typename T>
    auto    area(const Size2<T>& sz)
    {
        return sz.x*sz.y;
    }
    
    /*! \brief Shrinks a size to fit the frame
        
        This shrinks a size to fit the given frame (no spillage), preserving aspect ratio.
        
        \param[in]  dims    Size being shrunk
        \param[in]  frame   Desired frame
    */
    template <typename T>
    Size2<T>  shrink_to_fit_within(const Size2<T>& dims, const Size2<T>& frame)
    {
        using sq_t  = decltype(T()*T());
    
        if(within(frame, dims))
            return dims;
        if(area(frame) == sq_t{})  // frame is bogus
            return dims;
        if(area(dims) == sq_t{})    // dims is bogus
            return dims;

        /*
            The relative "shrink" factor in each dimension
            
            fx = dims.x / frame.x
            fy = dims.y / frame.y
            
            And the greater one dictates which axis is held constant 
            while the other is shrunk by the same amount.  However
            the above would decimate precision with integer numbers,
            therefore we need to rewrite the conditional to avoid.
            
            dims.y      dims.x
            -------  > --------
            frame.y     frame.x
            
            dims.y * frame.x > dim.x * frame.y
            
            Still have the issue of integer overflow... 
            address that later if that becomes a concern.
        */
        
        if(dims.y*frame.x > dims.x*frame.y){
            return Size2<T>{ (dims.x * frame.y)/dims.y , frame.y };
        } else {
            return Size2<T>{ frame.x, (dims.y*frame.x)/dims.x };
        }
        
    }

    template <typename T>
    bool   within(const Size2<T>& big, const Size2<T>& small)
    {
        return (big.x >= small.x) && (big.y >= small.y);
    }

}
