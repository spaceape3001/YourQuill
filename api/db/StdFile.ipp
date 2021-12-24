////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

bool        StdFile::empty() const
{
    return body.empty() && subs.empty();
}



bool        StdFile::read(ByteArray&&buffer, const std::string& fname) 
{
    return KVTree::parse(buffer, (has_body()?&body:nullptr), recursive_attributes(), fname);
}

void        StdFile::reset() 
{
    subs.clear();
    body.clear();
}

namespace {
}


bool        StdFile::write(yq::Stream& buffer) const
{
    KVTree::write(buffer);
    if(!body.empty())
        buffer << '\n' << body;
    return true;
}


