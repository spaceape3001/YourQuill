////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentColumn.hpp"
#include <mithril/fragment/FragmentCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    fragment_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    fragment_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    fragment_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_name();
        ret.label       = "Name";
        return ret;
    }

    IdColumn    fragment_path()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::fragment_path();
        ret.label       = "Path";
        return ret;
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
