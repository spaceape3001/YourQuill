////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>

namespace yq {
    struct Absolute {
        double      threshhold;
        bool operator()(double error, double) const
        {
            return fabs(error) <= threshhold;
        }
    };
}
