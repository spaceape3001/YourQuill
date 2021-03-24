////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirectoryTableModel.hpp"
#include "DirectoryTableView.hpp"
#include <gui/model/U64TableModelImpl.hpp>
#include <QHeaderView>

template class StdTableModel<Directory>;
template class U64TableModel<Directory>;
template class StdTableView<Directory>;

DirectoryTableModel::DirectoryTableModel(DirectoryProvider ctp, QObject*parent) : 
    U64TableModel<Directory>(ctp?ctp:provider::all_directories(), parent)
{
    customRO("Key", [](Directory d) -> auto { return cdb::skey(d); });
    customRO("Dirs", [](Directory d) -> auto { return (quint64) cdb::directories_count(d); });
    customRO("Frags", [](Directory d) -> auto { return (quint64) cdb::fragments_count(d); });
    customRO("Path", [](Directory f) -> auto { return cdb::path(f); });
}

DirectoryTableModel::~DirectoryTableModel()
{
}


DirectoryTableView::DirectoryTableView(DirectoryTableModel*ctm, QWidget*parent) : StdTableView<Directory>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

DirectoryTableView::~DirectoryTableView()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_DirectoryTableModel.cpp"
#include "moc_DirectoryTableView.cpp"
