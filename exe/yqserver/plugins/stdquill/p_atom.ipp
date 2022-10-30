////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void page_atom(WebHtml& h)
    {
        //   this will redirect the atom appropriately
        h << "TODO";
    }
    
    void page_atoms(WebHtml& h)
    {
        //   this will be a search-spot for the appropriate atoms
        h << "TODO";
    }

    void reg_atom()
    {
        reg_webpage<page_atom>("/atom").argument("ID", "Atom ID");
        reg_webpage<page_atoms>("/atoms");
    }
}
