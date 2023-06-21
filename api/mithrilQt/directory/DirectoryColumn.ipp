////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryColumn.hpp"
#include <mithril/directory/DirectoryCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    directory_hidden()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_hidden();
        ret.label       = "Hidden";
        return ret;
    }
    
    IdColumn    directory_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    directory_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    directory_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_name();
        ret.label       = "Name";
        return ret;
    }
    
    IdColumn    directory_path()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::directory_path();
        ret.label       = "Path";
        return ret;
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
