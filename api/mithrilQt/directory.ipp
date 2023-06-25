////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directory.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon qIcon(Directory)
    {
        static QIcon s_ico(":/generic/directory.svg");
        return s_ico;
    }
}


namespace yq::mithril::column {
    IdColumn    directory_hidden(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::directory_hidden();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::directory_icon();
        ret.label               = "Hidden";
        return ret;
    }
    
    IdColumn    directory_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::directory_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::directory_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    directory_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::directory_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::directory_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    directory_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_name();
        ret.label       = "Name";
        return ret;
    }
    
    IdColumn    directory_path(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::directory_path();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::directory_icon();
        ret.label               = "Path";
        return ret;
    }

    void    reg_directory_columns()
    {
        IdColumn::declare<Directory>(Column::Hidden,     directory_hidden);
        IdColumn::declare<Directory>(Column::Id,         directory_id);
        IdColumn::declare<Directory>(Column::Key,        directory_key);
        IdColumn::declare<Directory>(Column::Name,       directory_name);
        IdColumn::declare<Directory>(Column::Path,       directory_path);

        IdColumn::set_defaultList<Directory>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Directory>({{ Column::Key, ColOpt::Icon },  Column::Hidden, Column::Path });
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  directory_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Directory>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  directory_hidden()
    {
        return [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return cdb::hidden(a) ? true : false;
        };
    }
    
    IdColumn::VariantFN  directory_id()
    {
        return [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  directory_key()
    {
        return [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  directory_name()
    {
        return [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
    
    IdColumn::VariantFN  directory_path()
    {
        return [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::path(a).string());
        };
    }
}