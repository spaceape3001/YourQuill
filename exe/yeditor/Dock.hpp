////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"
#include "Action.hpp"
#include <yq/core/Flags.hpp>
#include <QDockWidget>
#include <QIcon>
#include <QKeySequence>

class DreamMW;
class QAction;
class Window;

/*! \brief Singular dock

    This is a singular dock for the main window.  
    It'll use singals/slots for communications to main window.
*/
class Dock : public QDockWidget {
    Q_OBJECT
public:

    using FNCreate  = std::function<Dock*()>;

    struct Info {
        class Writer;
    
        FNCreate            fnCreate;
        ActionInfo          action;
        Qt::DockWidgetArea  startArea       = Qt::LeftDockWidgetArea;
        Qt::DockWidgetAreas allowedAreas    = Qt::AllDockWidgetAreas;
    };

    static Info::Writer  reg(const QString&, FNCreate&&);
    static const std::vector<Info>&  all();


    Dock(QWidget*parent=nullptr);
    ~Dock();
    
signals:
    void        popupRequested(Id, const QActionList& acts = QActionList());
    void        openRequested(Id);
    
protected:
    void        closeEvent(QCloseEvent*) override;
    void        showEvent(QShowEvent*) override;
    void        hideEvent(QHideEvent*) override;

    DreamMW*    dream() const;

private slots:
    void        triggered(bool);
private:
    friend class DreamMW;
    QAction*    m_action    = nullptr;
    
    void    _update();
    
    struct Repo;
    static Repo&    repo();
};

class Dock::Info::Writer {
public:

    Writer(){}
    
    Writer& area(Qt::DockWidgetArea dwa)
    {
        if(m_info)
            m_info -> startArea   = dwa;
        return *this;
    }
    
    
    Writer& autoStart()
    {
        if(m_info)
            m_info -> action.checked  = true;
        return *this;
    }

    Writer& icon(const QIcon& ico)
    {
        if(m_info)
            m_info -> action.icon    = ico;
        return *this;
    }
    
    Writer& shortcut(const QKeySequence& ks)
    {
        if(m_info)
            m_info -> action.shortcut = ks;
        return *this;
    }
    
    Writer& toolTip(const QString& s)
    {
        if(m_info)
            m_info -> action.toolTip = s;
        return *this;
    }

private:
    friend class Dock;
    
    Writer(Dock::Info* c) : m_info(c) {}
    
    Info*    m_info  = nullptr;

};

template <typename D>
concept Dockable = (std::derived_from<D,Dock> && !std::same_as<D,Dock>); 

Dock::Info::Writer   register_dock(const QString&, Dock::FNCreate&&);

template <Dockable D>
Dock::Info::Writer   register_dock(const QString& label)
{
    return register_dock(label, []() -> Dock* { return new D; });
}



