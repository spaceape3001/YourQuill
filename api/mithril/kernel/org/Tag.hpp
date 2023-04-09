////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/preamble.hpp>

namespace yq {
    /*! Tag in the cache database
    */
    struct Tag {
        static constexpr const char*    szExtension = "tag";
        struct Info;
        struct Data;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        using Lock  = IDLock<Tag>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Tag&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }

        //  --------------------------------------
        //  These all QUERY the cache (either direct or via CURL)
        //  --------------------------------------

        static std::vector<Tag> all(Sorted sorted=Sorted{});
        static std::vector<Tag> all(Atom, Sorted sorted=Sorted{});
        static std::vector<Tag> all(const string_set_t&);
        static std::vector<Tag> all(const string_view_set_t&);
        
        static size_t           count();
        static size_t           count(Atom);

        static bool             exists(uint64_t);
        
        static Tag              find(uint64_t);
        static Tag              find(std::string_view);
        static Tag              find(Document);
        
        
        std::string             brief() const;
        Document                document() const;
        Image                   icon() const;
        Info                    info(bool autoKeyToName=false) const;
        std::string             key() const;
        std::string             label() const;
        Leaf                    leaf() const;
        std::string             name() const;
        NKI                     nki(bool autoKeyToName=false) const;
        bool                    valid() const;
    };
    
}

YQ_TYPE_DECLARE(yq::Tag)
