////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MainWin.hpp"
#include <gui/undo/UndoCallback.hpp>

class QUndoCommand;
class QUndoStack;

class UndoMain : public MainWin, public UndoCallback {
    Q_OBJECT
public:

    void                pushUndo(QUndoCommand*) override;

    bool                isClean() const;
    bool                canRedo() const;
    bool                canUndo() const;
    QString             redoText() const;
    QString             undoText() const;

signals:
    void                cleanChanged(bool);

protected:
    UndoMain(QWidget* editor=nullptr);
    virtual ~UndoMain();
    
    virtual void        connectNotify(const QMetaMethod&);
    void                setClean();
    void                clearUndo();
    
    QWidget*            editor() const { return m_editor; }

private:    
    QWidget*        m_editor;
    QUndoStack*     m_undo;

    enum {
        CC      = 1
    };
    unsigned int    m_connected;
};

