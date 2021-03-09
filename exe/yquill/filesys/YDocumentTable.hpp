////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/filesys/DocumentProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Document;
class DocumentTableView;
class DocumentTableModel;

class YDocumentTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YDocumentTable(DocumentProvider stp = DocumentProvider{}, QWidget*parent=nullptr);
    ~YDocumentTable();
    
    DocumentTableModel*  model() const { return m_model; }
    DocumentTableView*   view() const { return m_view; }
    
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
    DocumentTableModel*  m_model;
    DocumentTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

