////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Attribute.hpp"

#include <kernel/db/CacheFwd.hpp>
#include <kernel/file/Document.hpp>
#include <vector>

namespace yq {
    namespace agw {
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

    }
    
    namespace cdb {

        /*! \brief Gets ALL attributes
        */
        agw::AttributeVector   all_attributes();

        /*! \brief Gets ALL attributes for given document
        */
        agw::AttributeVector    all_attributes(Document);
        size_t                  all_attributes_count();
        size_t                  all_attributes_count(Document);

        /*! \brief Top level attributes for given document
        */
        agw::AttributeVector    attributes(Document);
        size_t                  attributes_count(Document);


        agw::AttributeVector    child_attributes(agw::Attribute);
        size_t                  child_attributes_count(agw::Attribute);
        
        agw::Attribute          db_attribute(Document doc, agw::Attribute par, uint64_t idx, std::string_view k, std::string_view uid, std::string_view val);
        
        Document                document(agw::Attribute);

        //! Erases *ALL* attributes for specified document
        void                    erase_all_attributes(Document);

        bool                    exists(agw::Attribute);
        bool                    exists_attribute(uint64_t);

        //! Index in the file's list
        uint64_t                index(agw::Attribute);
        agw::Attribute::Info    info(agw::Attribute);
        

        std::string             key(agw::Attribute);

        //! Used in the change detection
        std::vector<agw::Attribute::KVUA>    kvua(agw::Attribute);
        //! Used in the change detection
        std::vector<agw::Attribute::KVUA>    kvua(Document);
        
        agw::Attribute          parent(agw::Attribute);
        
        std::string             uid(agw::Attribute);
        std::string             value(agw::Attribute);
    }
}

