////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractTableModel>

#include <meta/Variant.hpp>
#include <util/Compare.hpp>
#include <util/Vector.hpp>

#include <QVariant>
#include <exception>

class QValidator;

template <typename T>
class StdTableModel : public QAbstractTableModel {
public:

    using Base = StdTableModel;

    StdTableModel(QObject*parent=nullptr);
    virtual ~StdTableModel();
    
    void                append(const T&);

    void                clear();
    int                 column(const String&) const;

    int                 columnCount() const
    {
        return (int) m_columns.size();
    }
    virtual int         columnCount(const QModelIndex&) const override 
    { 
        return columnCount();
    }
    
    
    virtual QVariant    data(const QModelIndex&, int) const override;
    
    Qt::ItemFlags       flags(const QModelIndex&) const override;
    QVariant            headerData(int, Qt::Orientation, int) const override;
    
    void                moveRowUp(int);
    void                moveRowDown(int);
    
    
    bool                readOnly() const { return m_readOnly; }
    
    void                removeRow(int);
    
    int                 rowCount() const;
    
    virtual int         rowCount(const QModelIndex&) const override
    {
        return rowCount();
    }

    bool                setData(const QModelIndex&, const QVariant&, int) override;
    void                setReadOnly(bool);
     
    void                sort(int, Qt::SortOrder) override;
    
    class Adder;
    class Column;
    class Delegator;
    
    const Vector<Column*>&  columns() const { return m_columns; }
    const Vector<T>&        rows() const { return m_rows; }

protected:
    struct ColumnWriter;

    Vector<Column*>     m_columns;
    Vector<T>           m_rows;
    size_t              m_adders;
    bool                m_readOnly;
    bool                m_canAdd;
    bool                m_dragReorder;  // pending...
    
    void                allChanged();
    
    
    ColumnWriter        col(Column* c);
    ColumnWriter        col(const String&, String(T::*), bool cs=false);
    template <typename E>
    ColumnWriter        col(const String&, EnumImpl<E> (T::*));
    template <typename K, typename V>
    ColumnWriter        col(EnumMap<K,EnumImpl<V>> (T::*), K);
    template <typename K, typename V>
    ColumnWriter        col(const String&, EnumMap<K,EnumImpl<V>> (T::*), K);

    //! Creates a custom read-only column using lambdas
    template <typename Getter>
    ColumnWriter        customRO(const String&, Getter);
    
    //! Creates a custom read-only column using lambdas
    template <typename Getter, typename Less>
    ColumnWriter        customRO(const String&, Getter, Less);
    
    //! Creates a custom read-write column using lambdas
    template <typename Getter, typename Setter>
    ColumnWriter        customRW(const String&, Getter, Setter);
    
    //! Creates a custom read-write column using lambdas
    template <typename Getter, typename Setter, typename Less>
    ColumnWriter        customRW(const String&, Getter, Setter, Less);

    //template <String (T::*)>
    //void                add(const String&, const QVariant&v=QVariant, Adder* a=nullptr)
    

    const Column*       _column(int c) const;
    const Column*       _column(const QModelIndex& idx) const;

    T*                  _row(int r);
    const T*            _row(int r) const;

    //! Returns the pointer into the vector for the given index (or NULL if not good)
    T*                  _row(const QModelIndex& idx);
    //! Returns the pointer into the vector for the given index (or NULL if not good)
    const T*            _row(const QModelIndex& idx) const;
    
};
