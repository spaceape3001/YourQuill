////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ComboBox.hpp"

class QColor;

class ColorCombo : public ComboBox {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor USER true)
public:
    ColorCombo(QWidget* parent=nullptr);
    virtual ~ColorCombo();
    
    QColor color() const;
    void    setColor(QColor);
    
    //  TODO: Currently, the *ACTUAL* undo tie in is not yet implemented

private:
    void    populate();
};

