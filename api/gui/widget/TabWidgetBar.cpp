////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TabWidgetBar.hpp"

#include "util/Vector.hpp"

#include <QDragEnterEvent>
#include <QIcon>
#include <QMimeData>
#include <QVariant>

Q_DECLARE_METATYPE(QWidget*)


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TabWidgetBar::TabWidgetBar(QWidget* parent) : QTabBar(parent), 
    m_height(30),
    m_tab(-1),
    m_dragEnable(false),
    m_dropEnable(false)
{
    connect(this, &QTabBar::tabMoved, this, &TabWidgetBar::_recalc);
}

TabWidgetBar::~TabWidgetBar()
{
}

void        TabWidgetBar::_recalc()
{
    m_wid2idx.clear();
    for(int i=0;i<count();++i){
        QWidget*w   = tabWidget(i);
        if(w)
            m_wid2idx[w]    = i;
    }
}

int         TabWidgetBar::addTab(QWidget* w, bool autoSwitch)
{
    int i   = QTabBar::addTab(w->windowIcon(), w->windowTitle());
    setTabData(i, QVariant::fromValue(w));
    m_wid2idx[w]    = i;
    if(autoSwitch){
        bool    f = blockSignals(true);
        setCurrentIndex(i);
        blockSignals(f);
    }
    return i;
}

Vector<QWidget*>    TabWidgetBar::allWidgets() const
{
    Vector<QWidget*>    ret;
    for(int i=0;i<count();++i){
        QWidget*    w   = tabWidget(i);
        if(w)
            ret << w;
    }
    return ret;
}

QWidget*    TabWidgetBar::currentWidget() const
{
    return tabWidget(currentIndex());
}

void    TabWidgetBar::dragEnterEvent(QDragEnterEvent* evt)
{
        //  TODO: Add FILE Support....
    if(m_dropEnable){
        const QMimeData*    data    = evt -> mimeData();
        if(data && data->hasFormat(m_dragMimeType)){
            evt -> acceptProposedAction();
        } else
            evt -> ignore();
    } else
        evt -> ignore();
}

void    TabWidgetBar::dragMoveEvent(QDragMoveEvent*evt) 
{
    if(m_dropEnable){
        const QMimeData*    data    = evt -> mimeData();
        if(data && data->hasFormat(m_dragMimeType)){
            evt -> acceptProposedAction();
        } else
            evt -> ignore();
    } else
        evt -> ignore();
}


#if 0
void    TabWidgetBar::dragMoveEvent(QDragMoveEvent* evt) 
{
    qDebug() << "DragMoveEvent";
    
    if(m_tab >= 0){
        QPoint      gp  = QCursor::pos(); // evt -> globalPos();
        if(abs(gp.y() - m_start.y()) >= m_height)
            QMessageBox::information(nullptr, tr("Drag Detected"), 
                tr("A drag action was detected!"));
    }
    QTabBar::dragMoveEvent(evt);
}
#endif


void    TabWidgetBar::dropEvent(QDropEvent*evt)
{
    if(m_dropEnable){
        const QMimeData*    data    = evt -> mimeData();
        if(data && data->hasFormat(m_dragMimeType)){
            evt -> acceptProposedAction();
        } else
            evt -> ignore();
    } else
        evt -> ignore();
}

void    TabWidgetBar::mousePressEvent(QMouseEvent*evt) 
{
    m_inDrag    = false;
    if(m_dragEnable){
        if(evt->button() == Qt::LeftButton){
            QPoint      p   = evt->pos();
            QPoint      gp  = evt->globalPos();
            m_start         = gp;
            m_tab           = tabAt(p);
            if(m_tab >= 0)
                m_height     = tabRect(m_tab).height();
            //qDebug() << "Mouse Press, " << p.x() << p.y() << " [global " << gp.x() << gp.y() << "] tab " << m_tab;
        }
    }
    QTabBar::mousePressEvent(evt);
}

void    TabWidgetBar::mouseMoveEvent(QMouseEvent* evt) 
{
    if(m_dragEnable){
        if(m_inDrag){
            evt -> ignore();
            return ;
        }
        
        QSize       sz  = size();
        if(m_tab >= 0){
            //QPoint      p   = evt->pos();
            QPoint      gp  = evt->globalPos();
            if(abs(m_start.y() - gp.y()) >= sz.height()){
                int     tab = m_tab;
                m_tab       = -1;
                emit dragStarted(tab);
                m_inDrag    = true;
            }
        }
    }
    QTabBar::mouseMoveEvent(evt);
}

void    TabWidgetBar::mouseReleaseEvent(QMouseEvent*evt) 
{
    if(m_dragEnable){
        if(m_inDrag)
            evt -> ignore();
        else
            QTabBar::mouseReleaseEvent(evt);

        m_tab        = -1;
        m_inDrag    = false;
    } else
        QTabBar::mouseMoveEvent(evt);
}

void        TabWidgetBar::removeTab(QWidget* w)
{
    int i   = m_wid2idx.get(w, -1);
    if(i>=0){
        QTabBar::removeTab(i);
        _recalc();
    }
}

void        TabWidgetBar::setDragMimeType(const QString& t)
{
    m_dragMimeType  = t;
}

void        TabWidgetBar::setDragEnabled(bool f)
{
    m_dragEnable   = f;
}

void        TabWidgetBar::setDropEnabled(bool f)
{
    m_dropEnable   = f;
    setAcceptDrops(f);
}

void        TabWidgetBar::setTabIndex(QWidget* w)
{
    int i   = tabIndex(w);
    if(i>= 0)
        setCurrentIndex(i);
}


int         TabWidgetBar::tabIndex(QWidget*w) const
{
    return m_wid2idx.get(w,-1);
}

void        TabWidgetBar::tabInserted(int) 
{
    _recalc();
}

void        TabWidgetBar::tabRemoved(int) 
{
    _recalc();
}
    
QWidget*    TabWidgetBar::tabWidget(int i) const
{
    static const int    utWidget    = qMetaTypeId<QWidget*>();
    if(i<0)
        return nullptr;
    QVariant    var = tabData(i);
    if(var.userType() != utWidget)
        return nullptr;
    return var.value<QWidget*>();
}


void        TabWidgetBar::updateTabIcon(QWidget*w)
{
    int i   = tabIndex(w);
    if(i>=0){
        setTabIcon(i, w->windowIcon());
    }
}
    

void        TabWidgetBar::updateTabText(QWidget*w)
{
    int i   = tabIndex(w);
    if(i>=0){
        setTabText(i, w->windowTitle());
    }
}

#include "moc_TabWidgetBar.cpp"
