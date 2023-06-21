////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderColumn.hpp"
#include <mithril/folder/FolderCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    folder_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::folder_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    folder_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::folder_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    folder_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::folder_name();
        ret.label       = "Name";
        return ret;
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  folder_id()
    {
        return [](Id i) -> QVariant {
            Folder    a   = i.as<Folder>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  folder_key()
    {
        return [](Id i) -> QVariant {
            Folder    a   = i.as<Folder>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  folder_name()
    {
        return [](Id i) -> QVariant {
            Folder    a   = i.as<Folder>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
