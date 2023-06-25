////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>
#include <basic/Hash.hpp>
#include "ystudio.hpp"

using MainWindow            = yq::gluon::MainWindow;
class Command;

class DreamMW : public MainWindow {
    Q_OBJECT
public:

    DreamMW();
    ~DreamMW();
    
    MainWindow*   newMain() override;
    
public slots:
    void    updateTitle();
    void    newBrowser();
    
private slots:
    void    popupRequested(Id);
    void    openRequested(Id);
    void    commandTriggered(uint64_t);
    
    void    cmd_delete();
    void    cmd_edit();
    void    cmd_properties();
    void    cmd_view();
    
protected:
    virtual QDockWidget*    addDock(Qt::DockWidgetArea, QWidget*) override;
    virtual void            reconnect(QWidget*) override;    

private:
    std::vector<Dock*>      m_docks;
    std::vector<QAction*>   m_cmdActions;
    Id                      m_idForCmd      = Id();
    QAction*                m_actEdit       = nullptr;
    QAction*                m_actView       = nullptr;
    QAction*                m_actDelete     = nullptr;
    QAction*                m_actProperties = nullptr;
    QMenu*                  m_popupMenu     = nullptr;
    const Command*          m_cmdEdit       = nullptr;
    const Command*          m_cmdView       = nullptr;
    const Command*          m_cmdDelete     = nullptr;
    const Command*          m_cmdProperties = nullptr;
};
