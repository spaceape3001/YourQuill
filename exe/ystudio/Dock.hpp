////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Flags.hpp>
#include <QDockWidget>
#include <QIcon>
#include <QKeySequence>


class DreamMW;
class QAction;

/*! \brief Singular dock

    This is a singular dock for the main window.  
    It'll use singals/slots for communications to main window.
*/
class Dock : public QDockWidget {
    Q_OBJECT
public:

    using CreateFN  = std::function<Dock*(QWidget*)>;

    struct Creator {
        class Writer;
    
        CreateFN            fnCreate;
        QString             label;
        QString             toolTip;
        QKeySequence        shortcut;
        QIcon               icon;
        Qt::DockWidgetArea  startArea       = Qt::LeftDockWidgetArea;
        Qt::DockWidgetAreas allowedAreas    = Qt::AllDockWidgetAreas;
        bool                autoStart       = false;
    };

    static Creator::Writer  reg(const QString&, CreateFN&&);
    static const std::vector<Creator>&  all();


    Dock(QWidget*parent=nullptr);
    ~Dock();
    
protected:
    void        closeEvent(QCloseEvent*) override;
    void        showEvent(QShowEvent*) override;
    void        hideEvent(QHideEvent*) override;
    
private slots:
    void        triggered(bool);
private:
    friend class DreamMW;
    QAction*    m_action    = nullptr;
    
    void    _update();
    
    struct Repo;
    static Repo&    repo();
};

class Dock::Creator::Writer {
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
            m_info -> autoStart = true;
        return *this;
    }

    Writer& icon(const QIcon& ico)
    {
        if(m_info)
            m_info -> icon    = ico;
        return *this;
    }
    
    Writer& shortcut(const QKeySequence& ks)
    {
        if(m_info)
            m_info -> shortcut = ks;
        return *this;
    }
    
    Writer& toolTip(const QString& s)
    {
        if(m_info)
            m_info -> toolTip = s;
        return *this;
    }

private:
    friend class Dock;
    
    Writer(Dock::Creator* c) : m_info(c) {}
    
    Creator*    m_info  = nullptr;

};

template <typename D>
concept Dockable = (std::derived_from<D,Dock> && !std::same_as<D,Dock>); 

Dock::Creator::Writer   register_dock(const QString&, Dock::CreateFN&&);

template <Dockable D>
Dock::Creator::Writer   register_dock(const QString& label)
{
    return register_dock(label, [](QWidget*parent) -> Dock* { return new D(parent); });
}



