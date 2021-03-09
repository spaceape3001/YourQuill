////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FieldTableModel.hpp"
#include "FieldTableView.hpp"
#include <gui/model/U64TableModelImpl.hpp>
#include <QHeaderView>

template class StdTableModel<Field>;
template class U64TableModel<Field>;
template class StdTableView<Field>;

FieldTableModel::FieldTableModel(FieldProvider ctp, QObject*parent) : U64TableModel<Field>(ctp?ctp:provider::all_fields(), parent)
{
    customRO("Key", [](Field f) -> auto { return cdb::key(f); });
    customRO("Field", [](Field f) -> auto { return cdb::name(f); });
    customRO("Class", [](Field f) -> auto { return cdb::name(cdb::parent(f)); });
    customRO("Description", [](Field f) -> auto { return cdb::brief(f); });
}

FieldTableModel::~FieldTableModel()
{
}


FieldTableView::FieldTableView(FieldTableModel*ctm, QWidget*parent) : StdTableView<Field>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

FieldTableView::~FieldTableView()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_FieldTableModel.cpp"
#include "moc_FieldTableView.cpp"
