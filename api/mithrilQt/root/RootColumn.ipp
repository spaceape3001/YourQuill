////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootColumn.hpp"
#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    root_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::root_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    root_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::root_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    root_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::root_name();
        ret.label       = "Name";
        return ret;
    }

    IdColumn    root_path()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::root_path();
        ret.label       = "Path";
        return ret;
    }
    
    IdColumn    root_template()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::root_template();
        ret.label       = "Template";
        return ret;
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  root_id()
    {
        return [](Id i) -> QVariant {
            Root    a   = i.as<Root>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  root_key()
    {
        return [](Id i) -> QVariant {
            Root    a   = i.as<Root>();
            if(!a)
                return QVariant();
            const RootDir*  rt  = root_dir(a);
            if(!rt)
                return QVariant();
            return gluon::qString(rt->key);
        };
    }
    
    IdColumn::VariantFN  root_name()
    {
        return [](Id i) -> QVariant {
            Root    a   = i.as<Root>();
            if(!a)
                return QVariant();
            const RootDir*  rt  = root_dir(a);
            if(!rt)
                return QVariant();
            return gluon::qString(rt->name);
        };
    }
    
    IdColumn::VariantFN  root_path()
    {
        return [](Id i) -> QVariant {
            Root    a   = i.as<Root>();
            if(!a)
                return QVariant();
            const RootDir*  rt  = root_dir(a);
            if(!rt)
                return QVariant();
            return gluon::qString(rt->path.string());
        };
    }
    
    IdColumn::VariantFN  root_template()
    {
        return [](Id i) -> QVariant {
            Root    a   = i.as<Root>();
            if(!a)
                return QVariant();
            const RootDir*  rt  = root_dir(a);
            if(!rt)
                return QVariant();
            return rt->is_template;
        };
    }
}


