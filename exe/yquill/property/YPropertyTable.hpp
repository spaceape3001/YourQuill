////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/property/PropertyProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Property;
class PropertyTableView;
class PropertyTableModel;

class YPropertyTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YPropertyTable(PropertyProvider stp = PropertyProvider{}, QWidget*parent=nullptr);
    ~YPropertyTable();
    
    PropertyTableModel*  model() const { return m_model; }
    PropertyTableView*   view() const { return m_view; }
    
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
    PropertyTableModel*  m_model;
    PropertyTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

