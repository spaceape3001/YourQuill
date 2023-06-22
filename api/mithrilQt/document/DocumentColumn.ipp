////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/document/DocumentCDB.hpp>

namespace yq::mithril::column {
    IdColumn    document_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::document_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    document_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::document_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    document_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::document_name();
        ret.label       = "Name";
        return ret;
    }

    void    reg_document_columns()
    {
        IdColumn::declare<Document>(Column::Id,    document_id);
        IdColumn::declare<Document>(Column::Key,   document_key);
        IdColumn::declare<Document>(Column::Name,  document_name);
    }
    
    YQ_INVOKE(reg_document_columns();)
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  document_id()
    {
        return [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  document_key()
    {
        return [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  document_name()
    {
        return [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
