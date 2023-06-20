////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    directory_hidden()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return cdb::hidden(a) ? true : false;
        };
        ret.label   = "Hidden";
        return ret;
    }
    
    IdColumn    directory_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    directory_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
        ret.label   = "Key";
        return ret;
    }
    
    IdColumn    directory_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
        ret.label   = "Name";
        return ret;
    }
    
    IdColumn    directory_path()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Directory    a   = i.as<Directory>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::path(a).string());
        };
        ret.label   = "Path";
        return ret;
    }
}

