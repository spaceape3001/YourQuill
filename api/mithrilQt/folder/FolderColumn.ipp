////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/folder/FolderCDB.hpp>

namespace yq::mithril::column {
    IdColumn    folder_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::folder_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    folder_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::folder_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    folder_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::folder_name();
        ret.label       = "Name";
        return ret;
    }

    void    reg_folder_columns()
    {
        IdColumn::declare<Folder>(Column::Id,    folder_id);
        IdColumn::declare<Folder>(Column::Key,   folder_key);
        IdColumn::declare<Folder>(Column::Name,  folder_name);
    }
    
    YQ_INVOKE(reg_folder_columns();)
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
