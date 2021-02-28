////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UndoMain.hpp"
#include <util/FileName.hpp>

class QUndoStack;

class SimpleEditor : public UndoMain
{
    Q_OBJECT
public:

    bool    okayToClose() override;
    
    const FileName&   file() const { return m_file; }

public slots:
    void            cmdNew();
    void            cmdOpen();
    void            cmdSave();
    void            cmdSaveAs();
    virtual void    updateTitle();
    
protected:
    SimpleEditor(QWidget* editor);
    ~SimpleEditor();
    
    virtual QString     ourData() const { return tr("Data"); }
    virtual QString     ourName() const { return tr("SimpleEditor"); }
    virtual QString     ourSaveType() const { return QString(); }
    
    virtual bool        newFile() { return false; }
    virtual bool        openFile(const QString&) { return false; }
    virtual bool        saveFile(const QString&) const { return false; }
    
    //  Override to alter open-file-dialog
    virtual QString     promptOpenFile();
    
    //  Override to alter save-file-dialog
    virtual QString     promptSaveFile();

private:
    FileName  m_file;
};

