////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ComboBox.hpp"
#include "util/Enum.hpp"
#include <optional>


class GenericEnumCombo : public ComboBox {
    Q_OBJECT
public:
    
    GenericEnumCombo(const EnumDef*, QWidget*parent=nullptr);
    virtual ~GenericEnumCombo();
    
    Enum    enumValue() const;
    int     intValue() const;
    
    bool    setIntValue(int);
    bool    setEnumValue(Enum);
    
private:
    const EnumDef* const    m_enum;
    Map<int,int>            m_val2idx;
    Map<int,int>            m_idx2val;
};
