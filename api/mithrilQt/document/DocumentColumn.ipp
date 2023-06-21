////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentColumn.hpp"
#include <mithril/document/DocumentCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    document_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::document_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    document_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::document_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    document_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::document_name();
        ret.label       = "Name";
        return ret;
    }
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
