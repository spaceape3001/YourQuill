////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StdTableModel.hpp"
#include <gui/delegate/StringDelegate.hpp>
#include <gui/delegate/EnumDelegate.hpp>
#include <QValidator>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      ADDER
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class StdTableModel<T>::Adder {
    public:

        const QVariant&     message() const { return m_message; }
        virtual Result<T>   make(const QVariant&) = 0;
        virtual ~Adder(){}

    protected:
        Adder(const QVariant&m) : m_message(m) {}
    private:
        QVariant            m_message;
    };


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      DELEGATORS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    template <typename T>
    class StdTableModel<T>::Delegator {
    public:
        virtual Delegate*   create(QObject*)  = 0;
        virtual ~Delegator(){}
    };

    template <typename T, class D>
    class STMSimpleDelegator : public StdTableModel<T>::Delegator {
    public:
        virtual ~STMSimpleDelegator(){}
        virtual Delegate*   create(QObject*par) override
        {
            return new D(par);
        }
    };

    template <typename T, class D>
    class STMValidatorDelegator : public StdTableModel<T>::Delegator {
    public:
        ~STMValidatorDelegator(){}
        virtual Delegate*   create(QObject* par) override
        {
            return new StringDelegate( new D(par), par);
        }
    };


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      COLUMN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class StdTableModel<T>::Column {
public:

    Adder*              adder() const { return m_add.get(); }
    
    Delegate*           delegate(QObject*obj) const
    {
        return m_delegate ? m_delegate->create(obj) : nullptr;
    }

    int                 id() const { return m_id; }
    bool                settable() const { return m_settable; }
    const String&       key() const { return m_key; }
    const QVariant&     label() const { return m_label; }

    virtual QVariant    display(const T&) const = 0;
    virtual QVariant    edit(const T&v) const { return display(v); }
    virtual bool        set(T&, const QVariant&) const { return false; }
    
    bool                sortable() const { return m_sortable; }
    virtual bool        less(const T&, const T&) const { return false; }
    virtual ~Column(){}
    
protected:
    
    Column(const String&k) : m_key(k), m_label(k.qString()), m_id(-1), m_settable(true), m_sortable(false)
    {
    }
    
    friend class StdTableModel;
    friend struct ColumnWriter;
    
    std::unique_ptr<Adder>      m_add;
    std::unique_ptr<Delegator>  m_delegate;
    String                      m_key;
    QVariant                    m_label;
    int                         m_id;
    bool                        m_settable;
    bool                        m_sortable;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      COLUMN WRITER
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct StdTableModel<T>::ColumnWriter {
        
        ColumnWriter&   add(Adder* a)
        {
            if(m_column)
                m_column -> m_add   = a;
            return *this;
        }
        
        ColumnWriter&   label(const QVariant& v)
        {
            if(m_column)
                m_column -> m_label = v;
            return *this;
        }
        
        ColumnWriter&   readOnly() 
        {
            if(m_column)
                m_column -> m_settable  = false;
            return *this;
        }
        
        ColumnWriter&   delegate(Delegator* d)
        {
            if(m_column)
                m_column -> m_delegate  = d;
            return *this;
        }
        
        ColumnWriter&   unsortable()
        {
            if(m_column)
                m_column -> m_sortable  = false;
            return *this;
        }
        

        template <class D>
        ColumnWriter&  delegate()
        {
            if(m_column){
                if constexpr (std::is_base_of_v<Delegate, D>){
                    m_column -> m_delegate.reset(new STMSimpleDelegator<T,D>);
                } else if constexpr(std::is_base_of_v<QValidator, D>){
                    m_column -> m_delegate.reset(new STMValidatorDelegator<T,D>);
                }
            }
            return *this;
        }
        
        
        Column*         m_column;
    };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      VARIOUS COLUMNS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct STMStringMVColumn : public StdTableModel<T>::Column {
        using Column    = typename StdTableModel<T>::Column;
        typedef String  (T::*Ptr);
        Ptr                 m_ptr;
        bool                m_cs;

        STMStringMVColumn(const String&k, Ptr p, bool cs) : Column(k), m_ptr(p), m_cs(cs) 
        {
            Column::m_sortable  = true;
        }
        
        ~STMStringMVColumn(){}

        QVariant    display(const T&d) const override
        {
            return (d.*m_ptr).qString();
        }
        
        bool        set(T& d, const QVariant& v) const override
        {
            d.*m_ptr    = String(v.toString()).trimmed();
            return true;
        }
        
        bool        less(const T& a, const T& b) const override
        {
            return is_less( m_cs ? compare(a.*m_ptr,b.*m_ptr) : compare_igCase(a.*m_ptr,b.*m_ptr));
        }
        
    };


    template <typename T>
    StdTableModel<T>::ColumnWriter        StdTableModel<T>::col(const String&k, String(T::*p), bool cs)
    {
        return col(new STMStringMVColumn(k, p, cs));
    }


    template <typename T, typename E>
    struct STMEnumMVColumn : public StdTableModel<T>::Column {
        using Column    = typename StdTableModel<T>::Column;
        typedef E (T::*Ptr);
        Ptr         m_ptr;
        
        STMEnumMVColumn(const String& k, Ptr p) : Column(k), m_ptr(p)
        {
            Column::m_sortable  = true;
            Column::m_delegate.reset(new STMSimpleDelegator<T, EnumDelegate<E>>);
        }
        
        ~STMEnumMVColumn(){}
        
        QVariant    display(const T&d) const override
        {
            return QVariant::fromValue(d.*m_ptr);
        }
        
        bool        set(T& d, const QVariant& v) const override
        {
            d.*m_ptr    = v.value<E>();
            return true;
        }
        
        bool        less(const T& a, const T& b) const override
        {
            return a.*m_ptr < b.*m_ptr;
        }
    };
        

    template <typename T>
        template <typename E>
    StdTableModel<T>::ColumnWriter        StdTableModel<T>::col(const String&k, EnumImpl<E> (T::*p))
    {
        return col(new STMEnumMVColumn<T,EnumImpl<E>>(k, p));
    }


    template <typename T, typename K, typename V>
    struct  STMEnumMapMVColumn : public StdTableModel<T>::Column {
        using Column = StdTableModel<T>::Column;
        typedef EnumMap<K,V>    (T::*Ptr);
        Ptr             m_ptr;
        K               m_ekey;
        
        STMEnumMapMVColumn(const String&k, K e, Ptr p) : Column(k), m_ptr(p), m_ekey(e) 
        {
            Column::m_sortable  = true;
            Column::m_delegate.reset(new STMSimpleDelegator<T, EnumDelegate<V>>);
        }
        ~STMEnumMapMVColumn(){}
        
        QVariant    display(const T&d) const override
        {
            return QVariant::fromValue((d.*m_ptr)[m_ekey]);
        }

        bool        set(T& d, const QVariant& v) const override
        {
            (d.*m_ptr)[m_ekey]    = v.value<V>();
            return true;
        }
        
        bool        less(const T& a, const T& b) const override
        {
            return (a.*m_ptr)[m_ekey] < (b.*m_ptr)[m_ekey];
        }
    };


    template <typename T>
        template <typename K, typename V>
    StdTableModel<T>::ColumnWriter         StdTableModel<T>::col(const String& k, EnumMap<K,EnumImpl<V>> (T::*p), K e)
    {
        return col(new STMEnumMapMVColumn<T,K,EnumImpl<V>>(k, e, p));
    }

    template <typename T>
        template <typename K, typename V>
    StdTableModel<T>::ColumnWriter         StdTableModel<T>::col(EnumMap<K,EnumImpl<V>> (T::*p), K e)
    {
        return col(new STMEnumMapMVColumn<T,K,EnumImpl<V>>(e.key(), e, p));
    }

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


template <typename T>
StdTableModel<T>::StdTableModel(QObject*parent) : QAbstractTableModel(parent), m_adders(0), m_readOnly(false), m_canAdd(false), m_dragReorder(false)
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
const StdTableModel<T>::Column*       StdTableModel<T>::_column(int c) const
{
    return m_columns.value((size_t) c, nullptr);
}

template <typename T>
const StdTableModel<T>::Column*       StdTableModel<T>::_column(const QModelIndex& idx) const
{
    return _column(idx.column());
}

template <typename T>
T*                  StdTableModel<T>::_row(int r) 
{
    if(r<0)
        return nullptr;
    if(r>=(int) m_rows.size())
        return nullptr;
    return &m_rows[r];
}

template <typename T>
const T*            StdTableModel<T>::_row(int r) const
{
    if(r<0)
        return nullptr;
    if(r>=(int) m_rows.size())
        return nullptr;
    return &m_rows[r];
}

template <typename T>
T*                  StdTableModel<T>::_row(const QModelIndex& idx) 
{   
    return _row(idx.row());
}

template <typename T>
const T*            StdTableModel<T>::_row(const QModelIndex& idx) const
{
    return _row(idx.row());
}


template <typename T>
void                StdTableModel<T>::append(const T& item)
{
    if(m_readOnly)
        return;
    int n   = rowCount();
    beginInsertRows(QModelIndex(), n, n);
    m_rows << item;
    endInsertRows();
}

template <typename T>
void                StdTableModel<T>::allChanged()
{
    dataChanged(createIndex(0,0), createIndex(m_rows.size(), m_columns.size()));
}

template <typename T>
StdTableModel<T>::ColumnWriter  StdTableModel<T>::col(Column* c)
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
void                StdTableModel<T>::clear()
{
    if(m_readOnly)
        return;
    beginRemoveRows(QModelIndex(), 0, m_rows.size());
    m_rows.clear();
    endRemoveRows();
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
QVariant            StdTableModel<T>::data(const QModelIndex&idx, int role) const
{
    const Column* c = _column(idx);
    const T*      r = _row(idx);
    if(!(c&&r))
        return QVariant();
    switch(role){
    case Qt::EditRole:
        return c->edit(*r);
    case Qt::DisplayRole:
        return c->display(*r);
    default:
        return QVariant();
    }
}


template <typename T>
Qt::ItemFlags       StdTableModel<T>::flags(const QModelIndex&idx) const
{
    const Column* c = _column(idx);
    const T*      r = _row(idx);
    if(!(c&&r))
        return Qt::NoItemFlags;
    
    Qt::ItemFlags       ret = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(c->settable() && !m_readOnly)
        ret |= Qt::ItemIsEditable;
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
    T*            r = _row(idx);
    if(!c)
        return false;
    if(role != Qt::EditRole)
        return false;
    if(r){
        if(!c->settable())
            return false;
        bool f = c->set(*r, var);
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
}

template <typename T>
void                StdTableModel<T>::setReadOnly(bool f)
{
    m_readOnly      = f;
}
 
template <typename T>
void                StdTableModel<T>::sort(int col, Qt::SortOrder order) 
{
    const Column*c  = _column(col);
    if(!c)
        return;
    if(!c->sortable())
        return ;
    auto less   = [c](const T& a, const T&b)->bool {
        return c->less(a,b);
    };
    if(order == Qt::AscendingOrder){
        std::sort(m_rows.begin(), m_rows.end(), less);
    } else {
        std::sort(m_rows.rbegin(), m_rows.rend(), less);
    }
    allChanged();
}

