////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CacheFwd.hpp"
#include "Document.hpp"
#include <db/bit/KeyValue.hpp>
#include <db/enum/Change.hpp>
#include <util/Vector.hpp>
#include <QString>

/*! \brief Attribute of documents
*/
struct Attribute {
    uint64_t    id  = 0ULL;
    constexpr auto    operator<=>(const Attribute&rhs) const = default; 
    constexpr operator bool() const { return id != 0ULL; }

};


namespace cdb {

    struct KVU {
        QString     key, value, uid;
    };

    struct KVUA {
        QString     key, value, uid;
        Attribute   attr;
        uint64_t    idx;
    };
    
    /*! \brief Used for reporting the merged attributes between new and old
    
        If modified, the values will be of the *NEW* attriubtes
    */
    struct KVDiff {
        Vector<KVDiff>      sub;                //!< Sub values (assuming recursive)
        QString             key;                //<! Key of the attribute (if modified, the new one)
        QString             value;              //<! Value of the attribute (if modified, the new one)
        QString             uid;                //<! User-ID of the attriubte (if modified, the new one
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

    struct KVReport {
        Vector<KVDiff>      items;      //!< Items
        DocOrAttr           parent;
        bool                changed;     //!< TRUE/FALSE for any changes
    };

    Vector<Attribute>   all_attributes();
    Vector<Attribute>   all_attributes(Document);
    size_t              all_attributes_count();
    size_t              all_attributes_count(Document);
    

    //! Gets attributes (sub)
    Vector<Attribute>   attributes(Attribute);

    //! Gets top-level attirbutes
    Vector<Attribute>   attributes(Document);
    
    size_t              attributes_count(Attribute);
    size_t              attributes_count(Document);

    /*! \brief Reports on the change between old and the new
    */
    KVReport            changes(Document old, const KVTree&, ssize_t depth=-1);

    /*! \brief Reports on the change between old and the new
    */
    KVReport            changes(Attribute old, const KVTree&, ssize_t depth=-1);
    
    /*! \brief Reports on the change between old and the new
    
        \param[in]  old     Old vector of attributes
        \param[in]  kv      Vector of new attributes (from file)
        \param[in]  depth   Number of sub-levels to descend
        \return Vector, a union of old/new that reports how things have changed
    */
    KVReport            changes(const Vector<KVUA>& old, const KVTree&kv, ssize_t depth=-1);

    Document            document(Attribute);
    
    bool                exists(Attribute);
    bool                exists_attribute(uint64_t);

    //! Index in the file's list
    uint64_t            index(Attribute);
    

    QString             key(Attribute);
    KVU                 kvu(Attribute);
    Vector<KVUA>        kvua(Attribute);
    Vector<KVUA>        kvua(Document);
    
    
    DocOrAttr           parent(Attribute);
    
    //! User defined ID (from the file)
    QString             uid(Attribute);
    QString             value(Attribute);
}
