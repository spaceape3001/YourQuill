////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/cdb_common.hpp>
#include <db/enum/sorted.hpp>

namespace yq {
    class Root;
    class ByteArray;
    struct Directory;
    struct Fragment;
    
    namespace cdb {

        Vector<Directory>          all_directories(const Root*, Sorted sorted=Sorted());
        size_t                     all_directories_count(const Root*);

        Vector<Fragment>           all_fragments(const Root*,Sorted sorted=Sorted());
        size_t                     all_fragments_count(const Root*);

        Vector<DirOrFrag>          children(const Root*,Sorted sorted=Sorted());

        Directory                       db_root(const Root*, bool *wasCreated=nullptr);

        Directory                       directory(const Root*);
        Directory                       directory(const Root*, std::string_view);
        
        Vector<Directory>           directories(const Root*, Sorted sorted=Sorted());
        
        size_t                          directories_count(const Root*);

        bool                            exists(const Root*, std::string_view );

        bool                            exists_fragment(uint64_t);

        Fragment                        fragment(const Root*, std::string_view );
        Vector<Fragment>                fragments(const Root*, Sorted sorted=Sorted());

        std::string                     key(const Root*);

        std::filesystem::path           path(const Root*, std::string_view , bool fMakePath=false);
        std::filesystem::path           path(const Root*, Document, bool fMakePath=false);          // goes to root

    }
}
