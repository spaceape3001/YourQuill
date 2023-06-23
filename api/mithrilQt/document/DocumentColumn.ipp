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
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril::column {
    IdColumn    document_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::document_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::document_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    document_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::document_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::document_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    document_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::document_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::document_icon();
        ret.label               = "Name";
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

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  document_icon()
    {
        static QIcon    qico(":/generic/document.svg");
        return [](Id i) -> QVariant {
            Document   a   = i.as<Document>();
            if(!a)
                return QVariant();
            Image   img = cdb::icon(a);
            if(img)
                return qIcon(img);
            return qico;
        };
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
