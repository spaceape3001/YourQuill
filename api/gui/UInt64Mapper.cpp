////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "UInt64Mapper.hpp"

UInt64Mapper::UInt64Mapper(QObject*parent) : QObject(parent)
{
}

UInt64Mapper::~UInt64Mapper()
{
}

void        UInt64Mapper::setMapping(QObject*obj, uint64_t val)
{
    m_wid2u64[obj]  = (quint64) val;
}


void        UInt64Mapper::map()
{
    QObject *obj    = sender();
    auto i = m_wid2u64.find(obj);
    if(i != m_wid2u64.end())
        mapped(i->second);
}


#include "moc_UInt64Mapper.cpp"

