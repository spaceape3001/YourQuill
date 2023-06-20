////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    document_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    document_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
        ret.label   = "Key";
        return ret;
    }
    
    IdColumn    document_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
        ret.label   = "Name";
        return ret;
    }
}

