////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "MainWin.hpp"
#include "SubWin.hpp"

#include <gui/widget/TabWidget.hpp>
#include <gui/widget/TabWidgetBar.hpp>

#include <util/Utilities.hpp>

#include <QCloseEvent>
#include <QDockWidget>
#include <QDrag>
#include <QMenu>
#include <QMenuBar>
#include <QMetaMethod>
#include <QMimeData>
#include <QStatusBar>

namespace {
    static const char* szMime_TabDrag = "application/x-window-drag";

    template <typename T>
    T*                   ownerFor(QObject* obj)
    {
        for(QObject* q = obj; q; q = q -> parent()){
            T* w   = qobject_cast<T*>(q);
            if(w)
                return w;
        }
        return nullptr;
    }
}

struct MainWin::WinInfo {
    QWidget*    const           widget;
    SubWin*     const           subwin;
    Map<QString,QMetaMethod>    act2slot;
    Set<QString>                actions;
    TabWidget*                  tabs;
    QDockWidget*                dock;
    
    explicit WinInfo(QWidget* w) : widget(w), subwin(qobject_cast<SubWin*>(w)), tabs(nullptr), dock(nullptr)
    {
        const QMetaObject* mo = w -> metaObject();
        for(int i=0;i<mo->methodCount();++i){
            QMetaMethod mm  = mo -> method(i);
            if(mm.parameterCount() > 0)
                continue;
            if((mm.methodType() == QMetaMethod::Slot) || (mm.methodType() == QMetaMethod::Signal)){
                QString s   = QString::fromLatin1(mm.name());
                act2slot[s] = mm;
                actions << s;
            }
        }
        
        QObject::connect(w, &QWidget::windowIconChanged, [this](const QIcon& ico){
            if(tabs && tabs->bar())
                tabs->bar()->updateTabIcon(widget);
            if(dock)
                dock -> setWindowIcon(ico);
        });
     
        QObject::connect(w, &QWidget::windowTitleChanged, [this](const QString& text){
            if(tabs && tabs->bar())
                tabs->bar()->updateTabText(widget);
            if(dock)
                dock -> setWindowTitle(text);
        });
    }
};
    
struct MainWin::Repo {
    Hash<QObject*,WinInfo*>     winInfo;
    
    WinInfo*    info(QObject* obj)
    {
        return winInfo.get(obj, nullptr);
    }
    
    WinInfo*    info(QWidget*obj, bool fCreate=false)
    {
        WinInfo*    ret = winInfo.get(obj, nullptr);
        if(ret || !fCreate)
            return ret;
        
        ret = winInfo[obj] = new WinInfo(obj);
        
            //  take care of the "destroyed" signal here... btw, signal is 
            //  issued right before the delete, so obj is still valid
        QObject::connect(obj, &QObject::destroyed, [this, ret ](QObject*obj){
            if(ret -> tabs)
                ret -> tabs -> removeTab(ret->widget);
            winInfo.erase(obj);
            delete ret;
        });
        return ret;
    }
};

MainWin::Repo& MainWin::repo()
{
    static Repo s_repo;
    return s_repo;
}

MainWin* MainWin::mainWinFor(QWidget* w)
{
    return ownerFor<MainWin>(w);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MainWin::MainWin() : 
    m_autoEnableCmds(false), m_detachableTabs(false), m_tdi(nullptr)
{
    connect(this, &MainWin::threadSync_status, this, &MainWin::status, Qt::QueuedConnection);
}

MainWin::~MainWin()
{
}

void            MainWin::activeChanged()
{
        //  temporary until the GUI gets more complex....
    WinInfo*    wi  = activeWinInfo();
    if(wi){
        if(m_autoEnableCmds)
            enableDefaultedCommands( wi -> actions);
        if(wi -> subwin && !wi -> subwin -> currentStatus().isEmpty())
            status(wi->subwin->currentStatus());
        activeWindowChanged();
    }
}


//bool            MainWin::activateMdi()
//{
    //if(m_tdi)
        //return false;
    //if(m_mdi)   // already there
        //return true;
        
    //m_mdi       = new QMdiArea;
    //connect(m_mdi, &QMdiArea::subWindowActivated, this, &MainWin::subActivated);
    //setCentralWidget(m_mdi);
    //return true;
//}

//void            MainWin::activateTdi()
//{
    ////if(m_mdi)
        ////return false;
    ////if(m_tdi)
        ////return true;
    
    //m_tdi   = new Area;
    //connect(m_tdi, &MainWin::Area::dragStarted, this, &MainWin::dragStarted);
    //setCentralWidget(m_tdi);
    ////return true;
//}

//QWidget*        MainWin::activeWindow()
//{
    //WinInfo*    wi  = activeWinInfo();
    //if(wi)
        //return wi -> widget;
    //return nullptr;
//}

//MainWin::WinInfo*  MainWin::activeWinInfo()
//{
    //QWidget*    sub = nullptr;
    ////if(m_mdi)
        ////sub = m_mdi -> activeSubWindow();
    ////else 
    //if(m_tdi)
        //sub = m_tdi -> activeSubWindow();

    //if(!sub)
        //return nullptr;
    //return m_winInfo.get(sub, nullptr);
//}

void     MainWin::activateTabs()
{
    if(m_tdi)   // already activated
        return ;
    m_tdi       = new TabWidget(this);
    setCentralWidget(m_tdi);
    m_tdi -> bar() -> setUsesScrollButtons(true);
    connect(m_tdi, &TabWidget::currentChanged, this, &MainWin::tabChanged);
}

QWidget*        MainWin::activeWindow() const
{
    if(m_tdi)
        return m_tdi -> currentWidget();
    return nullptr;
}


MainWin::WinInfo*   MainWin::activeWinInfo() const
{
    QWidget*w   = activeWindow();
    if(w)
        return repo().info(w);
    return nullptr;
}


MainWin::Action          MainWin::addAction(const QString& name,const QString& text)
{
    QAction*act = m_actions.get(name, nullptr);
    if(act)
        return Action(nullptr, act, QString());
    act = new QAction(text, this);
    m_actions[name]  = act;
    m_actionKeys << name;
    m_act2str[act]  = name;
    return Action(this, act, name);
}

QDockWidget*    MainWin::addDock(Qt::DockWidgetArea a, QWidget*w)
{
    QDockWidget*    dw  = qobject_cast<QDockWidget*>(w);
    if(dw){
        addDockWidget(a, dw);
    } else {
        dw      = new QDockWidget;
        dw->setWidget(w);
        dw->setWindowTitle(w->windowTitle());
        dw->setWindowIcon(w->windowIcon());
        connect(w, &QWidget::windowTitleChanged, dw, &QWidget::setWindowTitle);
        connect(w, &QWidget::windowIconChanged, dw, &QWidget::setWindowIcon);
        addDockWidget(a, dw);
    }
    return dw;
}

void            MainWin::addToMenu(QMenu* men, const QStringList& cmds)
{
    for(const QString& c : cmds){
        if(c == "--"){
            men -> addSeparator();
            continue;
        }
        QMenu*      m2 = m_menus.get(c, nullptr);
        if(m2){
            men->addMenu(m2);
            continue;
        }
        QAction*    act = m_actions.get(c, nullptr);
        if(act){
            men->addAction(act);
            continue;
        }
    }
}

void            MainWin::addWindow(QWidget* w)
{
    if(m_tdi){
        WinInfo*    wi  = repo().info(w, true);
        wi -> tabs  = m_tdi;
        wi -> dock  = nullptr;
        m_tdi -> addWidget(w);
        reconnect(w);
    }
}


void   MainWin::closeEvent(QCloseEvent*evt)
{
    if(okayToClose())
        evt -> accept();
    else
        evt -> ignore();
}


void    MainWin::doCommand(const QString&k)
{
    WinInfo*    wi  = activeWinInfo();
    if(wi){
        QMetaMethod mm = wi->act2slot.get(k);
        if(mm.isValid())
            mm.invoke(wi->widget);
    }
}


void            MainWin::enableAutoEnableCmds()
{
    m_autoEnableCmds    = true;
}

void            MainWin::enableClosableTabs()
{
    if(m_tdi && m_tdi -> bar())
        m_tdi -> bar() -> setTabsClosable(true);
}

void            MainWin::enableDefaultedCommands(const Set<QString>& enableThese)
{
    for(auto& i : m_actions){
        if(m_defConnected.has(i.first))
            i.second -> setEnabled(enableThese.has(i.first));
    }
}

void            MainWin::enableDetachableTabs()
{
    if(m_tdi && m_tdi -> bar()){
        TabWidgetBar* tb  = m_tdi -> bar();
        tb -> setMovable(true);
        tb -> setDragEnabled(true);
        tb -> setDropEnabled(true);
        tb -> setDragMimeType(szMime_TabDrag);

        QObject::connect(m_tdi, &TabWidget::dragStarted, this, &MainWin::tabDragStarted);
    }
}

void            MainWin::enableMoveableTabs()
{
    if(m_tdi && m_tdi -> bar())
        m_tdi -> bar() -> setMovable(true);
}

QMenu*          MainWin::makeMenu(const QString&name, const QString&label, const QStringList&cmds, bool fAddToMenuBar)
{
    QMenu* men  = m_menus.get(name, nullptr);
    if(men)
        return men;
    men     = new QMenu(label);
    m_menus[name]   = men;
    addToMenu(men, cmds);
    if(fAddToMenuBar)
        menuBar() -> addMenu(men);

    return men;
}

QMenu*          MainWin::makeMenu(const QString&name, const QString&label, bool fAddToMenuBar)
{
    return makeMenu(name, label, QStringList(), fAddToMenuBar);
}

void            MainWin::mapAction()
{
    QString     s   = m_act2str.get(sender());
    if(!s.isEmpty())
        doCommand(s);
}

void            MainWin::mapStatus(const QString&msg)
{
    WinInfo*    wi  = repo().info(sender());
    if(!wi)
        return ;
    if(wi != activeWinInfo())
        return ;
    status(msg);
}

void            MainWin::reconnect(QWidget*w)
{
    WinInfo*    wi  = repo().info(w,true);
    if(wi -> subwin)
        connect(wi -> subwin, &SubWin::statusMessage, this, &MainWin::mapStatus);
}


void            MainWin::status(const QString& msg, unsigned int timeout)
{
    if(isMainThread()){
        statusBar() -> showMessage(msg, (int) timeout);
    } else
        emit threadSync_status(msg, timeout);
}

void            MainWin::tabChanged()
{
    activeChanged();
}

void        MainWin::tabDragStarted(int tab)
{
    QWidget*w   = tabWidget(tab);
    if(!w)
        return ;
        
    QRect       rect    = QRect(QPoint(0,0), w -> size());
    QPixmap pixmap(rect.size());
    w -> render(&pixmap, QPoint(), QRegion(rect));
    
    //Mime*       mimeData    = new Mime(w);
    QMimeData*  mimeData    = new QMimeData;
    mimeData -> setData(szMime_TabDrag, QByteArray());
    QDrag*      drag        = new QDrag(this);
    drag -> setMimeData(mimeData);
    drag -> setPixmap(pixmap);
    drag -> exec();
    
        /*
            This may or may not be the best implementation, however, I just
            need a basic one to work (doesn't have to be perfect...)
        */
    
    MainWin*        tgt  = ownerFor<MainWin>(drag -> target());
    MainWin*        mw   = this;
    if(!tgt){
        QPoint  gp  = QCursor::pos();
        
        // relocate the tab....
        mw  = newMain();
        m_tdi -> removeTab(tab);
        mw -> resize(size());
        mw -> move(gp);
        
        disconnect(w);
        w -> disconnect(this);
        mw -> addWindow(w);
        mw -> show();
    } else if(tgt != this){
        m_tdi-> removeTab(tab);
        tgt  -> addWindow(w);
        disconnect(w);
        w -> disconnect(this);
    }
    
    if(m_tdi -> count() <= 0)
        close();
}

QWidget*        MainWin::tabWidget(int tab) const
{
    if(!m_tdi)
        return nullptr;
    return m_tdi -> widget(tab);
}

MainWin::WinInfo*   MainWin::tabWinInfo(int tab) const
{
    if(!m_tdi)
        return nullptr;
    return repo().info(m_tdi -> currentWidget());
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


MainWin::Action::Action(Action&& mv) : 
    m_win(mv.m_win), m_action(mv.m_action), m_command(mv.m_command), m_connected(mv.m_connected)
{
    mv.m_win    = nullptr;
    mv.m_action = nullptr;
}

MainWin::Action& MainWin::Action::operator=(Action&&mv )
{
    if(&mv != this){
        m_win   = mv.m_win;
        m_action    = mv.m_action;
        m_command   = mv.m_command;
        m_connected = mv.m_connected;
        mv.m_win    = nullptr;
        mv.m_action = nullptr;
    }
    return *this;
}

MainWin::Action::~Action()
{
    if(m_win && m_action && !m_connected){
        QObject::connect(m_action, &QAction::triggered, m_win, &MainWin::mapAction );
        m_win -> m_defConnected << m_command;
    }
}


MainWin::Action::Action(MainWin*mw, QAction*act, const QString&cmd) : 
    m_win(mw), m_action(act), m_command(cmd), m_connected(false)
{
}

MainWin::Action&     MainWin::Action::icon(const QString& ico)
{
    if(!ico.isEmpty())
        icon(fetch_icon(ico));
    return *this;
}

MainWin::Action&     MainWin::Action::icon(const QIcon& ico)
{
    if(!ico.isNull())
        m_action -> setIcon(ico);
    return *this;
}


MainWin::Action&     MainWin::Action::shortcut(const QString& scut)
{
    if(!scut.isEmpty())
        m_action -> setShortcut(scut);
    return *this;
}

MainWin::Action&     MainWin::Action::shortcut(const QKeySequence& kseq)
{
    m_action -> setShortcut(kseq);
    return *this;
}


#include "moc_MainWin.cpp"
