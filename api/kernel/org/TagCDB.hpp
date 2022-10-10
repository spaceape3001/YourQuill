////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/db/CacheFwd.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/leaf/Leaf.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/org/Tag.hpp>
#include <kernel/org/TagFile.hpp>

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
 
        /*! \brief Get all tags
            \param[in] sorted   Set to have tags sorted by key
        */
        std::vector<Tag>        all_tags(Sorted sorted=Sorted());
        /*! \brief Total number of tags
        */
        size_t                  all_tags_count();
        
        //! Searches for the best image for the tag
        Image                   best_image(Tag);
        /*! \brief Brief description of tag
        */
        std::string             brief(Tag);

        /*! \brief Creates/Gets a tag by document
        */
        Tag                     db_tag(Document, bool* wasCreated=nullptr);

        /*! \brief Creates/Gets a tag by fragment
        */
        Tag                     db_tag(Fragment, bool* wasCreated=nullptr);

        /*! \brief Creates/Gets a tag by key
        */
        Tag                     db_tag(std::string_view,  bool* wasCreated=nullptr);

        /*! \brief Creates tags by keys
        */
        std::vector<Tag>        db_tags(string_view_set_t);

        /*! \brief Creates tags by keys
        */
        std::vector<Tag>        db_tags(string_set_t);

        /*! \brief Gets the tag's document
        */
        Document                document(Tag);
        
        /*! \brief Checks for tags existance
        */
        bool                    exists(Tag);

        /*! \brief Checks for tags existance
        */
        bool                    exists_tag(uint64_t);
        
        /*! \brief Icon for tag
        */
        Image                   icon(Tag);
        
        /*! \brief Info for tag
        */
        Tag::Info               info(Tag, bool autoKeyToName=false);

        //bool                    is_tagged(Field, Tag);

        /*! \brief Key for tag
        */
        std::string             key(Tag);
        
        /*! \brief Label for tag
        */
        std::string             label(Tag);
        
        /*! \brief Leaf for tag
        */
        Leaf                    leaf(Tag t);
        
        /*! \brief Makes a tag
        */
        Tag                     make_tag(std::string_view, const Root* rt=nullptr, cdb_options_t opts=0, bool* wasCreated=nullptr);
        
        /*! \brief Merged data for tag
        
            Fuses the data from all roots and fragments, returns it.
        */
        Tag::SharedData         merged(Tag, cdb_options_t opts=0);
        
        /*! \brief Name for tag
        */
        std::string             name(Tag);
        
        /*! \brief Name, Key, and Icon for tag
        */
        NKI                     nki(Tag, bool autoKeyToName=false);
    
        //! \brief Returns the FIRST tag fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Tag::SharedFile         read(Tag, const Root*, cdb_options_t opts=0);
        
        /*! \brief All fragments for tag
        */
        std::vector<TagFragDoc> reads(Tag, cdb_options_t opts=0);

        /*! \brief Fragments for tag in root
        */
        std::vector<TagFragDoc> reads(Tag, class Root*, cdb_options_t opts=0);
        
        /*! \brief Sets the brief information for tag
        */
        bool                    set_brief(Tag, std::string_view, class Root* rt=nullptr);

        /*! \brief Sets the name  information for tag
        */
        bool                    set_name(Tag, std::string_view, class Root* rt=nullptr);

        /*! \brief Sets the notes information for tag
        */
        bool                    set_notes(Tag, std::string_view, class Root* rt=nullptr);
        
        /*! \brief Looks up tag by key
        */
        Tag                     tag(std::string_view);
        
        /*! \brief Looks up tag by ID
        */
        Tag                     tag(uint64_t);

        /*! \brief Looks up tag by document
        */
        Tag                     tag(Document, bool calc=false);
        
        //! \brief Tag File (&Data) for fragment
        Tag::SharedFile         tag_doc(Fragment, cdb_options_t opts=0);

        /*! \brief Tags for key-set */
        std::vector<Tag>        tags(const string_set_t&, bool noisy=false);
        /*! \brief Tags for key-set */
        std::set<Tag>           tags_set(const string_set_t&, bool noisy=false);
        /*! \brief Tags for key-set */
        std::set<Tag>           tags_set(const string_view_set_t&, bool noisy=false);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Tag::SharedFile         write(Tag, const Root*, cdb_options_t opts=0);
    };

}
