////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "folder.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Folder f)
    {
        static QIcon    qico(":/generic/folder.svg");
        if(!f)
            return QIcon();
        Image   img = cdb::icon(f);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    folder_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::folder_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::folder_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    folder_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::folder_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::folder_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    folder_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::folder_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::folder_icon();
        ret.label               = "Name";
        return ret;
    }

    void    reg_folder_columns()
    {
        IdColumn::declare<Folder>(Column::Id,    folder_id);
        IdColumn::declare<Folder>(Column::Key,   folder_key);
        IdColumn::declare<Folder>(Column::Name,  folder_name);

        IdColumn::set_defaultList<Folder>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Folder>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_folder_columns();)
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  folder_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Folder>());
        };
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
