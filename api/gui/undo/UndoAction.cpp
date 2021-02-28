////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "UndoAction.hpp"
#include <QUndoCommand>

UndoAction::UndoAction (QUndoCommand* cmd) : m_action(cmd)
{
}

UndoAction::UndoAction (UndoAction&& mv) : m_action(nullptr)
{
    std::swap(m_action, mv.m_action);
}

UndoAction::~UndoAction()
{
    if(m_action){
        m_action->redo();
        m_action   = nullptr;
    }
}

UndoAction& UndoAction::operator=(UndoAction&&mv)
{
    if(this != &mv){
        if(m_action){
            m_action->redo();
            m_action   = nullptr;
        }
        std::swap(m_action, mv.m_action);
    }
    return *this;
}

QUndoCommand*       UndoAction::release()
{
    QUndoCommand*   cmd = nullptr;
    std::swap(cmd, m_action);
    return cmd;
}
