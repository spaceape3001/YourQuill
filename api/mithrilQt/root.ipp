////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "root.hpp"
#include <0/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon qIcon(Root)
    {
        static QIcon s_ico(":/generic/root.svg");
        return s_ico;
    }
}

namespace yq::mithril::column {
    IdColumn    root_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::root_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::root_icon();
        ret.fnCompare           = compareFN::root_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    root_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::root_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::root_icon();
        ret.fnCompare           = compareFN::root_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    root_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::root_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::root_icon();
        ret.fnCompare           = compareFN::root_name();
        ret.label               = "Name";
        return ret;
    }

    IdColumn    root_path(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::root_path();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::root_icon();
        ret.fnCompare           = compareFN::root_path();
        ret.label               = "Path";
        return ret;
    }
    
    IdColumn    root_template(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::root_template();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::root_icon();
        ret.fnCompare           = compareFN::root_template();
        ret.label               = "Template";
        return ret;
    }

    void    reg_root_columns()
    {
        IdColumn::declare<Root>(Column::Id,         root_id);
        IdColumn::declare<Root>(Column::Key,        root_key);
        IdColumn::declare<Root>(Column::Name,       root_name);
        IdColumn::declare<Root>(Column::Path,       root_path);
        IdColumn::declare<Root>(Column::Template,   root_template);

        IdColumn::set_defaultList<Root>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Root>({{ Column::Key, ColOpt::Icon }, Column::Name, Column::Path });
    }
    
    YQ_INVOKE(reg_root_columns();)
}



namespace yq::mithril::compareFN {
    IdColumn::CompareFN root_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Root>().id, b.as<Root>().id);
        };
    }

    IdColumn::CompareFN root_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Root>()), cdb::key(b.as<Root>()));
        };
    }
    
    IdColumn::CompareFN root_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Root>()), cdb::name(b.as<Root>()));
        };
    }

    IdColumn::CompareFN root_path()
    {
        return [](Id a, Id b) -> Compare {
            return compare(cdb::path(a.as<Root>()), cdb::path(b.as<Root>()));
        };
    }

    IdColumn::CompareFN root_template()
    {
        return [](Id a, Id b) -> Compare {
            return compare(cdb::is_template(a.as<Root>()), cdb::is_template(b.as<Root>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  root_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Root>());
        };
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


