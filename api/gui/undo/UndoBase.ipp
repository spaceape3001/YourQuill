////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

int      UndoBase::makeUndoId()
{
    static std::atomic<int>     sId(0);
    return ++sId;
}

void            UndoBase::setUndoCallback(UndoCallback* newCallback)
{
    m_callback  = newCallback;
    if(m_recursive){
        // TODO
    }
}

void            UndoBase::execute(QUndoCommand*cmd)
{
    if(m_callback){
        m_callback -> pushUndo(cmd);
    } else {
        cmd -> redo();
        delete cmd;
    }
}

