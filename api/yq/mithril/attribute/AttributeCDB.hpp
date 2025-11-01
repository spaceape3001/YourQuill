////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/attribute/Attribute.hpp>
#include <yq/mithril/preamble.hpp>
#include <yq/mithril/document/Document.hpp>
#include <vector>

#define TBL_ATTRIBUTES      "Attributes"

namespace yq::mithril {
    struct Attribute::Info {
        std::string     key, value, uid;
        Document        doc;
        Attribute       parent;
        uint64_t        idx     = 0;
    };

    struct Attribute::KVUA {
        std::string     key, value, uid;
        Attribute       attr;   //!< Attribute of this one
        uint64_t        idx     = 0;
    };

    namespace cdb {

        /*! \brief Gets ALL attributes
        */
        AttributeVector         all_attributes(Sorted sorted={});

        /*! \brief Gets ALL attributes for given document
        */
        AttributeVector         all_attributes(Document);
        size_t                  all_attributes_count();
        size_t                  all_attributes_count(Document);

        /*! \brief Top level attributes for given document
        */
        AttributeVector         attributes(Document);
        size_t                  attributes_count(Document);


        AttributeVector         child_attributes(Attribute);
        size_t                  child_attributes_count(Attribute);
        
        Attribute               db_attribute(Document doc, Attribute par, uint64_t idx, std::string_view k, std::string_view uid, std::string_view val);
        
        Document                document(Attribute);

        //! Erases *ALL* attributes for specified document
        void                    erase_all_attributes(Document);

        bool                    exists(Attribute);
        bool                    exists_attribute(uint64_t);

        //! Index in the file's list
        uint64_t                index(Attribute);
        Attribute::Info         info(Attribute);
        

        std::string             key(Attribute);

        //! Used in the change detection
        std::vector<Attribute::KVUA>    kvua(Attribute);
        //! Used in the change detection
        std::vector<Attribute::KVUA>    kvua(Document);
        
        //! Just the key, but rename for auto-id
        std::string             name(Attribute);

        Attribute               parent(Attribute);
        
        std::string             uid(Attribute);
        std::string             value(Attribute);
    }
}
