////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StdTableModel.hpp"
#include "StdColumn.hpp"


template <typename T>
StdTableModel<T>::StdTableModel(QObject*parent) : QAbstractTableModel(parent), 
    m_adders(0), m_readOnly(false), m_canAdd(false), m_dragReorder(false), m_tickCol(-1)
{
}

template <typename T>
StdTableModel<T>::~StdTableModel()
{
    for(Column* c : m_columns)
        delete c;
    m_columns.clear();
}

template <typename T>
const StdColumn<T>*       StdTableModel<T>::_column(int c) const
{
    return m_columns.value((size_t) c, nullptr);
}

template <typename T>
const StdColumn<T>*       StdTableModel<T>::_column(const QModelIndex& idx) const
{
    return _column(idx.column());
}

template <typename T>
StdTableModel<T>::Row*          StdTableModel<T>::_row(int r) 
{
    if(r<0)
        return nullptr;
    if(r>=(int) m_rows.size())
        return nullptr;
    return &m_rows[r];
}

template <typename T>
const StdTableModel<T>::Row*    StdTableModel<T>::_row(int r) const
{
    if(r<0)
        return nullptr;
    if(r>=(int) m_rows.size())
        return nullptr;
    return &m_rows[r];
}

template <typename T>
StdTableModel<T>::Row*          StdTableModel<T>::_row(const QModelIndex& idx) 
{   
    return _row(idx.row());
}

template <typename T>
const StdTableModel<T>::Row*    StdTableModel<T>::_row(const QModelIndex& idx) const
{
    return _row(idx.row());
}

template <typename T>
void                StdTableModel<T>::allChanged()
{
    dataChanged(createIndex(0,0), createIndex(m_rows.size(), m_columns.size()));
}

template <typename T>
void                StdTableModel<T>::append(const T& item)
{
    if(m_readOnly)
        return;
    int n   = rowCount();
    beginInsertRows(QModelIndex(), n, n);
    m_rows << Row{item, false};
    endInsertRows();
}

template <typename T>
void                StdTableModel<T>::append(const Vector<T>&items)
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
void                StdTableModel<T>::clear()
{
    if(m_readOnly)
        return;
    beginRemoveRows(QModelIndex(), 0, m_rows.size());
    m_rows.clear();
    endRemoveRows();
}

template <typename T>
StdColumnWriter<T>  StdTableModel<T>::col(Column* c)
{
    if(!c)
        return ColumnWriter{};
    c -> m_id = (int) m_columns.size();
    m_columns << c;
    if(c->adder())
        ++m_adders;
    return ColumnWriter{c};
}
    

template <typename T>
StdColumnWriter<T>        StdTableModel<T>::col(const String&k, String(T::*p), bool cs)
{
    return col(new SCMStringMV(k, p, cs));
}

template <typename T>
    template <typename E>
StdColumnWriter<T>        StdTableModel<T>::col(const String&k, EnumImpl<E> (T::*p))
{
    return col(new SCMEnumMV<T,EnumImpl<E>>(k, p));
}

template <typename T>
    template <typename K, typename V>
StdColumnWriter<T>         StdTableModel<T>::col(const String& k, EnumMap<K,EnumImpl<V>> (T::*p), K e)
{
    return col(new SCMEnumMapMV<T,K,EnumImpl<V>>(k, e, p));
}

template <typename T>
    template <typename K, typename V>
StdColumnWriter<T>         StdTableModel<T>::col(EnumMap<K,EnumImpl<V>> (T::*p), K e)
{
    return col(new SCMEnumMapMV<T,K,EnumImpl<V>>(e.key(), e, p));
}


template <typename T>
int                 StdTableModel<T>::column(const String& k) const
{
    for(const Column* c : m_columns)
        if(is_equal(compare_igCase(c->key(), k)))
            return c->id();
    return -1;
}

template <typename T>
String              StdTableModel<T>::columnKey(int i) const
{
    const Column*c  = _column(i);
    return c ? c -> key() : String();
}

template <typename T>
String              StdTableModel<T>::columnKey(const QModelIndex&idx) const
{
    return columnKey(idx.column());
}


template <typename T>
    template <typename Getter>
StdColumnWriter<T>        StdTableModel<T>::customRO(const String&k, Getter g)
{
    return col(new SCMCustomRO<T,Getter>(k, g));
}

template <typename T>
    template <typename Getter, typename Less>
StdColumnWriter<T>        StdTableModel<T>::customRO(const String&k, Getter g, Less l)
{
    return col(new SCMCustomROL<T,Getter,Less>(k,g,l));
}


template <typename T>
    template <typename Getter, typename Setter>
StdColumnWriter<T>        StdTableModel<T>::customRW(const String& k, Getter g, Setter s)
{
    return col(new SCMCustomRW<T,Getter,Setter>(k,g,s));
}


template <typename T>
    template <typename Getter, typename Setter, typename Less>
StdColumnWriter<T>        StdTableModel<T>::customRW(const String&k, Getter g, Setter s, Less l)
{
    return col(new SCMCustomRWL<T,Getter,Setter,Less>(k, g, s, l));
}


template <typename T>
QVariant            StdTableModel<T>::data(const QModelIndex&idx, int role) const
{
    const Column* c = _column(idx);
    const Row*    r = _row(idx);
    if(!(c&&r))
        return QVariant();
    switch(role){
    case Qt::EditRole:
        return c->edit(r->data);
    case Qt::DisplayRole:
        return c->display(r->data);
    case Qt::DecorationRole:
        return c->decoration(r->data);
    case Qt::CheckStateRole:
        if(idx.column() == m_tickCol){
            return r->tick ? Qt::Checked : Qt::Unchecked;
        }
        return QVariant();
    case Qt::ToolTipRole:
        return c->toolTip(r->data);
    case Qt::StatusTipRole: 
        return c->statusTip(r->data);
    case Qt::WhatsThisRole:
        return c->whatsThis(r->data);
    case Qt::SizeHintRole:
        return c->sizeHint(r->data);
    case Qt::FontRole:
        return c->font(r->data);
    case Qt::TextAlignmentRole:
        return c->textAlign(r->data);
    case Qt::BackgroundRole:
        return c->background(r->data);
    case Qt::ForegroundRole:
        return c->foreground(r->data);
    case Qt::AccessibleTextRole:
        return c->accText(r->data);
    case Qt::AccessibleDescriptionRole:
        return c->accDesc(r->data);
    default:
        return QVariant();
    }
}


template <typename T>
Qt::ItemFlags       StdTableModel<T>::flags(const QModelIndex&idx) const
{
    const Column* c = _column(idx);
    const Row*    r = _row(idx);
    if(!c)
        return Qt::NoItemFlags;
        
            // factor in "ADD" later....
    if(!r)
        return Qt::NoItemFlags;
    
    Qt::ItemFlags       ret = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(c->settable() && !m_readOnly)
        ret |= Qt::ItemIsEditable;
    if(idx.column() == m_tickCol)
        ret |= Qt::ItemIsUserCheckable;
    return ret;
}

template <typename T>
QVariant            StdTableModel<T>::headerData(int n, Qt::Orientation ori, int role) const 
{
    if((role == Qt::DisplayRole) && (ori == Qt::Horizontal)){
        const Column*   c    = _column(n);
        if(c) 
            return c->label();
    }
    return QAbstractTableModel::headerData(n, ori, role);
}

template <typename T>
void                StdTableModel<T>::moveRowUp(int n)
{
    if(m_readOnly || (n<0) || m_rows.empty() || (n>=(int)m_rows.size()-1))
        return ;
    beginMoveRows(QModelIndex(), n, n, QModelIndex(), n+1);
    std::swap(m_rows[n], m_rows[n+1]);
    endMoveRows();
}

template <typename T>
void                StdTableModel<T>::moveRowDown(int n)
{
    if(m_readOnly || (n<=0) || m_rows.empty() || (n>=(int)m_rows.size()))
        return;
    
    beginMoveRows(QModelIndex(), n, n, QModelIndex(), n-1);
    std::swap(m_rows[n], m_rows[n-1]);
    endMoveRows();
}


template <typename T>
void                StdTableModel<T>::removeRow(int n)
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
void                StdTableModel<T>::removeRowIf(Pred p)
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
int                 StdTableModel<T>::rowCount() const
{
    return static_cast<int>(m_canAdd && !m_readOnly) + (int) m_rows.size();
}


template <typename T>
bool                StdTableModel<T>::setData(const QModelIndex&idx, const QVariant&var, int role) 
{
    if(m_readOnly)
        return false;
        
    const Column* c = _column(idx);
    Row*          r = _row(idx);
    if(!c)
        return false;
    switch(role){
    case Qt::EditRole:
        if(r){
            if(!c->settable())
                return false;
            bool f = c->set(r->data, var);
            if(f)
                emit dataChanged(idx, idx);
            return f;
        } else if(c->adder()){
            auto  res   = c->adder()->make(var);
            if(res.good)
                append(res.value);
            return res.good;
        } else
            return false;
        break;
    case Qt::CheckStateRole:
        if(r && idx.column() == m_tickCol){
            r->tick = var.value<Qt::CheckState>() != Qt::Unchecked;
            emit dataChanged(idx, idx);
            return true;
        }
        break;
    }
    return false;
}

template <typename T>
void                StdTableModel<T>::setReadOnly(bool f)
{
    m_readOnly      = f;
}

template <typename T>
void                StdTableModel<T>::setTickColumn(const String& k)
{
    int n   = column(k);
    if(n>=0)
        setTickColumn(n);
}

template <typename T>
void                StdTableModel<T>::setTickColumn(int n)
{
    if(n>=-1 && n<(int)m_columns.size())
        m_tickCol       = n;
}

 
template <typename T>
void                StdTableModel<T>::sort(int col, Qt::SortOrder order) 
{
    const Column*c  = _column(col);
    if(!c)
        return;
    if(!c->sortable())
        return ;
    auto less   = [c](const Row& a, const Row&b)->bool {
        return c->less(a.data,b.data);
    };
    if(order == Qt::AscendingOrder){
        std::sort(m_rows.begin(), m_rows.end(), less);
    } else {
        std::sort(m_rows.rbegin(), m_rows.rend(), less);
    }
    allChanged();
}

template <typename T>
void                StdTableModel<T>::updateTickColumn()
{
    if(m_tickCol >= 0){
        QAbstractTableModel::dataChanged(
            QAbstractTableModel::createIndex(0,m_tickCol),
            QAbstractTableModel::createIndex((int) m_rows.size(),m_tickCol)
        );
    }
}

