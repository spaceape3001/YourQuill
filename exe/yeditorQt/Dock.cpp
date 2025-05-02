////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Dock.hpp"
#include "DreamMW.hpp"
#include <QCloseEvent>
#include <gluon/core/Utilities.hpp>

using namespace yq;
using namespace yq::gluon;

struct ZDock::Repo {
    std::vector<Info>    all;
};

ZDock::Repo& ZDock::repo()
{
    static Repo s_repo;
    return s_repo;
}

const std::vector<ZDock::Info>&  ZDock::all()
{
    return repo().all;
}

ZDock::Info::Writer      ZDock::reg(const QString& lab, FNCreate&& fn)
{
    if(lab.isEmpty() || !fn)
        return { nullptr };
    
    Repo& _r    = repo();
    Info     cinfo;
    cinfo.fnCreate          = std::move(fn);
    cinfo.action.label      = lab;
    _r.all.push_back(std::move(cinfo));
    return { &_r.all.back() };
}

ZDock::Info::Writer   register_dock(const QString&label, ZDock::FNCreate&& fn)
{
    return ZDock::reg(label, std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////

ZDock::ZDock(QWidget*parent) : QDockWidget(parent)
{
    setFloating(false);
    setFeatures(DockWidgetClosable|DockWidgetMovable);
}

ZDock::~ZDock()
{
}

void    ZDock::_update()
{
    if(m_action)
        m_action -> setChecked(isVisible());
}

void        ZDock::closeEvent(QCloseEvent*evt)
{
    hide();
    _update();
    evt -> ignore();
}

DreamMW*    ZDock::dream() const
{
    return static_cast<DreamMW*>(DreamMW::mainWinFor(const_cast<ZDock*>(this)));
}

void        ZDock::hideEvent(QHideEvent*evt) 
{
    QDockWidget::hideEvent(evt);
    _update();
}

void        ZDock::showEvent(QShowEvent*evt) 
{
    QDockWidget::showEvent(evt);
    _update();
}

void        ZDock::triggered(bool f)
{
    setVisible(f);
    _update();
}

#include "moc_Dock.cpp"
