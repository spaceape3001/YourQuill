////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/Atom.hpp>
#include <kernel/attr/Attribute.hpp>
#include <kernel/notify/AtomNotifier.hpp>

namespace {
    void    u_atom(Atom, Document, const Attribute::Report&, cdb_options_t = 0, const ClassSet& cset=ClassSet());
    void    i_atom_create(const KVTree& attrs, Document doc, Atom par=Atom());
}

