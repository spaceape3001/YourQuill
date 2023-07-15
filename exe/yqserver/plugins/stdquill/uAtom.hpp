////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    i_atom_notify(Atom x, bool recursive=true);
    void    s3_atom_bind(Atom, bool recursive=true);
    Atom    s3_atom_create(KVTree& attrs, Document doc);
    void    s3_atom_notify(Atom x, bool recursive=true);
    Atom    u_atom(KVTree& attrs, Document doc);
}
