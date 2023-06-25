////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "organization.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/organization/OrganizationCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Organization ch)
    {
        static QIcon    qico(":/generic/organization.svg");
        if(!ch)
            return QIcon();
        Image   img = cdb::icon(ch);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    organization_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::organization_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::organization_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    organization_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::organization_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::organization_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    organization_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::organization_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::organization_icon();
        ret.label               = "Name";
        return ret;
    }

    void    reg_organization_columns()
    {
        IdColumn::declare<Organization>(Column::Id,    organization_id);
        IdColumn::declare<Organization>(Column::Key,   organization_key);
        IdColumn::declare<Organization>(Column::Name,  organization_name);

        IdColumn::set_defaultList<Organization>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Organization>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_organization_columns();)
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  organization_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Organization>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     organization_id()
    {
        return [](Id i) -> QVariant {
            Organization    a   = i.as<Organization>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN     organization_key()
    {
        return [](Id i) -> QVariant {
            Organization    a   = i.as<Organization>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN     organization_name()
    {
        return [](Id i) -> QVariant {
            Organization    a   = i.as<Organization>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}

