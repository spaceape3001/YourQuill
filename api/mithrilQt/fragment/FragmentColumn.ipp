////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/fragment/FragmentCDB.hpp>

namespace yq::mithril::column {
    IdColumn    fragment_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    fragment_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    fragment_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_name();
        ret.label       = "Name";
        return ret;
    }

    IdColumn    fragment_path(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_path();
        ret.label       = "Path";
        return ret;
    }

    void    reg_fragment_columns()
    {
        IdColumn::declare<Fragment>(Column::Id,         fragment_id);
        IdColumn::declare<Fragment>(Column::Key,        fragment_key);
        IdColumn::declare<Fragment>(Column::Name,       fragment_name);
        IdColumn::declare<Fragment>(Column::Path,       fragment_path);
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  fragment_id()
    {
        return [](Id i) -> QVariant {
            Fragment    a   = i.as<Fragment>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  fragment_key()
    {
        return [](Id i) -> QVariant {
            Fragment    a   = i.as<Fragment>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  fragment_name()
    {
        return [](Id i) -> QVariant {
            Fragment    a   = i.as<Fragment>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
    
    IdColumn::VariantFN  fragment_path()
    {
        return [](Id i) -> QVariant {
            Fragment    a   = i.as<Fragment>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::path(a).string());
        };
    }
}
