////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/atom/AtomProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Atom;
class AtomTableView;
class AtomTableModel;

class YAtomTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YAtomTable(AtomProvider stp = AtomProvider{}, QWidget*parent=nullptr);
    ~YAtomTable();
    
    AtomTableModel*  model() const { return m_model; }
    AtomTableView*   view() const { return m_view; }
    
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
    AtomTableModel*  m_model;
    AtomTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

