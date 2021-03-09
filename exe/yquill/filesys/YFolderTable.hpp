////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/filesys/FolderProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Folder;
class FolderTableView;
class FolderTableModel;

class YFolderTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YFolderTable(FolderProvider stp = FolderProvider{}, QWidget*parent=nullptr);
    ~YFolderTable();
    
    FolderTableModel*  model() const { return m_model; }
    FolderTableView*   view() const { return m_view; }
    
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
    FolderTableModel*  m_model;
    FolderTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

