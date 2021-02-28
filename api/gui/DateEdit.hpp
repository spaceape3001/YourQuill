////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UndoBase.hpp"
#include <QDateEdit>

class DateEdit : public QDateEdit, public UndoBase {
    Q_OBJECT
public:
    DateEdit(QWidget* parent = nullptr);
    ~DateEdit();
    //  TODO: Currently, the *ACTUAL* undo tie in is not yet implemented
};

