////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>
#include <doodlerQt/QProject.hpp>

class DoodleMW : public yq::gluon::MainWindow {
    Q_OBJECT
public:
    DoodleMW(yq::doodler::QProjectPtr qpp={});
    ~DoodleMW();
    
    virtual DoodleMW* newMain();
    
private slots:
    void        cmdClear();
    void        cmdCopy();
    void        cmdCut();
    void        cmdFind();
    void        cmdOpen();
    void        cmdPaste();
    void        cmdPrint();
    void        cmdRedo();
    void        cmdSave();
    void        cmdSaveAs();
    void        cmdSetTitle();
    void        cmdUndo();
    void        updateTitle();
    
private:
    yq::doodler::QProjectPtr      m_project;
};
