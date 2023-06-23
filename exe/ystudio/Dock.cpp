////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Dock.hpp"
#include <QCloseEvent>
#include <gluon/core/Utilities.hpp>

using namespace yq;
using namespace yq::gluon;

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

const std::vector<const DockInfo*>&  DockInfo::all()
{
    return dockRepo().all;
}


DockInfo::DockInfo(std::string_view name, const ObjectInfo&parent, const std::source_location& sl) : ObjectInfo(name, parent, sl)
{
    dockRepo().all.push_back(this);
    m_label = qString(name);
}

////////////////////////////////////////////////////////////////////////////////

Dock::Dock(QWidget*parent) : QDockWidget(parent)
{
    setFloating(false);
    setFeatures(DockWidgetClosable|DockWidgetMovable);
}

Dock::~Dock()
{
}

void    Dock::_update()
{
    if(m_action)
        m_action -> setChecked(isVisible());
}

void        Dock::closeEvent(QCloseEvent*evt)
{
    hide();
    _update();
    evt -> ignore();
}

void        Dock::hideEvent(QHideEvent*evt) 
{
    QDockWidget::hideEvent(evt);
    _update();
}

void        Dock::showEvent(QShowEvent*evt) 
{
    QDockWidget::showEvent(evt);
    _update();
}

void        Dock::triggered(bool f)
{
    setVisible(f);
    _update();
}

////////////////////////////////////////////////////////////////////////////////

#include "moc_Dock.cpp"
