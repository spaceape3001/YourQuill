////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>

#include <db/Class.hpp>
#include <dbgui/class/ClassProvider.hpp>


class ClassTableModel;
class ClassTableView;
class RootComboBox;
class LineEdit;
class QPushButton;

class YClassTable : public SubWin {
    Q_OBJECT
public:

    YClassTable(ClassProvider cpp = ClassProvider(), QWidget* parent=nullptr);
    ~YClassTable();
    
    ClassTableModel*    model() const { return m_model; }
    ClassTableView*     view() const { return m_view; }
    
    Class               classAt(const QPoint&) const;
    
    bool                addEnabled() const { return m_addEnable; }
    void                setAddEnable(bool);
    bool                readOnly() const;
    void                setReadOnly(bool);
signals:
    void                editReq(uint64_t);
    void                contextMenu(const QPoint&);

public slots:
    void                cmdAdd();
    virtual void        refresh() override;
    virtual void        check() override;
    
private slots:
    void                doubleClicked(const QModelIndex&);
    
private:
    ClassTableModel*    m_model;
    ClassTableView*     m_view;
    RootComboBox*       m_addRoot;
    LineEdit*           m_addKey;
    QPushButton*        m_addBtn;
    bool                m_addEnable;
    //int                 m_colID, m_colKey;
    Class               m_ctxClass;
};
