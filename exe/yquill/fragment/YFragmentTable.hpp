////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/fragment/FragmentProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Fragment;
class FragmentTableView;
class FragmentTableModel;

class YFragmentTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YFragmentTable(FragmentProvider stp = FragmentProvider{}, QWidget*parent=nullptr);
    ~YFragmentTable();
    
    FragmentTableModel*  model() const { return m_model; }
    FragmentTableView*   view() const { return m_view; }
    
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
    FragmentTableModel*  m_model;
    FragmentTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

