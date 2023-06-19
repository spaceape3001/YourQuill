////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/directory.hpp>
#include "DirectoryTable.hpp"


DirectoryTable::DirectoryTable(QWidget*parent) : IdTableT<Directory>(
    []() -> std::vector<Directory> {
        return cdb::all_directories(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string> ("Key", [](Directory d) -> std::string {
        return cdb::key(d);
    });
    makeColumn<bool> ("Hidden", [](Directory d) -> bool {
        return cdb::hidden(d);
    });
    makeColumn<std::string> ("Path", [](Directory d) -> std::string {
        return cdb::path(d).string();
    });

    setWindowTitle(tr("Directories"));
}

DirectoryTable::~DirectoryTable()
{
}

#include "moc_DirectoryTable.cpp"
