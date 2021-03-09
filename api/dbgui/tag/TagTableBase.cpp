////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagTableModel.hpp"
#include "TagTableView.hpp"
#include <gui/model/U64TableModelImpl.hpp>
#include <QHeaderView>

template class StdTableModel<Tag>;  // explicitly instantiate the template here
template class U64TableModel<Tag>;  // explicitly instantiate the template here
template class StdTableView<Tag>;  // explicitly instantiate the template here

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TagTableModel::TagTableModel(TagProvider stp, QObject* parent) : 
    U64TableModel<Tag>(stp ? stp : provider::all_tags(), parent)
{
    customRO("Key", [](Tag t) -> auto {
        return cdb::key(t);
    });
    
    customRW("Name", [](Tag t) -> QString {
        return cdb::name(t);
    }, [](Tag& t, const QString& k) -> bool {
        return cdb::set_name(t, k.simplified());
    });
    
    customRW("Brief", [](Tag t) -> QString {
        return cdb::brief(t);
    }, [](Tag& t, const QString& k) -> bool {
        return cdb::set_brief(t, k.simplified());
    });
}

TagTableModel::~TagTableModel()
{
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


TagTableView::TagTableView(TagTableModel* m, QWidget*parent) : StdTableView<Tag>(m, parent), m_model(m)
{
    setModel(m);
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

TagTableView::~TagTableView()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_TagTableModel.cpp"
#include "moc_TagTableView.cpp"
