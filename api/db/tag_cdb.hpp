////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb_common.hpp"
#include "document.hpp"
#include "image.hpp"
#include "tag_file.hpp"

#include <yq/enum/Sorted.hpp>

namespace yq {
    
    struct Tag::Info {
        std::string     brief;
        Document        doc;
        std::string     key;
        Image           icon;
        //Leaf        leaf;
        std::string     name;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        using TagFragDoc    = std::pair<Fragment, Tag::SharedFile>;
 
        Vector<Tag>             all_tags(Sorted sorted=Sorted());
        size_t                  all_tags_count();
        
        //! Searches for the best image for the tag
        Image                   best_image(Tag);
        std::string             brief(Tag);

        Tag                     db_tag(Document, bool* wasCreated=nullptr);
        Tag                     db_tag(Fragment, bool* wasCreated=nullptr);
        Tag                     db_tag(std::string_view,  bool* wasCreated=nullptr);
        Vector<Tag>             db_tags(string_view_set_t);
        Vector<Tag>             db_tags(string_set_t);

        Document                document(Tag);

        void                    erase(Tag);
        bool                    exists(Tag);
        bool                    exists_tag(uint64_t);
        Image                   icon(Tag);
        
        Tag::Info               info(Tag, bool autoKeyToName=false);

        std::string             key(Tag);
        
        std::string             label(Tag);
        //Leaf                    leaf(Tag t);
        
        Tag                     make_tag(std::string_view, const Root* rt=nullptr, unsigned int opts=0);
        Tag::SharedData         merged(Tag, unsigned int opts=0);
        
        std::string             name(Tag);
        
        NKI                     nki(Tag, bool autoKeyToName=false);
    
        //! \brief Returns the FIRST tag fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Tag::SharedFile         read(Tag, const Root*, unsigned int opts=0);
        
        Vector<TagFragDoc>      reads(Tag, unsigned int opts=0);
        Vector<TagFragDoc>      reads(Tag, class Root*, unsigned int opts=0);
        
        bool                    set_brief(Tag, std::string_view, class Root* rt=nullptr);
        bool                    set_name(Tag, std::string_view, class Root* rt=nullptr);
        bool                    set_notes(Tag, std::string_view, class Root* rt=nullptr);
        
        Tag                     tag(std::string_view);
        Tag                     tag(uint64_t);
        
        Tag::SharedFile         tag_doc(Fragment, unsigned int opts=0);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Tag::SharedFile         write(Tag, const Root*, unsigned int opts=0);
    };

}
