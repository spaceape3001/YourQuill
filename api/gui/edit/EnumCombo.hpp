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
    using StringDescVec  = typename E::StringDescVec;
    using QStringDescVec = typename E::QStringDescVec;

    static auto to_int(const QStringDescVec& them)
    {
        Vector<std::pair<QString,int>> ret;
        for(auto& i : them)
            ret << QStringIntPair{ i.first, (int) i.second };
        return ret;
    }

    static auto to_int(const StringDescVec& them)
    {
        Vector<std::pair<QString,int>> ret;
        for(auto& i : them)
            ret << QStringIntPair{ i.first, (int) i.second.qString() };
        return ret;
    }

    EnumCombo(QWidget*parent=nullptr) : GenericEnumCombo( E::staticEnumInfo(), parent ) 
    {
    }
    
    EnumCombo(const QStringDescVec& values, QWidget*parent=nullptr) : 
        GenericEnumCombo( E::staticEnumInfo(), to_int(values), parent ) 
    {
    }

    EnumCombo(const StringDescVec& values, QWidget*parent=nullptr) : 
        GenericEnumCombo( E::staticEnumInfo(), to_int(values), parent ) 
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

