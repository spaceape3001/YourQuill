////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ClassTableModel.hpp"
#include "ClassTableView.hpp"
#include <gui/model/StdTableModelImpl.hpp>
#include <gui/model/U64TableModelImpl.hpp>
#include <QHeaderView>

template class StdTableModel<Class>;
template class U64TableModel<Class>;
template class StdTableView<Class>;

#if 0
namespace {
    QString     stringify(const Vector<const Root*> rts)
    {
        QString     ret;
        for(const Root* r : rts)
            ret += " " + r -> key();
        return ret.isEmpty() ? QString() : ret.mid(1);
    }

    static Map<QString,CTMColumn,IgCase>        makeColumns() {
        static const CTMColumn kColumns[] = {
            { "id", QObject::tr("ID"), 
                [](const CTMData& d) -> QVariant { return (quint64) d.cls.id(); },
                nullptr,
                nullptr,
                [](const CTMData& a, const CTMData& b) -> bool {
                    return a.cls.id() < b.cls.id();
                }
            },
            { "roots", QObject::tr("Roots"),
                [](const CTMData& d) -> QVariant { 
                    return stringify(d.info.doc.roots());
                }, nullptr, nullptr, nullptr 
            },
            { "key", QObject::tr("Key"),
                [](const CTMData& d) -> QVariant {
                    return d.info.key;
                },
                nullptr,
                nullptr,
                [](const CTMData& a, const CTMData& b) -> bool {
                    return IgCase()(a.info.key, b.info.key);
                }
            }, 
            { "name", QObject::tr("Name"),
                [](const CTMData& d) -> QVariant {
                    return d.info.name;
                }, 
                [](const CTMData& d) -> QVariant {
                    return d.info.name;
                }, 
                [](CTMData& d, const QVariant& v) -> bool {
                    d.info.name = v.toString().trimmed();
                    d.cls.set_name(d.info.name);
                    return true;
                },
                [](const CTMData& a, const CTMData& b) -> bool {
                    return IgCase()(a.info.name, b.info.name);
                }
            },
            { "plural", QObject::tr("Plural"),
                [](const CTMData& d) -> QVariant {
                    return d.info.plural;
                }, 
                [](const CTMData& d) -> QVariant {
                    return d.info.plural;
                }, 
                [](CTMData& d, const QVariant& v) -> bool {
                    d.info.plural = v.toString().trimmed();
                    d.cls.set_plural(d.info.plural);
                    return true;
                },
                [](const CTMData& a, const CTMData& b) -> bool {
                    return IgCase()(a.info.name, b.info.name);
                }
            },
            { "brief", QObject::tr("Brief"),
                [](const CTMData& d) -> QVariant {
                    return d.info.brief;
                }, 
                [](const CTMData& d) -> QVariant {
                    return d.info.brief;
                }, 
                [](CTMData& d, const QVariant& v) -> bool {
                    d.info.brief = v.toString().trimmed();
                    d.cls.set_brief(d.info.brief);
                    return true;
                },
                nullptr
            }
        };
        
        Map<QString,CTMColumn,IgCase>   ret;
        for(auto& c : kColumns)
            ret[c.k]    = c;
        return ret;
    }
    
    const Map<QString,CTMColumn,IgCase>&    columns()
    {
        static const auto ret = makeColumns();
        return ret;
    }

}
#endif


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


ClassTableModel::ClassTableModel(ClassProvider cpp, QObject*parent) : U64TableModel<Class>(cpp?cpp:provider::all_classes(), parent)
{
    customRO("Key", [](Class c) -> auto { return cdb::key(c); });
    customRO("Name", [](Class c) -> QString {
        return cdb::name(c);
    });
    customRO("Brief", [](Class c) -> QString {
        return cdb::brief(c);
    });
}

ClassTableModel::~ClassTableModel()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


ClassTableView::ClassTableView(ClassTableModel*ctm, QWidget*parent) : StdTableView<Class>(ctm, parent), m_model(ctm)
{
    setModel(ctm);
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
    setSortingEnabled(true);
}

ClassTableView::~ClassTableView()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_ClassTableModel.cpp"
#include "moc_ClassTableView.cpp"
