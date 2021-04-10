////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    using EnumList = Vector<QStringIntPair>;

    auto makePairs(const EnumDef* def)
    {
        EnumList  ret;
        for(const String& s : def -> all_keys())
            ret << QStringIntPair{ s.qString(), def -> value_of(s) };
        return ret;
    }
    
    const EnumList& autoPairs(const EnumDef* def)
    {
        static Map<const EnumDef*,EnumList> s_map;
        EnumList&       l   = s_map[def];
        if(def && l.empty())
            l   = makePairs(def);
        return l;
    }
}


GenericEnumCombo::GenericEnumCombo(const EnumDef*def, const EnumList& values, QWidget*parent)  : ComboBox(parent), m_enum(def)
{
    assert(def);
    
    int     i   = 0;
    for(auto& k : values){
        addItem(k.first);
        m_idx2val[i]            = k.second;
        m_val2idx[k.second]     = i;
        ++i;
    }
    setIntValue(m_enum -> default_value());
}

GenericEnumCombo::GenericEnumCombo(const EnumDef*def, QWidget*parent) : GenericEnumCombo(def, autoPairs(def), parent)
{
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


