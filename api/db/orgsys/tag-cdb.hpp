////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/cdb_common.hpp>
#include <db/filesys/document.hpp>
#include <db/docsys/leaf.hpp>
#include <db/enum/sorted.hpp>
#include <db/imagesys/image.hpp>
#include <db/orgsys/tag.hpp>
#include <db/orgsys/tag-file.hpp>

namespace yq {
    
    struct Tag::Info {
        std::string     brief;
        Document        doc;
        std::string     key;
        Image           icon;
        Leaf            leaf;
        std::string     name;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        using TagFragDoc    = std::pair<Fragment, Tag::SharedFile>;
 
        std::vector<Tag>        all_tags(Sorted sorted=Sorted());
        size_t                  all_tags_count();
        
        //! Searches for the best image for the tag
        Image                   best_image(Tag);
        std::string             brief(Tag);

        Tag                     db_tag(Document, bool* wasCreated=nullptr);
        Tag                     db_tag(Fragment, bool* wasCreated=nullptr);
        Tag                     db_tag(std::string_view,  bool* wasCreated=nullptr);
        std::vector<Tag>        db_tags(string_view_set_t);
        std::vector<Tag>        db_tags(string_set_t);

        Document                document(Tag);

        void                    erase(Tag);
        bool                    exists(Tag);
        bool                    exists_tag(uint64_t);
        Image                   icon(Tag);
        
        Tag::Info               info(Tag, bool autoKeyToName=false);

        //bool                    is_tagged(Field, Tag);

        std::string             key(Tag);
        
        std::string             label(Tag);
        Leaf                    leaf(Tag t);
        
        Tag                     make_tag(std::string_view, const Root* rt=nullptr, unsigned int opts=0);
        Tag::SharedData         merged(Tag, unsigned int opts=0);
        
        std::string             name(Tag);
        
        NKI                     nki(Tag, bool autoKeyToName=false);
    
        //! \brief Returns the FIRST tag fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Tag::SharedFile         read(Tag, const Root*, unsigned int opts=0);
        
        std::vector<TagFragDoc> reads(Tag, unsigned int opts=0);
        std::vector<TagFragDoc> reads(Tag, class Root*, unsigned int opts=0);
        
        bool                    set_brief(Tag, std::string_view, class Root* rt=nullptr);
        bool                    set_name(Tag, std::string_view, class Root* rt=nullptr);
        bool                    set_notes(Tag, std::string_view, class Root* rt=nullptr);
        
        Tag                     tag(std::string_view);
        Tag                     tag(uint64_t);
        
        Tag::SharedFile         tag_doc(Fragment, unsigned int opts=0);

        std::vector<Tag>        tags(const string_set_t&, bool noisy=false);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Tag::SharedFile         write(Tag, const Root*, unsigned int opts=0);
    };

}
