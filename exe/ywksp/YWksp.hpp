////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SimpleEditor.hpp>


class QUndoStack;
class QuillEdit;

class YWksp : public SimpleEditor {
    Q_OBJECT
public:

    using MainWin::addAction;
    YWksp();
    ~YWksp();
    
public slots:
    void        cmdAddChain();
    void        cmdAddTemplate();
    
protected:
    QuillEdit*       m_editor;

    virtual QString     ourData() const override { return tr("Workspace"); }
    virtual QString     ourName() const override { return tr("WorkspaceEditor"); }
    virtual QString     ourSaveType() const override { return tr("YourQuill Workspace (*.yquill)"); }
    
    virtual bool        newFile() override;
    virtual bool        openFile(const QString&) override;
    virtual bool        saveFile(const QString&) const override;

    //  Override to alter open-file-dialog
    virtual QString     promptOpenFile() override;
    
    //  Override to alter save-file-dialog
    virtual QString     promptSaveFile() override;
};
