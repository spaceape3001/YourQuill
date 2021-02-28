////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GenericEnumCombo.hpp"
#include <optional>

template <typename E>
class EnumCombo : public GenericEnumCombo {
public:
    EnumCombo(QWidget*parent=nullptr) : GenericEnumCombo( E::staticEnumInfo() ) 
    {
    }
    
    virtual ~EnumCombo()
    {
    }
    
    void    setValue(E val)
    {
        GenericEnumCombo::setIntValue(val.value());
    }
    
    E       value() const 
    {
        return E(GenericEnumCombo::intValue());
    }
    
    void    set(const std::optional<E>& val,const E& def=E())
    {
        setValue(val?*val:def);
    }
    
    std::optional<E> get(const E& def=E()) const
    {
        std::optional<E> ret;
        E   val = value();
        if(val != def)
            ret = val;
        return ret;
    }
};

