////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <dbgui/tag/TagProvider.hpp>

class QLineEdit;
class QPushButton;
class QToolBar;
class QTimer;
class Tag;
class TagTableView;
class TagTableModel;

class YTagTable : public SubWin {
    Q_OBJECT
public:
    //class AddBar;
    
    YTagTable(TagProvider stp = TagProvider{}, QWidget*parent=nullptr);
    ~YTagTable();
    
    TagTableModel*  model() const { return m_model; }
    TagTableView*   view() const { return m_view; }
    
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
    TagTableModel*  m_model;
    TagTableView*   m_view;
    QLineEdit*      m_addKey;
    QPushButton*    m_addBtn;
    bool            m_addEnable;
};

