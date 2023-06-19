////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/document.hpp>
#include "DocumentTable.hpp"


DocumentTable::DocumentTable(QWidget*parent) : IdTableT<Document>(
    []() -> std::vector<Document> {
        return cdb::all_documents(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string> ("Key", [](Document d) -> std::string {
        return cdb::key(d);
    });
    makeColumn<unsigned>("Frags", [](Document d) -> unsigned {
        return cdb::fragments_count(d);
    });
    makeColumn<std::string> ("Label", [](Document d) -> std::string {
        return cdb::label(d);
    });

    setWindowTitle(tr("Documents"));
}

DocumentTable::~DocumentTable()
{
}

#include "moc_DocumentTable.cpp"
