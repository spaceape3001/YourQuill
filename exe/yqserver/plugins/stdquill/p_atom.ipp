////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void p_atom(WebHtml& h)
    {
        //   this will redirect the atom appropriately
        h << "TODO";
    }
    
    void p_atoms(WebHtml& h)
    {
        //   this will be a search-spot for the appropriate atoms
        h << "TODO";
    }

    void reg_atom()
    {
        reg_webpage<p_atom>("/atom").argument("ID", "Atom ID");
        reg_webpage<p_atoms>("/atoms");
    }
}
