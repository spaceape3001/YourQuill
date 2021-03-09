////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/atomsys/FieldProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Field;
class FieldTableView;
class FieldTableModel;

class YFieldTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YFieldTable(FieldProvider stp = FieldProvider{}, QWidget*parent=nullptr);
    ~YFieldTable();
    
    FieldTableModel*  model() const { return m_model; }
    FieldTableView*   view() const { return m_view; }
    
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
    FieldTableModel*  m_model;
    FieldTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

