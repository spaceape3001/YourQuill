////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UndoBase.hpp"
#include <QLineEdit>
#include <optional>

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


/*! \brief Line edit that ties into the undo system
*/
class LineEdit : public QLineEdit, public UndoBase {
    Q_OBJECT
public:
    LineEdit(QWidget* parent=nullptr);
    ~LineEdit();
    
        //  returns an unset optional if the line edit is blank
    std::optional<QString>  get() const;
    void                    set(const std::optional<QString>&);

private slots:
    void    createUndo();

private:
    class Undo;
    //  TODO: Currently, the *ACTUAL* undo tie in is not yet implemented
};
