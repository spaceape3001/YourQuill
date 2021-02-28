////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "gui/UndoBase.hpp"
#include <QComboBox>


/*! \brief Combo Box with undo API
*/
class ComboBox : public QComboBox, public UndoBase {
    Q_OBJECT
public:

    ComboBox(QWidget* parent=nullptr);
    virtual ~ComboBox();
    
public slots:
    virtual void    reset();
};

