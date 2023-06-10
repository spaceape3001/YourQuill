////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdEncoding.hpp"
#include <mithril/root/RootDir.hpp>

uint64_t u64(const yq::mithril::RootDir* rt)
{
    return u64_encode(MType::Root, rt ? rt->id : 0ULL);
}


const yq::mithril::RootDir* u64_rootdir(uint64_t u)
{
    if(u64_type(u) != MType::Root)
        return nullptr;
    return yq::mithril::RootDir::by_id(u64_id(u));
}

