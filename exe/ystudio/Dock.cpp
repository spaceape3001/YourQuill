////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Dock.hpp"

using namespace yq;

YQ_OBJECT_IMPLEMENT(Dock)

namespace {
    struct DockInfoRepo {
        std::vector<const DockInfo*>    all;
    
    };
    
    DockInfoRepo& dockRepo()
    {
        static DockInfoRepo s_repo;
        return s_repo;
    }
}


DockInfo::DockInfo(std::string_view name, const ObjectInfo&parent, const std::source_location& sl) : ObjectInfo(name, parent, sl)
{
    dockRepo().all.push_back(this);
}

////////////////////////////////////////////////////////////////////////////////

Dock::Dock(QWidget*parent) : QDockWidget(parent)
{
}

Dock::~Dock()
{
}

////////////////////////////////////////////////////////////////////////////////

#include "moc_Dock.cpp"
