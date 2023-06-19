////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/root.hpp>
#include "RootTable.hpp"


RootTable::RootTable(QWidget*parent) : IdTableT<Root>(
    []() -> std::vector<Root> {
        return cdb::all_roots();
    }, parent
)
{
    makeColumn<std::string> ("Key", [](Root r) -> std::string {
        const RootDir*  rt  = root_dir(r);
        if(!rt)
            return std::string();
        return rt->key;
    });
    makeColumn<std::string> ("Name", [](Root r) -> std::string {
        const RootDir*  rt  = root_dir(r);
        if(!rt)
            return std::string();
        return rt->name;
    });
    makeColumn<std::string> ("Path", [](Root r) -> std::string {
        const RootDir*  rt  = root_dir(r);
        if(!rt)
            return std::string();
        return rt->path.string();
    });

    setWindowTitle(tr("Roots"));
}

RootTable::~RootTable()
{
}

#include "moc_RootTable.cpp"
