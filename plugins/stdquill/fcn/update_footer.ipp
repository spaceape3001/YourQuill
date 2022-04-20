////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

void    update_footer(unsigned int opts)
{
    std::string     r;
    Fragment    f   = cdb::first_fragment(cdb::document(".footer"));
    if(f)
        r   = cdb::frag_string(f, DONT_LOCK);
    if(r.empty())
        r       = file_string(gSharedFooterFile);
    gFooter = new WebTemplate(std::move(r));
}

