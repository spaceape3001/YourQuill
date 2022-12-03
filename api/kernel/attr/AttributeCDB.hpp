////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Attribute.hpp"

#include <kernel/preamble.hpp>
#include <kernel/file/Document.hpp>
#include <vector>

namespace yq {
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
        std::vector<Attribute>  all_attributes();

        /*! \brief Gets ALL attributes for given document
        */
        std::vector<Attribute>  all_attributes(Document);
        size_t                  all_attributes_count();
        size_t                  all_attributes_count(Document);

        /*! \brief Top level attributes for given document
        */
        std::vector<Attribute>  attributes(Document);
        size_t                  attributes_count(Document);


        std::vector<Attribute>  child_attributes(Attribute);
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
        
        Attribute               parent(Attribute);
        
        std::string             uid(Attribute);
        std::string             value(Attribute);
    }
}

