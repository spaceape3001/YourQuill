////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>
#include <yq/container/Hash.hpp>
#include <yq/container/Map.hpp>
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
    void    popupRequested(Id, const QActionList& acts = QActionList());
    void    openRequested(Id);
    void    commandTriggered(uint64_t);
    
    void    popupCommand(uint64_t);
    
protected:
    virtual QDockWidget*    addDock(Qt::DockWidgetArea, QWidget*) override;
    virtual void            reconnect(QWidget*) override;    

private:
    std::vector<Dock*>      m_docks;
    std::vector<QAction*>   m_cmdActions;
    Id                      m_idForCmd      = Id();

    struct PopupItem {
        const Command*      cmd         = nullptr;
        QAction*            act         = nullptr;
        bool                last        = false;
        bool                separator   = false;
        void                set(Id);
    };
    
    std::vector<PopupItem>  m_popupItems;
};
