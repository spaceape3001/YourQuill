////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EnumCombo.hpp"


GenericEnumCombo::GenericEnumCombo(const EnumDef*def, QWidget*parent) : ComboBox(parent), m_enum(def)
{
    assert(def);
    
    int     i   = 0;
    for(const String& k : m_enum -> all_keys()){
        addItem(k.qString());
        int val         = m_enum -> value_of(k);
        m_idx2val[i]    = val;
        m_val2idx[val]  = i;
        ++i;
    }
    setIntValue(m_enum -> default_value());
}

GenericEnumCombo::~GenericEnumCombo()
{
}

Enum    GenericEnumCombo::enumValue() const
{
    int     i   = currentIndex();
    auto itr    = m_idx2val.find(i);
    if(itr != m_idx2val.end())
        return m_enum -> make_enum(itr -> second);
    else
        return Enum();
}

int     GenericEnumCombo::intValue() const
{
    return m_idx2val.get(currentIndex(), m_enum -> default_value());
}


bool    GenericEnumCombo::setIntValue(int v)
{
    int i       = m_val2idx.get( v, -1);
    if(i >= 0){
        setCurrentIndex(i);
        return true;
    } else
        return false;
}

bool    GenericEnumCombo::setEnumValue(Enum e)
{
    if(e.definition() != m_enum)
        return false;
    return setIntValue(e.value());
}


#include "moc_GenericEnumCombo.cpp"
