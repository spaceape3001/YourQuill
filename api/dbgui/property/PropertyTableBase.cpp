////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "PropertyTableModel.hpp"
#include "PropertyTableView.hpp"
#include <gui/model/U64TableModelImpl.hpp>
#include <QHeaderView>

template class StdTableModel<Property>;
template class U64TableModel<Property>;
template class StdTableView<Property>;

PropertyTableModel::PropertyTableModel(PropertyProvider ctp, QObject*parent) : 
    U64TableModel<Property>(ctp?ctp:provider::all_properties(), parent)
{
    customRO("Key", [](Property f) -> auto { return cdb::key(f); });
    customRO("Value", [](Property f) -> auto { return cdb::value(f); });
    customRO("UID", [](Property f) -> auto { return cdb::uid(f); });
}

PropertyTableModel::~PropertyTableModel()
{
}


PropertyTableView::PropertyTableView(PropertyTableModel*ctm, QWidget*parent) : StdTableView<Property>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

PropertyTableView::~PropertyTableView()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_PropertyTableModel.cpp"
#include "moc_PropertyTableView.cpp"
