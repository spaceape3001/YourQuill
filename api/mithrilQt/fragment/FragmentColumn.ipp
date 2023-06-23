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
#include <QIcon>

namespace yq::mithril {
    QIcon qIcon(Fragment)
    {
        static QIcon s_ico(":/generic/fragment.svg");
        return s_ico;
    }
}

namespace yq::mithril::column {
    IdColumn    fragment_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::fragment_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::fragment_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    fragment_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::fragment_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::fragment_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    fragment_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::fragment_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::fragment_icon();
        ret.label               = "Name";
        return ret;
    }

    IdColumn    fragment_path(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::fragment_path();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::fragment_icon();
        ret.label               = "Path";
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

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  fragment_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Fragment>());
        };
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
