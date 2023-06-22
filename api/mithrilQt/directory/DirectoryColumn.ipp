////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/directory/DirectoryCDB.hpp>

namespace yq::mithril::column {
    IdColumn    directory_hidden(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_hidden();
        ret.label       = "Hidden";
        return ret;
    }
    
    IdColumn    directory_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    directory_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_key();
        ret.label       = "Key";
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
        ret.fnDisplay   = displayFN::directory_path();
        ret.label       = "Path";
        return ret;
    }

    void    reg_directory_columns()
    {
        IdColumn::declare<Directory>(Column::Hidden,     directory_hidden);
        IdColumn::declare<Directory>(Column::Id,         directory_id);
        IdColumn::declare<Directory>(Column::Key,        directory_key);
        IdColumn::declare<Directory>(Column::Name,       directory_name);
        IdColumn::declare<Directory>(Column::Path,       directory_path);
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
