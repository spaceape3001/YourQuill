////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "StdFile.hpp"


#include <util/Logging.hpp>
#include <util/Reverse.hpp>
#include <util/Set.hpp>
#include <util/Stream.hpp>

#include <cctype>
#include <iostream>

bool        StdFile::empty() const
{
    return body.empty() && subs.empty();
}



bool        StdFile::read(Vector<char>&buffer, const std::string& fname) 
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


bool        StdFile::write(Vector<char>& buffer) 
{
    Stream  out(buffer);
    KVTree::write(out);
    if(!body.empty())
        out << '\n' << body;
    return true;
}


