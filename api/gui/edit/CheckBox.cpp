////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CheckBox.hpp"
#include <QUndoCommand>

class CheckBox::UndoClick : public QUndoCommand {
public:
    UndoClick(CheckBox* cb, Qt::CheckState oldState) : m_cb(cb), m_old(oldState), m_first(true)
    {
        m_new       = cb -> checkState();
    }
    
    void            redo()
    {
        if(m_first){
            m_first = false;
        } else {
            m_cb -> setCheckState(m_new);
        }
    }
    
    void            undo()
    {
        m_cb -> setCheckState(m_old);
    }
    
    CheckBox*       m_cb;
    Qt::CheckState  m_old, m_new;
    bool            m_first;
};

CheckBox::CheckBox(QWidget* parent) : QCheckBox(parent), m_old{}
{
}

CheckBox::CheckBox(const QString& text, QWidget* parent) : QCheckBox(text, parent), m_old{}
{
}

CheckBox::~CheckBox()
{
}

void    CheckBox::checkUndo()
{
    if(checkState() != m_old)
        execute(new UndoClick(this, m_old));
}


bool    CheckBox::isChecked() const
{
    return checkState() ? true : false;
}

void    CheckBox::setChecked(bool f)
{
    setCheckState(f?Qt::Checked:Qt::Unchecked);
}

void    CheckBox::keyPressEvent(QKeyEvent* evt) 
{
    m_old   = checkState();
    QCheckBox::keyPressEvent(evt);
}

void    CheckBox::keyReleaseEvent(QKeyEvent* evt) 
{
    QCheckBox::keyReleaseEvent(evt);
    checkUndo();
}

void    CheckBox::mousePressEvent(QMouseEvent* evt)  
{
    m_old       = checkState();
    QCheckBox::mousePressEvent(evt);
}

void    CheckBox::mouseReleaseEvent(QMouseEvent* evt) 
{
    QCheckBox::mouseReleaseEvent(evt);
    checkUndo();
}


#include "moc_CheckBox.cpp"
