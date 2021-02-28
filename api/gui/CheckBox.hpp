////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UndoBase.hpp"
#include <QCheckBox>

/*! \brief Undoable checkbox with boolean value
*/
class CheckBox : public QCheckBox, public UndoBase {
    Q_OBJECT
public:
    CheckBox(QWidget* parent=nullptr);
    CheckBox(const QString& text, QWidget* parent=nullptr);
    virtual     ~CheckBox();
    
    //  TODO: Currently, the *ACTUAL* undo tie in is not yet implemented

    bool        isChecked() const;
    void        setChecked(bool);
    
private:
    Qt::CheckState  m_old;    //<! For tracking user's clicks....
    virtual void    keyPressEvent(QKeyEvent*) override;
    virtual void    keyReleaseEvent(QKeyEvent*) override;
    virtual void    mousePressEvent(QMouseEvent*) override;
    virtual void    mouseReleaseEvent(QMouseEvent*) override;
    void            checkUndo();
    class UndoClick;
};
