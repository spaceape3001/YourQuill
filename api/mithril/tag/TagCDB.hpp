////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagInfo.hpp>
#include <mithril/tag/TagFile.hpp>

#define TBL_TAGS    "Tags"

namespace yq::mithril {
    using TagFragDoc    = std::pair<Fragment, Tag::SharedFile>;
}

        
namespace yq::mithril::cdb {

    /*! \brief Get all tags
        \param[in] sorted   Set to have tags sorted by key
    */
    TagVector               all_tags(Sorted sorted=Sorted());
    
    /*! \brief Total number of tags
    */
    size_t                  count_tags();

    /*! \brief Checks for tags existance
    */
    bool                    exists_tag(uint64_t);

    /*! \brief Looks up tag by ID
    */
    Tag                     find_tag(uint64_t);

    /*! \brief Looks up tag by key
    */
    Tag                     find_tag(std::string_view);
    
    /*! \brief Looks up tag by document
    */
    Tag                     find_tag(Document, bool calc=false);

    /*! \brief Tags for key-set */
    TagVector               find_tags(const string_set_t&, bool noisy=false);
    
    /*! \brief Tags for key-set */
    TagVector               find_tags(const string_view_set_t&, bool noisy=false);

    /*! \brief Tags for key-set */
    TagSet                  find_tags_set(const string_set_t&, bool noisy=false);
    
    /*! \brief Tags for key-set */
    TagSet                  find_tags_set(const string_view_set_t&, bool noisy=false);

    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  To distinguish the ones taking in a Tag as a primary argument
    
    //! Searches for the best image for the tag
    Image                   best_image(Tag);
    /*! \brief Brief description of tag
    */
    std::string             brief(Tag);

    /*! \brief Gets the tag's document
    */
    Document                document(Tag);

    /*! \brief Icon for tag
    */
    Image                   icon(Tag);
    
    /*! \brief Info for tag
    */
    Tag::Info               info(Tag, bool autoKeyToName=false);

    /*! \brief Key for tag
    */
    std::string             key(Tag);
    
    /*! \brief Label for tag
    */
    std::string             label(Tag);
    
    /*! \brief Leaf for tag
    */
    Leaf                    leaf(Tag t);
    
    /*! \brief Name for tag
    */
    std::string             name(Tag);
    
    /*! \brief Name, Key, and Icon for tag
    */
    NKI                     nki(Tag, bool autoKeyToName=false);


    /*! \brief Checks for tag's validity
    */
    bool                    valid(Tag);

    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  TODO (sort later)

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
    TagVector               db_tags(const string_view_set_t&);

    /*! \brief Creates tags by keys
    */
    TagVector               db_tags(const string_set_t&);

    
    /*! \brief Creates tags by keys
    */
    TagSet                  db_tags_set(const string_view_set_t&);

    /*! \brief Creates tags by keys
    */
    TagSet                  db_tags_set(const string_set_t&);

    
    //bool                    is_tagged(Field, Tag);

    /*! \brief Makes a tag
    */
    Tag                     make_tag(std::string_view, const RootDir* rt=nullptr, cdb_options_t opts=0, bool* wasCreated=nullptr);
    
    /*! \brief Merged data for tag
    
        Fuses the data from all roots and fragments, returns it.
    */
    Tag::SharedData         merged(Tag, cdb_options_t opts=0);
    

    //! \brief Returns the FIRST tag fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Tag::SharedFile         read(Tag, const RootDir*, cdb_options_t opts=0);
    
    /*! \brief All fragments for tag
    */
    std::vector<TagFragDoc> reads(Tag, cdb_options_t opts=0);

    /*! \brief Fragments for tag in root_dir
    */
    std::vector<TagFragDoc> reads(Tag, class RootDir*, cdb_options_t opts=0);
    
    /*! \brief Sets the brief information for tag
    */
    bool                    set_brief(Tag, std::string_view, class RootDir* rt=nullptr);

    /*! \brief Sets the name  information for tag
    */
    bool                    set_name(Tag, std::string_view, class RootDir* rt=nullptr);

    /*! \brief Sets the notes information for tag
    */
    bool                    set_notes(Tag, std::string_view, class RootDir* rt=nullptr);
    
    Tag                     tag(Document);
    
    //! \brief Tag File (&Data) for fragment
    Tag::SharedFile         tag_doc(Fragment, cdb_options_t opts=0);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Tag::SharedFile         write(Tag, const RootDir*, cdb_options_t opts=0);
};
