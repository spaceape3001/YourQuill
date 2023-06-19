////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/folder.hpp>
#include "FolderTable.hpp"


FolderTable::FolderTable(QWidget*parent) : IdTableT<Folder>(
    []() -> std::vector<Folder> {
        return cdb::all_folders(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string> ("Key", [](Folder f) -> std::string {
        return cdb::key(f);
    });
    makeColumn<unsigned> ("Docs", [](Folder f) -> unsigned {
        return cdb::child_documents_count(f);
    });
    makeColumn<unsigned> ("Subs", [](Folder f) -> unsigned {
        return cdb::child_folders_count(f);
    });
    makeColumn<std::string> ("Label", [](Folder f) -> std::string {
        return cdb::label(f);
    });

    setWindowTitle(tr("Folders"));
}

FolderTable::~FolderTable()
{
}

#include "moc_FolderTable.cpp"
