////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/filesys/DirectoryProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Directory;
class DirectoryTableView;
class DirectoryTableModel;

class YDirectoryTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YDirectoryTable(DirectoryProvider stp = DirectoryProvider{}, QWidget*parent=nullptr);
    ~YDirectoryTable();
    
    DirectoryTableModel*  model() const { return m_model; }
    DirectoryTableView*   view() const { return m_view; }
    
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
    DirectoryTableModel*  m_model;
    DirectoryTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

