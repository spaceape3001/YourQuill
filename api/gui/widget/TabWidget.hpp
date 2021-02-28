////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QWidget>
#include "util/Hash.hpp"

class QAction;
class QBoxLayout;
class QMenu;
class QToolButton;
class QStackedLayout;

class TabWidgetBar;

class TabWidget : public QWidget {
    Q_OBJECT
public:

    TabWidget(QWidget*parent=nullptr);
    ~TabWidget();
    
    TabWidgetBar*   bar() const { return m_tabs; }
    void            addWidget(QWidget*);
    int             count() const;
    QWidget*        currentWidget() const;
    void            delWidget(QWidget*);
    
    
    bool            listEnabled() const;
    TabWidgetBar*   tabs() const { return m_tabs; }
    bool            topSide() const;
    
    void            removeTab(int);
    void            removeTab(QWidget*);
    
    QWidget*        widget(int) const;
    
public slots:
    void            setTopSide(bool);
    void            setListEnable(bool);
    void            closeAll();
    void            setCurrent(QWidget*);
    
signals:
    void            addRequest();
    void            dragStarted(int);
    void            currentChanged();
    
private slots:
    
    void            listWidgets();
    void            tabClicked(int);
    void            tabCloseRequest(int);
    
protected:
    void            connectNotify(const QMetaMethod&);
    void            disconnectNotify(const QMetaMethod&);

private:
    QToolButton*        m_add;
    int                 m_addCount;
    QBoxLayout*         m_layout;
    QToolButton*        m_list;
    QStackedLayout*     m_stack;
    TabWidgetBar*       m_tabs;
    
    void    _incAdd();
    void    _decAdd();
};
