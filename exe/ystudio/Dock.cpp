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

struct Dock::Repo {
    std::vector<Creator>    all;
};

Dock::Repo& Dock::repo()
{
    static Repo s_repo;
    return s_repo;
}

const std::vector<Dock::Creator>&  Dock::all()
{
    return repo().all;
}

Dock::Creator::Writer      Dock::reg(const QString& lab, CreateFN&& fn)
{
    if(lab.isEmpty() || !fn)
        return { nullptr };
    
    Repo& _r    = repo();
    Creator     cinfo;
    cinfo.fnCreate  = std::move(fn);
    cinfo.label     = lab;
    _r.all.push_back(std::move(cinfo));
    return { &_r.all.back() };
}

Dock::Creator::Writer   register_dock(const QString&label, Dock::CreateFN&& fn)
{
    return Dock::reg(label, std::move(fn));
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
