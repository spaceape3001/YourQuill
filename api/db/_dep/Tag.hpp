////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagFile.hpp"
#include "FileSys.hpp"
#include <yq/enum/Update.hpp>

namespace yq {

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
        String      brief;
        Document    doc;
        String      key;
        //Leaf        leaf;
        String      name;
        bool operator==(const Info&) const = default;
    };
        
    namespace cdb {
        using SharedTagData     = std::shared_ptr<TagData>;

        TagVec                  all_tags(Sorted sorted=Sorted());
        size_t                  all_tags_count();
        
        String                  brief(Tag);

        Tag                     db_tag(Document, bool* wasCreated=nullptr);
        Tag                     db_tag(Fragment, bool* wasCreated=nullptr);
        Tag                     db_tag(const String&, bool* wasCreated=nullptr);
        TagVec                  db_tags(const StringSet&);


        Document                document(Tag);

        bool                    exists(Tag);

        bool                    exists_tag(uint64_t);
        Image                   icon(Tag);
        
        
        Tag::Info               info(Tag, bool autoKeyToName=false);
        

        String                  key(Tag);
        
        String                  label(Tag);
        Leaf                    leaf(Tag t);
        
        Tag                     make_tag(const String&, const Root* rt=nullptr);
        SharedTagData           merged(Tag, unsigned int opts=0);
        
        String                  name(Tag);
        
        
        NKI                     nki(Tag, bool autoKeyToName=false);

        QList<QVariant>         qvar_list(const Set<Tag>&);

            //! \brief Returns the FIRST trag fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        TagFile::Shared         read(Tag, const Root*);
        
        Vector<Tag::FragDoc>    reads(Tag);
        Vector<Tag::FragDoc>    reads(Tag, class Root*);
        
        bool                    set_brief(Tag, const String&, class Root* rt=nullptr);
        bool                    set_name(Tag, const String&, class Root* rt=nullptr);
        bool                    set_notes(Tag, const String&, class Root* rt=nullptr);
        
        
        Tag                     tag(const String&);
        Tag                     tag(uint64_t);
        
        TagFile::Shared         tag_doc(Fragment, bool fAllowEmpty=false);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        TagFile::Shared         write(Tag, const Root*);

    }
}
