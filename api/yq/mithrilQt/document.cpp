////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "document.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithrilQt/image.hpp>
#include <QFile>
#include <QIcon>

namespace yq::mithril {
    QIcon qIcon(Document doc)
    {
        static QIcon                                qico(":/generic/document.svg");
        static std::map<std::string, QIcon, IgCase> qrepo;
        
        if(!doc)
            return QIcon();
            
        Image   img = cdb::icon(doc);
        if(img)
            return qIcon(img);
        
        cdb::Extension   ext = cdb::suffix(doc);
        if(!ext.ext.empty()){
            auto [i,f]  = qrepo.try_emplace(ext.ext, qico);
            if(f){
                QString     fname   = QString(":/ext/%1.svg").arg(QString::fromStdString(ext.ext)).toLower();
                if(QFile::exists(fname)){
                    i->second   = QIcon(fname);
                    if(i->second.isNull())
                        i->second   = qico;
                }
            }
            return i->second;
        }
        
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    document_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::document_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::document_icon();
        ret.fnCompare           = compareFN::document_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    document_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::document_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::document_icon();
        ret.fnCompare           = compareFN::document_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    document_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::document_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::document_icon();
        ret.fnCompare           = compareFN::document_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_document_columns()
    {
        IdColumn::declare<Document>(Column::Id,    document_id);
        IdColumn::declare<Document>(Column::Key,   document_key);
        IdColumn::declare<Document>(Column::Name,  document_name);

        IdColumn::set_defaultList<Document>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Document>({{ Column::Key, ColOpt::Icon },  Column::Hidden, Column::Path });
    }
    
    YQ_INVOKE(reg_document_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN document_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Document>().id, b.as<Document>().id);
        };
    }
    
    IdColumn::CompareFN document_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Document>()), cdb::key(b.as<Document>()));
        };
    }
    
    IdColumn::CompareFN document_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Document>()), cdb::name(b.as<Document>()));
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  document_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Document>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  document_id()
    {
        return [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  document_key()
    {
        return [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  document_name()
    {
        return [](Id i) -> QVariant {
            Document    a   = i.as<Document>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
