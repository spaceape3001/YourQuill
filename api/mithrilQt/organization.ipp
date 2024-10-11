////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "organization.hpp"
#include <yq/core/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/organization/OrganizationCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
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
        ret.fnCompare           = compareFN::organization_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    organization_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::organization_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::organization_icon();
        ret.fnCompare           = compareFN::organization_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    organization_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::organization_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::organization_icon();
        ret.fnCompare           = compareFN::organization_name();
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

namespace yq::mithril::compareFN {
    IdColumn::CompareFN organization_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Organization>().id, b.as<Organization>().id);
        };
    }
    
    IdColumn::CompareFN organization_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Organization>()), cdb::key(b.as<Organization>()));
        };
    }
    
    IdColumn::CompareFN organization_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Organization>()), cdb::name(b.as<Organization>()));
        };
    }
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

