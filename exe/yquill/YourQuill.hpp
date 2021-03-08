////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/MainWin.hpp>


class YourQuill : public MainWin {
    Q_OBJECT
public:
    YourQuill();
    ~YourQuill();
    
    virtual YourQuill*  newMain() { return new YourQuill; }
    
public slots:
    //void            cmdNewCalculator();
    void            cmdNewClassTable();
    void            cmdNewDateGen();
    //void            cmdNewExplorer();
    void            cmdNewLeafTable();
    ////void            cmdNewMonthView();
    void            cmdNewSqlTool();
    void            cmdNewTagTable();
    void            cmdNewTimeGen();
    void            cmdNewWeb();
    
protected:
    virtual void    reconnect(QWidget*) override;

private slots:
    void            subWindowTitleChanged();
    void            updateTitle();
    
    //void            ctxClassTable(const QPoint&);
    //void            ctxCmdAtoms();
    //void            ctxCmdEdit(uint64_t);
    //void            ctxCmdDelete(uint64_t);

private:
    //Class           m_ctxClass;
    //QMenu*          m_ctxClassMenu;
    
};
