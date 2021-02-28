////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

class QUndoCommand;


//  Idea is to make a smart undo pointer, so that undos can be passed
//  to an undo stack, or simply executed as-is
class UndoAction {
public:
    UndoAction (QUndoCommand* cmd = nullptr);
    UndoAction (UndoAction&&);
    UndoAction& operator=(UndoAction&&);
        
    ~UndoAction();
    
    QUndoCommand*       release();
    
private:
    QUndoCommand*       m_action;
};


