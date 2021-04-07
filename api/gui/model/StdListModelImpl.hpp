////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StdListModel.hpp"
#include "StdColumn.hpp"

template <typename T>
StdListModel<T>::StdListModel(QObject* parent) : m_column(nullptr), m_header(nullptr)
{
}

template <typename T>
StdListModel<T>::~StdListModel()
{
    if(m_column){
        delete m_column;
        m_column = nullptr;
    }
    if(m_header){
        delete m_header;
        m_header = {};
    }
}

template <typename T>
StdListModel<T>::Row*  StdListModel<T>::_row(int r)
{
    if(r<0)
        return nullptr;
    if(r>=(int) m_rows.size())
        return nullptr;
    return &m_rows[r];
}

template <typename T>
const StdListModel<T>::Row*  StdListModel<T>::_row(int r) const
{
    if(r<0)
        return nullptr;
    if(r>=(int) m_rows.size())
        return nullptr;
    return &m_rows[r];
}

template <typename T>
StdListModel<T>::Row*  StdListModel<T>::_row(const QModelIndex& idx)
{
    return _row(idx.row());
}

template <typename T>
const StdListModel<T>::Row*  StdListModel<T>::_row(const QModelIndex& idx) const
{
    return _row(idx.row());
}

template <typename T>
void                StdListModel<T>::allChanged()
{
    dataChanged(createIndex(0,0), createIndex(m_rows.size(), 1));
}

template <typename T>
void                StdListModel<T>::append(const T& item)
{
    if(m_readOnly)
        return ;
    int n   = rowCount();
    beginInsertRows(QModelIndex(), n, n);
    m_rows << Row{item, false};
    endInsertRows();
}

template <typename T>
void                StdListModel<T>::append(const Vector<T>&items)
{
    if(m_readOnly || items.empty())
        return;
    int n   = rowCount();
    beginInsertRows(QModelIndex(), n, n+items.size());
    for(auto t : items)
        m_rows << Row{t,false};
    endInsertRows();
}


template <typename T>
void                StdListModel<T>::clear()
{
    if(m_readOnly)
        return;
    beginRemoveRows(QModelIndex(), 0, m_rows.size());
    m_rows.clear();
    endRemoveRows();
}

template <typename T>
StdColumnWriter<T>  StdListModel<T>::col(Column* c)
{
    if(!c || (c == m_column))
        return StdColumnWriter<T>{};
    if(m_column)
        delete m_column;
    m_column    = c;
    return StdColumnWriter<T>{c};
}


template <typename T>
StdColumnWriter<T>  StdListModel<T>::col(const String& k, String(T::*p), bool cs)
{
    return col(new SCMStringMV(k, p, cs));
}

template <typename T>
    template <typename E>
StdColumnWriter<T>  StdListModel<T>::col(const String&k, EnumImpl<E> (T::*p))
{
    return col(new SCMEnumMV<T,EnumImpl<E>>(k, p));
}

template <typename T>
    template <typename K, typename V>
StdColumnWriter<T>  StdListModel<T>::col(EnumMap<K,EnumImpl<V>> (T::*p), K e)
{
    return col(new SCMEnumMapMV<T,K,EnumImpl<V>>(e.key(), e, p));
}

template <typename T>
    template <typename K, typename V>
StdColumnWriter<T>  StdListModel<T>::col(const String&k, EnumMap<K,EnumImpl<V>> (T::*p), K e)
{
    return col(new SCMEnumMapMV<T,K,EnumImpl<V>>(k, e, p));
}

template <typename T>
    template <typename Getter>
StdColumnWriter<T>  StdListModel<T>::customRO(const String&k, Getter g)
{
    return col(new SCMCustomRO<T,Getter>(k, g));
}

template <typename T>
    template <typename Getter, typename Less>
StdColumnWriter<T>  StdListModel<T>::customRO(const String&k, Getter g, Less l)
{
    return col(new SCMCustomROL<T,Getter,Less>(k,g,l));
}

template <typename T>
    template <typename Getter, typename Setter>
StdColumnWriter<T>  StdListModel<T>::customRW(const String&k, Getter g, Setter s)
{
    return col(new SCMCustomRW<T,Getter,Setter>(k,g,s));
}

template <typename T>
    template <typename Getter, typename Setter, typename Less>
StdColumnWriter<T>  StdListModel<T>::customRW(const String&k, Getter g, Setter s, Less l)
{
    return col(new SCMCustomRWL<T,Getter,Setter,Less>(k, g, s, l));
}

template <typename T>
QVariant            StdListModel<T>::data(const QModelIndex&idx, int role) const 
{
    if(!m_column)
        return QVariant();
        
    const Row*    r = _row(idx);
    if(!r)
        return QVariant();
        
    switch(role){
    case Qt::EditRole:
        return m_column->edit(r->data);
    case Qt::DisplayRole:
        return m_column->display(r->data);
    case Qt::DecorationRole:
        return m_column->decoration(r->data);
    case Qt::CheckStateRole:
        if(m_tickable){
            return r->tick ? Qt::Checked : Qt::Unchecked;
        }
        return QVariant();
    case Qt::ToolTipRole:
        return m_column->toolTip(r->data);
    case Qt::StatusTipRole: 
        return m_column->statusTip(r->data);
    case Qt::WhatsThisRole:
        return m_column->whatsThis(r->data);
    case Qt::SizeHintRole:
        return m_column->sizeHint(r->data);
    case Qt::FontRole:
        return m_column->font(r->data);
    case Qt::TextAlignmentRole:
        return m_column->textAlign(r->data);
    case Qt::BackgroundRole:
        return m_column->background(r->data);
    case Qt::ForegroundRole:
        return m_column->foreground(r->data);
    case Qt::AccessibleTextRole:
        return m_column->accText(r->data);
    case Qt::AccessibleDescriptionRole:
        return m_column->accDesc(r->data);
    default:
        return QVariant();
    }
}

template <typename T>
Qt::ItemFlags       StdListModel<T>::flags(const QModelIndex&idx) const 
{
    if(!m_column)
        return Qt::NoItemFlags;
    
    const Row*    r = _row(idx);
            // factor in "ADD" later....
    if(!r)
        return Qt::NoItemFlags;
    Qt::ItemFlags       ret = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(m_column->settable() && !m_readOnly)
        ret |= Qt::ItemIsEditable;
    if(m_tickable)
        ret |= Qt::ItemIsUserCheckable;
    return ret;
}

template <typename T>
QVariant            StdListModel<T>::headerData(int n, Qt::Orientation ori, int role) const 
{
    if(ori == Qt::Horizontal)
        return m_label;
    if(!m_header)
        return QVariant();
        
    const Row*    r = _row(n);
    if(!r)
        return QVariant();

    switch(role){
    case Qt::EditRole:
        return m_header->edit(r->data);
    case Qt::DisplayRole:
        return m_header->display(r->data);
    case Qt::DecorationRole:
        return m_header->decoration(r->data);
    case Qt::CheckStateRole:
        return QVariant();
    case Qt::ToolTipRole:
        return m_header->toolTip(r->data);
    case Qt::StatusTipRole: 
        return m_header->statusTip(r->data);
    case Qt::WhatsThisRole:
        return m_header->whatsThis(r->data);
    case Qt::SizeHintRole:
        return m_header->sizeHint(r->data);
    case Qt::FontRole:
        return m_header->font(r->data);
    case Qt::TextAlignmentRole:
        return m_header->textAlign(r->data);
    case Qt::BackgroundRole:
        return m_header->background(r->data);
    case Qt::ForegroundRole:
        return m_header->foreground(r->data);
    case Qt::AccessibleTextRole:
        return m_header->accText(r->data);
    case Qt::AccessibleDescriptionRole:
        return m_header->accDesc(r->data);
    default:
        return QVariant();
    }
}


template <typename T>
void                StdListModel<T>::moveRowUp(int n)
{
    if(m_readOnly || (n<0) || m_rows.empty() || (n>=(int)m_rows.size()-1))
        return ;
    beginMoveRows(QModelIndex(), n, n, QModelIndex(), n+1);
    std::swap(m_rows[n], m_rows[n+1]);
    endMoveRows();
}


template <typename T>
void                StdListModel<T>::moveRowDown(int n)
{
    if(m_readOnly || (n<=0) || m_rows.empty() || (n>=(int)m_rows.size()))
        return;
    
    beginMoveRows(QModelIndex(), n, n, QModelIndex(), n-1);
    std::swap(m_rows[n], m_rows[n-1]);
    endMoveRows();
}

template <typename T>
void                StdListModel<T>::removeRow(int n)
{
    if(m_readOnly)
        return;
    if(!_row(n))
        return ;
        
    beginRemoveRows(QModelIndex(), n, n);
    m_rows.erase_at(n);
    endRemoveRows();
}


template <typename T>
    template <typename Pred>
void                StdListModel<T>::removeRowIf(Pred p)
{
    if(m_readOnly || m_rows.empty())
        return ;
    
    int l   = -1;
    int n   = -1;
    for(n=m_rows.size()-1;n>=0;--n){
        if(p(m_rows[n].data)){
            if(l<0)
                l   = n;
        } else if(l>=0){
            //  Got a bunch to remove
            beginRemoveRows(QModelIndex(), n+1, l);
            m_rows.erase_at((size_t) n+1, (size_t)(l-n));
            endRemoveRows();
            l = -1;
        }
    }
    
    if(l>=0){
        beginRemoveRows(QModelIndex(), 0, l);
        m_rows.erase_at(0, l+1);
        endRemoveRows();
    }
}

template <typename T>
int                 StdListModel<T>::rowCount() const
{
    return static_cast<int>(m_canAdd && !m_readOnly) + (int) m_rows.size();
}

template <typename T>
void                StdListModel<T>::setAllData(const std::vector<T>& newVals)
{
    beginResetModel();
    m_rows.clear();
    for(auto& t : newVals)
        m_rows << Row{ t, false };
    endResetModel();
}

template <typename T>
bool                StdListModel<T>::setData(const QModelIndex& idx, const QVariant& var, int role) 
{
    if(m_readOnly)
        return false;
    if(!m_column)
        return false;
    Row*          r = _row(idx);
    switch(role){
    case Qt::EditRole:
        if(r){
            if(!m_column->settable())
                return false;
            bool f = m_column->set(r->data, var);
            if(f)
                emit dataChanged(idx, idx);
            return f;
        } else if(m_column->adder()){
            auto  res   = m_column->adder()->make(var);
            if(res.good)
                append(res.value);
            return res.good;
        } else
            return false;
        break;
    case Qt::CheckStateRole:
        if(r && m_tickable){
            r->tick = var.value<Qt::CheckState>() != Qt::Unchecked;
            emit dataChanged(idx, idx);
            return true;
        }
        break;
    }
    return false;
}

template <typename T>
void                StdListModel<T>::setReadOnly(bool f)
{
    m_readOnly      = f;
}

template <typename T>
void                StdListModel<T>::setTickable(bool b)
{
    m_tickable  = b;
}
 
template <typename T>
void                StdListModel<T>::sort(int, Qt::SortOrder order) 
{
    if(m_column && m_column->sortable()){
        auto less   = [&](const Row& a, const Row&b)->bool {
            return m_column->less(a.data,b.data);
        };
        if(order == Qt::AscendingOrder){
            std::sort(m_rows.begin(), m_rows.end(), less);
        } else {
            std::sort(m_rows.rbegin(), m_rows.rend(), less);
        }
        allChanged();
    }
}

