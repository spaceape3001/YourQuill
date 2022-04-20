////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

void    update_page(unsigned int opts)
{
    Ref<WebTemplate>        wTemplate;
    std::string     r;
    
    for(Fragment f : fragments(document(kPage))){
        r       = frag_string(f, opts);
        if(r.empty())
            continue;
    }
    
    if(r.empty())
        r   = file_string(gSharedPageFile);
    
    wTemplate   = new WebTemplate(std::move(r));
    web::set_template(wTemplate);
}       



