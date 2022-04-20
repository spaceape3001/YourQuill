////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

void    update_index(unsigned int opts)
{
    std::string r;
    Document    d   = cdb::first_document(kIndexFiles);
    if(d)
        r   = cdb::frag_string(cdb::first_fragment(d), opts);
    if(r.empty())
        r       = file_string(gSharedIndexFile);
    gIndex  = new PageTemplate(std::move(r));
}
