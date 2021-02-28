////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "gui/UndoBase.hpp"
#include <QDoubleSpinBox>


class DoubleSpinBox : public QDoubleSpinBox, public UndoBase {
    Q_OBJECT
public:
    DoubleSpinBox(QWidget*parent=nullptr);
    ~DoubleSpinBox();
    
    //  TODO: Currently, the *ACTUAL* undo tie in is not yet implemented

public slots:   
    virtual void    reset();
};
