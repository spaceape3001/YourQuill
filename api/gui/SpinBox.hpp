////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UndoBase.hpp"
#include <QSpinBox>
#include <optional>

class SpinBox : public QSpinBox, public UndoBase {
    Q_OBJECT
public:
    SpinBox(QWidget*parent=nullptr);
    ~SpinBox();
    
    //  TODO: Currently, the *ACTUAL* undo tie in is not yet implemented
    void                        set(const std::optional<unsigned int>&, unsigned int z=0);
    std::optional<unsigned int> getUInt(unsigned int z=0) const;

public slots:   
    virtual void    reset();
};
