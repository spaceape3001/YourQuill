////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

void    update_summary(unsigned int opts)
{
    std::string     r;
    Fragment    f   = cdb::first_fragment(cdb::document(".summary"));
    if(f)
        r   = cdb::frag_string(f);
    if(r.empty())
        r       = file_string(gSharedSummaryFile);
    gSummary = new WebTemplate(std::move(r));
}

