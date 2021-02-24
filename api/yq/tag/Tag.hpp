////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagFile.hpp"
#include "yq/enum/Update.hpp"
#include "yq/file/FileSys.hpp"

class TagFile;

struct Tag {
    static constexpr const char*    szExtension = "tag";
    struct Info;
    uint64_t  id  = 0ULL;
    using FragDoc = std::pair<Fragment,TagFile::Shared>;
    constexpr auto    operator<=>(const Tag&rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};


struct Tag::Info {
    QString     brief;
    Document    doc;
    QString     key;
    //Leaf        leaf;
    QString     name;
    auto operator<=>(const Info&) const = default;
};
    
namespace cdb {
    Vector<Tag>             all_tags(Sorted sorted=Sorted());
    size_t                  all_tags_count();
    
    QString                 brief(Tag);

    Tag                     db_tag(Document, bool* wasCreated=nullptr);
    Tag                     db_tag(Fragment, bool* wasCreated=nullptr);
    Tag                     db_tag(const QString&, bool* wasCreated=nullptr);

    Document                document(Tag);

    bool                    exists(Tag);

    bool                    exists_tag(uint64_t);
    Image                   icon(Tag);
    
    
    Tag::Info               info(Tag, bool autoKeyToName=false);
    

    QString                 key(Tag);
    
    QString                 label(Tag);
    Leaf                    leaf(Tag t);
    
    TagFile::Shared         merged(Tag, unsigned int opts=0);
    
    QString                 name(Tag);
    
    
    NKI                     nki(Tag, bool autoKeyToName=false);
        //! \brief Returns the FIRST trag fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    TagFile::Shared         read(Tag, const Root*);
    
    Vector<Tag::FragDoc>    reads(Tag);
    Vector<Tag::FragDoc>    reads(Tag, class Root*);
    
    
    
    Tag                     tag(const QString&);
    Tag                     tag(uint64_t);
    
    TagFile::Shared         tag_doc(Fragment, bool fAllowEmpty=false);
    
    void                    update(Tag, UpdateFlags);

    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    TagFile::Shared         write(Tag, const Root*);

}
