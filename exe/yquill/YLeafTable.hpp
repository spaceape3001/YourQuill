////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/leaf/LeafProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Leaf;
class LeafTableView;
class LeafTableModel;

class YLeafTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YLeafTable(LeafProvider stp = LeafProvider{}, QWidget*parent=nullptr);
    ~YLeafTable();
    
    LeafTableModel*  model() const { return m_model; }
    LeafTableView*   view() const { return m_view; }
    
    bool            addEnabled() const { return m_addEnable; }
    void            setAddEnable(bool);
    
    bool            readOnly() const;
    void            setReadOnly(bool);
    
signals:
    void            editReq(uint64_t);
    
public slots:
    void            cmdAdd();
    virtual void    refresh() override;
    virtual void    check() override;
    
private slots:
    void            doubleClicked(const QModelIndex&);
    
private:
    LeafTableModel*  m_model;
    LeafTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

