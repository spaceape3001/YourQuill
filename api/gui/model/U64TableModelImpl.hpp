////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "U64TableModel.hpp"
#include "StdTableModelImpl.hpp"

template <typename T>
U64TableModel<T>::U64TableModel(P p, QObject*parent) : StdTableModel<T>(parent), m_source(p)
{
    assert(m_source);
    StdTableModel<T>::customRO("ID", [](T v) -> auto {
        return (quint64) v.id;
    });
    _refresh();
    
}

template <typename T>
U64TableModel<T>::~U64TableModel()
{
}

template <typename T>
void    U64TableModel<T>::_refresh()
{
    auto cur    = m_source -> all();
    for(T t : cur)
        StdTableModel<T>::m_rows.push_back({ t, false });
    m_current = makeSet(cur);
}


template <typename T>
void    U64TableModel<T>::append(const T&v) 
{
    if(v == T{})
        return ;
    if(m_current.has(v))
        return ;
    m_current << v;
    StdTableModel<T>::append(v);
}



template <typename T>
void    U64TableModel<T>::check()
{
    Vector<T>   cur     = m_source -> all();
    if(cur.empty()){       // nothing from nothing....
        if(StdTableModel<T>::m_rows.empty())
            return ;
        StdTableModel<T>::clear();
        return ;
    }
    
    Set<T> c = makeSet(cur);    // set of current
    Set<T> n = c - m_current;   // set of new
    Set<T> r = m_current - c;   // set of removed
    Set<T> u = c & m_current;   // set of update-only
    
        // strip the incoming vector of exisitng items
    cur.erase_if([&](T t) -> bool { 
        return m_current.has(t);
    });
    
        // exectute the removal
    StdTableModel<T>::removeRowIf([&](T t) -> bool {
        return r.has(t);
    });
    
    StdTableModel<T>::allChanged();     // update existing
    m_current  = c;                     // set new registry
    StdTableModel<T>::append(cur);      // Add new items 
}


template <typename T>
T       U64TableModel<T>::get(int r) const
{
    auto t = StdTableModel<T>::_row(r);
    return t ? t->data : T{};
}

template <typename T>
T       U64TableModel<T>::get(const QModelIndex&idx) const
{
    return get(idx.row());
}

template <typename T>
bool    U64TableModel<T>::has(T v) const
{
    return m_current.has(v);
}

template <typename T>
bool    U64TableModel<T>::isTicked(T t) const
{
    for(auto & r : StdTableModel<T>::m_rows)
        if((r.data == t) && r.tick)
            return true;
    return false;
}


// SLOTS, but not slots due to it being a template
template <typename T>
void    U64TableModel<T>::refresh()
{
    QAbstractTableModel::beginResetModel();
    _refresh();
    QAbstractTableModel::endResetModel();
}

template <typename T>
void    U64TableModel<T>::setTicked(const Set<T>& them)
{
    for(auto& r : StdTableModel<T>::m_rows)
        r.tick  = them.has(r.data);
    StdTableModel<T>::updateTickColumn();
}

template <typename T>
void    U64TableModel<T>::tick(T t)
{
    for(auto& r : StdTableModel<T>::m_rows)
        if(r.data == t)
            r.tick  = true;
    StdTableModel<T>::updateTickColumn();
}

template <typename T>
Set<T>  U64TableModel<T>::ticked() const
{
    Set<T>  ret;
    for(auto& r : StdTableModel<T>::m_rows)
        if(r.tick)
            ret << r.data;
    return ret;
}

template <typename T>
void    U64TableModel<T>::untick(T t)
{
    for(auto& r : StdTableModel<T>::m_rows)
        if(r.data == t)
            r.tick  = false;
    StdTableModel<T>::updateTickColumn();
}

