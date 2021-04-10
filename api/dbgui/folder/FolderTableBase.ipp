////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

template class StdTableModel<Folder>;
template class U64TableModel<Folder>;
template class StdTableView<Folder>;

FolderTableModel::FolderTableModel(FolderProvider ctp, QObject*parent) : 
    U64TableModel<Folder>(ctp?ctp:provider::all_folders(), parent)
{
    customRO("Name", [](Folder f) -> auto { return cdb::name(f); });
    customRO("Dirs", [](Folder f) -> auto { return (quint64) cdb::directories_count(f); });
    customRO("Docs", [](Folder f) -> auto { return (quint64) cdb::documents_count(f); });
    customRO("Frags", [](Folder f) -> auto { return (quint64) cdb::fragments_count(f); });
    customRO("Key", [](Folder f) -> auto { return cdb::key(f); });
}

FolderTableModel::~FolderTableModel()
{
}


FolderTableView::FolderTableView(FolderTableModel*ctm, QWidget*parent) : StdTableView<Folder>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

FolderTableView::~FolderTableView()
{
}

