////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

UndoWin::UndoWin(QWidget*parent) : SubWin(parent), m_undo(nullptr), m_connected(0)
{
    m_undo  = new QUndoStack(this);
    m_undo -> setClean();
}

UndoWin::~UndoWin()
{
}

bool                UndoWin::canRedo() const
{
    return m_undo -> canRedo();
}

bool                UndoWin::canUndo() const
{
    return m_undo -> canUndo();
}

void    UndoWin::clearUndo()
{
    m_undo -> clear();
}

void    UndoWin::connectNotify(const QMetaMethod&mm)
{
    SubWin::connectNotify(mm);
        
        //  Given that we don't actually need to connect the signals to the 
        //  unto stack until the client explicitly connected to our signals, 
        //  we defer this until here... just one way, don't bother with
        //  the disconnect.
    
    if(mm == QMetaMethod::fromSignal(&UndoWin::cleanChanged) && !(m_connected & CC)){
        connect(m_undo, &QUndoStack::cleanChanged, this, &UndoWin::cleanChanged);
        m_connected |= CC;
    }
    if(mm == QMetaMethod::fromSignal(&UndoWin::canRedoChanged) && !(m_connected & RC)){
        connect(m_undo, &QUndoStack::canRedoChanged, this, &UndoWin::canRedoChanged);
        m_connected |= RC;
    }
    if(mm == QMetaMethod::fromSignal(&UndoWin::canUndoChanged) && !(m_connected & UC)){
        connect(m_undo, &QUndoStack::canUndoChanged, this, &UndoWin::canUndoChanged);
        m_connected |= UC;
    }
    if(mm == QMetaMethod::fromSignal(&UndoWin::redoTextChanged) && !(m_connected & RT)){
        connect(m_undo, &QUndoStack::redoTextChanged, this, &UndoWin::redoTextChanged);
        m_connected |= RT;
    }
    if(mm == QMetaMethod::fromSignal(&UndoWin::undoTextChanged) && !(m_connected & UT)){
        connect(m_undo, &QUndoStack::undoTextChanged, this, &UndoWin::undoTextChanged);
        m_connected |= UT;
    }
}

    
bool                UndoWin::isClean() const
{
    return m_undo -> isClean();
}

void                UndoWin::pushUndo(QUndoCommand* cmd) 
{
    m_undo -> push(cmd);
}

void                UndoWin::redo()
{
    m_undo -> redo();
}

QString             UndoWin::redoText() const
{
    return m_undo -> redoText();
}

void                UndoWin::setClean()
{
    m_undo -> setClean();
}

void                UndoWin::undo()
{
    m_undo -> undo();
}

QString             UndoWin::undoText() const
{
    return m_undo -> undoText();
}

