////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FragmentTableModel.hpp"
#include "FragmentTableView.hpp"
#include <gui/model/U64TableModelImpl.hpp>
#include <QHeaderView>

template class StdTableModel<Fragment>;
template class U64TableModel<Fragment>;
template class StdTableView<Fragment>;

FragmentTableModel::FragmentTableModel(FragmentProvider ctp, QObject*parent) : 
    U64TableModel<Fragment>(ctp?ctp:provider::all_fragments(), parent)
{
    customRO("Name", [](Fragment f) -> auto { return cdb::name(f); });
    customRO("Path", [](Fragment f) -> auto { return cdb::path(f); });
}

FragmentTableModel::~FragmentTableModel()
{
}


FragmentTableView::FragmentTableView(FragmentTableModel*ctm, QWidget*parent) : StdTableView<Fragment>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

FragmentTableView::~FragmentTableView()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_FragmentTableModel.cpp"
#include "moc_FragmentTableView.cpp"
