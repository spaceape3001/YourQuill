////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "util/Hash.hpp"
#include <QPoint>
#include <QTabBar>


/*! \brief Tab Bar where tabs represent widgets
*/
class TabWidgetBar : public QTabBar {
    Q_OBJECT
    
public:

    TabWidgetBar(QWidget* parent=nullptr);
    virtual ~TabWidgetBar();
    
    using QTabBar::addTab;
    
    int                 addTab(QWidget*,bool autoSwitch=false);
    Vector<QWidget*>    allWidgets() const;
    QWidget*            currentWidget() const;
    bool                dragEnabled() const { return m_dragEnable; }
    const QString&      dragMimeType() const { return m_dragMimeType; }
    bool                dropEnabled() const { return m_dropEnable; }
    
    using QTabBar::removeTab;
    void                removeTab(QWidget*);
    int                 tabIndex(QWidget*) const;
    QWidget*            tabWidget(int) const;
    
public slots:
    void                setDragMimeType(const QString&);
    void                setDragEnabled(bool);
    void                setDropEnabled(bool);
    void                updateTabIcon(QWidget*);
    void                updateTabText(QWidget*);
    void                setTabIndex(QWidget*);


protected:
    virtual void        mousePressEvent(QMouseEvent*) override;
    virtual void        mouseMoveEvent(QMouseEvent*) override;
    virtual void        mouseReleaseEvent(QMouseEvent*) override;
    
    virtual void        dragEnterEvent(QDragEnterEvent*) override;
    virtual void        dragMoveEvent(QDragMoveEvent*) override;
    virtual void        dropEvent(QDropEvent*) override;

    virtual void        tabInserted(int) override;
    virtual void        tabRemoved(int) override;
    
    //void    dragMoveEvent(QDragMoveEvent*) override;

signals:
    void    dragStarted(int tab);
private:
//    friend class TDIWidget;
    
    QString     m_dragMimeType;
    QPoint      m_start;
    int         m_height;
    int         m_tab;
    bool        m_inDrag;
    bool        m_dragEnable;
    bool        m_dropEnable;

    Hash<QWidget*,int>  m_wid2idx;
    void        _recalc();
};
