////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "attribute-cdb.hpp"

namespace yq {
    struct KVTree;
    struct KeyValue;
    
    
    /*! \brief Used for reporting the merged attributes between new and old
    
        If modified, the values will be of the *NEW* attriubtes
    */
    struct Attribute::Diff {
        std::vector<Diff>   sub;                //!< Sub values (assuming recursive)
        std::string         key;                //<! Key of the attribute (if modified, the new one)
        std::string         value;              //<! Value of the attribute (if modified, the new one)
        std::string         uid;                //<! User-ID of the attriubte (if modified, the new one
        Attribute           attr;               //<! Attribute of any existing 
        uint64_t            nidx    = 0;        //<! Index of the new Attribute, 
        uint64_t            oidx    = 0;        //<! Index of the old attribute
        unsigned            chg     = 0;        //<! Changed flags
        
        //  Change bits
        enum {
            INSERT          = 1,                //!< Item was added
            DELETE          = 2,                //!< Item was removed
            MODIFY          = 3,                //!< Item was trivially modified (ie, case change)
            XMASK           = 3,                //!< Mask for change type
            INDEX           = 4,                //!< Index was changed (due to insert/modifyed)
            SUB             = 8                 //!< A sub (or sub-sub, etc) has one o rmore changes
        };
        
        bool            added() const { return (chg & XMASK) == INSERT; }
        bool            deleted() const { return (chg & XMASK) == DELETE; }
        bool            modified() const { return (chg & XMASK) == MODIFY; }
        bool            unchanged() const { return (chg & XMASK) == 0; }
        bool            indexed() const { return (chg & INDEX) != 0; }
        bool            subchanged() const { return (chg & SUB) != 0; }
    };

    struct Attribute::Report {
        std::vector<Diff>   items;      //!< Items
        Attribute           parent;     //!< Attribute 
        Document            document;
        bool                changed = false;     //!< TRUE/FALSE for any changes
        
        void                exec_inserts();
        void                exec_reindex();
        void                exec_removals();
    };

    namespace diff {
        /*! \brief Reports on the change between old and the new
        */
        Attribute::Report    changes(Document old, const KVTree& newAttrs, ssize_t depth=-1);

        /*! \brief Reports on the change between old and the new
        */
        Attribute::Report    changes(Attribute old, const KVTree& newAttrs, ssize_t depth=-1);
        
        /*! \brief Reports on the change between old and the new
        
            \param[in]  old         Old vector of attributes
            \param[in]   newAttrs Vector of new attributes (from file)
            \param[in]  depth       Number of sub-levels to descend
            \return Vector, a union of old/new that reports how things have changed
        */
        Attribute::Report    changes(const Vector<Attribute::KVUA>& old, const KVTree& newAttrs, ssize_t depth=-1);
    }
}

