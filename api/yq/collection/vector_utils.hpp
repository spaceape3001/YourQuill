////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>

namespace yq {
    template <typename T, typename Alloc>
    std::vector<T, Alloc>&    operator+=(std::vector<T, Alloc>& a, const std::vector<T, Alloc>& b)
    {
        a.insert(a.end(), b.begin(), b.end());
        return a;
    }
}
