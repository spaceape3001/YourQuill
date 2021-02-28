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
    return body.empty() && attrs.empty();
}



bool        StdFile::read(Vector<char>&buffer, const std::string& fname) 
{
    return AttrTree::parse(buffer, (has_body()?&body:nullptr), recursive_attributes(), fname);
}

void        StdFile::reset() 
{
    attrs.clear();
    body.clear();
}

namespace {
}


bool        StdFile::write(Vector<char>& buffer) 
{
    Stream  out(buffer);
    AttrTree::write(out);
    if(!body.empty())
        out << '\n' << body;
    return true;
}


