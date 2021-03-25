////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/attribute/AttributeProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Attribute;
class AttributeTableView;
class AttributeTableModel;

class YAttributeTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YAttributeTable(AttributeProvider stp = AttributeProvider{}, QWidget*parent=nullptr);
    ~YAttributeTable();
    
    AttributeTableModel*  model() const { return m_model; }
    AttributeTableView*   view() const { return m_view; }
    
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
    AttributeTableModel*  m_model;
    AttributeTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

