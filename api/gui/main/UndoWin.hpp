////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SubWin.hpp"
#include "gui/undo/UndoCallback.hpp"

class QUndoCommand;
class QUndoStack;

class UndoWin : public SubWin, public UndoCallback {
    Q_OBJECT
public:

    void                pushUndo(QUndoCommand*) override;

    bool                isClean() const;
    bool                canRedo() const;
    bool                canUndo() const;
    QString             redoText() const;
    QString             undoText() const;

signals:    
    void                canRedoChanged(bool);
    void                canUndoChanged(bool);
    void                redoTextChanged(const QString&);
    void                undoTextChanged(const QString&);
    void                cleanChanged(bool);

public slots:
    void                undo();
    void                redo();
    
protected:
    UndoWin(QWidget*parent=nullptr);
    virtual ~UndoWin();
    
    virtual void    connectNotify(const QMetaMethod&);
    void                setClean();
    void                clearUndo();

private:
    QUndoStack*     m_undo;
    
    enum {
        CC      =  1,
        RC      =  2,
        UC      =  4,
        RT      =  8,
        UT      = 16
    };
    unsigned int    m_connected;
};
