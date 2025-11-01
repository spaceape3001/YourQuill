////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/atom/AtomProvider.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/tag/Tag.hpp>

namespace yq::mithril::provider {
    IdProvider all_atoms()
    {
        return [](){
            return ids(cdb::all_atoms(Sorted::YES));
        };
    }

    IdProvider  all_atom(Class x)
    {
        return [x](){
            return ids(cdb::all_atoms(x, Sorted::YES));
        };
    }

    IdProvider  all_atoms(Tag x)
    {
        return [x](){
            return ids(cdb::all_atoms(x, Sorted::YES));
        };
    }
}

