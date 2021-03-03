////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LineEdit.hpp"

class UShortSetEdit : public LineEdit {
    Q_OBJECT
public:
    UShortSetEdit(QWidget*parent=nullptr);
    ~UShortSetEdit();
    
    Set<uint16_t>       get() const;
    void                set(const Set<uint16_t>&);
};
